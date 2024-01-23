#include "pch.h"
#include "lispenv.h"
#include "host.h"
#include "node.h"

using namespace std;

lisp_env_t::lisp_env_t(const char* filename, std::istream& in, std::ostream& out) : _is_top(false), _input(in), _output(out), _file_name(filename)
{
}

lisp_env_t::lisp_env_t(std::istream& in, std::ostream& out) : _is_top(true), _input(in), _output(out)
{
}

void lisp_env_t::reploop(void)
{
	_readtable._line_cnt = 1;
	while (!_exit_status)
	{
		if (_is_top)
			_output << "->";
		if (read() && eval() && _is_top)
			print();
	}
}

int lisp_env_t::read(void)
{
	try
	{
		_top = _readtable.read(_input,1);
		if (!_top) // eof
		{
			_exit_status = pTrue;
			return 0;
		}
	}
	catch(read_exception_t* e)
	{
		e->_line_number = _readtable._line_cnt;
		rlerror(_file_name, *e);
		_top = nullptr;
		if (!_is_top)
			_exit_status = nil;
		e->Delete();
		return 0;
	}
	pPLUS3->set_value(pPLUS2->get_value());
	pPLUS2->set_value(pPLUS1->get_value());
	pPLUS1->set_value(pMINUS->get_value());
	pMINUS->set_value(_top);
	return 1;
}

int lisp_env_t::eval(void)
{
	busy_t t;
	try
	{
		_result = _top->eval();
	}
	catch(eval_exception_t* e)
	{
		e->_line_number = _readtable._line_cnt;
		rlerror(_file_name, *e);
		_result = nullptr;
		if (!_is_top)
			_exit_status = nil;
		e->Delete();
		return 0;
	}
	catch(block_return_exception_t* e)
	{
		robosim_exception_t re(UNKNOWN_BLOCK_NAME, _readtable._line_cnt);
		rlerror(_file_name, re);
		_result = nullptr;
		if (!_is_top)
			_exit_status = nil;
		e->Delete();
		return 0;
	}
	catch(interrupt_exception_t* e)
	{
		_result = nullptr;
		if (!_is_top)
			_exit_status = nil;
		e->Delete();
		return 0;
	}
	catch(other_exception_t* e)
	{
		robosim_exception_t re(e->_tag, UNCAUGHT_EXCEPTION, _readtable._line_cnt);
		_result = nullptr;
		rlerror(_file_name, re);
		if (!_is_top)
			_exit_status = nil;
		e->Delete();
		return 0;
	}
	return 1;
}

void lisp_env_t::print(void)
{
	if (_result)
	{
		pSTAR3->set_value(pSTAR2->get_value());
		pSTAR2->set_value(pSTAR1->get_value());
		pSTAR1->set_value(_result);
		pDIV3->set_value(pDIV2->get_value());
		pDIV2->set_value(pDIV1->get_value());
		pDIV1->set_value(_result);
		_result->print(cout);
	}
	cout << "\n";
}
