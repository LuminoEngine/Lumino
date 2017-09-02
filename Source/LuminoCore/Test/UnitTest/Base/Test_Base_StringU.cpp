#include <TestConfig.h>
#include <unordered_map>
#include <regex>
#include <Lumino/Base/Locale.h>
#include <Lumino/Base/StringU.h>

class Test_Base_UString : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

#define NSSO_STR	_TT("abcdefghij1234567890")

TEST_F(Test_Base_UString, Constructor)
{
	// <Test> SSO
	{
		{
			UString s1(_TT("abc"));
			UString s2(s1, 1);
			UString s3(s1, 1, 1);
			ASSERT_EQ(_TT("bc"), s2);
			ASSERT_EQ(_TT("b"), s3);
			UString s4(UString(_TT("")));
			ASSERT_EQ(_TT(""), s4);
		}
		{
			UString s1;
			UString s2(_TT("abc"));
			UString s3(_TT("abc"), 2);
			UString s5(_TT(""));
			ASSERT_EQ(true, s1.isEmpty());
			ASSERT_EQ(_TT("abc"), s2);
			ASSERT_EQ(_TT("ab"), s3);
			ASSERT_EQ(_TT(""), s5);
		}
		{
			UString s1(3, _TT('a'));
			UString s2(0, _TT('a'));
			ASSERT_EQ(_TT("aaa"), s1);
			ASSERT_EQ(_TT(""), s2);
		}
	}
	// <Test> NonSSO
	{
		{
			UString s1(NSSO_STR);
			UString s2(s1, 1);
			UString s3(s1, 1, 1);
			ASSERT_EQ(_TT("bc"), s2);
			ASSERT_EQ(_TT("b"), s3);
		}
		{
			UString s1;
			UString s2(NSSO_STR);
			UString s3(NSSO_STR, 2);
			UString s5(_TT(""));
			ASSERT_EQ(true, s1.isEmpty());
			ASSERT_EQ(_TT("abc"), s2);
			ASSERT_EQ(_TT("ab"), s3);
			ASSERT_EQ(_TT(""), s5);
		}
		{
			UString s1(20, _TT('a'));
			ASSERT_EQ(_TT("aaaaaaaaaaaaaaaaaaaa"), s1);
		}
	}
}

