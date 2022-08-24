#pragma once
#include <vector>
#include <string>

#include "rlerror.h"
#include "rexcept.h"

enum node_type_e
{
	//TYPE_NODE = 0,
	TYPE_CONS = 2,
	TYPE_SYMBOL = 3,
	TYPE_STRING = 4,
	//TYPE_NUM = 6,
	TYPE_DOT = 7,
	TYPE_LONG = 8,
	TYPE_FLOAT = 9,
	TYPE_VECTOR = 10,
	//TYPE_ARRAY = 11,
	TYPE_PACKAGE = 13,
	TYPE_FUNCTION = 14,
	TYPE_SPECIALFORM = 15,
	TYPE_MACRO = 16,
	TYPE_STACK = 17,
	TYPE_BIND = 18,
	TYPE_SIMOB = 20,
	TYPE_MAT44 = 21,
	TYPE_STREAM = 22
};

enum gc_flag_e
{
	FLAG_NODE_NONE = 0,
	FLAG_NODE_IN_USE = 1,
	FLAG_NODE_JUST_CREATED = 2
};

#define DECLARE_NODE(T, nodes_in_block) \
public:\
	static node_list_t<T, nodes_in_block> plist;\
	void *operator new(size_t)\
		{return plist.get_free_node();}\
	void *operator new(size_t nSize, const char * lpszFileName, int nLine)\
		{return plist.get_free_node();}

class form_t;
class node_t;
class number_node_t;
class cons_t;

typedef node_t *(*PFORMCALL)(cons_t *);
typedef node_t *(*PFUNCCALL)(int,node_t **);
typedef node_t *(*PNUMFUNCCALL)(int,number_node_t **);

struct node_list_base_t
{
	virtual void clear_gc_flag() = 0;
	virtual int delete_unused() = 0;
	virtual void status(std::ostream& ostr) = 0;
};

/* Contains an array of all node lists. */
class node_list_array_t
{
	std::vector<node_list_base_t*> _data;
public:
	bool _garbage_collected = false;
	void clear_flags();
	int garbage_collect();
	void status(std::ostream &ostr);
	void add(node_list_base_t* node);
};

extern node_list_array_t NodeListArray;

/* Tracks a block of nodes. */
template <typename T, size_t nodes_in_block>
struct node_block_t
{
	char _data[sizeof(T) * nodes_in_block] = {};
	/* map tracks which elements in block are in use. */
	char _map[nodes_in_block] = {};
	T* node_of_block(size_t index)
	{
		return (T*)(_data + index * sizeof(T));
	}
};

/* node_list_t tracks all blocks for a given node type. */
template <typename T, size_t nodes_in_block>
class node_list_t final : public node_list_base_t
{
protected:
	constexpr static int node_size = sizeof(T); // size of node
	using block_t = node_block_t<T, nodes_in_block>;
	std::vector<std::unique_ptr<block_t>> _blocks; // Each block is nodes_in_block*size chars.
	size_t _current_node = 0;
	size_t _current_block = 0;
	size_t _nodes_total = 0;
	size_t _nodes_alloc = 0;
	const char *_name;

public:
	node_list_t(const char* n) : _name(n)
	{
		NodeListArray.add(this);
		add_entry();
	}

	~node_list_t()
	{
		for (auto& block : _blocks)
		{
			for (int j = 0; j < nodes_in_block; j++)
			{
				if (block->_map[j])
				{
					block->node_of_block(j)->~T();
					block->_map[j] = (char)0;
					_nodes_alloc--;
				}
			}
		}
	}

	void add_entry()
	{
		_blocks.push_back(std::make_unique<block_t>());
		/* Set current block to last black and current node to first. */
		_current_node = 0;
		_current_block = _blocks.size() - 1;
		_nodes_total += nodes_in_block;
	}

