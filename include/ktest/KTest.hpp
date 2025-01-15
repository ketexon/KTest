#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <string>

#define TEST_CASE(message, test) TEST_CASE2(message, test, __COUNTER__)
#define TEST_CASE2(message, test, number) TEST_CASE3(message, test, number)
#define TEST_CASE3(message, test, number) static TestCase _testCase_##number(message, test);

namespace KTest {
	struct TestCaseAssertionFailed : public std::exception {
		TestCaseAssertionFailed() : std::exception() {}
		TestCaseAssertionFailed(const char* message) : std::exception(message) {}
	};

	struct TestCaseExpectedException : public TestCaseAssertionFailed {
	private:
		std::string message;

	public:
		TestCaseExpectedException(const char* m, std::string expected, std::string got) {
			message = m;
			message += "\nExpected: ";
			message += expected;
			message += "\nGot: ";
			message += got;
		}

		const char* what() const noexcept override {
			return message.c_str();
		}
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