#include "stdafx.h"
#include "package.h"
#include "node.h"

void package_t::mark_in_use(void)
{
//	if (is_in_use())
//		return;
//	node_t::mark_in_use();
	for (auto& entry : _map)
	{
		auto psym = entry.second;
		if (!psym)
			throw_eval_exception(FATAL_ERROR);
		psym->mark_in_use();
	}
}

symbol_t *package_t::get_symbol(const char *name)
{
	symbol_t* psym;
	auto iter = _map.find(name);
	if (iter == _map.end())
	{
		psym = new symbol_t(name);
		_map[name] = psym;
		if (name[0] == ':') // keyword symbol
			psym->set_constant_value(psym);
	}
	else
		psym = iter->second;
	return psym;
}

void package_t::add_symbol(const char *name,symbol_t *psym)
{
	_map[name] = psym;
}
