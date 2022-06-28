#include "Common.hpp"
#include <unordered_map>
#include <LuminoCore/Base/String.hpp>
#include <LuminoCore/Text/Encoding.hpp>

class Test_Base_String : public ::testing::Test {
protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
};

#define NSSO_STR _TT("abcdefghij1234567890")

TEST_F(Test_Base_String, Constructor) {
    // <Test> SSO
    {
        { String s1(_TT("abc"));
    String s2(s1, 1);
    String s3(s1, 1, 1);
    ASSERT_EQ(_TT("bc"), s2);
    ASSERT_EQ(_TT("b"), s3);
    String s4(String(_TT("")));
    ASSERT_EQ(_TT(""), s4);
}
{
    String s1;
    String s2(_TT("abc"));
    String s3(_TT("abc"), 2);
    String s5(_TT(""));
    ASSERT_EQ(true, s1.isEmpty());
    ASSERT_EQ(_TT("abc"), s2);
    ASSERT_EQ(_TT("ab"), s3);
    ASSERT_EQ(_TT(""), s5);
}
{
    String s1(3, _TT('a'));
    String s2(0, _TT('a'));
    ASSERT_EQ(_TT("aaa"), s1);
    ASSERT_EQ(_TT(""), s2);
}
}
// <Test> NonSSO
{
    {
        String s1(NSSO_STR);
        String s2(s1, 1);
        String s3(s1, 1, 1);
        ASSERT_EQ(_TT("bcdefghij1234567890"), s2);
        ASSERT_EQ(_TT("b"), s3);
    }
    {
        String s1;
        String s2(NSSO_STR);
        String s3(NSSO_STR, 2);
        String s5(_TT(""));
        ASSERT_EQ(true, s1.isEmpty());
        ASSERT_EQ(NSSO_STR, s2);
        ASSERT_EQ(_TT("ab"), s3);
        ASSERT_EQ(_TT(""), s5);
    }
    {
        String s1(20, _TT('a'));
        ASSERT_EQ(_TT("aaaaaaaaaaaaaaaaaaaa"), s1);
    }
}
}

TEST_F(Test_Base_String, CopyFramework) {
    // <Test> copy (SSO)
    {
        String s1;
        String s2(s1);
        String s3(_TT("abc"));
        String s4(s3);
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
        String s1;
        String s2(s1);
        String s3(NSSO_STR);
        String s4(s3);
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
        String s1;
        String s2(std::move(s1));
        String s3(_TT("abc"));
        String s4(std::move(s3));
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
        String s1;
        String s2(std::move(s1));
        String s3(NSSO_STR);
        String s4(std::move(s3));
        ASSERT_EQ(true, s1.isEmpty());
        ASSERT_EQ(true, s2.isEmpty());
        ASSERT_EQ(true, s3.isEmpty());
        ASSERT_EQ(NSSO_STR, s4);

        s1 = std::move(s4);
        ASSERT_EQ(NSSO_STR, s1);
        ASSERT_EQ(true, s4.isEmpty());
    }
}

TEST_F(Test_Base_String, resize) {
    // <Test>
    {
        String s1 = _TT("abd");
        s1.resize(2);
        ASSERT_EQ(_TT("ab"), s1);

        s1.resize(5, _TT('c'));
        ASSERT_EQ(_TT("abccc"), s1);
    }
    // <Test> 内部状態切り替えのテスト
    {
        String s1, s2;

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
        String s3;
        s3 += NSSO_STR;
        ASSERT_EQ(NSSO_STR, s3);
    }
}

