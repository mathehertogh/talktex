#pragma once

#include <string>
#include <fstream>

/**
 * Tries to open the file at [path] in read mode.
 * On success, returns true and sets [out_ifstream] to the created ifsteam.
 * On failure, returns false and logs an error message to stderr.
 */
bool try_open_input_file(const std::string& path, std::ifstream& out_ifstream);

/**
 * Tries to open the file at [path] in write mode.
 * On success, returns true and sets [out_ifstream] to the created ifsteam.
 * On failure, returns false and logs an error message to stderr.
 */
bool try_open_output_file(const std::string& path, std::ofstream& out_ofstream);
