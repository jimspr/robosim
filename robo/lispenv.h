#pragma once
#ifndef __LISPENV_H
#define __LISPENV_H

#include <iostream>
#include "readtabl.h"
#include "node.h"

class lisp_env_t
{
	bool _is_top;
	std::istream& _input;
	std::ostream& _output;
public:
	read_table_t _readtable;
	const char* _file_name = nullptr; /* null if console */
	node_t* top = nullptr;
	node_t* res = nullptr;
	node_t* _exit_status = nullptr;
	lisp_env_t(const char* filename, std::istream& in, std::ostream& out);
	lisp_env_t(std::istream& in, std::ostream& out);
	void reploop(void);
	int read(void);
	int eval(void);
	void print(void);
};

extern lisp_env_t lisp_env;
#endif