TEST_F(Test_Base_String, Operators) {
    // <Test> =
    {
        String s1, s2, s3(_TT("abc"));
        s1 = s2;
        s2 = s3;
        s3 = _TT('a');
        ASSERT_EQ(true, s1.isEmpty());
        ASSERT_EQ(_TT("abc"), s2);
        ASSERT_EQ(_TT("a"), s3);
        ASSERT_EQ(1, s3.length());
    }
    // <Test> operator=
    {
        String str;

        // Char
        str = '0';
        ASSERT_EQ(_TT("0"), str);
        str = _TT('1');
        ASSERT_EQ(_TT("1"), str);

        // Char*
        str = _TT("a");
        ASSERT_EQ(_TT("a"), str);
        str = NSSO_STR;
        ASSERT_EQ(NSSO_STR, str);

        // StringView
        str = StringView(_TT("b"));
        ASSERT_EQ(_TT("b"), str);
        str = StringView(NSSO_STR);
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
        String ss = _TT("a");
        String s1 = _TT("a");
        ASSERT_EQ(true, s1 == ss);

        // Char*
        ASSERT_EQ(true, s1 == _TT("a"));
#ifdef LN_STRING_FUZZY_CONVERSION
        ASSERT_EQ(true, s1 == "a");
#endif

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
        String ss = _TT("a");
        String s1 = _TT("b");
        ASSERT_EQ(true, s1 != ss);

        // Char*
        ASSERT_EQ(true, s1 != _TT("c"));
#ifdef LN_STRING_FUZZY_CONVERSION
        ASSERT_EQ(true, s1 != "c");
#endif

        // self
        ASSERT_EQ(false, s1 != s1);

        // null Char*
        ASSERT_EQ(true, s1 != ((Char*)NULL));
        s1.clear();
        ASSERT_EQ(false, s1 != ((Char*)NULL));
    }
    // <Test> operator+=
    {
        String s1;
        String s2 = _TT("a");

        // String
        s1 += s2;
        ASSERT_EQ(_TT("a"), s1);

        // StringView
        s1 += StringView(_TT("b"));
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
        String s1;
        s1 = String(_TT("a")) + String(_TT("b"));
        ASSERT_EQ(_TT("ab"), s1);

        // String + Char*
        s1 = String(_TT("a")) + _TT("b");
        ASSERT_EQ(_TT("ab"), s1);

        // Char* + String
        s1 = _TT("a") + String(_TT("b"));
        ASSERT_EQ(_TT("ab"), s1);
    }
    // <Test> operator <
    {
        String s1 = _TT("a"), s2 = _TT("A");

        // String
        ASSERT_EQ(true, s2 < s1);
        ASSERT_EQ(false, s2 < String());

        // Char*
        ASSERT_EQ(true, s1 < _TT("b"));
        ASSERT_EQ(false, s1 < _TT(""));
        ASSERT_EQ(false, s1 < ((Char*)NULL));
    }
    // <Test> operator >
    {
        String s1 = _TT("a"), s2 = _TT("A");

        // String
        ASSERT_EQ(false, s2 > s1);
        ASSERT_EQ(true, s2 > String());

        // Char*
        ASSERT_EQ(false, s1 > _TT("b"));
        ASSERT_EQ(true, s1 > _TT(""));
        ASSERT_EQ(true, s1 > ((Char*)NULL));
    }
    // <Test> operator <=
    {
        String s1 = _TT("a"), s2 = _TT("A"), s3 = _TT("a");

        // String
        ASSERT_EQ(true, s2 <= s1);
        ASSERT_EQ(true, s2 <= s3);

        // Char*
        ASSERT_EQ(false, s1 <= _TT("A"));
        ASSERT_EQ(true, s1 <= _TT("a"));
    }
    // <Test> operator >=
    {
        String s1 = _TT("a"), s2 = _TT("A"), s3 = _TT("a");

        // String
        ASSERT_EQ(false, s2 >= s1);
        ASSERT_EQ(false, s2 >= s3);

        // Char*
        ASSERT_EQ(true, s1 >= _TT("A"));
        ASSERT_EQ(true, s1 >= _TT("a"));
    }
}

#ifdef LN_STRING_FUZZY_CONVERSION
TEST_F(Test_Base_String, assignFromCStr) {
    // <Test> コンストラクタへ const char*, char 指定
    {
        String s1("abc");
        String s2(2, 'a');
        ASSERT_EQ(3, s1.length());
        ASSERT_EQ(_TT("abc"), s1);
        ASSERT_EQ(_TT("aa"), s2);
    }
    // <Test> 処理系関数へ const char*, char 指定
    {
        String s1("abc");
        //ASSERT_EQ(1, s1.indexOf("b"));
        ASSERT_EQ(1, s1.indexOf('b'));
    }
}
#endif

TEST_F(Test_Base_String, clear) {
    // <Test> (SSO)
    {
        String s1, s2(_TT("abc"));
        s1.clear();
        s2.clear();
        ASSERT_EQ(true, s1.isEmpty());
        ASSERT_EQ(_TT(""), s1);
        ASSERT_EQ(true, s2.isEmpty());
        ASSERT_EQ(_TT(""), s2);
    }
    // <Test> (NonSSO)
    {
        String s1, s2(NSSO_STR);
        s1.clear();
        s2.clear();
        ASSERT_EQ(true, s1.isEmpty());
        ASSERT_EQ(_TT(""), s1);
        ASSERT_EQ(true, s2.isEmpty());
        ASSERT_EQ(_TT(""), s2);
    }
    // <Test> SSO <=> NonSSO
    {
        String s1 = NSSO_STR;
        String s2 = s1;
        s1.clear();
        ASSERT_EQ(NSSO_STR, s2);
    }
}

