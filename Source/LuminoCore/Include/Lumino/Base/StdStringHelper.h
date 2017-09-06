// TODO: ïpî…Ç…ïœçXíÜÅBà¿íËÇµÇΩÇÁ StringHelper.h Ç…ìùçáÇ∑ÇÈÅB

#include <string>
#include <codecvt> 
#include "String.h"
#include "../Text/Encoding.h"

LN_NAMESPACE_BEGIN

class StdStringHelper
{
public:

	static std::string makeStdString(const String& str)
	{
		return str.toStdString();
	}

	static std::string makeStdString(const char* str)
	{
		return std::string(str);
	}

	static std::string makeStdString(const char* str, size_t len)
	{
		return std::string(str, len);
	}

	static std::string makeStdString(const wchar_t* str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
		return cv.to_bytes(str);
	}

	static std::string makeStdString(const wchar_t* str, size_t len)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
		return cv.to_bytes(str, str + len);
	}

	template <class TString, class A1, class A2>
	static TString cat(const A1& s1, const A2& s2)
	{
		TString result = s1;
		result.reserve(len(s1) + len(s2));
		result.append(s2);
		return result;
	}

	template <class TString, class A1, class A2, class A3>
	static TString cat(const A1& s1, const A2& s2, const A3& s3)
	{
		TString result = s1;
		result.reserve(len(s1) + len(s2) + len(s3));
		result.append(s2);
		result.append(s3);
		return result;
	}

	template <class TString, class A1, class A2, class A3, class A4>
	static TString cat(const A1& s1, const A2& s2, const A3& s3, const A4& s4)
	{
		TString result = s1;
		result.reserve(len(s1) + len(s2) + len(s3) + len(s4));
		result.append(s2);
		result.append(s3);
		result.append(s4);
		return result;
	}

	static size_t len(const char* s) { return strlen(s); }
	static size_t len(const std::string& s) { return s.length(); }
	static size_t len(const wchar_t* s) { return wcslen(s); }
	static size_t len(const std::wstring& s) { return s.length(); }

	template <class TString, class A1>
	static bool contains(const TString& s1, const A1& a1)
	{
		return s1.find(a1) != TString::npos;
	}
};

LN_NAMESPACE_END
