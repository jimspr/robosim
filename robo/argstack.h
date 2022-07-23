#pragma once
#ifndef __ARGSTACK_H
#define __ARGSTACK_H

#include <vector>
#include "node.h"

/* frame_stack_t holds all arguments to functions that are currently running.
   Its primary use is so that the GC can mark all of the arguments as in use. */
class frame_stack_t
{
protected:
	std::vector<node_t*> _args;
	friend class frame_stack_state_t;

public:
	frame_stack_t()
	{
		_args.reserve(128);
	}

	void mark_in_use(void);
	void print(std::ostream &) const;
	void push(node_t *pn)
	{
		_args.push_back(pn);
	}
	node_t* pop();
	void pop(int n);
	node_t* top();
	node_t** get_base(int n);
	void duplicate(int n);
	void clear(void) { _args.clear(); }
};

/* RAII type for managing binding stack. Remembers the last known position in node_stack
   and resets to that when destroyed. */
class frame_stack_state_t
{
	frame_stack_t& _stack;
	size_t _position;
public:
	frame_stack_state_t(frame_stack_t& stack) : _stack(stack)
	{
		_position = _stack._args.size();
	}
	~frame_stack_state_t()
	{
		/* Should only remove items. */
		ASSERT(_position <= _stack._args.size());
		_stack._args.resize(_position);
	}
};

class binding_stack_t
{
protected:
	bound_symbol_t *_cur = nullptr;
	std::vector<bound_symbol_t*> _ns1;
	std::vector<bound_symbol_t*> _ns2;

	void rebind(void);
	bound_symbol_t* pop_ns1();
	bound_symbol_t* pop_ns2();
public:
	void print(std::ostream &ostr) const;
	int bind(cons_t *arglist,node_t **vals);
	void bind(symbol_t *ps,node_t *val);
	void unbind(int numargs=1);
	void unbind_to(bound_symbol_t *nb);
	bound_symbol_t *get_env(void) const;
	bound_symbol_t *swap_stack(bound_symbol_t *nb);
	void mark_in_use(void);
};

/* RAII type for managing binding stack. */
class binding_stack_state_t
{
	binding_stack_t& _stack;
public:
	bound_symbol_t* _prev = nullptr;
	binding_stack_state_t(binding_stack_t& stack, bound_symbol_t* bs) : _stack(stack)
	{
		_prev = _stack.swap_stack(bs);
	}
	~binding_stack_state_t()
	{
		_stack.swap_stack(_prev);
	}
};

extern frame_stack_t g_frame_stack;
extern binding_stack_t g_bind_stack;
#endif
