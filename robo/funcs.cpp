#include "stdafx.h"
#include <errno.h>
#include "node.h"
#include "package.h"
#include "argstack.h"
#include "lispenv.h"

using namespace std;

static node_t *func_multiply(int numargs,number_node_t **args);
static node_t *func_add(int numargs,number_node_t **args);
static node_t *func_subtract(int numargs,number_node_t **args);
static node_t *func_divide(int numargs,number_node_t **args);
static node_t *func_1plus(int numargs,number_node_t **args);
static node_t *func_1minus(int numargs,number_node_t **args);
static node_t *func_eqeq(int numargs,number_node_t **args);
static node_t *func_neq(int numargs,number_node_t **args);
static node_t *func_lt(int numargs,number_node_t **args);
static node_t *func_gt(int numargs,number_node_t **args);
static node_t *func_lte(int numargs,number_node_t **args);
static node_t *func_gte(int numargs,number_node_t **args);

//static node_t *func_abort(int numargs,node_t **args);
static node_t *func_abs(int numargs,node_t **args);
//static node_t *func_acons(int numargs,node_t **args);
static node_t *func_acos(int numargs,node_t **args);
//static node_t *func_acosh(int numargs,node_t **args);
//static node_t *func_adjoin(int numargs,node_t **args);
//static node_t *func_adjust__array(int numargs,node_t **args);
//static node_t *func_adjustable__array__p(int numargs,node_t **args);
//static node_t *func_alpha__char__p(int numargs,node_t **args);
//static node_t *func_alphanumericp(int numargs,node_t **args);
//static node_t *func_alter(int numargs,node_t **args);
static node_t *func_append(int numargs,node_t **args);
static node_t *func_apply(int numargs,node_t **args);
//static node_t *func_applyhook(int numargs,node_t **args);
//static node_t *func_apropos(int numargs,node_t **args);
//static node_t *func_apropos__list(int numargs,node_t **args);
static node_t *func_aref(int numargs,node_t **args);
//static node_t *func_arithmetic__error__operands(int numargs,node_t **args);
//static node_t *func_arithmetic__error__operation(int numargs,node_t **args);
//static node_t *func_array__dimension(int numargs,node_t **args);
//static node_t *func_array__dimensions(int numargs,node_t **args);
//static node_t *func_array__element__type(int numargs,node_t **args);
//static node_t *func_array__has__fill__pointer__p(int numargs,node_t **args);
//static node_t *func_array__in__bounds(int numargs,node_t **args);
//static node_t *func_array__rank(int numargs,node_t **args);
//
//static node_t *func_array__row__major__index(int numargs,node_t **args);
//static node_t *func_array__total__size(int numargs,node_t **args);
static node_t *func_arrayp(int numargs,node_t **args);
//static node_t *func_ash(int numargs,node_t **args);
static node_t *func_asin(int numargs,node_t **args);
////static node_t *func_asinh(int numargs,node_t **args);
//static node_t *func_assoc(int numargs,node_t **args);
//static node_t *func_assoc__if(int numargs,node_t **args);
//static node_t *func_assoc__if__not(int numargs,node_t **args);
static node_t *func_atan(int numargs,node_t **args);
////static node_t *func_atanh(int numargs,node_t **args);
static node_t *func_atom(int numargs,node_t **args);
//static node_t *func_augment__environment(int numargs,node_t **args);
//static node_t *func_bit(int numargs,node_t **args);
//static node_t *func_bit__and(int numargs,node_t **args);
//static node_t *func_bit__andc1(int numargs,node_t **args);
//static node_t *func_bit__andc2(int numargs,node_t **args);
//static node_t *func_bit__eqv(int numargs,node_t **args);
//static node_t *func_bit__ior(int numargs,node_t **args);
//static node_t *func_bit__nand(int numargs,node_t **args);
//static node_t *func_bit__nor(int numargs,node_t **args);
//static node_t *func_bit__not(int numargs,node_t **args);
//static node_t *func_bit__orc1(int numargs,node_t **args);
//static node_t *func_bit__orc2(int numargs,node_t **args);
static node_t *func_bit__vector__p(int numargs,node_t **args);
//static node_t *func_bit__xor(int numargs,node_t **args);
//static node_t *func_boole(int numargs,node_t **args);
//static node_t *func_both__case__p(int numargs,node_t **args);
//static node_t *func_boundp(int numargs,node_t **args);
//static node_t *func_break(int numargs,node_t **args);
//static node_t *func_broadcast__stream__streams(int numargs,node_t **args);
//static node_t *func_butlast(int numargs,node_t **args);
//static node_t *func_byte(int numargs,node_t **args);
//static node_t *func_byte__position(int numargs,node_t **args);
//static node_t *func_byte__size(int numargs,node_t **args);
static node_t *func_car(int numargs,node_t **args);
static node_t *func_cdr(int numargs,node_t **args);
static node_t *func_caar(int numargs,node_t **args);
static node_t *func_cadr(int numargs,node_t **args);
static node_t *func_cdar(int numargs,node_t **args);
static node_t *func_cddr(int numargs,node_t **args);
static node_t *func_caaar(int numargs,node_t **args);
static node_t *func_caadr(int numargs,node_t **args);
static node_t *func_cadar(int numargs,node_t **args);
static node_t *func_caddr(int numargs,node_t **args);
static node_t *func_cdaar(int numargs,node_t **args);
static node_t *func_cdadr(int numargs,node_t **args);
static node_t *func_cddar(int numargs,node_t **args);
static node_t *func_cdddr(int numargs,node_t **args);
static node_t *func_caaaar(int numargs,node_t **args);
static node_t *func_caaadr(int numargs,node_t **args);
static node_t *func_caadar(int numargs,node_t **args);
static node_t *func_caaddr(int numargs,node_t **args);
static node_t *func_cadaar(int numargs,node_t **args);
static node_t *func_cadadr(int numargs,node_t **args);
static node_t *func_caddar(int numargs,node_t **args);
static node_t *func_cadddr(int numargs,node_t **args);
static node_t *func_cdaaar(int numargs,node_t **args);
static node_t *func_cdaadr(int numargs,node_t **args);
static node_t *func_cdadar(int numargs,node_t **args);
static node_t *func_cdaddr(int numargs,node_t **args);
static node_t *func_cddaar(int numargs,node_t **args);
static node_t *func_cddadr(int numargs,node_t **args);
static node_t *func_cdddar(int numargs,node_t **args);
static node_t *func_cddddr(int numargs,node_t **args);
//static node_t *func_catenate(int numargs,node_t **args);
//static node_t *func_call__next__method(int numargs,node_t **args);
//static node_t *func_ceiling(int numargs,node_t **args);
//static node_t *func_cell__error__name(int numargs,node_t **args);
//static node_t *func_cerror(int numargs,node_t **args);
//static node_t *func_char(int numargs,node_t **args);
//static node_t *func_char__bit(int numargs,node_t **args);
//static node_t *func_char__bits(int numargs,node_t **args);
//static node_t *func_char__code(int numargs,node_t **args);
//static node_t *func_char__downcase(int numargs,node_t **args);
//static node_t *func_char__equal(int numargs,node_t **args);
//static node_t *func_char__font(int numargs,node_t **args);
//static node_t *func_char__greaterp(int numargs,node_t **args);
//static node_t *func_char__int(int numargs,node_t **args);
//
//static node_t *func_char__lessp(int numargs,node_t **args);
//static node_t *func_char__name(int numargs,node_t **args);
//static node_t *func_char__not__equal(int numargs,node_t **args);
//static node_t *func_char__not__greaterp(int numargs,node_t **args);
//static node_t *func_char__not__lessp(int numargs,node_t **args);
//static node_t *func_char__upcase(int numargs,node_t **args);
//static node_t *func_char_neq(int numargs,node_t **args);
//static node_t *func_char(int numargs,node_t **args);
//static node_t *func_char_eq(int numargs,node_t **args);
//static node_t *func_character(int numargs,node_t **args);
static node_t *func_characterp(int numargs,node_t **args);
//static node_t *func_choose(int numargs,node_t **args);
//static node_t *func_choose__if(int numargs,node_t **args);
//static node_t *func_chunk(int numargs,node_t **args);
//static node_t *func_cis(int numargs,node_t **args);
//static node_t *func_class__of(int numargs,node_t **args);
//static node_t *func_clear__input(int numargs,node_t **args);
//static node_t *func_clear__output(int numargs,node_t **args);
static node_t *func_close(int numargs,node_t **args);
//static node_t *func_clrhash(int numargs,node_t **args);
//static node_t *func_code__char(int numargs,node_t **args);
//static node_t *func_coerce(int numargs,node_t **args);
//static node_t *func_collect(int numargs,node_t **args);
//static node_t *func_collect__alist(int numargs,node_t **args);
//static node_t *func_collect__and(int numargs,node_t **args);
//static node_t *func_collect__append(int numargs,node_t **args);
//static node_t *func_collect__file(int numargs,node_t **args);
//static node_t *func_collect__first(int numargs,node_t **args);
//static node_t *func_collect__fn(int numargs,node_t **args);
//static node_t *func_collect__hash(int numargs,node_t **args);
//static node_t *func_collect__last(int numargs,node_t **args);
//static node_t *func_collect__length(int numargs,node_t **args);
//static node_t *func_collect__max(int numargs,node_t **args);
//static node_t *func_collect__min(int numargs,node_t **args);
//static node_t *func_collect__nconc(int numargs,node_t **args);
//static node_t *func_collect__nth(int numargs,node_t **args);
//static node_t *func_collect__or(int numargs,node_t **args);
//static node_t *func_collect__plist(int numargs,node_t **args);
//static node_t *func_collect__sum(int numargs,node_t **args);
//static node_t *func_collecting__fn(int numargs,node_t **args);
//static node_t *func_commonp(int numargs,node_t **args);
//static node_t *func_compile(int numargs,node_t **args);
//static node_t *func_compile__file(int numargs,node_t **args);
//static node_t *func_compile__file__pathname(int numargs,node_t **args);
//static node_t *func_compiled__function__p(int numargs,node_t **args);
//static node_t *func_compiler__macro__function(int numargs,node_t **args);
//static node_t *func_compiler__macroexpand(int numargs,node_t **args);
//static node_t *func_compiler__macroexpand1(int numargs,node_t **args);
//static node_t *func_complement(int numargs,node_t **args);
//static node_t *func_complex(int numargs,node_t **args);
static node_t *func_complexp(int numargs,node_t **args);
//
//static node_t *func_compute__applicable__methods(int numargs,node_t **args);
//static node_t *func_compute__restarts(int numargs,node_t **args);
//static node_t *func_concatenate(int numargs,node_t **args);
//static node_t *func_concatenated__stream__streams(int numargs,node_t **args);
//static node_t *func_conjugate(int numargs,node_t **args);
static node_t *func_cons(int numargs,node_t **args);
static node_t *func_consp(int numargs,node_t **args);
//static node_t *func_constantp(int numargs,node_t **args);
//static node_t *func_continue(int numargs,node_t **args);
//static node_t *func_copy__alist(int numargs,node_t **args);
//static node_t *func_copy__list(int numargs,node_t **args);
//static node_t *func_copy__pprint__dispatch(int numargs,node_t **args);
//static node_t *func_copy__readtable(int numargs,node_t **args);
//static node_t *func_copy__seq(int numargs,node_t **args);
//static node_t *func_copy__symbol(int numargs,node_t **args);
//static node_t *func_copy__tree(int numargs,node_t **args);
static node_t *func_cos(int numargs,node_t **args);
static node_t *func_cosh(int numargs,node_t **args);
//static node_t *func_cotruncate(int numargs,node_t **args);
//static node_t *func_count(int numargs,node_t **args);
//static node_t *func_count__if(int numargs,node_t **args);
//static node_t *func_count__if__not(int numargs,node_t **args);
//static node_t *func_declaration__information(int numargs,node_t **args);
//static node_t *func_decode__float(int numargs,node_t **args);
//static node_t *func_decode__universal__time(int numargs,node_t **args);
//static node_t *func_delete(int numargs,node_t **args);
//static node_t *func_delete__duplicates(int numargs,node_t **args);
//static node_t *func_delete__file(int numargs,node_t **args);
//static node_t *func_delete__if(int numargs,node_t **args);
//static node_t *func_delete__if__not(int numargs,node_t **args);
//static node_t *func_delete__package(int numargs,node_t **args);
//static node_t *func_denominator(int numargs,node_t **args);
//static node_t *func_deposit__field(int numargs,node_t **args);
//static node_t *func_describe(int numargs,node_t **args);
//static node_t *func_digit__char(int numargs,node_t **args);
//static node_t *func_digit__char__p(int numargs,node_t **args);
//static node_t *func_directory(int numargs,node_t **args);
//static node_t *func_directory__namestring(int numargs,node_t **args);
//static node_t *func_disassemble(int numargs,node_t **args);
//static node_t *func_documentation(int numargs,node_t **args);
//static node_t *func_dpb(int numargs,node_t **args);
//static node_t *func_dribble(int numargs,node_t **args);
//static node_t *func_echo__stream__input__stream(int numargs,node_t **args);
//static node_t *func_echo__stream__output__stream(int numargs,node_t **args);
//static node_t *func_ed(int numargs,node_t **args);
//static node_t *func_eighth(int numargs,node_t **args);
//static node_t *func_elt(int numargs,node_t **args);
//static node_t *func_enclose(int numargs,node_t **args);
//static node_t *func_encode__universal__time(int numargs,node_t **args);
static node_t *func_endp(int numargs,node_t **args);
//
//static node_t *func_enough__namestring(int numargs,node_t **args);
//static node_t *func_ensure__generic__function(int numargs,node_t **args);
static node_t *func_eq(int numargs,node_t **args);
static node_t *func_eql(int numargs,node_t **args);
static node_t *func_equal(int numargs,node_t **args);
static node_t *func_equalp(int numargs,node_t **args);
//static node_t *func_error(int numargs,node_t **args);
static node_t *func_eval(int numargs,node_t **args);
//static node_t *func_evalhook(int numargs,node_t **args);
static node_t *func_evenp(int numargs,number_node_t **args);
//static node_t *func_every(int numargs,node_t **args);
static node_t *func_exp(int numargs,node_t **args);
//static node_t *func_expand(int numargs,node_t **args);
//static node_t *func_export(int numargs,node_t **args);
//static node_t *func_expt(int numargs,node_t **args);
//static node_t *func_fboundp(int numargs,node_t **args);
//static node_t *func_fceiling(int numargs,node_t **args);
//static node_t *func_fdefinition(int numargs,node_t **args);
//static node_t *func_ffloor(int numargs,node_t **args);
//static node_t *func_fifth(int numargs,node_t **args);
//static node_t *func_file__author(int numargs,node_t **args);
//static node_t *func_file__error__pathname(int numargs,node_t **args);
//static node_t *func_file__length(int numargs,node_t **args);
//static node_t *func_file__namestring(int numargs,node_t **args);
//static node_t *func_file__position(int numargs,node_t **args);
//static node_t *func_file__string__length(int numargs,node_t **args);
//static node_t *func_file__write__data(int numargs,node_t **args);
//static node_t *func_fill(int numargs,node_t **args);
//static node_t *func_fill__pointer(int numargs,node_t **args);
//static node_t *func_find(int numargs,node_t **args);
//static node_t *func_find__all__symbols(int numargs,node_t **args);
//static node_t *func_find__class(int numargs,node_t **args);
//static node_t *func_find__if(int numargs,node_t **args);
//static node_t *func_find__if__not(int numargs,node_t **args);
//static node_t *func_find__package(int numargs,node_t **args);
//static node_t *func_find__restart(int numargs,node_t **args);
//static node_t *func_find__symbol(int numargs,node_t **args);
//static node_t *func_finish__output(int numargs,node_t **args);
//static node_t *func_first(int numargs,node_t **args);
//static node_t *func_float(int numargs,node_t **args);
//static node_t *func_float__digits(int numargs,node_t **args);
//static node_t *func_float__precision(int numargs,node_t **args);
//static node_t *func_float__radix(int numargs,node_t **args);
//static node_t *func_float__sign(int numargs,node_t **args);
static node_t *func_floatp(int numargs,node_t **args);
//static node_t *func_floor(int numargs,node_t **args);
//static node_t *func_fmakunbound(int numargs,node_t **args);
//static node_t *func_force__output(int numargs,node_t **args);
//static node_t *func_format(int numargs,node_t **args);
//static node_t *func_fourth(int numargs,node_t **args);
//static node_t *func_fresh__line(int numargs,node_t **args);
//static node_t *func_fround(int numargs,node_t **args);
//static node_t *func_ftruncate(int numargs,node_t **args);
static node_t *func_funcall(int numargs,node_t **args);
//
//static node_t *func_function__information(int numargs,node_t **args);
//static node_t *func_funcation__lambda__expression(int numargs,node_t **args);
static node_t *func_functionp(int numargs,node_t **args);
//static node_t *func_gatherer(int numargs,node_t **args);
//static node_t *func_gcd(int numargs,node_t **args);
//static node_t *func_generator(int numargs,node_t **args);
//static node_t *func_gensym(int numargs,node_t **args);
//static node_t *func_gentemp(int numargs,node_t **args);
//static node_t *func_get(int numargs,node_t **args);
//static node_t *func_get__decoded__time(int numargs,node_t **args);
//static node_t *func_get__dispatch__macro__character(int numargs,node_t **args);
//static node_t *func_get__internal__real__time(int numargs,node_t **args);
//static node_t *func_get__internal__run__time(int numargs,node_t **args);
//static node_t *func_get__macro__character(int numargs,node_t **args);
//static node_t *func_get__output__stream__string(int numargs,node_t **args);
//static node_t *func_get__properties(int numargs,node_t **args);
//static node_t *func_get__setf__method(int numargs,node_t **args);
//static node_t *func_get__setf__method__multiple__value(int numargs,node_t **args);
//static node_t *func_get__universal__time(int numargs,node_t **args);
//static node_t *func_getf(int numargs,node_t **args);
//static node_t *func_gethash(int numargs,node_t **args);
//static node_t *func_graphic__char__p(int numargs,node_t **args);
//static node_t *func_hash__table__count(int numargs,node_t **args);
//static node_t *func_hash__table__p(int numargs,node_t **args);
//static node_t *func_hash__table__rehash__size(int numargs,node_t **args);
//static node_t *func_hash__table__rehash__threshold(int numargs,node_t **args);
//static node_t *func_hash__table__size(int numargs,node_t **args);
//static node_t *func_hash__table__test(int numargs,node_t **args);
//static node_t *func_host__namestring(int numargs,node_t **args);
//static node_t *func_identity(int numargs,node_t **args);
//static node_t *func_imagpart(int numargs,node_t **args);
//static node_t *func_import(int numargs,node_t **args);
//static node_t *func_in__package(int numargs,node_t **args);
//static node_t *func_input__stream__p(int numargs,node_t **args);
//static node_t *func_inspect(int numargs,node_t **args);
//static node_t *func_int__char(int numargs,node_t **args);
//static node_t *func_integer__decode__float(int numargs,node_t **args);
//static node_t *func_integer__length(int numargs,node_t **args);
static node_t *func_integerp(int numargs,node_t **args);
//static node_t *func_interactive__stream__p(int numargs,node_t **args);
//static node_t *func_intern(int numargs,node_t **args);
//static node_t *func_intersection(int numargs,node_t **args);
//static node_t *func_invalid__method__error(int numargs,node_t **args);
//
//static node_t *func_invoke__debugger(int numargs,node_t **args);
//static node_t *func_invoke__restart(int numargs,node_t **args);
//static node_t *func_invoke__restart__interactively(int numargs,node_t **args);
//static node_t *func_tively(int numargs,node_t **args);
//static node_t *func_isqrt(int numargs,node_t **args);
//static node_t *func_keywordp(int numargs,node_t **args);
static node_t *func_last(int numargs,node_t **args);
//static node_t *func_latch(int numargs,node_t **args);
//static node_t *func_lcm(int numargs,node_t **args);
//static node_t *func_ldb(int numargs,node_t **args);
//static node_t *func_ldb__test(int numargs,node_t **args);
//static node_t *func_ldiff(int numargs,node_t **args);
//static node_t *func_length(int numargs,node_t **args);
//static node_t *func_lisp__implementation__type(int numargs,node_t **args);
//static node_t *func_lisp__implementation__version(int numargs,node_t **args);
static node_t *func_list(int numargs,node_t **args);
//static node_t *func_list_star(int numargs,node_t **args);
//static node_t *func_list__all__packages(int numargs,node_t **args);
//static node_t *func_list__length(int numargs,node_t **args);
//static node_t *func_listen(int numargs,node_t **args);
static node_t *func_listp(int numargs,node_t **args);
static node_t *func_load(int numargs,node_t **args);
//static node_t *func_load__logical__pathname__translations(int numargs,node_t **args);
static node_t *func_log(int numargs,node_t **args);
//static node_t *func_logand(int numargs,node_t **args);
//static node_t *func_logandc1(int numargs,node_t **args);
//static node_t *func_logandc2(int numargs,node_t **args);
//static node_t *func_logbitp(int numargs,node_t **args);
//static node_t *func_logcount(int numargs,node_t **args);
//static node_t *func_logeqv(int numargs,node_t **args);
//static node_t *func_logical__pathname(int numargs,node_t **args);
//static node_t *func_logical__pathname__translations(int numargs,node_t **args);
//static node_t *func_logior(int numargs,node_t **args);
//static node_t *func_lognand(int numargs,node_t **args);
//static node_t *func_lognor(int numargs,node_t **args);
//static node_t *func_lognot(int numargs,node_t **args);
//static node_t *func_logorc1(int numargs,node_t **args);
//static node_t *func_logorc2(int numargs,node_t **args);
//static node_t *func_logtest(int numargs,node_t **args);
//static node_t *func_logxor(int numargs,node_t **args);
//static node_t *func_long__site__name(int numargs,node_t **args);
//static node_t *func_lower__case__p(int numargs,node_t **args);
//static node_t *func_machine__instance(int numargs,node_t **args);
//static node_t *func_machine__type(int numargs,node_t **args);
//static node_t *func_machine__version(int numargs,node_t **args);
//static node_t *func_macro__function(int numargs,node_t **args);
//static node_t *func_macroexpand(int numargs,node_t **args);
//static node_t *func_macroexpand__1(int numargs,node_t **args);
//static node_t *func_make__array(int numargs,node_t **args);
//static node_t *func_make__broadcase__stream(int numargs,node_t **args);
//static node_t *func_make__char(int numargs,node_t **args);
//
//static node_t *func_make__concatenated__stream(int numargs,node_t **args);
//static node_t *func_make__condition(int numargs,node_t **args);
//static node_t *func_make__dispatch__macro__character(int numargs,node_t **args);
//static node_t *func_make__echo__stream(int numargs,node_t **args);
//static node_t *func_make__hash__table(int numargs,node_t **args);
//static node_t *func_make__list(int numargs,node_t **args);
//static node_t *func_make__load__form__saving__slots(int numargs,node_t **args);
//static node_t *func_make__package(int numargs,node_t **args);
//static node_t *func_make__pathname(int numargs,node_t **args);
//static node_t *func_make__random__state(int numargs,node_t **args);
//static node_t *func_make__seqence(int numargs,node_t **args);
//static node_t *func_make__string(int numargs,node_t **args);
//static node_t *func_make__string__output__stream(int numargs,node_t **args);
//static node_t *func_make__symbol(int numargs,node_t **args);
//static node_t *func_make__synonym__stream(int numargs,node_t **args);
//static node_t *func_make__two__way__stream(int numargs,node_t **args);
//static node_t *func_makunbound(int numargs,node_t **args);
//static node_t *func_map(int numargs,node_t **args);
//static node_t *func_map__fn(int numargs,node_t **args);
//static node_t *func_map__into(int numargs,node_t **args);
//static node_t *func_mapc(int numargs,node_t **args);
//static node_t *func_mapcan(int numargs,node_t **args);
static node_t *func_mapcar(int numargs,node_t **args);
//static node_t *func_mapcon(int numargs,node_t **args);
//static node_t *func_maphash(int numargs,node_t **args);
//static node_t *func_mapl(int numargs,node_t **args);
//static node_t *func_maplist(int numargs,node_t **args);
//static node_t *func_mask(int numargs,node_t **args);
//static node_t *func_mask__field(int numargs,node_t **args);
//static node_t *func_max(int numargs,node_t **args);
//static node_t *func_member(int numargs,node_t **args);
//static node_t *func_member__if(int numargs,node_t **args);
//static node_t *func_member__if__not(int numargs,node_t **args);
//static node_t *func_merge(int numargs,node_t **args);
//static node_t *func_merge__pathnames(int numargs,node_t **args);
//static node_t *func_method__combination__error(int numargs,node_t **args);
//static node_t *func_min(int numargs,node_t **args);
//static node_t *func_mingle(int numargs,node_t **args);
static node_t *func_minusp(int numargs,number_node_t **args);
//static node_t *func_mismatch(int numargs,node_t **args);
//static node_t *func_mod(int numargs,node_t **args);
//static node_t *func_muffle__warning(int numargs,node_t **args);
//static node_t *func_name__char(int numargs,node_t **args);
//static node_t *func_namestring(int numargs,node_t **args);
//static node_t *func_nbutlast(int numargs,node_t **args);
//static node_t *func_nconc(int numargs,node_t **args);
//static node_t *func_next__method__p(int numargs,node_t **args);
//
//static node_t *func_next__out(int numargs,node_t **args);
//static node_t *func_nintersection(int numargs,node_t **args);
//static node_t *func_ninth(int numargs,node_t **args);
static node_t *func_not(int numargs,node_t **args);
//static node_t *func_notany(int numargs,node_t **args);
//static node_t *func_notevery(int numargs,node_t **args);
//static node_t *func_nreconc(int numargs,node_t **args);
//static node_t *func_nreverse(int numargs,node_t **args);
//static node_t *func_nset__difference(int numargs,node_t **args);
//static node_t *func_nset__exclusive__or(int numargs,node_t **args);
//static node_t *func_nstring__capitalize(int numargs,node_t **args);
//static node_t *func_nstring__downcase(int numargs,node_t **args);
//static node_t *func_nstring__upcase(int numargs,node_t **args);
//static node_t *func_nsublis(int numargs,node_t **args);
//static node_t *func_nsubst(int numargs,node_t **args);
//static node_t *func_nsubst__if(int numargs,node_t **args);
//static node_t *func_nsubst__if__not(int numargs,node_t **args);
//static node_t *func_nsubstitute(int numargs,node_t **args);
//static node_t *func_nsubstitute__if(int numargs,node_t **args);
//static node_t *func_nsubstitute__if__not(int numargs,node_t **args);
static node_t *func_nth(int numargs,node_t **args);
static node_t *func_nthcdr(int numargs,node_t **args);
static node_t *func_null(int numargs,node_t **args);
static node_t *func_numberp(int numargs,node_t **args);
//static node_t *func_numerator(int numargs,node_t **args);
//static node_t *func_nunion(int numargs,node_t **args);
static node_t *func_oddp(int numargs,number_node_t **args);
static node_t *func_open(int numargs,node_t **args);
static node_t *func_open__stream__p(int numargs,node_t **args);
//static node_t *func_output__stream__p(int numargs,node_t **args);
//static node_t *func_package__error__package(int numargs,node_t **args);
//static node_t *func_package__name(int numargs,node_t **args);
//static node_t *func_pacakge__nicknames(int numargs,node_t **args);
//static node_t *func_package__shadowing__symbols(int numargs,node_t **args);
//static node_t *func_package__use__list(int numargs,node_t **args);
//static node_t *func_package__used__by__list(int numargs,node_t **args);
static node_t *func_packagep(int numargs,node_t **args);
//static node_t *func_pairlis(int numargs,node_t **args);
//static node_t *func_parse__integer(int numargs,node_t **args);
//static node_t *func_parse__macro(int numargs,node_t **args);
//static node_t *func_parse__namestring(int numargs,node_t **args);
//static node_t *func_pathname(int numargs,node_t **args);
//static node_t *func_pathname__device(int numargs,node_t **args);
//static node_t *func_pathname__directory(int numargs,node_t **args);
//static node_t *func_pathname__host(int numargs,node_t **args);
//static node_t *func_pathname__match__p(int numargs,node_t **args);
//
//static node_t *func_pathname__name(int numargs,node_t **args);
//static node_t *func_pathname__type(int numargs,node_t **args);
//static node_t *func_pathname__version(int numargs,node_t **args);
//static node_t *func_pathnamep(int numargs,node_t **args);
//static node_t *func_peek__char(int numargs,node_t **args);
//static node_t *func_phase(int numargs,node_t **args);
static node_t *func_plusp(int numargs,number_node_t **args);
//static node_t *func_position(int numargs,node_t **args);
//static node_t *func_position__if(int numargs,node_t **args);
//static node_t *func_position__if__not(int numargs,node_t **args);
//static node_t *func_positions(int numargs,node_t **args);
//static node_t *func_pprint(int numargs,node_t **args);
//static node_t *func_pprint__dispatch(int numargs,node_t **args);
//static node_t *func_pprint__fill(int numargs,node_t **args);
//static node_t *func_pprint__indent(int numargs,node_t **args);
//static node_t *func_pprint__linear(int numargs,node_t **args);
//static node_t *func_pprint__newline(int numargs,node_t **args);
//static node_t *func_pprint__tab(int numargs,node_t **args);
//static node_t *func_pprint__tabular(int numargs,node_t **args);
//static node_t *func_previous(int numargs,node_t **args);
static node_t *func_prin1(int numargs,node_t **args);
//static node_t *func_prin1__to__string(int numargs,node_t **args);
static node_t *func_princ(int numargs,node_t **args);
//static node_t *func_princ__to__string(int numargs,node_t **args);
static node_t *func_print(int numargs,node_t **args);
//static node_t *func_probe__file(int numargs,node_t **args);
//static node_t *func_proclaim(int numargs,node_t **args);
//static node_t *func_provide(int numargs,node_t **args);
//static node_t *func_random(int numargs,node_t **args);
//static node_t *func_random__state__p(int numargs,node_t **args);
//static node_t *func_rassoc(int numargs,node_t **args);
//static node_t *func_rassoc__if(int numargs,node_t **args);
//static node_t *func_rassoc__if__not(int numargs,node_t **args);
//static node_t *func_rational(int numargs,node_t **args);
//static node_t *func_rationalize(int numargs,node_t **args);
static node_t *func_rationalp(int numargs,node_t **args);
//static node_t *func_read(int numargs,node_t **args);
//static node_t *func_read__byte(int numargs,node_t **args);
//static node_t *func_read__char(int numargs,node_t **args);
//static node_t *func_read__char__no__hang(int numargs,node_t **args);
//static node_t *func_read__delimited__list(int numargs,node_t **args);
//static node_t *func_read__from__string(int numargs,node_t **args);
//static node_t *func_read__line(int numargs,node_t **args);
//static node_t *func_read__preserving__whitespace(int numargs,node_t **args);
//static node_t *func_readtable__case(int numargs,node_t **args);
//static node_t *func_readtablep(int numargs,node_t **args);
static node_t *func_realp(int numargs,node_t **args);
//static node_t *func_realpart(int numargs,node_t **args);
//static node_t *func_reduce(int numargs,node_t **args);
//static node_t *func_rem(int numargs,node_t **args);
//static node_t *func_remhash(int numargs,node_t **args);
//
//static node_t *func_remove(int numargs,node_t **args);
//static node_t *func_remove__duplicates(int numargs,node_t **args);
//static node_t *func_remove__if(int numargs,node_t **args);
//static node_t *func_remove__if__not(int numargs,node_t **args);
//static node_t *func_remprop(int numargs,node_t **args);
//static node_t *func_rename__file(int numargs,node_t **args);
//static node_t *func_rename__package(int numargs,node_t **args);
//static node_t *func_replace(int numargs,node_t **args);
//static node_t *func_require(int numargs,node_t **args);
//static node_t *func_rest(int numargs,node_t **args);
//static node_t *func_restart__name(int numargs,node_t **args);
//static node_t *func_result__of(int numargs,node_t **args);
//static node_t *func_revappend(int numargs,node_t **args);
//static node_t *func_reverse(int numargs,node_t **args);
static node_t *func_room(int numargs,node_t **args);
//static node_t *func_round(int numargs,node_t **args);
//static node_t *func_row__major__aref(int numargs,node_t **args);
static node_t *func_rplaca(int numargs,node_t **args);
static node_t *func_rplacd(int numargs,node_t **args);
//static node_t *func_sbit(int numargs,node_t **args);
//static node_t *func_scale__float(int numargs,node_t **args);
//static node_t *func_scan(int numargs,node_t **args);
//static node_t *func_scan__alist(int numargs,node_t **args);
//static node_t *func_scan__file(int numargs,node_t **args);
//static node_t *func_scan__fn(int numargs,node_t **args);
//static node_t *func_scan__fn__inclusive(int numargs,node_t **args);
//static node_t *func_scan__hash(int numargs,node_t **args);
//static node_t *func_scan__lists__of__lists(int numargs,node_t **args);
//static node_t *func_scan__lists__of__lists__fringe(int numargs,node_t **args);
//static node_t *func_scan__multiple(int numargs,node_t **args);
//static node_t *func_scan__plist(int numargs,node_t **args);
//static node_t *func_scan__range(int numargs,node_t **args);
//static node_t *func_scan__sublists(int numargs,node_t **args);
//static node_t *func_scan__symbols(int numargs,node_t **args);
//static node_t *func_schar(int numargs,node_t **args);
//static node_t *func_search(int numargs,node_t **args);
//static node_t *func_second(int numargs,node_t **args);
//static node_t *func_series(int numargs,node_t **args);
static node_t *func_set(int numargs,node_t **args);
//static node_t *func_set__char__bit(int numargs,node_t **args);
//static node_t *func_set__difference(int numargs,node_t **args);
//static node_t *func_set__dispatch__macro__character(int numargs,node_t **args);
//static node_t *func_set__exclusive__or(int numargs,node_t **args);
//static node_t *func_set__macro__character(int numargs,node_t **args);
//static node_t *func_set__pprint__dispatch(int numargs,node_t **args);
//static node_t *func_set__syntax__from__char(int numargs,node_t **args);
//static node_t *func_seventh(int numargs,node_t **args);
//static node_t *func_shadow(int numargs,node_t **args);
//static node_t *func_shadowing__import(int numargs,node_t **args);
//static node_t *func_short__site__name(int numargs,node_t **args);
//static node_t *func_signal(int numargs,node_t **args);
//
//static node_t *func_signum(int numargs,node_t **args);
static node_t *func_simple__bit__vector__p(int numargs,node_t **args);
//static node_t *func_simple__condition__format__arguments(int numargs,node_t **args);
//static node_t *func_simple__condition__format__string(int numargs,node_t **args);
static node_t *func_simple__string__p(int numargs,node_t **args);
static node_t *func_simple__vector__p(int numargs,node_t **args);
static node_t *func_sin(int numargs,node_t **args);
static node_t *func_sinh(int numargs,node_t **args);
//static node_t *func_sixth(int numargs,node_t **args);
static node_t *func_sleep(int numargs,node_t **args);
//static node_t *func_slot__boundp(int numargs,node_t **args);
//static node_t *func_slot__exists__p(int numargs,node_t **args);
//static node_t *func_slot__makunbound(int numargs,node_t **args);
//static node_t *func_slot__value(int numargs,node_t **args);
//static node_t *func_software__type(int numargs,node_t **args);
//static node_t *func_software__version(int numargs,node_t **args);
//static node_t *func_some(int numargs,node_t **args);
//static node_t *func_sort(int numargs,node_t **args);
//static node_t *func_special__form__p(int numargs,node_t **args);
//static node_t *func_split(int numargs,node_t **args);
//static node_t *func_split__if(int numargs,node_t **args);
static node_t *func_sqrt(int numargs,node_t **args);
//static node_t *func_stable__sort(int numargs,node_t **args);
//static node_t *func_standard__char__p(int numargs,node_t **args);
//static node_t *func_store__value(int numargs,node_t **args);
//static node_t *func_stream__element__type(int numargs,node_t **args);
//static node_t *func_stream__error__stream(int numargs,node_t **args);
//static node_t *func_stream__external__format(int numargs,node_t **args);
static node_t *func_streamp(int numargs,node_t **args);
//static node_t *func_string(int numargs,node_t **args);
//static node_t *func_string__capitalize(int numargs,node_t **args);
//static node_t *func_string__char__p(int numargs,node_t **args);
//static node_t *func_string__downcase(int numargs,node_t **args);
//static node_t *func_string__equal(int numargs,node_t **args);
//static node_t *func_string__greaterp(int numargs,node_t **args);
//static node_t *func_string__left__trim(int numargs,node_t **args);
//static node_t *func_string__lessp(int numargs,node_t **args);
//static node_t *func_string__not__equal(int numargs,node_t **args);
//static node_t *func_string__not__greaterp(int numargs,node_t **args);
//
//static node_t *func_string__not__lessp(int numargs,node_t **args);
//static node_t *func_string__right__trim(int numargs,node_t **args);
//static node_t *func_string__trim(int numargs,node_t **args);
//static node_t *func_string__upcase(int numargs,node_t **args);
//static node_t *func_string_neq(int numargs,node_t **args);
//static node_t *func_string_lt(int numargs,node_t **args);
//static node_t *func_string_lte(int numargs,node_t **args);
//static node_t *func_string_gt(int numargs,node_t **args);
//static node_t *func_string_gte(int numargs,node_t **args);
//static node_t *func_string(int numargs,node_t **args);
static node_t *func_stringp(int numargs,node_t **args);
//static node_t *func_sublis(int numargs,node_t **args);
//static node_t *func_subseq(int numargs,node_t **args);
//static node_t *func_subseries(int numargs,node_t **args);
//static node_t *func_subsetp(int numargs,node_t **args);
//static node_t *func_subst(int numargs,node_t **args);
//static node_t *func_subst__if(int numargs,node_t **args);
//static node_t *func_subst__if__not(int numargs,node_t **args);
//static node_t *func_substitute(int numargs,node_t **args);
//static node_t *func_substitute__if(int numargs,node_t **args);
//static node_t *func_substitute__if__not(int numargs,node_t **args);
//static node_t *func_subtypep(int numargs,node_t **args);
//static node_t *func_svref(int numargs,node_t **args);
//static node_t *func_sxhash(int numargs,node_t **args);
//static node_t *func_symbol__function(int numargs,node_t **args);
//static node_t *func_symbol__name(int numargs,node_t **args);
//static node_t *func_symbol__package(int numargs,node_t **args);
//static node_t *func_symbol__plist(int numargs,node_t **args);
//static node_t *func_symbol__value(int numargs,node_t **args);
static node_t *func_symbolp(int numargs,node_t **args);
//static node_t *func_synonym__stream__symbol(int numargs,node_t **args);
//static node_t *func_tailp(int numargs,node_t **args);
static node_t *func_tan(int numargs,node_t **args);
static node_t *func_tanh(int numargs,node_t **args);
//static node_t *func_tenth(int numargs,node_t **args);
static node_t *func_terpri(int numargs,node_t **args);
//static node_t *func_third(int numargs,node_t **args);
//static node_t *func_to__alter(int numargs,node_t **args);
//static node_t *func_translate__logical__pathname(int numargs,node_t **args);
//static node_t *func_translate__pathname(int numargs,node_t **args);
//static node_t *func_tree__equal(int numargs,node_t **args);
//static node_t *func_truename(int numargs,node_t **args);
//static node_t *func_truncate(int numargs,node_t **args);
//
//static node_t *func_two__way__stream__input__stream(int numargs,node_t **args);
//static node_t *func_two__way__stream__output__stream(int numargs,node_t **args);
//static node_t *func_type__error__datum(int numargs,node_t **args);
//static node_t *func_type__error__expected__type(int numargs,node_t **args);
//static node_t *func_type__of(int numargs,node_t **args);
//static node_t *func_typep(int numargs,node_t **args);
//static node_t *func_unexport(int numargs,node_t **args);
//static node_t *func_unintern(int numargs,node_t **args);
//static node_t *func_union(int numargs,node_t **args);
//static node_t *func_unread__char(int numargs,node_t **args);
//static node_t *func_until(int numargs,node_t **args);
//static node_t *func_until__if(int numargs,node_t **args);
//static node_t *func_unuse__package(int numargs,node_t **args);
//static node_t *func_upgraded__array__element__type(int numargs,node_t **args);
//static node_t *func_upgraded__complex__part__type(int numargs,node_t **args);
//static node_t *func_upper__case__p(int numargs,node_t **args);
//static node_t *func_use__package(int numargs,node_t **args);
//static node_t *func_use__value(int numargs,node_t **args);
//static node_t *func_user__homedir__pathname(int numargs,node_t **args);
//static node_t *func_values(int numargs,node_t **args);
//static node_t *func_values__list(int numargs,node_t **args);
//static node_t *func_variable__information(int numargs,node_t **args);
static node_t *func_vector(int numargs,node_t **args);
//static node_t *func_vector__pop(int numargs,node_t **args);
//static node_t *func_vector__push(int numargs,node_t **args);
//static node_t *func_vector__push__extend(int numargs,node_t **args);
static node_t *func_vectorp(int numargs,node_t **args);
//static node_t *func_warn(int numargs,node_t **args);
//static node_t *func_wild__pathname__p(int numargs,node_t **args);
//static node_t *func_write(int numargs,node_t **args);
//static node_t *func_write__byte(int numargs,node_t **args);
//static node_t *func_write__char(int numargs,node_t **args);
//static node_t *func_write__line(int numargs,node_t **args);
//static node_t *func_write__string(int numargs,node_t **args);
//static node_t *func_write__to__string(int numargs,node_t **args);
//static node_t *func_y__or__n__p(int numargs,node_t **args);
//static node_t *func_yes__or__no__p(int numargs,node_t **args);
static node_t *func_zerop(int numargs,number_node_t **args);

