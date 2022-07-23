#include "stdafx.h"
#include "parsen.h"
#include "node.h"
#include "readtabl.h"

#define DIGIT 1
#define EXPONENT_MARKER 2

int parse_number_t::numtype[256];

void parse_number_t::initialize()
{
	int i;
	for (i = 0; i < 256; i++)
	{
		parse_number_t::numtype[i] = ILLEGAL;
	}

	static char* DIG = "0123456789";
	static char* EXPMARK = "esfdlESFDL";
	int nLen = lstrlen(DIG);
	for (i = 0; i < nLen; i++)
		parse_number_t::numtype[DIG[i]] = DIGIT;
	nLen = lstrlen(EXPMARK);
	for (i = 0; i < nLen; i++)
		parse_number_t::numtype[EXPMARK[i]] = EXPONENT_MARKER;
}

parse_number_t::parse_number_t(const char *pBUF)
{
	_current = pBUF;
	_nsign = _expsign = 1;
	_mancnt = 0;
	_abscissa = _mantissa = _expval = 0L;
	_node = (node_t *)NULL;
}

int parse_number_t::parse()
{
	sign(_nsign);
	if (*_current == '.')
	{
		_current++;
		return flo1();
	}
	else
		return n1();
}

int parse_number_t::n1()
{
	if (!digits(_abscissa,1))
		return 0;
	if (*_current == (char)0)
		return makenumber(TYPE_LONG);
	if (exponent())
	{
		if (*_current)
			return 0;
		return makenumber(TYPE_FLOAT);
	}
	if (*_current == '.')
	{
		_current++;
		if (*_current == (char)0)
			return makenumber(TYPE_LONG);
		if (!afterdigits(1))
			return 0;
		exponent();
		if (*_current)
			return 0;
		return makenumber(TYPE_FLOAT);
	}
	return 0;

}

int parse_number_t::flo1()
{
	if (!afterdigits(1))
		return 0;
	exponent();
	if (*_current) // if not at end of string
		return 0;
	return makenumber(TYPE_FLOAT);
}

void parse_number_t::sign(int &result)
{
	result = 1;
	if (*_current == '-')
	{
		_current++;
		result = -1;
	}
	else if (*_current == '+')
		_current++;
	return;
}

int parse_number_t::digits(long &result,int min)
{
	while (numtype[*_current]==DIGIT)
	{
		min--;
		result = result * 10 + (int) (*_current) - (int) '0';
		_current++;
	}
	if (min <= 0)
		return 1;
	return 0;
}

int parse_number_t::afterdigits(int min)
{
	while (numtype[*_current]==DIGIT)
	{
		_mancnt++;
		_mantissa = _mantissa * 10 + (int) (*_current) - (int) '0';
		_current++;
	}
	if (_mancnt < min)
		return 0;
	return 1;
}

int parse_number_t::exponent()
{
	if (!exponent_marker())
		return 0;
	sign(_expsign);
	if (!digits(_expval,1))
		return 0;
	return 1;
}

int parse_number_t::exponent_marker()
{
	if (numtype[*_current] == EXPONENT_MARKER)
	{
		_current++;
		return 1;
	}
	else
		return 0;
}

int parse_number_t::makenumber(int type)
{
	switch(type)
	{
		case TYPE_LONG:
			_node = (node_t *)(new number_node_t((long)_nsign * _abscissa));
			break;
		case TYPE_FLOAT:
			double tmp = (double)_mantissa;
			for (int i=0;i<_mancnt;i++)
				tmp /= 10.;
			tmp += (long)_abscissa;
			tmp *= (long)_nsign;
			tmp *= pow(10.,(double)_expsign*(double)_expval);
			_node = (node_t *)(new number_node_t((float)tmp));
			break;
	}
	return 1;
}
