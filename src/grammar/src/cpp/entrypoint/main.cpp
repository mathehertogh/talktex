#include <grammar.h>

int main() {
	std::string test("b");
	int parseResult = grammar::generate_from_string(test);
	return parseResult;
}