#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <stdexcept>

#define TEST_CASE(message, test) void test(); TEST_CASE2(message, test, __COUNTER__); void test()
#define TEST_CASE2(message, test, number) TEST_CASE3(message, test, number)
#define TEST_CASE3(message, test, number) static TestCase _testCase_##number(message, test)

namespace KTest {
	struct TestCaseAssertionFailed : public std::runtime_error {
		TestCaseAssertionFailed(const char* message) : std::runtime_error(message) {}
	};

	struct TestCaseExpectedException : public TestCaseAssertionFailed {
	private:
		static const char* MakeMessage(const char* m, std::string expected, std::string got) {
			std::string message = m;
			message += "\nExpected: ";
			message += expected;
			message += "\nGot: ";
			message += got;
			return m;
		}

	public:
		TestCaseExpectedException(const char* m, std::string expected, std::string got)
			: TestCaseAssertionFailed(MakeMessage(m, expected, got))
		{}
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
		static constexpr void AssertEq(T a, U b, const char* message) {
			using std::to_string;
			if (a != b) {
				throw TestCaseExpectedException(message, to_string(a), to_string(b));
			}
		}

		static size_t NumTests();
	};
}