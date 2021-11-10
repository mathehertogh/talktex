/* Let Flex know we will not use yyunput and input functions
	- unput: fabricate input text to be scanned
	- input: directly read input
*/
%option nounput noinput
%{
/* lexeme of identifier or reserved word */
const int MAXTOKENLEN = 40;
char tokenString[MAXTOKENLEN+1];
%}


/* C declarations */
%{
#include <iostream>
#include "bison.compiler.h"

/* extern C declarations */
#if defined(__cplusplus)
extern "C" {
#endif

/* should be defined in stdio.h */
extern int fileno(FILE *);
int yylex(void);

#if defined(__cplusplus)
}
#endif

%}

letter 			[a-z]
capital  		"capital "[a-z]
whitespace 		[ \t]+
typesetting 	"bold"|"caligraphic"|"fraktur"
accent 			"tilde"|"hat"|"bar"

/* Patterns */
%%


{typesetting} { yylval.phrase = strdup(yytext); return TYPESETTING; }
{accent} { yylval.phrase = strdup(yytext); return ACCENT; }
{letter} { yylval.letter = yytext[0]; return LETTER; }
{capital} { yylval.letter = toupper(yytext[8]); return LETTER;}


{whitespace}    {/* skip whitespace */}
<<EOF>>         {return ENDFILE;}

%%