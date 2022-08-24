#pragma once
#ifndef __REXCEPT_H
#define __REXCEPT_H

class node_t;
class symbol_t;

#include <vector>
#include "errors.h"

class robosim_exception_t : public CException
{
	DECLARE_DYNAMIC(robosim_exception_t)
public:
	int _line_number = 0;
	error_e _error_code = error_none;
	const node_t* _evalnode = nullptr;
	std::vector<const char*> _function_names;
	robosim_exception_t(const node_t* pnode, error_e err, int line) :
		CException(TRUE), _evalnode(pnode), _error_code(err), _line_number(line)
	{
	}
	robosim_exception_t(error_e err, int line) : CException(TRUE), _line_number(line), _error_code(err)
	{
	}
	robosim_exception_t(bool auto_delete) : CException(auto_delete)
	{
	}
	robosim_exception_t() : CException(TRUE)
	{
	}
};

class done_exception_t : public robosim_exception_t
{
	DECLARE_DYNAMIC(done_exception_t)
public:
	done_exception_t(BOOL bAutoDelete = TRUE) : robosim_exception_t(bAutoDelete) {}
};

class read_exception_t : public robosim_exception_t
{
	DECLARE_DYNAMIC(read_exception_t)
public:
	read_exception_t(error_e err) : robosim_exception_t()
	{
		_error_code = err;
	}
};

class eval_exception_t : public robosim_exception_t
{
	DECLARE_DYNAMIC(eval_exception_t)
public:
	eval_exception_t(const node_t* n, error_e err) : robosim_exception_t()
	{
		_evalnode = n;
		_error_code = err;
	}
};

class block_return_exception_t : public robosim_exception_t
{
	DECLARE_DYNAMIC(block_return_exception_t)
public:
	const symbol_t* _block;
	node_t* _retval;
	block_return_exception_t(const symbol_t* b, node_t* val)
		: robosim_exception_t(), _block(b), _retval(val)
	{
	}
};

class interrupt_exception_t : public robosim_exception_t
{
	DECLARE_DYNAMIC(interrupt_exception_t)
public:
	interrupt_exception_t() : robosim_exception_t()
	{
	}
};

class other_exception_t : public robosim_exception_t
{
	DECLARE_DYNAMIC(other_exception_t)
public:
	const node_t* _tag;
	node_t* _retval;
	other_exception_t(const node_t* p, node_t* val) : 
		robosim_exception_t(), _tag(p), _retval(val)
	{
	}
};

void throw_done_exception();
void throw_read_exception(error_e err);
void throw_eval_exception(const node_t* n, error_e err);
void throw_eval_exception(error_e err);
void throw_block_return_exception(const symbol_t* b, node_t* val);
void throw_interrupt_exception();
void throw_other_exception(const node_t* p, node_t* val);

#endif