TEST_F(Test_Base_UString, CopyFramework)
{
	// <Test> copy (SSO)
	{
		UString s1;
		UString s2(s1);
		UString s3(_TT("abc"));
		UString s4(s3);
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(true, s2.isEmpty());
		ASSERT_EQ(_TT("abc"), s3);
		ASSERT_EQ(_TT("abc"), s4);

		s1 = s4;
		ASSERT_EQ(_TT("abc"), s1);
		ASSERT_EQ(_TT("abc"), s4);
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
		UString s3(_TT("abc"));
		UString s4(std::move(s3));
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(true, s2.isEmpty());
		ASSERT_EQ(true, s3.isEmpty());
		ASSERT_EQ(_TT("abc"), s4);

		s1 = std::move(s4);
		ASSERT_EQ(_TT("abc"), s1);
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
		UString s1 = _TT("abd");
		s1.resize(2);
		ASSERT_EQ(_TT("ab"), s1);

		s1.resize(5, _TT('c'));
		ASSERT_EQ(_TT("abccc"), s1);
	}
	// <Test> 内部状態切り替えのテスト
	{
		UString s1, s2;

		// SSO -> SSO
		s1 = _TT("a");
		s1 += _TT("b");
		ASSERT_EQ(_TT("ab"), s1);

		// SSO -> NonSSO
		s1 = _TT("a");
		s1 += NSSO_STR;
		ASSERT_EQ(_TT("aabcdefghij1234567890"), s1);

		// NonSSO -> SSO
		s1 = NSSO_STR;
		s1.resize(1);
		ASSERT_EQ(_TT("a"), s1);

		// NonSSO -> SSO (shared)
		s1 = NSSO_STR;
		s2 = s1;
		s2.resize(1);
		ASSERT_EQ(NSSO_STR, s1);
		ASSERT_EQ(_TT("a"), s2);

		// NonSSO -> NonSSO
		s1 = NSSO_STR;
		s1 += _TT("a");
		ASSERT_EQ(_TT("abcdefghij1234567890a"), s1);

		// NonSSO -> NonSSO (shared)
		s1 = NSSO_STR;
		s2 = s1;
		s2 += _TT("a");
		ASSERT_EQ(NSSO_STR, s1);
		ASSERT_EQ(_TT("abcdefghij1234567890a"), s2);

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
		UString s1, s2, s3(_TT("abc"));
		s1 = s2;
		s2 = s3;
		s3 = _TT('a');
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(_TT("abc"), s2);
		ASSERT_EQ(_TT("a"), s3); ASSERT_EQ(1, s3.getLength());
	}
	// <Test> operator=
	{
		UString str;

		// Char
		str = '0';
		ASSERT_EQ(_TT("0"), str);
		str = _TT('1');
		ASSERT_EQ(_TT("1"), str);

		// Char*
		str = "a";
		ASSERT_EQ(_TT("a"), str);
		str = NSSO_STR;
		ASSERT_EQ(NSSO_STR, str);

		// StringRef
		str = UStringRef(_TT("b"));
		ASSERT_EQ(_TT("b"), str);
		str = UStringRef(NSSO_STR);
		ASSERT_EQ(NSSO_STR, str);

		// <Test> operator= (自己代入)
		str = _TT("b");
		str = str;
		ASSERT_EQ(_TT("b"), str);
		str = NSSO_STR;
		str = str;
		ASSERT_EQ(NSSO_STR, str);

		// null Char*
		str = ((Char*)NULL);
		ASSERT_EQ(true, str.isEmpty());
	}
	// <Test> operator==
	{
		// String
		UString ss = _TT("a");
		UString s1 = _TT("a");
		ASSERT_EQ(true, s1 == ss);

		// Char*
		ASSERT_EQ(true, s1 == _TT("a"));
		ASSERT_EQ(true, s1 == "a");

		// self
		ASSERT_EQ(true, s1 == s1);

		// null Char*
		ASSERT_EQ(false, s1 == ((Char*)NULL));
		s1.clear();
		ASSERT_EQ(true, s1 == ((Char*)NULL));
	}
	// <Test> operator!=
	{
		// String
		UString ss = _TT("a");
		UString s1 = _TT("b");
		ASSERT_EQ(true, s1 != ss);

		// Char*
		ASSERT_EQ(true, s1 != _TT("c"));
		ASSERT_EQ(true, s1 != "c");

		// self
		ASSERT_EQ(false, s1 != s1);

		// null Char*
		ASSERT_EQ(true, s1 != ((Char*)NULL));
		s1.clear();
		ASSERT_EQ(false, s1 != ((Char*)NULL));
	}
	// <Test> operator+=
	{
		UString s1;
		UString s2 = _TT("a");

		// String
		s1 += s2;
		ASSERT_EQ(_TT("a"), s1);

		// StringRef
		s1 += UStringRef(_TT("b"));
		ASSERT_EQ(_TT("ab"), s1);

		// Char*
		s1 += _TT("c");
		ASSERT_EQ(_TT("abc"), s1);

		// Char
		s1 += _TT('d');
		ASSERT_EQ(_TT("abcd"), s1);
	}
	// <Test> operator +
	{
		// String + String
		UString s1;
		s1 = UString(_TT("a")) + UString(_TT("b"));
		ASSERT_EQ(_TT("ab"), s1);

		// String + Char*
		s1 = UString(_TT("a")) + _TT("b");
		ASSERT_EQ(_TT("ab"), s1);

		// Char* + String
		s1 = _TT("a") + UString(_TT("b"));
		ASSERT_EQ(_TT("ab"), s1);
	}
	// <Test> operator <
	{
		UString s1 = _TT("a"), s2 = _TT("A");

		// String
		ASSERT_EQ(true, s2 < s1);
		ASSERT_EQ(false, s2 < UString());

		// Char*
		ASSERT_EQ(true, s1 < _TT("b"));
		ASSERT_EQ(false, s1 < _TT(""));
		ASSERT_EQ(false, s1 < ((Char*)NULL));
	}
	// <Test> operator >
	{
		UString s1 = _TT("a"), s2 = _TT("A");

		// String
		ASSERT_EQ(false, s2 > s1);
		ASSERT_EQ(true, s2 > UString());

		// Char*
		ASSERT_EQ(false, s1 > _TT("b"));
		ASSERT_EQ(true, s1 > _TT(""));
		ASSERT_EQ(true, s1 > ((Char*)NULL));
	}
	// <Test> operator <=
	{
		UString s1 = _TT("a"), s2 = _TT("A"), s3 = _TT("a");

		// String
		ASSERT_EQ(true, s2 <= s1);
		ASSERT_EQ(true, s2 <= s3);

		// Char*
		ASSERT_EQ(false, s1 <= _TT("A"));
		ASSERT_EQ(true, s1 <= _TT("a"));
	}
	// <Test> operator >=
	{
		UString s1 = _TT("a"), s2 = _TT("A"), s3 = _TT("a");

		// String
		ASSERT_EQ(false, s2 >= s1);
		ASSERT_EQ(false, s2 >= s3);

		// Char*
		ASSERT_EQ(true, s1 >= _TT("A"));
		ASSERT_EQ(true, s1 >= _TT("a"));
	}
}

