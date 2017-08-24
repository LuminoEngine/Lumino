#include <TestConfig.h>
#include <unordered_map>
#include <regex>
#include <Lumino/Base/StringU.h>

class Test_Base_UString : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

#define NSSO_STR	u"abcdefghij1234567890"

TEST_F(Test_Base_UString, Constructor)
{
	// <Test> SSO
	{
		{
			UString s1(u"abc");
			UString s2(s1, 1);
			UString s3(s1, 1, 1);
			ASSERT_EQ(u"bc", s2);
			ASSERT_EQ(u"b", s3);
			UString s4(UString(u""));
			ASSERT_EQ(u"", s4);
		}
		{
			UString s1;
			UString s2(u"abc");
			UString s3(u"abc", 2);
			UString s5(u"");
			ASSERT_EQ(true, s1.isEmpty());
			ASSERT_EQ(u"abc", s2);
			ASSERT_EQ(u"ab", s3);
			ASSERT_EQ(u"", s5);
		}
		{
			UString s1(3, u'a');
			UString s2(0, u'a');
			ASSERT_EQ(u"aaa", s1);
			ASSERT_EQ(u"", s2);
		}
	}
	// <Test> NonSSO
	{
		{
			UString s1(NSSO_STR);
			UString s2(s1, 1);
			UString s3(s1, 1, 1);
			ASSERT_EQ(u"bc", s2);
			ASSERT_EQ(u"b", s3);
		}
		{
			UString s1;
			UString s2(NSSO_STR);
			UString s3(NSSO_STR, 2);
			UString s5(u"");
			ASSERT_EQ(true, s1.isEmpty());
			ASSERT_EQ(u"abc", s2);
			ASSERT_EQ(u"ab", s3);
			ASSERT_EQ(u"", s5);
		}
		{
			UString s1(20, u'a');
			ASSERT_EQ(u"aaaaaaaaaaaaaaaaaaaa", s1);
		}
	}
}

TEST_F(Test_Base_UString, CopyFramework)
{
	// <Test> copy (SSO)
	{
		UString s1;
		UString s2(s1);
		UString s3(u"abc");
		UString s4(s3);
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(true, s2.isEmpty());
		ASSERT_EQ(u"abc", s3);
		ASSERT_EQ(u"abc", s4);

		s1 = s4;
		ASSERT_EQ(u"abc", s1);
		ASSERT_EQ(u"abc", s4);
	}
	// <Test> copy (Non SSO)
	{
		UString s1;
		UString s2(s1);
		UString s3(NSSO_STR);
		UString s4(s3);
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(true, s2.isEmpty());
		ASSERT_EQ(NSSO_STR, s3);
		ASSERT_EQ(NSSO_STR, s4);

		s1 = s4;
		ASSERT_EQ(NSSO_STR, s1);
		ASSERT_EQ(NSSO_STR, s4);
	}
	// <Test> move (SSO)
	{
		UString s1;
		UString s2(std::move(s1));
		UString s3(u"abc");
		UString s4(std::move(s3));
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(true, s2.isEmpty());
		ASSERT_EQ(true, s3.isEmpty());
		ASSERT_EQ(u"abc", s4);

		s1 = std::move(s4);
		ASSERT_EQ(u"abc", s1);
		ASSERT_EQ(true, s4.isEmpty());
	}
	// <Test> move (Non SSO)
	{
		UString s1;
		UString s2(std::move(s1));
		UString s3(NSSO_STR);
		UString s4(std::move(s3));
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(true, s2.isEmpty());
		ASSERT_EQ(true, s3.isEmpty());
		ASSERT_EQ(NSSO_STR, s4);

		s1 = std::move(s4);
		ASSERT_EQ(NSSO_STR, s1);
		ASSERT_EQ(true, s4.isEmpty());
	}
}

