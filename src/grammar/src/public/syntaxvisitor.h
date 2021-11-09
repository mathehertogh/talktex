/* Class to build the SyntaxTree */

#ifndef SYNTAXVISITOR_H
#define SYNTAXVISITOR_H

#include <logger.h>

class SyntaxVisitor {
public:
	explicit SyntaxVisitor(Logger& logger) : logger(logger) {};

	// A reference to the logger
	// Required to print errors, warnings and info as needed
	Logger& logger;
	
};

#endif