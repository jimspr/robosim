#include "stdafx.h"
#include "node.h"
#include "package.h"
#include "constant.h"

symbol_t* key_optional;
symbol_t* key_rest;
symbol_t* key_key;
symbol_t* key_allow_other_keys;
symbol_t* key_aux;
symbol_t* key_body;
symbol_t* key_whole;
symbol_t* key_environment;
symbol_t* key_int;
symbol_t* key_float;
symbol_t* key_string;
symbol_t* key_symbol;
symbol_t* key_color;
symbol_t* key_relative_to;
symbol_t* key_classic;
symbol_t* key_min;
symbol_t* key_max;
symbol_t* key_position;
symbol_t* key_ambient;
symbol_t* key_diffuse;
symbol_t* key_specular;
symbol_t* key_shininess;

nil_t* nil;
symbol_t* pTrue;
dot_t* dot_node;

symbol_t* pPLUS1 = nullptr;
symbol_t* pPLUS2 = nullptr;
symbol_t* pPLUS3 = nullptr;
symbol_t* pSTAR1 = nullptr;
symbol_t* pSTAR2 = nullptr;
symbol_t* pSTAR3 = nullptr;
symbol_t* pDIV1 = nullptr;
symbol_t* pDIV2 = nullptr;
symbol_t* pDIV3 = nullptr;
symbol_t* pMINUS = nullptr;

#define decvariable(n,v) current_package->get_symbol(n)->set_value(v);

void init_variables()
{
	decvariable("*",    nil);
	decvariable("**",   nil);
	decvariable("***",  nil);
	decvariable("*applyhook*",  nil);
	decvariable("*break-on-signals*",   nil);
	decvariable("*break-on-warning*",   nil);
	decvariable("*compile-file-pathname*",  nil);
	decvariable("*compile-file-truename*",  nil);
	decvariable("*compile-print*",  nil);
	decvariable("*compile-verbose*",    nil);
	decvariable("*debug-io*",   nil);
	decvariable("*debugger-hook*",  nil);
	decvariable("*default-pathname-defaults*",  nil);
	decvariable("*error-output*",   nil);
	decvariable("*evalhook*",   nil);
	decvariable("*features*",   nil);
	decvariable("*gensym-counter*", nil);
	decvariable("*load-pathname*",  nil);
	decvariable("*load-print*", nil);
	decvariable("*load-truename*",  nil);
	decvariable("*load-verbose*",   nil);
	decvariable("*macroexpand-hook*",   nil);
	decvariable("*modules*",    nil);
	decvariable("*package*",    nil);
	decvariable("*print-array*",    nil);
	decvariable("*print-base*", nil);
	decvariable("*print-case*", nil);
	decvariable("*print-circle*",   nil);
	decvariable("*print-escape*",   nil);
	decvariable("*print-gensym*",   nil);
	decvariable("*print-length*",   nil);
	decvariable("*print-level*",    nil);
	decvariable("*print-lines*",    nil);
	decvariable("*print-miser-width*",  nil);
	decvariable("*print-pprint-dispatch*",  nil);
	decvariable("*print-pretty*",   nil);
	decvariable("*print-radix*",    nil);
	decvariable("*print-readably*", nil);
	decvariable("*print-right-margin*", nil);
	decvariable("*query-io*",   nil);
	decvariable("*random-state*",   nil);
	decvariable("*read-base*",  nil);
	decvariable("*read-default-float-format*",  nil);
	decvariable("*read-eval*",  nil);
	decvariable("*read-suppress*",  nil);
	decvariable("*readtable*",  nil);
	decvariable("*standard-input*", nil);
	decvariable("*standard-output*",    nil);
	decvariable("*suppress-series-warnings*",   nil);
	decvariable("*terminal-io*",    nil);
	decvariable("*trace-output*",   nil);
	decvariable("+",    nil);
	decvariable("++",   nil);
	decvariable("+++",  nil);
	decvariable("-",    nil);
	decvariable("/",    nil);
	decvariable("//",   nil);
	decvariable("///",  nil);

	pPLUS1 = current_package->get_symbol("+");
	pPLUS2 = current_package->get_symbol("++");
	pPLUS3 = current_package->get_symbol("+++");
	pSTAR1 = current_package->get_symbol("*");
	pSTAR2 = current_package->get_symbol("**");
	pSTAR3 = current_package->get_symbol("***");
	pDIV1 = current_package->get_symbol("/");
	pDIV2 = current_package->get_symbol("//");
	pDIV3 = current_package->get_symbol("///");
	pMINUS = current_package->get_symbol("-");
}


#define decconstant(n,v) current_package->get_symbol(n)->set_constant_value((node_t *)v);
#define deckeyword(var,n)\
{\
	var = new symbol_t(n);\
	var->set_constant_value(var);\
	current_package->add_symbol(n,var);\
}

