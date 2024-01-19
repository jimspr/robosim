#include "stdafx.h"
#include <errno.h>
#include "node.h"
#include "argstack.h"
#include "package.h"
#include "ik.h"

using namespace std;

extern void main_win_mark_in_use();

void node_list_array_t::clear_flags()
{
	size_t n = _data.size();
	for (size_t i = 0; i < n; i++)
		_data[i]->clear_gc_flag();
	_garbage_collected = false;
}

int node_list_array_t::garbage_collect()
{
	int cnt=0;
	_garbage_collected = true;
	size_t n = _data.size();

//	MessageBeep(MB_ICONASTERISK);
	/* Traverse all roots and mark as in use. */
	g_frame_stack.mark_in_use();
	g_bind_stack.mark_in_use();
	dot_node->mark_in_use();
	nil->mark_in_use();
	pTrue->mark_in_use();
	current_package->mark_in_use();
	main_win_mark_in_use();
	for (auto& ik : invkin)
		ik->mark_in_use();

	for (size_t i = 0; i < n; i++)
		cnt += _data[i]->delete_unused();
	return cnt;
}

void node_list_array_t::status(ostream &ostr)
{
	size_t n = _data.size();
	for (size_t i = 0; i < n; i++)
		_data[i]->status(ostr);
}

void node_list_array_t::add(node_list_base_t* node)
{
	_data.push_back(node);
}


/************************************************
******************          *********************
****************** symbol_t *********************
******************          *********************
************************************************/

void symbol_t::princ(ostream &ostr) const
{
	if (_print_name[0] != '|')
		print(ostr);
	size_t n = _print_name.size();
	for (size_t i = 1; i < n - 1; i++)
		ostr << _print_name[i];
}

node_t *symbol_t::eval()
{
	if (!_value)
		throw_eval_exception(this,UNBOUND_VARIABLE);
	return _value;
}

void symbol_t::mark_in_use()
{
	if (is_in_use())
		return;
	node_t::mark_in_use();
	if (_value)
		_value->mark_in_use();
	if (_form)
		_form->mark_in_use();
}

void symbol_t::verify_can_set()
{
	if (is_constant_value())
		throw_eval_exception(REDEF_CONSTANT);
	if (is_constant_form())
		throw_eval_exception(REDEF_FORM);
}

void symbol_t::set_value(node_t *v)
{
	verify_can_set();
	_value = v;
}

void symbol_t::set_constant_value(node_t *v)
{
	verify_can_set();
	_value = v;
	set_symbol_flag(SYMBOL_CONSTANT_VALUE);
}

void symbol_t::set_form(form_t *f)
{
	verify_can_set();
	_form = f;
}

void symbol_t::set_constant_form(form_t *f)
{
	_form = f;
	set_symbol_flag(SYMBOL_CONSTANT_FORM);
}

void symbol_t::declare_special_variable()
{
	verify_can_set();
	set_symbol_flag(SYMBOL_SPECIAL_VALUE);
}

void symbol_t::set_special_variable(node_t *v)
{
	declare_special_variable();
	_value = v;
}

form_t *symbol_t::check_form() const
{
	if (!_form)
		throw_eval_exception(UNBOUND_FUNCTION);
	return _form;
}

/************************************************
******************        ***********************
****************** number_node_t ***********************
******************        ***********************
************************************************/

void number_node_t::print(ostream &ostr) const
{
	if (get_sub_type() == TYPE_FLOAT)
		ostr << fval;
	else
		ostr << lval;
}

number_node_t number_node_t::operator+(const number_node_t &n) const
{
	if (!is_a_number() || !n.is_a_number())
		throw_eval_exception(BAD_ARG_TYPE);
	if (is_a(TYPE_FLOAT) || n.is_a(TYPE_FLOAT))
	{
		return number_node_t((float)(*this) + (float)n);
	}
	else
		return number_node_t(lval+n.lval);
}

number_node_t number_node_t::operator-(const number_node_t &n) const
{
	if (!is_a_number() || !n.is_a_number())
		throw_eval_exception(BAD_ARG_TYPE);
	if (is_a(TYPE_FLOAT) || n.is_a(TYPE_FLOAT))
	{
		return number_node_t((float)(*this) - (float)n);
	}
	else
		return number_node_t(lval-n.lval);
}

number_node_t number_node_t::operator*(const number_node_t &n) const
{
	if (!is_a_number() || !n.is_a_number())
		throw_eval_exception(BAD_ARG_TYPE);
	if (is_a(TYPE_FLOAT) || n.is_a(TYPE_FLOAT))
	{
		return number_node_t((float)(*this) * (float)n);
	}
	else
		return number_node_t(lval * n.lval);
}

number_node_t number_node_t::operator/(const number_node_t &n) const
{
	if (!is_a_number() || !n.is_a_number())
		throw_eval_exception(BAD_ARG_TYPE);
	if ( (float)n == 0.)
		throw_eval_exception(DIVIDE_BY_ZERO);
	if (is_a(TYPE_FLOAT) || n.is_a(TYPE_FLOAT))
		return number_node_t((float)(*this) / (float)n);
	else
		return number_node_t(lval / n.lval);
}