TEST_F(Test_Base_UString, assignFromCStr)
{
	// <Test> コンストラクタへ const char*, char 指定
	{
		UString s1("abc");
		UString s2(2, 'a');
		ASSERT_EQ(3, s1.getLength());
		ASSERT_EQ(_TT("abc"), s1);
		ASSERT_EQ(_TT("aa"), s2);
	}
	// <Test> 処理系関数へ const char*, char 指定
	{
		UString s1("abc");
		//ASSERT_EQ(1, s1.indexOf("b"));
		ASSERT_EQ(1, s1.indexOf('b'));
	}
}

TEST_F(Test_Base_UString, clear)
{
	// <Test> (SSO)
	{
		UString s1, s2(_TT("abc"));
		s1.clear();
		s2.clear();
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(_TT(""), s1);
		ASSERT_EQ(true, s2.isEmpty());
		ASSERT_EQ(_TT(""), s2);
	}
	// <Test> (NonSSO)
	{
		UString s1, s2(NSSO_STR);
		s1.clear();
		s2.clear();
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(_TT(""), s1);
		ASSERT_EQ(true, s2.isEmpty());
		ASSERT_EQ(_TT(""), s2);
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
	UString s1(_TT("abcdef"));
	ASSERT_EQ(1, s1.indexOf(_TT("bcd")));
	ASSERT_EQ(-1, s1.indexOf(_TT("a"), 1));
	ASSERT_EQ(0, s1.indexOf(_TT("A"), 0, CaseSensitivity::CaseInsensitive));
	ASSERT_EQ(4, s1.indexOf(_TT('e')));
}

TEST_F(Test_Base_UString, lastIndexOf)
{
	{
		UString str = _TT("abcdef");
		ASSERT_EQ(3, str.lastIndexOf(_TT("de")));
		ASSERT_EQ(1, str.lastIndexOf(_TT("bc"), 2));
		ASSERT_EQ(-1, str.lastIndexOf(_TT("cd"), 2));
		ASSERT_EQ(2, str.lastIndexOf(_TT("cd"), 4, 3));
		ASSERT_EQ(-1, str.lastIndexOf(_TT("bc"), 4, 3));
	}
	// .NET の System.String.LastIndexOf() と同様の動作
	{
		UString str = _TT("crazy azimuths");
		UString se = _TT("");
		ASSERT_EQ(6, str.lastIndexOf(_TT("az")));			// 6
		ASSERT_EQ(2, str.lastIndexOf(_TT("az"), 6));		// 2 ("crazy a"
		ASSERT_EQ(6, str.lastIndexOf(_TT("az"), 7));		// 6 ("crazy az"
		ASSERT_EQ(0, str.lastIndexOf(_TT("cr"), 6));		// 0
		ASSERT_EQ(-1, str.lastIndexOf(_TT("cr"), 6, 4));	// -1
		ASSERT_EQ(-1, str.lastIndexOf(_TT("cr"), 6, 6));	// -1
		ASSERT_EQ(0, str.lastIndexOf(_TT("cr"), 6, 7));		// 0
		ASSERT_EQ(5, str.lastIndexOf(_TT(""), 5, 2));		// 5
		ASSERT_EQ(3, str.lastIndexOf(_TT(""), 3, 0));
		ASSERT_EQ(0, se.lastIndexOf(_TT("")));
	}
}

TEST_F(Test_Base_UString, startsWith)
{
	UString s1(_TT("abc.txt"));
	ASSERT_EQ(true, s1.startsWith(_TT("abc")));
	ASSERT_EQ(false, s1.startsWith(_TT(".txt")));
}

TEST_F(Test_Base_UString, endsWith)
{
	UString s1(_TT("abc.txt"));
	ASSERT_EQ(false, s1.endsWith(_TT("abc")));
	ASSERT_EQ(true, s1.endsWith(_TT(".txt")));
}

TEST_F(Test_Base_UString, trim)
{
	// 前後
	{
		UString str1(_TT(" abc def "));
		UString t = str1.trim();
		ASSERT_EQ(_TT("abc def"), t);
	}
	// 前だけ
	{
		UString str1(_TT(" abc def"));
		UString t = str1.trim();
		ASSERT_EQ(_TT("abc def"), t);
	}
	// 後ろだけ
	{
		UString str1(_TT("abc def "));
		UString t = str1.trim();
		ASSERT_EQ(_TT("abc def"), t);
	}
	// 空文字
	{
		UString str1(_TT(""));
		UString t = str1.trim();
		ASSERT_EQ(_TT(""), t);
	}
	// 空白だけ
	{
		UString str1(_TT(" "));
		UString t = str1.trim();
		ASSERT_EQ(_TT(""), t);
	}
	// 空白だけ * 2
	{
		UString str1(_TT("  "));
		UString t = str1.trim();
		ASSERT_EQ(_TT(""), t);
	}
	// 空白だけ * 3
	{
		UString str1(_TT("   "));
		UString t = str1.trim();
		ASSERT_EQ(_TT(""), t);
	}
}

TEST_F(Test_Base_UString, remove)
{
	UString str1(_TT("abcdef"));

	UString str2 = str1.remove(_TT('c'));
	ASSERT_EQ(_TT("abdef"), str2);

	// 大文字小文字を区別しない
	UString str3 = str2.remove(_TT("D"), CaseSensitivity::CaseInsensitive);
	ASSERT_EQ(_TT("abef"), str3);
}


TEST_F(Test_Base_UString, replace)
{
	// <Test> 部分一致を置換できること。
	{
		UString str1(_TT("test"));
		UString str2 = str1.replace(_TT("es"), _TT("b"));
		ASSERT_EQ(_TT("tbt"), str2);
		UString str3 = str2.replace(_TT("t"), _TT(""));
		ASSERT_EQ(_TT("b"), str3);
		UString str4 = str3.replace(_TT("b"), _TT(""));
		ASSERT_EQ(_TT(""), str4);
	}
	// <Test> 全ての一致を置換できること。
	// <Test> 破壊的変更にならないこと。
	{
		UString str1(_TT("aaa"));
		UString str2 = str1.replace(_TT("a"), _TT("b"));
		ASSERT_EQ(_TT("bbb"), str2);
		ASSERT_EQ(_TT("aaa"), str1);
	}
	// <Test> UString を渡せること。
	{
		UString str1(_TT("a"));
		UString str2 = str1.replace(UString(_TT("a")), UString(_TT("b")));
		ASSERT_EQ(_TT("b"), str2);
	}
	// <Test> UStringRef を渡せること。
	{
		UString str1(_TT("reabcdef"));
		UChar buf1[] = _TT("abc");
		UChar buf2[] = _TT("def");
		UString str2 = str1.replace(UStringRef(buf1, buf1 + 2), UStringRef(buf2, buf2 + 2));
		ASSERT_EQ(_TT("redecdef"), str2);
	}
	// <Test> 文字列を置換する
	{
		UString str1 = _TT("abcdcd");

		// "ab" を "12" に置き換える
		str1 = str1.replace(_TT("ab"), _TT("12"));

		// "cd" を "345" に置き換える
		UString from = _TT("cd");
		UString to = _TT("345");
		str1 = str1.replace(from, to);

		ASSERT_EQ(_TT("12345345"), str1);
	}
}

TEST_F(Test_Base_UString, compare)
{
	int a1 = strcmp("abc", "abcd");
	int a2 = strcmp("bbb", "aaaa");
	int a3 = strncmp("abc", "abcd", 4);
	int a4 = strncmp("abcd", "abc", 4);

	// <Issue> 文字列の先頭が同じだけで一致判定にならないこと。
	{
		UString str1(_TT("abc"));
		UString str2(_TT("abcd"));
		ASSERT_EQ(true, UString::compare(str1, str2) < 0);
		ASSERT_EQ(true, UString::compare(str2, str1) > 0);
	}
	{
		UString str1(_TT("abc"));
		EXPECT_EQ(0, UString::compare(str1, _TT("abc")));
		EXPECT_LE(0, UString::compare(str1, _TT("ab")));	// v1 < v2
		EXPECT_GT(0, UString::compare(str1, _TT("abd")));	// v1 > v2
	}

	{
		UString str1(_TT("abc"));
		UString str2(_TT("abcd"));
		ASSERT_TRUE(str1 < str2);
	}

	// <Test> UStringRef との比較
	{
		ASSERT_EQ(true, UString(_TT("abc")) == UStringRef(_TT("abc")));
		ASSERT_EQ(false, UString(_TT("abcd")) == UStringRef(_TT("abc")));
		ASSERT_EQ(false, UString(_TT("abc")) == UStringRef(_TT("abcd")));
		ASSERT_EQ(false, UString(_TT("abc")) == UStringRef(_TT("a")));
		ASSERT_EQ(false, UString(_TT("abc")) == UStringRef(_TT("ab")));
		ASSERT_EQ(false, UString(_TT("a")) == UStringRef(_TT("abc")));
		ASSERT_EQ(false, UString(_TT("ab")) == UStringRef(_TT("abc")));
		ASSERT_EQ(false, UString(_TT("abc")) == UStringRef(_TT("")));
		ASSERT_EQ(false, UString(_TT("")) == UStringRef(_TT("abc")));

		ASSERT_EQ(true, UString(_TT("abc")) == UStringRef(_TT("abcd"), 3));
		ASSERT_EQ(false, UString(_TT("abcd")) == UStringRef(_TT("abcd"), 3));
		ASSERT_EQ(false, UString(_TT("abc")) == UStringRef(_TT("abcd"), 4));
		ASSERT_EQ(false, UString(_TT("abc")) == UStringRef(_TT("abcd"), 1));
		ASSERT_EQ(false, UString(_TT("abc")) == UStringRef(_TT("abcd"), 2));
		ASSERT_EQ(false, UString(_TT("a")) == UStringRef(_TT("abcd"), 3));
		ASSERT_EQ(false, UString(_TT("ab")) == UStringRef(_TT("abcd"), 3));
		ASSERT_EQ(false, UString(_TT("abc")) == UStringRef(_TT("abcd"), 0));
		ASSERT_EQ(false, UString(_TT("")) == UStringRef(_TT("abcd"), 3));
	}

	// <Test> 比較
	{
		ASSERT_LE(0, StringTraits::compare(_TT("abcd"), 4, _TT("abc"), 3, 4));	// 0 < result (1)
		ASSERT_GT(0, StringTraits::compare(_TT("abc"), 3, _TT("abcd"), 4, 4));	// result < 0 (-1)
	}
}

TEST_F(Test_Base_UString, substring)
{
	UString str1(_TT("abcdef"));
	ASSERT_EQ(_TT("ab"), str1.substring(0, 2));
	ASSERT_EQ(_TT("ef"), str1.substring(str1.getLength() - 2));
	ASSERT_EQ(_TT("cde"), str1.substring(2, 3));
}

TEST_F(Test_Base_UString, split)
{
	// 普通の分割
	{
		UString str(_TT("a,b,c"));
		List<UString> lines = str.split(_TT(","), StringSplitOptions::None);
		ASSERT_EQ(3, lines.getCount());
		ASSERT_EQ(_TT("a"), lines[0]);
		ASSERT_EQ(_TT("b"), lines[1]);
		ASSERT_EQ(_TT("c"), lines[2]);
	}

	// 空文字
	{
		UString str = _TT("");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::None);
		ASSERT_EQ(1, lines.getCount());
	}

	// 空文字の RemoveEmptyEntries
	{
		UString str = _TT("");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
		ASSERT_EQ(0, lines.getCount());
	}

	// 区切り無し
	{
		UString str = _TT("abc");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::None);
		ASSERT_EQ(1, lines.getCount());
		ASSERT_EQ(_TT("abc"), lines[0]);
	}

	// 区切り無し RemoveEmptyEntries
	{
		UString str = _TT("abc");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
		ASSERT_EQ(1, lines.getCount());
	}

	// トークン無し
	{
		UString str = _TT(",");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::None);
		ASSERT_EQ(2, lines.getCount());
		ASSERT_EQ(_TT(""), lines[0]);
		ASSERT_EQ(_TT(""), lines[1]);
	}

	// トークン無し RemoveEmptyEntries
	{
		UString str = _TT(",");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
		ASSERT_EQ(0, lines.getCount());
	}

	// トークン無し 2
	{
		UString str = _TT(",,");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::None);
		ASSERT_EQ(3, lines.getCount());
	}

	// トークン無し 2 RemoveEmptyEntries
	{
		UString str = _TT(",,");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
		ASSERT_EQ(0, lines.getCount());
	}

	// 空要素付き
	{
		UString str = _TT("a,,c");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::None);
		ASSERT_EQ(3, lines.getCount());
		ASSERT_EQ(_TT("a"), lines[0]);
		ASSERT_EQ(_TT(""), lines[1]);
		ASSERT_EQ(_TT("c"), lines[2]);
	}

	// 空要素付き RemoveEmptyEntries
	{
		UString str = _TT("a,,c");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
		ASSERT_EQ(2, lines.getCount());
		ASSERT_EQ(_TT("a"), lines[0]);
		ASSERT_EQ(_TT("c"), lines[1]);
	}

	// 先頭空要素付き
	{
		UString str = _TT(",b,c");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::None);
		ASSERT_EQ(3, lines.getCount());
		ASSERT_EQ(_TT(""), lines[0]);
		ASSERT_EQ(_TT("b"), lines[1]);
		ASSERT_EQ(_TT("c"), lines[2]);
	}

	// 先頭空要素付き RemoveEmptyEntries
	{
		UString str = _TT(",b,c");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
		ASSERT_EQ(2, lines.getCount());
		ASSERT_EQ(_TT("b"), lines[0]);
		ASSERT_EQ(_TT("c"), lines[1]);
	}

	// 終端空要素付き
	{
		UString str = _TT("a,b,");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::None);
		ASSERT_EQ(3, lines.getCount());
		ASSERT_EQ(_TT("a"), lines[0]);
		ASSERT_EQ(_TT("b"), lines[1]);
		ASSERT_EQ(_TT(""), lines[2]);
	}

	// 終端空要素付き RemoveEmptyEntries
	{
		UString str = _TT("a,b,");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
		ASSERT_EQ(2, lines.getCount());
		ASSERT_EQ(_TT("a"), lines[0]);
		ASSERT_EQ(_TT("b"), lines[1]);
	}

	// 両端空要素付き
	{
		UString str = _TT(",b,");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::None);
		ASSERT_EQ(3, lines.getCount());
		ASSERT_EQ(_TT(""), lines[0]);
		ASSERT_EQ(_TT("b"), lines[1]);
		ASSERT_EQ(_TT(""), lines[2]);
	}

	// 両端空要素付き RemoveEmptyEntries
	{
		UString str = _TT(",b,");
		List<UString> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
		ASSERT_EQ(1, lines.getCount());
		ASSERT_EQ(_TT("b"), lines[0]);
	}
	// 長さ2以上のデリミタ
	{
		UString str = _TT("a::b");
		List<UString> lines = str.split(_TT("::"), StringSplitOptions::None);
		ASSERT_EQ(2, lines.getCount());
		ASSERT_EQ(_TT("a"), lines[0]);
		ASSERT_EQ(_TT("b"), lines[1]);
	}
}