static node_t *func_gc(int numargs,node_t **args);


/*
static node_t *func_(int numargs,node_t **args);
*/


#define func(n,f,min,max) current_package->get_symbol(n)->set_form(new sysfunction_t(n,f,min,max))

void init_funcs()
{

	func("ABS", func_abs,       1,1);
	func("ACOS",    func_acos,      1,1);
//  func("ACOSH",   func_acosh,     1,1);
	func("ASIN",    func_asin,      1,1);
//  func("ASINH",   func_asinh,     1,1);
	func("ATAN",    func_atan,      1,2);
//  func("ATANH",   func_atanh,     1,1);
	func("COS", func_cos,       1,1);
	func("COSH",    func_cosh,      1,1);
	func("EXP", func_exp,       1,1);
	func("LOG", func_log,       1,1);
	func("SIN", func_sin,       1,1);
	func("SINH",    func_sinh,      1,1);
	func("SQRT",    func_sqrt,      1,1);
	func("TAN", func_tan,       1,1);
	func("TANH",    func_tanh,      1,1);

	func("*",   (PFUNCCALL)func_multiply,      0,-1);
	func("+",   (PFUNCCALL)func_add,       0,-1);
	func("-",   (PFUNCCALL)func_subtract,      1,-1);
	func("/",   (PFUNCCALL)func_divide,        1,-1);
	func("1+",  (PFUNCCALL)func_1plus,     1,1);
	func("1-",  (PFUNCCALL)func_1minus,        1,1);
	func("<",   (PFUNCCALL)func_lt,        1,-1);
	func("<=",  (PFUNCCALL)func_lte,       1,-1);
	func("=",   (PFUNCCALL)func_eqeq,      1,-1);
	func("/=",  (PFUNCCALL)func_neq,       1,-1);
	func(">",   (PFUNCCALL)func_gt,        1,-1);
	func(">=",  (PFUNCCALL)func_gte,       1,-1);

	func("ZEROP",   (PFUNCCALL)func_zerop,     1,1);
	func("PLUSP",   (PFUNCCALL)func_plusp,     1,1);
	func("MINUSP",  (PFUNCCALL)func_minusp,        1,1);
	func("ODDP",    (PFUNCCALL)func_oddp,      1,1);
	func("EVENP",   (PFUNCCALL)func_evenp,     1,1);
	func("ENDP",    func_endp,      1,1);

	func("EQ",  func_eq,        2,2);
	func("EQL", func_eql,       2,2);
	func("EQUAL",   func_equal,     2,2);
	func("EQUALP",  func_equalp,        2,2);

	func("APPEND",  func_append,        2,2);

	func("CAR", func_car,       1,1);
	func("CDR", func_cdr,       1,1);
	func("CAAR",    func_caar,      1,1);
	func("CADR",    func_cadr,      1,1);
	func("CDAR",    func_cdar,      1,1);
	func("CDDR",    func_cddr,      1,1);
	func("CAAAR",   func_caaar,     1,1);
	func("CAADR",   func_caadr,     1,1);
	func("CADAR",   func_cadar,     1,1);
	func("CADDR",   func_caddr,     1,1);
	func("CDAAR",   func_cdaar,     1,1);
	func("CDADR",   func_cdadr,     1,1);
	func("CDDAR",   func_cddar,     1,1);
	func("CDDDR",   func_cdddr,     1,1);
	func("CAAAAR",  func_caaaar,        1,1);
	func("CAAADR",  func_caaadr,        1,1);
	func("CAADAR",  func_caadar,        1,1);
	func("CAADDR",  func_caaddr,        1,1);
	func("CADAAR",  func_cadaar,        1,1);
	func("CADADR",  func_cadadr,        1,1);
	func("CADDAR",  func_caddar,        1,1);
	func("CADDDR",  func_cadddr,        1,1);
	func("CDAAAR",  func_cdaaar,        1,1);
	func("CDAADR",  func_cdaadr,        1,1);
	func("CDADAR",  func_cdadar,        1,1);
	func("CDADDR",  func_cdaddr,        1,1);
	func("CDDAAR",  func_cddaar,        1,1);
	func("CDDADR",  func_cddadr,        1,1);
	func("CDDDAR",  func_cdddar,        1,1);
	func("CDDDDR",  func_cddddr,        1,1);

	func("CONS",    func_cons,      2,2);
	func("EVAL",    func_eval,      1,1);
	func("GC",  func_gc,        0,0);
	func("LIST",    func_list,      1,-1);
	func("LOAD",    func_load,      0,1);
	func("NOT", func_not,       1,1);
	func("NULL",    func_null,  1,1);
	func("SET", func_set,       2,2);
	func("ROOM",	func_room,        0,1);
	func("VECTOR",  func_vector,        0,-1);
	func("AREF",    func_aref,      1,-1);

	func("FUNCALL", func_funcall,   1,-1);
	func("APPLY",   func_apply, 2,-1);

	func("SYMBOLP", func_symbolp,   1,1);
	func("ATOM",    func_atom,  1,1);
	func("CONSP",   func_consp, 1,1);
	func("LISTP",   func_listp, 1,1);
	func("NUMBERP", func_numberp,   1,1);
	func("INTEGERP",func_integerp,  1,1);
	func("RATIONALP",func_rationalp,1,1);
	func("FLOATP",  func_floatp,    1,1);
	func("REALP",   func_realp, 1,1);
	func("COMPLEXP",func_complexp,  1,1);
	func("CHARACTERP",func_characterp,1,1);
	func("STRINGP", func_stringp,   1,1);
	func("BIT-VECTOR-P",func_bit__vector__p,1,1);
	func("VECTORP", func_vectorp,   1,1);
	func("SIMPLE-VECTOR-P",func_simple__vector__p,1,1);
	func("SIMPLE-STRING-P",func_simple__string__p,1,1);
	func("SIMPLE-BIT-VECTOR-P",func_simple__bit__vector__p,1,1);
	func("ARRAYP",  func_arrayp,    1,1);
	func("PACKAGEP",func_packagep,  1,1);
	func("FUNCTIONP",func_functionp,1,1);

	func("MAPCAR",  func_mapcar,    2,-1);

	func("PRIN1",   func_prin1, 1,2);
	func("PRINC",   func_princ, 1,2);
	func("PRINT",   func_print, 1,2);
	func("TERPRI",  func_terpri,    0,1);

	func("CLOSE",	func_close,	1,1);
	func("OPEN",	func_open,	1,1);
	func("STREAMP", func_streamp,	1,1);
	func("OPEN-STREAM-P",func_open__stream__p,1,1);
	func("RPLACA",	func_rplaca,	2,2);
	func("RPLACD",	func_rplacd,	2,2);
	func("LAST",	func_last,	1,1);
	func("NTH",	func_nth,	2,2);
	func("NTHCDR",	func_nthcdr,	2,2);

	func("SLEEP",	func_sleep,		1,1);
}



