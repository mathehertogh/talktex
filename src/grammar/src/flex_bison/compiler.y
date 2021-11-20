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

/* Types to pass between lexer, rules and actions*/
%union {
	char letter;
	char* phrase;
	Syntax_tree* tree;
	Typesetting_type ts_type;
	Accent_type ac_type;
}

/* TODO?: Start symbol */
/* Note: if no start symbol is provided,
 	bison takes the first rule as start */
/* %start program */

/* Tokens */
/* symbol */
%token LETTER GREEK DIGIT SYMBOL
/* keywords */
%token OF FROM TO FUNCTION FRACTION OVER MAPS OPEN CLOSE PARENTHESIS END
/* operators */
%token UNOP MINUS BINOP NOT RANGEOP
/* typesettings */
%token TS_BOLD TS_CALL TS_FRAK
/* accents */
%token AC_TILDE AC_HAT AC_BAR
/* endfile */
%token ENDFILE 0

/* fixing shift/reduce conflict */
%right END NOEND

/*  If the token’s precedence is higher, the choice is to shift. If the rule’s precedence is higher, the choice is to reduce. If they have equal precedence, the choice is made based on the associativity of that precedence level. Each rule gets its precedence from the last terminal symbol mentioned in the components */
%right OF NOT BINOP

%parse-param {Syntax_visitor& syntax_visitor}

%%

/* TODO: memory managament! Note: bison has a variant option, can't get it to work though. */
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
				| openexpr binop anyexpr %prec BINOP {
					$<tree>$ = new Syntax_tree(Con::Type::Expr_binop);
					move_in_subtree(*$<tree>$, $<tree>1);
					move_in_subtree(*$<tree>$, $<tree>2);
					move_in_subtree(*$<tree>$, $<tree>3);
				}
				| RANGEOP range anyexpr {
					$<tree>$ = new Syntax_tree(Con::Type::Expr_binop);
					// Should be <rangeop_type> instead of <phrase> if the lexer provides it
					$<tree>$->append_subtree(Syntax_tree(
						Con::Type::Rangeop, move_to_string($<phrase>1)
					));
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
				| variable {
					$<tree>$ = new Syntax_tree(Con::Type::Symbol_variable);
					move_in_subtree(*$<tree>$, $<tree>1);
				}
				| SYMBOL {
					// Should be <special_symbol_type> instead of <phrase> if the lexer provides it
					$<tree>$ = new Syntax_tree(
						Con::Type::Symbol_special, move_to_string($<phrase>1)
					);
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
unop 			: UNOP {
					// Should be <unop_type> instead of <phrase> if the lexer provides it
					$<tree>$ = new Syntax_tree(Con::Type::Unop, move_to_string($<phrase>1));
				}
				| MINUS {
					// Should be Unop_type::Negate instead of "minus" if we start using those
					$<tree>$ = new Syntax_tree(Con::Type::Unop, "minus");
				};
binop 			: BINOP {
					// Should be <binop_type> instead of <phrase> if the lexer provides it
					$<tree>$ = new Syntax_tree(Con::Type::Binop, move_to_string($<phrase>1));
				}
				| NOT BINOP {
					$<tree>$ = new Syntax_tree(Con::Type::Binop_negated);
					// Should be <binop_type> instead of <phrase> if the lexer provides it
					$<tree>$->append_subtree(Syntax_tree(
						Con::Type::Binop, move_to_string($<phrase>2)
					));
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
%%

static void yyerror(Syntax_visitor& vis, const char* s) {
    (&vis)->logger.error(-1) << s << '\n';
}

int yywrap() {
    return 1;
}
