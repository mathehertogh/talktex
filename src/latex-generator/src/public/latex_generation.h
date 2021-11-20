/*
	This should be in the latex-generator, but I don't understand meson...
*/

#pragma once

#include <string>
#include "syntax_tree.h"

std::string to_latex(Syntax_tree::const_traverser t);
