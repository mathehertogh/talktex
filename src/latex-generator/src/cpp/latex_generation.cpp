#include "latex_generation.h"

#include <map>

//==================================================================================================
// Helper functions and constants
//==================================================================================================

std::string command(const std::string& name) {
	return "\\" + name;
}

const std::string ERROR_TARGET =
	command(" ") + command("text{Internal TalkTeX Error}") + command(" ");

std::string parenthesize(const std::string& inner) {
	return command("left(") + inner + command("right)");
}

std::string brace(const std::string& inner) {
	return "{" + inner + "}";
}

std::string texify_rangeop(Rangeop_type type) {
	static const std::map<Rangeop_type, std::string> map{
		{Rangeop_type::Sum,             command("sum")},
		{Rangeop_type::Product,         command("prod")},
		{Rangeop_type::Integral,        command("int")}
	};
	auto it = map.find(type);
	if (it != map.end()) return it->second;
	return ERROR_TARGET;
}

std::string texify_binop(Binop_type type) {
	static const std::map<Binop_type, std::string> map{
		{Binop_type::Plus,          "+"},
		{Binop_type::Minus,         "-"},
		{Binop_type::Times,         command("cdot ")},
		{Binop_type::Power,         "^"},
		{Binop_type::Divided_by,    "/"},
		{Binop_type::Divides,       command("mid ")},
		{Binop_type::Equal,         "="},
		{Binop_type::Isomorphic,    command("cong ")},
		{Binop_type::Less,          "<"},
		{Binop_type::Greater,       ">"},
		{Binop_type::Less_equal,    command("leq ")},
		{Binop_type::Greater_equal, command("geq ")},
		{Binop_type::And,           command("wedge ")},
		{Binop_type::Or,            command("vee ")},
		{Binop_type::Implies,       command("implies ")},
		{Binop_type::Equivalent,    command("Leftrightarrow ")},
		{Binop_type::Union,         command("cup ")},
		{Binop_type::Intersection,  command("cap ")},
		{Binop_type::Set_minus,     command("setminus ")},
		{Binop_type::Subset,        command("subset ")},
		{Binop_type::In,            command("in ")}
	};
	auto it = map.find(type);
	if (it != map.end()) return it->second;
	return ERROR_TARGET;
}

std::string texify_unop(Unop_type type) {
	static const std::map<Unop_type, std::string> map{
		{Unop_type::Square_root, command("sqrt ")},
		{Unop_type::Sin,         command("sin ")},
		{Unop_type::Cos,         command("cos ")},
		{Unop_type::Tan,         command("tan ")},
		{Unop_type::Exp,         command("exp ")},
		{Unop_type::Log,         command("log ")},
		{Unop_type::Negate,      command("neg ")},
		{Unop_type::For_all,     command("forall ")},
		{Unop_type::Exists,      command("exists ")},
		{Unop_type::Minus,       "-"}
	};
	auto it = map.find(type);
	if (it != map.end()) return it->second;
	return ERROR_TARGET;
}

std::string texify_special_symbol(Special_symbol_type type) {
	static const std::map<Special_symbol_type, std::string> map{
		{Special_symbol_type::Empty_set, command("emptyset")},
		{Special_symbol_type::Infinity,  command("infty")}
	};
	auto it = map.find(type);
	if (it != map.end()) return it->second;
	return ERROR_TARGET;
}

std::string texify_accent(Accent_type type) {
	static const std::map<Accent_type, std::string> map{
		{Accent_type::Tilde, command("tilde")},
		{Accent_type::Hat,   command("hat")},
		{Accent_type::Bar,   command("bar")}
	};
	auto it = map.find(type);
	if (it != map.end()) return it->second;
	return ERROR_TARGET;
}

