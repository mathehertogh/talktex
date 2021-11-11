#include <iostream>

#include <grammar.h>
#include <syntaxvisitor.h>

const char* tests[] = 	{"b", "caligraphic b", "b tilde", "bold b hat", "capital a", "bold capital a hat", 
						"alpha", "alpha tilde", "4", "empty set", "infinity", "fraction alpha over beta", 
						"sin of capital d", "for all alpha tilde", "minus x", "function capital f from capital a to capital b", 
						"function capital f from capital a to capital b maps alpha to beta"};

int main() {
	Logger logger(std::cerr, std::cerr, std::cerr);
	SyntaxVisitor vis(logger);

	for (const char* test : tests)
		grammar::generate_from_string(test, vis);
	return 0;
}