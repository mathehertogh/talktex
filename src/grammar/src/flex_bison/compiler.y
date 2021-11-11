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
%token UNOP MINUS
/* endfile */
%token ENDFILE 0

%parse-param {SyntaxVisitor& vis}

%%

/* TODO: memory managament! */
/* TODO: build actual SyntaxTree */
/* Grammar Rules and Actions */
start 			: expr {
					std::cout << $<phrase>1 << "\n";
				};
expr 			: openexpr end {
					$<phrase>$ = $<phrase>1;
				};
end 			: END | %empty; /* either the `end` keyword or empty. In both cases: do nothing */
openexpr 		: symbol { 
					$<phrase>$ = $<phrase>1;
				}
				| frac {
					$<phrase>$ = $<phrase>1;
				}
				| unop OF expr {
					$<phrase>$ = concat($<phrase>1, "", parenthesis($<phrase>3));
				}
				| unop expr {
					$<phrase>$ = concat($<phrase>1, " ", $<phrase>2);
				};
symbol 			: DIGIT {
					$<phrase>$ = new char($<digit>1);
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
					$<phrase>$ = new char($<letter>1);
				}
				| GREEK {
					$<phrase>$ = texify($<phrase>1);
				};
frac 			: FRACTION expr OVER expr {
					$<phrase>$ = ourformat("frac", $<phrase>2, $<phrase>4);
				};
unop 			: UNOP { 
					$<phrase>$ = texify($<phrase>1);
				}
				| MINUS {
					$<phrase>$ = new char('-');
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

int yywrap() {
    return 1;
}