//#ifdef LN_USTRING16
//#else
////------------------------------------------------------------------------------
//TEST_F(Test_Base_String, sprintf)
//{
//	// <Test> max 文字数チェック
//	{
//		String str1 = String::sprintf(_LT("%d"), 10);
//		ASSERT_EQ(_LT("10"), str1);
//	}
//	// <Test> max 文字数チェック
//	{
//		Char buf1[2048 + 1] = { 0 };
//		for (int i = 0; i < 2048; i++) {
//			buf1[i] = 'a';
//		}
//		buf1[2048] = '\0';
//
//		String str1 = String::sprintf(_LT("%s"), buf1);
//		ASSERT_EQ(str1, buf1);	// 同じ文字ができていればOK
//	}
//}
//#endif

TEST_F(Test_Base_String, indexOf) {
    String s1(_TT("abcdef"));
    ASSERT_EQ(1, s1.indexOf(_TT("bcd")));
    ASSERT_EQ(-1, s1.indexOf(_TT("a"), 1));
    ASSERT_EQ(0, s1.indexOf(_TT("A"), 0, CaseSensitivity::CaseInsensitive));
    ASSERT_EQ(4, s1.indexOf(_TT('e')));
}

TEST_F(Test_Base_String, lastIndexOf) {
    {
        String str = _TT("abcdef");
        ASSERT_EQ(3, str.lastIndexOf(_TT("de")));
        ASSERT_EQ(1, str.lastIndexOf(_TT("bc"), 2));
        ASSERT_EQ(-1, str.lastIndexOf(_TT("cd"), 2));
        ASSERT_EQ(2, str.lastIndexOf(_TT("cd"), 4, 3));
        ASSERT_EQ(-1, str.lastIndexOf(_TT("bc"), 4, 3));
    }
    // .NET の System.String.LastIndexOf() と同様の動作
    {
        String str = _TT("crazy azimuths");
        String se = _TT("");
        ASSERT_EQ(6, str.lastIndexOf(_TT("az")));        // 6
        ASSERT_EQ(2, str.lastIndexOf(_TT("az"), 6));     // 2 ("crazy a"
        ASSERT_EQ(6, str.lastIndexOf(_TT("az"), 7));     // 6 ("crazy az"
        ASSERT_EQ(0, str.lastIndexOf(_TT("cr"), 6));     // 0
        ASSERT_EQ(-1, str.lastIndexOf(_TT("cr"), 6, 4)); // -1
        ASSERT_EQ(-1, str.lastIndexOf(_TT("cr"), 6, 6)); // -1
        ASSERT_EQ(0, str.lastIndexOf(_TT("cr"), 6, 7));  // 0
        ASSERT_EQ(5, str.lastIndexOf(_TT(""), 5, 2));    // 5
        ASSERT_EQ(3, str.lastIndexOf(_TT(""), 3, 0));
        ASSERT_EQ(0, se.lastIndexOf(_TT("")));
    }
}

TEST_F(Test_Base_String, startsWith) {
    String s1(_TT("abc.txt"));
    ASSERT_EQ(true, s1.startsWith(_TT("abc")));
    ASSERT_EQ(false, s1.startsWith(_TT(".txt")));
}

TEST_F(Test_Base_String, endsWith) {
    String s1(_TT("abc.txt"));
    ASSERT_EQ(false, s1.endsWith(_TT("abc")));
    ASSERT_EQ(true, s1.endsWith(_TT(".txt")));
}

TEST_F(Test_Base_String, substring) {
    // <Test> substring
    {
        String str1(_TT("abcdef"));
        ASSERT_EQ(_TT("ab"), str1.substr(0, 2));
        ASSERT_EQ(_TT("ef"), str1.substr(str1.length() - 2));
        ASSERT_EQ(_TT("cde"), str1.substr(2, 3));
    }
    // <Test> left
    {
        ASSERT_EQ(_LT("ab"), String(_LT("abcdef")).left(2));
    }
    // <Test> right
    {
        ASSERT_EQ(_LT("ef"), String(_LT("abcdef")).right(2));
    }
}

