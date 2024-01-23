#pragma once
#ifndef __PACKAGE_H
#define __PACKAGE_H

#include <string>
#include <unordered_map>

class symbol_t;

class package_t
{
	std::unordered_map<std::string, symbol_t*> _symbols;
	std::string _name;
	std::string _nickname;
public:
	package_t(const char* n, const char* nk) : _name(n), _nickname(nk)
	{
	}

	void mark_in_use(void);
	symbol_t *get_symbol(const char *name);
	void add_symbol(const char *name,symbol_t *psym);
};

#endif
