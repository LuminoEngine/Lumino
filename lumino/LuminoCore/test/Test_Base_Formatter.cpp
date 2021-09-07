#include "Common.hpp"
#include <LuminoCore/Base/String.hpp>

class Test_Base_Formatter : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//---------------------------------------------------------------------
TEST_F(Test_Base_Formatter, Basic)
{
	//struct LocalSt { int a; } st;
	//String::format(_TT("{}"), st);

	ASSERT_EQ(_TT(""), String::format(_TT("")));
	ASSERT_EQ(_TT("a"), String::format(_TT("a")));
	ASSERT_EQ(_TT("a"), String::format(_TT("{0}"), _TT("a")));
	ASSERT_EQ(_TT("ab"), String::format(_TT("{0}{1}"), _TT("a"), _TT("b")));
	ASSERT_EQ(_TT("ba"), String::format(_TT("{1}{0}"), _TT("a"), _TT("b")));
	ASSERT_EQ(_TT("aba"), String::format(_TT("{0}{1}{0}"), _TT("a"), _TT("b")));
	ASSERT_EQ(_TT("{0}"), String::format(_TT("{{0}}"), _TT("a"), _TT("b")));

	// <Test> インデント
	{
		ASSERT_EQ(_TT("   1"), String::format(_TT("{0:4}"), 1));
		ASSERT_EQ(_TT("a   "), String::format(_TT("{0:4}"), _TT("a")));
		ASSERT_EQ(_TT("  ab"), String::format(_TT("{0:>4}"), _TT("ab")));
		ASSERT_EQ(_TT("a   "), String::format(_TT("{0:<4}"), _TT("a")));
		ASSERT_EQ(_TT("ab  "), String::format(_TT("{0:<4}"), _TT("ab")));
		ASSERT_EQ(_TT("   aa   ab  "), String::format(_TT("{0:>4}{0:<4}{1:<4}"), _TT("a"), _TT("ab")));
		ASSERT_EQ(_TT("abcd"), String::format(_TT("{0:3}"), _TT("abcd")));
		ASSERT_EQ(_TT("abcd"), String::format(_TT("{0:4}"), _TT("abcd")));
		ASSERT_EQ(_TT(" abcd"), String::format(_TT("{0:>5}"), _TT("abcd")));
		ASSERT_EQ(_TT("abcd"), String::format(_TT("{0:>3}"), _TT("abcd")));
		ASSERT_EQ(_TT("abcd"), String::format(_TT("{0:>4}"), _TT("abcd")));
		ASSERT_EQ(_TT("abcd "), String::format(_TT("{0:<5}"), _TT("abcd")));
	}
	// <Test> ロケール
	{
		//Locale lc(u"fr");
		//ASSERT_EQ(u"10", String::format(lc, _TT("{0}"), 10));
		//ASSERT_EQ(u"2147483647", String::format(_TT("{0}"), INT32_MAX));
	}

	// <Test> char/wchar_t
	{
		ASSERT_EQ(_TT("a"), String::format(_TT("{0}"), _TT('a')));
	}
	// <Test> int16_t
	{
		ASSERT_EQ(_TT("42"), String::format(_TT("{0}"), (int16_t)42));
		ASSERT_EQ(_TT("32767"), String::format(_TT("{0}"), INT16_MAX));
		ASSERT_EQ(_TT("-32768"), String::format(_TT("{0}"), INT16_MIN));
	}
	// <Test> uint16_t
	{
		ASSERT_EQ(_TT("42"), String::format(_TT("{0}"), (uint16_t)42));
		ASSERT_EQ(_TT("ffff"), String::format(_TT("{0:x}"), UINT16_MAX));
	}
	// <Test> int32_t
	{
		ASSERT_EQ(_TT("42"), String::format(_TT("{0}"), (int32_t)42));
		ASSERT_EQ(_TT("2147483647"), String::format(_TT("{0}"), INT32_MAX));
		ASSERT_EQ(_TT("-2147483648"), String::format(_TT("{0}"), INT32_MIN));
	}
	// <Test> uint32_t
	{
		ASSERT_EQ(_TT("42"), String::format(_TT("{0}"), (uint32_t)42));
		ASSERT_EQ(_TT("ffffffff"), String::format(_TT("{0:x}"), UINT32_MAX));
	}
	// <Test> int64_t
	{
		ASSERT_EQ(_TT("42"), String::format(_TT("{0}"), (int64_t)42));
		ASSERT_EQ(_TT("9223372036854775807"), String::format(_TT("{0}"), INT64_MAX));
		ASSERT_EQ(_TT("-9223372036854775808"), String::format(_TT("{0}"), INT64_MIN));
	}
	// <Test> uint64_t
	{
		ASSERT_EQ(_TT("42"), String::format(_TT("{0}"), (uint64_t)42));
		ASSERT_EQ(_TT("ffffffffffffffff"), String::format(_TT("{0:x}"), UINT64_MAX));
	}
	// <Test> float
	{
		ASSERT_EQ(_TT("1.5"), String::format(_TT("{0}"), 1.5f));
	}
	// <Test> double
	{
		ASSERT_EQ(_TT("1.5"), String::format(_TT("{0}"), 1.5));
	}
	// <Test> bool
	{
		bool a = true;
		ASSERT_EQ(_TT("true"), String::format(_TT("{0}"), a));
		ASSERT_EQ(_TT("false"), String::format(_TT("{0}"), false));
	}
	// <Test> char[]/wchar_t[]
	{
		ASSERT_EQ(_TT("abc"), String::format(_TT("{0}"), _TT("abc")));
	}
	// <Test> char*/wchar_t*
	{
		Char buf[] = _TT("abc");
		Char* t1 = (Char*)buf;
		const Char* t2 = _TT("def");
		ASSERT_EQ(_TT("abcdef"), String::format(_TT("{0}{1}"), t1, t2));
	}
#if 0	// TODO:
	// <Test> std::string
	{
		std::basic_string<Char> s = _TT("abc");
		ASSERT_EQ(_TT("abc"), String::format(_TT("{0}"), s));
	}
#endif
	// <Test> String
	{
		String s = _TT("abc");
		ASSERT_EQ(_TT("abc"), String::format(_TT("{0}"), s));
	}
#if 0	// TODO:
	// <Test> Path
	{
		Path s = _TT("abc");
		ASSERT_EQ(_TT("abc"), String::format(_TT("{0}"), s));
	}
#endif

	//auto aa = String::format(Locale::GetDefault(), _TT("{0}"), 12445.6789);

	// <Test> D
	{
		ASSERT_EQ(_TT("15"), String::format(_TT("{0:d}"), 15));
		ASSERT_EQ(_TT("0015"), String::format(_TT("{0:04d}"), 15));
	}
	// <Test> X
	{
		ASSERT_EQ(_TT("F"), String::format(_TT("{0:X}"), 15));
		ASSERT_EQ(_TT("f"), String::format(_TT("{0:x}"), 15));
		ASSERT_EQ(_TT("000f"), String::format(_TT("{0:04x}"), 15));
	}
	// <Test> F
	{
		ASSERT_EQ(_TT("25.187900"), String::format(_TT("{0:F}"), 25.1879));
		ASSERT_EQ(_TT("25.187900"), String::format(_TT("{0:f}"), 25.1879));
		ASSERT_EQ(_TT("25.19"), String::format(_TT("{0:.2F}"), 25.1879));
#ifdef LN_WIN32
		ASSERT_EQ(_TT("25,187900"), String::format(Locale(_TT("fr")), _TT("{0:F}"), 25.1879));
#endif
	}
	// <Test> E
	{
		// 環境によっていろいろ異なるので簡易的に見る
		ASSERT_EQ(true, String::format(_TT("{0:e}"), 100.5).contains(_TT("e+0")));	// => "1.005000e+02" 
		ASSERT_EQ(true, String::format(_TT("{0:E}"), 100.5).contains(_TT("E+0")));	// => "1.005000E+02" 
//#if _MSC_VER >= 1900 || defined(__EMSCRIPTEN__)	// VS2015 でちょっと変わった？, Emscripten もこっち
//		std::cout << String::format(_TT("{0:e2}"), 100.5).toStdString() << std::endl;
//		ASSERT_EQ(_TT("1.005000e+02"), String::format(_TT("{0:e}"), 100.5));
//		ASSERT_EQ(_TT("1.005000E+02"), String::format(_TT("{0:E}"), 100.5));
//		ASSERT_EQ(_TT("1.01e+02"), String::format(_TT("{0:e2}"), 100.5));
//		ASSERT_EQ(_TT("1.00500000E+02"), String::format(_TT("{0:E8}"), 100.5));
//#else
//		ASSERT_EQ(_TT("1.005000e+002"), String::format(_TT("{0:e}"), 100.5));
//		ASSERT_EQ(_TT("1.005000E+002"), String::format(_TT("{0:E}"), 100.5));
//		ASSERT_EQ(_TT("1.01e+002"), String::format(_TT("{0:e2}"), 100.5));
//		ASSERT_EQ(_TT("1.00500000E+002"), String::format(_TT("{0:E8}"), 100.5));
//#endif
	}

	// <Test> {}
	{
		ASSERT_EQ(_TT("{0}"), String::format(_TT("{{0}}")));
		ASSERT_EQ(_TT("{1}"), String::format(_TT("{{{0}}}"), 1));
	}
}

