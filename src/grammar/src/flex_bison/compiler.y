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
static char* ourformat(char*, char*);
static char* texify(char*);

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
}

/* TODO?: Start symbol */
/* Note: if no start symbol is provided, 
 	bison takes the first rule as start */
/* %start program */

/* Tokens */
/* variable */
%token LETTER TYPESETTING ACCENT GREEK
/* end */
%token END
/* endfile */
%token ENDFILE 0

%parse-param {SyntaxVisitor& vis}

%%

/* TODO: memory managament! */
/* Grammar Rules and Actions */
expr 			: openexpr end {
					std::cout << $<phrase>1 << "\n";
				};
end 			: END | %empty; /* either the `end` keyword or empty. In both cases: do nothing */
openexpr 		: variable { /* should become symbol */
					$<phrase>$ = $<phrase>1;
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
				}
%% 

static void yyerror(SyntaxVisitor& vis, const char* s) {
    (&vis)->logger.error(-1) << s << '\n';
}

//TODO: tmp until we have actual token processing :)
static char* ourformat(char* a, char* b) {
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
static char* texify(char* a) {
	size_t lena = strlen(a);
	char* b = (char*)malloc(lena + 2);

	strcpy(b, "\\");
	strcat(b, a);

	return b;
}

int yywrap() {
    return 1;
}
