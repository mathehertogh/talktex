/* generate functions for the whole process text --> SyntaxTree */

#ifndef GRAMMAR_H
#define GRAMMAR_H 

#include <string>
#include <syntaxvisitor.h>

namespace grammar {
	/**
	 * Generates [SyntaxTree] from an input string
	 * @param input the inputstring
	 * @return the returncode
	 */
	int generate_from_string(const std::string& input, SyntaxVisitor& visitor);
}

#endif