#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

#define TEST_CASE(message, test) void test(); static ::KTest::TestCase _TestCase##test(message, test); void test()

namespace KTest {
	struct TestCaseAssertionFailed : public std::runtime_error {
		TestCaseAssertionFailed(std::string message);
	};

	struct TestCaseExpectedException : public TestCaseAssertionFailed {
		TestCaseExpectedException(std::string m, std::string actual, std::string expected);
	};

	class TestCase {
		std::function<void()> test;
		const char* name;

	public:
		TestCase(const char* name, std::function<void()> test);
		bool Run() const;
		static void RunAll();

		static constexpr void Assert(bool condition, const char* message) {
			if (!condition) {
				throw TestCaseAssertionFailed(message);
			}
		}

		template<class T, class U>
		static constexpr void AssertEq(const T& actual, const U& expected, const char* message) {
			if (!(actual == expected)) {
				std::stringstream actualSS, expectedSS;
				actualSS << actual;
				expectedSS << expected;
				throw TestCaseExpectedException(message, actualSS.str(), expectedSS.str());
			}
		}

		static size_t NumTests();
	};
}