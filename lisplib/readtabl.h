#pragma once
#ifndef __READTABL_H
#define __READTABL_H
#include <string>
#include <iostream>

#define ILLEGAL 0
#define WHITESPACE 1
#define CONSTITUENT 2
#define TERM_MACRO 3
#define NONTERM_MACRO 4
#define SINGLE_ESCAPE 5
#define MULT_ESCAPE 6
#define PACKAGE_MARKER 7
#define CHAR_DOT 8

#define MAX_STRING_LENGTH 1000
#define MAX_CONSTITUENT_LENGTH 1000

class node_t;

class read_table_t
{
public:
	static int _paren_level;
	std::string _buffer;
	int _type[256]{};
	int _current;
	char _me; /* mult-escape */
	bool _in_escape;
	int _line_cnt;


	read_table_t();
	int getnextchar(std::istream& istr)
	{
		if (istr.peek() == '\n')
			_line_cnt++;
		return istr.get();
	}
	void putback(std::istream& istr, char ch)
	{
		if (ch == '\n')
			_line_cnt--;
		istr.putback(ch);
	}
	node_t *read(std::istream &,int);
	node_t *parse_constituent(std::istream &istr);
	node_t *parse_dot(std::istream &istr);
	node_t *parse_lparen(std::istream &istr);
	node_t *parse_rparen(std::istream &istr);
	node_t *parse_quote(std::istream &istr);
	node_t *parse_function(std::istream &istr);
	node_t *parse_string(std::istream &istr);
	node_t *parse_comment(std::istream &istr);
	node_t *parse_tmacro(std::istream &istr);
	node_t *parse_ntmacro(std::istream &istr);
	node_t *parse_vector(std::istream &istr);
	node_t *makecell(const char *buf);
	void consume(std::istream &istr);
};

#endif