TEST_F(Test_Base_UString, resize)
{
	// <Test>
	{
		UString s1 = u"abd";
		s1.resize(2);
		ASSERT_EQ(u"ab", s1);

		s1.resize(5, u'c');
		ASSERT_EQ(u"abccc", s1);
	}
	// <Test> 内部状態切り替えのテスト
	{
		UString s1, s2;

		// SSO -> SSO
		s1 = u"a";
		s1 += u"b";
		ASSERT_EQ(u"ab", s1);

		// SSO -> NonSSO
		s1 = u"a";
		s1 += NSSO_STR;
		ASSERT_EQ(u"aabcdefghij1234567890", s1);

		// NonSSO -> SSO
		s1 = NSSO_STR;
		s1.resize(1);
		ASSERT_EQ(u"a", s1);

		// NonSSO -> SSO (shared)
		s1 = NSSO_STR;
		s2 = s1;
		s2.resize(1);
		ASSERT_EQ(NSSO_STR, s1);
		ASSERT_EQ(u"a", s2);

		// NonSSO -> NonSSO
		s1 = NSSO_STR;
		s1 += u"a";
		ASSERT_EQ(u"abcdefghij1234567890a", s1);

		// NonSSO -> NonSSO (shared)
		s1 = NSSO_STR;
		s2 = s1;
		s2 += u"a";
		ASSERT_EQ(NSSO_STR, s1);
		ASSERT_EQ(u"abcdefghij1234567890a", s2);

		// NonSSO(init) -> NonSSO
		UString s3;
		s3 += NSSO_STR;
		ASSERT_EQ(NSSO_STR, s3);
	}
}

TEST_F(Test_Base_UString, Operators)
{
	// <Test> =
	{
		UString s1, s2, s3(u"abc");
		s1 = s2;
		s2 = s3;
		s3 = u'a';
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(u"abc", s2);
		ASSERT_EQ(u"a", s3); ASSERT_EQ(1, s3.getLength());
	}
	// <Test> operator=
	{
		UString str;

		// Char
		str = '0';
		ASSERT_EQ(u"0", str);
		str = u'1';
		ASSERT_EQ(u"1", str);

		// Char*
		str = "a";
		ASSERT_EQ(u"a", str);
		str = NSSO_STR;
		ASSERT_EQ(NSSO_STR, str);

		// StringRef
		str = UStringRef(u"b");
		ASSERT_EQ(u"b", str);
		str = UStringRef(NSSO_STR);
		ASSERT_EQ(NSSO_STR, str);

		// <Test> operator= (自己代入)
		str = u"b";
		str = str;
		ASSERT_EQ(u"b", str);
		str = NSSO_STR;
		str = str;
		ASSERT_EQ(NSSO_STR, str);

		// null Char*
		str = ((char16_t*)NULL);
		ASSERT_EQ(true, str.isEmpty());
	}
	// <Test> operator==
	{
		// String
		UString ss = u"a";
		UString s1 = u"a";
		ASSERT_EQ(true, s1 == ss);

		// Char*
		ASSERT_EQ(true, s1 == u"a");
		ASSERT_EQ(true, s1 == "a");

		// self
		ASSERT_EQ(true, s1 == s1);

		// null Char*
		ASSERT_EQ(false, s1 == ((char16_t*)NULL));
		s1.clear();
		ASSERT_EQ(true, s1 == ((char16_t*)NULL));
	}
	// <Test> operator!=
	{
		// String
		UString ss = u"a";
		UString s1 = u"b";
		ASSERT_EQ(true, s1 != ss);

		// Char*
		ASSERT_EQ(true, s1 != u"c");
		ASSERT_EQ(true, s1 != "c");

		// self
		ASSERT_EQ(false, s1 != s1);

		// null Char*
		ASSERT_EQ(true, s1 != ((char16_t*)NULL));
		s1.clear();
		ASSERT_EQ(false, s1 != ((char16_t*)NULL));
	}
	// <Test> operator+=
	{
		UString s1;
		UString s2 = u"a";

		// String
		s1 += s2;
		ASSERT_EQ(u"a", s1);

		// StringRef
		s1 += UStringRef(u"b");
		ASSERT_EQ(u"ab", s1);

		// Char*
		s1 += u"c";
		ASSERT_EQ(u"abc", s1);

		// Char
		s1 += u'd';
		ASSERT_EQ(u"abcd", s1);
	}
	// <Test> operator +
	{
		// String + String
		UString s1;
		s1 = UString(u"a") + UString(u"b");
		ASSERT_EQ(u"ab", s1);

		// String + Char*
		s1 = UString(u"a") + u"b";
		ASSERT_EQ(u"ab", s1);

		// Char* + String
		s1 = u"a" + UString(u"b");
		ASSERT_EQ(u"ab", s1);
	}
	// <Test> operator <
	{
		UString s1 = u"a", s2 = u"A";

		// String
		ASSERT_EQ(true, s2 < s1);
		ASSERT_EQ(false, s2 < UString());

		// Char*
		ASSERT_EQ(true, s1 < u"b");
		ASSERT_EQ(false, s1 < u"");
		ASSERT_EQ(false, s1 < ((char16_t*)NULL));
	}
	// <Test> operator >
	{
		UString s1 = u"a", s2 = u"A";

		// String
		ASSERT_EQ(false, s2 > s1);
		ASSERT_EQ(true, s2 > UString());

		// Char*
		ASSERT_EQ(false, s1 > u"b");
		ASSERT_EQ(true, s1 > u"");
		ASSERT_EQ(true, s1 > ((char16_t*)NULL));
	}
	// <Test> operator <=
	{
		UString s1 = u"a", s2 = u"A", s3 = u"a";

		// String
		ASSERT_EQ(true, s2 <= s1);
		ASSERT_EQ(true, s2 <= s3);

		// Char*
		ASSERT_EQ(false, s1 <= u"A");
		ASSERT_EQ(true, s1 <= u"a");
	}
	// <Test> operator >=
	{
		UString s1 = u"a", s2 = u"A", s3 = u"a";

		// String
		ASSERT_EQ(false, s2 >= s1);
		ASSERT_EQ(false, s2 >= s3);

		// Char*
		ASSERT_EQ(true, s1 >= u"A");
		ASSERT_EQ(true, s1 >= u"a");
	}
}