std::string texify_typesetting(Typesetting_type type) {
	static const std::map<Typesetting_type, std::string> map{
		{Typesetting_type::Bold,         command("mathbb")},
		{Typesetting_type::Calligraphic, command("mathcal")},
		{Typesetting_type::Fraktur,      command("mathfrak")}
	};
	auto it = map.find(type);
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
// Public function implementations
//==================================================================================================

std::string to_latex(Syntax_tree::const_traverser t) {
	/// Shorthand to convert a child subtree to latex, since it's used so much.
	auto child              = [&](size_t index){return to_latex(t.child(index));};

	// Shorthands to get a specific type of data from the node that t is pointing at:

	auto get_typesetting    = [&](){return std::get<Typesetting_type>   (*t->data);};
	auto get_accent         = [&](){return std::get<Accent_type>        (*t->data);};
	auto get_special_symbol = [&](){return std::get<Special_symbol_type>(*t->data);};
	auto get_unary          = [&](){return std::get<Unop_type>          (*t->data);};
	auto get_binary         = [&](){return std::get<Binop_type>         (*t->data);};
	auto get_range          = [&](){return std::get<Rangeop_type>       (*t->data);};
	auto get_string         = [&](){return std::get<std::string>        (*t->data);};
	auto get_char           = [&](){return std::get<char>               (*t->data);};

	switch (t->type) {
	case Construction::Type::Expr_parentheses:
		return parenthesize(child(0));
	case Construction::Type::Expr_range:
		return child(0) + child(1) + child(2);
	case Construction::Type::Expr_binop:
		return child(0) + child(1) + child(2);
	case Construction::Type::Expr_unop:
		return child(0) + child(1);
	case Construction::Type::Expr_of:
		return child(0) + parenthesize(child(1));
	case Construction::Type::Expr_func:
		return child(0);
	case Construction::Type::Expr_frac:
		return command("frac") + brace(child(0)) + brace(child(1));
	case Construction::Type::Expr_symbol:
		return child(0);
	case Construction::Type::Func:
		return child(0);
	case Construction::Type::Func_mapsto:
		return child(0) + ", " + child(1);
	case Construction::Type::Mapsto:
		return child(0) + " " + command("mapsto ") + child(1);
	case Construction::Type::Openfunc:
		return child(0) + ": " + child(1) + " " + command("to ") + child(2);
	case Construction::Type::Range:
		return "_" + brace(child(0)) + "^" + brace(child(1));
	case Construction::Type::Rangeop:
		return texify_rangeop(get_range());
	case Construction::Type::Binop:
		return texify_binop(get_binary());
	case Construction::Type::Binop_negated:
		return command("not ") + child(0);
	case Construction::Type::Unop:
		return texify_unop(get_unary());
	case Construction::Type::Symbol_variable:
		return child(0);
	case Construction::Type::Symbol_digit:
		return child(0);
	case Construction::Type::Symbol_special:
		return texify_special_symbol(get_special_symbol());
	case Construction::Type::Variable_accent:
		return child(1) + brace(child(0));
	case Construction::Type::Variable_typesetting:
		return child(0) + brace(child(1));
	case Construction::Type::Variable_letter:
		return child(0);
	case Construction::Type::Accent:
		return texify_accent(get_accent());
	case Construction::Type::Typesetting:
		return texify_typesetting(get_typesetting());
	case Construction::Type::Letter:
		return std::string(1, get_char()); // Why is there no constructor for just a char??
	case Construction::Type::Greek_symbol:
		return texify_greek_symbol(get_string());
	case Construction::Type::Digit:
		return texify_digit(get_string());
	}
	return ERROR_TARGET; // Invalid type, should never happen
}


std::string to_display_style(const std::string& latex_expression) {
	return command("[") + " " + latex_expression + " " + command("]");
}

/** Returns the TalkTeX LaTeX header, including \begin{document} */
std::string talktex_header() {
	static const std::string header =
R"latex(% This file was generated by TalkTeX

\documentclass[12pt, english, DIV=9]{scrartcl}
\usepackage{talktex}

\begin{document}

\begin{centering}
	\Huge \talktex
\end{centering}

)latex";
	return header;
}

/** Returns the TalkTeX LaTeX footer, including \end{document} */
std::string talktex_footer() {
	static const std::string footer =
R"latex(
\end{document}
)latex";
	return footer;
}
