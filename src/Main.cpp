#include <ktest/KTest.hpp>

int main(){
	std::cout << "Running " << KTest::TestCase::NumTests() << " tests" << std::endl;
	KTest::TestCase::RunAll();
	return 0;
}