/*******************************************************************
********************************************************************
********************************************************************
*********************            ***********************************
********************* MATH FUNCS ***********************************
*********************            ***********************************
********************************************************************
********************************************************************
*******************************************************************/

#define singlemath(x)\
	args[0]->check_number();\
	errno = 0;\
	double t = ##x((double)(float)(*((number_node_t *)args[0])));\
	if (errno != 0)\
		throw_eval_exception(MATH_ERROR);\
	return new number_node_t((float)t)

static node_t *func_abs(int,node_t **args){singlemath(fabs);}
static node_t *func_acos(int,node_t **args){singlemath(acos);}
static node_t *func_asin(int,node_t **args){singlemath(asin);}
static node_t *func_cos(int,node_t **args){singlemath(cos);}
static node_t *func_cosh(int,node_t **args){singlemath(cosh);}
static node_t *func_exp(int,node_t **args){singlemath(exp);}
static node_t *func_log(int,node_t **args){singlemath(log);}
static node_t *func_sin(int,node_t **args){singlemath(sin);}
static node_t *func_sinh(int,node_t **args){singlemath(sinh);}
static node_t *func_sqrt(int,node_t **args){singlemath(sqrt);}
static node_t *func_tan(int,node_t **args){singlemath(tan);}
static node_t *func_tanh(int,node_t **args){singlemath(tanh);}

