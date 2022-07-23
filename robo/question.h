#pragma once

#include <string>
class node_t;

enum class question_type_e
{
	question_float,
	question_long,
	question_string,
	question_symbol
};

class question
{
public:
	std::string question_text;

	question_type_e type;
	//	union {
	std::string cresult;
	long lresult = 0;
	float fresult = 0.f;
	//	};
	float incr = 1.f;
	node_t* get_node();
};

