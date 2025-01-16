#include <ktest/KTest.hpp>

#include <memory>
#include <map>

static std::unique_ptr<std::vector<KTest::TestCase>> testCases;
static std::unique_ptr<std::map<std::string, KTest::TestCase*>> testCaseIDMap;

static const char* redText = "\x1b[0;31m";
static const char* redBoldText = "\x1b[1;31m";
static const char* greenText = "\x1b[0;32m";
static const char* yellowText = "\x1b[0;33m";
static const char* clearStyle = "\x1b[0m";


KTest::TestCase::TestCase(std::string name, TestFunction test, std::string id)
	: test(test)
	, name(name)
	, id(id)
{
	if(!testCases) {
		testCases = std::make_unique<std::vector<TestCase>>();
	}
	testCases->push_back(*this);
}

bool KTest::TestCase::Run(KTest::TestCaseOptions options) const {
	TestContext ctx;
	bool passed = true;
	std::string failMessage;
	try {
		test(ctx);
	}
	catch (const TestCaseAssertionFailed& e) {
		passed = false;
		failMessage = e.what();
	}
	catch(const std::exception& e) {
		passed = false;
		std::stringstream ss;
		ss << "Uncaught exception: " << e.what();
	}
	// we want to catch all exceptions, as otherwise
	// we would get no error message
	catch(...) {
		passed = false;
		failMessage = "Uncaught exception (unknown type)";
	}

	bool shouldPrint = passed && options.printSuccessfulTestCases || !passed;
	bool shouldPrintOutput = shouldPrint && (
		passed && options.printOutputOnSuccess
		|| !passed && options.printOutputOnFailure
	);

	if(shouldPrint){
		std::cout << "Running test case " << id << ": " << name << "...";
		if(passed){
			std::cout << greenText << " PASSED" << clearStyle << std::endl;
		}
		else {
			std::cout
				<< redBoldText << " FAILED\n" << clearStyle
				<< redText << "Reason: " << clearStyle << failMessage
				<< std::endl;
		}
	}

	if(shouldPrintOutput){
		const auto& output = ctx.cout.str();
		const char* headerFooterColor = passed ? greenText : redText;
		if(output.length() > 0){
			const char* outputHeader = "=====   OUTPUT   =====";
			const char* outputFooter = "===== END OUTPUT =====";
			std::cout << headerFooterColor << outputHeader << '\n'
				<< yellowText << output << clearStyle
				<< headerFooterColor << outputFooter << clearStyle << '\n' << std::endl;
		}
	}
	return passed;
}

static void PrintSuccess(size_t nSuccess, size_t nTests) {
	const char* successStyle = nSuccess == nTests ? greenText : yellowText;
	std::cout << "Passed: "
		<< successStyle << nSuccess << clearStyle << "/"
		<< greenText << nTests << clearStyle << std::endl;

}

void KTest::TestCase::RunAll(TestCaseOptions options) {
	int nSuccess = 0;
	if(testCases){
		for (auto& testCase : *testCases) {
			bool res = testCase.Run(options);
			nSuccess += res;
		}
	}
	PrintSuccess(nSuccess, testCases->size());
}

void KTest::TestCase::RunSome(std::vector<std::string> ids, TestCaseOptions options) {
	int nSuccess = 0;
	int nFound = 0;
	if(testCases){
		for (const auto& id : ids) {
			const auto testCase = Get(id);
			if(!testCase.has_value()) {
				std::cerr << redText << "Test case `" << id << "' not found" << clearStyle << std::endl;
				continue;
			}
			++nFound;
			bool res = (*testCase)->Run(options);
			nSuccess += res;
		}
	}
	PrintSuccess(nSuccess, nFound);
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

bool MakeTestCaseIDMap(){
	if(!testCases) {
		return false;
	}
	if(!testCaseIDMap) {
		testCaseIDMap = std::make_unique<std::map<std::string, KTest::TestCase*>>();
		for (auto& testCase : *testCases) {
			testCaseIDMap->insert({testCase.ID(), &testCase});
		}
	}
	return true;
}

bool KTest::TestCase::Exists(std::string id) {
	return Get(id).has_value();
}

std::optional<const KTest::TestCase*> KTest::TestCase::Get(std::string id) {
	if(!MakeTestCaseIDMap()) {
		return std::nullopt;
	}
	auto it = testCaseIDMap->find(id);
	return it != testCaseIDMap->end()
		? std::optional<const KTest::TestCase*>(it->second)
		: std::nullopt;
}

const KTest::TestCase* KTest::TestCase::Get(size_t i) {
	return &(*testCases).at(i);
}