	T* get_node_in_current_block()
	{
		auto& cblock = _blocks[_current_block];
		auto cmap = cblock->_map;
		while (_current_node != nodes_in_block)
		{
			T* ptr;
			if (cmap[_current_node] == (char)0)
			{
				/* Found a free node in the current block. */
				cmap[_current_node] = (char)1;
				_nodes_alloc++;
				ptr = cblock->node_of_block(_current_node);
				_current_node++;
				memset(ptr, 0, node_size);
				((node_t*)ptr)->set_gc_flag(FLAG_NODE_JUST_CREATED);
				return ptr;
			}
			_current_node++;
		}
		/* No free node found in the current block (after initial current_node). */
		return nullptr;
	}

	/*
		1. Check From Current Position to end for free node
		2. Check From beginning to current position for free node
		3. Garbage collect
		4. if free nodes is less than nodes_in_block then make new block
			a. set current position in new block
			b. return node
		5. if plenty of free nodes, reset current position to beginning
		6. goto 1
	*/

	T* get_free_node()
	{
		/* See if free node in current block. */
		auto ptr = get_node_in_current_block();
		if (ptr)
			return ptr;
		/* None left in the current block(starting at current_node). */
		size_t n = _blocks.size();
		for (size_t i = 0; i < n; i++)
		{
			/* Search next block from beginning. */
			_current_block++;
			if (_current_block == n) /* If last block, start from block zero. */
				_current_block = 0;
			_current_node = 0;
			if ((ptr = get_node_in_current_block()) != nullptr)
				return ptr;
		}
		/* Nothing available in any block. */
		NodeListArray.garbage_collect();
		if (_nodes_total < nodes_in_block + _nodes_alloc)
		{
			add_entry();
			return get_node_in_current_block();
		}
		_current_block = _current_node = 0;
		/* Try again after GC. */
		return get_free_node();
	}

	void clear_gc_flag()
	{
		size_t n = _blocks.size();
		for (size_t i = 0; i < n; i++)
		{
			auto& block = _blocks[i];
			for (size_t j = 0; j < nodes_in_block; j++)
			{
				auto ptr = block->node_of_block(j);
				ptr->clear_gc_flags();
			}
		}
	}


	//delete_unused is called by garbage collection routines
	// current_block and current_node are reset.
	int delete_unused()
	{
		int cnt = 0;
		for (auto& block : _blocks)
		{
			auto& map = block->_map;
			for (int j = 0; j < nodes_in_block; j++)
			{
				auto ptr = block->node_of_block(j);
				if (map[j])
				{
					if (!ptr->is_in_use() && !ptr->is_just_created())
					{
						ptr->~T();
						map[j] = (char)0;
						cnt++;
						_nodes_alloc--;
					}
				}
			}
		}
		_current_node = _current_block = 0;
		return cnt;
	}

	void status(std::ostream& ostr)
	{
		ostr << _name;
		ostr << "   BS=" << nodes_in_block;
		ostr << "   TS=" << _nodes_total * node_size;
		ostr << "   TN=" << _nodes_total;
		ostr << "   NA=" << _nodes_alloc;
		ostr << "   LB=" << _current_block;
		ostr << "   LN=" << _current_node;
		ostr << "\n";
	}
};

class node_t
{
protected:
	node_type_e _type;
	gc_flag_e _flags{ FLAG_NODE_JUST_CREATED };
public:
	node_t(node_type_e t) : _type(t)
	{}
	virtual ~node_t() {}
//
	virtual node_t *eval() { return this;}
	virtual void mark_in_use() {set_gc_flag(FLAG_NODE_IN_USE);}
	virtual void print(std::ostream &) const = 0;
	virtual void princ(std::ostream &ostr) const {print(ostr);}
	virtual node_t *car() const {throw_eval_exception(BAD_ARG_TYPE);return NULL;}
	virtual node_t *cdr() const {throw_eval_exception(BAD_ARG_TYPE);return NULL;}

	node_type_e get_type() const{ return _type;}
	int is_a(node_type_e t) const {return t == _type;}
	int is_a_number() const { return (_type==TYPE_FLOAT || _type== TYPE_LONG); }
	void check_number() const {if (!is_a_number()) throw_eval_exception(this,BAD_ARG_TYPE);}

