#include <TestConfig.h>
#include <unordered_map>
#include <Lumino/Text/Encoding.h>



class Test_Base_String : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

#if 0
String g_str1;
std::string g_str2;

static String GetTest1()
{
	static const int i = 0;
	char buf[] = "01111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";
	buf[0] = +(i % 10);
	//return String(buf);
	g_str1 = buf;
	return g_str1;
}
static std::string GetTest2()
{
	static const int i = 0;
	char buf[] = "01111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";
	buf[0] = +(i % 10);
	//return std::string(buf);
	g_str2 = buf;
	return g_str2;
}
#endif

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, Constructor)
{
#if 0 // COW パフォーマンステスト
	String str1 = "";
	String str12 = "";
	std::string str2 = "";
	std::string str22 = "";
	List<String> ary1;
	List<String> ary2;


	printf("----\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.Start();
		for (int i = 0; i < 1000000; ++i)
		{
			str1 = GetTest1();
			str12 = str1;
		}
		printf("t:%llu\n", timer.GetElapsedTime());
	}
	printf("----\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.Start();
		for (int i = 0; i < 1000000; ++i)
		{
			str2 = GetTest2();
			str22 = str2;
		}
		printf("t:%llu\n", timer.GetElapsedTime());
	}
	printf("----\n");
#endif

	{
		std::string str1("test", 0, 4);
		StringA str2("test", 0, 4);
		EXPECT_EQ(str1, str2.c_str());
	}

	{
		//std::string str1(NULL);
		//EXPECT_TRUE(str1.empty());
	}

	{
		StringA strA((char*)NULL);
		ASSERT_TRUE(strA.IsEmpty());
		StringW strW((wchar_t*)NULL);
		ASSERT_TRUE(strW.IsEmpty());
	}

}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, Operators)
{
	std::string strAStd = "a";
	std::wstring strWStd = L"w";
	StringA strASample = "a";
	StringW strWSample = L"w";
	StringA strAEmpty;
	StringW strWEmpty;
	StringA strA;
	StringW strW;

	// <Test> operator= (文字列ポインタ)
	{
		strA = "a";
		strW = L"w";
		ASSERT_EQ("a", strA);
		ASSERT_EQ(L"w", strW);
	}
	// <Test> operator= (GenericString)
	{
		strA = strASample;
		strW = strWSample;
		ASSERT_EQ("a", strA);
		ASSERT_EQ(L"w", strW);
	}
	// <Test> operator= (std::string)
	{
		strA = strAStd;
		strW = strWStd;
		ASSERT_EQ("a", strA);
		ASSERT_EQ(L"w", strW);
	}
	// <Test> operator= (文字列ポインタ・ASCII/Wide逆の型)
	{
		strA = L"w";
		strW = "a";
		ASSERT_EQ("w", strA);
		ASSERT_EQ(L"a", strW);
	}
	// <Test> operator= (NULL)
	{
		strA = ((char*)NULL);
		strW = ((wchar_t*)NULL);
		ASSERT_TRUE(strA.IsEmpty());
		ASSERT_TRUE(strW.IsEmpty());
	}
	// <Test> operator= (自己代入)
	{
		strA = strASample;
		strW = strWSample;
		strA = strA;
		strW = strW;
		ASSERT_EQ("a", strA);
		ASSERT_EQ(L"w", strW);
	}

	strA = "a";
	strW = L"w";
	// <Test> operator== (GenericString)
	{
		ASSERT_TRUE(strA == strASample);
		ASSERT_TRUE(strW == strWSample);
	}
	// <Test> operator== (文字列ポインタ)
	{
		ASSERT_TRUE(strA == "a");
		ASSERT_TRUE(strW == L"w");
	}
	// <Test> operator== (NULL)
	{
		ASSERT_FALSE(strA == ((char*)NULL));
		ASSERT_FALSE(strW == ((wchar_t*)NULL));
		ASSERT_TRUE(strAEmpty == ((char*)NULL));
		ASSERT_TRUE(strWEmpty == ((wchar_t*)NULL));
	}
	// <Test> operator== (自己比較)
	{
		ASSERT_TRUE(strA == strA);
		ASSERT_TRUE(strW == strW);
	}
	// <Test> operator!= (GenericString)
	{
		ASSERT_FALSE(strA != strASample);
		ASSERT_FALSE(strW != strWSample);
	}
	// <Test> operator!= (文字列ポインタ)
	{
		ASSERT_FALSE(strA != "a");
		ASSERT_FALSE(strW != L"w");
	}
	// <Test> operator!= (NULL)
	{
		ASSERT_TRUE(strA != ((char*)NULL));
		ASSERT_TRUE(strW != ((wchar_t*)NULL));
		ASSERT_FALSE(strAEmpty != ((char*)NULL));
		ASSERT_FALSE(strWEmpty != ((wchar_t*)NULL));
	}

	strA = "a";
	strW = L"w";
	// <Test> operator+= (GenericString)
	{
		strA += strASample;
		strW += strWSample;
		ASSERT_EQ("aa", strA);
		ASSERT_EQ(L"ww", strW);
	}
	// <Test> operator+= (文字列ポインタ)
	{
		strA += "a";
		strW += L"w";
		ASSERT_EQ("aaa", strA);
		ASSERT_EQ(L"www", strW);
	}
	// <Test> operator+= (文字)
	{
		strA += 'a';
		strW += L'w';
		ASSERT_EQ("aaaa", strA);
		ASSERT_EQ(L"wwww", strW);
	}
	// <Test> operator+= (NULL)
	{
		strA += ((char*)NULL);
		strW += ((wchar_t*)NULL);
		ASSERT_EQ("aaaa", strA);
		ASSERT_EQ(L"wwww", strW);
	}

	strA = "a";
	strW = L"w";
	StringA strA2 = "A";
	StringW strW2 = L"W";
	// <Test> operator < (GenericString)
	{
		ASSERT_TRUE(strA2 < strA);
		ASSERT_TRUE(strW2 < strW);
	}
	// <Test> operator < (文字列ポインタ)
	{
		ASSERT_TRUE(strA < "b");
		ASSERT_TRUE(strW < L"x");
	}
	// <Test> operator < (空文字列)
	{
		ASSERT_FALSE(strA < StringA());
		ASSERT_FALSE(strW < StringW());
		ASSERT_FALSE(strA < "");
		ASSERT_FALSE(strW < L"");
	}
	// <Test> operator < (NULL)
	{
		ASSERT_FALSE(strA < ((char*)NULL));
		ASSERT_FALSE(strW < ((wchar_t*)NULL));
	}

	// <Test> operator > (GenericString)
	{
		ASSERT_FALSE(strA2 > strA);
		ASSERT_FALSE(strW2 > strW);
	}
	// <Test> operator > (文字列ポインタ)
	{
		ASSERT_FALSE(strA > "b");
		ASSERT_FALSE(strW > L"x");
	}
	// <Test> operator > (空文字列)
	{
		ASSERT_TRUE(strA > StringA());
		ASSERT_TRUE(strW > StringW());
		ASSERT_TRUE(strA > "");
		ASSERT_TRUE(strW > L"");
	}
	// <Test> operator > (NULL)
	{
		ASSERT_TRUE(strA > ((char*)NULL));
		ASSERT_TRUE(strW > ((wchar_t*)NULL));
	}

	// <Test> operator <= (GenericString)
	{
		ASSERT_TRUE(strA2 <= strA);
		ASSERT_TRUE(strW2 <= strW);
	}
	// <Test> operator <= (文字列ポインタ)
	{
		ASSERT_TRUE(strA <= "b");
		ASSERT_TRUE(strW <= L"x");
	}
	// <Test> operator >= (GenericString)
	{
		ASSERT_FALSE(strA2 >= strA);
		ASSERT_FALSE(strW2 >= strW);
	}
	// <Test> operator >= (文字列ポインタ)
	{
		ASSERT_FALSE(strA >= "b");
		ASSERT_FALSE(strW >= L"x");
	}

	// <Test> operator +
	{
		String str = String(_T("a")) + String(_T("b"));
		ASSERT_EQ(_T("ab"), str);
	}
	// <Test> operator +
	{
		String str = String(_T("a")) + _T("b");
		ASSERT_EQ(_T("ab"), str);
	}
	// <Test> operator +
	{
		String str = _T("a") + String(_T("b"));
		ASSERT_EQ(_T("ab"), str);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, XYChar)
{

	//String str1("multi");
	//ASSERT_STREQ(_T("multi"), str1.c_str());

	////String str2(L"wide");
	////ASSERT_STREQ(_T("wide"), str2.c_str());

	//str1 = "multi";
	//ASSERT_STREQ(_T("multi"), str1.c_str());

	//str2 = L"wide";
	//ASSERT_STREQ(_T("wide"), str2.c_str());
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, AssignCStr)
{
	// char、wchar_t の相互変換
	{ 
		StringA str1;
		str1.AssignCStr("test");
		StringA str2;
		str2.AssignCStr(L"test");
		StringW wstr1;
		wstr1.AssignCStr("test");
		StringW wstr2;
		wstr2.AssignCStr(L"test");

		ASSERT_STREQ("test", str1.c_str());
		ASSERT_STREQ("test", str2.c_str());
		ASSERT_STREQ(L"test", wstr1.c_str());
		ASSERT_STREQ(L"test", wstr2.c_str());

		StringW tstr3;
		tstr3.AssignCStr("f");
		ASSERT_EQ(L"f", tstr3);
	}

	// 部分変換
	{
		StringA str1;
		str1.AssignCStr("test", 0, 2);
		ASSERT_EQ("te", str1);

		StringA str2;
		str2.AssignCStr(L"test", 0, 2);
		ASSERT_EQ("te", str1);

		StringW wstr1;
		wstr1.AssignCStr("test", 0, 2);
		ASSERT_EQ(L"te", wstr1);

		StringW wstr2;
		wstr2.AssignCStr(L"test", 0, 2);
		ASSERT_EQ(L"te", wstr1);
	}

	// 同型の NULL ポインタを指定する => 空文字が設定される
	{
		StringA strA;
		strA.AssignCStr((char*)NULL, 1, 1);
		ASSERT_TRUE(strA.IsEmpty());

		StringW strW;
		strW.AssignCStr((wchar_t*)NULL, 1, 1);
		ASSERT_TRUE(strA.IsEmpty());
	}

	//// 例外
	//{
	//	StringA str1;
	//	ASSERT_THROW(
	//		str1.AssignCStr((const char*)NULL, 1, 1),
	//		ArgumentException);

	//	ASSERT_THROW(
	//		str1.AssignCStr((const wchar_t*)NULL, 1, 1),
	//		ArgumentException);
	//}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, SPrintf)
{
	// StringA Max 文字数チェック
	{
		char buf1[2048 + 1] = { 0 };
		for (int i = 0; i < 2048; i++) {
			buf1[i] = 'a';
		}

		StringA str1 = StringA::SPrintf("%s", buf1);
		ASSERT_EQ(str1, buf1);	// 同じ文字ができていればOK
		
#if 0	// 制限撤廃
		ASSERT_THROW(
			StringA::Format("%sb", buf1),	// 1文字多い。失敗する
			ArgumentException);
#endif
	}

//#ifdef LN_MSVC
//	// 可変長実引数への指定は保障外だが、一応動くか見ておく。ちなみに GCC ではコンパイルエラーになる。
//	{
//		String str1(_T("str1"));
//		String str2 = String::SPrintf(_T("[%s]"), str1);
//		ASSERT_EQ(_T("[str1]"), str2);
//	}
//#endif

	// StringW Max 文字数チェック
	{
		wchar_t buf1[2048 + 1] = { 0 };
		for (int i = 0; i < 2048; i++) {
			buf1[i] = L'a';
		}

		StringW str1 = StringW::SPrintf(L"%s", buf1);
		ASSERT_TRUE(str1 == buf1);	// 同じ文字ができていればOK
		
#if 0	// 制限撤廃
		ASSERT_THROW(
			StringW::SPrintf(L"%sb", buf1),	// 1文字多い。失敗する
			ArgumentException);
#endif
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, ConvertTo)
{
	{
		StringA str1("test");
		ByteBuffer buf = str1.ConvertTo(Encoding::GetWideCharEncoding());
		wchar_t* wstr = (wchar_t*)buf.GetData();
		ASSERT_EQ(sizeof(wchar_t) * 5, buf.GetSize());
		ASSERT_EQ(L't', wstr[0]);
		ASSERT_EQ(L'e', wstr[1]);
		ASSERT_EQ(L's', wstr[2]);
		ASSERT_EQ(L't', wstr[3]);
		ASSERT_EQ(L'\0', wstr[4]);
	}

	{
		StringW str1(L"test");
		ByteBuffer buf = str1.ConvertTo(Encoding::GetSystemMultiByteEncoding());
		char* astr = (char*)buf.GetData();
		ASSERT_EQ(5, buf.GetSize());
		ASSERT_EQ('t', astr[0]);
		ASSERT_EQ('e', astr[1]);
		ASSERT_EQ('s', astr[2]);
		ASSERT_EQ('t', astr[3]);
		ASSERT_EQ('\0', astr[4]);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, IndexOf)
{
	String str1(_T("abcdef"));
	ASSERT_EQ(1, str1.IndexOf(_T("bcd")));
	ASSERT_EQ(4, str1.IndexOf(_T('e')));

	ASSERT_EQ(1, str1.IndexOf(String(_T("bcd"))));
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, LastIndexOf)
{
	{
		String str = _T("abcdef");
		ASSERT_EQ(3, str.LastIndexOf(_T("de")));
		ASSERT_EQ(1, str.LastIndexOf(_T("bc"), 2));
		ASSERT_EQ(-1, str.LastIndexOf(_T("cd"), 2));
		ASSERT_EQ(2, str.LastIndexOf(_T("cd"), 4, 3));
		ASSERT_EQ(-1, str.LastIndexOf(_T("bc"), 4, 3));
	}
	// .NET の System.String.LastIndexOf() と同様の動作
	{
		String str = _T("crazy azimuths");
		String se = _T("");
		ASSERT_EQ(6, str.LastIndexOf(_T("az")));       // 6
		ASSERT_EQ(2, str.LastIndexOf(_T("az"), 6));    // 2 ("crazy a")
		ASSERT_EQ(6, str.LastIndexOf(_T("az"), 7));    // 6 ("crazy az")
		ASSERT_EQ(0, str.LastIndexOf(_T("cr"), 6));    // 0
		ASSERT_EQ(-1, str.LastIndexOf(_T("cr"), 6, 4));    // -1
		ASSERT_EQ(-1, str.LastIndexOf(_T("cr"), 6, 6));    // -1
		ASSERT_EQ(0, str.LastIndexOf(_T("cr"), 6, 7));    // 0
		ASSERT_EQ(5, str.LastIndexOf(_T(""), 5, 2));    // 5
		ASSERT_EQ(3, str.LastIndexOf(_T(""), 3, 0));
		ASSERT_EQ(0, se.LastIndexOf(_T("")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, Trim)
{
	// 前後
	{
		String str1(_T(" abc def "));
		String t = str1.Trim();
		ASSERT_EQ(_T("abc def"), t);
	}
	// 前だけ
	{
		String str1(_T(" abc def"));
		String t = str1.Trim();
		ASSERT_EQ(_T("abc def"), t);
	}
	// 後ろだけ
	{
		String str1(_T("abc def "));
		String t = str1.Trim();
		ASSERT_EQ(_T("abc def"), t);
	}
	// 空文字
	{
		String str1(_T(""));
		String t = str1.Trim();
		ASSERT_EQ(_T(""), t);
	}
	// 空白だけ
	{
		String str1(_T(" "));
		String t = str1.Trim();
		ASSERT_EQ(_T(""), t);
	}
	// 空白だけ * 2
	{
		String str1(_T("  "));
		String t = str1.Trim();
		ASSERT_EQ(_T(""), t);
	}
	// 空白だけ * 3
	{
		String str1(_T("   "));
		String t = str1.Trim();
		ASSERT_EQ(_T(""), t);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, Remove)
{
	String str1(_T("abcdef"));

	String str2 = str1.Remove(_T('c'));
	ASSERT_EQ(_T("abdef"), str2);

	// 大文字小文字を区別しない
	String str3 = str2.Remove(_T('D'), CaseSensitivity::CaseInsensitive);
	ASSERT_EQ(_T("abef"), str3);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, Replace)
{
	// <Test> 部分一致を置換できること。
	// <Test> TCHAR* を渡せること。
	{
		StringA str1("test");
		StringA str2 = str1.Replace("es", "b");
		ASSERT_EQ('t', str2[0]);
		ASSERT_EQ('b', str2[1]);
		ASSERT_EQ('t', str2[2]);
	}
	// <Test> 全ての一致を置換できること。
	// <Test> 破壊的変更にならないこと。
	{
		String str1(_T("aaa"));
		String str2 = str1.Replace(String(_T("a")), String(_T("b")));
		ASSERT_EQ(_T("bbb"), str2);
		ASSERT_EQ(_T("aaa"), str1);
	}
	// <Test> String を渡せること。
	{
		String str1(_T("a"));
		String str2 = str1.Replace(String(_T("a")), String(_T("b")));
		ASSERT_EQ(_T("b"), str2);
	}
	// <Test> StringRef を渡せること。
	{
		String str1(_T("reabcdef"));
		TCHAR buf1[] = _T("abc");
		TCHAR buf2[] = _T("def");
		String str2 = str1.Replace(StringRef(buf1, buf1 + 2), StringRef(buf2, buf2 + 2));
		ASSERT_EQ(_T("redecdef"), str2);
	}
	// <Test> 文字列を置換する
	{
		String str1 = _T("abcdcd");

		// "ab" を "12" に置き換える
		str1 = str1.Replace(_T("ab"), _T("12"));

		// "cd" を "345" に置き換える
		String from = _T("cd");
		String to = _T("345");
		str1 = str1.Replace(from, to);

		ASSERT_EQ(_T("12345345"), str1);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, Equals)
{
	// <Issue> 文字列の先頭が同じだけで一致判定にならないこと。
	{
		String str1("abc");
		String str2("abcd");
		ASSERT_FALSE(str1.Equals(str2));
		ASSERT_FALSE(str2.Equals(str1));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, Compare)
{
	int a1 = strcmp("abc", "abcd");
	int a2 = strcmp("bbb", "aaaa");
	int a3 = strncmp("abc", "abcd", 4);
	int a4 = strncmp("abcd", "abc", 4);

	{
		String str1("abc");
		EXPECT_EQ(0, str1.Compare(_T("abc")));
		EXPECT_LE(0, str1.Compare(_T("ab")));	// v1 < v2
		EXPECT_GT(0, str1.Compare(_T("abd")));	// v1 > v2
	}

	{
		String str1("abc");
		String str2("abcd");
		ASSERT_TRUE(str1 < str2);
	}

	// <Test> StringRef との比較
	{
		ASSERT_EQ(true,  String(_T("abc")) == StringRef(_T("abc")));
		ASSERT_EQ(false, String(_T("abcd")) == StringRef(_T("abc")));
		ASSERT_EQ(false, String(_T("abc")) == StringRef(_T("abcd")));
		ASSERT_EQ(false, String(_T("abc")) == StringRef(_T("a")));
		ASSERT_EQ(false, String(_T("abc")) == StringRef(_T("ab")));
		ASSERT_EQ(false, String(_T("a")) == StringRef(_T("abc")));
		ASSERT_EQ(false, String(_T("ab")) == StringRef(_T("abc")));
		ASSERT_EQ(false, String(_T("abc")) == StringRef(_T("")));
		ASSERT_EQ(false, String(_T("")) == StringRef(_T("abc")));

		ASSERT_EQ(true,  String(_T("abc")) == StringRef(_T("abcd"), 3));
		ASSERT_EQ(false, String(_T("abcd")) == StringRef(_T("abcd"), 3));
		ASSERT_EQ(false, String(_T("abc")) == StringRef(_T("abcd"), 4));
		ASSERT_EQ(false, String(_T("abc")) == StringRef(_T("abcd"), 1));
		ASSERT_EQ(false, String(_T("abc")) == StringRef(_T("abcd"), 2));
		ASSERT_EQ(false, String(_T("a")) == StringRef(_T("abcd"), 3));
		ASSERT_EQ(false, String(_T("ab")) == StringRef(_T("abcd"), 3));
		ASSERT_EQ(false, String(_T("abc")) == StringRef(_T("abcd"), 0));
		ASSERT_EQ(false, String(_T("")) == StringRef(_T("abcd"), 3));
	}

	// <Test> 比較
	{
		ASSERT_LE(0, StringTraits::Compare(_T("abcd"), 4, _T("abc"), 3, 4));	// 0 < result (1)
		ASSERT_GT(0, StringTraits::Compare(_T("abc"), 3, _T("abcd"), 4, 4));	// result < 0 (-1)
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, Left)
{
	String str1(_T("abcdef"));
	ASSERT_EQ(_T("ab"), str1.Left(2));
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, Right)
{
	String str1(_T("abcdef"));
	ASSERT_EQ(_T("ef"), str1.Right(2));
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, Mid)
{
	String str1(_T("abcdef"));
	ASSERT_EQ(_T("cde"), str1.Mid(2, 3));
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, Split)
{
	// 普通の分割
	{
		String str(_T("a,b,c"));
		List<String> lines = str.Split(_T(","), StringSplitOptions::None);
		EXPECT_EQ(3, lines.GetCount());
		ASSERT_EQ(_T("a"), lines[0]);
		ASSERT_EQ(_T("b"), lines[1]);
		ASSERT_EQ(_T("c"), lines[2]);
	}

	// 空文字
	{
		String str = _T("");
		List<String> lines = str.Split(_T(","), StringSplitOptions::None);
		EXPECT_EQ(1, lines.GetCount());
	}

	// 空文字の RemoveEmptyEntries
	{
		String str = _T("");
		List<String> lines = str.Split(_T(","), StringSplitOptions::RemoveEmptyEntries);
		EXPECT_EQ(0, lines.GetCount());
	}

	// 区切り無し
	{
		String str = _T("abc");
		List<String> lines = str.Split(_T(","), StringSplitOptions::None);
		EXPECT_EQ(1, lines.GetCount());
	}

	// 区切り無し RemoveEmptyEntries
	{
		String str = _T("abc");
		List<String> lines = str.Split(_T(","), StringSplitOptions::RemoveEmptyEntries);
		EXPECT_EQ(1, lines.GetCount());
	}

	// トークン無し
	{
		String str = _T(",");
		List<String> lines = str.Split(_T(","), StringSplitOptions::None);
		EXPECT_EQ(2, lines.GetCount());
	}

	// トークン無し RemoveEmptyEntries
	{
		String str = _T(",");
		List<String> lines = str.Split(_T(","), StringSplitOptions::RemoveEmptyEntries);
		EXPECT_EQ(0, lines.GetCount());
	}

	// トークン無し 2
	{
		String str = _T(",,");
		List<String> lines = str.Split(_T(","), StringSplitOptions::None);
		EXPECT_EQ(3, lines.GetCount());
	}

	// トークン無し 2 RemoveEmptyEntries
	{
		String str = _T(",,");
		List<String> lines = str.Split(_T(","), StringSplitOptions::RemoveEmptyEntries);
		EXPECT_EQ(0, lines.GetCount());
	}

	// 空要素付き
	{
		String str = _T("a,,c");
		List<String> lines = str.Split(_T(","), StringSplitOptions::None);
		EXPECT_EQ(3, lines.GetCount());
		ASSERT_EQ(_T("a"), lines[0]);
		ASSERT_EQ(_T(""), lines[1]);
		ASSERT_EQ(_T("c"), lines[2]);
	}

	// 空要素付き RemoveEmptyEntries
	{
		String str = _T("a,,c");
		List<String> lines = str.Split(_T(","), StringSplitOptions::RemoveEmptyEntries);
		EXPECT_EQ(2, lines.GetCount());
		ASSERT_EQ(_T("a"), lines[0]);
		ASSERT_EQ(_T("c"), lines[1]);
	}

	// 先頭空要素付き
	{
		String str = _T(",b,c");
		List<String> lines = str.Split(_T(","), StringSplitOptions::None);
		EXPECT_EQ(3, lines.GetCount());
		ASSERT_EQ(_T(""), lines[0]);
		ASSERT_EQ(_T("b"), lines[1]);
		ASSERT_EQ(_T("c"), lines[2]);
	}

	// 先頭空要素付き RemoveEmptyEntries
	{
		String str = _T(",b,c");
		List<String> lines = str.Split(_T(","), StringSplitOptions::RemoveEmptyEntries);
		EXPECT_EQ(2, lines.GetCount());
		ASSERT_EQ(_T("b"), lines[0]);
		ASSERT_EQ(_T("c"), lines[1]);
	}

	// 終端空要素付き
	{
		String str = _T("a,b,");
		List<String> lines = str.Split(_T(","), StringSplitOptions::None);
		EXPECT_EQ(3, lines.GetCount());
		ASSERT_EQ(_T("a"), lines[0]);
		ASSERT_EQ(_T("b"), lines[1]);
		ASSERT_EQ(_T(""), lines[2]);
	}

	// 終端空要素付き RemoveEmptyEntries
	{
		String str = _T("a,b,");
		List<String> lines = str.Split(_T(","), StringSplitOptions::RemoveEmptyEntries);
		EXPECT_EQ(2, lines.GetCount());
		ASSERT_EQ(_T("a"), lines[0]);
		ASSERT_EQ(_T("b"), lines[1]);
	}

	// 両端空要素付き
	{
		String str = _T(",b,");
		List<String> lines = str.Split(_T(","), StringSplitOptions::None);
		EXPECT_EQ(3, lines.GetCount());
		ASSERT_EQ(_T(""), lines[0]);
		ASSERT_EQ(_T("b"), lines[1]);
		ASSERT_EQ(_T(""), lines[2]);
	}

	// 両端空要素付き RemoveEmptyEntries
	{
		String str = _T(",b,");
		List<String> lines = str.Split(_T(","), StringSplitOptions::RemoveEmptyEntries);
		EXPECT_EQ(1, lines.GetCount());
		ASSERT_EQ(_T("b"), lines[0]);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, NewLine)
{
#ifdef _WIN32
	const char* nl = StringA::GetNewLine().c_str();
	ASSERT_EQ('\r', nl[0]);
	ASSERT_EQ('\n', nl[1]);
	ASSERT_EQ('\0', nl[2]);

	const wchar_t* wnl = StringW::GetNewLine().c_str();
	ASSERT_EQ(L'\r', wnl[0]);
	ASSERT_EQ(L'\n', wnl[1]);
	ASSERT_EQ(L'\0', wnl[2]);

	const TCHAR* tnl = String::GetNewLine().c_str();
	ASSERT_EQ(_T('\r'), tnl[0]);
	ASSERT_EQ(_T('\n'), tnl[1]);
	ASSERT_EQ(_T('\0'), tnl[2]);
#else
	const char* nl = StringA::GetNewLine().c_str();
	ASSERT_EQ('\n', nl[0]);
	ASSERT_EQ('\0', nl[1]);

	const wchar_t* wnl = StringW::GetNewLine().c_str();
	ASSERT_EQ(L'\n', wnl[0]);
	ASSERT_EQ(L'\0', wnl[1]);

	const TCHAR* tnl = String::GetNewLine().c_str();
	ASSERT_EQ(_T('\n'), tnl[0]);
	ASSERT_EQ(_T('\0'), tnl[1]);
#endif
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, ToInt)
{
	// 実行できるか
	ASSERT_EQ(10, String(_T("10")).ToInt8());
	ASSERT_EQ(10, String(_T("10")).ToInt16());
	ASSERT_EQ(10, String(_T("10")).ToInt32());
	ASSERT_EQ(10, String(_T("10")).ToInt64());
	ASSERT_EQ(10, String(_T("10")).ToUInt8());
	ASSERT_EQ(10, String(_T("10")).ToUInt16());
	ASSERT_EQ(10, String(_T("10")).ToUInt32());
	ASSERT_EQ(10, String(_T("10")).ToUInt64());

	// 異常系
	ASSERT_THROW(String(_T("10")).ToInt8(1), ArgumentException);
	ASSERT_THROW(String(_T("-")).ToInt8(), InvalidFormatException);
	ASSERT_THROW(String(_T("qwer")).ToInt8(), InvalidFormatException);
	ASSERT_THROW(String(_T("0xfffffffffffffffff")).ToInt8(), OverflowException);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, TryToInt)
{
	// 実行できるか
	{
		int8_t v;
		ASSERT_TRUE(String(_T("10")).TryToInt8(&v));
		ASSERT_EQ(10, v);
	}
	{
		int16_t v;
		ASSERT_TRUE(String(_T("10")).TryToInt16(&v));
		ASSERT_EQ(10, v);
	}
	{
		int32_t v;
		ASSERT_TRUE(String(_T("10")).TryToInt32(&v));
		ASSERT_EQ(10, v);
	}
	{
		int64_t v;
		ASSERT_TRUE(String(_T("10")).TryToInt64(&v));
		ASSERT_EQ(10, v);
	}
	{
		uint8_t v;
		ASSERT_TRUE(String(_T("10")).TryToUInt8(&v));
		ASSERT_EQ(10, v);
	}
	{
		uint16_t v;
		ASSERT_TRUE(String(_T("10")).TryToUInt16(&v));
		ASSERT_EQ(10, v);
	}
	{
		uint32_t v;
		ASSERT_TRUE(String(_T("10")).TryToUInt32(&v));
		ASSERT_EQ(10, v);
	}
	{
		uint64_t v;
		ASSERT_TRUE(String(_T("10")).TryToUInt64(&v));
		ASSERT_EQ(10, v);
	}

	// 異常系
	int8_t v;
	ASSERT_FALSE(String(_T("10")).TryToInt8(&v, 1));
	ASSERT_FALSE(String(_T("-")).TryToInt8(&v));
	ASSERT_FALSE(String(_T("qwer")).TryToInt8(&v));
	ASSERT_FALSE(String(_T("0xfffffffffffffffff")).TryToInt8(&v));
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, FromNativeCharString)
{
	String str = String::FromNativeCharString("abc");
	ASSERT_EQ(_T("abc"), str);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, FromNativeWCharString)
{
	String str = String::FromNativeCharString(L"abc");
	ASSERT_EQ(_T("abc"), str);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, unordered_map)
{
	// <Test> unordered_map のキーにできること (char)
	{
		std::unordered_map<StringA, int> map1;
		map1["key1"] = 1;
		map1.insert(std::pair<StringA, int>("key2", 2));
		ASSERT_EQ(1, map1["key1"]);
		ASSERT_EQ(2, map1.find("key2")->second);

		std::unordered_map<StringA, int> map2 =
		{
			{ "key1", 1 },
			{ "key2", 2 },
		};
		ASSERT_EQ(1, map2["key1"]);
		ASSERT_EQ(2, map2.find("key2")->second);
	}
	// <Test> unordered_map のキーにできること (wchar_t)
	{
		std::unordered_map<StringW, int> map1;
		map1[L"key1"] = 1;
		map1.insert(std::pair<StringW, int>(L"key2", 2));
		ASSERT_EQ(1, map1[L"key1"]);
		ASSERT_EQ(2, map1.find(L"key2")->second);

		std::unordered_map<StringW, int> map2 =
		{
			{ L"key1", 1 },
			{ L"key2", 2 },
		};
		ASSERT_EQ(1, map2[L"key1"]);
		ASSERT_EQ(2, map2.find(L"key2")->second);
	}
	// <Test> std::hash の String の特殊化テスト
	{
		std::hash<ln::StringA> hash1;
		ASSERT_NE(hash1(StringA("key1")), hash1(StringA("key2")));

		std::hash<ln::StringW> hash2;
		ASSERT_NE(hash2(StringW("key1")), hash2(StringW("key2")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, Concat)
{
	// <Integration> 文字列を連結する
	{
		String str1 = _T("12");
		String str2 = _T("56");
		str1 = str1 + _T("34");
		str1 += str2;
		str1.Append(_T("78"));
		ASSERT_EQ(_T("12345678"), str1);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, ToUpper)
{
	String str1 = _T("AbCd");
	ASSERT_EQ(_T("ABCD"), str1.ToUpper());
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, ToLower)
{
	String str1 = _T("AbCd");
	ASSERT_EQ(_T("abcd"), str1.ToLower());
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_String, Issue)
{
	// <Issue> 空文字列への += で、他の String の初期値が変わってしまう。
	{
		String s1;
		String s2;
		s2 += _T("a");
		ASSERT_EQ(_T(""), s1);
		ASSERT_EQ(_T("a"), s2);
	}
}