TEST_F(Test_Base_Formatter, AutomaticFieldNumbering)
{
	ASSERT_EQ(_TT("1-2"), String::format(_TT("{}-{}"), 1, 2));
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Formatter, Illigal)
{
	auto oldHandler = Exception::notificationHandler();
	Exception::setNotificationHandler([](Exception& e) { return true; });

	String s1;
	// <Test> 引数不足の確認
	{
		s1 = String::format(_TT("{0}")); ASSERT_EQ(true, s1.isEmpty());
		s1 = String::format(_TT("{1}"), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
	}
	// <Test> 文法エラーの確認
	{
		s1 = String::format(_TT("{")); ASSERT_EQ(true, s1.isEmpty());
		s1 = String::format(_TT("}")); ASSERT_EQ(true, s1.isEmpty());
		s1 = String::format(_TT("{a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = String::format(_TT("a}")); ASSERT_EQ(true, s1.isEmpty());
		s1 = String::format(_TT("{0,a}"), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = String::format(_TT("{0,-a}"), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = String::format(_TT("{0, "), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = String::format(_TT("{0:"), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = String::format(_TT("{0:D"), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = String::format(_TT("{0:D "), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
	}

	Exception::setNotificationHandler(oldHandler);
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Formatter, Examples)
{
	{
		String name = _TT("file");
		int index = 5;
		String fileName = String::format(_TT("{0}_{1}.txt"), name, index);
		ASSERT_EQ(_TT("file_5.txt"), fileName);
	}
	{
		ASSERT_EQ(_TT("12345"), String::format(_TT("{0:d}"), 12345));
		ASSERT_EQ(_TT("-12345"), String::format(_TT("{0:d}"), -12345));
		ASSERT_EQ(_TT("00012345"), String::format(_TT("{0:08d}"), 12345));
	}
//	{
//#if _MSC_VER >= 1900	// VS2015 でちょっと変わった？
//		ASSERT_EQ(_TT("1.234568e+04"), String::format(_TT("{0:e}"), 12345.6789));
//		ASSERT_EQ(_TT("1.2345678900E+04"), String::format(_TT("{0:E10}"), 12345.6789));
//		ASSERT_EQ(_TT("1.2346e+04"), String::format(_TT("{0:e4}"), 12345.6789));
//#else
//		ASSERT_EQ(_TT("1.234568e+004"), String::format(_TT("{0:e}"), 12345.6789));
//		ASSERT_EQ(_TT("1.2345678900E+004"), String::format(_TT("{0:E10}"), 12345.6789));
//		ASSERT_EQ(_TT("1.2346e+004"), String::format(_TT("{0:e4}"), 12345.6789));
//#endif
//	}

	{
		ASSERT_EQ(_TT("2045e"), String::format(_TT("{0:x}"), 0x2045e));
		ASSERT_EQ(_TT("2045E"), String::format(_TT("{0:X}"), 0x2045e));
		ASSERT_EQ(_TT("0002045E"), String::format(_TT("{0:08X}"), 0x2045e));
		ASSERT_EQ(_TT("0xFF"), String::format(_TT("0x{0:X}"), 255));
	}
}
