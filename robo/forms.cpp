#include "stdafx.h"
#include <errno.h>
#include "node.h"
#include "argstack.h"
#include "package.h"
#include <chrono>

using namespace std;


static node_t *form_block(cons_t *ths);
static node_t *form_catch(cons_t *ths);
//static node_t *form_compiler__let(cons_t *ths);
//static node_t *form_declare(cons_t *ths);
//static node_t *form_eval__when(cons_t *ths);
//static node_t *form_flet(cons_t *ths);
static node_t *form_function(cons_t *ths);
//static node_t *form_generic__flet(cons_t *ths);
//static node_t *form_generic__labels(cons_t *ths);
//static node_t *form_go(cons_t *ths);
static node_t *form_if(cons_t *ths);
//static node_t *form_labels(cons_t *ths);
static node_t *form_let(cons_t *ths);
static node_t *form_let_star(cons_t *ths);
//static node_t *form_load__time__value(cons_t *ths);
//static node_t *form_locally(cons_t *ths);
//static node_t *form_macrolet(cons_t *ths);
//static node_t *form_multiple__value__call(cons_t *ths);
//static node_t *form_multiple__value__prog1(cons_t *ths);
static node_t *form_progn(cons_t *ths);
//static node_t *form_progv(cons_t *ths);
static node_t *form_quote(cons_t *ths);
static node_t *form_return__from(cons_t *ths);
static node_t *form_setq(cons_t *ths);
//static node_t *form_symbol__macrolet(cons_t *ths);
//static node_t *form_tagbody(cons_t *ths);
//static node_t *form_the(cons_t *ths);
static node_t *form_throw(cons_t *ths);
static node_t *form_unwind__protect(cons_t *ths);
//static node_t *form_with__added__methods(cons_t *ths);