TEST_F(Test_Base_String, trim) {
    // 前後
    {
        String str1(_TT(" abc def "));
        String t = str1.trim();
        ASSERT_EQ(_TT("abc def"), t);
    }
    // 前だけ
    {
        String str1(_TT(" abc def"));
        String t = str1.trim();
        ASSERT_EQ(_TT("abc def"), t);
    }
    // 後ろだけ
    {
        String str1(_TT("abc def "));
        String t = str1.trim();
        ASSERT_EQ(_TT("abc def"), t);
    }
    // 空文字
    {
        String str1(_TT(""));
        String t = str1.trim();
        ASSERT_EQ(_TT(""), t);
    }
    // 空白だけ
    {
        String str1(_TT(" "));
        String t = str1.trim();
        ASSERT_EQ(_TT(""), t);
    }
    // 空白だけ * 2
    {
        String str1(_TT("  "));
        String t = str1.trim();
        ASSERT_EQ(_TT(""), t);
    }
    // 空白だけ * 3
    {
        String str1(_TT("   "));
        String t = str1.trim();
        ASSERT_EQ(_TT(""), t);
    }
}

TEST_F(Test_Base_String, remove) {
    String str1(_TT("abcdef"));

    String str2 = str1.remove(_TT('c'));
    ASSERT_EQ(_TT("abdef"), str2);

    // 大文字小文字を区別しない
    String str3 = str2.remove(_TT("D"), CaseSensitivity::CaseInsensitive);
    ASSERT_EQ(_TT("abef"), str3);
}

TEST_F(Test_Base_String, replace) {
    // <Test> 部分一致を置換できること。
    {
        String str1(_TT("test"));
        String str2 = str1.replace(_TT("es"), _TT("b"));
        ASSERT_EQ(_TT("tbt"), str2);
        String str3 = str2.replace(_TT("t"), _TT(""));
        ASSERT_EQ(_TT("b"), str3);
        String str4 = str3.replace(_TT("b"), _TT(""));
        ASSERT_EQ(_TT(""), str4);
    }
    // <Test> 全ての一致を置換できること。
    // <Test> 破壊的変更にならないこと。
    {
        String str1(_TT("aaa"));
        String str2 = str1.replace(_TT("a"), _TT("b"));
        ASSERT_EQ(_TT("bbb"), str2);
        ASSERT_EQ(_TT("aaa"), str1);
    }
    // <Test> String を渡せること。
    {
        String str1(_TT("a"));
        String str2 = str1.replace(String(_TT("a")), String(_TT("b")));
        ASSERT_EQ(_TT("b"), str2);
    }
    // <Test> StringView を渡せること。
    {
        String str1(_TT("reabcdef"));
        Char buf1[] = _TT("abc");
        Char buf2[] = _TT("def");
        String str2 = str1.replace(StringView(buf1, buf1 + 2), StringView(buf2, buf2 + 2));
        ASSERT_EQ(_TT("redecdef"), str2);
    }
    // <Test> 文字列を置換する
    {
        String str1 = _TT("abcdcd");

        // "ab" を "12" に置き換える
        str1 = str1.replace(_TT("ab"), _TT("12"));

        // "cd" を "345" に置き換える
        String from = _TT("cd");
        String to = _TT("345");
        str1 = str1.replace(from, to);

        ASSERT_EQ(_TT("12345345"), str1);
    }
    // <Test> NonSSO の置換
    {
        String s1 = _TT("abcdefghij1234567890");
        String s2 = s1.replace(_TT("cd"), _TT("AAAA"));
        ASSERT_EQ(_TT("abAAAAefghij1234567890"), s2);
    }
}

TEST_F(Test_Base_String, insert) {
    ASSERT_EQ(_TT("123abc"), String(_TT("abc")).insert(0, _TT("123")));
    ASSERT_EQ(_TT("a123bc"), String(_TT("abc")).insert(1, _TT("123")));
    ASSERT_EQ(_TT("ab123c"), String(_TT("abc")).insert(2, _TT("123")));
    ASSERT_EQ(_TT("abc123"), String(_TT("abc")).insert(3, _TT("123")));
    ASSERT_EQ(_TT("123"), String(_TT("")).insert(0, _TT("123")));
}