static node_t *func_atan(int numargs,node_t **args)
{
	double t;
	args[0]->check_number();
	if (numargs == 1)
		t = atan((double)(float)(*((number_node_t *)args[0])));
	else
	{
		args[1]->check_number();
		t = atan2(
			(double)(float)(*((number_node_t *)args[0])),
			(double)(float)(*((number_node_t *)args[1]))
			);
	}
	if (errno)
		throw_eval_exception(MATH_ERROR);
	return new number_node_t((float)t);
}

static node_t *func_add(int numargs,number_node_t **args)
{
	number_node_t *res = new number_node_t(0L);
	for (int i=0;i<numargs;i++)
	{
		args[i]->check_number();
		*res += *(args[i]);
	}
	return res;
}

static node_t *func_multiply(int numargs,number_node_t **args)
{
	number_node_t *res = new number_node_t(1L);
	for (int i=0;i<numargs;i++)
	{
		args[i]->check_number();
		*res *= *(args[i]);
	}
	return res;
}

static node_t *func_subtract(int numargs,number_node_t **args)
{
	number_node_t *res = new number_node_t(0L);
	args[0]->check_number();
	if (numargs == 1)
		*res -= *args[0];
	else
		*res += *args[0];
	for (int i=1;i<numargs;i++)
	{
		args[i]->check_number();
		*res -= *(args[i]);
	}
	return res;
}

