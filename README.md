# KTest

Ketexon's small testing library written in C++17.

## Installation

KTest does not require any dependencies other than the C++ standard library.

KTest is best included using CMake's `FetchContent`.

```cmake
include(FetchContent)

FetchContent_Declare(
    KTest
    GIT_URL https://github.com/ketexon/KTest
)

FetchContent_MakeAvailable(KTest)

target_link_libraries(
    <your_target>
    PRIVATE KTestWithMain
)
```

This automatically defines a main function. If you want to have your own main function, you can link against `KTest` instead. See [usage](#manually-running-test-cases) for more details.

## Usage

### Basic Usage

If linking with `KTestWithMain`, then you can create test cases like this

```cpp
#include <ktest/KTest.hpp>

int Factorial(int n){
	return n <= 1 ? 1 : n * Factorial(n - 1);
}

TEST_CASE("Factorial of 0 is 1", Factorial0) {
	test.AssertEq(Factorial(0), 1, "Factorial of 0 is 1");
}

TEST_CASE("Factorial of 1 is 1", Factorial1) {
	test.AssertEq(Factorial(1), 1, "Factorial of 1 is 1");
}

TEST_CASE("Factorial of 2 is 2", Factorial2) {
	test.AssertEq(Factorial(2), 2, "Factorial of 2 is 2");
}

TEST_CASE("Factorial of 3 is 6", Factorial3) {
	test.AssertEq(Factorial(3), 6, "Factorial of 3 is 6");
}
```

This creates a CLI that allows you to run test cases. You can use the `-h` flag to see the available options.

```bash
./your_program
Running 4 tests
Running test case Factorial0: Factorial of 0 is 1... PASSED
Running test case Factorial1: Factorial of 1 is 1... PASSED
Running test case Factorial2: Factorial of 2 is 2... PASSED
Running test case Factorial3: Factorial of 3 is 6... PASSED
Passed: 4/4
```

You can cherry-pick test cases by passing the *identifier* as a positional argument:

```bash
./your_program Factorial0 Factorial2
Running 2 tests
Running test case Factorial0: Factorial of 0 is 1... PASSED
Running test case Factorial2: Factorial of 2 is 2... PASSED
Passed: 2/2
```

You can also list test cases using `-l`

```bash
./your_program -l
Factorial0: Factorial of 0 is 1
Factorial1: Factorial of 1 is 1
Factorial2: Factorial of 2 is 2
Factorial3: Factorial of 3 is 6
```

### TestCase internals

Each test case is defined using the `TestCase` class, which is a wrapper over an `std::function<void(KTest::TestContext&)>`. 

The `TestContext` class gives access to the various test methods, like `Assert`.

On construction, `TestCase` registers a pointer to itself into a **static** vector of test cases. Thus, you must make sure the test case is still alive by the time you run the test cases.

However, you don't need to worry about that if you use the convienience macro `TEST_CASE`.

```c++
TEST_CASE("Description", Identifier) {
    // this implicitly creates a function with the argument
    // KTest::TestContext& test
    test.Assert(true, "This test always passes");
}
```

This creates a TestCase with the given description and name. It also creates a function *with that exact name*, so it must be unique within the translation unit.

### TestContext

#### Assertions and Expectations

TestContext allows you to assert conditions and print output.

Assertions use exceptions under the hood. The basic assertion is `Assert`:

```cpp
test.Assert(bool_condition, "Description of the condition");
```

There are also `Expected` assertions that allow asserting expectations about values. This requires the types to overload the `std::ostream::operator <<` operator to print. There are also helpers for common operations, like `AssertEq`, which require certain operators to be defined on the types.

```cpp
test.Expect(bool_condition, actual_value, expected_value, "Description of the expectation");
// requires actual_value == expected_value to be valid
test.AssertEq(actual_value, expected_value, "Description of the expectation");
test.AssertNeq(actual_value, expected_value, "Description of the expectation");
// requires std::abs(actual_value - expected_value) < epsilon to be valid
test.AssertApprox(actual_value, expected_value, "Description of the expectation");
test.AssertApprox(actual_value, expected_value, "Description of the expectation", custom_epsilon);
```

#### Printing

`test.cout` is a `std::ostream` that you can use to print out information during the test. The `TestCaseOptions` determine when the contents are actually printed. By default, it is printed only if the test fails.

```cpp
test.cout << "Current value: " << current_value << std::endl;
test.Assert(false, "What!");
```

### Manually running test cases

If you want to run test cases manually, you can use the static methods below:

```cpp
int main(){
    KTest::TestCaseOptions options;

    options.printOutputOnFailure = true; // Print test.cout output on failure
    options.printOutputOnSuccess = false; // Do not print test.cout output on success
    options.printSuccessfulTestCases = true; // Print all test cases

    // runs all registered test cases
    KTest::TestCase::RunAll();
    KTest::TestCase::RunAll(options);

    // runs test cases with given names
    std::vector<std::string> testCaseNames = {"Factorial0", "Factorial2"};
    KTest::TestCase::RunSome(testCaseNames);
    KTest::TestCase::RunSome(testCaseNames, options);

    // checks if a test case exists, retrieves it, then runs it
    if (KTest::TestCase::Exists("Factorial1")) {
        const KTest::TestCase* testCase = KTest::TestCase::Get("Factorial1");
        // prints some information
        std::cout
            << "Running test case: "
            << testCase->Name() // ie. the message
            << " (ID: "
            << testCase->ID() // eg. Factorial1
            << ")" << std::endl;
        testCase->Run();
        testCase->Run(options);
    }

    // iterates over all test cases
    for (int i = 0; i < KTest::TestCases::NumCases(); ++i){
        const KTest::TestCase* testCase = KTest::TestCases::Get(i);
        std::cout << "Running test case: " << testCase->Name() << std::endl;
        testCase->Run();
        testCase->Run(options);
    }
}
```

#### `TestCaseOptions`

`TestCaseOptions` controls options for the test cases.

```cpp
struct TestCaseOptions {
  bool printOutputOnFailure = true;
  bool printOutputOnSuccess = false;
  bool printSuccessfulTestCases = true;
};
```

`printOutputOnFailure` displays the contents of `TestCaseContext::cout` if the test case fails.

`printOutputOnSuccess` displays the contents of `TestCaseContext::cout` if the test case succeeds.

`printSuccessfulTestCases` displays the name of the test case if it succeeds. Otherwise, it only shows failed test cases and total test case counts.

## TODO

- [ ] Test scenarios
- [ ] Preconditions
- [ ] Better test naming (eg. namespaces)
- [ ] More assertion/test types
  - [ ] Range-based tests
  - [ ] Randomized tests
- [ ] Customize the `TEST_CASE` parameter name
- [ ] Separate `TestCase` from the registration mechanism (eg. maybe a `TestCaseRegistration` class?)
