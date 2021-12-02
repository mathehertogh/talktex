/* C declarations */
%{

#include <iostream>
#include <cstring>

#include "syntax_tree.h"
#include "syntax_visitor.h"

/* Import from comp.l*/
#ifdef __cplusplus
extern "C" {
#endif

int yylex();   /* Lexer function*/
int yywrap(void);

#ifdef __cplusplus
}
#endif

// Shorthand for the grammar actions
using Con = Construction;

// Moves in [subtree] under [tree] and deletes [subtree].
static void move_in_subtree(Syntax_tree& tree, Syntax_tree* subtree) {
	tree.append_subtree(std::move(*subtree));
	delete subtree;
}

// Converts [c_string] to an std::string and deletes [c_string]
std::string move_to_string(char* c_string) {
	auto ret = std::string(c_string);
	free(c_string);
	return ret;
}

static void yyerror(Syntax_visitor&, const char*);

%}

/* Provides more useful error messages */
%define parse.error verbose

/* Types to pass between lexer, rules and actions.
   Note: bison has a variant option, which would alleviate the need to allocate and deallocate
   c-strings and Syntax_tree objects all the time. Can't get it to work though. */
%union {
	char letter;
	char* phrase;
	Syntax_tree* tree;
	Typesetting_type ts_type;
	Accent_type ac_type;
	Special_symbol_type ss_type;
	Unop_type u_type;
	Binop_type b_type;
	Rangeop_type r_type;
}

/* TODO?: Start symbol */
/* Note: if no start symbol is provided,
 	bison takes the first rule as start */
/* %start program */

/* Tokens */
/* symbol */
%token LETTER GREEK DIGIT
/* keywords */
%token OF FROM TO FUNCTION FRACTION OVER MAPS OPEN CLOSE PARENTHESIS END
/* operators */
%token MINUS NOT
/* typesettings */
%token TS_BOLD TS_CALL TS_FRAK
/* accents */
%token AC_TILDE AC_HAT AC_BAR
/* special symbols */
%token SS_EMPTY SS_INFTY
/* unary operators */
%token U_SQRT U_SIN U_COS U_TAN U_EXP U_LOG U_NEG U_FORALL U_EXISTS
/* binary operators */
%token B_PLUS B_TIMES B_POWER B_DIV B_MID B_EQ B_ISO B_LT B_GT B_LE B_GE B_AND B_OR B_IMPL B_EQUIV B_CUP B_CAP B_SMINUS B_SUBSET B_IN
/* range operators */
%token R_SUM R_PROD R_INTG
/* endfile */
%token ENDFILE 0

/* fixing shift/reduce conflict */
%right END NOEND

/*  If the token’s precedence is higher, the choice is to shift. If the rule’s precedence is higher, the choice is to reduce. If they have equal precedence, the choice is made based on the associativity of that precedence level. Each rule gets its precedence from the last terminal symbol mentioned in the components */
%right OF NOT B_PLUS B_TIMES B_POWER B_DIV B_MID B_EQ B_ISO B_LT B_GT B_LE B_GE B_AND B_OR B_IMPL B_EQUIV B_CUP B_CAP B_SMINUS B_SUBSET B_IN MINUS

%parse-param {Syntax_visitor& syntax_visitor}

%%

/* Grammar Rules and Actions */
start 			: anyexpr {
					syntax_visitor.syntax_tree = std::move(*$<tree>1);
					delete $<tree>1;
				};
anyexpr 		: openexpr %prec NOEND {
					$<tree>$ = $<tree>1;
				}
				| closedexpr %prec END {
					$<tree>$ = $<tree>1;
				}
openexpr		: expr %prec NOEND {
					$<tree>$ = $<tree>1;
				}
closedexpr 		: expr END {
					$<tree>$ = $<tree>1;
				}
