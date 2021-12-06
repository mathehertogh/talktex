#include <iostream>
#include <string>
#include <sstream>

#include <tclap/CmdLine.h>

#include "avds/tree/tree_output.h"
#include "grammar.h"
#include "syntax_tree.h"
#include "syntax_visitor.h"
#include "aec_styles.h"
#include "io_util.h"

// =================================================================================================
// Command-line interface
// =================================================================================================

const std::string SEPARATOR = "\n" + std::string(140, '=') + "\n\n";

const char* tests[] = {
	"b", "calligraphic b", "b tilde", "bold b hat", "capital a", "bold capital a hat",
	"alpha", "alpha tilde", "four", "empty set", "infinity", "fraction alpha over beta",
	"sin of capital d", "for all alpha tilde", "minus x", "function capital f from capital a to capital b",
	"function capital f from capital a to capital b maps alpha to beta", "a plus b", "sin of cos of a",
	"function f from capital a to capital b maps for all x to sin of fraction x over alpha",
	"fraction b over c plus d", "fraction b over c end plus d", "a not equal b",
	"sum from x equal zero to infinity x power two", "open parenthesis x plus two close parenthesis"
};

bool parse_and_print(Syntax_visitor& visitor, std::istream& is) {
	bool success = true;
	std::string line;
	while (std::getline(is, line)) {
		std::cerr << "Input: " << aec_style::input << line << aec::reset << "\n";

		auto code = grammar::generate_from_string(line, visitor);
		if (code != 0) success = false;

		std::cerr << "Parse tree:\n\n";
		avds::tree::print_horizontal(std::cout, visitor.syntax_tree.entrance());

		std::cerr << SEPARATOR;
	}
	return success;
}

bool parse_and_print(Syntax_visitor& visitor, const std::string& str) {
	std::stringstream ss(str);
	return parse_and_print(visitor, ss);
}

int main(int argc, char** argv) {
	bool success = true;

	TCLAP::CmdLine cmd("TalkTex compiler - Grammar Parser", ' ', "1.0");

	try {
		TCLAP::ValueArg<std::string> input_file_path_arg("f", "file", "Path to source file.", false, "", "string");
		TCLAP::ValueArg<std::string> input_arg("i", "input", "Input string to parse.", false, "", "string");
		TCLAP::SwitchArg test_switch("t", "tests", "Perform tests", false);

		TCLAP::OneOf inputs;
		inputs.add(input_file_path_arg).add(input_arg).add(test_switch);
		cmd.add(inputs);
		cmd.parse(argc, argv);

		Logger logger(std::cerr, std::cerr, std::cerr);
		Syntax_visitor vis(logger);

		std::cerr << SEPARATOR;

		if (test_switch.isSet()) {
			for (const char* test : tests) {
				if (!parse_and_print(vis, test)) success = false;
			}
		}
		else if (input_file_path_arg.isSet()) {
			const std::string& path = input_file_path_arg.getValue();
			std::ifstream file;
			if (try_open_input_file(path, file)) {
				if (!parse_and_print(vis, file)) success = false;
			}
			else {
				std::cerr << SEPARATOR;
			}
		} else if (input_arg.isSet()) {
			const std::string& input = input_arg.getValue();
			if (!parse_and_print(vis, input)) success = false;
		}

	} catch (TCLAP::ArgException& e) {
		std::cerr << aec_style::error << "command-line error: " << aec::reset << e.error()
		          << " for arg " << e.argId() << std::endl;
		return 1;
	}

	std::cerr << "You might want to make your terminal fullscreen :)\n";

	return (success ? 0 : 1);
}
