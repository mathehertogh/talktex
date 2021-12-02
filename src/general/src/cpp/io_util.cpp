#include "io_util.h"

#include <iostream>
#include "aec_styles.h"

static void print_open_file_error_message(const std::string& path) {
	std::cerr << aec_style::error << "Error:" << aec::reset << " Could not open "
	          << aec_style::filename << path << aec::reset << "\n";
}

bool try_open_input_file(const std::string& path, std::ifstream& out_ifstream) {
	out_ifstream.open(path);
	if (!out_ifstream.is_open()) {
		print_open_file_error_message(path);
		return false;
	}
	return true;
}

bool try_open_output_file(const std::string& path, std::ofstream& out_ofstream) {
	out_ofstream.open(path);
	if (!out_ofstream.is_open()) {
		print_open_file_error_message(path);
		return false;
	}
	return true;
}