int number_node_t::operator<(const number_node_t &n) const
{
	return (is_a(TYPE_LONG) && n.is_a(TYPE_LONG)) ?
		(lval < n.lval) :
		( (float)(*this) < (float)n);
}

int number_node_t::operator>(const number_node_t &n) const
{
	return (is_a(TYPE_LONG) && n.is_a(TYPE_LONG)) ?
		(lval > n.lval) :
		( (float)(*this) > (float)n);
}

int number_node_t::operator<=(const number_node_t &n) const
{
	return (is_a(TYPE_LONG) && n.is_a(TYPE_LONG)) ?
		(lval <= n.lval) :
		( (float)(*this) <= (float)n);
}

int number_node_t::operator>=(const number_node_t &n) const
{
	return (is_a(TYPE_LONG) && n.is_a(TYPE_LONG)) ?
		(lval >= n.lval) :
		( (float)(*this) >= (float)n);
}

int number_node_t::operator==(const number_node_t &n) const
{
	return (is_a(TYPE_LONG) && n.is_a(TYPE_LONG)) ?
		(lval == n.lval) :
		( (float)(*this) == (float)n);
}

int number_node_t::operator<(long l) const
{
	return (is_a(TYPE_LONG)) ?
		(lval < l) :
		( (float)(*this) < (float)l);
}

int number_node_t::operator>(long l) const
{
	return (is_a(TYPE_LONG)) ?
		(lval > l) :
		( (float)(*this) > (float)l);
}

int number_node_t::operator<=(long l) const
{
	return (is_a(TYPE_LONG)) ?
		(lval <= l) :
		( (float)(*this) <= (float)l);
}

int number_node_t::operator>=(long l) const
{
	return (is_a(TYPE_LONG)) ?
		(lval >= l) :
		( (float)(*this) >= (float)l);
}

int number_node_t::operator==(long l) const
{
	return (is_a(TYPE_LONG)) ?
		(lval == l) :
		( (float)(*this) == (float)l);
}

/************************************************
******************        ***********************
****************** vector_t ***********************
******************        ***********************
************************************************/

vector_t::vector_t(std::vector<node_t*>&& v) : node_t(TYPE_VECTOR), _contents(std::move(v))
{
}

void vector_t::mark_in_use()
{
	if (is_in_use())
		return;
	node_t::mark_in_use();
	for (auto& item: _contents)
		item->mark_in_use();
}

void vector_t::print(ostream &ostr) const
{
	ostr << "#(";
	size_t li;
	for (li=0;li< _contents.size();li++)
	{
		if (li)
			ostr << ' ';
		_contents[li]->print(ostr);
	}
	ostr << ')';
}


/************************************************
******************        ***********************
****************** ARRAY  ***********************
******************        ***********************
************************************************/

#if 0
class ARRAY : public node_t
{
protected:
	node_t **pvec;
	long *dims;
	short rank;

public:
	ARRAY(short r,long *d);
	{
		short i;
		long n=1;
		dims = new long[r];
		for (i=0;i<r;i++)
		{
			dims[i] = d[i];
			n*= d[i];
		}
		pvec = new node_t*[n];
	}
	~ARRAY() { delete pvec; delete dims;}
//
	node_type_e get_type() const { return TYPE_ARRAY;}
	int is_a(node_type_e t) const { return t == TYPE_ARRAY; }
	void mark_in_use();
	void print(ostream &) const;
//
	long GetNumVecElements() const { return pvector->dim; }
	node_t *GetVecElement(long i) const { return pvector->pvec[i]; }
	node_t *&operator[](long i) { return pvector->pvec[i]; }
};
#endif

/************************************************
******************        ***********************
******************  cons_t  ***********************
******************        ***********************
************************************************/

cons_t* cons_t::make_list()
{
	return (cons_t*)nil;
}

cons_t* cons_t::append_cons(node_t* p)
{
	ASSERT(_right == nil);
	auto cons = new cons_t{ p, nil };
	_right = cons;
	return cons;
}

node_t *cons_t::eval()
{
	ASSERT(is_a(TYPE_CONS));
	static MSG keymsg;
	static int cnt=0;
	if (NodeListArray._garbage_collected)
		NodeListArray.clear_flags();
	if (! (cnt++%100))
	{
		if (PeekMessage(&keymsg,NULL,WM_KEYDOWN,WM_KEYDOWN,PM_NOREMOVE))
			if (AfxMessageBox("Do you want to stop?", MB_YESNO|MB_APPLMODAL|MB_ICONQUESTION)==IDYES)
				throw_interrupt_exception();
		AfxGetApp()->OnIdle(0); // this makes sure cleanup occurs during long processing
	}
	if (!Car()->is_a(TYPE_SYMBOL))
		throw_eval_exception(Car(),BAD_FUNCTION);
	try
	{
		return Car()->as<symbol_t>()->check_form()->eval(CdrCONS());
	}
	catch(eval_exception_t* e)
	{
		if (!e->_evalnode)
			e->_evalnode = this;
		throw;
	}
	return NULL;
}

