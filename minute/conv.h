// The MIT License (MIT)
// C++ 17 String Number Conversion 2.0.0
// Copyright (C) 2023 - 2024 by Shao Voon Wong (shaovoon@yahoo.com)
//
// http://opensource.org/licenses/MIT

// version 1.0 : First release
// version 1.1 : Thanks to Colin2 for fixing the warnings on g++ and clang++. 
//               Add float to string conv() with precision.
// version 2.0 : Rename the functions from conv to meaningful name and make conv the struct name

#pragma once

#include <charconv>
#include <cstdint>
#include <string_view>
#include <string>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#define SV_SPRINTF sprintf_s
#else
#define SV_SPRINTF std::sprintf
#include <cstring>
#endif

struct conv
{
private:
    // convert std::wstring to std::string
    //=====================================
    [[nodiscard]] static bool wstr_to_str(const std::wstring_view& view, std::string& str) {
        try {
            str.reserve(view.size());
        }
        catch (std::bad_alloc&)
        {
            return false;
        }
        for (wchar_t c : view)
            str += (char)c;
        return true;
    }
    [[nodiscard]] static bool wstr_to_str(const std::wstring& wstr, std::string& str) {
        try {
            str.reserve(wstr.size());
        }
        catch (std::bad_alloc&)
        {
            return false;
        }
        for (wchar_t c : wstr)
            str += (char)c;
        return true;
    }
    [[nodiscard]] static bool wstr_to_str(const wchar_t* wstr, std::string& str) {
        std::size_t len = wcslen(wstr);
        try {
            str.reserve(len);
        }
        catch (std::bad_alloc&)
        {
            return false;
        }
        for (size_t i = 0; i < len; ++i)
            str += (char)wstr[i];
        return true;
    }

    // convert std::string to std::wstring
    //===================================
    [[nodiscard]] static bool str_to_wstr(const std::string_view& view, std::wstring& wstr) {
        try {
            wstr = std::wstring(view.cbegin(), view.cend());
        }
        catch (std::bad_alloc&)
        {
            return false;
        }
        return true;
    }
    [[nodiscard]] static bool str_to_wstr(const std::string& str, std::wstring& wstr) {
        try {
            wstr = std::wstring(str.cbegin(), str.cend());
        }
        catch (std::bad_alloc&)
        {
            return false;
        }
        return true;
    }
    [[nodiscard]] static bool str_to_wstr(const char* str, std::wstring& wstr) {
        std::size_t len = strlen(str);
        try {
            wstr = std::wstring(str, str + len);
        }
        catch (std::bad_alloc&)
        {
            return false;
        }
        return true;
    }
public:
    // convert std::string_view to float
    //===================================
    [[nodiscard]] static bool str_to_float(const std::string_view& str, float& num, std::chars_format fmt = std::chars_format::general, std::errc* ec = nullptr) {
        auto ret = std::from_chars(str.data(), str.data() + str.length(), num, fmt);
        if (ec) *ec = ret.ec;
        return ret.ec == std::errc();
    }
    [[nodiscard]] static bool str_to_float(const std::string_view& str, double& num, std::chars_format fmt = std::chars_format::general, std::errc* ec = nullptr) {
        auto ret = std::from_chars(str.data(), str.data() + str.length(), num, fmt);
        if (ec) *ec = ret.ec;
        return ret.ec == std::errc();
    }
    // convert std::string to float
    //===================================
    [[nodiscard]] static bool str_to_float(const std::string& str, float& num, std::chars_format fmt = std::chars_format::general, std::errc* ec = nullptr) {
        return str_to_float(std::string_view(str.c_str(), str.size()), num, fmt, ec);
    }
    [[nodiscard]] static bool str_to_float(const std::string& str, double& num, std::chars_format fmt = std::chars_format::general, std::errc* ec = nullptr) {
        return str_to_float(std::string_view(str.c_str(), str.size()), num, fmt, ec);
    }
    // convert const char* to float
    //===================================
    [[nodiscard]] static bool str_to_float(const char* str, float& num, std::chars_format fmt = std::chars_format::general, std::errc* ec = nullptr) {
        return str_to_float(std::string_view(str), num, fmt, ec);
    }
    [[nodiscard]] static bool str_to_float(const char* str, double& num, std::chars_format fmt = std::chars_format::general, std::errc* ec = nullptr) {
        return str_to_float(std::string_view(str), num, fmt, ec);
    }
    // convert std::wstring_view to float
    //===================================
    [[nodiscard]] static bool str_to_float(const std::wstring_view& wstr, float& num, std::chars_format fmt = std::chars_format::general, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_float(std::string_view(str.c_str(), str.size()), num, fmt, ec);
    }
    [[nodiscard]] static bool str_to_float(const std::wstring_view& wstr, double& num, std::chars_format fmt = std::chars_format::general, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_float(std::string_view(str.c_str(), str.size()), num, fmt, ec);
    }
    // convert std::wstring to float
    //===================================
    [[nodiscard]] static bool str_to_float(const std::wstring& wstr, float& num, std::chars_format fmt = std::chars_format::general, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_float(std::string_view(str.c_str(), str.size()), num, fmt, ec);
    }
    [[nodiscard]] static bool str_to_float(const std::wstring& wstr, double& num, std::chars_format fmt = std::chars_format::general, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_float(std::string_view(str.c_str(), str.size()), num, fmt, ec);
    }
    // convert const wchar_t* to float
    //===================================
    [[nodiscard]] static bool str_to_float(const wchar_t* wstr, float& num, std::chars_format fmt = std::chars_format::general, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_float(std::string_view(str.c_str(), str.size()), num, fmt, ec);
    }
    [[nodiscard]] static bool str_to_float(const wchar_t* wstr, double& num, std::chars_format fmt = std::chars_format::general, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_float(std::string_view(str.c_str(), str.size()), num, fmt, ec);
    }

