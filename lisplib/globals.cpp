#include <pch.h>

#include "node.h"
#include "matrix.h"

symbol_t::node_list_type symbol_t::list("symbol_t");
cons_t::node_list_type cons_t::list("cons_t");
number_node_t::node_list_type number_node_t::list("number_node_t");
vector_t::node_list_type vector_t::list("vector_t");
string_node_t::node_list_type string_node_t::list("string_node_t");
stream_node_t::node_list_type stream_node_t::list("stream_node_t");
bound_symbol_t::node_list_type bound_symbol_t::list("bound_symbol_t");
special_form_t::node_list_type special_form_t::list("special_form_t");
macro_t::node_list_type macro_t::list("macro_t");
usrfunction_t::node_list_type usrfunction_t::list("usrfunction_t");
sysfunction_t::node_list_type sysfunction_t::list("sysfunction_t");
mat44::node_list_type mat44::list("mat44_t");