TEST_F(Test_Base_UString, assignFromCStr)
{
	// <Test> コンストラクタへ const char*, char 指定
	{
		UString s1("abc");
		UString s2(2, 'a');
		ASSERT_EQ(3, s1.getLength());
		ASSERT_EQ(u"abc", s1);
		ASSERT_EQ(u"aa", s2);
	}
	// <Test> 処理系関数へ const char*, char 指定
	{
		UString s1("abc");
		ASSERT_EQ(1, s1.indexOf("b"));
		ASSERT_EQ(1, s1.indexOf('b'));
	}
}

TEST_F(Test_Base_UString, clear)
{
	// <Test> (SSO)
	{
		UString s1, s2(u"abc");
		s1.clear();
		s2.clear();
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(u"", s1);
		ASSERT_EQ(true, s2.isEmpty());
		ASSERT_EQ(u"", s2);
	}
	// <Test> (NonSSO)
	{
		UString s1, s2(NSSO_STR);
		s1.clear();
		s2.clear();
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(u"", s1);
		ASSERT_EQ(true, s2.isEmpty());
		ASSERT_EQ(u"", s2);
	}
	// <Test> SSO <=> NonSSO
	{
		UString s1 = NSSO_STR;
		UString s2 = s1;
		s1.clear();
		ASSERT_EQ(NSSO_STR, s2);
	}
}

TEST_F(Test_Base_UString, indexOf)
{
	UString s1(u"abcdef");
	ASSERT_EQ(1, s1.indexOf(u"bcd"));
	ASSERT_EQ(-1, s1.indexOf(u"a", 1));
	ASSERT_EQ(0, s1.indexOf(u"A", 0, CaseSensitivity::CaseInsensitive));
	ASSERT_EQ(4, s1.indexOf(u'e'));
}