TEST_F(Test_Base_String, compare) {
    int a1 = strcmp("abc", "abcd");
    int a2 = strcmp("bbb", "aaaa");
    int a3 = strncmp("abc", "abcd", 4);
    int a4 = strncmp("abcd", "abc", 4);

    // <Issue> 文字列の先頭が同じだけで一致判定にならないこと。
    {
        String str1(_TT("abc"));
        String str2(_TT("abcd"));
        ASSERT_EQ(true, String::compare(str1, str2) < 0);
        ASSERT_EQ(true, String::compare(str2, str1) > 0);
    }
    {
        String str1(_TT("abc"));
        EXPECT_EQ(0, String::compare(str1, _TT("abc")));
        EXPECT_LE(0, String::compare(str1, _TT("ab")));  // v1 < v2
        EXPECT_GT(0, String::compare(str1, _TT("abd"))); // v1 > v2
    }

    {
        String str1(_TT("abc"));
        String str2(_TT("abcd"));
        ASSERT_TRUE(str1 < str2);
    }

    // <Test> StringView との比較
    {
        ASSERT_EQ(true, String(_TT("abc")) == StringView(_TT("abc")));
        ASSERT_EQ(false, String(_TT("abcd")) == StringView(_TT("abc")));
        ASSERT_EQ(false, String(_TT("abc")) == StringView(_TT("abcd")));
        ASSERT_EQ(false, String(_TT("abc")) == StringView(_TT("a")));
        ASSERT_EQ(false, String(_TT("abc")) == StringView(_TT("ab")));
        ASSERT_EQ(false, String(_TT("a")) == StringView(_TT("abc")));
        ASSERT_EQ(false, String(_TT("ab")) == StringView(_TT("abc")));
        ASSERT_EQ(false, String(_TT("abc")) == StringView(_TT("")));
        ASSERT_EQ(false, String(_TT("")) == StringView(_TT("abc")));

        ASSERT_EQ(true, String(_TT("abc")) == StringView(_TT("abcd"), 3));
        ASSERT_EQ(false, String(_TT("abcd")) == StringView(_TT("abcd"), 3));
        ASSERT_EQ(false, String(_TT("abc")) == StringView(_TT("abcd"), 4));
        ASSERT_EQ(false, String(_TT("abc")) == StringView(_TT("abcd"), 1));
        ASSERT_EQ(false, String(_TT("abc")) == StringView(_TT("abcd"), 2));
        ASSERT_EQ(false, String(_TT("a")) == StringView(_TT("abcd"), 3));
        ASSERT_EQ(false, String(_TT("ab")) == StringView(_TT("abcd"), 3));
        ASSERT_EQ(false, String(_TT("abc")) == StringView(_TT("abcd"), 0));
        ASSERT_EQ(false, String(_TT("")) == StringView(_TT("abcd"), 3));
    }

    // <Test> 比較
    {
        ASSERT_LE(0, StringHelper::compare(_TT("abcd"), 4, _TT("abc"), 3, 4)); // 0 < result (1)
        ASSERT_GT(0, StringHelper::compare(_TT("abc"), 3, _TT("abcd"), 4, 4)); // result < 0 (-1)
    }
}

