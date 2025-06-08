// The MIT License (MIT)
// C++ MIN Unit Test Easier (MINUTE) 0.1.0
// Copyright (C) 2025, by Wong Shao Voon (shaovoon@yahoo.com)
//
// http://opensource.org/licenses/MIT

#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <ctime>
#include <chrono>

#ifdef _MSC_VER
    #include <Windows.h>
#endif

class UnitTest
{
public:
	UnitTest() = default;
	static void Add(const std::string& suite, const std::string& testName, std::function<void()> test)
	{
		TestList[suite].push_back(std::make_pair(testName, test));
	}
	// return number of errors
	static int RunAllTests()
	{
#ifdef _MSC_VER
		DebuggerPresent = ::IsDebuggerPresent();
#endif

		int errors = 0;
		int local_errors = 0;
		long long totalTime = 0;
		size_t totalTests = CountTests();

		printf("[==========] Running %zu tests from %zu test suites.\n", totalTests, TestList.size());
		if (File)
			fprintf(File, "[==========] Running %zu tests from %zu test suites.\n", totalTests, TestList.size());


		for (auto& list : TestList)
		{
			local_errors = 0;
			printf("[----------] %zu tests from %s\n", list.second.size(), list.first.c_str());
			if (File)
				fprintf(File, "[----------] %zu tests from %s\n", list.second.size(), list.first.c_str());

			long long suiteTime = 0;
			for (auto& pr : list.second)
			{
				printf("[ RUN      ] %s.%s\n", list.first.c_str(), pr.first.c_str());
				if (File)
					fprintf(File, "%s: running\n", pr.first.c_str());

				try
				{
					Error = false;
					std::chrono::high_resolution_clock::time_point beginTime = std::chrono::high_resolution_clock::now();
					pr.second();
					std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
					auto dur = endTime - beginTime;
					auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
					suiteTime += ms;
					if (Error == false)
					{
						printf("[       \033[92mOK\033[0m ] %s.%s (%lld ms)\n", list.first.c_str(), pr.first.c_str(), ms);
						if (File)
							fprintf(File, "[       OK ] %s.%s (%lld ms)\n", list.first.c_str(), pr.first.c_str(), ms);
					}
					else
					{
						printf("[     \033[91mFAIL\033[0m ] %s.%s (%lld ms)\n", list.first.c_str(), pr.first.c_str(), ms);
						if (File)
							fprintf(File, "[     FAIL ] %s.%s (%lld ms)\n", list.first.c_str(), pr.first.c_str(), ms);
						++local_errors;
					}
				}
				catch (std::exception& e)
				{
					++local_errors;
					fprintf(stderr, "Unknown Exception thrown while testing %s, Exception message: %s\n", pr.first.c_str(), e.what());
					if (File)
						fprintf(File, "%sUnknown Exception thrown while testing %s, Exception message: %s\n", GetTimestamp().c_str(), pr.first.c_str(), e.what());
				}
				catch (...)
				{
					++local_errors;
					fprintf(stderr, "Unknown Exception thrown while testing %s\n", pr.first.c_str());
					if (File)
						fprintf(File, "%sUnknown Exception thrown while testing %s\n", GetTimestamp().c_str(), pr.first.c_str());
				}
			}
			errors += local_errors;
			size_t local_count = list.second.size();

			printf("[----------] %zu tests from %s (%llu ms total)\n\n", list.second.size(), list.first.c_str(), suiteTime);

			if (File)
				fprintf(File, "[----------] %zu tests from %s (%llu ms total)\n\n", list.second.size(), list.first.c_str(), suiteTime);

			totalTime += suiteTime;
		}

		printf("[==========] Running %zu tests from %zu test suites ran. (%llu ms total)\n", totalTests, TestList.size(), totalTime);
		if (File)
			fprintf(File, "[==========] Running %zu tests from %zu test suites ran. (%llu ms total)\n", totalTests, TestList.size(), totalTime);


		if (errors == 0)
		{
			printf("\n[  \033[92mPASSED\033[0m  ] %zu tests.\n", totalTests);
			if (File)
				fprintf(File, "\n[  PASSED  ] %zu tests.\n", totalTests);
		}
		else
		{
			printf("\n[  \033[91mFAILED\033[0m  ] %d / %zu tests failed.\n", errors, totalTests);
			if (File)
				fprintf(File, "\n[  FAILED  ] %d / %zu tests failed.\n", errors, totalTests);
		}

		if (File)
		{
			fflush(File);
			fclose(File);
			File = nullptr;
		}
		return errors;
	}
	static const char* GetClr(bool pass)
	{
		return pass ? "\033[92m" : "\033[91m";
	}
	static void SetError(bool error)
	{
		Error = error;
	}
	static size_t CountTests()
	{
		size_t count = 0;
		for (auto& list : TestList)
		{
			count += list.second.size();
		}
		return count;
	}
	static void Check(bool result, const char* op1, 
		const char* compare, const char* op2, 
		const std::string& a, const std::string& b, 
		const char* file, int line_num, const char* func)
	{
		if (result == false)
		{
			UnitTest::SetError(true);
			printf("%s (%d, 1): %s: ", file, line_num, func);
			printf("CHECK(%s %s %s) \033[91mfailed\033[0m: ", op1, compare, op2);
			printf("(%s %s %s)\n", a.c_str(), compare, b.c_str());

			if (File)
			{
				fprintf(File, "%s%s (%d, 1): %s: ", GetTimestamp().c_str(), file, line_num, func);
				fprintf(File, "CHECK(%s %s %s) failed: ", op1, compare, op2);
				fprintf(File, "(%s %s %s)\n", a.c_str(), compare, b.c_str());
			}

#ifdef _MSC_VER
			if (DebuggerPresent)
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "%s (%d, 1): %s: ", file, line_num, func);
				OutputDebugStringA(buf);
				snprintf(buf, sizeof(buf), "CHECK(%s %s %s) failed: ", op1, compare, op2);
				OutputDebugStringA(buf);
				snprintf(buf, sizeof(buf), "(%s %s %s)\n", a.c_str(), compare, b.c_str());
				OutputDebugStringA(buf);
			}
#endif
		}
	}
	static bool SetResultFilePath(const char* path)
	{
		if (File)
		{
			fclose(File);
			File = nullptr;
		}
		File = fopen(path, "wt");

		return File != nullptr;
	}
	static void CloseFile()
	{
		if (File)
		{
			fclose(File);
			File = nullptr;
		}
	}
	static bool IsFileOpen()
	{
		return File != nullptr;
	}
	static FILE* GetFilePtr()
	{
		return File;
	}
	static void Print(const char* text)
	{
		printf("%s", text);
		if (File)
		{
			fprintf(File, "%s%s", GetTimestamp().c_str(), text);
		}
#ifdef _MSC_VER
		if (DebuggerPresent)
		{
			OutputDebugStringA(text);
		}
#endif
	}
	static void EnableTimestamp(bool enable)
	{
		Timestamp = enable;
	}
	static bool IsTimestampEnabled()
	{
		return Timestamp;
	}
	static std::string GetTimestamp()
	{
		std::string str;
		if (Timestamp == false)
			return {};

#ifdef _MSC_VER
		tm t;
		char buf[200];

		time_t e = time(NULL);
		errno_t error = localtime_s(&t, &e);

		if (error)
			return {};

		snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d: ",
			t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

		str = buf;
#else
		struct tm temp;
		char buf[200];

		time_t e = time(NULL);
		struct tm* d = localtime_r(&e, &temp);

		snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
			d->tm_year + 1900, d->tm_mon + 1, d->tm_mday, d->tm_hour, d->tm_min, d->tm_sec);

		str = buf;
