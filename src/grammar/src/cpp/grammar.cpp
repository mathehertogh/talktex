#include "grammar.h"

int yyparse(SyntaxVisitor&);
int yy_scan_string(const char* str);
int yylex_destroy();

int grammar::generate_from_string(const std::string& input, SyntaxVisitor& visitor) {
    yy_scan_string(input.c_str());
    int parsed = yyparse(visitor);
    yylex_destroy(); // memory management
    return parsed;
}