TEST_F(Test_Base_UString, ToInt)
{
	// TODO:
}

TEST_F(Test_Base_UString, convertNativeCharString)
{
	UString str = _TT("abc");
	ASSERT_EQ("abc", str.toStdString());
	ASSERT_EQ(L"abc", str.toStdWString());

	// TODO:
}

TEST_F(Test_Base_UString, unordered_map)
{
	// <Test> unordered_map のキーにできること (char)
	{
		std::unordered_map<UString, int> map1;
		map1[_TT("key1")] = 1;
		map1.insert(std::pair<UString, int>(_TT("key2"), 2));
		ASSERT_EQ(1, map1[_TT("key1")]);
		ASSERT_EQ(2, map1.find(_TT("key2"))->second);

		std::unordered_map<UString, int> map2 =
		{
			{ _TT("key1"), 1 },
			{ _TT("key2"), 2 },
		};
		ASSERT_EQ(1, map2[_TT("key1")]);
		ASSERT_EQ(2, map2.find(_TT("key2"))->second);
	}
	// <Test> std::hash の String の特殊化テスト
	{
		std::hash<ln::UString> hash1;
		ASSERT_NE(hash1(UString(_TT("key1"))), hash1(UString(_TT("key2"))));
	}
	// <Test> StringRef で検索
	{
		UString key1 = _TT("key1");
		UStringRef key1ref(key1);
		UString key2 = _TT("key2");
		UStringRef key2ref(key2);

		std::unordered_map<UString, int> map1;
		map1[key1ref] = 1;
		map1.insert(std::pair<UString, int>(key2ref, 2));
		ASSERT_EQ(1, map1[key1ref]);
		ASSERT_EQ(2, map1.find(key2ref)->second);
	}
}