expr 			: func {
					$<tree>$ = new Syntax_tree(Con::Type::Expr_func);
					move_in_subtree(*$<tree>$, $<tree>1);
				}
				| FRACTION openexpr OVER anyexpr {
					$<tree>$ = new Syntax_tree(Con::Type::Expr_frac);
					move_in_subtree(*$<tree>$, $<tree>2);
					move_in_subtree(*$<tree>$, $<tree>4);
				}
				| openexpr binop anyexpr  {
					$<tree>$ = new Syntax_tree(Con::Type::Expr_binop);
					move_in_subtree(*$<tree>$, $<tree>1);
					move_in_subtree(*$<tree>$, $<tree>2);
					move_in_subtree(*$<tree>$, $<tree>3);
				}
				| range_op range anyexpr {
					$<tree>$ = new Syntax_tree(Con::Type::Expr_binop);
					$<tree>$->append_subtree(Syntax_tree(Con::Type::Rangeop, $<r_type>1));
					move_in_subtree(*$<tree>$, $<tree>2);
					move_in_subtree(*$<tree>$, $<tree>3);
				}
				| OPEN PARENTHESIS openexpr CLOSE PARENTHESIS {
					$<tree>$ = new Syntax_tree(Con::Type::Expr_parentheses);
					move_in_subtree(*$<tree>$, $<tree>3);
				}
				| simpleexpr {
					$<tree>$ = $<tree>1;
				}
simpleexpr		: unop OF openexpr {
					$<tree>$ = new Syntax_tree(Con::Type::Expr_of);
					move_in_subtree(*$<tree>$, $<tree>1);
					move_in_subtree(*$<tree>$, $<tree>3);
				}
				| unop simpleexpr {
					$<tree>$ = new Syntax_tree(Con::Type::Expr_unop);
					move_in_subtree(*$<tree>$, $<tree>1);
					move_in_subtree(*$<tree>$, $<tree>2);
				}
				| symbol {
					$<tree>$ = new Syntax_tree(Con::Type::Expr_symbol);
					move_in_subtree(*$<tree>$, $<tree>1);
				}
symbol 			: DIGIT {
					$<tree>$ = new Syntax_tree(Con::Type::Symbol_digit);
					$<tree>$->append_subtree(Syntax_tree(
						Con::Type::Digit, move_to_string($<phrase>1)
					));
				}
				| variable_seq {
					$<tree>$ = new Syntax_tree(Con::Type::Symbol_variable);
					move_in_subtree(*$<tree>$, $<tree>1);
				}
				| special_symbol {
					$<tree>$ = new Syntax_tree(Con::Type::Symbol_special, $<ss_type>1);
				};
variable_seq	: variable_seq variable {
					move_in_subtree(*$<tree>$, $<tree>1);
					$<tree>$->append_subtree($<tree>2);
				}
				| variable {
					$<tree>$ = new Syntax_tree(Con(Con::Type::Variable_sequence));
					move_in_subtree(*$<tree>$, $<tree>1);
				};
variable 		: variable accent {
					$<tree>$ = new Syntax_tree(Con(Con::Type::Variable_accent));
					move_in_subtree(*$<tree>$, $<tree>1);
					$<tree>$->append_subtree(Syntax_tree(Con::Type::Accent, $<ac_type>2));
				}
				| typed_variable {
					$<tree>$ = $<tree>1;
				};
typed_variable	: letter {
					$<tree>$ = $<tree>1;
				}
				| typesetting typed_variable {
					$<tree>$ = new Syntax_tree(Con::Type::Variable_typesetting);
					$<tree>$->append_subtree(Syntax_tree(Con::Type::Typesetting, $<ts_type>1));
					move_in_subtree(*$<tree>$, $<tree>2);
				};
letter 			: LETTER {
					$<tree>$ = new Syntax_tree(Con(Con::Type::Letter, $<letter>1));
				}
				| GREEK {
					$<tree>$ = new Syntax_tree(Con::Type::Greek_symbol, move_to_string($<phrase>1));
				};
func 			: openfunc mapsto {
					$<tree>$ = new Syntax_tree(Con::Type::Func_mapsto);
					move_in_subtree(*$<tree>$, $<tree>1);
					move_in_subtree(*$<tree>$, $<tree>2);
				}
				| openfunc {
					$<tree>$ = new Syntax_tree(Con::Type::Func);
					move_in_subtree(*$<tree>$, $<tree>1);
				};
openfunc 		: FUNCTION variable FROM symbol TO symbol {
					$<tree>$ = new Syntax_tree(Con::Type::Openfunc);
					move_in_subtree(*$<tree>$, $<tree>2);
					move_in_subtree(*$<tree>$, $<tree>4);
					move_in_subtree(*$<tree>$, $<tree>6);
				};
mapsto 			: MAPS openexpr TO anyexpr {
					$<tree>$ = new Syntax_tree(Con::Type::Mapsto);
					move_in_subtree(*$<tree>$, $<tree>2);
					move_in_subtree(*$<tree>$, $<tree>4);
				};
unop 			: unary_op {
					$<tree>$ = new Syntax_tree(Con::Type::Unop, $<u_type>1);
				};
