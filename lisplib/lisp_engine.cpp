#include "pch.h"

#include "ik.h"
#include "lisp_engine.h"
#include "node.h"

void lisp_engine_t::post_garbage_collect()
{
	if (_garbage_collected)
	{
		_node_lists.clear_all_gc_flags();
		_garbage_collected = false;
	}
}

int lisp_engine_t::garbage_collect()
{
	int cnt = 0;
	_garbage_collected = true;

	//MessageBeep(MB_ICONASTERISK);
	
	/* Traverse all roots and mark as in use. */
	_frame_stack.mark_in_use();
	_bind_stack.mark_in_use();
	dot_node->mark_in_use();
	nil->mark_in_use();
	pTrue->mark_in_use();
	_package.mark_in_use();
	_host->mark_in_use();
	for (auto& ik : _invkin)
		ik->mark_in_use();

	return _node_lists.delete_unused();
}

busy_t::busy_t()
{
	lisp_engine._host->add_busy();
}

busy_t::~busy_t()
{
	lisp_engine._host->release_busy();
}

node_list_base_t::node_list_base_t()
{
	lisp_engine._node_lists.add(this);
}

void node_list_base_t::garbage_collect()
{
	lisp_engine.garbage_collect();
}
