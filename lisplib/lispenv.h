#pragma once
#ifndef __LISPENV_H
#define __LISPENV_H

#include <iostream>
#include "readtabl.h"

class node_t;

/* There is one top-level lisp_env_t. When function "load" is called, a new environment is created to process that file. */
class lisp_env_t
{
	bool _is_top; /* Set to true if top level. */
	std::istream& _input;
	std::ostream& _output;
public:
	read_table_t _readtable;
	const char* _file_name = nullptr; /* null if console */
	node_t* _top = nullptr;
	node_t* _result = nullptr;
	node_t* _exit_status = nullptr;
	lisp_env_t(const char* filename, std::istream& in, std::ostream& out);
	lisp_env_t(std::istream& in, std::ostream& out);
	void reploop(void);
	int read(void);
	int eval(void);
	void print(void);
};

#endif
