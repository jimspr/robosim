#pragma once
#ifndef __PACKAGE_H
#define __PACKAGE_H

#include <string>
#include <unordered_map>
#include "node.h"

class package_t
{
	std::unordered_map<std::string, symbol_t*> _map;
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

extern package_t* current_package;

#endif
