#pragma once
#ifndef __RLERROR_H
#define __RLERROR_H

#include <sstream>
#include "rexcept.h"
#include "errors.h"

//std::string get_rlerror_msg(const robosim_exception_t& e, const char* filename = nullptr);
std::string get_rlerror_msg(const block_return_exception_t& e, const char* filename = nullptr);
std::string get_rlerror_msg(const read_exception_t& e, const char* filename = nullptr);
std::string get_rlerror_msg(const lisp_exception_t& e, const char* filename = nullptr);
std::string get_rlerror_msg(const eval_exception_t& e, const char* filename = nullptr);

template <typename T>
void write_error(const T& e, const char* filename = nullptr)
{
	std::cerr << get_rlerror_msg(e, filename) << std::endl;
}

#endif