static node_t *macro_and(cons_t *ths);
//static node_t *macro_assert(cons_t *ths);
//static node_t *macro_call__method(cons_t *ths);
//static node_t *macro_case(cons_t *ths);
//static node_t *macro_ccase(cons_t *ths);
//static node_t *macro_check__type(cons_t *ths);
//static node_t *macro_compiler__let(cons_t *ths);
static node_t *macro_cond(cons_t *ths);
//static node_t *macro_ctypecase(cons_t *ths);
//static node_t *macro_decf(cons_t *ths);
//static node_t *macro_declaim(cons_t *ths);
//static node_t *macro_defclass(cons_t *ths);
static node_t *macro_defconstant(cons_t *ths);
//static node_t *macro_defgeneric(cons_t *ths);
//static node_t *macro_define__compiler__macro(cons_t *ths);
//static node_t *macro_define__condition(cons_t *ths);
//static node_t *macro_define__declaration(cons_t *ths);
//static node_t *macro_define__method__combination(cons_t *ths);
//static node_t *macro_define__modify__macro(cons_t *ths);
//static node_t *macro_define__setf__method(cons_t *ths);
//static node_t *macro_defmacro(cons_t *ths);
//static node_t *macro_defmethod(cons_t *ths);
//static node_t *macro_defpackage(cons_t *ths);
static node_t *macro_defparameter(cons_t *ths);
//static node_t *macro_defsetf(cons_t *ths);
//static node_t *macro_defstruct(cons_t *ths);
//static node_t *macro_deftype(cons_t *ths);
static node_t *macro_defun(cons_t *ths);
static node_t *macro_defvar(cons_t *ths);
//static node_t *macro_destructuring__bind(cons_t *ths);
static node_t *macro_do(cons_t *ths);
static node_t *macro_do_star(cons_t *ths);
//static node_t *macro_do__all__symbols(cons_t *ths);
//static node_t *macro_do__external__symbols(cons_t *ths);
//static node_t *macro_do__symbols(cons_t *ths);
//static node_t *macro_dolist(cons_t *ths);
//static node_t *macro_dotimes(cons_t *ths);
//static node_t *macro_ecase(cons_t *ths);
//static node_t *macro_encapsulated(cons_t *ths);
//static node_t *macro_etypecase(cons_t *ths);
//static node_t *macro_flet(cons_t *ths);
//static node_t *macro_formatter(cons_t *ths);
//static node_t *macro_gathering(cons_t *ths);
//static node_t *macro_generic__function(cons_t *ths);
//static node_t *macro_handler__bind(cons_t *ths);
//static node_t *macro_handler__case(cons_t *ths);
//static node_t *macro_ignore__errors(cons_t *ths);
//static node_t *macro_in__package(cons_t *ths);
//static node_t *macro_incf(cons_t *ths);
//static node_t *macro_iterate(cons_t *ths);
//static node_t *macro_labels(cons_t *ths);
//static node_t *macro_let(cons_t *ths);
//static node_t *macro_let_star(cons_t *ths);
//static node_t *macro_locally(cons_t *ths);
static node_t *macro_loop(cons_t *ths);
//static node_t *macro_loop__finish(cons_t *ths);
//static node_t *macro_macrolet(cons_t *ths);
//static node_t *macro_mapping(cons_t *ths);
//static node_t *macro_multiple__value__bind(cons_t *ths);
//static node_t *macro_multiple__value__list(cons_t *ths);
//static node_t *macro_multiple__value__setq(cons_t *ths);
//static node_t *macro_next__in(cons_t *ths);
//static node_t *macro_nth__value(cons_t *ths);
static node_t *macro_or(cons_t *ths);
//static node_t *macro_pop(cons_t *ths);
//static node_t *macro_pprint__exit__if__list__exhausted(cons_t *ths);
//static node_t *macro_pprint__logical__block(cons_t *ths);
//static node_t *macro_pprint__pop(cons_t *ths);
//static node_t *macro_pprint__unreadable__object(cons_t *ths);
//static node_t *macro_producing(cons_t *ths);
static node_t *macro_prog(cons_t *ths);
static node_t *macro_prog_star(cons_t *ths);
static node_t *macro_prog1(cons_t *ths);
static node_t *macro_prog2(cons_t *ths);
//static node_t *macro_psetf(cons_t *ths);
static node_t *macro_psetq(cons_t *ths);
//static node_t *macro_push(cons_t *ths);
//static node_t *macro_pushnew(cons_t *ths);
//static node_t *macro_remf(cons_t *ths);
//static node_t *macro_restart__bind(cons_t *ths);
//static node_t *macro_restart__case(cons_t *ths);
static node_t *macro_return(cons_t *ths);
//static node_t *macro_rotatef(cons_t *ths);
//static node_t *macro_setf(cons_t *ths);
//static node_t *macro_shiftf(cons_t *ths);
//static node_t *macro_step(cons_t *ths);
//static node_t *macro_terminate__producing(cons_t *ths);
static node_t *macro_time(cons_t *ths);
//static node_t *macro_trace(cons_t *ths);
//static node_t *macro_typecase(cons_t *ths);
static node_t *macro_unless(cons_t *ths);
//static node_t *macro_untrace(cons_t *ths);
static node_t *macro_when(cons_t *ths);
//static node_t *macro_with__accessors(cons_t *ths);
//static node_t *macro_with__compilation__unit(cons_t *ths);
//static node_t *macro_with__condition__restarts(cons_t *ths);
//static node_t *macro_with__hash__table__iterator(cons_t *ths);
//static node_t *macro_with__input__from__string(cons_t *ths);
//static node_t *macro_with__open__file(cons_t *ths);
//static node_t *macro_with__open__stream(cons_t *ths);
//static node_t *macro_with__output__to__string(cons_t *ths);
//static node_t *macro_with__package__iterator(cons_t *ths);
//static node_t *macro_with__simple__restart(cons_t *ths);
//static node_t *macro_with__slots(cons_t *ths);
//static node_t *macro_with__standard__io__syntax(cons_t *ths);


#define specform(n,f) current_package->get_symbol(n)->set_constant_form(new special_form_t(n,f))
#define macro(n,f) current_package->get_symbol(n)->set_form(new macro_t(n,f))

