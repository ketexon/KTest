#include <ktest/KTest.hpp>
#include <iostream>

int Factorial(int n){
	return n <= 1 ? 1 : n * Factorial(n - 1);
}

TEST_CASE("Factorial of 0 is 1", Factorial0) {
	KTest::TestCase::AssertEq(Factorial(0), 1, "Factorial of 0 is 1");
}

TEST_CASE("Factorial of 1 is 1", Factorial1) {
	KTest::TestCase::AssertEq(Factorial(1), 1, "Factorial of 1 is 1");
}

TEST_CASE("Factorial of 2 is 2", Factorial2) {
	KTest::TestCase::AssertEq(Factorial(2), 2, "Factorial of 2 is 2");
}

TEST_CASE("Factorial of 3 is 6", Factorial3) {
	KTest::TestCase::AssertEq(Factorial(3), 6, "Factorial of 3 is 6");
}