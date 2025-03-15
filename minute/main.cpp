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
	int num = 2;
	CHECK(2, == , num);

    //CHECK_EXP_THROW(2 == 2, std::runtime_error);
	CHECK_EXP_THROW(throw std::runtime_error("error"), std::runtime_error);
	//CHECK_EXP_THROW(3 == 3, std::runtime_error);
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
