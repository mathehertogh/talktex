#include <iostream>
#include <string>

#include "grammar.h"
#include "syntax_tree.h"
#include "syntax_visitor.h"
#include "aecpp.h"
#include "avds/tree/tree_output.h"
#include "latex_generation.h"

const std::string SEPARATOR = "\n" + std::string(140, '=') + "\n\n";

const char* tests[] = 	{"b", "calligraphic b", "b tilde", "bold b hat", "capital a", "bold capital a hat",
						"alpha", "alpha tilde", "four", "empty set", "infinity", "fraction alpha over beta",
						"sin of capital d", "for all alpha tilde", "minus x", "function capital f from capital a to capital b",
						"function capital f from capital a to capital b maps alpha to beta", "a plus b", "sin of cos of a",
						"function f from capital a to capital b maps for all x to sin of fraction x over alpha",
						"fraction b over c plus d", "fraction b over c end plus d", "a not equal b",
						"sum from x equal zero to infinity x power two", "open parenthesis x plus two close parenthesis"};

int main() {
	Logger logger(std::cerr, std::cerr, std::cerr);
	Syntax_visitor vis(logger);

	for (const char* test : tests) {
		std::cerr << SEPARATOR
		          << "Input: " << aec::bold + aec::green << test << aec::reset << "\n"
		          << "Parse tree:\n\n";

		grammar::generate_from_string(test, vis);

		avds::tree::print_horizontal(std::cerr, vis.syntax_tree.entrance());

		std::cerr << "\nLaTeX: " << to_latex(vis.syntax_tree.entrance()) << "\n";
	}
	std::cerr << SEPARATOR;

	std::cerr << "You might want to make your terminal fullscreen :)\n";

	return 0;
}