binop 			: binary_op  {
					$<tree>$ = new Syntax_tree(Con::Type::Binop, $<b_type>1);
				}
				| NOT binary_op  {
					$<tree>$ = new Syntax_tree(Con::Type::Binop_negated);
					$<tree>$->append_subtree(Syntax_tree(Con::Type::Binop, $<b_type>2));
				};
range 			: FROM openexpr TO anyexpr {
					$<tree>$ = new Syntax_tree(Con(Con::Type::Range));
					move_in_subtree(*$<tree>$, $<tree>2);
					move_in_subtree(*$<tree>$, $<tree>4);
				}
typesetting 	: TS_BOLD {
					$<ts_type>$ = Typesetting_type::Bold;
				}
				| TS_CALL {
					$<ts_type>$ = Typesetting_type::Calligraphic;
				}
				| TS_FRAK {
					$<ts_type>$ = Typesetting_type::Fraktur;
				};
accent 			: AC_TILDE {
					$<ac_type>$ = Accent_type::Tilde;
				}
				| AC_HAT {
					$<ac_type>$ = Accent_type::Hat;
				}
				| AC_BAR {
					$<ac_type>$ = Accent_type::Bar;
				};
special_symbol 	: SS_EMPTY {
					$<ss_type>$ = Special_symbol_type::Empty_set;
				}
				| SS_INFTY {
					$<ss_type>$ = Special_symbol_type::Infinity;
				};
unary_op 		: U_SQRT {
					$<u_type>$ = Unop_type::Square_root;
				}
				| U_SIN {
					$<u_type>$ = Unop_type::Sin;
				}
				| U_COS {
					$<u_type>$ = Unop_type::Cos;
				}
				| U_TAN {
					$<u_type>$ = Unop_type::Tan;
				}
				| U_EXP {
					$<u_type>$ = Unop_type::Exp;
				}
				| U_LOG {
					$<u_type>$ = Unop_type::Log;
				}
				| U_NEG {
					$<u_type>$ = Unop_type::Negate;
				}
				| U_FORALL {
					$<u_type>$ = Unop_type::For_all;
				}
				| U_EXISTS {
					$<u_type>$ = Unop_type::Exists;
				}
				| MINUS {
					$<u_type>$ = Unop_type::Minus;
				}
binary_op 		: B_PLUS {
					$<b_type>$ = Binop_type::Plus;
				}
				| MINUS {
					$<b_type>$ = Binop_type::Minus;
				}
				| B_TIMES {
					$<b_type>$ = Binop_type::Times;
				}
				| B_POWER {
					$<b_type>$ = Binop_type::Power;
				}
				| B_DIV {
					$<b_type>$ = Binop_type::Divided_by;
				}
				| B_MID {
					$<b_type>$ = Binop_type::Divides;
				}
				| B_EQ {
					$<b_type>$ = Binop_type::Equal;
				}
				| B_ISO {
					$<b_type>$ = Binop_type::Isomorphic;
				}
				| B_LT {
					$<b_type>$ = Binop_type::Less;
				}
				| B_GT {
					$<b_type>$ = Binop_type::Greater;
				}
				| B_LE {
					$<b_type>$ = Binop_type::Less_equal;
				}
				| B_GE {
					$<b_type>$ = Binop_type::Greater_equal;
				}
				| B_AND {
					$<b_type>$ = Binop_type::And;
				}
				| B_OR {
					$<b_type>$ = Binop_type::Or;
				}
				| B_IMPL {
					$<b_type>$ = Binop_type::Implies;
				}
				| B_EQUIV {
					$<b_type>$ = Binop_type::Equivalent;
				}
				| B_CUP {
					$<b_type>$ = Binop_type::Union;
				}
				| B_CAP {
					$<b_type>$ = Binop_type::Intersection;
				}
				| B_SMINUS {
					$<b_type>$ = Binop_type::Set_minus;
				}
				| B_SUBSET {
					$<b_type>$ = Binop_type::Subset;
				}
				| B_IN {
					$<b_type>$ = Binop_type::In;
				};
range_op 		: R_SUM {
					$<r_type>$ = Rangeop_type::Sum;
				}
				| R_PROD {
					$<r_type>$ = Rangeop_type::Product;
				}
				| R_INTG {
					$<r_type>$ = Rangeop_type::Integral;
				};
%%

static void yyerror(Syntax_visitor& vis, const char* s) {
    (&vis)->logger.error(-1) << s << '\n';
}

int yywrap() {
    return 1;
}