	void check_arg_type(node_type_e type) const {if (!is_a(type)) throw_eval_exception(this,BAD_ARG_TYPE);}
	template <typename T>
	T* as()
	{
		ASSERT(T::verify(this));
		if (!T::verify(this))
			throw_eval_exception(this, BAD_ARG_TYPE);
		return static_cast<T*>(this);
	}

	void set_gc_flag(gc_flag_e tem) { _flags = static_cast<gc_flag_e>(_flags | tem); }
	void clear_gc_flag(gc_flag_e tem) { _flags = static_cast<gc_flag_e>(_flags & ~tem); }
	void clear_gc_flags() { _flags = FLAG_NODE_NONE; }
	bool is_in_use() const { return (_flags & FLAG_NODE_IN_USE); }
	bool is_just_created() const {return (_flags & FLAG_NODE_JUST_CREATED); }
};

enum symbol_flags_e
{
	SYMBOL_FLAGS_NONE = 0,
	SYMBOL_SPECIAL_VALUE = 1, // defvar (global variable)
	//SYMBOL_SPECIAL_FORM  = 2,
	SYMBOL_CONSTANT_VALUE = 4,
	SYMBOL_CONSTANT_FORM = 8
};

class symbol_t : public node_t
{
protected:
	std::string _print_name;
	node_t* _prop_list = nullptr;
	node_t* _value = nullptr;
	form_t* _form = nullptr;
	symbol_flags_e _symbol_flags = SYMBOL_FLAGS_NONE;
public:
	static bool verify(node_t* p) { return p->get_type() == TYPE_SYMBOL; }
	symbol_t();
	symbol_t(const char* n);
	symbol_t(const char* n, node_t* v);
	symbol_t(const char* n, form_t* f);

	node_t* eval();
	void mark_in_use();
	void print(std::ostream& ostr) const { ostr << _print_name; }
	void princ(std::ostream& ostr) const;

	const char* get_name() const { return _print_name.c_str(); }

	void set_value(node_t* v);
	void set_constant_value(node_t* v);
	node_t* get_value() const { return _value; }

	void set_form(form_t* f);
	void set_constant_form(form_t* f);
	form_t* get_form() const { return _form; }
	form_t* check_form() const;

	void declare_special_variable();
	void set_special_variable(node_t* v);
	void set_symbol_flag(symbol_flags_e f) { _symbol_flags = static_cast<symbol_flags_e>(_symbol_flags | f); }

	bool is_special_value() const { return _symbol_flags & SYMBOL_SPECIAL_VALUE; }
	bool is_constant_value() const { return _symbol_flags & SYMBOL_CONSTANT_VALUE; }
	bool is_constant_form() const { return _symbol_flags & SYMBOL_CONSTANT_FORM; }

	//void add_prop(node_t* n, node_t* v);
	//void remove_prop(node_t* n);
	//node_t* find_prop(node_t* n);

	DECLARE_NODE(symbol_t, 1024)
};

class nil_t : public symbol_t
{
public:
	nil_t() : symbol_t("NIL") {set_constant_value(this);}

	node_t *car() const { return (node_t *)this;}
	node_t *cdr() const { return (node_t *)this;}
};

class dot_t : public symbol_t
{
public:
	dot_t() : symbol_t("DOT") {_type = TYPE_DOT;}
};

class number_node_t : public node_t
{
private:
public:
	union
	{
		float fval;
		long lval;
	};
	static bool verify(node_t* p) { return p->is_a_number(); }
	number_node_t(float f) : node_t(TYPE_FLOAT), fval(f) {}
	number_node_t(long li) : node_t(TYPE_LONG), lval(li) {}

	void print(std::ostream &) const;

	node_type_e get_sub_type() const { return _type;}
	void set_sub_type(node_type_e t) { _type = t;}

	operator float() const { return (get_sub_type() == TYPE_FLOAT) ? fval : (float)lval; }
	operator long() const { return (get_sub_type() == TYPE_FLOAT) ? (long)fval : lval; }

	number_node_t operator+(const number_node_t &n) const;
	number_node_t operator-(const number_node_t &n) const;
	number_node_t operator*(const number_node_t &n) const;
	number_node_t operator/(const number_node_t &n) const;

