#include "stdafx.h"
#include "node.h"
#include "argstack.h"

/*
				form_t
				 /|
				/ |
			   /  |
			  /   |
			 /    |
			/     |
		special_form_t   function
			 /     \
			/       \
		 usrfunction_t         sysfunction_t
*/

form_t::form_t(node_type_e type, const char* name) : node_t(type), _form_name(name)
{
}

special_form_t::special_form_t(const char* name, PFORMCALL pf) : form_t(TYPE_SPECIALFORM, name)
{
	pfunc = pf;
}

node_t* special_form_t::eval(cons_t* ths)
{
	try
	{
		return pfunc(ths);
	}
	catch (robosim_exception_t* e)
	{
		e->_function_names.insert(e->_function_names.begin(), _form_name.c_str());
		throw;
	}
	ASSERT(false);
	return nil;
}

function::function(const char* name, int min, int max) : form_t(TYPE_FUNCTION, name), _minargs(min), _maxargs(max)
{
}

node_t* function::eval(cons_t* ths)
{
	node_t* res = NULL;
	frame_stack_state_t state(g_frame_stack); // Resets stack when destroyed.
	try
	{
		int numargs = 0;
		while (ths->is_a(TYPE_CONS))
		{
			g_frame_stack.push(ths->Car()->eval());
			numargs++;
			ths = ths->CdrCONS();
		}
		res = eval(numargs, g_frame_stack.get_base(numargs));
	}
	catch (robosim_exception_t* e)
	{
		e->_function_names.insert(e->_function_names.begin(), _form_name.c_str());
		throw;
	}
	return res;
}

// evalnoargs doesn't eval the parameters passed in
// this is useful in certain situations such as for inverse kinematics
node_t* function::evalnoargs(cons_t* ths)
{
	node_t* res = NULL;
	frame_stack_state_t state(g_frame_stack); // Resets stack when destroyed.
	try
	{
		int numargs = 0;
		while (ths->is_a(TYPE_CONS))
		{
			g_frame_stack.push(ths->Car());
			numargs++;
			ths = ths->CdrCONS();
		}
		res = eval(numargs, g_frame_stack.get_base(numargs));
	}
	catch (robosim_exception_t* e)
	{
		e->_function_names.insert(e->_function_names.begin(), _form_name.c_str());
		throw;
	}
	return res;
}

void function::check_args(int n)
{
	if (_minargs >= 0 && n < _minargs)
		throw_eval_exception(TOO_FEW_ARGS);
	if (_maxargs >= 0 && n > _maxargs)
		throw_eval_exception(TOO_MANY_ARGS);
}

sysfunction_t::sysfunction_t(const char* name, PFUNCCALL pF, int min, int max) : function(name, min, max), _pfunc(pF)
{
}

node_t* sysfunction_t::eval(int numargs, node_t** base)
{
	check_args(numargs);
	return _pfunc(numargs, base);
}

usrfunction_t::usrfunction_t(const char* name, int min, int max, cons_t* varlist, cons_t* b, bound_symbol_t* e) : function(name, min, max), _bindings(varlist), _body(b), _env(e)
{
}

void usrfunction_t::mark_in_use(void)
{
	if (is_in_use())
		return;
	node_t::mark_in_use();
	_bindings->mark_in_use();
	_body->mark_in_use();
	if (_env)
		_env->mark_in_use();
}

node_t* usrfunction_t::eval(int numargs, node_t** base)
{
	check_args(numargs);
	node_t* result = nil;
	cons_t* tbody = _body;

	binding_stack_state_t bs_state(g_bind_stack, _env);
	frame_stack_state_t ns_state(g_frame_stack); /* Clean up g_frame_stack. */
	g_frame_stack.push(bs_state._prev);
	int num = g_bind_stack.bind(_bindings, base);

	try
	{
		while (tbody->is_a(TYPE_CONS))
		{
			result = tbody->Car()->eval();
			tbody = tbody->CdrCONS();
		}
	}
	catch (block_return_exception_t* e)
	{
		if ((e->_block == nil) && (_form_name.size() == 0))
			result = e->_retval;
		else if (_form_name != e->_block->get_name())
		{
			g_bind_stack.unbind(num);
			throw;
		}
		else
			result = e->_retval;
		e->Delete();
	}
	catch (CException*)
	{
		g_bind_stack.unbind(num);
		throw;
	}
	g_bind_stack.unbind(num);
	return result;
}
