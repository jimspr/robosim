#include "stdafx.h"
#include "rexcept.h"

IMPLEMENT_DYNAMIC(robosim_exception_t, CException)
IMPLEMENT_DYNAMIC(done_exception_t, robosim_exception_t)
IMPLEMENT_DYNAMIC(read_exception_t, robosim_exception_t)
IMPLEMENT_DYNAMIC(eval_exception_t, robosim_exception_t)
IMPLEMENT_DYNAMIC(block_return_exception_t, robosim_exception_t)
IMPLEMENT_DYNAMIC(interrupt_exception_t, robosim_exception_t)
IMPLEMENT_DYNAMIC(other_exception_t,robosim_exception_t)

void throw_done_exception()
{
	static done_exception_t simpleDoneException(FALSE);
	throw &simpleDoneException;
}

void throw_interrupt_exception()
{
	throw new interrupt_exception_t(TRUE);
}

void throw_read_exception(int err)
{
	read_exception_t *pReadException = new read_exception_t(err);
	throw pReadException;
}

void throw_eval_exception(const node_t *n,int err)
{
	eval_exception_t *pEvalException = new eval_exception_t(n, err);
	throw pEvalException;
}

void throw_eval_exception(int err)
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
