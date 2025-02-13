#include <ktest/KTest.hpp>
#include <string>
#include <filesystem>
#include <optional>
#include <map>
#include <algorithm>

static const char* greenText = "\x1b[0;32m";
static const char* clearStyle = "\x1b[0m";

enum class Flags : int {
	None = 0,
	Help = 1 << 0,
	List = 1 << 1,
	FailsOnly = 1 << 2,
	Print = 1 << 3,
	Silent = 1 << 4,
};

struct Options {
	Flags flags = Flags::None;
	std::vector<std::string> testCases;
};

std::map<std::string, char> longOptionMap = {
	{"help", 'h'},
	{"list", 'l'},
	{"fail-only", 'f'},
	{"print", 'p'},
	{"silent", 's'},
};

std::map<char, Flags> flagMap = {
	{'h', Flags::Help},
	{'l', Flags::List},
	{'f', Flags::FailsOnly},
	{'p', Flags::Print},
	{'s', Flags::Silent},
};

void Help(std::string exeName){
	std::cout << "Usage: " << exeName << " [options] [test cases]" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "  -h, --help: Show this help message" << std::endl;
	std::cout << "  -l, --list: List all test cases" << std::endl;
	std::cout << "  -f, --fail-only: Only print failed test cases (default: off)" << std::endl;
	std::cout << "  -p, --print: Print output for successful test cases (default: off)" << std::endl;
	std::cout << "  -s, --silent: Do not print output for failed  test cases (default: off)" << std::endl;
}

std::optional<Options> ParseOptions(int argc, char* argv[]){
	Flags flags = Flags::None;
	std::vector<std::string> testCases;

	for(int i = 1; i < argc; i++){
		std::string arg = argv[i];
		std::string shortArgs;
		bool unknownFlag = false;
		// long flag
		if(arg.length() > 2 && arg[0] == '-' && arg[1] == '-'){
			// get short version
			if(auto it = longOptionMap.find(arg.substr(2)); it != longOptionMap.end()){
				shortArgs = it->second;
			}
			else {
				unknownFlag = true;
			}
		}
		// short flag
		else if(arg.length() > 1 && arg[0] == '-'){
			shortArgs = arg.substr(1);
		}
		// test case
		else {
			if(!KTest::TestCase::Exists(arg)){
				std::cerr << "Unknown test case: " << arg << std::endl;
				return std::nullopt;
			}
			testCases.push_back(arg);
			continue;
		}

		// parse all short flags
		for(char shortArg : shortArgs){
			// convert it to a flag
			if(auto it = flagMap.find(shortArg); it != flagMap.end()){
				flags = static_cast<Flags>(static_cast<int>(flags) | static_cast<int>(it->second));
			}
			else {
				unknownFlag = true;
				break;
			}
		}

		if(unknownFlag) {
			std::cerr << "Unknown option: " << arg << std::endl;
			return std::nullopt;
		}
	}

	return Options { flags, testCases };
}

int main(int argc, char* argv[]) {
	const auto options = ParseOptions(argc, argv);
	const auto doHelp = [&]{
		std::filesystem::path exePath = argv[0];
		std::string exeName = exePath.filename().string();
		Help(exeName);
		return 1;
	};
	if(!options){
		doHelp();
		return 1;
	}

	int flags = static_cast<int>(options->flags);
	if(flags & static_cast<int>(Flags::Help)){
		doHelp();
		return 0;
	}

	if(flags & static_cast<int>(Flags::List)){
		std::vector<const KTest::TestCase*> testCases;
		for(int i = 0; i < KTest::TestCase::NumTests(); i++){
			auto testCase = KTest::TestCase::Get(i);
			testCases.push_back(testCase);
		}
		std::sort(
			testCases.begin(), testCases.end(),
			[](const KTest::TestCase* a, const KTest::TestCase* b){
				return a->ID() < b->ID();
			}
		);
		for(auto testCase : testCases){
			std::cout
				<< greenText << testCase->ID() << clearStyle
				<< ": " << testCase->Name() << std::endl;
		}
		return 0;
	}

	KTest::TestCaseOptions testCaseOptions;

	if(flags & static_cast<int>(Flags::FailsOnly)){
		testCaseOptions.printSuccessfulTestCases = false;
	}
	if(flags & static_cast<int>(Flags::Print)){
		testCaseOptions.printOutputOnSuccess = true;
	}
	if(flags & static_cast<int>(Flags::Silent)){
		testCaseOptions.printOutputOnFailure = false;
	}

	if(options->testCases.size() > 0){
		std::cout << "Running " << options->testCases.size() << " tests" << std::endl;
		KTest::TestCase::RunSome(options->testCases, testCaseOptions);
		return 0;
	}
	else {
		std::cout << "Running " << KTest::TestCase::NumTests() << " tests" << std::endl;
		KTest::TestCase::RunAll(testCaseOptions);
		return 0;
	}
}
