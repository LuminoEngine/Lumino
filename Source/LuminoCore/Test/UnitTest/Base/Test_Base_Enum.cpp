#include <TestConfig.h>

class Test_Base_Enum : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

LN_ENUM(TestValues0)
{
	Value1 = 100,
	Value2,
	Value3,
};
LN_ENUM_DECLARE(TestValues0);

LN_ENUM(TestValues)
{
	ID1 = 0,
	ID2,
	ID3 = TestValues0::Value3,
};
LN_ENUM_REFLECTION(TestValues, ID1, ID2, ID3);
LN_ENUM_DECLARE(TestValues);

LN_ENUM_FLAGS(TestFlags)
{
	Option0 = 0x00,
	Option1 = 0x01,
	Option2 = 0x02,
	Option3 = 0x04,
};
LN_ENUM_FLAGS_REFLECTION(TestFlags, Option0, Option1, Option2, Option3);
LN_ENUM_FLAGS_DECLARE(TestFlags);

//------------------------------------------------------------------------------
TEST_F(Test_Base_Enum, Basic)
{
	TestValues v1 = TestValues::ID2;
	//TestValues v0 = 10;		// error

	TestFlags flags1 = TestFlags::Option1 | TestFlags::Option2;
	//TestFlags flags0 = 0x01;	// error
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Enum, ValuesOperators)
{
	// <Test> 初期化できる。
	{
		TestValues v1 = TestValues::ID1;
		TestValues v2(TestValues::ID1);
		ASSERT_EQ(TestValues::ID1, v1);
		ASSERT_EQ(TestValues::ID1, v2);
	}
	// <Test> operator =
	{
		TestValues v1;
		v1 = TestValues::ID1;
		ASSERT_EQ(TestValues::ID1, v1);
	}
	// <Test> operator ==
	{
		TestValues v1 = TestValues::ID1;
		TestValues v2 = TestValues::ID2;
		ASSERT_TRUE(v1 == TestValues::ID1);
		ASSERT_FALSE(v1 == TestValues::ID2);
		ASSERT_TRUE(TestValues::ID1 == v1);
		ASSERT_FALSE(TestValues::ID2 == v1);
		ASSERT_TRUE(v1 == v1);
		ASSERT_FALSE(v2 == v1);
		ASSERT_TRUE(v1 == 0);	// int と比較できる
	}
	// <Test> operator !=
	{
		TestValues v1 = TestValues::ID1;
		TestValues v2 = TestValues::ID2;
		ASSERT_FALSE(v1 != TestValues::ID1);
		ASSERT_TRUE(v1 != TestValues::ID2);
		ASSERT_FALSE(TestValues::ID1 != v1);
		ASSERT_TRUE(TestValues::ID2 != v1);
		ASSERT_FALSE(v1 != v1);
		ASSERT_TRUE(v2 != v1);
		ASSERT_FALSE(v1 != 0);	// int と比較できる
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Enum, toString)
{
	// <Test> enum 値を文字列に変換できる。
	{
		TestValues v1 = TestValues::ID2;
		ASSERT_EQ(_T("ID2"), v1.toString());

		v1 = TestValues::ID1;
		ASSERT_EQ(_T("ID1"), v1.toString());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Enum, parse)
{
	// <Test> 文字列を enum 値に変換できる。
	{
		TestValues v1 = TestValues::parse(_TT("ID2"));
		ASSERT_EQ(TestValues::ID2, v1);
	}
	// <Test> 別の enum の値でメンバの値を設定できる。
	{
		TestValues v1 = TestValues::parse(_TT("ID3"));
		ASSERT_EQ(102, v1);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Enum, FlagsOperators)
{
	// <Test> 初期化できる。
	{
		TestFlags v1 = TestFlags::Option1;
		TestFlags v2(TestFlags::Option2);
		ASSERT_EQ(TestFlags::Option1, v1);
		ASSERT_EQ(TestFlags::Option2, v2);
	}
	// <Test> operator =
	{
		TestFlags v1;
		v1 = TestFlags::Option1;
		ASSERT_EQ(TestFlags::Option1, v1);
	}
	// <Test> operator ==
	{
		TestFlags v1 = TestFlags::Option1;
		TestFlags v2 = TestFlags::Option2;
		ASSERT_TRUE(v1 == TestFlags::Option1);
		ASSERT_FALSE(v1 == TestFlags::Option2);
		ASSERT_TRUE(TestFlags::Option1 == v1);
		ASSERT_FALSE(TestFlags::Option2 == v1);
		ASSERT_TRUE(v1 == v1);
		ASSERT_FALSE(v2 == v1);
	}
	// <Test> operator !=
	{
		TestFlags v1 = TestFlags::Option1;
		TestFlags v2 = TestFlags::Option2;
		ASSERT_FALSE(v1 != TestFlags::Option1);
		ASSERT_TRUE(v1 != TestFlags::Option2);
		ASSERT_FALSE(TestFlags::Option1 != v1);
		ASSERT_TRUE(TestFlags::Option2 != v1);
		ASSERT_FALSE(v1 != v1);
		ASSERT_TRUE(v2 != v1);
	}
	// <Test> operator |
	{
		TestFlags v1;
		TestFlags v2 = TestFlags::Option2;
		v1 = TestFlags::Option1 | TestFlags::Option2;
		ASSERT_EQ(TestFlags::Option1 | TestFlags::Option2, v1);

		v1 = TestFlags::Option1;
		v1 = v1 | v2;
		ASSERT_EQ(TestFlags::Option1 | TestFlags::Option2, v1);

		// どちらの辺でもOK
		v1 = TestFlags::Option1;
		v1 = v1 | TestFlags::Option2;
		v1 = TestFlags::Option3 | v1;
		ASSERT_EQ(TestFlags::Option1 | TestFlags::Option2 | TestFlags::Option3, v1);
	}
	// <Test> operator &
	{
		TestFlags v1;
		v1 = TestFlags::Option1 & TestFlags::Option2;
		ASSERT_EQ(TestFlags::Option0, v1);

		TestFlags v2 = TestFlags::Option1;
		v1 = TestFlags::Option1;
		v1 = v1 & v2;
		ASSERT_EQ(TestFlags::Option1, v1);

		// どちらの辺でもOK
		v1 = TestFlags::Option1 | TestFlags::Option2 | TestFlags::Option3;
		v1 = v1 & TestFlags::Option2;
		v1 = TestFlags::Option3 & v1;
		ASSERT_EQ(TestFlags::Option0, v1);
	}
	// <Test> operator &=
	{
		TestFlags v1 = TestFlags::Option1 | TestFlags::Option2;
		v1 &= TestFlags::Option1;
		ASSERT_EQ(TestFlags::Option1, v1);

		TestFlags v2 = TestFlags::Option2;
		v1 &= v2;
		ASSERT_EQ(TestFlags::Option0, v1);
	}
	// <Test> operator |=
	{
		TestFlags v1;
		v1 |= TestFlags::Option1;
		ASSERT_EQ(TestFlags::Option1, v1);

		TestFlags v2 = TestFlags::Option2;
		v1 |= v2;
		ASSERT_EQ(TestFlags::Option1 | TestFlags::Option2, v1);
	}
	// <Test> &= ~ でフラグを落とす。
	{
		TestFlags v1 = TestFlags::Option1 | TestFlags::Option2;

		v1 &= ~TestFlags::Option3;
		ASSERT_EQ(TestFlags::Option1 | TestFlags::Option2, v1);	// 変化無し

		v1 &= ~TestFlags::Option2;
		ASSERT_EQ(TestFlags::Option1, v1);	// Option2 が消えている

		v1 &= ~v1;							// 自分自身で落としてみる
		ASSERT_EQ(TestFlags::Option0, v1);					// 全部消えている
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Enum, FlagsToString)
{
	// <Test> フラグ enum は | で区切って文字列化できる。
	{
		TestFlags v1 = TestFlags::Option2 | TestFlags::Option3;
		ASSERT_EQ(_T("Option2|Option3"), v1.toString());
	}
	// <Test> 0 もメンバとして存在すれば文字列化できる。
	{
		TestFlags v1 = TestFlags::Option0;
		ASSERT_EQ(_T("Option0"), v1.toString());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Enum, FlagsParse)
{
	// <Test> | で区切った文字列を enum(flags) 値に変換できる。
	{
		TestFlags v1 = TestFlags::parse(_TT("Option2|Option3"));
		ASSERT_EQ(TestFlags::Option2 | TestFlags::Option3, v1);
	}
	// <Test> 区切り文字を変更できる
	{
		TestFlags v1 = TestFlags::parse(_TT("Option1, Option3"), _TT(','));	// .NET スタイル
		ASSERT_EQ(TestFlags::Option1 | TestFlags::Option3, v1);
	}
	// <Illegal> 異常系チェック。前後や間に空白があってもOK。
	{
		TestFlags v1;
		v1 = TestFlags::parse(_TT(" Option2|Option3"));
		ASSERT_EQ(TestFlags::Option2 | TestFlags::Option3, v1);
		v1 = TestFlags::parse(_TT("Option2|Option3 "));
		ASSERT_EQ(TestFlags::Option2 | TestFlags::Option3, v1);
		v1 = TestFlags::parse(_TT("Option2\t|Option3"));
		ASSERT_EQ(TestFlags::Option2 | TestFlags::Option3, v1);
		v1 = TestFlags::parse(_TT("Option2| Option3"));
		ASSERT_EQ(TestFlags::Option2 | TestFlags::Option3, v1);
		v1 = TestFlags::parse(_TT(" Option2 | Option3 "));
		ASSERT_EQ(TestFlags::Option2 | TestFlags::Option3, v1);
	}
}




/*
	if (flags1 == 0x03) のような比較はできません。
	if (flags1 == ((TestFlags1)0x03)) のように型を合わせます。
*/


//==============================================================================
class Test_Base_EnumFlags : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

enum class TestFlags1
{
	Option0 = 0x00,
	Option1 = 0x01,
	Option2 = 0x02,
	Option3 = 0x04,
};
LN_ENUM_FLAGS_OPERATORS(TestFlags1);

//------------------------------------------------------------------------------
TEST_F(Test_Base_EnumFlags, Basic)
{
	EnumFlags<TestFlags1> flags1 = TestFlags1::Option1 | TestFlags1::Option2;
	ASSERT_EQ(((TestFlags1)0x03), flags1);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_EnumFlags, FlagsOperators)
{
	// <Test> 初期化できる。
	{
		EnumFlags<TestFlags1> v1 = TestFlags1::Option1;
		EnumFlags<TestFlags1> v2(TestFlags1::Option2);
		ASSERT_EQ(TestFlags1::Option1, v1);
		ASSERT_EQ(TestFlags1::Option2, v2);
	}
	// <Test> operator =
	{
		EnumFlags<TestFlags1> v1;
		v1 = TestFlags1::Option1;
		ASSERT_EQ(TestFlags1::Option1, v1);
	}
	// <Test> operator ==
	{
		EnumFlags<TestFlags1> v1 = TestFlags1::Option1;
		EnumFlags<TestFlags1> v2 = TestFlags1::Option2;
		ASSERT_TRUE(v1 == TestFlags1::Option1);
		ASSERT_FALSE(v1 == TestFlags1::Option2);
		//ASSERT_TRUE(TestFlags1::Option1 == v1);	// TODO: Not Implemented
		//ASSERT_FALSE(TestFlags1::Option2 == v1);	// TODO: Not Implemented
		ASSERT_TRUE(v1 == v1);
		ASSERT_FALSE(v2 == v1);
	}
	// <Test> operator !=
	{
		EnumFlags<TestFlags1> v1 = TestFlags1::Option1;
		EnumFlags<TestFlags1> v2 = TestFlags1::Option2;
		ASSERT_FALSE(v1 != TestFlags1::Option1);
		ASSERT_TRUE(v1 != TestFlags1::Option2);
		//ASSERT_FALSE(TestFlags1::Option1 != v1);	// TODO: Not Implemented
		//ASSERT_TRUE(TestFlags1::Option2 != v1);	// TODO: Not Implemented
		ASSERT_FALSE(v1 != v1);
		ASSERT_TRUE(v2 != v1);
	}
	// <Test> operator |
	{
		EnumFlags<TestFlags1> v1;
		EnumFlags<TestFlags1> v2 = TestFlags1::Option2;
		v1 = TestFlags1::Option1 | TestFlags1::Option2;
		ASSERT_EQ(TestFlags1::Option1 | TestFlags1::Option2, v1);

		v1 = TestFlags1::Option1;
		v1 = v1 | v2;
		ASSERT_EQ(TestFlags1::Option1 | TestFlags1::Option2, v1);

		// どちらの辺でもOK
		v1 = TestFlags1::Option1;
		v1 = v1 | TestFlags1::Option2;
		//v1 = TestFlags1::Option3 | v1;	// TODO: Not Implemented
		ASSERT_EQ(TestFlags1::Option1 | TestFlags1::Option2/* | TestFlags1::Option3*/, v1);
	}
	// <Test> operator &
	{
		EnumFlags<TestFlags1> v1;
		v1 = TestFlags1::Option1 & TestFlags1::Option2;
		ASSERT_EQ(TestFlags1::Option0, v1);

		EnumFlags<TestFlags1> v2 = TestFlags1::Option1;
		v1 = TestFlags1::Option1;
		v1 = v1 & v2;
		ASSERT_EQ(TestFlags1::Option1, v1);

		// どちらの辺でもOK
		v1 = TestFlags1::Option1 | TestFlags1::Option2 | TestFlags1::Option3;
		v1 = v1 & TestFlags1::Option2;
		v1 = TestFlags1::Option3 & v1;
		ASSERT_EQ(TestFlags1::Option0, v1);
	}
	// <Test> operator &=
	{
		EnumFlags<TestFlags1> v1 = TestFlags1::Option1 | TestFlags1::Option2;
		v1 &= TestFlags1::Option1;
		ASSERT_EQ(TestFlags1::Option1, v1);

		EnumFlags<TestFlags1> v2 = TestFlags1::Option2;
		v1 &= v2;
		ASSERT_EQ(TestFlags1::Option0, v1);
	}
	// <Test> operator |=
	{
		EnumFlags<TestFlags1> v1;
		v1 |= TestFlags1::Option1;
		ASSERT_EQ(TestFlags1::Option1, v1);

		EnumFlags<TestFlags1> v2 = TestFlags1::Option2;
		v1 |= v2;
		ASSERT_EQ(TestFlags1::Option1 | TestFlags1::Option2, v1);
	}
	// <Test> &= ~ でフラグを落とす。
	{
		EnumFlags<TestFlags1> v1 = TestFlags1::Option1 | TestFlags1::Option2;

		v1 &= ~TestFlags1::Option3;
		ASSERT_EQ(TestFlags1::Option1 | TestFlags1::Option2, v1);	// 変化無し

		v1 &= ~TestFlags1::Option2;
		ASSERT_EQ(TestFlags1::Option1, v1);	// Option2 が消えている

		v1 &= ~v1;							// 自分自身で落としてみる
		ASSERT_EQ(TestFlags1::Option0, v1);	// 全部消えている
	}
}
