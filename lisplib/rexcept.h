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

// This class will track the stack of lisp calls when the exception occurs.
struct tracking_exception_t : public base_exception_t
{
	std::vector<std::string> _function_names;
	void prepend_function_name(const std::string& str)
	{
		_function_names.insert(_function_names.begin(), str);
	}
};

// Used for exceptions that are actually "errors".
class error_exception_t : public tracking_exception_t
{
public:
	const error_e _error_code = error_none;
protected:
	error_exception_t(error_e err) : _error_code(err)
	{
	}
};

class read_exception_t : public error_exception_t
{
public:
	read_exception_t(error_e err) : error_exception_t(err)
	{
	}
};

class eval_exception_t : public error_exception_t
{
public:
	const node_t* _evalnode = nullptr;
	eval_exception_t(const node_t* n, error_e err) : error_exception_t(err), _evalnode(n)
	{
	}
	eval_exception_t(error_e err) : error_exception_t(err)
	{
	}
};

// This exception is used by "return" and "return-from".
class block_return_exception_t : public tracking_exception_t
{
public:
	const symbol_t* const _block;
	node_t* const _retval;
	block_return_exception_t(const symbol_t* b, node_t* val) : _block(b), _retval(val)
	{
	}
};

// The interrupt exception is used to cancel long running simulations or calculations.
class interrupt_exception_t : public base_exception_t
{
public:
	interrupt_exception_t() : base_exception_t()
	{
	}
};

// A lisp_exception_t represents an exception catchable in lisp code itself.
class lisp_exception_t : public tracking_exception_t
{
public:
	const node_t* const _tag;
	node_t* const _retval;
	lisp_exception_t(const node_t* p, node_t* val) : _tag(p), _retval(val)
	{
	}
};

#endif