static node_t *func_divide(int numargs,number_node_t **args)
{
	number_node_t *res = new number_node_t(1L);
	args[0]->check_number();
	if (numargs == 1)
		*res /= *args[0];
	else
		*res *= *args[0];
	for (int i=1;i<numargs;i++)
	{
		args[0]->check_number();
		*res /= *(args[i]);
	}
	return res;
}

static node_t *func_1plus(int,number_node_t **args)
{
	args[0]->check_number();
	number_node_t *res = new number_node_t(1L);
	*res += *args[0];
	return res;
}

static node_t *func_1minus(int,number_node_t **args)
{
	args[0]->check_number();
	number_node_t *res = new number_node_t(-1L);
	*res += *args[0];
	return res;
}

static node_t *func_zerop(int,number_node_t **args)
{
	args[0]->check_number();
	return ( *args[0] == 0L ) ? pTrue : nil;
}

static node_t *func_plusp(int,number_node_t **args)
{
	args[0]->check_number();
	return ( *args[0] > 0L ) ? pTrue : nil;
}

static node_t *func_minusp(int,number_node_t **args)
{
	args[0]->check_number();
	return ( *args[0] < 0L ) ? pTrue : nil;
}

static node_t *func_oddp(int,number_node_t **args)
{
	args[0]->check_number();
	if (args[0]->get_sub_type() != TYPE_LONG)
		throw_eval_exception(BAD_ARG_TYPE);
	return ( args[0]->lval%2 == 0L ) ? nil : pTrue;
}

