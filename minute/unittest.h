#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <ctime>

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
		int errors = 0;
		int local_errors = 0;
		for (auto& list : TestList)
		{
			local_errors = 0;
			printf("[%s] running\n", list.first.c_str());
			if (File)
				fprintf(File, "[%s] running\n", list.first.c_str());

			for (auto& pr : list.second)
			{
				printf("%s: running\n", pr.first.c_str());
				if (File)
					fprintf(File, "%s: running\n", pr.first.c_str());

				try
				{
					Error = false;
					pr.second();
					if (Error == false)
					{
						printf("%s: \033[92mpassed!\033[0m\n", pr.first.c_str());
						if (File)
							fprintf(File, "%s: passed!\n", pr.first.c_str());
					}
					else
					{
						printf("%s: \033[91mfailed!\033[0m\n", pr.first.c_str());
						if (File)
							fprintf(File, "%s: failed!\n", pr.first.c_str());
						++local_errors;
					}
				}
				catch (std::exception& e)
				{
					++local_errors;
					fprintf(stderr, "Unknown Exception thrown while testing %s, Exception message: %s\n", pr.first.c_str(), e.what());
					if (File)
						fprintf(File, "%s: Unknown Exception thrown while testing %s, Exception message: %s\n", GetTimestamp().c_str(), pr.first.c_str(), e.what());
				}
				catch (...)
				{
					++local_errors;
					fprintf(stderr, "Unknown Exception thrown while testing %s\n", pr.first.c_str());
					if (File)
						fprintf(File, "%s: Unknown Exception thrown while testing %s\n", GetTimestamp().c_str(), pr.first.c_str());
				}
			}
			errors += local_errors;
			size_t local_count = list.second.size();

			printf("\n[%s] ", list.first.c_str());
			if (File)
				fprintf(File, "\n[%s] ", list.first.c_str());

			printf("%s%zu / %zu passed!\033[0m\n", GetClr(local_errors == 0), local_count - local_errors, local_count);
			if (File)
				fprintf(File, "%zu / %zu passed!\n", local_count - local_errors, local_count);
		}
		size_t count = CountTests();

		printf("%s\nTotal: %zu / %zu passed!\033[0m\n", GetClr(errors == 0), count - errors, count);

		if (File)
			fprintf(File, "\nTotal: %zu / %zu passed!\n", count - errors, count);

		if (errors == 0)
		{
			printf("\n\033[92mALL PASSED!\033[0m\n");
			if (File)
				fprintf(File, "\nALL PASSED!\n");
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
			printf("%s (line #%d): %s:", file, line_num, func);
			printf("CHECK(%s %s %s) \033[91mfailed\033[0m: ", op1, compare, op2);
			printf("(%s %s %s)\n", a.c_str(), compare, b.c_str());

			if (File)
			{
				fprintf(File, "%s: %s (line #%d): %s:", GetTimestamp().c_str(), file, line_num, func);
				fprintf(File, "CHECK(%s %s %s) failed: ", op1, compare, op2);
				fprintf(File, "(%s %s %s)\n", a.c_str(), compare, b.c_str());
			}
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
			fprintf(File, "%s: %s", GetTimestamp().c_str(), text);
		}
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

		snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
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
             "%s (line #%d): CHECK_EXP_THROW(%s, %s) did not throw in %s\n",  \
              __FILE__, __LINE__,  #op, #exception, MINUTE_FUNCTION);         \
        UnitTest::Print(buf);                       \
        UnitTest::SetError(true);                   \
    }                                               \
}

