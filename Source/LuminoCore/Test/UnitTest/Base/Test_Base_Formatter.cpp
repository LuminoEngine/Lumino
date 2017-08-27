#include <TestConfig.h>
#include <Lumino/Base/StringBuilder.h>
#include <Lumino/Base/Locale.h>

#ifdef LN_USTRING
#else

namespace ln
{

	//// GenericString
	//template<>
	//struct Formatter<Char, detail::FormatArgType::KindString, String>
	//{
	//	static GenericString<Char> Format(const Locale& locale, const GenericStringRef<TCHAR>& format, const GenericStringRef<TCHAR>& formatParam, String value)
	//	{
	//		return value;
	//	}
	//};
	//composite formatting
}

class Test_Base_Formatter : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};


//template<typename T, std::size_t N>
//struct IsCharArray<std::is_same<T, wchar_t[N]>> : std::true_type{};
//template<typename T, std::size_t N>
//struct IsCharArray<std::is_same<T, const wchar_t[N]>> : std::true_type{};

//---------------------------------------------------------------------
TEST_F(Test_Base_Formatter, Basic)
{
	ASSERT_EQ(_T(""), String::format(_T("")));
	ASSERT_EQ(_T("a"), String::format(_T("a")));
	ASSERT_EQ(_T("a"), String::format(_T("{0}"), _T("a")));
	ASSERT_EQ(_T("ab"), String::format(_T("{0}{1}"), _T("a"), _T("b")));
	ASSERT_EQ(_T("ba"), String::format(_T("{1}{0}"), _T("a"), _T("b")));
	ASSERT_EQ(_T("aba"), String::format(_T("{0}{1}{0}"), _T("a"), _T("b")));
	ASSERT_EQ(_T("{0}"), String::format(_T("{{0}}"), _T("a"), _T("b")));

	// <Test> インデント
	{
		ASSERT_EQ(_T("   a"), String::format(_T("{0,4}"), _T("a")));
		ASSERT_EQ(_T("  ab"), String::format(_T("{0,4}"), _T("ab")));
		ASSERT_EQ(_T("a   "), String::format(_T("{0,-4}"), _T("a")));
		ASSERT_EQ(_T("ab  "), String::format(_T("{0,-4}"), _T("ab")));
		ASSERT_EQ(_T("   aa   ab  "), String::format(_T("{0,4}{0,-4}{1,-4}"), _T("a"), _T("ab")));
		ASSERT_EQ(_T("abcd"), String::format(_T("{0,3}"), _T("abcd")));
		ASSERT_EQ(_T("abcd"), String::format(_T("{0,4}"), _T("abcd")));
		ASSERT_EQ(_T(" abcd"), String::format(_T("{0,5}"), _T("abcd")));
		ASSERT_EQ(_T("abcd"), String::format(_T("{0,-3}"), _T("abcd")));
		ASSERT_EQ(_T("abcd"), String::format(_T("{0,-4}"), _T("abcd")));
		ASSERT_EQ(_T("abcd "), String::format(_T("{0,-5}"), _T("abcd")));
	}
	// <Test> ロケール
	{
		//Locale lc(_T("fr"));
		//ASSERT_EQ(_T("10"), String::format(lc, _T("{0}"), 10));
		//ASSERT_EQ(_T("2147483647"), String::format(_T("{0}"), INT32_MAX));
	}

	// <Test> char/wchar_t
	{
		ASSERT_EQ(_T("a"), String::format(_T("{0}"), _T('a')));
	}
	// <Test> int16_t
	{
		ASSERT_EQ(_T("42"), String::format(_T("{0}"), (int16_t)42));
		ASSERT_EQ(_T("32767"), String::format(_T("{0}"), INT16_MAX));
		ASSERT_EQ(_T("-32768"), String::format(_T("{0}"), INT16_MIN));
	}
	// <Test> uint16_t
	{
		ASSERT_EQ(_T("42"), String::format(_T("{0}"), (uint16_t)42));
		ASSERT_EQ(_T("ffff"), String::format(_T("{0:x}"), UINT16_MAX));
	}
	// <Test> int32_t
	{
		ASSERT_EQ(_T("42"), String::format(_T("{0}"), (int32_t)42));
		ASSERT_EQ(_T("2147483647"), String::format(_T("{0}"), INT32_MAX));
		ASSERT_EQ(_T("-2147483648"), String::format(_T("{0}"), INT32_MIN));
	}
	// <Test> uint32_t
	{
		ASSERT_EQ(_T("42"), String::format(_T("{0}"), (uint32_t)42));
		ASSERT_EQ(_T("ffffffff"), String::format(_T("{0:x}"), UINT32_MAX));
	}
	// <Test> int64_t
	{
		ASSERT_EQ(_T("42"), String::format(_T("{0}"), (int64_t)42));
		ASSERT_EQ(_T("9223372036854775807"), String::format(_T("{0}"), INT64_MAX));
		ASSERT_EQ(_T("-9223372036854775808"), String::format(_T("{0}"), INT64_MIN));
	}
	// <Test> uint64_t
	{
		ASSERT_EQ(_T("42"), String::format(_T("{0}"), (uint64_t)42));
		ASSERT_EQ(_T("ffffffffffffffff"), String::format(_T("{0:x}"), UINT64_MAX));
	}
	// <Test> float
	{
		ASSERT_EQ(_T("1.5"), String::format(_T("{0}"), 1.5f));
	}
	// <Test> double
	{
		ASSERT_EQ(_T("1.5"), String::format(_T("{0}"), 1.5));
	}
	// <Test> bool
	{
		bool a = true;
		ASSERT_EQ(_T("True"), String::format(_T("{0}"), a));
		ASSERT_EQ(_T("False"), String::format(_T("{0}"), false));
	}
	// <Test> char[]/wchar_t[]
	{
		ASSERT_EQ(_T("abc"), String::format(_T("{0}"), _T("abc")));
	}
	// <Test> char*/wchar_t*
	{
		Char buf[] = _T("abc");
		Char* t1 = (Char*)buf;
		const Char* t2 = _T("def");
		ASSERT_EQ(_T("abcdef"), String::format(_T("{0}{1}"), t1, t2));
	}
	// <Test> std::string
	{
		std::basic_string<Char> s = _T("abc");
		ASSERT_EQ(_T("abc"), String::format(_T("{0}"), s));
	}
	// <Test> String
	{
		String s = _T("abc");
		ASSERT_EQ(_T("abc"), String::format(_T("{0}"), s));
	}
	// <Test> PathName
	{
		PathName s = _T("abc");
		ASSERT_EQ(_T("abc"), String::format(_T("{0}"), s));
	}

	//auto aa = String::format(Locale::GetDefault(), _T("{0}"), 12445.6789);

	// <Test> D
	{
		ASSERT_EQ(_T("15"), String::format(_T("{0:D}"), 15));
		ASSERT_EQ(_T("15"), String::format(_T("{0:d}"), 15));
		ASSERT_EQ(_T("0015"), String::format(_T("{0:D4}"), 15));
	}
	// <Test> X
	{
		ASSERT_EQ(_T("F"), String::format(_T("{0:X}"), 15));
		ASSERT_EQ(_T("f"), String::format(_T("{0:x}"), 15));
		ASSERT_EQ(_T("000f"), String::format(_T("{0:x4}"), 15));
	}
	// <Test> F
	{
		ASSERT_EQ(_T("25.187900"), String::format(_T("{0:F}"), 25.1879));
		ASSERT_EQ(_T("25.187900"), String::format(_T("{0:f}"), 25.1879));
		ASSERT_EQ(_T("25.19"), String::format(_T("{0:F2}"), 25.1879));
		ASSERT_EQ(_T("25,187900"), String::format(Locale(_T("fr")), _T("{0:F}"), 25.1879));
	}
	// <Test> E
	{
#if _MSC_VER >= 1900	// VS2015 でちょっと変わった？
		ASSERT_EQ(_T("1.005000e+02"), String::format(_T("{0:e}"), 100.5));
		ASSERT_EQ(_T("1.005000E+02"), String::format(_T("{0:E}"), 100.5));
		ASSERT_EQ(_T("1.01e+02"), String::format(_T("{0:e2}"), 100.5));
		ASSERT_EQ(_T("1.00500000E+02"), String::format(_T("{0:E8}"), 100.5));
#else
		ASSERT_EQ(_T("1.005000e+002"), String::format(_T("{0:e}"), 100.5));
		ASSERT_EQ(_T("1.005000E+002"), String::format(_T("{0:E}"), 100.5));
		ASSERT_EQ(_T("1.01e+002"), String::format(_T("{0:e2}"), 100.5));
		ASSERT_EQ(_T("1.00500000E+002"), String::format(_T("{0:E8}"), 100.5));
#endif
	}

	// <Test> {}
	{
		ASSERT_EQ(_T("{0}"), String::format(_T("{{0}}")));
		ASSERT_EQ(_T("{1}"), String::format(_T("{{{0}}}"), 1));
	}

}

