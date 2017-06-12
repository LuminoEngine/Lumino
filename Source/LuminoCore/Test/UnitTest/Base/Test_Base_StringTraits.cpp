#include <TestConfig.h>


//==============================================================================
class Test_Base_StringUtils : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, StrLen)
{
	UTF8 str_u8[] = { 'A', 'B', 'C', 0x00 };
	UTF16 str_u16[] = { 'A', 'B', 'C', 0x00 };
	UTF32 str_u32[] = { 'A', 'B', 'C', 0x00 };

	ASSERT_EQ(3, StringTraits::tcslen(str_u8));
	ASSERT_EQ(3, StringTraits::tcslen(str_u16));
	ASSERT_EQ(3, StringTraits::tcslen(str_u32));
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, ToUpper)
{
	{
		UTF8 ch_u8 = 'a';
		UTF16 ch_u16 = 'a';
		UTF32 ch_u32 = 'a';

		ASSERT_EQ(0x41, StringTraits::ToUpper(ch_u8));
		ASSERT_EQ(0x41, StringTraits::ToUpper(ch_u16));
		ASSERT_EQ(0x41, StringTraits::ToUpper(ch_u32));
	}
	{
		UTF8 ch_u8 = '+';
		UTF16 ch_u16 = '+';
		UTF32 ch_u32 = '+';

		ASSERT_EQ(0x2B, StringTraits::ToUpper(ch_u8));
		ASSERT_EQ(0x2B, StringTraits::ToUpper(ch_u16));
		ASSERT_EQ(0x2B, StringTraits::ToUpper(ch_u32));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, IsSpace)
{
	{
		UTF8 str_u8[] = { '\t', '\n', '\v', '\f', '\r', ' ' };
		UTF16 str_u16[] = { '\t', '\n', '\v', '\f', '\r', ' ' };
		UTF32 str_u32[] = { '\t', '\n', '\v', '\f', '\r', ' ' };
		for (int i = 0; i < LN_ARRAY_SIZE_OF(str_u8); i++)
		{
			ASSERT_TRUE(StringTraits::IsSpace(str_u8[i]));
			ASSERT_TRUE(StringTraits::IsSpace(str_u16[i]));
			ASSERT_TRUE(StringTraits::IsSpace(str_u32[i]));
		}
	}
	{
		UTF8 str_u8[] = { 'A' };
		UTF16 str_u16[] = { 'A' };
		UTF32 str_u32[] = { 'A' };
		for (int i = 0; i < LN_ARRAY_SIZE_OF(str_u8); i++)
		{
			ASSERT_FALSE(StringTraits::IsSpace(str_u8[i]));
			ASSERT_FALSE(StringTraits::IsSpace(str_u16[i]));
			ASSERT_FALSE(StringTraits::IsSpace(str_u32[i]));
		}
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, compare)
{
	// <Test> 文字数指定無しの全体比較
	{
		ASSERT_EQ(true, StringTraits::compare("aa", -1, "aaa", -1, 2) == 0);
		ASSERT_EQ(true, StringTraits::compare("aaa", -1, "aa", -1, 2) == 0);
		ASSERT_EQ(true, StringTraits::compare("aa", -1, "aaa", -1, 3) < 0);
		ASSERT_EQ(true, StringTraits::compare("aaa", -1, "aa", -1, 3) > 0);
	}
	// <Test> 文字数指定有りの全体比較
	{
		ASSERT_EQ(true, StringTraits::compare("aa", 2, "aaa", 3, 2) == 0);
		ASSERT_EQ(true, StringTraits::compare("aaa", 3, "aa", 2, 2) == 0);
		ASSERT_EQ(true, StringTraits::compare("aa", 2, "aaa", 3, 3) < 0);
		ASSERT_EQ(true, StringTraits::compare("aaa", 3, "aa", 2, 3) > 0);
	}
	// <Test> 文字数指定無し・比較数指定無し
	{
		ASSERT_EQ(true, StringTraits::compare("aaa", -1, "aaa", -1, -1) == 0);
		ASSERT_EQ(true, StringTraits::compare("aa", -1, "aaa", -1, -1) < 0);
		ASSERT_EQ(true, StringTraits::compare("aaa", -1, "aa", -1, -1) > 0);
	}
	// <Test> 文字数指定有りの比較数指定
	{
		ASSERT_EQ(true, StringTraits::compare("aa", 2, "aaa", 3, -1) < 0);
		ASSERT_EQ(true, StringTraits::compare("aaa", 3, "aa", 2, -1) > 0);
	}
	// <Test> 部分比較
	{
		ASSERT_EQ(true, StringTraits::compare("aaa", 3, "aaa", 3, 4) == 0);
		ASSERT_EQ(true, StringTraits::compare("aaa", 2, "aaa", 3, 4) < 0);
		ASSERT_EQ(true, StringTraits::compare("aaa", 3, "aaa", 2, 4) > 0);
	}
	// <Test> デフォルト引数では大文字小文字を区別する
	{
		ASSERT_EQ(true, StringTraits::compare("aaa", -1, "aaa", -1, -1) == 0);
		ASSERT_EQ(true, StringTraits::compare("aaa", -1, "AAA", -1, -1) != 0);
	}
	// <Test> 大文字小文字を区別する
	{
		ASSERT_EQ(true, StringTraits::compare("aaa", -1, "aaa", -1, -1, CaseSensitivity::CaseSensitive) == 0);
		ASSERT_EQ(true, StringTraits::compare("aaa", -1, "AAA", -1, -1, CaseSensitivity::CaseSensitive) != 0);
	}
	// <Test> 大文字小文字を区別しない
	{
		ASSERT_EQ(true, StringTraits::compare("aaa", -1, "aaa", -1, -1, CaseSensitivity::CaseInsensitive) == 0);
		ASSERT_EQ(true, StringTraits::compare("aaa", -1, "AAA", -1, -1, CaseSensitivity::CaseInsensitive) == 0);
	}

	// <Test> 空文字列の比較
	{
		ASSERT_EQ(true, StringTraits::compare("", -1, "", -1, 0) == 0);
		ASSERT_EQ(true, StringTraits::compare("", 0, "", 0, 0) == 0);
		ASSERT_EQ(true, StringTraits::compare("", -1, "a", -1, 0) == 0);
		ASSERT_EQ(true, StringTraits::compare("a", -1, "", -1, 0) == 0);
		ASSERT_EQ(true, StringTraits::compare("", 0, "a", 1, 0) == 0);
		ASSERT_EQ(true, StringTraits::compare("a", 1, "", 0, 0) == 0);

		ASSERT_EQ(true, StringTraits::compare("", -1, "a", -1, 1) < 0);
		ASSERT_EQ(true, StringTraits::compare("a", -1, "", -1, 1) > 0);
		ASSERT_EQ(true, StringTraits::compare("", 0, "a", 1, 1) < 0);
		ASSERT_EQ(true, StringTraits::compare("a", 1, "", 0, 1) > 0);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, EndsWith)
{
	// 普通に比較
	{
		const TCHAR* str1 = _T("abc def");
		const TCHAR* str2 = _T("def");
		ASSERT_EQ(true, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseSensitive));
	}
	// 完全一致
	{
		const TCHAR* str1 = _T("def");
		const TCHAR* str2 = _T("def");
		ASSERT_EQ(true, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseSensitive));
	}
	// 不一致 (後ろ方向)
	{
		const TCHAR* str1 = _T("def");
		const TCHAR* str2 = _T("de");
		ASSERT_EQ(false, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseSensitive));
	}
	// 一致 (前方向方向)
	{
		const TCHAR* str1 = _T("def");
		const TCHAR* str2 = _T("ef");
		ASSERT_EQ(true, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseSensitive));
	}
	// 不一致 (str2が長い)
	{
		const TCHAR* str1 = _T("def");
		const TCHAR* str2 = _T("defghi");
		ASSERT_EQ(false, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseSensitive));
	}
	// 空文字どうし
	{
		const TCHAR* str1 = _T("");
		const TCHAR* str2 = _T("");
		ASSERT_EQ(true, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseSensitive));
	}
	// 空文字
	{
		const TCHAR* str1 = _T("a");
		const TCHAR* str2 = _T("");
		ASSERT_EQ(true, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseSensitive));
	}
	// 空文字
	{
		const TCHAR* str1 = _T("");
		const TCHAR* str2 = _T("a");
		ASSERT_EQ(false, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseSensitive));
	}

	// 普通に比較 (IgnoreCase)
	{
		const TCHAR* str1 = _T("abc def");
		const TCHAR* str2 = _T("DeF");
		ASSERT_EQ(true, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseInsensitive));
	}
	// 完全一致 (IgnoreCase)
	{
		const TCHAR* str1 = _T("DEF");
		const TCHAR* str2 = _T("def");
		ASSERT_EQ(true, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseInsensitive));
	}
	// 不一致 (後ろ方向) (IgnoreCase)
	{
		const TCHAR* str1 = _T("def");
		const TCHAR* str2 = _T("DE");
		ASSERT_EQ(false, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseInsensitive));
	}
	// 一致 (前方向方向) (IgnoreCase)
	{
		const TCHAR* str1 = _T("DEF");
		const TCHAR* str2 = _T("ef");
		ASSERT_EQ(true, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseInsensitive));
	}
	// 不一致 (str2が長い) (IgnoreCase)
	{
		const TCHAR* str1 = _T("def");
		const TCHAR* str2 = _T("defGHI");
		ASSERT_EQ(false, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseInsensitive));
	}
	// 空文字どうし (IgnoreCase)
	{
		const TCHAR* str1 = _T("");
		const TCHAR* str2 = _T("");
		ASSERT_EQ(true, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseInsensitive));
	}
	// 空文字 (IgnoreCase)
	{
		const TCHAR* str1 = _T("A");
		const TCHAR* str2 = _T("");
		ASSERT_EQ(true, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseInsensitive));
	}
	// 空文字 (IgnoreCase)
	{
		const TCHAR* str1 = _T("");
		const TCHAR* str2 = _T("A");
		ASSERT_EQ(false, StringTraits::EndsWith(str1, -1, str2, -1, CaseSensitivity::CaseInsensitive));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, CountString)
{
	ASSERT_EQ(1, StringTraits::CountString(_T("abc"), -1, _T("ab"), -1));
	ASSERT_EQ(2, StringTraits::CountString(_T("CaseSensitivity"), -1, _T("it"), -1));
	ASSERT_EQ(0, StringTraits::CountString(_T("abc"), -1, _T("abcd"), -1));
	ASSERT_EQ(2, StringTraits::CountString(_T("*****"), -1, _T("**"), -1));
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, ToInt8)
{
	int8_t n;
	const TCHAR* end;
	NumberConversionResult r;

	// Min Overflow
	n = StringTraits::ToInt8(_T("-129"), -1, 0, &end, &r);
	ASSERT_EQ(-128, n);
	ASSERT_EQ(NumberConversionResult::Overflow, r);

	// Min Success
	n = StringTraits::ToInt8(_T("-128"), -1, 0, &end, &r);
	ASSERT_EQ(-128, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Success
	n = StringTraits::ToInt8(_T("127"), -1, 0, &end, &r);
	ASSERT_EQ(127, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Overflow
	n = StringTraits::ToInt8(_T("128"), -1, 0, &end, &r);
	ASSERT_EQ(127, n);
	ASSERT_EQ(NumberConversionResult::Overflow, r);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, ToUInt8)
{
	uint8_t n;
	const TCHAR* end;
	NumberConversionResult r;

	// Neg Success
	n = StringTraits::ToUInt8(_T("-1"), -1, 0, &end, &r);
	ASSERT_EQ(0xFF, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Min Success
	n = StringTraits::ToUInt8(_T("0"), -1, 0, &end, &r);
	ASSERT_EQ(0, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Success
	n = StringTraits::ToUInt8(_T("255"), -1, 0, &end, &r);
	ASSERT_EQ(255, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Overflow
	n = StringTraits::ToUInt8(_T("256"), -1, 0, &end, &r);
	ASSERT_EQ(255, n);
	ASSERT_EQ(NumberConversionResult::Overflow, r);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, ToInt16)
{
	int16_t n;
	const TCHAR* end;
	NumberConversionResult r;

	// Min Overflow
	n = StringTraits::ToInt16(_T("-32769"), -1, 0, &end, &r);
	ASSERT_EQ(-32768, n);
	ASSERT_EQ(NumberConversionResult::Overflow, r);

	// Min Success
	n = StringTraits::ToInt16(_T("-32768"), -1, 0, &end, &r);
	ASSERT_EQ(-32768, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Success
	n = StringTraits::ToInt16(_T("32767"), -1, 0, &end, &r);
	ASSERT_EQ(32767, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Overflow
	n = StringTraits::ToInt16(_T("32768"), -1, 0, &end, &r);
	ASSERT_EQ(32767, n);
	ASSERT_EQ(NumberConversionResult::Overflow, r);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, ToUInt16)
{
	uint16_t n;
	const TCHAR* end;
	NumberConversionResult r;

	// Neg Success
	n = StringTraits::ToUInt16(_T("-1"), -1, 0, &end, &r);
	ASSERT_EQ(0xFFFF, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Min Success
	n = StringTraits::ToUInt16(_T("0"), -1, 0, &end, &r);
	ASSERT_EQ(0, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Success
	n = StringTraits::ToUInt16(_T("65535"), -1, 0, &end, &r);
	ASSERT_EQ(65535, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Overflow
	n = StringTraits::ToUInt16(_T("65536"), -1, 0, &end, &r);
	ASSERT_EQ(65535, n);
	ASSERT_EQ(NumberConversionResult::Overflow, r);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, ToInt32)
{
	int32_t n;
	const TCHAR* end;
	NumberConversionResult r;

	// Min Overflow
	n = StringTraits::ToInt32(_T("-2147483649"), -1, 0, &end, &r);
	ASSERT_EQ(INT32_MIN, n);
	ASSERT_EQ(NumberConversionResult::Overflow, r);

	// Min Success
	n = StringTraits::ToInt32(_T("-2147483648"), -1, 0, &end, &r);
	ASSERT_EQ(INT32_MIN, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Success
	n = StringTraits::ToInt32(_T("2147483647"), -1, 0, &end, &r);
	ASSERT_EQ(INT32_MAX, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Overflow
	n = StringTraits::ToInt32(_T("2147483648"), -1, 0, &end, &r);
	ASSERT_EQ(INT32_MAX, n);
	ASSERT_EQ(NumberConversionResult::Overflow, r);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, ToUInt32)
{
	uint32_t n;
	const TCHAR* end;
	NumberConversionResult r;

	// Neg Success
	n = StringTraits::ToUInt32(_T("-1"), -1, 0, &end, &r);
	ASSERT_EQ(UINT32_MAX, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Min Success
	n = StringTraits::ToUInt32(_T("0"), -1, 0, &end, &r);
	ASSERT_EQ(0, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Success
	n = StringTraits::ToUInt32(_T("4294967295"), -1, 0, &end, &r);
	ASSERT_EQ(UINT32_MAX, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Overflow
	n = StringTraits::ToUInt32(_T("4294967296"), -1, 0, &end, &r);
	ASSERT_EQ(UINT32_MAX, n);
	ASSERT_EQ(NumberConversionResult::Overflow, r);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, ToInt64)
{
	int64_t n;
	const TCHAR* end;
	NumberConversionResult r;

	// Min Overflow
	n = StringTraits::ToInt64(_T("-9223372036854775809"), -1, 0, &end, &r);
	ASSERT_EQ(INT64_MIN, n);
	ASSERT_EQ(NumberConversionResult::Overflow, r);

	// Min Success
	n = StringTraits::ToInt64(_T("-9223372036854775808"), -1, 0, &end, &r);
	ASSERT_EQ(INT64_MIN, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Success
	n = StringTraits::ToInt64(_T("9223372036854775807"), -1, 0, &end, &r);
	ASSERT_EQ(INT64_MAX, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Overflow
	n = StringTraits::ToInt64(_T("9223372036854775808"), -1, 0, &end, &r);
	ASSERT_EQ(INT64_MAX, n);
	ASSERT_EQ(NumberConversionResult::Overflow, r);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, ToUInt64)
{
	uint64_t n;
	const TCHAR* end;
	NumberConversionResult r;

	// Neg Success
	n = StringTraits::ToUInt64(_T("-1"), -1, 0, &end, &r);
	ASSERT_EQ(UINT64_MAX, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Min Success
	n = StringTraits::ToUInt64(_T("0"), -1, 0, &end, &r);
	ASSERT_EQ(0, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Success
	n = StringTraits::ToUInt64(_T("0xffffffffffffffff"), -1, 0, &end, &r);
	ASSERT_EQ(UINT64_MAX, n);
	ASSERT_EQ(NumberConversionResult::Success, r);

	// Max Overflow
	n = StringTraits::ToUInt64(_T("0xfffffffffffffffff"), -1, 0, &end, &r);
	ASSERT_EQ(UINT64_MAX, n);
	ASSERT_EQ(NumberConversionResult::Overflow, r);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, ToUInt)
{
	const TCHAR* str;
	const TCHAR* end;
	NumberConversionResult r;

	// 基数指定で 2 進数
	ASSERT_EQ(123, StringTraits::ToInt32(_T("1111011"), -1, 2));

	// 基数指定で 10 進数
	ASSERT_EQ(123, StringTraits::ToInt32(_T("123"), -1, 10));

	// 8進数として自動判別
	ASSERT_EQ(8, StringTraits::ToInt32(_T("010")));

	// 基数指定で 8 進数
	ASSERT_EQ(8, StringTraits::ToInt32(_T("010"), -1, 8, &end, &r));

	// 16進数として自動判別
	ASSERT_EQ(16, StringTraits::ToInt32(_T("0x10")));

	// 基数指定で 16進数
	ASSERT_EQ(16, StringTraits::ToInt32(_T("0x10"), -1, 16, &end, &r));

	// 16進数大文字
	ASSERT_EQ(0xABCDEF, StringTraits::ToInt32(_T("0xABCDEF")));

	// 16進数小文字
	ASSERT_EQ(0xABCDEF, StringTraits::ToInt32(_T("0xabcdef")));

	// 符号
	ASSERT_EQ(-10, StringTraits::ToInt32(_T("-10")));
	ASSERT_EQ(10, StringTraits::ToInt32(_T("+10")));

	// 前方に変な文字
	str = _T("R1");
	StringTraits::ToInt32(str, -1, 0, &end, &r);
	ASSERT_EQ(NumberConversionResult::Success, r);
	ASSERT_TRUE(str == end);

	// 後方に変な文字
	str = _T("1R");
	StringTraits::ToInt32(str, -1, 0, &end, &r);
	ASSERT_EQ(NumberConversionResult::Success, r);
	ASSERT_TRUE(str + 1 == end);

	// 文字数指定
	ASSERT_EQ(12, StringTraits::ToInt32(_T("123"), 2));

	/*
	https://msdn.microsoft.com/ja-jp/library/dwhawy9k(v=vs.110).aspx#FFormatString
	//double dd = 123.12345678901234;     // -> 123.123456789012
	//double dd = 5123.12345678901234;    // -> 5123.12345678901
	// double dd = 12.005;    // -> 12.005
	//double dd = 1.005;    // -> 1.005
	//double dd = 0.005;    // -> 0.005
	//double dd = .0000023;   // ->2.3E-06
	//double dd = 0.000023;   // ->2.3E-05
	// double dd = 0.00023;   // ->0.00023
	//double dd = 0.000235;   // ->0.000235
	double dd = 5.0000023;   // ->5.0000023
	var s = dd.ToString();
	*/
	//printf("%f", 5.000000001);
	//_itoa(10, );

	//const char* str2 = "1.5";
	//char* end2;
	//double d = strtod(str2, &end2);

	//printf("%f", 5.000000001);
	//DBL_MANT_DIG;
	//DBL_MAX_EXP;
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringUtils, ToDouble)
{
	const TCHAR* str;
	const TCHAR* end;
	NumberConversionResult r;

	// 普通に
	str = _T("1.52");
	ASSERT_DOUBLE_EQ(1.52, StringTraits::ToDouble(str, -1, &end, &r));
	ASSERT_EQ(NumberConversionResult::Success, r);
	ASSERT_TRUE(str + 4 == end);

	// 文字数指定
	str = _T("1.52");
	ASSERT_DOUBLE_EQ(1.5, StringTraits::ToDouble(str, 3, &end, &r));
	ASSERT_EQ(NumberConversionResult::Success, r);
	ASSERT_TRUE(str + 3 == end);

	// オーバーフロー
	str = _T("1.52E+999");
	StringTraits::ToDouble(str, -1, &end, &r);
	ASSERT_EQ(NumberConversionResult::Overflow, r);

	// オーバーフロー
	str = _T("1.52E-999");
	StringTraits::ToDouble(str, -1, &end, &r);
	ASSERT_EQ(NumberConversionResult::Overflow, r);
}

