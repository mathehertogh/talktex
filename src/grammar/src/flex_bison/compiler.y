/* C declarations */
%{
#include <iostream>

/* Import from comp.l*/
#ifdef __cplusplus
extern "C" {
#endif

int yylex();   /* Lexer function*/
int yywrap(void);

#ifdef __cplusplus
}
#endif

static void yyerror(const char*);


%}

/* Provides more useful error messages */
%define parse.error verbose

/* Types to pass between lexer, rules and actions*/
%union {
	char letter;
}

/* TODO?: Start symbol */
/* %start program */

/* Tokens */
/* basics */
%token LETTER
/* endfile */
%token ENDFILE 0

%%

/* Grammar Rules and Actions */
letter		: LETTER {
				std::cout << "letter: " << $<letter>$;
			};

%% 

static void yyerror(const char* s) {
    //TODO: something with logger eventually
    std::cout << "Oh no, an error: " << s << std::endl;
}

int yywrap() {
    return 1;
}
