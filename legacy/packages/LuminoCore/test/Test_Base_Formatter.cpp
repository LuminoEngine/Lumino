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
	//ln::format(_TT("{}"), st);

	ASSERT_EQ(_TT(""), ln::format(_TT("")));
	ASSERT_EQ(_TT("a"), ln::format(_TT("a")));
	ASSERT_EQ(_TT("a"), ln::format(_TT("{0}"), _TT("a")));
	ASSERT_EQ(_TT("ab"), ln::format(_TT("{0}{1}"), _TT("a"), _TT("b")));
	ASSERT_EQ(_TT("ba"), ln::format(_TT("{1}{0}"), _TT("a"), _TT("b")));
	ASSERT_EQ(_TT("aba"), ln::format(_TT("{0}{1}{0}"), _TT("a"), _TT("b")));
	ASSERT_EQ(_TT("{0}"), ln::format(_TT("{{0}}"), _TT("a"), _TT("b")));

	// <Test> インデント
	{
		ASSERT_EQ(_TT("   1"), ln::format(_TT("{0:4}"), 1));
		ASSERT_EQ(_TT("a   "), ln::format(_TT("{0:4}"), _TT("a")));
		ASSERT_EQ(_TT("  ab"), ln::format(_TT("{0:>4}"), _TT("ab")));
		ASSERT_EQ(_TT("a   "), ln::format(_TT("{0:<4}"), _TT("a")));
		ASSERT_EQ(_TT("ab  "), ln::format(_TT("{0:<4}"), _TT("ab")));
		ASSERT_EQ(_TT("   aa   ab  "), ln::format(_TT("{0:>4}{0:<4}{1:<4}"), _TT("a"), _TT("ab")));
		ASSERT_EQ(_TT("abcd"), ln::format(_TT("{0:3}"), _TT("abcd")));
		ASSERT_EQ(_TT("abcd"), ln::format(_TT("{0:4}"), _TT("abcd")));
		ASSERT_EQ(_TT(" abcd"), ln::format(_TT("{0:>5}"), _TT("abcd")));
		ASSERT_EQ(_TT("abcd"), ln::format(_TT("{0:>3}"), _TT("abcd")));
		ASSERT_EQ(_TT("abcd"), ln::format(_TT("{0:>4}"), _TT("abcd")));
		ASSERT_EQ(_TT("abcd "), ln::format(_TT("{0:<5}"), _TT("abcd")));
	}
	// <Test> ロケール
	{
		//Locale lc(u"fr");
		//ASSERT_EQ(u"10", ln::format(lc, _TT("{0}"), 10));
		//ASSERT_EQ(u"2147483647", ln::format(_TT("{0}"), INT32_MAX));
	}

	// <Test> char/wchar_t
	{
		ASSERT_EQ(_TT("a"), ln::format(_TT("{0}"), _TT('a')));
	}
	// <Test> int16_t
	{
		ASSERT_EQ(_TT("42"), ln::format(_TT("{0}"), (int16_t)42));
		ASSERT_EQ(_TT("32767"), ln::format(_TT("{0}"), INT16_MAX));
		ASSERT_EQ(_TT("-32768"), ln::format(_TT("{0}"), INT16_MIN));
	}
	// <Test> uint16_t
	{
		ASSERT_EQ(_TT("42"), ln::format(_TT("{0}"), (uint16_t)42));
		ASSERT_EQ(_TT("ffff"), ln::format(_TT("{0:x}"), UINT16_MAX));
	}
	// <Test> int32_t
	{
		ASSERT_EQ(_TT("42"), ln::format(_TT("{0}"), (int32_t)42));
		ASSERT_EQ(_TT("2147483647"), ln::format(_TT("{0}"), INT32_MAX));
		ASSERT_EQ(_TT("-2147483648"), ln::format(_TT("{0}"), INT32_MIN));
	}
	// <Test> uint32_t
	{
		ASSERT_EQ(_TT("42"), ln::format(_TT("{0}"), (uint32_t)42));
		ASSERT_EQ(_TT("ffffffff"), ln::format(_TT("{0:x}"), UINT32_MAX));
	}
	// <Test> int64_t
	{
		ASSERT_EQ(_TT("42"), ln::format(_TT("{0}"), (int64_t)42));
		ASSERT_EQ(_TT("9223372036854775807"), ln::format(_TT("{0}"), INT64_MAX));
		ASSERT_EQ(_TT("-9223372036854775808"), ln::format(_TT("{0}"), INT64_MIN));
	}
	// <Test> uint64_t
	{
		ASSERT_EQ(_TT("42"), ln::format(_TT("{0}"), (uint64_t)42));
		ASSERT_EQ(_TT("ffffffffffffffff"), ln::format(_TT("{0:x}"), UINT64_MAX));
	}
	// <Test> float
	{
		ASSERT_EQ(_TT("1.5"), ln::format(_TT("{0}"), 1.5f));
	}
	// <Test> double
	{
		ASSERT_EQ(_TT("1.5"), ln::format(_TT("{0}"), 1.5));
	}
	// <Test> bool
	{
		bool a = true;
		ASSERT_EQ(_TT("true"), ln::format(_TT("{0}"), a));
		ASSERT_EQ(_TT("false"), ln::format(_TT("{0}"), false));
	}
	// <Test> char[]/wchar_t[]
	{
		ASSERT_EQ(_TT("abc"), ln::format(_TT("{0}"), _TT("abc")));
	}
	// <Test> char*/wchar_t*
	{
		Char buf[] = _TT("abc");
		Char* t1 = (Char*)buf;
		const Char* t2 = _TT("def");
		ASSERT_EQ(_TT("abcdef"), ln::format(_TT("{0}{1}"), t1, t2));
	}
