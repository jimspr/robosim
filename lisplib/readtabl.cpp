#include "pch.h"
#include "parsen.h"
#include "readtabl.h"
#include "node.h"
#include "package.h"
#include "lisp_engine.h"

using namespace std;

int read_table_t::_paren_level = 0;

#define checkeof(x) if (x.eof()) throw read_exception_t(UNEXPECTED_EOF)

void process_type(int type[256], const char* s, int cat)
{
	auto len = strlen(s);
	for (size_t i = 0; i < len; i++)
	{
		assert(type[s[i]] == 0);
		type[s[i]] = cat;
	}
}

read_table_t::read_table_t() : _line_cnt(1)
{
	static const char* WS = " \t\r\n";
	static const char* C1 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static const char* C2 = "0123456789!$%&*+-/<=>?@[]^_{}~";
	static const char* SE = "\\";
	static const char* ME = "|";
	static const char* NTM = "#";
	static const char* TM = "\"'(),;`";
	static const char* PM = ":";
	static const char* CD = ".";

	process_type(_type, WS, WHITESPACE);
	process_type(_type, C1, CONSTITUENT);
	process_type(_type, C2, CONSTITUENT);
	process_type(_type, SE, SINGLE_ESCAPE);
	process_type(_type, ME, MULT_ESCAPE);
	process_type(_type, NTM, NONTERM_MACRO);
	process_type(_type, PM, PACKAGE_MARKER);
	process_type(_type, CD, CHAR_DOT);
	process_type(_type, TM, TERM_MACRO);

	parse_number_t::initialize();
}

node_t *read_table_t::read(istream &istr,int btop)
{
	_current = getnextchar(istr);
	_buffer.clear();
	while (!istr.eof())
	{
		switch (_type[_current])
		{
			case ILLEGAL:
				consume(istr);
				throw read_exception_t(ILLEGAL_CHAR);
				break;
			case NONTERM_MACRO:
				return parse_ntmacro(istr);
				break;
			case WHITESPACE:
				break;
			case TERM_MACRO:
				if (_current ==';')
					consume(istr);
				else
					return parse_tmacro(istr);
				break;
			case CONSTITUENT:
			case SINGLE_ESCAPE:
			case MULT_ESCAPE:
			case PACKAGE_MARKER:
				return parse_constituent(istr);
				break;
			case CHAR_DOT:
				return parse_dot(istr);
				break;
			default:
				consume(istr);
				cout << (int)_current << " " << _type[_current] << "\n";
				throw read_exception_t(READTABLE_CORRUPT);
				break;
		}
		_current = getnextchar(istr);
	}
	if (!btop)
		throw read_exception_t(UNEXPECTED_EOF);
	return (node_t *)NULL;
}

node_t *read_table_t::parse_tmacro(istream &istr)
{
	switch(_current)
	{
		case '(':
			return parse_lparen(istr);
		case ')':
			return parse_rparen(istr);
		case '\'':
			return parse_quote(istr);
		case ';':
			return parse_comment(istr);
		case '\"':
			return parse_string(istr);
		default:
			consume(istr);
			throw read_exception_t(INVALID_TMACRO);
			break;
	}
	throw read_exception_t(FATAL_ERROR);
	return nil; // will never get here
}

node_t *read_table_t::parse_ntmacro(istream &istr)
{
	_current = getnextchar(istr);
	switch(_current)
	{
		case '(':
			return parse_vector(istr);
		case '\'':
			return parse_function(istr);
		default:
			consume(istr);
			throw read_exception_t(ILLEGAL_NT_MACRO);
	}
	throw read_exception_t(FATAL_ERROR);
	return nil; // will never get here
}

node_t *read_table_t::parse_vector(istream &istr)
{
	std::vector<node_t*> vec;
	node_t *t;
	_paren_level++;
	try
	{
		while (t = read(istr,0))
		{
			if (is_dot(t))
			{
				consume(istr);
				throw read_exception_t(INVALID_DOTTED_PAIR);
			}
			vec.push_back(t);
		}
	}
	catch (const base_exception_t&)
	{
		_paren_level--;
		throw;
	}
	return new vector_t(std::move(vec));
}

node_t *read_table_t::parse_constituent(istream &istr)
{
	_me = 0;
	_in_escape = false;
	while (!istr.eof())
	{
		switch(_type[_current])
		{
			case PACKAGE_MARKER:
// add support for packages here in place of ignoring.
// a symbol with a leading colon will be treated as a constant evaluating to itself
			case CHAR_DOT:
			case CONSTITUENT:
				if (_me)
					_buffer.push_back(_current);
				else
					_buffer.push_back((char)toupper(_current));
				break;
			case WHITESPACE:
			case TERM_MACRO:
			case NONTERM_MACRO:
				if (_me)
					_buffer.push_back((char)_current);
				else
				{
					putback(istr, (char)_current);
					return makecell(_buffer.c_str());
				}
				break;
			case SINGLE_ESCAPE:
				if (_me)
					_buffer.push_back((char)_current);
				else
				{
					_current = getnextchar(istr);
					checkeof(istr);
					_buffer.push_back((char)_current);
					_in_escape = true;
				}
				break;
			case MULT_ESCAPE:
				_me = (_me) ? (char)0 : (char)_current;
				_in_escape = true;
				break;
			case ILLEGAL:
				consume(istr);
				throw read_exception_t(ILLEGAL_CHAR);
				break;
			default:
				cout << (int)_current << " " << _type[_current] << "\n";
				consume(istr);
				throw read_exception_t(READTABLE_CORRUPT);
				break;
		}
		if (_buffer.size() >= (MAX_CONSTITUENT_LENGTH-1))
		{
			consume(istr);
			throw read_exception_t(CONSTITUENT_TOO_LONG);
		}
		_current = getnextchar(istr);
	}
	throw read_exception_t(UNEXPECTED_EOF);
	return nil; // will never get here
}

