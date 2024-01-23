#pragma once
#ifndef __RLERROR_H
#define __RLERROR_H

#include "rexcept.h"
#include "errors.h"

void rlerror(const char* fname, robosim_exception_t& e);
const char *get_rlerror_msg(error_e type);
std::string get_rlerror_msg(const char* pszFileName, robosim_exception_t& e);
std::string get_rlerror_msg(robosim_exception_t& e);

#endif