TEST_F(Test_Base_String, split) {
    // 普通の分割
    {
        String str(_TT("a,b,c"));
        List<String> lines = str.split(_TT(","), StringSplitOptions::None);
        ASSERT_EQ(3, lines.size());
        ASSERT_EQ(_TT("a"), lines[0]);
        ASSERT_EQ(_TT("b"), lines[1]);
        ASSERT_EQ(_TT("c"), lines[2]);
    }

    // 空文字
    {
        String str = _TT("");
        List<String> lines = str.split(_TT(","), StringSplitOptions::None);
        ASSERT_EQ(1, lines.size());
    }

    // 空文字の RemoveEmptyEntries
    {
        String str = _TT("");
        List<String> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
        ASSERT_EQ(0, lines.size());
    }

    // 区切り無し
    {
        String str = _TT("abc");
        List<String> lines = str.split(_TT(","), StringSplitOptions::None);
        ASSERT_EQ(1, lines.size());
        ASSERT_EQ(_TT("abc"), lines[0]);
    }

    // 区切り無し RemoveEmptyEntries
    {
        String str = _TT("abc");
        List<String> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
        ASSERT_EQ(1, lines.size());
    }

    // トークン無し
    {
        String str = _TT(",");
        List<String> lines = str.split(_TT(","), StringSplitOptions::None);
        ASSERT_EQ(2, lines.size());
        ASSERT_EQ(_TT(""), lines[0]);
        ASSERT_EQ(_TT(""), lines[1]);
    }

    // トークン無し RemoveEmptyEntries
    {
        String str = _TT(",");
        List<String> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
        ASSERT_EQ(0, lines.size());
    }

    // トークン無し 2
    {
        String str = _TT(",,");
        List<String> lines = str.split(_TT(","), StringSplitOptions::None);
        ASSERT_EQ(3, lines.size());
    }

    // トークン無し 2 RemoveEmptyEntries
    {
        String str = _TT(",,");
        List<String> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
        ASSERT_EQ(0, lines.size());
    }

    // 空要素付き
    {
        String str = _TT("a,,c");
        List<String> lines = str.split(_TT(","), StringSplitOptions::None);
        ASSERT_EQ(3, lines.size());
        ASSERT_EQ(_TT("a"), lines[0]);
        ASSERT_EQ(_TT(""), lines[1]);
        ASSERT_EQ(_TT("c"), lines[2]);
    }

    // 空要素付き RemoveEmptyEntries
    {
        String str = _TT("a,,c");
        List<String> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
        ASSERT_EQ(2, lines.size());
        ASSERT_EQ(_TT("a"), lines[0]);
        ASSERT_EQ(_TT("c"), lines[1]);
    }

    // 先頭空要素付き
    {
        String str = _TT(",b,c");
        List<String> lines = str.split(_TT(","), StringSplitOptions::None);
        ASSERT_EQ(3, lines.size());
        ASSERT_EQ(_TT(""), lines[0]);
        ASSERT_EQ(_TT("b"), lines[1]);
        ASSERT_EQ(_TT("c"), lines[2]);
    }

    // 先頭空要素付き RemoveEmptyEntries
    {
        String str = _TT(",b,c");
        List<String> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
        ASSERT_EQ(2, lines.size());
        ASSERT_EQ(_TT("b"), lines[0]);
        ASSERT_EQ(_TT("c"), lines[1]);
    }

    // 終端空要素付き
    {
        String str = _TT("a,b,");
        List<String> lines = str.split(_TT(","), StringSplitOptions::None);
        ASSERT_EQ(3, lines.size());
        ASSERT_EQ(_TT("a"), lines[0]);
        ASSERT_EQ(_TT("b"), lines[1]);
        ASSERT_EQ(_TT(""), lines[2]);
    }

    // 終端空要素付き RemoveEmptyEntries
    {
        String str = _TT("a,b,");
        List<String> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
        ASSERT_EQ(2, lines.size());
        ASSERT_EQ(_TT("a"), lines[0]);
        ASSERT_EQ(_TT("b"), lines[1]);
    }

    // 両端空要素付き
    {
        String str = _TT(",b,");
        List<String> lines = str.split(_TT(","), StringSplitOptions::None);
        ASSERT_EQ(3, lines.size());
        ASSERT_EQ(_TT(""), lines[0]);
        ASSERT_EQ(_TT("b"), lines[1]);
        ASSERT_EQ(_TT(""), lines[2]);
    }

    // 両端空要素付き RemoveEmptyEntries
    {
        String str = _TT(",b,");
        List<String> lines = str.split(_TT(","), StringSplitOptions::RemoveEmptyEntries);
        ASSERT_EQ(1, lines.size());
        ASSERT_EQ(_TT("b"), lines[0]);
    }
    // 長さ2以上のデリミタ
    {
        String str = _TT("a::b");
        List<String> lines = str.split(_TT("::"), StringSplitOptions::None);
        ASSERT_EQ(2, lines.size());
        ASSERT_EQ(_TT("a"), lines[0]);
        ASSERT_EQ(_TT("b"), lines[1]);
    }
}

