#pragma once

#include <string>
#include <optional>
#include <variant>
#include <ostream>

//==================================================================================================
// Data enums
//==================================================================================================

enum class Typesetting_type {
	Bold,
	Calligraphic,
	Fraktur
};

enum class Accent_type {
	Tilde,
	Hat,
	Bar
};

enum class Special_symbol_type {
	Empty_set,
	Infinity
};

enum class Unop_type {
	Square_root,
	Sin,
	Cos,
	Tan,
	Exp,
	Log,
	Negate,
	For_all,
	Exists, 
	Minus
};

enum class Binop_type {
	Plus,
	Minus,
	Times,
	Power,
	Divided_by,
	Divides,
	Equal,
	Isomorphic,
	Less,
	Greater,
	Less_equal,
	Greater_equal,
	And,
	Or,
	Implies,
	Equivalent,
	Union,
	Intersection,
	Set_minus,
	Subset,
	In
};

enum class Rangeop_type {
	Sum,
	Product,
	Integral
};

//==================================================================================================
// Construction struct
//==================================================================================================

struct Construction {
	enum class Type {
		Expr_parentheses,
		Expr_range,
		Expr_binop,
		Expr_unop,
		Expr_of,
		Expr_func,
		Expr_frac,
		Expr_symbol,
		Func,
		Func_mapsto,
		Mapsto,
		Openfunc,
		Range,
		Rangeop,
		Binop,
		Binop_negated,
		Unop,
		Symbol_variable,
		Symbol_digit,
		Symbol_special,
		Variable_sequence,
		Variable_accent,
		Variable_typesetting,
		Variable_letter,
		Accent,
		Typesetting,
		Letter,
		Greek_symbol,
		Digit
	};
	using Data = std::variant<
		Typesetting_type,
		Accent_type,
		Special_symbol_type,
		Unop_type,
		Binop_type,
		Rangeop_type,
		std::string,
		char
	>;

	Type type;
	std::optional<Data> data;
	Construction(Type type = Type::Digit) : type(type) {}
	Construction(Type type, const Data& data) : type(type), data(data) {}
};

//==================================================================================================
// Print functions (for debugging)
//==================================================================================================

inline std::string to_string(Typesetting_type type) {
	switch (type) {
	case Typesetting_type::Bold: return "Bold";
	case Typesetting_type::Calligraphic: return "Calligraphic";
	case Typesetting_type::Fraktur: return "Fraktur";
	}
	return "Error";
}
inline std::ostream& operator<<(std::ostream& os, Typesetting_type type) {
	return os << to_string(type);
}

inline std::string to_string(Accent_type type) {
	switch (type) {
	case Accent_type::Tilde: return "Tilde";
	case Accent_type::Hat: return "Hat";
	case Accent_type::Bar: return "Bar";
	}
	return "Error";
}
inline std::ostream& operator<<(std::ostream& os, Accent_type type) {
	return os << to_string(type);
}

inline std::string to_string(Special_symbol_type type) {
	switch (type) {
	case Special_symbol_type::Empty_set: return "Empty_set";
	case Special_symbol_type::Infinity: return "Infinity";
	}
	return "Error";
}
inline std::ostream& operator<<(std::ostream& os, Special_symbol_type type) {
	return os << to_string(type);
}

inline std::string to_string(Unop_type type) {
	switch (type) {
	case Unop_type::Square_root: return "Square_root";
	case Unop_type::Sin: return "Sin";
	case Unop_type::Cos: return "Cos";
	case Unop_type::Tan: return "Tan";
	case Unop_type::Exp: return "Exp";
	case Unop_type::Log: return "Log";
	case Unop_type::Negate: return "Negate";
	case Unop_type::For_all: return "For_all";
	case Unop_type::Exists: return "Exists";
	case Unop_type::Minus: return "UMinus";
	}
	return "Error";
}
inline std::ostream& operator<<(std::ostream& os, Unop_type type) {
	return os << to_string(type);
}

