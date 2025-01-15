#include <ktest/KTest.hpp>

int main(){
	std::cout << "Running " << KTest::TestCase::NumTests() << " tests" << std::endl;
	KTest::TestCase::RunAll();
	std::cout << "MSVC Version: " << _MSVC_LANG << std::endl;
	return 0;
}