#endif
		return str;
	}
private:
	static inline FILE* File = nullptr;
	static inline bool Timestamp = false;
	static inline bool Error = false;
	static inline std::unordered_map<std::string,
	std::vector<std::pair<std::string, std::function<void()> > > > TestList;
#ifdef _MSC_VER
	static inline bool DebuggerPresent = false;
#endif
};

#if defined(__GNUC__)
#define MINUTE_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define MINUTE_FUNCTION __FUNCSIG__
#else
#define MINUTE_FUNCTION __func__
#endif

#define CHECK(op1, compare, op2)                                                 \
    {   auto v1 = op1; auto v2 = op2; auto res = (v1 compare v2);                \
        if (!res)                                                                \
        {                                                                        \
            std::ostringstream oss1; oss1 << v1;                                 \
            std::ostringstream oss2; oss2 << v2;                                 \
            UnitTest::Check((v1 compare v2), #op1, #compare, #op2,               \
	            oss1.str(), oss2.str(), __FILE__, __LINE__, MINUTE_FUNCTION);    \
        }                                                                        \
    }

#define CHECK_EXP_THROW(op, exception)              \
{                                                   \
    bool thrown = false;                            \
    try                                             \
    {                                               \
        op;                                         \
    }                                               \
    catch (exception ex)                            \
    {                                               \
	    thrown = true;                              \
    }                                               \
    if (thrown == false)                            \
    {                                               \
        char buf[2000];                             \
        snprintf(buf, sizeof(buf),                  \
             "%s (%d, 1): %s: CHECK_EXP_THROW(%s, %s) did not throw %s\n",  \
              __FILE__, __LINE__, MINUTE_FUNCTION, #op, #exception, #exception);         \
        UnitTest::Print(buf);                       \
        UnitTest::SetError(true);                   \
    }                                               \
}