TEST_F(Test_Base_UString, lastIndexOf)
{
	{
		UString str = u"abcdef";
		ASSERT_EQ(3, str.lastIndexOf(u"de"));
		ASSERT_EQ(1, str.lastIndexOf(u"bc", 2));
		ASSERT_EQ(-1, str.lastIndexOf(u"cd", 2));
		ASSERT_EQ(2, str.lastIndexOf(u"cd", 4, 3));
		ASSERT_EQ(-1, str.lastIndexOf(u"bc", 4, 3));
	}
	// .NET の System.String.LastIndexOf() と同様の動作
	{
		UString str = u"crazy azimuths";
		UString se = u"";
		ASSERT_EQ(6, str.lastIndexOf(u"az"));			// 6
		ASSERT_EQ(2, str.lastIndexOf(u"az", 6));		// 2 ("crazy a"
		ASSERT_EQ(6, str.lastIndexOf(u"az", 7));		// 6 ("crazy az"
		ASSERT_EQ(0, str.lastIndexOf(u"cr", 6));		// 0
		ASSERT_EQ(-1, str.lastIndexOf(u"cr", 6, 4));	// -1
		ASSERT_EQ(-1, str.lastIndexOf(u"cr", 6, 6));	// -1
		ASSERT_EQ(0, str.lastIndexOf(u"cr", 6, 7));		// 0
		ASSERT_EQ(5, str.lastIndexOf(u"", 5, 2));		// 5
		ASSERT_EQ(3, str.lastIndexOf(u"", 3, 0));
		ASSERT_EQ(0, se.lastIndexOf(u""));
	}
}

TEST_F(Test_Base_UString, trim)
{
	// 前後
	{
		UString str1(u" abc def ");
		UString t = str1.trim();
		ASSERT_EQ(u"abc def", t);
	}
	// 前だけ
	{
		UString str1(u" abc def");
		UString t = str1.trim();
		ASSERT_EQ(u"abc def", t);
	}
	// 後ろだけ
	{
		UString str1(u"abc def ");
		UString t = str1.trim();
		ASSERT_EQ(u"abc def", t);
	}
	// 空文字
	{
		UString str1(u"");
		UString t = str1.trim();
		ASSERT_EQ(u"", t);
	}
	// 空白だけ
	{
		UString str1(u" ");
		UString t = str1.trim();
		ASSERT_EQ(u"", t);
	}
	// 空白だけ * 2
	{
		UString str1(u"  ");
		UString t = str1.trim();
		ASSERT_EQ(u"", t);
	}
	// 空白だけ * 3
	{
		UString str1(u"   ");
		UString t = str1.trim();
		ASSERT_EQ(u"", t);
	}
}

TEST_F(Test_Base_UString, remove)
{
	UString str1(u"abcdef");

	UString str2 = str1.remove(u'c');
	ASSERT_EQ(u"abdef", str2);

	// 大文字小文字を区別しない
	UString str3 = str2.remove(u"D", CaseSensitivity::CaseInsensitive);
	ASSERT_EQ(u"abef", str3);
}