TEST_F(Test_Base_String, splitLines) {
    {
        String str = U"";
        ASSERT_EQ(0, str.splitLines().length());
    }
    {
        String str = U"a";
        auto lines = str.splitLines();
        ASSERT_EQ(1, lines.length());
        ASSERT_EQ(U"a", lines[0]);
    }
    {
        String str = U"\na";
        auto lines = str.splitLines();
        ASSERT_EQ(2, lines.length());
        ASSERT_EQ(U"", lines[0]);
        ASSERT_EQ(U"a", lines[1]);
    }
    {
        String str = U"a\n";
        auto lines = str.splitLines();
        ASSERT_EQ(2, lines.length());
        ASSERT_EQ(U"a", lines[0]);
        ASSERT_EQ(U"", lines[1]);
    }
    {
        String str = U"\n";
        auto lines = str.splitLines();
        ASSERT_EQ(2, lines.length());
        ASSERT_EQ(U"", lines[0]);
        ASSERT_EQ(U"", lines[1]);
    }
    {
        String str = U"\r\n";
        auto lines = str.splitLines();
        ASSERT_EQ(2, lines.length());
        ASSERT_EQ(U"", lines[0]);
        ASSERT_EQ(U"", lines[1]);
    }
    {
        String str = U"\n\r";
        auto lines = str.splitLines();
        ASSERT_EQ(3, lines.length());
        ASSERT_EQ(U"", lines[0]);
        ASSERT_EQ(U"", lines[1]);
        ASSERT_EQ(U"", lines[2]);
    }
    {
        String str = U"\n\n";
        auto lines = str.splitLines();
        ASSERT_EQ(3, lines.length());
        ASSERT_EQ(U"", lines[0]);
        ASSERT_EQ(U"", lines[1]);
        ASSERT_EQ(U"", lines[2]);
    }
    {
        String str = U"a\nb\nc";
        auto lines = str.splitLines();
        ASSERT_EQ(3, lines.length());
        ASSERT_EQ(U"a", lines[0]);
        ASSERT_EQ(U"b", lines[1]);
        ASSERT_EQ(U"c", lines[2]);
    }
    {
        String str = U"a\r\nb\nc\rd";
        auto lines = str.splitLines();
        ASSERT_EQ(4, lines.length());
        ASSERT_EQ(U"a", lines[0]);
        ASSERT_EQ(U"b", lines[1]);
        ASSERT_EQ(U"c", lines[2]);
        ASSERT_EQ(U"d", lines[3]);
    }
}

TEST_F(Test_Base_String, toStdString) {
    ASSERT_EQ("abc", String(_LT("abc")).toStdString());
}

TEST_F(Test_Base_String, toStdWString) {
    ASSERT_EQ(L"abc", String(_LT("abc")).toStdWString());
}

//## 文字列と数値の変換
TEST_F(Test_Base_String, ConvertNumeric) {
    //-[ ] 文字列 > 数値
    {
        ASSERT_EQ(10, String(_LT("10")).toInt8());
        ASSERT_EQ(10, String(_LT("10")).toInt16());
        ASSERT_EQ(10, String(_LT("10")).toInt32());
        ASSERT_EQ(10, String(_LT("10")).toInt64());
        ASSERT_EQ(10, String(_LT("10")).toUInt8());
        ASSERT_EQ(10, String(_LT("10")).toUInt16());
        ASSERT_EQ(10, String(_LT("10")).toUInt32());
        ASSERT_EQ(10, String(_LT("10")).toUInt64());
    }

    //-[ ] 数値 > 文字列
    {
        ASSERT_EQ(_TT("10"), String::fromNumber(10));
        ASSERT_EQ(_TT("10.500000"), String::fromNumber(10.5, 'F', 6));
    }

    // TODO:異常系
    //ASSERT_THROW(String(_LT("10")).toInt8(1), ArgumentException);
    //ASSERT_THROW(String(_LT("-")).toInt8(), InvalidFormatException);
    //ASSERT_THROW(String(_LT("qwer")).toInt8(), InvalidFormatException);
    //ASSERT_THROW(String(_LT("0xfffffffffffffffff")).toInt8(), OverflowException);
}

TEST_F(Test_Base_String, convertNativeCharString) {
    String str = _TT("abc");
    ASSERT_EQ("abc", str.toStdString());
    ASSERT_EQ(L"abc", str.toStdWString());

    //* [ ] Native(MultiByte) -> ln::String
    {
        Char str1[] = { 0x3042, 0x0000 }; // "あ"

        if (TextEncoding::systemMultiByteEncoding()->name() == _TT("UTF-8")) {
            uint8_t s[] = { 0xE3, 0x81, 0x82, 0x00 }; // "あ"
            ASSERT_EQ(str1, String::fromCString((char*)s));
        }
#ifdef LN_OS_WIN32
        // CI 環境では英語版の Windows が使われることがあるので、ひとまず逃げる
        else if (TextEncoding::systemMultiByteEncoding()->name() == _TT("cp932")) {
            uint8_t s[] = { 0x82, 0xA0, 0x00 }; // "あ" (SJIS)
            ASSERT_EQ(str1, String::fromCString((char*)s));
        }
#endif

        wchar_t s[] = { 0x3042, 0x0000 };
        ASSERT_EQ(str1, String::fromCString(s));
    }
}