void init_forms()
{
	specform("BLOCK",       form_block);
	specform("FUNCTION",    form_function);
	specform("IF",          form_if);
	specform("PROGN",       form_progn);
	specform("QUOTE",       form_quote);
	specform("RETURN-FROM",	form_return__from);
	specform("SETQ",        form_setq);
	specform("LET",         form_let);
	specform("LET*",        form_let_star);
	specform("UNWIND-PROTECT",form_unwind__protect);
	specform("CATCH",	form_catch);
	specform("THROW",	form_throw);

// macros
	macro("AND",   macro_and);
	macro("COND",  macro_cond);
	macro("DEFCONSTANT",macro_defconstant);
	macro("DEFPARAMETER",macro_defparameter);
	macro("DEFUN", macro_defun);
	macro("DEFVAR",macro_defvar);
	macro("DO",    macro_do);
	macro("DO*",   macro_do_star);
	macro("LOOP",  macro_loop);
	macro("OR",    macro_or);
	macro("PROG",  macro_prog);
	macro("PROG*",  macro_prog_star);
	macro("PROG1", macro_prog1);
	macro("PROG2", macro_prog2);
	macro("PSETQ", macro_psetq);
	macro("RETURN",macro_return);
	macro("UNLESS",macro_unless);
	macro("WHEN",  macro_when);
	macro("TIME",  macro_time);
}


/*******************************************************************
********************************************************************
********************************************************************
*********************            ***************************
*********************   SUPPORT FUNCS    ***************************
*********************            ***************************
********************************************************************
********************************************************************
*******************************************************************/


class vardecl
{
public:
	symbol_t *psymbol = nullptr;
	node_t *init = nullptr;
	node_t *step = nullptr;
	bool _pop = false;
	void set(cons_t *n,int bStep);
	vardecl(const vardecl&) = delete;
	vardecl(vardecl&& v)
	{
		std::swap(psymbol, v.psymbol);
		std::swap(init, v.init);
		std::swap(step, v.step);
		std::swap(_pop, v._pop);
	}
	vardecl() = default;
	~vardecl();
};

void vardecl::set(cons_t *var,int bStep)
{
	if (var->is_a(TYPE_SYMBOL))
	{
		psymbol = (symbol_t *)var;
		init = nil;
		step = NULL;
	}
	else if (var->is_a(TYPE_CONS))
	{
		var->check_range_num_args(1,(bStep)? 3 : 2);
		psymbol = var->Car()->as<symbol_t>();
		init = nil;
		step = NULL;
		var = var->CdrCONS();
		if (var->is_a(TYPE_CONS))
		{
			init = var->Car()->eval();
			var = var->CdrCONS();
			if (var->is_a(TYPE_CONS))
				step = var->Car();
		}
	}
	else
		throw_eval_exception(BAD_ARG_TYPE);
	g_frame_stack.push(init);
	g_frame_stack.push(step);
	_pop = true;
}

vardecl::~vardecl()
{
	if (_pop)
		g_frame_stack.pop(2);
}

static int dobindings(vector<vardecl> &varlist,cons_t *b,int bStep)
{
	volatile int shadowi;
	int i,numvars;
	b->check_arg_type(TYPE_CONS);
	numvars = b->get_num_items();
	if (numvars)
		varlist.resize(numvars);
	for (i=0;b->is_a(TYPE_CONS);b=b->CdrCONS(),i++)
		varlist[i].set(b->CarCONS(),bStep);
	try
	{
		for (shadowi=i=0;i<numvars;shadowi = ++i)
			g_bind_stack.bind(varlist[i].psymbol,varlist[i].init);
	}
	catch(CException*)
	{
		g_bind_stack.unbind(shadowi);
		throw;
	}
	return numvars;
}

static int dostarbindings(vector<vardecl> &varlist,cons_t *b,int bStep)
{
	int shadowi;
	int i,numvars;
	b->check_arg_type(TYPE_CONS);
	numvars = b->get_num_items();
	if (numvars)
		varlist.resize(numvars);
	try
	{
		for (shadowi=i=0;b->is_a(TYPE_CONS);b=b->CdrCONS(),shadowi=++i)
		{
			varlist[i].set(b->CarCONS(),bStep);
			g_bind_stack.bind(varlist[i].psymbol,varlist[i].init);
		}
	}
	catch(CException*)
	{
		g_bind_stack.unbind(shadowi);
		throw;
	}
	return numvars;
}


/*******************************************************************
********************************************************************
********************************************************************
*********************       ****************************************
********************* FORMS ****************************************
*********************       ****************************************
********************************************************************
********************************************************************
*******************************************************************/

static node_t *form_quote(cons_t *ths)
{
	ths->check_num_args(1);
	return ths->Car();
}

