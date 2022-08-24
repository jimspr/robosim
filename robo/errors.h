#pragma once

#define DAT(x, y) x,
enum error_e
{
	error_none = 0,
#include "errordat.h"
};
#undef DAT
