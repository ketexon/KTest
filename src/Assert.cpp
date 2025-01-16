#include <ktest/KTest.hpp>

void KTest::TestContext::Assert(bool condition, std::string message) {
	if (!condition) {
		throw TestCaseAssertionFailed(message);
	}
}