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

// TODO: temporary functions that help us print now, but must be replaced by visitor-functions
// static char* ourformat(const char*, const char*);
// static char* ourformat(const char*, const char*, const char*);
// static char* texify(const char*);
// static char* parenthesis(const char*);
// static char* concat(const char*, const char*, const char*);
// static char* char_to_string(char);
// static char* scope(const char*);

static void yyerror(Syntax_visitor&, const char*);

%}

// %code requires {
// 	class Syntax_tree;
// }

/* Provides more useful error messages */
%define parse.error verbose

/* Types to pass between lexer, rules and actions*/
%union {
	char letter;
	char* phrase;
	Syntax_tree* tree;
}

/* TODO?: Start symbol */
/* Note: if no start symbol is provided,
 	bison takes the first rule as start */
/* %start program */

/* Tokens */
/* symbol */
%token LETTER TYPESETTING ACCENT GREEK DIGIT SYMBOL
/* keywords */
%token OF FROM TO FUNCTION FRACTION OVER MAPS OPEN CLOSE PARENTHESIS END
/* operators */
%token UNOP MINUS BINOP NOT RANGEOP
/* endfile */
%token ENDFILE 0

/* fixing shift/reduce conflict */
%right END NOEND

/*  If the token’s precedence is higher, the choice is to shift. If the rule’s precedence is higher, the choice is to reduce. If they have equal precedence, the choice is made based on the associativity of that precedence level. Each rule gets its precedence from the last terminal symbol mentioned in the components */
%right OF NOT BINOP

%parse-param {Syntax_visitor& syntax_visitor}

%%

/* TODO: memory managament! Note: bison has a variant option, can't get it to work though. */
/* TODO: build actual SyntaxTree */
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
variable 		: variable ACCENT {
					$<tree>$ = new Syntax_tree(Con(Con::Type::Variable_accent));
					move_in_subtree(*$<tree>$, $<tree>1);
					// Should be <accent_type> instead of <phrase> if the lexer provides it
					$<tree>$->append_subtree(Syntax_tree(
						Con::Type::Accent, move_to_string($<phrase>2)
					));
				}
				| typed_variable {
					$<tree>$ = $<tree>1;
				};
typed_variable	: letter {
					$<tree>$ = $<tree>1;
				}
				| TYPESETTING typed_variable {
					$<tree>$ = new Syntax_tree(Con::Type::Variable_typesetting);
					// Should be <typesetting_type> instead of <phrase> if the lexer provides it
					$<tree>$->append_subtree(Syntax_tree(
						Con::Type::Accent, move_to_string($<phrase>1)
					));
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
					// Should be Unop_type::Negate instead of '-' if we start using those
					$<tree>$ = new Syntax_tree(Con::Type::Unop, '-');
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

%%

/*
//TODO: tmp until we have actual token processing :)
static char* ourformat(const char* a, const char* b) {
	size_t lena = strlen(a);
	size_t lenb = strlen(b);

	char* c = (char*)malloc(lena + lenb +4);
	strcpy(c, "\\");
	strcat(c, a);
	strcat(c, "{");
	strcat(c, b);
	strcat(c, "}");

	return c;
}

//TODO: tmp until we have actual token processing :)
static char* ourformat(const char* a, const char* b, const char* c) {
	size_t lena = strlen(a);
	size_t lenb = strlen(b);
	size_t lenc = strlen(c);

	char* d = (char*)malloc(lena + lenb + lenc +6);
	strcpy(d, "\\");
	strcat(d, a);
	strcat(d, "{");
	strcat(d, b);
	strcat(d, "}");
	strcat(d, "{");
	strcat(d, c);
	strcat(d, "}");

	return d;
}

//TODO: tmp until we have actual token processing :)
static char* texify(const char* a) {
	size_t lena = strlen(a);
	char* b = (char*)malloc(lena + 2);

	strcpy(b, "\\");
	strcat(b, a);

	return b;
}

//TODO: tmp until we have actual token processing :)
static char* parenthesis(const char* a) {
	size_t lena = strlen(a);
	char* b = (char*)malloc(lena+16);

	strcpy(b, "\\left( ");
	strcat(b, a);
	strcat(b, " \\right)");

	return b;
}

//TODO: tmp until we have actual token processing :)
static char* concat(const char* a, const char* delim, const char* b) {
	size_t lena = strlen(a);
	size_t lenb = strlen(b);
	size_t delimlen = strlen(delim);

	char* c = (char*)malloc(lena+lenb+delimlen+1);

	strcpy(c, a);
	strcat(c, delim);
	strcat(c, b);

	return c;
}

//TODO: tmp until we have actual token processing :)
static char* char_to_string(char c) {
	char* character = (char*)malloc(2*sizeof(char));
	character[0] = c;
	character[1] = '\0';
	return character;
}

//TODO: tmp until we have actual token processing :)
static char* scope(const char* a) {
	size_t lena = strlen(a);
	char* b = (char*)malloc(lena+3);

	strcpy(b, "{");
	strcat(b, a);
	strcat(b, "}");

	return b;
}
*/

static void yyerror(Syntax_visitor& vis, const char* s) {
    (&vis)->logger.error(-1) << s << '\n';
}

int yywrap() {
    return 1;
}
