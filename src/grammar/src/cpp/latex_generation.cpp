#include "latex_generation.h"

#include <map>

//==================================================================================================
// Helper functions and constants
//==================================================================================================

std::string ERROR_TARGET = "\\ \\text{Internal TalkTeX Error}\\ ";

std::string parenthesize(const std::string& inner) {
	return "\\left(" + inner + "\\right)";
}

std::string brace(const std::string& inner) {
	return "{" + inner + "}";
}

std::string command(const std::string& name) {
	return "\\" + name;
}

// TODO: All of these texify functions should really be done by the lexer, so that we can use
//       the various enums in Construction.h here instead.

std::string texify_rangeop(const std::string& str) {
	return command(str);
}

std::string texify_binop(const std::string& str) {
	// TODO
	return "[texified " + str + "]";
}

std::string texify_unop(const std::string& str) {
	// TODO
	return "[texified " + str + "]";
}

std::string texify_special_symbol(const std::string& str) {
	static const std::map<std::string, std::string> map{
		{"empty set", "\\emptyset"},
		{"infinity",  "\\infty"}
	};
	auto it = map.find(str);
	if (it != map.end()) return it->second;
	return ERROR_TARGET;
}

std::string texify_accent(const std::string& str) {
	static const std::map<std::string, std::string> map{
		{"tilde", "\\tilde"},
		{"hat",   "\\hat"},
		{"bar",   "\\bar"}
	};
	auto it = map.find(str);
	if (it != map.end()) return it->second;
	return ERROR_TARGET;
}

std::string texify_typesetting(const std::string& str) {
	static const std::map<std::string, std::string> map{
		{"bold",         "\\mathbb" },
		{"calligraphic", "\\mathcal"},
		{"fraktur",      "\\mathfrak"}
	};
	auto it = map.find(str);
	if (it != map.end()) return it->second;
	return ERROR_TARGET;
}

std::string texify_greek_symbol(const std::string& str) {
	return command(str); // I'm pretty sure this always holds
}

std::string texify_digit(const std::string& str) {
	static const std::map<std::string, std::string> map{
		{"zero",  "0"},
		{"one",   "1"},
		{"two",   "2"},
		{"three", "3"},
		{"four",  "4"},
		{"five",  "5"},
		{"six",   "6"},
		{"seven", "7"},
		{"eight", "8"},
		{"nine",  "9"}
	};
	auto it = map.find(str);
	if (it != map.end()) return it->second;
	return ERROR_TARGET;
}

//==================================================================================================
// to_latex definition
//==================================================================================================

std::string to_latex(Syntax_tree::const_traverser t) {
	/// Shorthand to convert a child subtree to latex, since it's used so much.
	auto child = [&](size_t index){return to_latex(t.child(index));};

	/// Shorthand to get the string data in the node that t is pointing at.
	auto get_string = [&](){return std::get<std::string>(*t->data);};
	/// Shorthand to get the char data in the node that t is pointing at.
	auto get_char   = [&](){return std::get<char>       (*t->data);};

	switch (t->type) {
	case Construction::Type::Expr_parentheses:
		return parenthesize(child(0));
	case Construction::Type::Expr_range:
		return child(0) + child(1) + " " + child(2);
	case Construction::Type::Expr_binop:
		return child(0) + " " + child(1) + " " + child(2);
	case Construction::Type::Expr_unop:
		return child(0) + " " + child(1);
	case Construction::Type::Expr_of:
		return child(0) + parenthesize(child(1));
	case Construction::Type::Expr_func:
		return child(0);
	case Construction::Type::Expr_frac:
		return child(0);
	case Construction::Type::Expr_symbol:
		return child(0);
	case Construction::Type::Func:
		return child(0);
	case Construction::Type::Func_mapsto:
		return child(0) + ", " + child(1);
	case Construction::Type::Mapsto:
		return child(0) + " \\mapsto " + child(1);
	case Construction::Type::Openfunc:
		return child(0) + ": " + child(1) + " \\to " + child(2);
	case Construction::Type::Frac:
		return "\\frac{" + child(0) + "}{" + child(1) + "}";
	case Construction::Type::Range:
		return "_{" + child(0) + "}^{" + child(1);
	case Construction::Type::Rangeop:
		return texify_rangeop(get_string());
	case Construction::Type::Binop:
		return texify_binop(get_string());
	case Construction::Type::Binop_negated:
		return "\\not " + child(0);
	case Construction::Type::Unop:
		return texify_unop(get_string());
	case Construction::Type::Symbol_variable:
		return child(0);
	case Construction::Type::Symbol_digit:
		return child(0);
	case Construction::Type::Symbol_special:
		return texify_special_symbol(get_string());
	case Construction::Type::Variable_accent:
		return child(1) + brace(child(0));
	case Construction::Type::Variable_typesetting:
		return child(0) + brace(child(1));
	case Construction::Type::Variable_letter:
		return child(0);
	case Construction::Type::Accent:
		return texify_accent(get_string());
	case Construction::Type::Typesetting:
		return texify_typesetting(get_string());
	case Construction::Type::Letter:
		return std::string(1, get_char()); // Why is there no constructor for just a char??
	case Construction::Type::Greek_symbol:
		return texify_greek_symbol(get_string());
	case Construction::Type::Digit:
		return texify_digit(get_string());
	}
	return ERROR_TARGET; // Invalid type, should never happen
}
