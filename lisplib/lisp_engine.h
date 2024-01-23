#pragma once
#ifndef __LISPENGINE_H
#define __LISPENGINE_H

#include "lispenv.h"
#include "host.h"
#include "package.h"
#include "argstack.h"
#include "ik.h"

struct lisp_engine_t
{
	bool _garbage_collected = false;
	lisp_host_t* _host = nullptr;

	node_list_array_t _node_lists;
	lisp_env_t _env{ std::cin, std::cout };
	package_t _package{ "COMMON-LISP", "CL" };
	std::vector<std::unique_ptr<ik_interface>> _invkin;


	/* _frame_stack is just a centralized place to put arguments to functions so that when the GC
	   runs, it can mark these nodes as in use. */
	frame_stack_t _frame_stack;
	binding_stack_t _bind_stack;

	void post_garbage_collect();
	int garbage_collect();


	lisp_engine_t()
	{
	}
	~lisp_engine_t()
	{
	}
};

extern lisp_engine_t lisp_engine;

struct unbinder_t
{
	binding_stack_t& _stack;
	int _count;
	unbinder_t(binding_stack_t& stack) : _stack(stack), _count(0)
	{
	}
	unbinder_t(binding_stack_t& stack, int count) : _stack(stack), _count(count)
	{
	}
	~unbinder_t()
	{
		_stack.unbind(_count);
	}
	void add_one()
	{
		++_count;
	}
};


#endif