static node_t *func_evenp(int,number_node_t **args)
{
	args[0]->check_number();
	if (args[0]->get_sub_type() != TYPE_LONG)
		throw_eval_exception(BAD_ARG_TYPE);
	return ( args[0]->lval%2 == 0L ) ? pTrue : nil;
}


/*******************************************************************
********************************************************************
********************************************************************
*********************       ****************************************
********************* FUNCS ****************************************
*********************       ****************************************
********************************************************************
********************************************************************
*******************************************************************/

static node_t *func_eval(int,node_t **args)
{
	return args[0]->eval();
}

static node_t *func_car(int,node_t **args)
{
	return args[0]->car();
}

static node_t *func_cdr(int,node_t **args)
{
	return args[0]->cdr();
}

static node_t *func_caar(int numargs,node_t **args)
{
	return args[0]->car()->car();
}

static node_t *func_cadr(int numargs,node_t **args)
{
	return args[0]->cdr()->car();
}

static node_t *func_cdar(int numargs,node_t **args)
{
	return args[0]->car()->cdr();
}

static node_t *func_cddr(int numargs,node_t **args)
{
	return args[0]->cdr()->cdr();
}

static node_t *func_caaar(int numargs,node_t **args)
{
	return args[0]->car()->car()->car();
}

static node_t *func_caadr(int numargs,node_t **args)
{
	return args[0]->cdr()->car()->car();
}

static node_t *func_cadar(int numargs,node_t **args)
{
	return args[0]->car()->cdr()->car();
}

static node_t *func_caddr(int numargs,node_t **args)
{
	return args[0]->cdr()->cdr()->car();
}

static node_t *func_cdaar(int numargs,node_t **args)
{
	return args[0]->car()->car()->cdr();
}

static node_t *func_cdadr(int numargs,node_t **args)
{
	return args[0]->cdr()->car()->cdr();
}

static node_t *func_cddar(int numargs,node_t **args)
{
	return args[0]->car()->cdr()->cdr();
}

static node_t *func_cdddr(int numargs,node_t **args)
{
	return args[0]->cdr()->cdr()->cdr();
}

static node_t *func_caaaar(int numargs,node_t **args)
{
	return args[0]->car()->car()->car()->car();
}

static node_t *func_caaadr(int numargs,node_t **args)
{
	return args[0]->cdr()->car()->car()->car();
}

static node_t *func_caadar(int numargs,node_t **args)
{
	return args[0]->car()->cdr()->car()->car();
}

static node_t *func_caaddr(int numargs,node_t **args)
{
	return args[0]->cdr()->cdr()->car()->car();
}

static node_t *func_cadaar(int numargs,node_t **args)
{
	return args[0]->car()->car()->cdr()->car();
}

static node_t *func_cadadr(int numargs,node_t **args)
{
	return args[0]->cdr()->car()->cdr()->car();
}

static node_t *func_caddar(int numargs,node_t **args)
{
	return args[0]->car()->cdr()->cdr()->car();
}

static node_t *func_cadddr(int numargs,node_t **args)
{
	return args[0]->cdr()->cdr()->cdr()->car();
}

static node_t *func_cdaaar(int numargs,node_t **args)
{
	return args[0]->car()->car()->car()->cdr();
}

static node_t *func_cdaadr(int numargs,node_t **args)
{
	return args[0]->cdr()->car()->car()->cdr();
}

static node_t *func_cdadar(int numargs,node_t **args)
{
	return args[0]->car()->cdr()->car()->cdr();
}

static node_t *func_cdaddr(int numargs,node_t **args)
{
	return args[0]->cdr()->cdr()->car()->cdr();
}

static node_t *func_cddaar(int numargs,node_t **args)
{
	return args[0]->car()->car()->cdr()->cdr();
}

static node_t *func_cddadr(int numargs,node_t **args)
{
	return args[0]->cdr()->car()->cdr()->cdr();
}

static node_t *func_cdddar(int numargs,node_t **args)
{
	return args[0]->car()->cdr()->cdr()->cdr();
}

static node_t *func_cddddr(int numargs,node_t **args)
{
	return args[0]->cdr()->cdr()->cdr()->cdr();
}

static node_t *func_gc(int,node_t **)
{
	int i = NodeListArray.garbage_collect();
	return new number_node_t((long)i);
}

static node_t *func_cons(int,node_t **args)
{
	return new cons_t(args[0],args[1]);
}

