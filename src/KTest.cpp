#include <ktest/KTest.hpp>

#include <memory>

std::unique_ptr<std::vector<KTest::TestCase>> testCases;

KTest::TestCase::TestCase(const char* name, std::function<void()> test) : test(test), name(name) {
	if(!testCases) {
		testCases = std::make_unique<std::vector<TestCase>>();
	}
	testCases->push_back(*this);
}

bool KTest::TestCase::Run() const {
	std::cout << "Running test case: " << name << std::endl;
	try {
		test();
	}
	catch (const TestCaseAssertionFailed& e) {
		std::cerr << "Test Case Failed: " << e.what() << std::endl;
		return false;
	}
	return true;
}

void KTest::TestCase::RunAll() {
	int nSuccess = 0;
	if(testCases){
		for (auto& testCase : *testCases) {
			bool res = testCase.Run();
			nSuccess += res;
		}
	}
	std::cout << "Passed: " << nSuccess << "/" << NumTests() << std::endl;
}

size_t KTest::TestCase::NumTests() {
	return testCases ? testCases->size() : 0;
}

KTest::TestCaseAssertionFailed::TestCaseAssertionFailed(std::string message) : std::runtime_error(message) {}

std::string MakeTestCaseExpectedExceptionMessage(std::string m, std::string actual, std::string expected) {
	std::string message = m;
	message += "\nActual: ";
	message += actual;
	message += "\nExpected: ";
	message += expected;
	return message;
}

KTest::TestCaseExpectedException::TestCaseExpectedException(std::string m, std::string actual, std::string expected)
	: TestCaseAssertionFailed(MakeTestCaseExpectedExceptionMessage(m, actual, expected))
{}