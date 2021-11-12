/* C declarations */
%{
#include <iostream>
#include <cstring>

#include <syntaxvisitor.h>

/* Import from comp.l*/
#ifdef __cplusplus
extern "C" {
#endif

int yylex();   /* Lexer function*/
int yywrap(void);

#ifdef __cplusplus
}
#endif

static void yyerror(SyntaxVisitor&, const char*);
// TODO: temporary functions that help us print now, but must be replaced by visitor-functions
static char* ourformat(const char*, const char*);
static char* ourformat(const char*, const char*, const char*);
static char* texify(const char*);
static char* parenthesis(const char*);
static char* concat(const char*, const char*, const char*);
static char* char_to_string(char);
static char* scope(const char*);

%}

%code requires {
	class SyntaxVisitor;
}

/* Provides more useful error messages */
%define parse.error verbose

/* Types to pass between lexer, rules and actions*/
%union {
	char letter;
	char* phrase;
	char digit;
}

/* TODO?: Start symbol */
/* Note: if no start symbol is provided, 
 	bison takes the first rule as start */
/* %start program */

/* Tokens */
/* symbol */
%token LETTER TYPESETTING ACCENT GREEK DIGIT SYMBOL
/* keywords */
%token OF FROM TO FUNCTION FRACTION OVER MAPS MAPPING OPEN CLOSE PARENTHESIS END
/* operators */
%token UNOP MINUS BINOP
/* endfile */
%token ENDFILE 0


/*  If the token’s precedence is higher, the choice is to shift. If the rule’s precedence is higher, the choice is to reduce. If they have equal precedence, the choice is made based on the associativity of that precedence level. Each rule gets its precedence from the last terminal symbol mentioned in the components */
%right BINOP UNOP

/* fixing shift/reduce conflict */
%right NOEND END



/*%precedence TO OVER 
%right BINOP*/

%parse-param {SyntaxVisitor& vis}

%%

/* TODO: memory managament! */
/* TODO: build actual SyntaxTree */
/* Grammar Rules and Actions */
start 			: expr {
					std::cout << $<phrase>1 << "\n";
				};
expr 			: openexpr %prec NOEND {
					$<phrase>$ = $<phrase>1;
				}
				| openexpr END {
					$<phrase>$ = $<phrase>1;
				};
openexpr 		: func {
					$<phrase>$ = $<phrase>1;
				} 
				| frac {
					$<phrase>$ = $<phrase>1;
				}
				| expr binop expr {
					$<phrase>$ = concat(scope($<phrase>1), $<phrase>2, scope($<phrase>1));
				}
				| symbol {
					$<phrase>$ = $<phrase>1;
				} 
				| unop OF expr {
					$<phrase>$ = concat($<phrase>1, "", parenthesis($<phrase>3));
				}
				| unop expr {
					$<phrase>$ = concat($<phrase>1, "", $<phrase>2);
				};
symbol 			: DIGIT {
					$<phrase>$ = char_to_string($<digit>1);
				}
				| variable { 
					$<phrase>$ = $<phrase>1;
				}
				| SYMBOL {
					$<phrase>$ = texify($<phrase>1);
				};
variable 		: variable ACCENT {
					$<phrase>$ = ourformat($<phrase>2, $<phrase>1);
				} 
				| typed_variable {
					$<phrase>$ = $<phrase>1;
				};
typed_variable	: letter {
					$<phrase>$ = $<phrase>1;
				}
				| TYPESETTING typed_variable {
					$<phrase>$ = ourformat($<phrase>1, $<phrase>2);
				};
letter 			: LETTER {
					$<phrase>$ = char_to_string($<letter>1);
				}
				| GREEK {
					$<phrase>$ = texify($<phrase>1);
				};
frac 			: FRACTION expr OVER expr {
					$<phrase>$ = ourformat("frac", $<phrase>2, $<phrase>4);
				};
func 			: openfunc mapsto {
					$<phrase>$ = concat($<phrase>1, ", ", $<phrase>2);
				}
				| openfunc {
					$<phrase>$ = $<phrase>1;
				};
openfunc 		: FUNCTION variable FROM symbol TO symbol {
					$<phrase>$ = concat($<phrase>2, ": ", concat($<phrase>4, " \\to ", $<phrase>6));
				};
mapsto 			: MAPS expr TO expr {
					$<phrase>$ = concat($<phrase>2, " \\mapsto ", $<phrase>4);
				};
unop 			: UNOP { 
					$<phrase>$ = concat(texify($<phrase>1), "", " ");
				}
				| MINUS {
					$<phrase>$ = char_to_string('-');
				};
binop 			: BINOP {
					$<phrase>$ = texify($<phrase>1);
				};


%% 

static void yyerror(SyntaxVisitor& vis, const char* s) {
    (&vis)->logger.error(-1) << s << '\n';
}

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

int yywrap() {
    return 1;
}
