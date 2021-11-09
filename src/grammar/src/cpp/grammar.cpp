#include "grammar.h"

int yyparse();
int yy_scan_string(const char *str);
int yylex_destroy();

int grammar::generate_from_string(const std::string& input) {
    yy_scan_string(input.c_str());
    int parsed = yyparse();
    yylex_destroy(); // memory management
    return parsed;
}