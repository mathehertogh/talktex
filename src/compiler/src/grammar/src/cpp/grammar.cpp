#include "grammar.h"

int yyparse(Syntax_visitor& syntax_visitor);
int yy_scan_string(const char* str);
int yylex_destroy();

extern FILE* yyin;

int grammar::generate_from_string(
	const std::string& input, Syntax_visitor& syntax_visitor
) {
	yy_scan_string(input.c_str());
	int parsed = yyparse(syntax_visitor);
	yylex_destroy(); // memory management
	return parsed;
}
