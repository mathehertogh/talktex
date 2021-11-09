#include <iostream>

#include <grammar.h>
#include <syntaxvisitor.h>

int main() {
	std::string test("b");
	Logger logger(std::cerr, std::cerr, std::cerr);
	SyntaxVisitor vis(logger);
	int parseResult = grammar::generate_from_string(test, vis);
	return parseResult;
}