TEST_F(Test_Base_UString, replace)
{
	// <Test> 部分一致を置換できること。
	{
		UString str1(u"test");
		UString str2 = str1.replace(u"es", u"b");
		ASSERT_EQ(u"tbt", str2);
		UString str3 = str2.replace(u"t", u"");
		ASSERT_EQ(u"b", str3);
		UString str4 = str3.replace(u"b", u"");
		ASSERT_EQ(u"", str4);
	}
	// <Test> 全ての一致を置換できること。
	// <Test> 破壊的変更にならないこと。
	{
		UString str1(u"aaa");
		UString str2 = str1.replace(u"a", u"b");
		ASSERT_EQ(u"bbb", str2);
		ASSERT_EQ(u"aaa", str1);
	}
	// <Test> UString を渡せること。
	{
		UString str1(u"a");
		UString str2 = str1.replace(UString(u"a"), UString(u"b"));
		ASSERT_EQ(u"b", str2);
	}
	// <Test> UStringRef を渡せること。
	{
		UString str1(u"reabcdef");
		UChar buf1[] = u"abc";
		UChar buf2[] = u"def";
		UString str2 = str1.replace(UStringRef(buf1, buf1 + 2), UStringRef(buf2, buf2 + 2));
		ASSERT_EQ(u"redecdef", str2);
	}
	// <Test> 文字列を置換する
	{
		UString str1 = u"abcdcd";

		// "ab" を "12" に置き換える
		str1 = str1.replace(u"ab", u"12");

		// "cd" を "345" に置き換える
		UString from = u"cd";
		UString to = u"345";
		str1 = str1.replace(from, to);

		ASSERT_EQ(u"12345345", str1);
	}
}






TEST_F(Test_Base_UString, concat)
{
	UString s1 = UString::concat(u"a", u"b");
	ASSERT_EQ(u"ab", s1);

	s1 = UString::concat(u"12345678901234567890", u"12345678901234567890");
	ASSERT_EQ(u"1234567890123456789012345678901234567890", s1);
}

TEST_F(Test_Base_UString, Issue)
{
}




