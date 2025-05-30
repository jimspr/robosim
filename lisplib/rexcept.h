#pragma once
#ifndef __REXCEPT_H
#define __REXCEPT_H

class node_t;
class symbol_t;

#include <vector>
#include <assert.h>
#include "errors.h"

struct base_exception_t
{
};

class robosim_exception_t : public base_exception_t
{
public:
	int _line_number = 0;
	error_e _error_code = error_none;
	const node_t* _evalnode = nullptr;
	std::vector<std::string> _function_names;
	void prepend_function_name(const std::string& str)
	{
		_function_names.insert(_function_names.begin(), str);
	}
protected:
	robosim_exception_t(const node_t* pnode, error_e err, int line) :
		_evalnode(pnode), _error_code(err), _line_number(line)
	{
	}
	robosim_exception_t(error_e err, int line) : _line_number(line), _error_code(err)
	{
	}
	robosim_exception_t(error_e err) : _error_code(err)
	{
	}
	robosim_exception_t()
	{
	}
};

class read_exception_t : public robosim_exception_t
{
public:
	read_exception_t(error_e err) : robosim_exception_t(err)
	{
	}
};

class eval_exception_t : public robosim_exception_t
{
public:
	eval_exception_t(const node_t* n, error_e err) : robosim_exception_t(err)
	{
		_evalnode = n;
	}
	eval_exception_t(error_e err) : robosim_exception_t(err)
	{
		_evalnode = nullptr;
	}
};

class block_return_exception_t : public robosim_exception_t
{
public:
	const symbol_t* _block;
	node_t* _retval;
	// UNKNOWN_BLOCK_NAME will be used if the exception is not caught
	block_return_exception_t(const symbol_t* b, node_t* val)
		: robosim_exception_t(), _block(b), _retval(val)
	{
	}
};

class interrupt_exception_t : public robosim_exception_t
{
public:
	interrupt_exception_t() : robosim_exception_t()
	{
	}
};

// A lisp_exception_t represents an exception catchable in lisp code itself.
class lisp_exception_t : public robosim_exception_t
{
public:
	const node_t* _tag;
	node_t* _retval;
	lisp_exception_t(const node_t* p, node_t* val) :
		robosim_exception_t(), _tag(p), _retval(val)
	{
	}
};

#endif
