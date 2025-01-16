#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <optional>

#define TEST_CASE(message, test_id) \
	void test_id(::KTest::TestContext&); \
	static ::KTest::TestCase _TestCase ## test_id(message, test_id, #test_id); \
	void test_id(::KTest::TestContext& test)

namespace KTest {
	struct TestCaseAssertionFailed : public std::runtime_error {
		TestCaseAssertionFailed(std::string message);
	};

	struct TestCaseExpectedException : public TestCaseAssertionFailed {
		TestCaseExpectedException(std::string m, std::string actual, std::string expected);
	};

	class TestContext {
	public:
		std::stringstream cout;

		void Assert(bool condition, std::string message);

		template<class T, class U>
		void Expect(bool condition, const T& actual, const U& expected, std::string message) {
			if (!condition) {
				std::stringstream actualSS, expectedSS;
				actualSS << actual;
				expectedSS << expected;
				throw TestCaseExpectedException(message, actualSS.str(), expectedSS.str());
			}
		}

		template<class T, class U>
		constexpr void AssertEq(const T& actual, const U& expected, std::string message) {
			Expect(actual == expected, actual, expected, message);
		}

		template<class T, class U>
		constexpr void AssertNeq(const T& actual, const U& expectedNot, std::string message) {
			Expect(
				!(actual == expectedNot),
				actual,
				(std::stringstream{} << "Not " << expectedNot).str(),
				message
			);
		}

		template<class T, class U>
		constexpr void AssertApprox(
			const T& actual,
			const U& expected,
			std::string message,
			decltype(std::abs(actual - expected)) epsilon = 1e-6f
		) {
			Expect(
				actual == expected || std::abs(actual - expected) < epsilon,
				actual, expected,
				message
			);
		}
	};

	struct TestCaseOptions {
		bool printOutputOnFailure = true;
		bool printOutputOnSuccess = false;
		bool printSuccessfulTestCases = true;
	};

	class TestCase {
	public:
		using TestFunction = std::function<void(TestContext&)>;

	private:
		TestFunction test;
		std::string name;
		std::string id;

	public:
		using TestFunction = std::function<void(TestContext&)>;

		TestCase(std::string name, TestFunction test, std::string id);
		bool Run(TestCaseOptions = {}) const;
		static void RunAll(TestCaseOptions = {});
		static void RunSome(std::vector<std::string>, TestCaseOptions = {});

		static size_t NumTests();
		static bool Exists(std::string id);
		static std::optional<const TestCase*> Get(std::string id);
		static const TestCase* Get(size_t i);

		constexpr const std::string& Name() const { return name; }
		constexpr const std::string& ID() const { return id; }
	};
}