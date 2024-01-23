#include "pch.h"

#include "rexcept.h"

int base_exception_t::_count = 0;

void throw_done_exception()
{
	static done_exception_t simpleDoneException(false);
	throw &simpleDoneException;
}

void throw_interrupt_exception()
{
	throw new interrupt_exception_t();
}

void throw_read_exception(error_e err)
{
	read_exception_t *pReadException = new read_exception_t(err);
	throw pReadException;
}

void throw_eval_exception(const node_t *n,error_e err)
{
	eval_exception_t *pEvalException = new eval_exception_t(n, err);
	throw pEvalException;
}

void throw_eval_exception(error_e err)
{
	eval_exception_t *pEvalException = new eval_exception_t(NULL, err);
	throw pEvalException;
}

void throw_block_return_exception(const symbol_t *b,node_t *val)
{
	block_return_exception_t *pBRException = new block_return_exception_t(b,val);
	throw pBRException;
}

void throw_other_exception(const node_t *b,node_t *val)
{
	other_exception_t *pCTException = new other_exception_t(b,val);
	throw pCTException;
}
