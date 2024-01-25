#include "pch.h"

#include "argstack.h"
#include "node.h"

void frame_stack_t::print(std::ostream &ostr) const
{
	ostr << "#<Stack::" << _args.size();
	for (auto& node : _args)
		ostr << ' ' << node;
	ostr << ">\n";
}

node_t* frame_stack_t::top()
{
	node_t* node = nullptr;
	if (!_args.empty())
	{
		node = _args.back();
	}
	return node;
}

node_t* frame_stack_t::pop(void)
{
	node_t* node = nullptr;
	if (!_args.empty())
	{
		node = _args.back();
		_args.pop_back();
	}
	return node;
}

void frame_stack_t::pop(int n)
{
	while (!_args.empty() && (n != 0))
	{
		_args.pop_back();
		--n;
	}
	if (n != 0)
		throw_eval_exception(STACK_CORRUPT);
};

void frame_stack_t::mark_in_use(void)
{
	for (auto& node : _args)
	{
		if (node != nullptr)
			node->mark_in_use();
	}
}

node_t** frame_stack_t::get_base(int n)
{
	if (n == 0)
		return nullptr;
	assert(_args.size() - n >= 0);
	return &_args[_args.size() - n];
}

void frame_stack_t::duplicate(int n)
{
	if (n > (int)_args.size())
		throw_eval_exception(STACK_CORRUPT);
	_args.insert(_args.end(), _args.end() - n, _args.end());
}

bound_symbol_t* binding_stack_t::pop_ns1()
{
	bound_symbol_t* ret = nullptr;
	if (!_ns1.empty())
	{
		ret = _ns1.back();
		_ns1.pop_back();
	}
	return ret;
}

bound_symbol_t* binding_stack_t::pop_ns2()
{
	bound_symbol_t* ret = nullptr;
	if (!_ns2.empty())
	{
		ret = _ns2.back();
		_ns2.pop_back();
	}
	return ret;
}

void binding_stack_t::print(std::ostream &ostr) const
{
	ostr << "#<_bind_stack:";
	for (bound_symbol_t *t = _cur; t; t = t->_child)
		ostr << ' ' << t;
	ostr << ">";
}

void binding_stack_t::bind(symbol_t *p,node_t *val)
{
	_cur = new bound_symbol_t(p,p->get_value(), _cur);
	p->set_value(val);
}

int binding_stack_t::bind(cons_t *arglist,node_t **vals)
{
	int numargs=0;
	symbol_t *p;
	while (arglist->is_a(TYPE_CONS))
	{
		p = arglist->Car()->as<symbol_t>();

		bind(p,*vals++);

		numargs++;
		arglist = arglist->CdrCONS();
	}
	return numargs;
}

void binding_stack_t::unbind(int numargs)
{
	bound_symbol_t *lns = NULL;
	while(numargs--)
	{
		if (!_cur)
			throw_eval_exception(STACK_CORRUPT);
		_cur->swap_values();
		if (!_cur->_symbol->is_special_value())
		{
			if (lns)
				lns->_child = _cur;
			lns = _cur;
		}
		_cur = _cur->_child;
	}
	if (lns)
		lns->_child = nullptr;
}


void binding_stack_t::unbind_to(bound_symbol_t *to)
{
	if (to)
		to = to->_child;
	while(_cur != to)
	{
		if (!_cur)
			throw_eval_exception(STACK_CORRUPT);
		if (!_cur->_symbol->is_special_value())
			_cur->swap_values();
		_cur = _cur->_child;
	}
}

bound_symbol_t *binding_stack_t::swap_stack(bound_symbol_t *nb)
{
	bound_symbol_t *tc = _cur;
	bound_symbol_t *old = _cur;
	bound_symbol_t *tn = nb;
	if (!tc && !tn)
		return tc;
	_ns1.clear();
	while (tc)
	{
		if (!tc->_symbol->is_special_value())
			_ns1.push_back(tc);
		tc = tc->_child;
	}
	_ns2.clear();
	while (tn)
	{
		if (!tn->_symbol->is_special_value())
			_ns2.push_back(tn);
		tn = tn->_child;
	}
	tc = pop_ns1();
	tn = pop_ns2();
	while (tc && tn)
	{
		if (tc != tn)
			break;
		tc = pop_ns1();
		tn = pop_ns2();
	}
	unbind_to(tc);
	_cur = nb;
	while(tn)
	{
		tn->swap_values();
		tn = pop_ns2();
	}
	return old;
}

void binding_stack_t::mark_in_use(void)
{
	if (_cur)
		_cur->mark_in_use();
}

bound_symbol_t *binding_stack_t::get_env(void) const
{
	bound_symbol_t *t = _cur;
	while (t)
	{
		if (!t->_symbol->is_special_value())
			return t;
		t = t->_child;
	}
	return t;
}