    // convert float to std::string
    //===================================
    [[nodiscard]] static bool float_to_str(float num, std::string& str, std::chars_format fmt = std::chars_format::general) {
        try {
            if (fmt == std::chars_format::general || fmt == std::chars_format::fixed)
                str = std::to_string(num);
            else if (fmt == std::chars_format::scientific)
            {
                const size_t len = 40;
                char buf[len];
                memset(buf, 0, len * sizeof(char));
                if (SV_SPRINTF(buf, "%e", num) < 0)
                    return false;
                str = buf;
            }
            else if (fmt == std::chars_format::hex)
            {
                const size_t len = 40;
                char buf[len];
                memset(buf, 0, len * sizeof(char));
                if (SV_SPRINTF(buf, "%a", num) < 0)
                    return false;
                str = buf;
            }
        }
        catch (std::bad_alloc&)
        {
            return false;
        }
        return true;
    }
    [[nodiscard]] static bool float_to_str(double num, std::string& str, std::chars_format fmt = std::chars_format::general) {
        try {
            if (fmt == std::chars_format::general || fmt == std::chars_format::fixed)
                str = std::to_string(num);
            else if (fmt == std::chars_format::scientific)
            {
                const size_t len = 40;
                char buf[len];
                memset(buf, 0, len * sizeof(char));
                if (SV_SPRINTF(buf, "%e", num) < 0)
                    return false;
                str = buf;
            }
            else if (fmt == std::chars_format::hex)
            {
                const size_t len = 40;
                char buf[len];
                memset(buf, 0, len * sizeof(char));
                if (SV_SPRINTF(buf, "%a", num) < 0)
                    return false;
                str = buf;
            }
        }
        catch (std::bad_alloc&)
        {
            return false;
        }
        return true;
    }
    [[nodiscard]] static bool float_to_str(float num, std::string& str, int precision) {
		try {
			std::ostringstream os_temp;

			if (precision > 0)
			{
				os_temp << std::fixed << std::showpoint << std::setprecision(precision);
			}

			os_temp << num;

            str = os_temp.str();
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}
    [[nodiscard]] static bool float_to_str(double num, std::string& str, int precision) {
		try {
			std::ostringstream os_temp;

			if (precision > 0)
			{
				os_temp << std::fixed << std::showpoint << std::setprecision(precision);
			}

			os_temp << num;

			str = os_temp.str();
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}
    // convert float to char*
    //===================================
    [[nodiscard]] static bool float_to_str(float num, char* str, size_t len, std::chars_format fmt = std::chars_format::general) {
        memset(str, 0, len * sizeof(char));
        auto ret = std::to_chars(str, str + len - 1, num, fmt);
        return ret.ec == std::errc();
    }
    [[nodiscard]] static bool float_to_str(double num, char* str, size_t len, std::chars_format fmt = std::chars_format::general) {
        memset(str, 0, len * sizeof(char));
        auto ret = std::to_chars(str, str + len - 1, num, fmt);
        return ret.ec == std::errc();
    }
    // convert float to std::wstring
    //===================================
    [[nodiscard]] static bool float_to_str(float num, std::wstring& wstr, std::chars_format fmt = std::chars_format::general) {
        try {
            if (fmt == std::chars_format::general || fmt == std::chars_format::fixed)
                wstr = std::to_wstring(num);
            else
            {
                std::string str;
                if (!float_to_str(num, str, fmt))
                    return false;

                if (!str_to_wstr(str, wstr))
                    return false;
            }
        }
        catch (std::bad_alloc&)
        {
            return false;
        }
        return true;
    }
    [[nodiscard]] static bool float_to_str(double num, std::wstring& wstr, std::chars_format fmt = std::chars_format::general) {
        try {
            if (fmt == std::chars_format::general || fmt == std::chars_format::fixed)
                wstr = std::to_wstring(num);
            else
            {
                std::string str;
                if (!float_to_str(num, str, fmt))
                    return false;

                if (!str_to_wstr(str, wstr))
                    return false;
            }
        }
        catch (std::bad_alloc&)
        {
            return false;
        }
        return true;
    }

    [[nodiscard]] static bool float_to_str(float num, std::wstring& str, int precision) {
		try {
			std::wostringstream os_temp;

			if (precision > 0)
			{
				os_temp << std::fixed << std::showpoint << std::setprecision(precision);
			}

			os_temp << num;

			str = os_temp.str();
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}
    [[nodiscard]] static bool float_to_str(double num, std::wstring& str, int precision) {
		try {
			std::wostringstream os_temp;

			if (precision > 0)
			{
				os_temp << std::fixed << std::showpoint << std::setprecision(precision);
			}

			os_temp << num;

			str = os_temp.str();
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}

    // convert float to wchar_t*
    //===================================
    [[nodiscard]] static bool float_to_str(float num, wchar_t* wstr, size_t len, std::chars_format fmt = std::chars_format::general) {
        memset(wstr, 0, len * sizeof(wchar_t));
        std::string str;
        str.resize(len);

        if (float_to_str(num, str.data(), len, fmt))
        {
            for (size_t i = 0; i < len; ++i) {
                if (str[i] == '\0')
                    break;

                wstr[i] = str[i];
            }

            return true;
        }
        return false;
    }
    [[nodiscard]] static bool float_to_str(double num, wchar_t* wstr, size_t len, std::chars_format fmt = std::chars_format::general) {
        memset(wstr, 0, len * sizeof(wchar_t));
        std::string str;
        str.resize(len);

        if (float_to_str(num, str.data(), len, fmt))
        {
            for (size_t i = 0; i < len; ++i) {
                if (str[i] == '\0')
                    break;

                wstr[i] = str[i];
            }

            return true;
        }
        return false;
    }
private:
    [[nodiscard]] static bool Is0x(const std::string_view& str, int base) {
        return (base == 16 && (str.length() > 2 && str[0] == '0' && (str[1] == 'X' || str[1] == 'x')));
    }
public:
    // convert std::string_view to number
    //===================================
    [[nodiscard]] static bool str_to_num(const std::string_view& str, std::int16_t& num, int base = 10, std::errc* ec = nullptr) {
        size_t offset = Is0x(str, base) ? 2 : 0;
        auto ret = std::from_chars(str.data() + offset, str.data() + str.length(), num, base);
        if (ec) *ec = ret.ec;
        return ret.ec == std::errc();
    }
    [[nodiscard]] static bool str_to_num(const std::string_view& str, std::uint16_t& num, int base = 10, std::errc* ec = nullptr) {
        size_t offset = Is0x(str, base) ? 2 : 0;
        auto ret = std::from_chars(str.data() + offset, str.data() + str.length(), num, base);
        if (ec) *ec = ret.ec;
        return ret.ec == std::errc();
    }
    [[nodiscard]] static bool str_to_num(const std::string_view& str, std::int32_t& num, int base = 10, std::errc* ec = nullptr) {
        size_t offset = Is0x(str, base) ? 2 : 0;
        auto ret = std::from_chars(str.data() + offset, str.data() + str.length(), num, base);
        if (ec) *ec = ret.ec;
        return ret.ec == std::errc();
    }
    [[nodiscard]] static bool str_to_num(const std::string_view& str, std::uint32_t& num, int base = 10, std::errc* ec = nullptr) {
        size_t offset = Is0x(str, base) ? 2 : 0;
        auto ret = std::from_chars(str.data() + offset, str.data() + str.length(), num, base);
        if (ec) *ec = ret.ec;
        return ret.ec == std::errc();
    }
    [[nodiscard]] static bool str_to_num(const std::string_view& str, std::int64_t& num, int base = 10, std::errc* ec = nullptr) {
        size_t offset = Is0x(str, base) ? 2 : 0;
        auto ret = std::from_chars(str.data() + offset, str.data() + str.length(), num, base);
        if (ec) *ec = ret.ec;
        return ret.ec == std::errc();
    }
    [[nodiscard]] static bool str_to_num(const std::string_view& str, std::uint64_t& num, int base = 10, std::errc* ec = nullptr) {
        size_t offset = Is0x(str, base) ? 2 : 0;
        auto ret = std::from_chars(str.data() + offset, str.data() + str.length(), num, base);
        if (ec) *ec = ret.ec;
        return ret.ec == std::errc();
    }
    // convert std::string to number
    //===================================
    [[nodiscard]] static bool str_to_num(const std::string& str, std::int16_t& num, int base = 10, std::errc* ec = nullptr) {
        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const std::string& str, std::uint16_t& num, int base = 10, std::errc* ec = nullptr) {
        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const std::string& str, std::int32_t& num, int base = 10, std::errc* ec = nullptr) {
        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const std::string& str, std::uint32_t& num, int base = 10, std::errc* ec = nullptr) {
        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const std::string& str, std::int64_t& num, int base = 10, std::errc* ec = nullptr) {
        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const std::string& str, std::uint64_t& num, int base = 10, std::errc* ec = nullptr) {
        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    // convert const char* to number
    //===================================
    [[nodiscard]] static bool str_to_num(const char* str, std::int16_t& num, int base = 10, std::errc* ec = nullptr) {
        return str_to_num(std::string_view(str), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const char* str, std::uint16_t& num, int base = 10, std::errc* ec = nullptr) {
        return str_to_num(std::string_view(str), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const char* str, std::int32_t& num, int base = 10, std::errc* ec = nullptr) {
        return str_to_num(std::string_view(str), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const char* str, std::uint32_t& num, int base = 10, std::errc* ec = nullptr) {
        return str_to_num(std::string_view(str), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const char* str, std::int64_t& num, int base = 10, std::errc* ec = nullptr) {
        return str_to_num(std::string_view(str), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const char* str, std::uint64_t& num, int base = 10, std::errc* ec = nullptr) {
        return str_to_num(std::string_view(str), num, base, ec);
    }
    // convert std::wstring_view to number
    //===================================
    [[nodiscard]] static bool str_to_num(const std::wstring_view& wstr, std::int16_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const std::wstring_view& wstr, std::uint16_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const std::wstring_view& wstr, std::int32_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const std::wstring_view& wstr, std::uint32_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const std::wstring_view& wstr, std::int64_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const std::wstring_view& wstr, std::uint64_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }

    // convert std::wstring to number
    //===================================
    [[nodiscard]] static bool str_to_num(const std::wstring& wstr, std::int16_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const std::wstring& wstr, std::uint16_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const std::wstring& wstr, std::int32_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const std::wstring& wstr, std::uint32_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const std::wstring& wstr, std::int64_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const std::wstring& wstr, std::uint64_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    // convert const wchar_t* to number
    //===================================
    [[nodiscard]] static bool str_to_num(const wchar_t* wstr, std::int16_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const wchar_t* wstr, std::uint16_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const wchar_t* wstr, std::int32_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const wchar_t* wstr, std::uint32_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const wchar_t* wstr, std::int64_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
    [[nodiscard]] static bool str_to_num(const wchar_t* wstr, std::uint64_t& num, int base = 10, std::errc* ec = nullptr) {
        std::string str;
        if (!wstr_to_str(wstr, str))
            return false;

        return str_to_num(std::string_view(str.c_str(), str.size()), num, base, ec);
    }
	// convert number to std::string
	//===================================
	[[nodiscard]] static bool num_to_str(std::int16_t num, std::string& str, int base = 10) {
		try {
			if (base == 10)
				str = std::to_string((int)num);
			else if (base == 8)
			{
				const size_t len = 10;
				char buf[len];
				memset(buf, 0, len * sizeof(char));
				if (SV_SPRINTF(buf, "%o", num) < 0)
					return false;
				str = buf;
			}
			else if (base == 16)
			{
				const size_t len = 10;
				char buf[len];
				memset(buf, 0, len * sizeof(char));
				if (SV_SPRINTF(buf, "%X", num) < 0)
					return false;
				str = buf;
			}
			else
				return false;
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}
	[[nodiscard]] static bool num_to_str(std::uint16_t num, std::string& str, int base = 10) {
		try {
			if (base == 10)
				str = std::to_string((unsigned int)num);
			else if (base == 8)
			{
				const size_t len = 10;
				char buf[len];
				memset(buf, 0, len * sizeof(char));
				if (SV_SPRINTF(buf, "%o", num) < 0)
					return false;
				str = buf;
			}
			else if (base == 16)
			{
				const size_t len = 10;
				char buf[len];
				memset(buf, 0, len * sizeof(char));
				if (SV_SPRINTF(buf, "%X", num) < 0)
					return false;
				str = buf;
			}
			else
				return false;
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}
	[[nodiscard]] static bool num_to_str(std::int32_t num, std::string& str, int base = 10) {
		try {
			if (base == 10)
				str = std::to_string(num);
			else if (base == 8)
			{
				const size_t len = 20;
				char buf[len];
				memset(buf, 0, len * sizeof(char));
				if (SV_SPRINTF(buf, "%o", num) < 0)
					return false;
				str = buf;
			}
			else if (base == 16)
			{
				const size_t len = 20;
				char buf[len];
				memset(buf, 0, len * sizeof(char));
				if (SV_SPRINTF(buf, "%X", num) < 0)
					return false;
				str = buf;
			}
			else
				return false;
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}
	[[nodiscard]] static bool num_to_str(std::uint32_t num, std::string& str, int base = 10) {
		try {
			if (base == 10)
				str = std::to_string(num);
			else if (base == 8)
			{
				const size_t len = 20;
				char buf[len];
				memset(buf, 0, len * sizeof(char));
				if (SV_SPRINTF(buf, "%o", num) < 0)
					return false;
				str = buf;
			}
			else if (base == 16)
			{
				const size_t len = 20;
				char buf[len];
				memset(buf, 0, len * sizeof(char));
				if (SV_SPRINTF(buf, "%X", num) < 0)
					return false;
				str = buf;
			}
			else
				return false;
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}
	[[nodiscard]] static bool num_to_str(std::int64_t num, std::string& str, int base = 10) {
		try {
			if (base == 10)
				str = std::to_string(num);
			else if (base == 8)
			{
				const size_t len = 40;
				char buf[len];
				memset(buf, 0, len * sizeof(char));
#ifdef _WIN32
				if (SV_SPRINTF(buf, "%llo", num) < 0)
					return false;
#else
				if (SV_SPRINTF(buf, "%lo", num) < 0)
					return false;
#endif
				str = buf;
			}
			else if (base == 16)
			{
				const size_t len = 40;
				char buf[len];
				memset(buf, 0, len * sizeof(char));
#ifdef _WIN32
				if (SV_SPRINTF(buf, "%llX", num) < 0)
					return false;
#else
				if (SV_SPRINTF(buf, "%lX", num) < 0)
					return false;
#endif
				str = buf;
			}
			else
				return false;
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}
	[[nodiscard]] static bool num_to_str(std::uint64_t num, std::string& str, int base = 10) {
		try {
			if (base == 10)
				str = std::to_string(num);
			else if (base == 8)
			{
				const size_t len = 40;
				char buf[len];
				memset(buf, 0, len * sizeof(char));
#ifdef _WIN32
				if (SV_SPRINTF(buf, "%llo", num) < 0)
					return false;
#else
				if (SV_SPRINTF(buf, "%lo", num) < 0)
					return false;
#endif
				str = buf;
			}
			else if (base == 16)
			{
				const size_t len = 40;
				char buf[len];
				memset(buf, 0, len * sizeof(char));
#ifdef _WIN32
				if (SV_SPRINTF(buf, "%llX", num) < 0)
					return false;
#else
				if (SV_SPRINTF(buf, "%lX", num) < 0)
					return false;
#endif
				str = buf;
			}
			else
				return false;
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}
	// convert number to char*
	//===================================
	[[nodiscard]] static bool num_to_str(std::int16_t num, char* str, size_t len, int base = 10) {
		memset(str, 0, len * sizeof(char));
		auto ret = std::to_chars(str, str + len - 1, num, base);
		return ret.ec == std::errc();
	}
	[[nodiscard]] static bool num_to_strnum_to_str(std::uint16_t num, char* str, size_t len, int base = 10) {
		memset(str, 0, len * sizeof(char));
		auto ret = std::to_chars(str, str + len - 1, num, base);
		return ret.ec == std::errc();
	}
	[[nodiscard]] static bool num_to_str(std::int32_t num, char* str, size_t len, int base = 10) {
		memset(str, 0, len * sizeof(char));
		auto ret = std::to_chars(str, str + len - 1, num, base);
		return ret.ec == std::errc();
	}
	[[nodiscard]] static bool num_to_str(std::uint32_t num, char* str, size_t len, int base = 10) {
		memset(str, 0, len * sizeof(char));
		auto ret = std::to_chars(str, str + len - 1, num, base);
		return ret.ec == std::errc();
	}
	[[nodiscard]] static bool num_to_str(std::int64_t num, char* str, size_t len, int base = 10) {
		memset(str, 0, len * sizeof(char));
		auto ret = std::to_chars(str, str + len - 1, num, base);
		return ret.ec == std::errc();
	}
	[[nodiscard]] static bool num_to_str(std::uint64_t num, char* str, size_t len, int base = 10) {
		memset(str, 0, len * sizeof(char));
		auto ret = std::to_chars(str, str + len - 1, num, base);
		return ret.ec == std::errc();
	}
	// convert number to std::wstring
	//===================================
	[[nodiscard]] static bool num_to_str(std::int16_t num, std::wstring& wstr, int base = 10) {
		try {
			if (base == 10)
				wstr = std::to_wstring((int)num);
			else
			{
				std::string str;
				if (!num_to_str(num, str, base))
					return false;

				if (!str_to_wstr(str, wstr))
					return false;
			}
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}
	[[nodiscard]] static bool num_to_str(std::uint16_t num, std::wstring& wstr, int base = 10) {
		try {
			if (base == 10)
				wstr = std::to_wstring((unsigned int)num);
			else
			{
				std::string str;
				if (!num_to_str(num, str, base))
					return false;

				if (!str_to_wstr(str, wstr))
					return false;
			}
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}
	[[nodiscard]] static bool num_to_str(std::int32_t num, std::wstring& wstr, int base = 10) {
		try {
			if (base == 10)
				wstr = std::to_wstring(num);
			else
			{
				std::string str;
				if (!num_to_str(num, str, base))
					return false;

				if (!str_to_wstr(str, wstr))
					return false;
			}
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}
	[[nodiscard]] static bool num_to_str(std::uint32_t num, std::wstring& wstr, int base = 10) {
		try {
			if (base == 10)
				wstr = std::to_wstring(num);
			else
			{
				std::string str;
				if (!num_to_str(num, str, base))
					return false;

				if (!str_to_wstr(str, wstr))
					return false;
			}
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}
	[[nodiscard]] static bool num_to_str(std::int64_t num, std::wstring& wstr, int base = 10) {
		try {
			if (base == 10)
				wstr = std::to_wstring(num);
			else
			{
				std::string str;
				if (!num_to_str(num, str, base))
					return false;

				if (!str_to_wstr(str, wstr))
					return false;
			}
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}
	[[nodiscard]] static bool num_to_str(std::uint64_t num, std::wstring& wstr, int base = 10) {
		try {
			if (base == 10)
				wstr = std::to_wstring(num);
			else
			{
				std::string str;
				if (!num_to_str(num, str, base))
					return false;

				if (!str_to_wstr(str, wstr))
					return false;
			}
		}
		catch (std::bad_alloc&)
		{
			return false;
		}
		return true;
	}
	// convert number to wchar_t*
	//===================================
	[[nodiscard]] static bool num_to_str(std::int16_t num, wchar_t* wstr, size_t len, int base = 10) {
		memset(wstr, 0, len * sizeof(wchar_t));
		std::string str;
		str.resize(len);

		if (num_to_str(num, str.data(), len, base))
		{
			for (size_t i = 0; i < len; ++i) {
				if (str[i] == '\0')
					break;

				wstr[i] = str[i];
			}

			return true;
		}
		return false;
	}
	[[nodiscard]] static bool num_to_str(std::uint16_t num, wchar_t* wstr, size_t len, int base = 10) {
		memset(wstr, 0, len * sizeof(wchar_t));
		std::string str;
		str.resize(len);

		if (num_to_str(num, str.data(), len, base))
		{
			for (size_t i = 0; i < len; ++i) {
				if (str[i] == '\0')
					break;

				wstr[i] = str[i];
			}

			return true;
		}
		return false;
	}
	[[nodiscard]] static bool num_to_str(std::int32_t num, wchar_t* wstr, size_t len, int base = 10) {
		memset(wstr, 0, len * sizeof(wchar_t));
		std::string str;
		str.resize(len);

		if (num_to_str(num, str.data(), len, base))
		{
			for (size_t i = 0; i < len; ++i) {
				if (str[i] == '\0')
					break;

				wstr[i] = str[i];
			}

			return true;
		}
		return false;
	}
	[[nodiscard]] static bool num_to_str(std::uint32_t num, wchar_t* wstr, size_t len, int base = 10) {
		memset(wstr, 0, len * sizeof(wchar_t));
		std::string str;
		str.resize(len);

		if (num_to_str(num, str.data(), len, base))
		{
			for (size_t i = 0; i < len; ++i) {
				if (str[i] == '\0')
					break;

				wstr[i] = str[i];
			}

			return true;
		}
		return false;
	}
	[[nodiscard]] static bool num_to_str(std::int64_t num, wchar_t* wstr, size_t len, int base = 10) {
		memset(wstr, 0, len * sizeof(wchar_t));
		std::string str;
		str.resize(len);

		if (num_to_str(num, str.data(), len, base))
		{
			for (size_t i = 0; i < len; ++i) {
				if (str[i] == '\0')
					break;

				wstr[i] = str[i];
			}

			return true;
		}
		return false;
	}
	[[nodiscard]] static bool num_to_str(std::uint64_t num, wchar_t* wstr, size_t len, int base = 10) {
		memset(wstr, 0, len * sizeof(wchar_t));
		std::string str;
		str.resize(len);

		if (num_to_str(num, str.data(), len, base))
		{
			for (size_t i = 0; i < len; ++i) {
				if (str[i] == '\0')
					break;

				wstr[i] = str[i];
			}

			return true;
		}
		return false;
	}
	[[nodiscard]] static std::string to_str(bool val)
	{
		return val ? "true" : "false";
	}

	[[nodiscard]] static std::string to_str(int16_t val)
	{
		std::string str;
		if (!num_to_str(val, str))
			throw std::runtime_error("num_to_str failed");

		return str;
	}

	[[nodiscard]] static std::string to_str(int32_t val)
	{
		std::string str;
		if (!num_to_str(val, str))
			throw std::runtime_error("num_to_str failed");

		return str;
	}

	[[nodiscard]] static std::string to_str(int64_t val)
	{
		std::string str;
		if (!num_to_str(val, str))
			throw std::runtime_error("num_to_str failed");

		return str;
	}

	[[nodiscard]] static std::string to_str(uint16_t val)
	{
		std::string str;
		if (!num_to_str(val, str))
			throw std::runtime_error("num_to_str failed");

		return str;
	}

	[[nodiscard]] static std::string to_str(uint32_t val)
	{
		std::string str;
		if (!num_to_str(val, str))
			throw std::runtime_error("num_to_str failed");

		return str;
	}

	[[nodiscard]] static std::string to_str(uint64_t val)
	{
		std::string str;
		if (!num_to_str(val, str))
			throw std::runtime_error("num_to_str failed");

		return str;
	}

	[[nodiscard]] static std::string to_str(float val)
	{
		std::string str;
		if (!float_to_str(val, str))
			throw std::runtime_error("float_to_str failed");

		return str;
	}

	[[nodiscard]] static std::string to_str(double val)
	{
		std::string str;
		if (!float_to_str(val, str))
			throw std::runtime_error("float_to_str failed");

		return str;
	}

	[[nodiscard]] static std::string to_str(char val)
	{
		std::string str;
		str += val;
		return str;
	}

	[[nodiscard]] static std::string to_str(unsigned char val)
	{
		std::string str;
		uint16_t val2 = val;
		if (!num_to_str(val2, str))
			throw std::runtime_error("num_to_str failed");

		return str;
	}

	[[nodiscard]] static std::string to_str(const std::string& val)
	{
		return val;
	}

	[[nodiscard]] static std::string to_str(const char* val)
	{
		return val;
	}

	[[nodiscard]] static std::string to_str(const std::wstring& val)
	{
		std::string str;
		if (!wstr_to_str(val, str))
			throw std::runtime_error("num_to_str failed");

		return str;
	}

	[[nodiscard]] static std::string to_str(const wchar_t* val)
	{
		std::string str;
		if (!wstr_to_str(val, str))
			throw std::runtime_error("num_to_str failed");

		return str;
	}

};
