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

int grammar::generate_from_file(
        const std::string& filename, Syntax_visitor& syntax_visitor
) {
    FILE* file = std::fopen(filename.c_str(), "r");
    if (!file) {
        std::cerr << "Could not open " << filename << std::endl;
        return -1;
    }
    yyin = file;
    int parsed = yyparse(syntax_visitor);
    fclose(file);
    yylex_destroy();
    return parsed;
}