static node_t *form_setq(cons_t *ths)
{
	int n = ths->get_num_items();
	if (n%2)
		throw_eval_exception(TOO_FEW_ARGS);
	node_t *res = nil;
	while (ths->is_a(TYPE_CONS))
	{
		symbol_t *sym = ths->Car()->as<symbol_t>();
		res = ths->Cadr()->eval();
		sym->set_value(res);
		ths = ths->CddrCONS();
	}
	return res;
}

static node_t *form_if(cons_t *ths)
{
	ths->check_min_num_args(2);
	if (ths->Car()->eval() == nil)
	{
		if (ths->Cddr()->is_a(TYPE_CONS))
			return ths->Caddr()->eval();
		else
			return nil;
	}
	else
		return ths->Cadr()->eval();
}

static node_t *form_progn(cons_t *ths)
{
	if (!ths->is_a(TYPE_CONS))
		return nil;
	while (ths->Cdr()->is_a(TYPE_CONS))
	{
		ths->Car()->eval();
		ths = ths->CdrCONS();
	}
	return ths->Car()->eval();
}

static node_t *form_unwind__protect(cons_t *ths)
{
	ths->check_min_num_args(1);
	node_t *res;
	try
	{
		res = ths->Car()->eval();
	}
	catch(CException*)
	{
		ths = ths->CdrCONS();
		while (ths->is_a(TYPE_CONS))
		{
			ths->Car()->eval();
			ths = ths->CdrCONS();
		}
		throw;
	}
	ths = ths->CdrCONS();
	while (ths->is_a(TYPE_CONS))
	{
		ths->Car()->eval();
		ths = ths->CdrCONS();
	}
	return res;
}

static node_t *form_block(cons_t *ths)
{
	node_t *result = nil;
	ths->check_min_num_args(1);
	symbol_t *psym = ths->Car()->as<symbol_t>();
	try
	{
		result = form_progn(ths->CdrCONS());
	}
	catch(block_return_exception_t* e)
	{
		if (e->_block == psym)
		{
			result = e->_retval;
			e->Delete();
		}
		else
			throw;
	}
	return result;
}

static node_t *form_function(cons_t *ths)
{
	ths->check_num_args(1);
	if (ths->Car()->is_a(TYPE_SYMBOL))
	{
		form_t *pf= ths->Car()->as<symbol_t>()->check_form();
		pf->check_arg_type(TYPE_FUNCTION);
		return pf;
	}
	ths = ths->CarCONS();
	ths->check_min_num_args(2); // at least lambda and nil or a list of parms
	symbol_t *lambda = ths->Car()->as<symbol_t>();
	if (lambda != current_package->get_symbol("LAMBDA"))
		throw_eval_exception(BAD_ARG_TYPE);
	cons_t *varlist = ths->CadrCONS();
	if (varlist != (cons_t *)nil && !varlist->is_a(TYPE_CONS))
		throw_eval_exception(BAD_ARG_TYPE);
	int num = varlist->get_num_items();
	return new usrfunction_t("",num,num,varlist,ths->CddrCONS(),g_bind_stack.get_env());
}

static node_t *form_return__from(cons_t *ths)
{
	ths->check_min_num_args(1);
	symbol_t *psym = ths->Car()->as<symbol_t>();
	cons_t *value = ths->CdrCONS();
	psym->check_arg_type(TYPE_SYMBOL);
	throw_block_return_exception(psym,(value->is_a(TYPE_CONS)) ? value->Car()->eval() : nil);
	return nil;
}

static node_t *form_let(cons_t *ths)
{
// (let ( {var | (var [value])}* ) {form_t}* )

	node_t *retval = nil;
	cons_t *bindings,*body;
	int numvars;
	vector<vardecl> varlist;

	ths->check_range_num_args(1,-1);
	bindings = ths->CarCONS();
	body = ths->CdrCONS();

	numvars = dobindings(varlist,bindings,0);
	varlist.clear();

	try
	{
		while (body->is_a(TYPE_CONS))
		{
			retval = body->Car()->eval();
			body = body->CdrCONS();
		}
	}
	catch(CException*)
	{
		g_bind_stack.unbind(numvars);
		throw;
	}

	g_bind_stack.unbind(numvars);
	return retval;
}

