#pragma once
#ifndef __CONSTANT_H
#define __CONSTANT_H

#include <limits.h>
#include <float.h>

#define LC_ARRAY_DIMENSION_LIMIT			0x7fffffffL
#define LC_ARRAY_RANK_LIMIT				0x7fffL
#define LC_ARRAY_TOTAL_SIZE_LIMIT			0x7fffffffL
#define LC_BOOLE_1					0L
#define LC_BOOLE_2					1L
#define LC_BOOLE_AND					2L
#define LC_BOOLE_ANDC1					3L
#define LC_BOOLE_ANDC2					4L
#define LC_BOOLE_C1					5L
#define LC_BOOLE_C2					6L
#define LC_BOOLE_CLR					7L
#define LC_BOOLE_EQV					8L
#define LC_BOOLE_IOR					9L
#define LC_BOOLE_NAND					10L
#define LC_BOOLE_NOR					11L
#define LC_BOOLE_ORC1					12L
#define LC_BOOLE_ORC2					13L
#define LC_BOOLE_SET					14L
#define LC_BOOLE_XOR					15L
#define LC_CALL_ARGUMENTS_LIMIT				50L
#define LC_CHAR_CODE_LIMIT				255L
#define LC_DOUBLE_FLOAT_EPSILON				((float)FLT_EPSILON)
#define LC_DOUBLE_FLOAT_NEGATIVE_EPSILON		(-(float)FLT_EPSILON)
// #define LC_INTERNAL_TIME_UNITS_PER_SECOND
// #define LC_LAMBDA_LIST_KEYWORDS
#define LC_LAMBDA_PARAMETERS_LIMIT			50L
#define LC_LEAST_NEGATIVE_DOUBLE_FLOAT			(-(float)FLT_MIN)
#define LC_LEAST_NEGATIVE_LONG_FLOAT			(-(float)FLT_MIN)
#define LC_LEAST_NEGATIVE_NORMALIZED_DOUBLE_FLOAT	(-(float)FLT_MIN)
#define LC_LEAST_NEGATIVE_NORMALIZED_LONG_FLOAT		(-(float)FLT_MIN)
#define LC_LEAST_NEGATIVE_NORMALIZED_SHORT_FLOAT	(-(float)FLT_MIN)
#define LC_LEAST_NEGATIVE_NORMALIZED_SINGLE_FLOAT	(-(float)FLT_MIN)
#define LC_LEAST_NEGATIVE_SHORT_FLOAT			(-(float)FLT_MIN)
#define LC_LEAST_NEGATIVE_SINGLE_FLOAT			(-(float)FLT_MIN)
#define LC_LEAST_POSITIVE_DOUBLE_FLOAT			((float)FLT_MIN)
#define LC_LEAST_POSITIVE_LONG_FLOAT			((float)FLT_MIN)
#define LC_LEAST_POSITIVE_NORMALIZED_DOUBLE_FLOAT	((float)FLT_MIN)
#define LC_LEAST_POSITIVE_NORMALIZED_LONG_FLOAT		((float)FLT_MIN)
#define LC_LEAST_POSITIVE_NORMALIZED_SHORT_FLOAT	((float)FLT_MIN)
#define LC_LEAST_POSITIVE_NORMALIZED_SINGLE_FLOAT	((float)FLT_MIN)
#define LC_LEAST_POSITIVE_SHORT_FLOAT			((float)FLT_MIN)
#define LC_LEAST_POSITIVE_SINGLE_FLOAT			((float)FLT_MIN)
#define LC_LONG_FLOAT_EPSILON				((float)FLT_EPSILON)
#define LC_LONG_FLOAT_NEGATIVE_EPSILON			(-(float)FLT_EPSILON)
#define LC_MOST_NEGATIVE_DOUBLE_FLOAT			(-(float)FLT_MAX)
#define LC_MOST_NEGATIVE_FIXNUM				((long)LONG_MIN)
#define LC_MOST_NEGATIVE_LONG_FLOAT			(-(float)FLT_MAX)
#define LC_MOST_NEGATIVE_SHORT_FLOAT			(-(float)FLT_MAX)
#define LC_MOST_NEGATIVE_SINGLE_FLOAT			(-(float)FLT_MAX)
#define LC_MOST_POSITIVE_DOUBLE_FLOAT			((float)FLT_MAX)
#define LC_MOST_POSITIVE_FIXNUM				((long)LONG_MAX)
#define LC_MOST_POSITIVE_LONG_FLOAT			((float)FLT_MAX)
#define LC_MOST_POSITIVE_SHORT_FLOAT			((float)FLT_MAX)
#define LC_MOST_POSITIVE_SINGLE_FLOAT			((float)FLT_MAX)
#define LC_MULTIPLE_VALUES_LIMIT			20L
#define LC_PI						((float)acos(-1.0))
#define LC_SHORT_FLOAT_EPSILON				((float)FLT_EPSILON)
#define LC_SHORT_FLOAT_NEGATIVE_EPSILON			(-(float)FLT_EPSILON)
#define LC_SINGLE_FLOAT_EPSILON				((float)FLT_EPSILON)
#define LC_SINGLE_FLOAT_NEGATIVE_EPSILON		(-(float)FLT_EPSILON)

#endif
