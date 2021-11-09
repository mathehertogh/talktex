#include <iostream>

#include <grammar.h>
#include <syntaxvisitor.h>

int main() {
	std::string test1("b");
	std::string test2("caligraphic b");
	std::string test3("b tilde");
	std::string test4("bold b hat");
	Logger logger(std::cerr, std::cerr, std::cerr);
	SyntaxVisitor vis(logger);
	int parseResult = grammar::generate_from_string(test1, vis);
	grammar::generate_from_string(test2, vis);
	grammar::generate_from_string(test3, vis);
	grammar::generate_from_string(test4, vis);
	return parseResult;
}