static node_t *form_let_star(cons_t *ths)
{
// (let* ( {var | (var [value])}* ) {form_t}* )

	node_t *retval = nil;
	cons_t *bindings,*body;
	int numvars;
	vector<vardecl> varlist;

	ths->check_range_num_args(1,-1);
	bindings = ths->CarCONS();
	body = ths->CdrCONS();

	numvars = dostarbindings(varlist,bindings,0);

	try
	{
		while (body->is_a(TYPE_CONS))
		{
			retval = body->Car()->eval();
			body = body->CdrCONS();
		}
	}
	catch(CException*)
	{
		g_bind_stack.unbind(numvars);
		throw;
	}

	g_bind_stack.unbind(numvars);
	return retval;
}
/*******************************************************************
********************************************************************
********************************************************************
*********************        ***************************************
********************* MACROS ***************************************
*********************        ***************************************
********************************************************************
********************************************************************
*******************************************************************/


static node_t *macro_and(cons_t *ths)
{
	node_t *res = nil;
	while(ths->is_a(TYPE_CONS))
	{
		res = ths->Car()->eval();
		if (res == nil)
			return nil;
		ths = ths->CdrCONS();
	}
	return res;
}

static node_t *macro_or(cons_t *ths)
{
	node_t *res = nil;
	while(ths->is_a(TYPE_CONS))
	{
		res = ths->Car()->eval();
		if (res != nil)
			return res;
		ths = ths->CdrCONS();
	}
	return res;
}

static node_t *macro_psetq(cons_t *ths)
{
	node_t *t = ths;
	int n = ths->get_num_items();
	if (n%2)
		throw_eval_exception(TOO_FEW_ARGS);
	if (n==0)
		return nil;
	vector<node_t*> res(n / 2);
	n=0;
	while (ths->is_a(TYPE_CONS))
	{
		res[n++] = ths->Cadr()->eval();
		ths = ths->CddrCONS();
	}
	n=0;
	while (ths->is_a(TYPE_CONS))
	{
		symbol_t *sym = ths->Car()->as<symbol_t>();
		sym->set_value(res[n++]);
		ths = ths->CddrCONS();
	}
	t = res[n-1];
	return t;
}

static node_t *macro_defconstant(cons_t *ths)
{
	symbol_t *name;
	ths->check_num_args(2);
	name = ths->Car()->as<symbol_t>();
	name->set_constant_value(ths->Cadr()->as<symbol_t>()->eval());
	return name;
}

static node_t *macro_defparameter(cons_t *ths)
{
	symbol_t *name;
	ths->check_num_args(2);
	name = ths->Car()->as<symbol_t>();
	name->set_value(ths->Cadr()->as<symbol_t>()->eval());
	return name;
}

static node_t *macro_defvar(cons_t *ths)
{
	node_t *value = NULL;
	symbol_t *name;
	ths->check_range_num_args(1,2);
	name = ths->Car()->as<symbol_t>();
	ths = ths->CdrCONS();
	if (ths->is_a(TYPE_CONS))
		value = ths->Car();
	if (name->get_value() || !value)
		name->declare_special_variable();
	else
		name->set_special_variable(value->eval());
	return name;
}

static node_t *macro_defun(cons_t *ths)
// arg1 is name; arg2 is varlist; arg3 is body
{
	ths->check_range_num_args(3,-1);
	symbol_t *name = ths->Car()->as<symbol_t>();
	cons_t *varlist = ths->CadrCONS();
	if (varlist != (cons_t *)nil && !varlist->is_a(TYPE_CONS))
		throw_eval_exception(BAD_ARG_TYPE);
	int num = varlist->get_num_items();
	name->set_form(
		new usrfunction_t(name->get_name(),
			num,num,varlist,ths->CddrCONS(),g_bind_stack.get_env())
		);
	return name;
}

static node_t *macro_prog(cons_t *ths)  // identical to let except implicit nil block around
{
// (prog ( {var | (var [value])}* ) {form_t}* )

	node_t *retval = nil;
	cons_t *bindings,*body;
	int numvars;
	vector<vardecl> varlist;

	ths->check_range_num_args(1,-1);
	bindings = ths->CarCONS();
	body = ths->CdrCONS();

	numvars = dobindings(varlist,bindings,0);

	try
	{
		while (body->is_a(TYPE_CONS))
		{
			retval = body->Car()->eval();
			body = body->CdrCONS();
		}
	}
	catch(block_return_exception_t* e)
	{
		if (e->_block == nil)
		{
			retval = e->_retval;
			e->Delete();
		}
		else
			throw;
	}
	catch(CException*)
	{
		g_bind_stack.unbind(numvars);
		throw;
	}

	g_bind_stack.unbind(numvars);
	return retval;
}

