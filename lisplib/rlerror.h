#pragma once
#ifndef __RLERROR_H
#define __RLERROR_H

#include <sstream>
#include "rexcept.h"
#include "errors.h"

struct file_position_t
{
	const char* filename;
	int line;
};

std::string get_rlerror_msg(const block_return_exception_t& e, const file_position_t& position);
std::string get_rlerror_msg(const read_exception_t& e, const file_position_t& position);
std::string get_rlerror_msg(const lisp_exception_t& e, const file_position_t& position);
std::string get_rlerror_msg(const eval_exception_t& e, const file_position_t& position);

template <typename T>
void write_error(const T& e, const file_position_t& position)
{
	std::cerr << get_rlerror_msg(e, position) << std::endl;
}

#endif