inline std::string to_string(Binop_type type) {
	switch (type) {
	case Binop_type::Plus: return "Plus";
	case Binop_type::Minus: return "Minus";
	case Binop_type::Times: return "Times";
	case Binop_type::Power: return "Power";
	case Binop_type::Divided_by: return "Divided_by";
	case Binop_type::Divides: return "Divides";
	case Binop_type::Equal: return "Equal";
	case Binop_type::Isomorphic: return "Isomorphic";
	case Binop_type::Less: return "Less";
	case Binop_type::Greater: return "Greater";
	case Binop_type::Less_equal: return "Less_equal";
	case Binop_type::Greater_equal: return "Greater_equal";
	case Binop_type::And: return "And";
	case Binop_type::Or: return "Or";
	case Binop_type::Implies: return "Implies";
	case Binop_type::Equivalent: return "Equivalent";
	case Binop_type::Union: return "Union";
	case Binop_type::Intersection: return "Intersection";
	case Binop_type::Set_minus: return "Set_minus";
	case Binop_type::Subset: return "Subset";
	case Binop_type::In: return "In";
	}
	return "Error";
}
inline std::ostream& operator<<(std::ostream& os, Binop_type type) {
	return os << to_string(type);
}

inline std::string to_string(Rangeop_type type) {
	switch (type) {
	case Rangeop_type::Sum: return "Sum";
	case Rangeop_type::Product: return "Product";
	case Rangeop_type::Integral: return "Integral";
	}
	return "Error";
}
inline std::ostream& operator<<(std::ostream& os, Rangeop_type type) {
	return os << to_string(type);
}

inline std::string to_string(Construction::Type type) {
	switch (type) {
	case Construction::Type::Expr_parentheses: return "Expr_parentheses";
	case Construction::Type::Expr_range: return "Expr_range";
	case Construction::Type::Expr_binop: return "Expr_binop";
	case Construction::Type::Expr_unop: return "Expr_unop";
	case Construction::Type::Expr_of: return "Expr_of";
	case Construction::Type::Expr_func: return "Expr_func";
	case Construction::Type::Expr_frac: return "Expr_frac";
	case Construction::Type::Expr_symbol: return "Expr_symbol";
	case Construction::Type::Func: return "Func";
	case Construction::Type::Func_mapsto: return "Func_mapsto";
	case Construction::Type::Mapsto: return "Mapsto";
	case Construction::Type::Openfunc: return "Openfunc";
	case Construction::Type::Range: return "Range";
	case Construction::Type::Rangeop: return "Rangeop";
	case Construction::Type::Binop: return "Binop";
	case Construction::Type::Binop_negated: return "Binop_negated";
	case Construction::Type::Unop: return "Unop";
	case Construction::Type::Symbol_variable: return "Symbol_variable";
	case Construction::Type::Symbol_digit: return "Symbol_digit";
	case Construction::Type::Symbol_special: return "Symbol_special";
	case Construction::Type::Variable_sequence: return "Variable_sequence";
	case Construction::Type::Variable_accent: return "Variable_accent";
	case Construction::Type::Variable_typesetting: return "Variable_typesetting";
	case Construction::Type::Variable_letter: return "Variable_letter";
	case Construction::Type::Accent: return "Accent";
	case Construction::Type::Typesetting: return "Typesetting";
	case Construction::Type::Letter: return "Letter";
	case Construction::Type::Greek_symbol: return "Greek_symbol";
	case Construction::Type::Digit: return "Digit";
	}
	return "Error";
}
inline std::ostream& operator<<(std::ostream& os, Construction::Type type) {
	return os << to_string(type);
}

inline std::ostream& operator<<(std::ostream& os, const Construction& construction) {
	os << construction.type;

	if (construction.data.has_value()) {
		os << "(";
		std::visit([&os](const auto& data){os << data;}, *construction.data);
		os << ")";
	}

	return os;
}
