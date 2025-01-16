#include <ktest/KTest.hpp>

struct CustomFloatingPoint {
	float Value;

	CustomFloatingPoint(float value) : Value(value) {}

	operator float() const {
		return Value;
	}

	CustomFloatingPoint operator+ (const CustomFloatingPoint& other) const {
		return CustomFloatingPoint{Value + other.Value};
	}
};

TEST_CASE("Approx works with floats", ApproxFloat) {
	float a = 1.f/7;
	float b = 2.f/7;
	double c = a + b;

	test.AssertNeq(c, 3./7, "c should not be exactly 0.3/7");
	test.AssertApprox(c, 3./7, "c should be approximately 0.3/7");
}

TEST_CASE("Approx works with custom floats", ApproxCustomFloat) {
	CustomFloatingPoint a = 1.f/7;
	CustomFloatingPoint b = 2.f/7;
	CustomFloatingPoint c = a + b;

	test.AssertNeq(c, 3.f/7, "c should not be exactly 0.3f/7");
	test.AssertApprox(c, 3.f/7, "c should be approximately 0.3f/7");
}