node_t *read_table_t::parse_dot(istream &istr)
{
	_buffer.push_back('.');
	_current = getnextchar(istr);
	while (!istr.eof())
	{
		switch(_type[_current])
		{
			case CHAR_DOT:
				_buffer.push_back('.');
				break;
			case SINGLE_ESCAPE:
			case MULT_ESCAPE:
			case CONSTITUENT:
				return parse_constituent(istr);
				break;
			case WHITESPACE:
			case TERM_MACRO:
			case NONTERM_MACRO:
				if (_buffer.size() == 1)
				{
					putback(istr, (char)_current);
					return dot_node;
				}
				consume(istr);
				throw read_exception_t(INVALID_SYMBOL);
				break;
			case PACKAGE_MARKER:
				consume(istr);
				throw read_exception_t(INVALID_PACKAGE_NAME);
				break;
			case ILLEGAL:
				consume(istr);
				throw read_exception_t(ILLEGAL_CHAR);
				break;
			default:
				cout << (int)_current << " " << _type[_current] << "\n";
				consume(istr);
				throw read_exception_t(READTABLE_CORRUPT);
				break;
		}
		if (_buffer.size() >= (MAX_CONSTITUENT_LENGTH-1))
		{
			consume(istr);
			throw read_exception_t(CONSTITUENT_TOO_LONG);
		}
		_current = getnextchar(istr);
	}
	throw read_exception_t(UNEXPECTED_EOF);
	return nil; // will never get here
}

node_t *read_table_t::parse_lparen(istream &istr)
{
	_paren_level++;
	node_t *t;
	cons_t *top,*trr;
	trr = top = (cons_t *)NULL;
	t = read(istr,0);
	if (!t)
		return nil;
	if (is_dot(t))
	{
		consume(istr);
		throw read_exception_t(INVALID_DOTTED_PAIR);
	}
	trr = top = new cons_t(t,nil);
	while (trr->is_a(TYPE_CONS))
	{
		t = read(istr,0);
		if (!t)
			return top;
		if (is_dot(t))
		{
			t = read(istr,0);
			if (!t)
			{
				consume(istr);
				throw read_exception_t(INVALID_DOTTED_PAIR);
			}
			if (is_dot(t))
			{
				consume(istr);
				throw read_exception_t(INVALID_DOTTED_PAIR);
			}
			trr->set_cdr(t);
			t = read(istr,0);
			if (t)
			{
				consume(istr);
				throw read_exception_t(INVALID_DOTTED_PAIR);
			}
			return top;
		}
		trr->set_cdr(new cons_t(t,nil));
		trr = trr->CdrCONS();
	}
	return top;
}


node_t *read_table_t::parse_rparen(istream &istr)
{
	if (!_paren_level)
	{
		consume(istr);
		throw read_exception_t(UNEXPECTED_RPAREN);
	}
	_paren_level--;
	return (node_t *)NULL;
}

node_t *read_table_t::parse_quote(istream &istr)
{
	node_t *t;
	t = read(istr,0);
	if (!t)
	{
		consume(istr);
		throw read_exception_t(UNEXPECTED_RPAREN);
	}
	if (is_dot(t))
	{
		consume(istr);
		throw read_exception_t(INVALID_DOTTED_PAIR);
	}
	return cons_t::make_list(lisp_engine._package.get_symbol("QUOTE"), t);
}

node_t *read_table_t::parse_function(istream &istr)
{
	node_t *t;
	t = read(istr,0);
	if (!t)
	{
		consume(istr);
		throw read_exception_t(UNEXPECTED_RPAREN);
	}
	if (is_dot(t))
	{
		consume(istr);
		throw read_exception_t(INVALID_DOTTED_PAIR);
	}
	return cons_t::make_list(lisp_engine._package.get_symbol("FUNCTION"), t);
}

node_t *read_table_t::parse_comment(istream &istr)
{
	consume(istr);
	return read(istr,0);
}

node_t *read_table_t::parse_string(istream &istr)
{
	static string str;
	str.clear();
	_current = getnextchar(istr);
	while (!istr.eof())
	{
		if ((char)_current == '\"')
		{
			return new string_node_t(str.c_str());
		}
		switch(_type[_current])
		{
			case SINGLE_ESCAPE:
				_current = getnextchar(istr);
				checkeof(istr);
				str.push_back((char)_current);
				break;
			default:
				str.push_back((char)_current);
				break;
		}
		_current = getnextchar(istr);
	}
	throw read_exception_t(UNEXPECTED_EOF);
	return nil; // will never get here
}

void read_table_t::consume(istream &istr)
{
	while (!istr.eof() && ((char)_current != '\n'))
	{
		_current = getnextchar(istr);
	}
	if (!istr.eof())
		_line_cnt--; //if not eof then set line count back one
}

node_t *read_table_t::makecell(const char *buf)
{
	parse_number_t oNum(buf);
	oNum.parse();
	if (_in_escape || !oNum._node)
		return lisp_engine._package.get_symbol(buf);
	else
		return oNum._node;
}