#if 0	// TODO:
	// <Test> std::string
	{
		std::basic_string<Char> s = _TT("abc");
		ASSERT_EQ(_TT("abc"), ln::format(_TT("{0}"), s));
	}
#endif
	// <Test> String
	{
		String s = _TT("abc");
		ASSERT_EQ(_TT("abc"), ln::format(_TT("{0}"), s));
	}
#if 0	// TODO:
	// <Test> Path
	{
		Path s = _TT("abc");
		ASSERT_EQ(_TT("abc"), ln::format(_TT("{0}"), s));
	}
#endif

	//auto aa = ln::format(Locale::GetDefault(), _TT("{0}"), 12445.6789);

	// <Test> D
	{
		ASSERT_EQ(_TT("15"), ln::format(_TT("{0:d}"), 15));
		ASSERT_EQ(_TT("0015"), ln::format(_TT("{0:04d}"), 15));
	}
	// <Test> X
	{
		ASSERT_EQ(_TT("F"), ln::format(_TT("{0:X}"), 15));
		ASSERT_EQ(_TT("f"), ln::format(_TT("{0:x}"), 15));
		ASSERT_EQ(_TT("000f"), ln::format(_TT("{0:04x}"), 15));
	}
	// <Test> F
	{
		ASSERT_EQ(_TT("25.187900"), ln::format(_TT("{0:F}"), 25.1879));
		ASSERT_EQ(_TT("25.187900"), ln::format(_TT("{0:f}"), 25.1879));
		ASSERT_EQ(_TT("25.19"), ln::format(_TT("{0:.2F}"), 25.1879));
#ifdef LN_WIN32
		ASSERT_EQ(_TT("25,187900"), ln::format(Locale(_TT("fr")), _TT("{0:F}"), 25.1879));
#endif
	}
	// <Test> E
	{
		// 環境によっていろいろ異なるので簡易的に見る
		ASSERT_EQ(true, ln::format(_TT("{0:e}"), 100.5).contains(_TT("e+0")));	// => "1.005000e+02" 
		ASSERT_EQ(true, ln::format(_TT("{0:E}"), 100.5).contains(_TT("E+0")));	// => "1.005000E+02" 
//#if _MSC_VER >= 1900 || defined(__EMSCRIPTEN__)	// VS2015 でちょっと変わった？, Emscripten もこっち
//		std::cout << ln::format(_TT("{0:e2}"), 100.5).toStdString() << std::endl;
//		ASSERT_EQ(_TT("1.005000e+02"), ln::format(_TT("{0:e}"), 100.5));
//		ASSERT_EQ(_TT("1.005000E+02"), ln::format(_TT("{0:E}"), 100.5));
//		ASSERT_EQ(_TT("1.01e+02"), ln::format(_TT("{0:e2}"), 100.5));
//		ASSERT_EQ(_TT("1.00500000E+02"), ln::format(_TT("{0:E8}"), 100.5));
//#else
//		ASSERT_EQ(_TT("1.005000e+002"), ln::format(_TT("{0:e}"), 100.5));
//		ASSERT_EQ(_TT("1.005000E+002"), ln::format(_TT("{0:E}"), 100.5));
//		ASSERT_EQ(_TT("1.01e+002"), ln::format(_TT("{0:e2}"), 100.5));
//		ASSERT_EQ(_TT("1.00500000E+002"), ln::format(_TT("{0:E8}"), 100.5));
//#endif
	}

	// <Test> {}
	{
		ASSERT_EQ(_TT("{0}"), ln::format(_TT("{{0}}")));
		ASSERT_EQ(_TT("{1}"), ln::format(_TT("{{{0}}}"), 1));
	}
}