static node_t *macro_prog_star(cons_t *ths)  // identical to let* except implicit nil block around
{
// (prog* ( {var | (var [value])}* ) {form_t}* )

	node_t *retval = nil;
	cons_t *bindings,*body;
	int numvars;
	vector<vardecl> varlist;

	ths->check_range_num_args(1,-1);
	bindings = ths->CarCONS();
	body = ths->CdrCONS();

	numvars = dobindings(varlist,bindings,0);

	try
	{
		while (body->is_a(TYPE_CONS))
		{
			retval = body->Car()->eval();
			body = body->CdrCONS();
		}
	}
	catch(block_return_exception_t* e)
	{
		if (e->_block == nil)
		{
			retval = e->_retval;
			e->Delete();
		}
		else
			throw;
	}
	catch(CException*)
	{
		g_bind_stack.unbind(numvars);
		throw;
	}

	g_bind_stack.unbind(numvars);
	return retval;
}

static node_t *macro_prog1(cons_t *ths)
{
	if (!ths->is_a(TYPE_CONS))
		return nil;
	node_t *result = ths->Car()->eval();
	ths = ths->CdrCONS();
	while (ths->is_a(TYPE_CONS))
	{
		ths->Car()->eval();
		ths = ths->CdrCONS();
	}
	return result;
}

static node_t *macro_prog2(cons_t *ths)
{
	if (!ths->is_a(TYPE_CONS))
		return nil;
	node_t *result = ths->Car()->eval();
	ths = ths->CdrCONS();
	if (!ths->is_a(TYPE_CONS))
		return result;
	result = ths->Car()->eval();
	while (ths->is_a(TYPE_CONS))
	{
		ths->Car()->eval();
		ths = ths->CdrCONS();
	}
	return result;
}

static node_t *macro_when(cons_t *ths)
{
	ths->check_min_num_args(2);
	node_t *result = ths->Car()->eval();
	if (result == nil)
		return nil;
	ths = ths->CdrCONS();
	while (ths->is_a(TYPE_CONS))
	{
		result = ths->Car()->eval();
		ths = ths->CdrCONS();
	}
	return result;
}

static node_t *macro_unless(cons_t *ths)
{
	ths->check_min_num_args(2);
	node_t *result = ths->Car()->eval();
	if (result != nil)
		return nil;
	ths = ths->CdrCONS();
	while (ths->is_a(TYPE_CONS))
	{
		result = ths->Car()->eval();
		ths = ths->CdrCONS();
	}
	return result;
}

static node_t *macro_cond(cons_t *ths)
{
	while (ths->is_a(TYPE_CONS))
	{
		cons_t *clause = ths->CarCONS();
		if (!clause->is_a(TYPE_CONS)) // make sure clause is a list
			throw_eval_exception(BAD_ARG_TYPE);
		node_t *test = clause->Car()->eval();
		if (test != nil) // evaluate car of clause
		{
			if (clause->Cdr()->is_a(TYPE_CONS))
				return form_progn(ths->CarCONS());
			else
				return test;
		}
		ths = ths->CdrCONS();
	}
	return nil;
}

static node_t *macro_return(cons_t *ths)
{
	throw_block_return_exception(nil,(ths->is_a(TYPE_CONS)) ? ths->Car()->eval() : nil);
	return nil;
}

static node_t *macro_loop(cons_t *ths)
{
	try
	{
		while (1)
			form_progn(ths);
	}
	catch(block_return_exception_t* e)
	{
		if (e->_block == nil )
		{
			return e->_retval;
			e->Delete();
		}
		else
			throw;
	}
	return nil; //should never get here
}


/*
	(do (bindings1 bindings2 ... bindingsn)
		(test result1 result2 ... resultn)
		body
	)
*/