//---------------------------------------------------------------------
TEST_F(Test_Base_Formatter, Illigal)
{
	// <Test> 引数不足の確認
	{
		ASSERT_THROW(String::format(_T("{0}")), InvalidFormatException);
		ASSERT_THROW(String::format(_T("{1}"), _T("a")), InvalidFormatException);
	}
	// <Test> 文法エラーの確認
	{
		ASSERT_THROW(String::format(_T("{")), InvalidFormatException);
		ASSERT_THROW(String::format(_T("}")), InvalidFormatException);
		ASSERT_THROW(String::format(_T("{a")), InvalidFormatException);
		ASSERT_THROW(String::format(_T("a}")), InvalidFormatException);
		ASSERT_THROW(String::format(_T("{0,a}"), _T("a")), InvalidFormatException);
		ASSERT_THROW(String::format(_T("{0,-a}"), _T("a")), InvalidFormatException);
		ASSERT_THROW(String::format(_T("{0, "), _T("a")), InvalidFormatException);
		ASSERT_THROW(String::format(_T("{0:"), _T("a")), InvalidFormatException);
		ASSERT_THROW(String::format(_T("{0:D"), _T("a")), InvalidFormatException);
		ASSERT_THROW(String::format(_T("{0:D "), _T("a")), InvalidFormatException);
	}
}


