#pragma once
#ifndef __RLERROR_H
#define __RLERROR_H

#include "rexcept.h"

#define ILLEGAL_CHAR 1
#define READTABLE_CORRUPT 2
#define UNEXPECTED_RPAREN 3
#define UNEXPECTED_EOF 4
#define INVALID_TMACRO 5
#define INVALID_DOTTED_PAIR 6
#define STRING_TOO_LONG 7
#define CONSTITUENT_TOO_LONG 8
#define BAD_FUNCTION 9
#define TOO_FEW_ARGS 10
#define TOO_MANY_ARGS 11
#define BAD_ARG_TYPE 12
#define UNBOUND_FUNCTION 13
#define UNBOUND_VARIABLE 14
#define DIVIDE_BY_ZERO 15
#define OUT_OF_MEMORY 16
#define UNKNOWN_NODE_TYPE 17
#define MATH_ERROR 18
#define UNKNOWN_BLOCK_NAME 19
#define UNKNOWN_ERROR 20
#define EOF_IN_CONSOLE 21
#define FATAL_ERROR 22
#define FILE_NOT_FOUND 23
#define STACK_OVERFLOW 24
#define ILLEGAL_NT_MACRO 25
#define INDEX_OUT_OF_BOUNDS 26
#define DOTTED_PAIR 27
#define REDEF_CONSTANT 28
#define REDEF_FORM 29
#define INVALID_SYMBOL 30
#define INVALID_PACKAGE_NAME 31
#define NOT_A_FUNCTION 32
#define STACK_CORRUPT 33
#define ARG_SHOULD_BE_GREATER_THAN_ZERO 34
#define VECTOR_NOT_POINT 35
#define INVALID_GRAPHIC_PRIMITIVE 36
#define DOMAIN_ERROR 37
#define INVALID_STREAM 38
#define BAD_KEYWORD 39
#define OBJECT_NOT_IN_ENV 40
#define NO_COMPLEX_IN_LINK 41
#define NOT_SERIAL_AGENT 42
#define NO_IK_AVAIL 43
#define IKLIB_NOT_VALID 44
#define UNCAUGHT_EXCEPTION 45
#define NUM_ERRMSGS 45

void rlerror(LPCSTR fname, robosim_exception_t& e);

void rlerror(const char *msg);
void rlmessage(const char *msg);
char *get_rlerror_msg(int type);
std::string get_rlerror_msg(LPCSTR pszFileName, robosim_exception_t& e);

#endif