static node_t *macro_do(cons_t *ths)
{
	node_t *test,
		 *retval = nil;
	cons_t *clause,*body,*result,*bindings;
	int numvars;
	int i=0;
	vector<vardecl> varlist;

	ths->check_range_num_args(2,-1);

	bindings = ths->CarCONS();
	numvars = dobindings(varlist,bindings,1);

	clause = ths->CadrCONS(); // (test {result}*)
	clause->check_arg_type(TYPE_CONS);

	test = clause->Car();
	result = clause->CdrCONS();

	body = ths->CddrCONS();
//  body = (body->is_a(TYPE_CONS)) ? body->Car() : nil;


	try
	{
		while (test->eval() == nil)
		{
			cons_t *bodstat = body;
			while (bodstat->is_a(TYPE_CONS))
			{
				bodstat->Car()->eval();
				bodstat = bodstat->CdrCONS();
			}
			for (i=0;i<numvars;i++)
				if (varlist[i].step)
					varlist[i].init = varlist[i].step->eval();
			for (i=0;i<numvars;i++)
				if (varlist[i].step)
					varlist[i].psymbol->set_value(varlist[i].init);
		}
		while (result->is_a(TYPE_CONS))
		{
			retval = result->Car()->eval();
			result = result->CdrCONS();
		}

	}
	catch(block_return_exception_t* e)
	{
		if (e->_block == nil)
		{
			retval = e->_retval;
			e->Delete();
		}
		else
		{
			g_bind_stack.unbind(numvars);
			throw;
		}
	}
	catch(CException*)
	{
		g_bind_stack.unbind(numvars);
		throw;
	}

	g_bind_stack.unbind(numvars);
	return retval;
}

static node_t *macro_do_star(cons_t *ths)
{
	node_t *retval = nil;
	cons_t *clause,*body,*result,*bindings;
	int numvars;
	int i=0;
	vector<vardecl> varlist;

	ths->check_range_num_args(2,-1);
	bindings = ths->CarCONS();
	bindings->check_arg_type(TYPE_CONS);
	numvars = dostarbindings(varlist,bindings,1);

	clause = ths->CadrCONS(); // (test {result}*)
	clause->check_arg_type(TYPE_CONS);

	node_t* test = clause->Car();
	result = clause->CdrCONS();

	body = ths->CddrCONS();
//  body = (body->is_a(TYPE_CONS)) ? body->Car() : nil;

	try
	{
		while (test->eval() == nil)
		{
			cons_t *bodstat = body;
			while (bodstat->is_a(TYPE_CONS))
			{
				bodstat->Car()->eval();
				bodstat = bodstat->CdrCONS();
			}
			for (i=0;i<numvars;i++)
				if (varlist[i].step)
				{
					varlist[i].init = varlist[i].step->eval();
					varlist[i].psymbol->set_value(varlist[i].init);
				}
		}
		while (result->is_a(TYPE_CONS))
		{
			retval = result->Car()->eval();
			result = result->CdrCONS();
		}

	}
	catch(block_return_exception_t* e)
	{
		if (e->_block == nil)
		{
			retval = e->_retval;
			e->Delete();
		}
		else
		{
			g_bind_stack.unbind(numvars);
			throw;
		}
	}
	catch(CException*)
	{
		g_bind_stack.unbind(numvars);
		throw;
	}

	g_bind_stack.unbind(numvars);
	return retval;
}

static node_t *macro_time(cons_t *ths)
{
	using namespace std::chrono;
	ths->check_min_num_args(1);
	auto start = high_resolution_clock::now();
	node_t *result = ths->Car()->eval();
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << duration.count() << " microseconds" << "\n";
	return result;
}

static node_t *form_catch(cons_t *ths)
{
	ths->check_min_num_args(1);
	node_t *tag = ths->Car()->eval();
	node_t *res = nil;
	ths = ths->CdrCONS();
	try
	{
		while (ths->is_a(TYPE_CONS))
		{
			res = ths->Car()->eval();
			ths = ths->CdrCONS();
		}
	}
	catch(other_exception_t* e)
	{
		if (e->_tag == tag)
		{
			res = e->_retval;
			e->Delete();
		}
		else
			throw;
	}
	return res;
}

static node_t *form_throw(cons_t *ths)
{
	ths->check_num_args(2);
	node_t *tag = ths->Car()->eval();
	node_t *res = ths->CadrCONS()->eval();
	throw_other_exception(tag,res);
	return nil;
}
