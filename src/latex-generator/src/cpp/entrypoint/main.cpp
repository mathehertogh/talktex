#include <iostream>
#include <string>

#include <tclap/CmdLine.h>

#include "grammar.h"
#include "syntax_tree.h"
#include "syntax_visitor.h"
#include "aec_styles.h"
#include "latex_generation.h"
#include "io_util.h"

// =================================================================================================
// C library API
// =================================================================================================

/**
 * Converts one or more lines of running text to corresponding LaTeX code, which can be used in
 * LaTeX text mode. One output line is generated for each input line.
 *
 * The running text should be given in [input]. The resulting LaTeX code is stored in *[output].
 * The pointers [input], *[output] and [output] should be freed by the caller.
 *
 * Returns false if one of the lines could not be fully parsed, and true otherwise.
 * The partial texifications of any erronous input lines, and the texifications of the lines after
 * any erronous input lines, are still included in the output.
 */
extern "C" bool texify(char* input, char** output) {
	bool success = true;
	Logger logger(std::cerr, std::cerr, std::cerr);
	Syntax_visitor visitor(logger);
	std::stringstream ss(input);
	std::string line;
	std::string output_string;
	while (std::getline(ss, line)) {
		auto code = grammar::generate_from_string(line, visitor);
		if (code != 0) success = false;
		auto latex = generation::to_latex(visitor.syntax_tree.entrance());
		output_string += generation::to_display_style(latex) + "\n";
	}
	*output = strdup(output_string.c_str());
	return success;
}

/**
 * Returns the TalkTeX LaTeX header, including \begin{document}.
 * The returned string should be freed by the caller.
 */
extern "C" const char* talktex_header() {
	return strdup(generation::talktex_header().c_str());
}

/**
 * Returns the TalkTeX LaTeX footer, including \end{document}.
 * The returned string should be freed by the caller.
 */
extern "C" const char* talktex_footer() {
	return strdup(generation::talktex_footer().c_str());
}

// =================================================================================================
// Command-line interface
// =================================================================================================

const std::string SEPARATOR = "\n" + std::string(100, '=') + "\n\n";

const char* tests[] = {
	"b", "calligraphic b", "b tilde", "bold b hat", "capital a", "bold capital a hat",
	"alpha", "alpha tilde", "four", "empty set", "infinity", "fraction alpha over beta",
	"sin of capital d", "for all alpha tilde", "minus x", "function capital f from capital a to capital b",
	"function capital f from capital a to capital b maps alpha to beta", "a plus b", "sin of cos of a",
	"function f from capital a to capital b maps for all x to sin of fraction x over alpha",
	"fraction b over c plus d", "fraction b over c end plus d", "a not equal b",
	"sum from x equal zero to infinity x power two", "open parenthesis x plus two close parenthesis"
};

bool convert_and_print(
	Syntax_visitor& visitor, std::istream& is, bool create_document, bool verbose
) {
	bool success = true;
	std::string line;
	while (std::getline(is, line)) {
		if (verbose) std::cerr << "Input: " << aec_style::input << line << aec::reset << "\n";

		auto code = grammar::generate_from_string(line, visitor);
		if (code != 0) success = false;

		if (verbose) std::cerr << "LaTeX: ";
		auto latex = generation::to_latex(visitor.syntax_tree.entrance());
		std::cout << (create_document ? generation::to_display_style(latex) : latex ) << "\n";

		if (verbose) {
			std::cerr << SEPARATOR;
		}
	}
	return success;
}

bool  convert_and_print(
	Syntax_visitor& visitor, const std::string& str, bool create_document, bool verbose
) {
	std::stringstream ss(str);
	return convert_and_print(visitor, ss, create_document, verbose);
}

int main(int argc, char** argv) {
	bool success = true;

	TCLAP::CmdLine cmd("TalkTex compiler - LaTeX generator", ' ', "1.0");

	try {
		TCLAP::ValueArg<std::string> input_file_path_arg("f", "file", "Path to source file.", false, "", "string");
		TCLAP::ValueArg<std::string> input_arg("i", "input", "Input string to parse.", false, "", "string");
		TCLAP::SwitchArg test_switch("t", "tests", "Perform tests", false);
		TCLAP::SwitchArg create_document_switch("d", "create-document", "Create a full LaTeX document.", cmd, false);
		TCLAP::SwitchArg verbose_switch("v", "verbose", "Show verbose output", cmd, false);

		TCLAP::OneOf inputs;
		inputs.add(input_file_path_arg).add(input_arg).add(test_switch);
		cmd.add(inputs);
		cmd.parse(argc, argv);

		Logger logger(std::cerr, std::cerr, std::cerr);
		Syntax_visitor vis(logger);

		bool create_document = create_document_switch.getValue();
		bool verbose = verbose_switch.getValue();

		if (verbose)
			std::cerr << SEPARATOR;

		if (create_document) {
			if (verbose) {
				std::cerr << "Header:\n\n";
			}
			std::cout << talktex_header();
			if (verbose) {
				std::cerr << SEPARATOR;
			}
		}

		if (test_switch.isSet()) {
			for (const char* test : tests) {
				if (!convert_and_print(vis, test, create_document, verbose)) success = false;
			}
		} else if (input_file_path_arg.isSet()) {
			const std::string& path = input_file_path_arg.getValue();
			std::ifstream file;
			if (try_open_input_file(path, file)) {
				if (!convert_and_print(vis, file, create_document, verbose)) success = false;
			}
			else if (verbose) {
				std::cerr << SEPARATOR;
			}
		} else if (input_arg.isSet()) {
			const std::string& input = input_arg.getValue();
			if (!convert_and_print(vis, input, create_document, verbose)) success = false;
		}

		if (create_document) {
			if (verbose) {
				std::cerr << "Footer:\n\n";
			}
			std::cout << talktex_footer();
			if (verbose) {
				std::cerr << SEPARATOR;
			}
		}

	} catch (TCLAP::ArgException& e) {
		std::cerr << aec_style::error << "command-line error: " << aec::reset << e.error()
		          << " for arg " << e.argId() << std::endl;
		return 1;
	}

	return (success ? 0 : 1);
}