//---------------------------------------------------------------------
TEST_F(Test_Base_Formatter, Examples)
{
	{
		String name = _T("file");
		int index = 5;
		String fileName = String::format(_T("{0}_{1}.txt"), name, index);
		ASSERT_EQ(_T("file_5.txt"), fileName);
	}
	{
		ASSERT_EQ(_T("12345"), String::format(_T("{0:D}"), 12345));
		ASSERT_EQ(_T("-12345"), String::format(_T("{0:d}"), -12345));
		ASSERT_EQ(_T("00012345"), String::format(_T("{0:D8}"), 12345));
	}
	{
#if _MSC_VER >= 1900	// VS2015 でちょっと変わった？
		ASSERT_EQ(_T("1.234568e+04"), String::format(_T("{0:e}"), 12345.6789));
		ASSERT_EQ(_T("1.2345678900E+04"), String::format(_T("{0:E10}"), 12345.6789));
		ASSERT_EQ(_T("1.2346e+04"), String::format(_T("{0:e4}"), 12345.6789));
		ASSERT_EQ(_T("1,234568E+04"), String::format(Locale(_T("fr")), _T("{0:E}"), 12345.6789));
#else
		ASSERT_EQ(_T("1.234568e+004"), String::format(_T("{0:e}"), 12345.6789));
		ASSERT_EQ(_T("1.2345678900E+004"), String::format(_T("{0:E10}"), 12345.6789));
		ASSERT_EQ(_T("1.2346e+004"), String::format(_T("{0:e4}"), 12345.6789));
		ASSERT_EQ(_T("1,234568E+004"), String::format(Locale(_T("fr")), _T("{0:E}"), 12345.6789));
#endif
	}

	{
		ASSERT_EQ(_T("2045e"), String::format(_T("{0:x}"), 0x2045e));
		ASSERT_EQ(_T("2045E"), String::format(_T("{0:X}"), 0x2045e));
		ASSERT_EQ(_T("0002045E"), String::format(_T("{0:X8}"), 0x2045e));
		ASSERT_EQ(_T("0xFF"), String::format(_T("0x{0:X}"), 255));
	}
}

#endif