static node_t *func_list(int numargs,node_t **args)
{
	if (!numargs)
		return nil;
	cons_t *top = new cons_t();
	top->set_car(args[0]);
	cons_t *cur = top;
	for (int i=1;i<numargs;i++)
	{
		cur->set_cdr(new cons_t());
		cur = (cons_t *)(cur->Cdr());
		cur->set_car(args[i]);
	}
	cur->set_cdr(nil);
	return top;
}

static node_t *func_append(int numargs,node_t **args)
{
	if (!numargs)
		return nil;
	if (numargs == 1)
		return args[0];
	cons_t *top = new cons_t();
	cons_t *cur = top;
	int i = 0;
	for (i=0;i<numargs-1;i++)
	{
		if (args[i] == nil)
			continue;
		args[i]->check_arg_type(TYPE_CONS);
		cons_t *p = (cons_t *)args[i];
		while (p->is_a(TYPE_CONS))
		{
			cur->set_cdr(new cons_t());
			cur = cur->CdrCONS();
			cur->set_car(p->Car());
			p = p->CdrCONS();
		}
	}
	cur->set_cdr(args[i]);
	return top->Cdr();
}

static node_t *func_set(int,node_t **args)
{
	binding_stack_state_t(g_bind_stack, nullptr); /* Cleans up on destruction. */
	args[0]->check_arg_type(TYPE_SYMBOL);
	((symbol_t *)args[0])->set_value(args[1]);
	return args[1];
}

static node_t *func_print(int numargs,node_t **args)
{
	ostream *pstream = &cout;
	if (numargs == 2)
	{
		args[1]->check_arg_type(TYPE_STREAM);
		((stream_node_t*)args[1])->check_stream();
		pstream = ((stream_node_t*)args[1])->get_stream();
	}
	(*pstream) << "\n";
	args[0]->print(*pstream);
	(*pstream) << ' ';
	pstream->flush();
	return args[0];

}

static node_t *func_terpri(int numargs,node_t **args)
{
	ostream *pstream = &cout;
	if (numargs == 1)
	{
		args[0]->check_arg_type(TYPE_STREAM);
		((stream_node_t*)args[0])->check_stream();
		pstream = ((stream_node_t*)args[0])->get_stream();
	}
	(*pstream) << "\n";
	pstream->flush();
	return nil;
}

static node_t *func_prin1(int numargs ,node_t **args)
{
	ostream *pstream = &cout;
	if (numargs == 2)
	{
		args[1]->check_arg_type(TYPE_STREAM);
		((stream_node_t*)args[1])->check_stream();
		pstream = ((stream_node_t*)args[1])->get_stream();
	}
	args[0]->print(*pstream);
	pstream->flush();
	return args[0];
}

static node_t *func_princ(int numargs,node_t **args)
{
	ostream *pstream = &cout;
	if (numargs == 2)
	{
		args[1]->check_arg_type(TYPE_STREAM);
		((stream_node_t*)args[1])->check_stream();
		pstream = ((stream_node_t*)args[1])->get_stream();
	}
	args[0]->princ(*pstream);
	pstream->flush();
	return args[0];
}

static node_t *func_room(int,node_t **)
{
	cout << "g_frame_stack\n";
	g_frame_stack.print(cout);
	cout << "g_bind_stack\n";
	g_bind_stack.print(cout);
	cout << "\n";
	NodeListArray.status(cout);

	cout.flush();
	return nil;
}

static node_t *func_not(int,node_t **args)
{
	return (args[0] == nil) ? pTrue : nil;
}

static node_t *func_null(int,node_t **args)
{
	return (args[0] == nil) ? pTrue : nil;
}

static node_t *func_lt(int numargs,number_node_t **args)
{
	for (int i=0;i<numargs-1;i++)
		if (!(*args[i] < *args[i+1]))
			return nil;
	return pTrue;
}

static node_t *func_gt(int numargs,number_node_t **args)
{
	for (int i=0;i<numargs-1;i++)
		if (!(*args[i] > *args[i+1]))
			return nil;
	return pTrue;
}

static node_t *func_lte(int numargs,number_node_t **args)
{
	for (int i=0;i<numargs-1;i++)
		if (!(*args[i] <= *args[i+1]))
			return nil;
	return pTrue;
}

static node_t *func_gte(int numargs,number_node_t **args)
{
	for (int i=0;i<numargs-1;i++)
		if (!(*args[i] >= *args[i+1]))
			return nil;
	return pTrue;
}

static node_t *func_eqeq(int numargs,number_node_t **args)
{
	for (int i=0;i<numargs-1;i++)
		if (!(*args[i] == *args[i+1]))
			return nil;
	return pTrue;
}

static node_t *func_neq(int numargs,number_node_t **args)
{
	for (int i=0;i<numargs-1;i++)
		for (int j=i+1;j<numargs;j++)
			if (*args[i] == *args[j])
				return nil;
	return pTrue;
}

static node_t *func_load(int numargs,node_t **args)
{
	if (numargs == 0)
		return nil;
	if (args[0] == nil)
		return nil;
	auto str = args[0]->as<string_node_t>();
	ifstream ifs(str->data(),ios::in | ios::_Nocreate);
	if (!ifs.is_open())
		throw_eval_exception(FILE_NOT_FOUND);
	lisp_env_t env(str->data(), ifs, cout);
	env.reploop();
	return env._exit_status;
}

static node_t *func_eq(int,node_t **args)
{
	return (args[0] == args[1]) ? pTrue : nil;
}

static node_t *func_eql(int,node_t **args)
{
	if (args[0] == args[1])
		return pTrue;
	if (args[0]->get_type() != args[1]->get_type())
		return nil;
	switch(args[0]->get_type())
	{
		case TYPE_STRING:
		case TYPE_SYMBOL:
		case TYPE_CONS:
			return (args[0] == args[1]) ? pTrue : nil;
			break;
		case TYPE_LONG:
			return (((number_node_t *)args[0])->lval == ((number_node_t *)args[1])->lval) ? pTrue : nil;
			break;
		case TYPE_FLOAT:
			return (((number_node_t *)args[0])->fval == ((number_node_t *)args[1])->fval) ? pTrue : nil;
			break;
	}
	throw_eval_exception(UNKNOWN_NODE_TYPE);
	return nil; // will never get here
}

static node_t *helper_equal(node_t *n1,node_t *n2)
{
	if (n1->get_type() != n2->get_type())
		return nil;
	switch(n1->get_type())
	{
		case TYPE_SYMBOL:
			return (n1 == n2) ? pTrue : nil;
			break;
		case TYPE_STRING:
			return (lstrcmp(((string_node_t*)n1)->data(),((string_node_t*)n2)->data())) ? nil : pTrue;
			break;
		case TYPE_CONS:
			return (helper_equal(((cons_t *)n1)->Car(),((cons_t *)n2)->Car()) && helper_equal(((cons_t *)n1)->Cdr(),((cons_t *)n2)->Cdr())) ? pTrue : nil;
			break;
		case TYPE_LONG:
			return (((number_node_t *)n1)->lval == ((number_node_t *)n2)->lval) ? pTrue : nil;
			break;
		case TYPE_FLOAT:
			return (((number_node_t *)n1)->fval == ((number_node_t *)n2)->fval) ? pTrue : nil;
			break;
	}
	throw_eval_exception(UNKNOWN_NODE_TYPE);
	return nil; // will never get here
}

static node_t *func_equal(int,node_t **args)
{
	return helper_equal(args[0],args[1]);
}

static node_t *helper_equalp(node_t *n1,node_t *n2)
{
	if (n1->is_a_number() && n2->is_a_number())
		return (*(number_node_t *)n1 == *(number_node_t *)n2) ? pTrue : nil;
	if (n1->get_type() != n2->get_type())
			return nil;
	switch(n1->get_type())
	{
		case TYPE_SYMBOL:
			return (n1 == n2) ? pTrue : nil;
			break;
		case TYPE_STRING:
			return (lstrcmpi(((string_node_t*)n1)->data(),((string_node_t*)n2)->data())) ? nil : pTrue;
			break;
		case TYPE_CONS:
			return (helper_equalp(((cons_t *)n1)->Car(),((cons_t *)n2)->Car()) && helper_equal(((cons_t *)n1)->Cdr(),((cons_t *)n2)->Cdr())) ? pTrue : nil;
			break;
	}
	throw_eval_exception(UNKNOWN_NODE_TYPE);
	return nil; // will never get here
}

static node_t *func_equalp(int,node_t **args)
{
	return helper_equalp(args[0],args[1]);
}

static node_t *func_endp(int,node_t **args)
{
	if (args[0] == nil)
		return pTrue;
	else if (args[0]->is_a(TYPE_CONS))
		return nil;
	else
		throw_eval_exception(BAD_ARG_TYPE);
	return nil; // should never get here
}

static node_t *func_vector(int numargs,node_t **args)
{
	long li = numargs;
	std::vector<node_t*> v;
	for (li=0;li<numargs;li++)
		v.push_back(args[li]);
	return new vector_t(std::move(v));
}

static node_t *func_aref(int numargs,node_t **args)
{
#ifdef ADSFDASFSA
	if (args[0]->is_a(TYPE_VECTOR))
	{
		if (numargs > 2)
			throw_eval_exception(TOO_MANY_ARGS);
		if (numargs < 2)
			throw_eval_exception(TOO_FEW_ARGS);
		if (!args[1]->is_a(TYPE_LONG))
			throw_eval_exception(BAD_ARG_TYPE);
		long idx = (long)(*args[1]);
		if (idx >= args[0]->pvector->dim)
			throw_eval_exception(INDEX_OUT_OF_BOUNDS);
		if (idx < 0)
			throw_eval_exception(INDEX_OUT_OF_BOUNDS);
		return args[0]->pvector->pvec[idx];
	}
	else if (args[0]->is_a(TYPE_ARRAY))
	{
		struct tagarray *pa = args[0]->parray;
		if (numargs-1 < pa->rank)
			throw_eval_exception(TOO_FEW_ARGS);
		if (numargs-1 > pa->rank)
			throw_eval_exception(TOO_MANY_ARGS);
		long cnt=0,li,t;
		for (li=0;li < pa->rank;li++)
		{
			t = (long)args[li+1];
			if (t<0 || t >= pa->dims[li])
				throw_eval_exception(INDEX_OUT_OF_BOUNDS);
			cnt = (cnt * pa->dims[li])+t;
		}
		return pa->pvec[cnt];
	}
	else
		throw_eval_exception(BAD_ARG_TYPE);
#endif
	return nil;
}