TEST_F(Test_Base_UString, conv_case)
{
	UString s1 = _TT("AbCd");
	ASSERT_EQ(_TT("ABCD"), s1.toUpper());
	UString s2 = _TT("AbCd");
	ASSERT_EQ(_TT("abcd"), s2.toLower());
	UString s3 = _TT("aBcD");
	ASSERT_EQ(_TT("Abcd"), s3.toTitleCase());
}






TEST_F(Test_Base_UString, concat)
{
	UString s1 = UString::concat(_TT("a"), _TT("b"));
	ASSERT_EQ(_TT("ab"), s1);

	s1 = UString::concat(_TT("12345678901234567890"), _TT("12345678901234567890"));
	ASSERT_EQ(_TT("1234567890123456789012345678901234567890"), s1);
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
	// <Test> PathName
	{
		PathName s = _TT("abc");
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
TEST_F(Test_Base_FormatterU, Illigal)
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
TEST_F(Test_Base_FormatterU, Examples)
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
		ASSERT_EQ(_TT("1,234568E+004"), UString::format(Locale(_TT("fr"), _TT("{0:E}"), 12345.6789));
#endif
	}

	{
		ASSERT_EQ(_TT("2045e"), UString::format(_TT("{0:x}"), 0x2045e));
		ASSERT_EQ(_TT("2045E"), UString::format(_TT("{0:X}"), 0x2045e));
		ASSERT_EQ(_TT("0002045E"), UString::format(_TT("{0:X8}"), 0x2045e));
		ASSERT_EQ(_TT("0xFF"), UString::format(_TT("0x{0:X}"), 255));
	}
}



class Test_IO_Path : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST_F(Test_IO_Path, Constructor)
{
	// <Test> String から変換できること
	{
		Path path = UString("dir");
	}
	// <Test> 
	{
		Path base = _TT("dir1/dir2");
		Path path(base, _TT("../file1.txt"));
#ifdef LN_OS_WIN32
		ASSERT_EQ(_TT("dir1/dir2\\../file1.txt"), path.getString());
#else
		ASSERT_STREQ(_TT("dir1/dir2/../file1.txt"), path.getString());
#endif
	}
	// <Test> 
	{
		Path path(_TT("a/"), _TT("b"));
		ASSERT_EQ(_TT("a/b"), path.getString());

	}
	// <Test> 空パスとの結合を確認する
	{
		Path path(Path(), _TT("a/b.txt"));
		ASSERT_EQ(_TT("a/b.txt"), path.getString());
	}
}

TEST_F(Test_IO_Path, getFileName)
{
	Path path1(_TT("dir/file.txt"));
	ASSERT_EQ(_TT("file.txt"), path1.getFileName());

	Path path2(_TT("file.txt"));
	ASSERT_EQ(_TT("file.txt"), path2.getFileName());
}

TEST_F(Test_IO_Path, getWithoutExtension)
{
	Path path;

	path = _TT("C:/dir/file.txt");
	ASSERT_EQ(_TT("C:/dir/file"), path.getWithoutExtension().getString());

	path = _TT("file.txt");
	ASSERT_EQ(_TT("file"), path.getWithoutExtension().getString());

	path = _TT("file");
	ASSERT_EQ(_TT("file"), path.getWithoutExtension().getString());

	path = _TT("");
	ASSERT_EQ(_TT(""), path.getWithoutExtension().getString());

	path = _TT("C:/dir.sub/file");
	ASSERT_EQ(_TT("C:/dir.sub/file"), path.getWithoutExtension().getString());

	path = _TT("dir/.git");
	ASSERT_EQ(_TT("dir/"), path.getWithoutExtension().getString());

	path = _TT(".git");
	ASSERT_EQ(_TT(""), path.getWithoutExtension().getString());
}

TEST_F(Test_IO_Path, getExtension)
{
	ASSERT_EQ(_TT(".txt"), Path(_TT("file.txt")).getExtension());
	ASSERT_EQ(_TT("txt"), Path(_TT("file.txt")).getExtension(false));
	ASSERT_EQ(_TT(".txt"), Path(_TT("file.tmp.txt")).getExtension());
	ASSERT_EQ(_TT(""), Path(_TT("file")).getExtension());
	ASSERT_EQ(_TT(""), Path(_TT("")).getExtension());
	ASSERT_EQ(_TT(""), Path(_TT(".")).getExtension());

	ASSERT_EQ(_TT(""), Path(_TT("dir.a/file")).getExtension());
	ASSERT_EQ(_TT(""), Path(_TT("file.")).getExtension());
	ASSERT_EQ(_TT(""), Path(_TT("..")).getExtension());
	ASSERT_EQ(_TT(""), Path(_TT("a/")).getExtension());
	ASSERT_EQ(_TT(""), Path(_TT("/")).getExtension());
}
