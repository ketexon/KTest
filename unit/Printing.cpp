#include <ktest/KTest.hpp>

TEST_CASE("Should not print any output", NoOutput) {
	test.cout << "SHOULD NOT PRINT" << std::endl;
}

TEST_CASE("[SHOULD FAIL] Should print output on failure", Output) {
	test.cout << "SHOULD PRINT" << std::endl;
	test.Assert(false, "[SHOULD FAIL]");
	test.cout << "SHOULD NOT PRINT" << std::endl;
}