TEST_F(Test_Base_Formatter, AutomaticFieldNumbering)
{
	ASSERT_EQ(_TT("1-2"), ln::format(_TT("{}-{}"), 1, 2));
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Formatter, Illigal)
{
	auto oldHandler = Exception::notificationHandler();
	Exception::setNotificationHandler([](Exception& e) { return true; });

	String s1;
	// <Test> 引数不足の確認
	{
		s1 = ln::format(_TT("{0}")); ASSERT_EQ(true, s1.isEmpty());
		s1 = ln::format(_TT("{1}"), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
	}
	// <Test> 文法エラーの確認
	{
		s1 = ln::format(_TT("{")); ASSERT_EQ(true, s1.isEmpty());
		s1 = ln::format(_TT("}")); ASSERT_EQ(true, s1.isEmpty());
		s1 = ln::format(_TT("{a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = ln::format(_TT("a}")); ASSERT_EQ(true, s1.isEmpty());
		s1 = ln::format(_TT("{0,a}"), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = ln::format(_TT("{0,-a}"), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = ln::format(_TT("{0, "), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = ln::format(_TT("{0:"), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = ln::format(_TT("{0:D"), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
		s1 = ln::format(_TT("{0:D "), _TT("a")); ASSERT_EQ(true, s1.isEmpty());
	}

	Exception::setNotificationHandler(oldHandler);
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Formatter, Examples)
{
	{
		String name = _TT("file");
		int index = 5;
		String fileName = ln::format(_TT("{0}_{1}.txt"), name, index);
		ASSERT_EQ(_TT("file_5.txt"), fileName);
	}
	{
		ASSERT_EQ(_TT("12345"), ln::format(_TT("{0:d}"), 12345));
		ASSERT_EQ(_TT("-12345"), ln::format(_TT("{0:d}"), -12345));
		ASSERT_EQ(_TT("00012345"), ln::format(_TT("{0:08d}"), 12345));
	}
//	{
//#if _MSC_VER >= 1900	// VS2015 でちょっと変わった？
//		ASSERT_EQ(_TT("1.234568e+04"), ln::format(_TT("{0:e}"), 12345.6789));
//		ASSERT_EQ(_TT("1.2345678900E+04"), ln::format(_TT("{0:E10}"), 12345.6789));
//		ASSERT_EQ(_TT("1.2346e+04"), ln::format(_TT("{0:e4}"), 12345.6789));
//#else
//		ASSERT_EQ(_TT("1.234568e+004"), ln::format(_TT("{0:e}"), 12345.6789));
//		ASSERT_EQ(_TT("1.2345678900E+004"), ln::format(_TT("{0:E10}"), 12345.6789));
//		ASSERT_EQ(_TT("1.2346e+004"), ln::format(_TT("{0:e4}"), 12345.6789));
//#endif
//	}

	{
		ASSERT_EQ(_TT("2045e"), ln::format(_TT("{0:x}"), 0x2045e));
		ASSERT_EQ(_TT("2045E"), ln::format(_TT("{0:X}"), 0x2045e));
		ASSERT_EQ(_TT("0002045E"), ln::format(_TT("{0:08X}"), 0x2045e));
		ASSERT_EQ(_TT("0xFF"), ln::format(_TT("0x{0:X}"), 255));
	}
}
