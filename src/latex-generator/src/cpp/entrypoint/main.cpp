#include <iostream>
#include <string>

#include "grammar.h"
#include "syntax_tree.h"
#include "syntax_visitor.h"
#include "aecpp.h"
#include "avds/tree/tree_output.h"
#include "latex_generation.h"

#include <tclap/CmdLine.h>

const std::string SEPARATOR = "\n" + std::string(140, '=') + "\n\n";
const auto STYLE_INPUT = aec::bold + aec::green;
const auto STYLE_ERROR = aec::bold + aec::red;

const char* tests[] = {
	"b", "calligraphic b", "b tilde", "bold b hat", "capital a", "bold capital a hat",
	"alpha", "alpha tilde", "four", "empty set", "infinity", "fraction alpha over beta",
	"sin of capital d", "for all alpha tilde", "minus x", "function capital f from capital a to capital b",
	"function capital f from capital a to capital b maps alpha to beta", "a plus b", "sin of cos of a",
	"function f from capital a to capital b maps for all x to sin of fraction x over alpha",
	"fraction b over c plus d", "fraction b over c end plus d", "a not equal b",
	"sum from x equal zero to infinity x power two", "open parenthesis x plus two close parenthesis"
};

int main(int argc, char** argv) {
	TCLAP::CmdLine cmd("TalkTex compiler - LaTeX generator", ' ', "1.0");

	try {
		TCLAP::ValueArg<std::string> inputFilenameArg("f", "file", "Path to source file.", false, "", "string");
		TCLAP::ValueArg<std::string> inputArg("i", "input", "Input string to parse.", false, "", "string");
		TCLAP::SwitchArg testSwitch("t", "tests", "Perform tests", false);
		TCLAP::SwitchArg verboseSwitch("v", "verbose", "Show verbose output", cmd, false);

		TCLAP::OneOf inputs;
		inputs.add(inputFilenameArg).add(inputArg).add(testSwitch);
		cmd.add(inputs);
		cmd.parse(argc, argv);

		Logger logger(std::cerr, std::cerr, std::cerr);
		Syntax_visitor vis(logger);

		bool verbose = verboseSwitch.getValue();

		if (testSwitch.isSet()) {
			for (const char* test : tests) {
				if (verbose)
					std::cerr << SEPARATOR << "Input: " << STYLE_INPUT << test << aec::reset << "\n";
				grammar::generate_from_string(test, vis);
				if (verbose)
					std::cerr << "LaTeX: ";
				std::cerr << to_latex(vis.syntax_tree.entrance()) << "\n";
			}
		} else if (inputFilenameArg.isSet()) {
			const std::string& inputFilePath = inputFilenameArg.getValue();
			if (verbose)
				std::cerr << SEPARATOR << "File: " << STYLE_INPUT << inputFilePath << aec::reset << "\n";
			grammar::generate_from_file(inputFilePath, vis);
			if (verbose)
				std::cerr << "LaTeX: ";
			std::cerr << to_latex(vis.syntax_tree.entrance()) << "\n";
		} else if (inputArg.isSet()) {
			const std::string& input = inputArg.getValue();
			if (verbose)
				std::cerr << SEPARATOR << "Input: " << STYLE_INPUT << input << aec::reset << "\n";
			grammar::generate_from_string(input, vis);
			if (verbose)
				std::cerr << "LaTeX: ";
			std::cerr << to_latex(vis.syntax_tree.entrance()) << "\n";
		}

		if (verbose)
			std::cerr << SEPARATOR;

	} catch (TCLAP::ArgException& e) {
		std::cerr << STYLE_ERROR << "command-line error: " << aec::reset << e.error() << " for arg " << e.argId() << std::endl;
		return 1;
	}

	return 0;
}
