#include "stdafx.h"
#include <strstream>
#include "rlerror.h"
#include "node.h"

using namespace std;

#define DAT(x, y) y,
static char* errors[] = {
	"",
#include "errordat.h"
};
#undef DAT

char* get_rlerror_msg(error_e type)
{
	if (type && (type < NUM_ERRMSGS))
		return errors[type];
	else
		return errors[NUM_ERRMSGS];
}

std::string get_rlerror_msg(LPCSTR pszFileName, robosim_exception_t& e)
{
	ostrstream ostr;
	int i = 0, j;
	for (auto& name : e._function_names)
	{
		for (j = 0; j < i; j++)
			ostr << "  ";
		ostr << '(' << name << " ... \n";
		++i;
	}
	for (j = 0; j < i; j++)
		ostr << ')';
	if (i != 0)
		ostr << '\n';
	ostr << get_rlerror_msg(e._error_code);
	if (e._evalnode != nullptr)
	{
		ostr << " : ";
		e._evalnode->print(ostr);
	}
	if (pszFileName != nullptr)
		ostr << "\nwhile loading " << pszFileName << '(' << e._line_number << ")\n";
	ostr << (char)0;
	return ostr.str();
}

std::string get_rlerror_msg(robosim_exception_t& e)
{
	return get_rlerror_msg(nullptr, e);
}

void rlerror(LPCSTR fname, robosim_exception_t& e)
{
	cerr << get_rlerror_msg(fname, e) << '\n';
	cerr.flush();
}