TEST_F(Test_Base_String, unordered_map) {
    // <Test> unordered_map のキーにできること (char)
    {
        std::unordered_map<String, int> map1;
        map1[_TT("key1")] = 1;
        map1.insert(std::pair<String, int>(_TT("key2"), 2));
        ASSERT_EQ(1, map1[_TT("key1")]);
        ASSERT_EQ(2, map1.find(_TT("key2"))->second);

        std::unordered_map<String, int> map2 = {
            { _TT("key1"), 1 },
            { _TT("key2"), 2 },
        };
        ASSERT_EQ(1, map2[_TT("key1")]);
        ASSERT_EQ(2, map2.find(_TT("key2"))->second);
    }
    // <Test> std::hash の String の特殊化テスト
    {
        std::hash<ln::String> hash1;
        ASSERT_NE(hash1(String(_TT("key1"))), hash1(String(_TT("key2"))));
    }
    // <Test> StringView で検索
    {
        String key1 = _TT("key1");
        StringView key1ref(key1);
        String key2 = _TT("key2");
        StringView key2ref(key2);

        std::unordered_map<String, int> map1;
        map1[key1ref] = 1;
        map1.insert(std::pair<String, int>(key2ref, 2));
        ASSERT_EQ(1, map1[key1ref]);
        ASSERT_EQ(2, map1.find(key2ref)->second);
    }
}

TEST_F(Test_Base_String, conv_case) {
    String s1 = _TT("AbCd");
    ASSERT_EQ(_TT("ABCD"), s1.toUpper());
    String s2 = _TT("AbCd");
    ASSERT_EQ(_TT("abcd"), s2.toLower());
    String s3 = _TT("aBcD");
    ASSERT_EQ(_TT("Abcd"), s3.toTitleCase());
}

TEST_F(Test_Base_String, concat) {
    String s1 = String::concat(_TT("a"), _TT("b"));
    ASSERT_EQ(_TT("ab"), s1);

    s1 = String::concat(_TT("12345678901234567890"), _TT("12345678901234567890"));
    ASSERT_EQ(_TT("1234567890123456789012345678901234567890"), s1);
}

TEST_F(Test_Base_String, MemoryAllocation) {
    // <Test> 共有も SSO も無い状態で clear してもメモリ再確保は発生しないこと
    {
        String str = _LT("1234567890123456789012345678901234567890");
        intptr_t pt1 = (intptr_t)str.c_str();
        int rr1 = str.capacity();
        str.clear();
        int rr2 = str.capacity();
        str = _LT("123");
        int rr3 = str.capacity();
        intptr_t pt2 = (intptr_t)str.c_str();
        ASSERT_EQ(rr1, rr2);
        ASSERT_EQ(rr1, rr3);
        ASSERT_EQ(rr1, rr3);
        ASSERT_EQ(pt1, pt2);
    }
}

#ifdef LN_STRING_FUZZY_CONVERSION
TEST_F(Test_Base_String, Issue) {
    {
        StringRef s1 = "aaa";
        StringRef s2 = s1;
    }
}
#endif

//## 自己代入のテスト (this ではなく c_str() 等で取れる char* 経由)
TEST_F(Test_Base_String, SelfAssign) {
    //- [ ] NonSSO -> NonSSO
    {
        String str = _LT("1234567890abcdefg");
        StringView r1 = str.substr(1);
        str = r1;
        ASSERT_EQ(_TT("234567890abcdefg"), str);
    }
    //- [ ] SSO -> SSO
    {
        String str = _LT("123");
        StringView r1 = str.substr(1);
        str = r1;
        ASSERT_EQ(_TT("23"), str);
    }
    //- [ ] NonSSO -> SSO
    {
        String str = _LT("1234567890abcdefg");
        StringView r1 = str.substr(10);
        str = r1;
        ASSERT_EQ(_TT("abcdefg"), str);
    }
    //- [ ] SSO ->  NonSSO
    {
        String str = _LT("1234567890");
        StringView r1 = str;
        str += r1;
        ASSERT_EQ(_TT("12345678901234567890"), str);
    }
}

//## Char 代入
TEST_F(Test_Base_String, SetAt) {
    //- [ ] インデックス指定の代入で文字列を変更できる
    {
        String str1 = _LT("12345");
        String str2 = str1;
        str1[2] = '_';
        ASSERT_EQ(_TT("12_45"), str1);
        ASSERT_EQ(_TT("12345"), str2); // 共有解除。変更されない。
    }
}

TEST_F(Test_Base_String, StringHelper_indexOf) {
    String str1 = U"abcdefg";
    ASSERT_EQ(-1, StringHelper::indexOf(str1.c_str(), 3, U"fg", 2));
}
