/* C declarations */
%{
#include <iostream>

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

%}

%code requires {
	class SyntaxVisitor;
}

/* Provides more useful error messages */
%define parse.error verbose

/* Types to pass between lexer, rules and actions*/
%union {
	char letter;
}

/* TODO?: Start symbol */
/* Note: if no start symbol is provided, 
 	bison takes the first rule as start */
/* %start program */

/* Tokens */
/* basics */
%token LETTER
/* endfile */
%token ENDFILE 0

%parse-param {SyntaxVisitor& vis}

%%

/* Grammar Rules and Actions */
letter		: LETTER {
				std::cout << "letter: " << $<letter>$;
			};

%% 

static void yyerror(SyntaxVisitor& vis, const char* s) {
    (&vis)->logger.error(-1) << s << '\n';
}

int yywrap() {
    return 1;
}
