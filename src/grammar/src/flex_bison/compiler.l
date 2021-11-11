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
digit  			[0-9]
capital  		"capital "[a-z]
whitespace 		[ \t]+
typesetting 	"bold"|"caligraphic"|"fraktur"
accent 			"tilde"|"hat"|"bar"
greek  			"alpha"|"beta"|"gamma"|"delta"|"epsilon"|"zeta"|"eta"|"theta"|"iota"|"kappa"|"lambda"|"mu"|"nu"|"xi"|"pi"|"rho"|"sigma"|"tau"|"upsilon"|"phi"|"chi"|"psi"|"omega"
symbol 			"empty set"|"infinity"

/* Patterns */
%%
"of"			{ return OF; }
"from"			{ return FROM; }
"to"			{ return TO; }
"function" 		{ return FUNCTION; }
"fraction"		{ return FRACTION; }
"over"			{ return OVER; }
"maps"			{ return MAPS; }
"mapping"		{ return MAPPING; }
"open"   		{ return OPEN; }
"close"			{ return CLOSE; }
"parenthesis"	{ return PARENTHESIS; }
"end" 			{ return END; }

{typesetting} 	{ yylval.phrase = strdup(yytext); return TYPESETTING; }
{accent} 		{ yylval.phrase = strdup(yytext); return ACCENT; }
{letter} 		{ yylval.letter = yytext[0]; return LETTER; }
{digit}			{ yylval.digit = yytext[0]; return DIGIT; }
{capital} 		{ yylval.letter = toupper(yytext[8]); return LETTER; }
{greek}			{ yylval.phrase = strdup(yytext); return GREEK; }
{symbol}   		{ yylval.phrase = strdup(yytext); return SYMBOL; }


{whitespace}    {/* skip whitespace */}
<<EOF>>         {return ENDFILE;}

%%