static node_t *func_funcall(int numargs,node_t **args)
{
	function *pf;
	if (args[0]->is_a(TYPE_FUNCTION))
		pf =  (function *)args[0];
	else
	{
		args[0]->check_arg_type(TYPE_SYMBOL);
		pf = (function *)(((symbol_t *)args[0])->check_form());
		if (!pf->is_a(TYPE_FUNCTION))
			throw_eval_exception(NOT_A_FUNCTION);
	}
	return pf->eval(numargs-1,&args[1]);
}

static node_t *func_apply(int numargs,node_t **args)
{
	cons_t *p=NULL;
	volatile cons_t *shadowp=NULL;
	function *pf;
	node_t *res;
	if (args[0]->is_a(TYPE_FUNCTION))
		pf =  (function *)args[0];
	else
	{
		args[0]->check_arg_type(TYPE_SYMBOL);
		pf = (function *)(((symbol_t *)args[0])->check_form());
		if (!pf->is_a(TYPE_FUNCTION))
			throw_eval_exception(NOT_A_FUNCTION);
	}
	/* There are zero or more arguments on the stack. */
	if (numargs > 0)
	{
		auto last = g_frame_stack.top();
		/* If the last one is a list, expand its items. */
		if (last->is_a(TYPE_CONS) || last == nil) /* nil is empty list */
		{
			int cnt = 0; /* Number of arguments in list. */
			frame_stack_state_t state(g_frame_stack);
			/* Copy numargs items on the stack. */
			g_frame_stack.duplicate(numargs);
			ASSERT(last == g_frame_stack.top());
			g_frame_stack.pop(); /* Remove cons/nil. */
			/* Iterate through list pushing arguments in last cons. */
			while (last->is_a(TYPE_CONS))
			{
				cons_t* cons = (cons_t *)last;
				g_frame_stack.push(cons->car());
				last = cons->cdr();
				++cnt;
			}
			auto actual_args = numargs - 2 + cnt;
			res = pf->eval(actual_args, g_frame_stack.get_base(actual_args));
		}
		else
			res = pf->eval(numargs - 1, &args[1]);
	}
	return res;
}


static node_t *func_symbolp(int numargs,node_t **args)
{
	return args[0]->is_a(TYPE_SYMBOL) ? pTrue : nil;
}

static node_t *func_atom(int numargs,node_t **args)
{
	return args[0]->is_a(TYPE_CONS) ? nil : pTrue;
}

static node_t *func_consp(int numargs,node_t **args)
{
	return args[0]->is_a(TYPE_CONS) ? pTrue : nil;
}

static node_t *func_listp(int numargs,node_t **args)
{
	return (args[0]->is_a(TYPE_CONS) || args[0] == nil) ? pTrue : nil;
}

static node_t *func_numberp(int numargs,node_t **args)
{
	return (args[0]->is_a(TYPE_FLOAT) || args[0]->is_a(TYPE_LONG)) ? pTrue : nil;
}

static node_t *func_integerp(int numargs,node_t **args)
{
	return args[0]->is_a(TYPE_LONG) ? pTrue : nil;
}

static node_t *func_rationalp(int numargs,node_t **args)
{
	return args[0]->is_a(TYPE_LONG) ? pTrue : nil;
}

static node_t *func_floatp(int numargs,node_t **args)
{
	return args[0]->is_a(TYPE_FLOAT) ? pTrue : nil;
}

static node_t *func_realp(int numargs,node_t **args)
{
	return (args[0]->is_a(TYPE_FLOAT) || args[0]->is_a(TYPE_LONG)) ? pTrue : nil;
}

static node_t *func_complexp(int numargs,node_t **args)
{
	return nil;
}

static node_t *func_characterp(int numargs,node_t **args)
{
	return nil;
}

static node_t *func_stringp(int numargs,node_t **args)
{
	return args[0]->is_a(TYPE_STRING) ? pTrue : nil;
}

static node_t *func_bit__vector__p(int numargs,node_t **args)
{
	return nil;
}

static node_t *func_vectorp(int numargs,node_t **args)
{
	return args[0]->is_a(TYPE_VECTOR) ? pTrue : nil;
}

static node_t *func_simple__vector__p(int numargs,node_t **args)
{
	return args[0]->is_a(TYPE_VECTOR) ? pTrue : nil;
}

static node_t *func_simple__string__p(int numargs,node_t **args)
{
	return args[0]->is_a(TYPE_STRING) ? pTrue : nil;
}

static node_t *func_simple__bit__vector__p(int numargs,node_t **args)
{
	return nil;
}

static node_t *func_arrayp(int numargs,node_t **args)
{
	return nil;
}

static node_t *func_packagep(int numargs,node_t **args)
{
	return args[0]->is_a(TYPE_PACKAGE) ? pTrue : nil;
}

static node_t *func_functionp(int numargs,node_t **args)
{
	return args[0]->is_a(TYPE_FUNCTION) ? pTrue : nil;
}

static node_t *func_mapcar(int numargs,node_t **args)
{
	volatile int i;
	int j;
	args[0]->check_arg_type(TYPE_FUNCTION);
	function *pf = (function *)args[0];
	for (i=1;i<numargs;i++)
		args[i]->check_arg_type(TYPE_CONS);
	std::vector<cons_t*> p(numargs - 1);
	cons_t *res,*top;
	res = top = new cons_t(nil,nil);
	for (i=1;i<numargs;i++)
		p[i-1] = (cons_t *)args[i];
	numargs--;
	while (1)
	{
		frame_stack_state_t state(g_frame_stack); // Resets g_frame_stack when destroyed.
		for (i=0;i<numargs;i++)
			g_frame_stack.push(p[i]->Car());
		res->set_car(pf->eval(numargs,g_frame_stack.get_base(numargs)));
		for (j=0;j<numargs;j++)
		{
			p[j] = p[j]->CdrCONS();
			if (!p[j]->is_a(TYPE_CONS))
				return top;
		}
		res->set_cdr(new cons_t(nil,nil));
		res = res->CdrCONS();
	}
	return nil;
}

static node_t *func_close(int numargs,node_t **args)
{
	args[0]->check_arg_type(TYPE_STREAM);
	((stream_node_t*)args[0])->close();
	return pTrue;
}

static node_t *func_open(int numargs,node_t **args)
{
	args[0]->check_arg_type(TYPE_STRING);
	stream_node_t*p = new stream_node_t( ((string_node_t*)args[0])->data());
	return (p->get_stream()) ? (node_t *)p : (node_t *)nil;
}

static node_t *func_streamp(int numargs,node_t **args)
{
	return (args[0]->is_a(TYPE_STREAM)) ? pTrue : nil;
}

static node_t *func_open__stream__p(int numargs,node_t **args)
{
	auto p = args[0]->as<stream_node_t>();
	if (!p->get_stream())
		return nil;
	if (p->get_stream()->is_open())
		return pTrue;
	return nil;
}

static node_t *func_rplaca(int numargs,node_t **args)
{
	args[0]->check_arg_type(TYPE_CONS);
	((cons_t *)args[0])->set_car(args[1]);
	return args[0];
}

static node_t *func_rplacd(int numargs,node_t **args)
{
	args[0]->check_arg_type(TYPE_CONS);
	((cons_t *)args[0])->set_cdr(args[1]);
	return args[0];
}

static node_t *func_last(int numargs,node_t **args)
{
	if (args[0] == nil)
		return nil;
	args[0]->check_arg_type(TYPE_CONS);
	cons_t *p = (cons_t *)args[0];
	while (p->Cdr()->is_a(TYPE_CONS))
		p = p->CdrCONS();
	return p;
}

static node_t *func_nth(int numargs,node_t **args)
{
	args[0]->check_arg_type(TYPE_LONG);
	if (args[1] == nil)
		return nil;
	args[1]->check_arg_type(TYPE_CONS);
	cons_t *p = (cons_t *)args[1];
	long n = (long)(*(number_node_t *)args[0]);
	if (n < 0)
		throw_eval_exception(DOMAIN_ERROR);
	while (n-- && p->is_a(TYPE_CONS))
		p = p->CdrCONS();
	return (p->is_a(TYPE_CONS)) ? p->Car() : nil;
}

static node_t *func_nthcdr(int numargs,node_t **args)
{
	args[0]->check_arg_type(TYPE_LONG);
	if (args[1] == nil)
		return nil;
	args[1]->check_arg_type(TYPE_CONS);
	cons_t *p = (cons_t *)args[1];
	long n = (long)(*(number_node_t *)args[0]);
	if (n < 0)
		throw_eval_exception(DOMAIN_ERROR);
	while (n-- && p->is_a(TYPE_CONS))
		p = p->CdrCONS();
	return p;
}

static node_t *func_sleep(int numargs,node_t **args)
{
	float t = *args[0]->as<number_node_t>();
	DWORD dw = GetTickCount() + (DWORD)(int)(t*1000.F);
	while (GetTickCount() < dw)
		;
	return nil;
}

