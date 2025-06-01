// lisp.cpp : This is a simple command line lisp interpreter. It uses the lisplib project.
//
#include <pch.h>

#include <iostream>

#include <node.h>
#include <lisp_engine.h>

extern void init_forms(void);
extern void init_funcs(void);
//extern void init_lfuncs(void);
extern void init_constants(void);
extern void init_variables(void);

lisp_engine_t lisp_engine;

// construct host

// set lisp_host to &host
// create package
// 	init_forms();
// init_funcs();
// init_lfuncs();
// init_constants();
// init_variables();


// Implement callbacks for hosting the Lisp system.
struct console_host : public lisp_host_t
{
	void periodic_notify() final
	{
		// Nothing to do
	}
	void mark_in_use() final
	{
		// No Lisp objects held
	}
	void add_busy() final
	{
		// Nothing to do
	}
	void release_busy() final
	{
		// Nothing to do
	}
	void exit() final
	{
		// Nothing to do
	}
};

console_host host;

int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";
	lisp_engine._host = &host;

	init_forms();
	init_funcs();
	//init_lfuncs();
	init_constants();
	init_variables();

	for (int i = 1; i < argc; ++i)
	{
		node_t* l = cons_t::make_list(lisp_engine._package.get_symbol("LOAD"), new string_node_t(argv[i]));
		try
		{
			auto node = l->eval();
			node->princ(std::cout);
			std::cout << std::endl;
		}
		catch (eval_exception_t& e)
		{
			file_position_t position{ argv[i], lisp_engine._env._readtable._line_cnt };
			std::cout << get_rlerror_msg(e, position) << std::endl;
		}
	}

	lisp_engine._env.reploop();
	return 0;
}
