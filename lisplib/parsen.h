#pragma once
#ifndef __PARSEN_H
#define __PARSEN_H
#include "node.h"

class parse_number_t
{
private:
	const char *_current;
	int _mancnt;
	int _nsign;
	int _expsign;
	long _abscissa;
	long _mantissa;
	long _expval;
public:
	node_t *_node;
	parse_number_t(const char *pBUF);
	int parse();
	int n1();
	int flo1();
	void sign(int &result);
	int digits(long &result,int min);
	int afterdigits(int min);
	int exponent();
	int exponent_marker();
	int makenumber(int type);

	static int numtype[256];
	static void initialize();
};

#endif
