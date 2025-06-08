#include <iostream>
#include "unittest.h"

void CompareNumbers()
{
    int num1 = 12;
	int num2 = 13;
	CHECK(num1, == , num2);

	CHECK(++num1, == , ++num2);
}

void CheckExceptionThrown()
{
	//CHECK_EXP_THROW(throw std::runtime_error("error"), std::runtime_error);
	CHECK_EXP_THROW(1 + 1, std::runtime_error);
}

int main()
{
	UnitTest::EnableTimestamp(true);
	UnitTest::SetResultFilePath("d:\\temp2\\unit_tests.txt"); // optional
    UnitTest::Add("AppSuite", "CompareNumbers", CompareNumbers);
	UnitTest::Add("AppSuite", "CheckExceptionThrown", CheckExceptionThrown);
	
	// RunAllTests() return number of errors
    return UnitTest::RunAllTests();
}
