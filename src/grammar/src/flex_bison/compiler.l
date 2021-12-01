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
#include "syntax_visitor.h"
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
digit  			"zero"|"one"|"two"|"three"|"four"|"five"|"six"|"seven"|"eight"|"nine"
capital  		"capital "[a-z]
whitespace 		[ \t]+
greek  			"alpha"|"beta"|"gamma"|"delta"|"epsilon"|"zeta"|"eta"|"theta"|"iota"|"kappa"|"lambda"|"mu"|"nu"|"xi"|"pi"|"rho"|"sigma"|"tau"|"upsilon"|"phi"|"chi"|"psi"|"omega"

%%
 /* Typesetting */
"bold" 			{ return TS_BOLD; }
"calligraphic" 	{ return TS_CALL; }
"fraktur"		{ return TS_FRAK; }

 /* Accent */
"tilde" 		{ return AC_TILDE; }
"hat"			{ return AC_HAT; }
"bar"			{ return AC_BAR; }

 /* Special Symbol */
"empty set" 	{ return SS_EMPTY; }
"infinity"		{ return SS_INFTY; }

 /* Unary Operators */
"square root" 	{ return U_SQRT; }
"sin"			{ return U_SIN; }
"cos"			{ return U_COS; }
"tan" 			{ return U_TAN; }
"exp"			{ return U_EXP; }
"log"			{ return U_LOG; }
"negate"		{ return U_NEG; }
"for all"		{ return U_FORALL; }
"exists"		{ return U_EXISTS; }

 /* Binary Operators */
"plus" 			{ return B_PLUS; }
"times"			{ return B_TIMES; }
"power"			{ return B_POWER; }
"divided by" 	{ return B_DIV; }
"divides"		{ return B_MID; }
"equals"		{ return B_EQ; }
"isomorphic"	{ return B_ISO; }
"less"			{ return B_LT; }
"greater"		{ return B_GT; }
"less equal"	{ return B_LE; }
"greater equal"	{ return B_GE; }
"and"			{ return B_AND; }
"or"			{ return B_OR; }
"implies"		{ return B_IMPL; }
"equivalent"	{ return B_EQUIV; }
"union" 		{ return B_CUP; }
"intersection"	{ return B_CAP; }
"set minus"		{ return B_SMINUS; }
"subset"		{ return B_SUBSET; }
"in"			{ return B_IN;}

 /* Range Operators */
"sum" 			{ return R_SUM; }
"product"		{ return R_PROD; }
"integral"		{ return R_INTG; }

 /* Patterns */
"of"			{ return OF; }
"from"			{ return FROM; }
"to"			{ return TO; }
"function" 		{ return FUNCTION; }
"fraction"		{ return FRACTION; }
"over"			{ return OVER; }
"maps"			{ return MAPS; }
"mapping"		{ return MAPS; }
"open"   		{ return OPEN; }
"close"			{ return CLOSE; }
"parenthesis"	{ return PARENTHESIS; }
"end" 			{ return END; }
"minus"			{ return MINUS; }
"not" 			{ return NOT; }

{letter} 		{ yylval.letter = yytext[0];          return LETTER;      }
{digit}			{ yylval.phrase = strdup(yytext);     return DIGIT;       }
{capital} 		{ yylval.letter = toupper(yytext[8]); return LETTER;      }
{greek}			{ yylval.phrase = strdup(yytext);     return GREEK;       }

{whitespace}    {/* skip whitespace */}
<<EOF>>         {return ENDFILE;}

%%
