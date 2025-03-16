# C++ MIN Unit Test Easier (MINUTE)

Existing unit test frameworks create many invisible arbitrary-named claesses and foist a paradigm on developer. For example, which parameters are the actual or expected values. This header-only 300 lines UT library does not do that. It is up to dev to determine which is the actual and expected values. Only free function is supported (no member function) since I do not need fixture setup and teardown.

Only two macros are supported: `CHECK` and `CHECK_EXP_THROW`. Common mistake for `CHECK` is forget to put commas around the 2nd parameter.

```Cpp
#include <iostream>
#include "unittest.h"

void CompareNumbers()
{
    int num1 = 12;
	int num2 = 12;

    CHECK(num1, == , num2);

	CHECK(++num1, == , ++num2);
}

void CheckExceptionThrown()
{
	CHECK_EXP_THROW(throw std::runtime_error("error"), std::runtime_error);
}

int main()
{
	UnitTest::EnableTimestamp(true); // Enable timestamp for file output
	UnitTest::SetResultFilePath("d:\\temp2\\unit_tests.txt"); // optional

    UnitTest::Add("AppSuite", "CompareNumbers", CompareNumbers);
	UnitTest::Add("AppSuite", "CheckExceptionThrown", CheckExceptionThrown);
	
	// RunAllTests() return number of errors
    return UnitTest::RunAllTests();
}
```

This is the test result output.

```
[==========] Running 2 tests from 1 test suites.
[----------] 2 tests from AppSuite
[ RUN      ] AppSuite.CompareNumbers
[       OK ] AppSuite.CompareNumbers (0 ms)
[ RUN      ] AppSuite.CheckExceptionThrown
[       OK ] AppSuite.CheckExceptionThrown (0 ms)
[----------] 2 tests from AppSuite (0 ms total)

[==========] Running 2 tests from 1 test suites ran. (0 ms total)

[  PASSED  ] 2 tests.
```

You can sprinkle `CHECK` in your codebase, not just in the unit tests. `CHECK` does not stop your code from continuing even when the check fails. It only saves the failure's timestamp, filename, line number and function that fails into the file specifed in `SetResultFilePath`.

```Cpp
UnitTest::EnableTimestamp(true); // Enable timestamp for file output
UnitTest::SetResultFilePath("d:\\temp2\\unit_tests.txt"); // optional
```