class Test_Base_FormatterU : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//---------------------------------------------------------------------
TEST_F(Test_Base_FormatterU, Basic)
{
	ASSERT_EQ(u"", UString::format(u""));
	ASSERT_EQ(u"a", UString::format(u"a"));
	ASSERT_EQ(u"a", UString::format(u"{0}", u"a"));
	ASSERT_EQ(u"ab", UString::format(u"{0}{1}", u"a", u"b"));
	ASSERT_EQ(u"ba", UString::format(u"{1}{0}", u"a", u"b"));
	ASSERT_EQ(u"aba", UString::format(u"{0}{1}{0}", u"a", u"b"));
	ASSERT_EQ(u"{0}", UString::format(u"{{0}}", u"a", u"b"));

	// <Test> インデント
	{
		ASSERT_EQ(u"   a", UString::format(u"{0,4}", u"a"));
		ASSERT_EQ(u"  ab", UString::format(u"{0,4}", u"ab"));
		ASSERT_EQ(u"a   ", UString::format(u"{0,-4}", u"a"));
		ASSERT_EQ(u"ab  ", UString::format(u"{0,-4}", u"ab"));
		ASSERT_EQ(u"   aa   ab  ", UString::format(u"{0,4}{0,-4}{1,-4}", u"a", u"ab"));
		ASSERT_EQ(u"abcd", UString::format(u"{0,3}", u"abcd"));
		ASSERT_EQ(u"abcd", UString::format(u"{0,4}", u"abcd"));
		ASSERT_EQ(u" abcd", UString::format(u"{0,5}", u"abcd"));
		ASSERT_EQ(u"abcd", UString::format(u"{0,-3}", u"abcd"));
		ASSERT_EQ(u"abcd", UString::format(u"{0,-4}", u"abcd"));
		ASSERT_EQ(u"abcd ", UString::format(u"{0,-5}", u"abcd"));
	}
	// <Test> ロケール
	{
		//Locale lc(u"fr");
		//ASSERT_EQ(u"10", UString::format(lc, u"{0}", 10));
		//ASSERT_EQ(u"2147483647", UString::format(u"{0}", INT32_MAX));
	}

	// <Test> char/wchar_t
	{
		ASSERT_EQ(u"a", UString::format(u"{0}", u'a'));
	}
	// <Test> int16_t
	{
		ASSERT_EQ(u"42", UString::format(u"{0}", (int16_t)42));
		ASSERT_EQ(u"32767", UString::format(u"{0}", INT16_MAX));
		ASSERT_EQ(u"-32768", UString::format(u"{0}", INT16_MIN));
	}
	// <Test> uint16_t
	{
		ASSERT_EQ(u"42", UString::format(u"{0}", (uint16_t)42));
		ASSERT_EQ(u"ffff", UString::format(u"{0:x}", UINT16_MAX));
	}
	// <Test> int32_t
	{
		ASSERT_EQ(u"42", UString::format(u"{0}", (int32_t)42));
		ASSERT_EQ(u"2147483647", UString::format(u"{0}", INT32_MAX));
		ASSERT_EQ(u"-2147483648", UString::format(u"{0}", INT32_MIN));
	}
	// <Test> uint32_t
	{
		ASSERT_EQ(u"42", UString::format(u"{0}", (uint32_t)42));
		ASSERT_EQ(u"ffffffff", UString::format(u"{0:x}", UINT32_MAX));
	}
	// <Test> int64_t
	{
		ASSERT_EQ(u"42", UString::format(u"{0}", (int64_t)42));
		ASSERT_EQ(u"9223372036854775807", UString::format(u"{0}", INT64_MAX));
		ASSERT_EQ(u"-9223372036854775808", UString::format(u"{0}", INT64_MIN));
	}
	// <Test> uint64_t
	{
		ASSERT_EQ(u"42", UString::format(u"{0}", (uint64_t)42));
		ASSERT_EQ(u"ffffffffffffffff", UString::format(u"{0:x}", UINT64_MAX));
	}
	// <Test> float
	{
		ASSERT_EQ(u"1.5", UString::format(u"{0}", 1.5f));
	}
	// <Test> double
	{
		ASSERT_EQ(u"1.5", UString::format(u"{0}", 1.5));
	}
	// <Test> bool
	{
		bool a = true;
		ASSERT_EQ(u"True", UString::format(u"{0}", a));
		ASSERT_EQ(u"False", UString::format(u"{0}", false));
	}
	// <Test> char[]/wchar_t[]
	{
		ASSERT_EQ(u"abc", UString::format(u"{0}", u"abc"));
	}
	// <Test> char*/wchar_t*
	{
		UChar buf[] = u"abc";
		UChar* t1 = (UChar*)buf;
		const UChar* t2 = u"def";
		ASSERT_EQ(u"abcdef", UString::format(u"{0}{1}", t1, t2));
	}
#if 0	// TODO:
	// <Test> std::string
	{
		std::basic_string<UChar> s = u"abc";
		ASSERT_EQ(u"abc", UString::format(u"{0}", s));
	}
#endif
	// <Test> String
	{
		UString s = u"abc";
		ASSERT_EQ(u"abc", UString::format(u"{0}", s));
	}
#if 0	// TODO:
	// <Test> PathName
	{
		PathName s = u"abc";
		ASSERT_EQ(u"abc", UString::format(u"{0}", s));
	}
#endif

	//auto aa = UString::format(Locale::GetDefault(), u"{0}", 12445.6789);

	// <Test> D
	{
		ASSERT_EQ(u"15", UString::format(u"{0:D}", 15));
		ASSERT_EQ(u"15", UString::format(u"{0:d}", 15));
		ASSERT_EQ(u"0015", UString::format(u"{0:D4}", 15));
	}
	// <Test> X
	{
		ASSERT_EQ(u"F", UString::format(u"{0:X}", 15));
		ASSERT_EQ(u"f", UString::format(u"{0:x}", 15));
		ASSERT_EQ(u"000f", UString::format(u"{0:x4}", 15));
	}
	// <Test> F
	{
		ASSERT_EQ(u"25.187900", UString::format(u"{0:F}", 25.1879));
		ASSERT_EQ(u"25.187900", UString::format(u"{0:f}", 25.1879));
		ASSERT_EQ(u"25.19", UString::format(u"{0:F2}", 25.1879));
		ASSERT_EQ(u"25,187900", UString::format(Locale(_T("fr")), u"{0:F}", 25.1879));
	}
	// <Test> E
	{
#if _MSC_VER >= 1900	// VS2015 でちょっと変わった？
		ASSERT_EQ(u"1.005000e+02", UString::format(u"{0:e}", 100.5));
		ASSERT_EQ(u"1.005000E+02", UString::format(u"{0:E}", 100.5));
		ASSERT_EQ(u"1.01e+02", UString::format(u"{0:e2}", 100.5));
		ASSERT_EQ(u"1.00500000E+02", UString::format(u"{0:E8}", 100.5));
#else
		ASSERT_EQ(u"1.005000e+002", UString::format(u"{0:e}", 100.5));
		ASSERT_EQ(u"1.005000E+002", UString::format(u"{0:E}", 100.5));
		ASSERT_EQ(u"1.01e+002", UString::format(u"{0:e2}", 100.5));
		ASSERT_EQ(u"1.00500000E+002", UString::format(u"{0:E8}", 100.5));
#endif
	}

	// <Test> {}
	{
		ASSERT_EQ(u"{0}", UString::format(u"{{0}}"));
		ASSERT_EQ(u"{1}", UString::format(u"{{{0}}}", 1));
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Base_FormatterU, Illigal)
{
	UString s1;
	// <Test> 引数不足の確認
	{
		s1 = UString::format(u"{0}"); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(u"{1}", u"a"); ASSERT_EQ(true, s1.isEmpty());
	}
	// <Test> 文法エラーの確認
	{
		s1 = UString::format(u"{"); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(u"}"); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(u"{a"); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(u"a}"); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(u"{0,a}", u"a"); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(u"{0,-a}", u"a"); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(u"{0, ", u"a"); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(u"{0:", u"a"); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(u"{0:D", u"a"); ASSERT_EQ(true, s1.isEmpty());
		s1 = UString::format(u"{0:D ", u"a"); ASSERT_EQ(true, s1.isEmpty());
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Base_FormatterU, Examples)
{
	{
		UString name = u"file";
		int index = 5;
		UString fileName = UString::format(u"{0}_{1}.txt", name, index);
		ASSERT_EQ(u"file_5.txt", fileName);
	}
	{
		ASSERT_EQ(u"12345", UString::format(u"{0:D}", 12345));
		ASSERT_EQ(u"-12345", UString::format(u"{0:d}", -12345));
		ASSERT_EQ(u"00012345", UString::format(u"{0:D8}", 12345));
	}
	{
#if _MSC_VER >= 1900	// VS2015 でちょっと変わった？
		ASSERT_EQ(u"1.234568e+04", UString::format(u"{0:e}", 12345.6789));
		ASSERT_EQ(u"1.2345678900E+04", UString::format(u"{0:E10}", 12345.6789));
		ASSERT_EQ(u"1.2346e+04", UString::format(u"{0:e4}", 12345.6789));
		ASSERT_EQ(u"1,234568E+04", UString::format(Locale(_T("fr")), u"{0:E}", 12345.6789));
#else
		ASSERT_EQ(u"1.234568e+004", UString::format(u"{0:e}", 12345.6789));
		ASSERT_EQ(u"1.2345678900E+004", UString::format(u"{0:E10}", 12345.6789));
		ASSERT_EQ(u"1.2346e+004", UString::format(u"{0:e4}", 12345.6789));
		ASSERT_EQ(u"1,234568E+004", UString::format(Locale(u"fr"), u"{0:E}", 12345.6789));
#endif
	}

	{
		ASSERT_EQ(u"2045e", UString::format(u"{0:x}", 0x2045e));
		ASSERT_EQ(u"2045E", UString::format(u"{0:X}", 0x2045e));
		ASSERT_EQ(u"0002045E", UString::format(u"{0:X8}", 0x2045e));
		ASSERT_EQ(u"0xFF", UString::format(u"0x{0:X}", 255));
	}
}
