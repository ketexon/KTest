#include <ktest/KTest.hpp>
#include <iostream>

struct CustomType {
	int a, b;

	bool operator==(const CustomType& other) const {
		return a == other.a && b == other.b;
	}
};

std::ostream& operator<<(std::ostream& os, const CustomType& x)
{
    os << "CustomType { " << x.a << ", " << x.b << " }";
    return os;
}

TEST_CASE("Custom type equality", CustomTypeEquality) {
	KTest::TestCase::AssertEq(CustomType{1, 2}, CustomType{1, 2}, "Custom type equality");
}

TEST_CASE("[SHOULD FAIL] Custom type inequality printing", CustomTypeInequality) {
	KTest::TestCase::AssertEq(CustomType{1, 2}, CustomType{3, 4}, "Custom type equality");
}