	void operator+=(const number_node_t &n)
	{
		if (is_a(TYPE_FLOAT))
			fval += (float)n;
		else if (n.is_a(TYPE_FLOAT))
		{
			fval = (float)lval + n.fval;
			set_sub_type(TYPE_FLOAT);
		}
		else
			lval += n.lval;
	}

	void operator-=(const number_node_t &n)
	{
		if (is_a(TYPE_FLOAT))
			fval -= (float)n;
		else if (n.is_a(TYPE_FLOAT))
		{
			fval = (float)lval - n.fval;
			set_sub_type(TYPE_FLOAT);
		}
		else
			lval -= n.lval;
	}

	void operator*=(const number_node_t &n)
	{
		if (is_a(TYPE_FLOAT))
			fval *= (float)n;
		else if (n.is_a(TYPE_FLOAT))
		{
			fval = (float)lval * n.fval;
			set_sub_type(TYPE_FLOAT);
		}
		else
			lval *= n.lval;
	}

	void operator/=(const number_node_t &n)
	{
		if ( (float)n == 0.)
			throw_eval_exception(DIVIDE_BY_ZERO);
		if (is_a(TYPE_FLOAT))
			fval /= (float)n;
		else if (n.is_a(TYPE_FLOAT))
		{
			fval = (float)lval / n.fval;
			set_sub_type(TYPE_FLOAT);
		}
		else
			lval /= n.lval;
	}

	int operator<(const number_node_t &n) const;
	int operator>(const number_node_t &n) const;
	int operator<=(const number_node_t &n) const;
	int operator>=(const number_node_t &n) const;
	int operator==(const number_node_t &n) const;
	int operator<(long l) const;
	int operator>(long l) const;
	int operator<=(long l) const;
	int operator>=(long l) const;
	int operator==(long l) const;
//
	DECLARE_NODE(number_node_t, 1024)
};

class vector_t : public node_t
{
protected:
	std::vector<node_t*> _contents;

public:
	vector_t(std::vector<node_t*>&& v);

	void mark_in_use();
	void print(std::ostream &) const;

	size_t size() const { return _contents.size(); }
	node_t *&operator[](long i) { return _contents[i]; }

	DECLARE_NODE(vector_t, 128)
};

class cons_t : public node_t
{
protected:
	node_t *_left = nullptr;
	node_t *_right = nullptr;
public:
	cons_t(node_t *l,node_t *r) : node_t(TYPE_CONS), _left(l), _right(r)
	{
	}
	cons_t() : node_t(TYPE_CONS)
	{
	}

	node_t *eval();
	void mark_in_use();
	void print(std::ostream &) const;
	void princ(std::ostream &) const;

	int get_num_items() const;
	void check_num_args(int n) const;
	void check_min_num_args(int n) const;
	int check_range_num_args(int l,int u) const;

#define CC(fn,field) \
	cons_t * fn##CONS() const{ return (cons_t *)(field);}\
	node_t * fn() const { return field;}

	CC(Car,_left)
	CC(Cdr,_right)

	CC(Caar,CarCONS()->Car())
	CC(Cadr,CdrCONS()->Car())
	CC(Cdar,CarCONS()->Cdr())
	CC(Cddr,CdrCONS()->Cdr())

	CC(Cdddr,CdrCONS()->CdrCONS()->Cdr())
	CC(Caddr,CdrCONS()->CdrCONS()->Car())

	node_t *car() const { return Car();}
	node_t *cdr() const { return Cdr();}

	void set_car(node_t *p) { _left = p; }
	void set_cdr(node_t *p) { _right = p; }

	DECLARE_NODE(cons_t, 1024)
};

class string_node_t : public node_t
{
protected:
	std::string _contents;
public:
	static bool verify(node_t* p) { return p->get_type() == TYPE_STRING; }
	string_node_t::string_node_t(const char* s) : node_t(TYPE_STRING), _contents(s)
	{
	}

	void print(std::ostream &ostr) const { ostr << '"' << _contents << '"';}
	void princ(std::ostream &ostr) const { ostr << _contents;}

