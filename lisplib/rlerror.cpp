#include "pch.h"

#include <sstream>
#include "rlerror.h"
#include "node.h"

using namespace std;

#define DAT(x, y) y,
static const char* errors[] = {
	"",
#include "errordat.h"
};
#undef DAT

static const char* get_error_description(error_e type)
{
	if (type && (type < NUM_ERRMSGS))
		return errors[type];
	else
		return errors[NUM_ERRMSGS];
}
static void get_exception_msg(ostringstream& ostr, const std::vector<std::string>& names)
{
	int i = 0, j;
	for (auto& name : names)
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
}

static void get_exception_msg(ostringstream& ostr, error_e error_code)
{
	ostr << get_error_description(error_code);
}

static void get_exception_msg(ostringstream& ostr, const node_t* evalnode)
{
	if (evalnode != nullptr)
	{
		ostr << " : ";
		evalnode->print(ostr);
	}
}

static void get_exception_msg(ostringstream& ostr, const file_position_t& position)
{
	if (position.filename != nullptr)
		ostr << "\nwhile loading " << position.filename << '(' << position.line << ")\n";
}

std::string get_rlerror_msg(const block_return_exception_t& e, const file_position_t& position)
{
	ostringstream ostr;
	get_exception_msg(ostr, e._function_names);
	get_exception_msg(ostr, UNKNOWN_BLOCK_NAME);
	get_exception_msg(ostr, position);
	return ostr.str();
}

std::string get_rlerror_msg(const lisp_exception_t& e, const file_position_t& position)
{
	ostringstream ostr;
	get_exception_msg(ostr, e._function_names);
	get_exception_msg(ostr, UNCAUGHT_EXCEPTION);
	get_exception_msg(ostr, position);
	return ostr.str();
}

std::string get_rlerror_msg(const read_exception_t& e, const file_position_t& position)
{
	ostringstream ostr;
	get_exception_msg(ostr, e._function_names);
	get_exception_msg(ostr, e._error_code);
	get_exception_msg(ostr, position);
	return ostr.str();
}

std::string get_rlerror_msg(const eval_exception_t& e, const file_position_t& position)
{
	ostringstream ostr;
	get_exception_msg(ostr, e._function_names);
	get_exception_msg(ostr, e._error_code);
	get_exception_msg(ostr, e._evalnode);
	get_exception_msg(ostr, position);
	return ostr.str();
}
