#include <TestConfig.h>
#include <Lumino/Base/StringBuilder.h>
#include <Lumino/Base/Locale.h>

class Test_Base_Formatter : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//---------------------------------------------------------------------
TEST_F(Test_Base_Formatter, Basic)
{
	ASSERT_EQ(_TT(""), UString::format(_TT("")));
	ASSERT_EQ(_TT("a"), UString::format(_TT("a")));
	ASSERT_EQ(_TT("a"), UString::format(_TT("{0}"), _TT("a")));
	ASSERT_EQ(_TT("ab"), UString::format(_TT("{0}{1}"), _TT("a"), _TT("b")));
	ASSERT_EQ(_TT("ba"), UString::format(_TT("{1}{0}"), _TT("a"), _TT("b")));
	ASSERT_EQ(_TT("aba"), UString::format(_TT("{0}{1}{0}"), _TT("a"), _TT("b")));
	ASSERT_EQ(_TT("{0}"), UString::format(_TT("{{0}}"), _TT("a"), _TT("b")));

	// <Test> インデント
	{
		ASSERT_EQ(_TT("   a"), UString::format(_TT("{0,4}"), _TT("a")));
		ASSERT_EQ(_TT("  ab"), UString::format(_TT("{0,4}"), _TT("ab")));
		ASSERT_EQ(_TT("a   "), UString::format(_TT("{0,-4}"), _TT("a")));
		ASSERT_EQ(_TT("ab  "), UString::format(_TT("{0,-4}"), _TT("ab")));
		ASSERT_EQ(_TT("   aa   ab  "), UString::format(_TT("{0,4}{0,-4}{1,-4}"), _TT("a"), _TT("ab")));
		ASSERT_EQ(_TT("abcd"), UString::format(_TT("{0,3}"), _TT("abcd")));
		ASSERT_EQ(_TT("abcd"), UString::format(_TT("{0,4}"), _TT("abcd")));
		ASSERT_EQ(_TT(" abcd"), UString::format(_TT("{0,5}"), _TT("abcd")));
		ASSERT_EQ(_TT("abcd"), UString::format(_TT("{0,-3}"), _TT("abcd")));
		ASSERT_EQ(_TT("abcd"), UString::format(_TT("{0,-4}"), _TT("abcd")));
		ASSERT_EQ(_TT("abcd "), UString::format(_TT("{0,-5}"), _TT("abcd")));
	}
	// <Test> ロケール
	{
		//Locale lc(u"fr");
		//ASSERT_EQ(u"10", UString::format(lc, _TT("{0}"), 10));
		//ASSERT_EQ(u"2147483647", UString::format(_TT("{0}"), INT32_MAX));
	}

	// <Test> char/wchar_t
	{
		ASSERT_EQ(_TT("a"), UString::format(_TT("{0}"), _TT('a')));
	}
	// <Test> int16_t
	{
		ASSERT_EQ(_TT("42"), UString::format(_TT("{0}"), (int16_t)42));
		ASSERT_EQ(_TT("32767"), UString::format(_TT("{0}"), INT16_MAX));
		ASSERT_EQ(_TT("-32768"), UString::format(_TT("{0}"), INT16_MIN));
	}
	// <Test> uint16_t
	{
		ASSERT_EQ(_TT("42"), UString::format(_TT("{0}"), (uint16_t)42));
		ASSERT_EQ(_TT("ffff"), UString::format(_TT("{0:x}"), UINT16_MAX));
	}
	// <Test> int32_t
	{
		ASSERT_EQ(_TT("42"), UString::format(_TT("{0}"), (int32_t)42));
		ASSERT_EQ(_TT("2147483647"), UString::format(_TT("{0}"), INT32_MAX));
		ASSERT_EQ(_TT("-2147483648"), UString::format(_TT("{0}"), INT32_MIN));
	}
	// <Test> uint32_t
	{
		ASSERT_EQ(_TT("42"), UString::format(_TT("{0}"), (uint32_t)42));
		ASSERT_EQ(_TT("ffffffff"), UString::format(_TT("{0:x}"), UINT32_MAX));
	}
	// <Test> int64_t
	{
		ASSERT_EQ(_TT("42"), UString::format(_TT("{0}"), (int64_t)42));
		ASSERT_EQ(_TT("9223372036854775807"), UString::format(_TT("{0}"), INT64_MAX));
		ASSERT_EQ(_TT("-9223372036854775808"), UString::format(_TT("{0}"), INT64_MIN));
	}
	// <Test> uint64_t
	{
		ASSERT_EQ(_TT("42"), UString::format(_TT("{0}"), (uint64_t)42));
		ASSERT_EQ(_TT("ffffffffffffffff"), UString::format(_TT("{0:x}"), UINT64_MAX));
	}
	// <Test> float
	{
		ASSERT_EQ(_TT("1.5"), UString::format(_TT("{0}"), 1.5f));
	}
	// <Test> double
	{
		ASSERT_EQ(_TT("1.5"), UString::format(_TT("{0}"), 1.5));
	}
	// <Test> bool
	{
		bool a = true;
		ASSERT_EQ(_TT("True"), UString::format(_TT("{0}"), a));
		ASSERT_EQ(_TT("False"), UString::format(_TT("{0}"), false));
	}
	// <Test> char[]/wchar_t[]
	{
		ASSERT_EQ(_TT("abc"), UString::format(_TT("{0}"), _TT("abc")));
	}
	// <Test> char*/wchar_t*
	{
		UChar buf[] = _TT("abc");
		UChar* t1 = (UChar*)buf;
		const UChar* t2 = _TT("def");
		ASSERT_EQ(_TT("abcdef"), UString::format(_TT("{0}{1}"), t1, t2));
	}
#if 0	// TODO:
	// <Test> std::string
	{
		std::basic_string<UChar> s = _TT("abc");
		ASSERT_EQ(_TT("abc"), UString::format(_TT("{0}"), s));
	}
#endif
	// <Test> String
	{
		UString s = _TT("abc");
		ASSERT_EQ(_TT("abc"), UString::format(_TT("{0}"), s));
	}
#if 0	// TODO:
	// <Test> Path
	{
		Path s = _TT("abc");
		ASSERT_EQ(_TT("abc"), UString::format(_TT("{0}"), s));
	}
#endif

	//auto aa = UString::format(Locale::GetDefault(), _TT("{0}"), 12445.6789);

	// <Test> D
	{
		ASSERT_EQ(_TT("15"), UString::format(_TT("{0:D}"), 15));
		ASSERT_EQ(_TT("15"), UString::format(_TT("{0:d}"), 15));
		ASSERT_EQ(_TT("0015"), UString::format(_TT("{0:D4}"), 15));
	}
	// <Test> X
	{
		ASSERT_EQ(_TT("F"), UString::format(_TT("{0:X}"), 15));
		ASSERT_EQ(_TT("f"), UString::format(_TT("{0:x}"), 15));
		ASSERT_EQ(_TT("000f"), UString::format(_TT("{0:x4}"), 15));
	}
	// <Test> F
	{
		ASSERT_EQ(_TT("25.187900"), UString::format(_TT("{0:F}"), 25.1879));
		ASSERT_EQ(_TT("25.187900"), UString::format(_TT("{0:f}"), 25.1879));
		ASSERT_EQ(_TT("25.19"), UString::format(_TT("{0:F2}"), 25.1879));
		ASSERT_EQ(_TT("25,187900"), UString::format(Locale(_TT("fr")), _TT("{0:F}"), 25.1879));
	}
	// <Test> E
	{
#if _MSC_VER >= 1900	// VS2015 でちょっと変わった？
		ASSERT_EQ(_TT("1.005000e+02"), UString::format(_TT("{0:e}"), 100.5));
		ASSERT_EQ(_TT("1.005000E+02"), UString::format(_TT("{0:E}"), 100.5));
		ASSERT_EQ(_TT("1.01e+02"), UString::format(_TT("{0:e2}"), 100.5));
		ASSERT_EQ(_TT("1.00500000E+02"), UString::format(_TT("{0:E8}"), 100.5));
#else
		ASSERT_EQ(_TT("1.005000e+002"), UString::format(_TT("{0:e}"), 100.5));
		ASSERT_EQ(_TT("1.005000E+002"), UString::format(_TT("{0:E}"), 100.5));
		ASSERT_EQ(_TT("1.01e+002"), UString::format(_TT("{0:e2}"), 100.5));
		ASSERT_EQ(_TT("1.00500000E+002"), UString::format(_TT("{0:E8}"), 100.5));
#endif
	}

	// <Test> {}
	{
		ASSERT_EQ(_TT("{0}"), UString::format(_TT("{{0}}")));
		ASSERT_EQ(_TT("{1}"), UString::format(_TT("{{{0}}}"), 1));
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Formatter, Illigal)
{
	UString s1;
	// <Test> 引数不足の確認
	{
		s1 = UString::format(_TT("{0}")); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(_TT("{1}"), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
	}
	// <Test> 文法エラーの確認
	{
		s1 = UString::format(_TT("{")); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(_TT("}")); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(_TT("{a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(_TT("a}")); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(_TT("{0,a}"), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(_TT("{0,-a}"), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(_TT("{0, "), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(_TT("{0:"), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(_TT("{0:D"), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(_TT("{0:D "), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Formatter, Examples)
{
	{
		UString name = _TT("file");
		int index = 5;
		UString fileName = UString::format(_TT("{0}_{1}.txt"), name, index);
		ASSERT_EQ(_TT("file_5.txt"), fileName);
	}
	{
		ASSERT_EQ(_TT("12345"), UString::format(_TT("{0:D}"), 12345));
		ASSERT_EQ(_TT("-12345"), UString::format(_TT("{0:d}"), -12345));
		ASSERT_EQ(_TT("00012345"), UString::format(_TT("{0:D8}"), 12345));
	}
	{
#if _MSC_VER >= 1900	// VS2015 でちょっと変わった？
		ASSERT_EQ(_TT("1.234568e+04"), UString::format(_TT("{0:e}"), 12345.6789));
		ASSERT_EQ(_TT("1.2345678900E+04"), UString::format(_TT("{0:E10}"), 12345.6789));
		ASSERT_EQ(_TT("1.2346e+04"), UString::format(_TT("{0:e4}"), 12345.6789));
		ASSERT_EQ(_TT("1,234568E+04"), UString::format(Locale(_TT("fr")), _TT("{0:E}"), 12345.6789));
#else
		ASSERT_EQ(_TT("1.234568e+004"), UString::format(_TT("{0:e}"), 12345.6789));
		ASSERT_EQ(_TT("1.2345678900E+004"), UString::format(_TT("{0:E10}"), 12345.6789));
		ASSERT_EQ(_TT("1.2346e+004"), UString::format(_TT("{0:e4}"), 12345.6789));
		ASSERT_EQ(_TT("1,234568E+004"), UString::format(Locale(_TT("fr")), _TT("{0:E}"), 12345.6789));
#endif
	}

	{
		ASSERT_EQ(_TT("2045e"), UString::format(_TT("{0:x}"), 0x2045e));
		ASSERT_EQ(_TT("2045E"), UString::format(_TT("{0:X}"), 0x2045e));
		ASSERT_EQ(_TT("0002045E"), UString::format(_TT("{0:X8}"), 0x2045e));
		ASSERT_EQ(_TT("0xFF"), UString::format(_TT("0x{0:X}"), 255));
	}
}