	const char *data() const { return _contents.c_str();}
	const std::string& str() const { return _contents; }

	DECLARE_NODE(string_node_t, 512)
};

class bound_symbol_t : public node_t
{
public:
	symbol_t* _symbol = nullptr;
	node_t* _value = nullptr;
	bound_symbol_t* _child = nullptr;

	bound_symbol_t(symbol_t* ps, node_t* v, bound_symbol_t* c) : node_t(TYPE_BIND), _symbol(ps), _value(v), _child(c)
	{
	}

	void mark_in_use();
	void print(std::ostream&) const;

	void swap_values();

	DECLARE_NODE(bound_symbol_t, 1024)
};

class stream_node_t : public node_t
{
protected:
	std::string _name;
	std::unique_ptr<std::ofstream> _stream;
public:
	static bool verify(node_t* p) { return p->get_type() == TYPE_STREAM; }
	stream_node_t(const char *s);
	~stream_node_t();

	void print(std::ostream &ostr) const { ostr << "#<stream_node_t: " << (std::uintptr_t)this << ">";}

	std::ofstream *get_stream() const {return _stream.get();}
	void close();
	void check_stream();

	DECLARE_NODE(stream_node_t, 8)
};

/*
				form_t
				 / |
				/  |
			   /   |
			  /    |
			 /     |
			/      |
	special_form_t function
					/\
				  /    \
		usrfunction_t     sysfunction_t
*/


class form_t : public node_t
{
public:
	form_t(node_type_e type, const char* name);
	void print(std::ostream& ostr) const { ostr << "#<SUBR: " << (uintptr_t)this << ">"; }
	virtual node_t* eval(cons_t* ths) = 0;
protected:
	std::string _form_name;
};

class special_form_t : public form_t
{
private:
	PFORMCALL pfunc;
public:
	special_form_t(const char* name, PFORMCALL pf);
	node_t* eval(cons_t* ths) override;
	DECLARE_NODE(special_form_t, 32)
};

class macro_t : public special_form_t
{
protected:
public:
	macro_t(const char* name, PFORMCALL pf) : special_form_t(name, pf)
	{
		_type = TYPE_MACRO;
	}
	DECLARE_NODE(macro_t, 32)
};

class function : public form_t
{
	int _minargs;
	int _maxargs;
public:
	static bool verify(node_t* p) { return p->get_type() == TYPE_FUNCTION; }
	function(const char* name, int min, int max);
	node_t* eval(cons_t* ths) override;
	node_t* evalnoargs(cons_t* ths); // this function has special uses
	virtual node_t* eval(int numargs, node_t** base) = 0;
	void check_args(int n);
};

class usrfunction_t : public function
{
protected:
	cons_t* _bindings;
	cons_t* _body;
	bound_symbol_t* _env;
public:
	usrfunction_t(const char* name, int min, int max, cons_t* varlist, cons_t* b, bound_symbol_t* e);
	node_t* eval(int numargs, node_t** base) override;
	void mark_in_use(void);
	DECLARE_NODE(usrfunction_t, 32)
};

class sysfunction_t : public function
{
protected:
public:
	PFUNCCALL _pfunc;
	sysfunction_t(const char* name, PFUNCCALL pF, int min, int max);
	node_t* eval(int numargs, node_t** base) override;
	DECLARE_NODE(sysfunction_t, 256)
};

class hour_glass_t
{
private:
	HCURSOR hCursor{ NULL };
	static HCURSOR hHourGlass;
	static int count;
public:
	hour_glass_t();
	~hour_glass_t();
};

extern nil_t* nil;
extern symbol_t* pTrue;
extern dot_t* dot_node;

extern symbol_t* pPLUS1;
extern symbol_t* pPLUS2;
extern symbol_t* pPLUS3;
extern symbol_t* pSTAR1;
extern symbol_t* pSTAR2;
extern symbol_t* pSTAR3;
extern symbol_t* pDIV1;
extern symbol_t* pDIV2;
extern symbol_t* pDIV3;
extern symbol_t* pMINUS;
