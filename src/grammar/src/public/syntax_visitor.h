/* Class to build the SyntaxTree */

#ifndef SYNTAXVISITOR_H
#define SYNTAXVISITOR_H

#include <logger.h>
#include <syntax_tree.h>

class Syntax_visitor {
public:
	explicit Syntax_visitor(Logger& logger) : logger(logger) {};

	// A reference to the logger
	// Required to print errors, warnings and info as needed
	Logger& logger;

	Syntax_tree syntax_tree;
};

#endif
