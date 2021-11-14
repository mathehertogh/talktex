/*
This logger file was created in the Compiler Construction project
by Sebastiaan Alvarez Rodriguez and Mariska IJpelaar

This version is slightly modified
*/

#include <ostream>
#include <string>

#include "logger.h"

NullStream NULL_STREAM;

Logger::Logger(std::ostream& info_stream, std::ostream& warning_stream, std::ostream& error_stream) : info_stream(info_stream), warning_stream(warning_stream), error_stream(error_stream), infos(0), warnings(0), errors(0) {
}

std::ostream& Logger::info(int line) {
    ++infos;
    print_tag(info_stream, aec::blue, "Info: ", line);
    return info_stream;
}

std::ostream& Logger::warn(int line) {
    ++warnings;
    print_tag(warning_stream, aec::yellow, "Warning: ", line);
    return warning_stream;
}

std::ostream& Logger::error(int line) {
    ++errors;
    print_tag(error_stream, aec::red, "Error: ", line);
    return error_stream;
}

uint64_t Logger::n_infos() const {
    return infos;
}

uint64_t Logger::n_warnings() const {
    return warnings;
}

uint64_t Logger::n_errors() const {
    return errors;
}

void Logger::print_tag(std::ostream& stream, aec::Color color, const std::string& tag, int line) {
    stream << aec::bold + color << tag;

    if (line >= 0) {
        stream << aec::creset << "line " << line << ": ";
    }

    stream << aec::reset;
}
