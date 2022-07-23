#pragma once
#ifndef __REXCEPT_H
#define __REXCEPT_H

class node_t;
class symbol_t;

#include <vector>

class robosim_exception_t : public CException
{
	DECLARE_DYNAMIC(robosim_exception_t)
public:
	int _line_number;
	int _error_code;
	const node_t* _evalnode;
	std::vector<const char*> _function_names;
	robosim_exception_t(const node_t* pnode, int err, int line,
		BOOL bAutoDelete = TRUE) : CException(bAutoDelete)
	{
		_line_number = line; _error_code = err; _evalnode = pnode;
	}
	robosim_exception_t(BOOL bAutoDelete = TRUE) : CException(bAutoDelete)
	{
		_line_number = 0; _error_code = 0; _evalnode = NULL;
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
	read_exception_t(int err, BOOL bAutoDelete = TRUE) : robosim_exception_t(bAutoDelete)
	{
		_error_code = err;
	}
};

class eval_exception_t : public robosim_exception_t
{
	DECLARE_DYNAMIC(eval_exception_t)
public:
	eval_exception_t(const node_t* n, int err, BOOL bAutoDelete = TRUE) : robosim_exception_t(bAutoDelete)
	{
		_evalnode = n;
		_error_code = err;
	}
};

class block_return_exception_t : public robosim_exception_t
{
	DECLARE_DYNAMIC(block_return_exception_t)
public:
	const symbol_t* block;
	node_t* retval;
	block_return_exception_t(const symbol_t* b, node_t* val, BOOL bAutoDelete = TRUE)
		: robosim_exception_t(bAutoDelete)
	{
		block = b;
		retval = val;
	}
};

class interrupt_exception_t : public robosim_exception_t
{
	DECLARE_DYNAMIC(interrupt_exception_t)
public:
	interrupt_exception_t(BOOL bAutoDelete = TRUE) : robosim_exception_t(bAutoDelete)
	{
	}
};

class other_exception_t : public robosim_exception_t
{
	DECLARE_DYNAMIC(other_exception_t)
public:
	const node_t* tag;
	node_t* retval;
	other_exception_t(const node_t* p, node_t* val, BOOL bAutoDelete = TRUE) : robosim_exception_t(bAutoDelete)
	{
		tag = p;
		retval = val;
	}
};

void throw_done_exception();
void throw_read_exception(int err);
void throw_eval_exception(const node_t* n, int err);
void throw_eval_exception(int err);
void throw_block_return_exception(const symbol_t* b, node_t* val);
void throw_interrupt_exception();
void throw_other_exception(const node_t* p, node_t* val);

#endif
