/*
This logger file was created in the Compiler Construction project
by Sebastiaan Alvarez Rodriguez and Mariska IJpelaar
*/

#ifndef COCO_FRAMEWORK_GENERAL_LOGGER
#define COCO_FRAMEWORK_GENERAL_LOGGER

#include <ostream>
#include <streambuf>
#include <string>

#include "aecpp.h"

// Logger class to print errors, warnings and informational messages.
// It also keeps the number of messages printed in each category.
class Logger {
    private:
    std::ostream& info_stream;
    std::ostream& warning_stream;
    std::ostream& error_stream;
    uint64_t infos, warnings, errors;

    public:
    // Logger constructor, where info messages are printed to `info_stream`, warnings to `warning_stream`,
    // and errors to `error_stream`.
    Logger(std::ostream& info_stream, std::ostream& warning_stream, std::ostream& error_stream);

    // Print an informational message at `line`. If `line` is negative it is omitted.
    std::ostream& info(int line);

    // Print a warning at `line`. If `line` is negative it is omitted.
    std::ostream& warn(int line);

    // Print an error message at `line`. If `line` is negative it is omitted.
    std::ostream& error(int line);

    // Get the number of informational messages printed.
    uint64_t n_infos() const;

    // Get the number of warnings printed.
    uint64_t n_warnings() const;

    // Get the number of errors printed.
    uint64_t n_errors() const;

    private:
    // Print a generic tag to `stream`, in `color` with message `tag` at `line`. If `line` is negative, it is omitted.
    static void print_tag(std::ostream& stream, aec::Color, const std::string& tag, int line);
};

// A helper class for a streambuf that discards any messages written to it
class NullBuffer : public std::streambuf {
    public:
    int overflow(int c) override {
        return c;
    }
};

// A helper stream that discards any messages written to it.
class NullStream : public std::ostream {
    NullBuffer buf;

    public:
    NullStream() : std::ostream(&buf) {
    }
};

// Global NullStream, for ease of use.
extern NullStream NULL_STREAM;

#endif