void init_constants()
{
	decconstant("ARRAY-DIMENSION-LIMIT", new number_node_t(LC_ARRAY_DIMENSION_LIMIT));
	decconstant("ARRAY-RANK-LIMIT", new number_node_t(LC_ARRAY_RANK_LIMIT));
	decconstant("ARRAY-TOTAL-SIZE-LIMIT", new number_node_t(LC_ARRAY_TOTAL_SIZE_LIMIT));
	decconstant("BOOLE-1", new number_node_t(LC_BOOLE_1));
	decconstant("BOOLE-2", new number_node_t(LC_BOOLE_2));
	decconstant("BOOLE-AND", new number_node_t(LC_BOOLE_AND));
	decconstant("BOOLE-ANDC1", new number_node_t(LC_BOOLE_ANDC1));
	decconstant("BOOLE-ANDC2", new number_node_t(LC_BOOLE_ANDC2));
	decconstant("BOOLE-C1", new number_node_t(LC_BOOLE_C1));
	decconstant("BOOLE-C2", new number_node_t(LC_BOOLE_C2));
	decconstant("BOOLE-CLR", new number_node_t(LC_BOOLE_CLR));
	decconstant("BOOLE-EQV", new number_node_t(LC_BOOLE_EQV));
	decconstant("BOOLE-IOR", new number_node_t(LC_BOOLE_IOR));
	decconstant("BOOLE-NAND", new number_node_t(LC_BOOLE_NAND));
	decconstant("BOOLE-NOR", new number_node_t(LC_BOOLE_NOR));
	decconstant("BOOLE-ORC1", new number_node_t(LC_BOOLE_ORC1));
	decconstant("BOOLE-ORC2", new number_node_t(LC_BOOLE_ORC2));
	decconstant("BOOLE-SET", new number_node_t(LC_BOOLE_SET));
	decconstant("BOOLE-XOR", new number_node_t(LC_BOOLE_XOR));
	decconstant("CALL-ARGUMENTS-LIMIT", new number_node_t(LC_CALL_ARGUMENTS_LIMIT));
	decconstant("CHAR-CODE-LIMIT", new number_node_t(LC_CHAR_CODE_LIMIT));
	decconstant("DOUBLE-FLOAT-EPSILON", new number_node_t(LC_DOUBLE_FLOAT_EPSILON));
	decconstant("DOUBLE-FLOAT-NEGATIVE-EPSILON", new number_node_t(LC_DOUBLE_FLOAT_NEGATIVE_EPSILON));
	decconstant("LAMBDA-PARAMETERS-LIMIT", new number_node_t(LC_LAMBDA_PARAMETERS_LIMIT));
	decconstant("LEAST-NEGATIVE-DOUBLE-FLOAT", new number_node_t(LC_LEAST_NEGATIVE_DOUBLE_FLOAT));
	decconstant("LEAST-NEGATIVE-LONG-FLOAT", new number_node_t(LC_LEAST_NEGATIVE_LONG_FLOAT));
	decconstant("LEAST-NEGATIVE-NORMALIZED-DOUBLE-FLOAT", new number_node_t(LC_LEAST_NEGATIVE_NORMALIZED_DOUBLE_FLOAT));
	decconstant("LEAST-NEGATIVE-NORMALIZED-LONG-FLOAT", new number_node_t(LC_LEAST_NEGATIVE_NORMALIZED_LONG_FLOAT));
	decconstant("LEAST-NEGATIVE-NORMALIZED-SHORT-FLOAT", new number_node_t(LC_LEAST_NEGATIVE_NORMALIZED_SHORT_FLOAT));
	decconstant("LEAST-NEGATIVE-NORMALIZED-SINGLE-FLOAT", new number_node_t(LC_LEAST_NEGATIVE_NORMALIZED_SINGLE_FLOAT));
	decconstant("LEAST-NEGATIVE-SHORT-FLOAT", new number_node_t(LC_LEAST_NEGATIVE_SHORT_FLOAT));
	decconstant("LEAST-NEGATIVE-SINGLE-FLOAT", new number_node_t(LC_LEAST_NEGATIVE_SINGLE_FLOAT));
	decconstant("LEAST-POSITIVE-DOUBLE-FLOAT", new number_node_t(LC_LEAST_POSITIVE_DOUBLE_FLOAT));
	decconstant("LEAST-POSITIVE-LONG-FLOAT", new number_node_t(LC_LEAST_POSITIVE_LONG_FLOAT));
	decconstant("LEAST-POSITIVE-NORMALIZED-DOUBLE-FLOAT", new number_node_t(LC_LEAST_POSITIVE_NORMALIZED_DOUBLE_FLOAT));
	decconstant("LEAST-POSITIVE-NORMALIZED-LONG-FLOAT", new number_node_t(LC_LEAST_POSITIVE_NORMALIZED_LONG_FLOAT));
	decconstant("LEAST-POSITIVE-NORMALIZED-SHORT-FLOAT", new number_node_t(LC_LEAST_POSITIVE_NORMALIZED_SHORT_FLOAT));
	decconstant("LEAST-POSITIVE-NORMALIZED-SINGLE-FLOAT", new number_node_t(LC_LEAST_POSITIVE_NORMALIZED_SINGLE_FLOAT));
	decconstant("LEAST-POSITIVE-SHORT-FLOAT", new number_node_t(LC_LEAST_POSITIVE_SHORT_FLOAT));
	decconstant("LEAST-POSITIVE-SINGLE-FLOAT", new number_node_t(LC_LEAST_POSITIVE_SINGLE_FLOAT));
	decconstant("LONG-FLOAT-EPSILON", new number_node_t(LC_LONG_FLOAT_EPSILON));
	decconstant("LONG-FLOAT-NEGATIVE-EPSILON", new number_node_t(LC_LONG_FLOAT_NEGATIVE_EPSILON));
	decconstant("MOST-NEGATIVE-DOUBLE-FLOAT", new number_node_t(LC_MOST_NEGATIVE_DOUBLE_FLOAT));
	decconstant("MOST-NEGATIVE-FIXNUM", new number_node_t(LC_MOST_NEGATIVE_FIXNUM));
	decconstant("MOST-NEGATIVE-LONG-FLOAT", new number_node_t(LC_MOST_NEGATIVE_LONG_FLOAT));
	decconstant("MOST-NEGATIVE-SHORT-FLOAT", new number_node_t(LC_MOST_NEGATIVE_SHORT_FLOAT));
	decconstant("MOST-NEGATIVE-SINGLE-FLOAT", new number_node_t(LC_MOST_NEGATIVE_SINGLE_FLOAT));
	decconstant("MOST-POSITIVE-DOUBLE-FLOAT", new number_node_t(LC_MOST_POSITIVE_DOUBLE_FLOAT));
	decconstant("MOST-POSITIVE-FIXNUM", new number_node_t(LC_MOST_POSITIVE_FIXNUM));
	decconstant("MOST-POSITIVE-LONG-FLOAT", new number_node_t(LC_MOST_POSITIVE_LONG_FLOAT));
	decconstant("MOST-POSITIVE-SHORT-FLOAT", new number_node_t(LC_MOST_POSITIVE_SHORT_FLOAT));
	decconstant("MOST-POSITIVE-SINGLE-FLOAT", new number_node_t(LC_MOST_POSITIVE_SINGLE_FLOAT));
	decconstant("MULTIPLE-VALUES-LIMIT", new number_node_t(LC_MULTIPLE_VALUES_LIMIT));
	decconstant("PI", new number_node_t(LC_PI));
	decconstant("SHORT-FLOAT-EPSILON", new number_node_t(LC_SHORT_FLOAT_EPSILON));
	decconstant("SHORT-FLOAT-NEGATIVE-EPSILON", new number_node_t(LC_SHORT_FLOAT_NEGATIVE_EPSILON));
	decconstant("SINGLE-FLOAT-EPSILON", new number_node_t(LC_SINGLE_FLOAT_EPSILON));
	decconstant("SINGLE-FLOAT-NEGATIVE-EPSILON", new number_node_t(LC_SINGLE_FLOAT_NEGATIVE_EPSILON));

	/* Add global nil symbol. */
	nil = new nil_t();
	current_package->add_symbol("NIL", (symbol_t*)nil);

	/* Add global "T" symbol for true. */
	pTrue = new symbol_t("T");
	pTrue->set_constant_value(pTrue);
	current_package->add_symbol("T", pTrue);

	dot_node = new dot_t();

	//  decconstant("LC-INTERNAL-TIME-UNITS-PER-SECOND
	//  decconstant("LC-LAMBDA-LIST-KEYWORDS

	deckeyword(key_optional, "&OPTIONAL");
	deckeyword(key_rest, "&REST");
	deckeyword(key_key, "&KEY");
	deckeyword(key_allow_other_keys, "&ALLOW-OTHER-KEYS");
	deckeyword(key_aux, "&AUX");
	deckeyword(key_body, "&BODY");
	deckeyword(key_whole, "&WHOLE");
	deckeyword(key_environment, "&ENVIRONMENT");
	deckeyword(key_int, ":INT");
	deckeyword(key_float, ":FLOAT");
	deckeyword(key_string, ":STRING");
	deckeyword(key_symbol, ":SYMBOL");
	deckeyword(key_color, ":COLOR");
	deckeyword(key_relative_to, ":RELATIVE-TO");
	deckeyword(key_classic, ":CLASSIC");
	deckeyword(key_min, ":MIN");
	deckeyword(key_max, ":MAX");
	deckeyword(key_position, ":POSITION");
	deckeyword(key_ambient, ":AMBIENT");
	deckeyword(key_diffuse, ":DIFFUSE");
	deckeyword(key_specular, ":SPECULAR");
	deckeyword(key_shininess, ":SHININESS");

	decconstant("LAMBDA-LIST-KEYWORDS", new cons_t(key_optional,
		new cons_t(key_rest,
			new cons_t(key_key,
				new cons_t(key_allow_other_keys,
					new cons_t(key_aux,
						new cons_t(key_body,
							new cons_t(key_whole,
								new cons_t(key_environment,
									nil)))))))));

}
