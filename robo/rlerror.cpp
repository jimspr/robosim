#include "stdafx.h"
#include <strstream>
#include "rlerror.h"
#include "node.h"

using namespace std;

static char *errors[NUM_ERRMSGS] = {
	"Illegal character",
	"Readtable is corrupt",
	"Unexpected right parenthesis",
	"Unexpected end of file",
	"Unsupported macro character",
	"Invalid dotted pair notation",
	"String too long",
	"Identifier too long",
	"Bad Function",
	"Too few arguments",
	"Too many arguments",
	"Bad argument type",
	"Unbound function",
	"Unbound variable",
	"Divide by zero",
	"Out of memory",
	"Unknown node type",
	"Math Error",
	"Unknown Block Name in return",
	"Unknown Error",
	"End of File in console",
	"Fatal Error",
	"File not found",
	"Stack Overflow",
	"Illegal character following non-terminating macro",
	"Index on array or vector out of bounds",
	"Invalid dotted pair notation",
	"Redefinition of constant not allowed",
	"Redefinition of special form not allowed",
	"Invalid Symbol",
	"Invalid Package Name",
	"Not a function",
	"Stack corrupt",
	"Argument should be greater than zero",
	"A point should be a vector of three numbers",
	"Graphic primitive must be composed of at least one point",
	"Argument not in valid range",
	"Stream is invalid",
	"Invalid keyword in function",
	"Object not in environment",
	"Cannot use an agent in creating a link",
	"Must be a serial agent",
	"No inverse kinematics routine available",
	"Inverse kinematics library not valid",
	"Exception not caught"
	};

char *get_rlerror_msg(int type)
{
	if (type && (type <= NUM_ERRMSGS))
		return errors[type-1];
	else
		return "System Error: Unknown error number";
}

std::string get_rlerror_msg(LPCSTR pszFileName, robosim_exception_t& e)
{
	ostrstream ostr;
	int i=0,j;
	for (auto& name : e._function_names)
	{
		for (j = 0; j < i; j++)
			ostr << "  ";
		ostr << '(' << name << " ... \n";
		++i;
	}
	for (j=0;j<i;j++)
		ostr << ')';
	if (i != 0)
		ostr << '\n';
	ostr << get_rlerror_msg(e._error_code);
	if (e._evalnode != NULL)
	{
		ostr << " : ";
		e._evalnode->print(ostr);
	}
	if (pszFileName != NULL)
		ostr << "\nwhile loading " << pszFileName << '(' << e._line_number << ")\n";
	ostr << (char)0;
	return ostr.str();
}

void rlerror(LPCSTR fname, robosim_exception_t& e)
{
	cerr << get_rlerror_msg(fname, e) << '\n';
	cerr.flush();
}

void rlerror(const char *msg)
{
	cerr << msg;
	cerr.flush();
}

void rlmessage(const char *msg)
{
	cerr << msg;
	cerr.flush();
}