void cons_t::mark_in_use()
{
	ASSERT(is_a(TYPE_CONS));
	cons_t *ths=this;
	while (ths->is_a(TYPE_CONS))
	{
		if (ths->is_in_use())
			return;
		ths->node_t::mark_in_use();
		ths->Car()->mark_in_use();
		ths = ths->CdrCONS();
	}
	if (!ths->is_in_use())
		((node_t*)ths)->mark_in_use();
}

void cons_t::print(ostream &ostr) const
{
	ASSERT(is_a(TYPE_CONS));
	const cons_t *ths = this;
	ostr << '(';
	while (ths->is_a(TYPE_CONS))
	{
		if (ths->Car())
			ths->Car()->print(ostr);
		else
			ostr << "L0000:0000";
		ths = ths->CdrCONS();
		if (!ths)
		{
			ostr << "R0000:0000";
			break;
		}
		if (ths->is_a(TYPE_CONS))
			ostr << ' ';
	}
	if ((const node_t *)ths != (const node_t *)nil)
	{
		ostr << " . ";
		((node_t*)ths)->print(ostr);
	}
	ostr << ')';
}

void cons_t::princ(ostream &ostr) const
{
	ASSERT(is_a(TYPE_CONS));
	const cons_t *ths = this;
	ostr << '(';
	while (ths->is_a(TYPE_CONS))
	{
		if (ths->Car())
			ths->Car()->princ(ostr);
		else
			ostr << "L0000:0000";
		ths = ths->CdrCONS();
		if (!ths)
		{
			ostr << "R0000:0000";
			break;
		}
		if (ths->is_a(TYPE_CONS))
			ostr << ' ';
	}
	if ((const node_t *)ths == (const node_t *)nil)
	{
		ostr << " . ";
		((node_t*)ths)->princ(ostr);
	}
	ostr << ')';
}

int cons_t::get_num_items() const
{
	const cons_t *t = this;
	int i = 0;
	while (t->is_a(TYPE_CONS))
	{
		i++;
		t = t->CdrCONS();
	}
	return i;
}

int cons_t::check_range_num_args(int l,int u) const
{
	int numargs = get_num_items();
	if (l>=0 && numargs<l)
		throw_eval_exception(TOO_FEW_ARGS);
	else if (u>=0 && numargs > u)
		throw_eval_exception(TOO_MANY_ARGS);
	return numargs;
}

void cons_t::check_num_args(int n) const
{
	int numargs = get_num_items();
	if (numargs == n)
		return;
	else if (numargs<n)
		throw_eval_exception(TOO_FEW_ARGS);
	else // numargs > n
		throw_eval_exception(TOO_MANY_ARGS);
}

void cons_t::check_min_num_args(int n) const
{
	if (get_num_items() >= n)
		return;
	throw_eval_exception(TOO_FEW_ARGS);
}

stream_node_t::stream_node_t(const char *s) : node_t(TYPE_STREAM), _name(s)
{
	_stream = make_unique<ofstream>(_name);
}

stream_node_t::~stream_node_t()
{
	close();
}

void stream_node_t::close()
{
	if (_stream)
		_stream->close();
	_stream.reset();
}

void stream_node_t::check_stream()
{
	if (!_stream)
		throw_eval_exception(INVALID_STREAM);
	if (!_stream->is_open())
		throw_eval_exception(INVALID_STREAM);
}

/************************************************
******************        ***********************
******************  bound_symbol_t  ***********************
******************        ***********************
************************************************/

void bound_symbol_t::swap_values()
{
	node_t *t = _value;
	_value = _symbol->get_value();
	_symbol->set_value(t);
}

void bound_symbol_t::mark_in_use()
{
	if (is_in_use())
		return;
	node_t::mark_in_use();
	if (_symbol)
		_symbol->mark_in_use();
	if (_value)
		_value->mark_in_use();
	if (_child)
		_child->mark_in_use();
}

void bound_symbol_t::print(ostream &ostr) const
{
	ostr << "#<bound_symbol_t: ";
	ostr << _symbol << ' ';
	ostr << _value << ' ';
	ostr << _child << " >";
}

HCURSOR hour_glass_t::hHourGlass = LoadCursor(NULL,IDC_WAIT);
int hour_glass_t::count = 0;

hour_glass_t::hour_glass_t()
{
	if (!count)
	{
		hCursor = SetCursor(hHourGlass);
		ShowCursor(TRUE);
	}
	SetCursor(hHourGlass);
	count++;
}   

hour_glass_t::~hour_glass_t()
{
	count--;
	if (!count)
	{
		ShowCursor(FALSE);
		SetCursor(hCursor);
		hCursor = NULL;
	}
}

