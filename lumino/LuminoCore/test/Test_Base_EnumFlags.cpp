#include "Common.hpp"
#include <LuminoCore/Base/EnumFlags.hpp>

//==============================================================================
//# enum class のビットフラグのテスト
class Test_Base_EnumClassBitFlags : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

enum class TestEnum1 : ln::FlagsType
{
	V0 = 0x0000,
	V1 = 0x0001,
	V2 = 0x0002,
	V3 = 0x0004,
	V1V2 = V1 | V2,
	All = V1 | V2 | V3,
};
LN_FLAGS_OPERATORS(TestEnum1);

//------------------------------------------------------------------------------
TEST_F(Test_Base_EnumClassBitFlags, Basic)
{
	Flags<TestEnum1> flags1;
	flags1 = TestEnum1::V1 | TestEnum1::V2;
	ASSERT_EQ(TestEnum1::V1 | TestEnum1::V2, flags1);

	flags1 |= TestEnum1::V3;
	ASSERT_EQ(TestEnum1::All, flags1);

	//flags1 = 0;                // -> NG
}

//------------------------------------------------------------------------------
//## operator のテスト
TEST_F(Test_Base_EnumClassBitFlags, Operators)
{
	Flags<TestEnum1> flags1;

	//-[ ] 代入系
	{
		flags1 = TestEnum1::V1;
		flags1 &= TestEnum1::V2;
		ASSERT_EQ(TestEnum1::V0, flags1);

		flags1 = TestEnum1::V1;
		flags1 |= TestEnum1::V2;
		ASSERT_EQ(TestEnum1::V1V2, flags1);

		flags1 = TestEnum1::V1;
		flags1 ^= TestEnum1::V1V2;
		ASSERT_EQ(TestEnum1::V2, flags1);
	}

	//-[ ] 左辺 Flags<> 系
	{
		flags1 = TestEnum1::V1;
		ASSERT_EQ(true, flags1 == TestEnum1::V1);
		ASSERT_EQ(false, flags1 != TestEnum1::V1);

		flags1 = Flags<TestEnum1>(TestEnum1::V1) & TestEnum1::V1;
		ASSERT_EQ(TestEnum1::V1, flags1);

		flags1 = Flags<TestEnum1>(TestEnum1::V1) |TestEnum1::V2;
		ASSERT_EQ(TestEnum1::V1V2, flags1);

		flags1 = Flags<TestEnum1>(TestEnum1::V1) ^ TestEnum1::V1V2;
		ASSERT_EQ(TestEnum1::V2, flags1);

		flags1 = ~Flags<TestEnum1>(TestEnum1::V1);
		ASSERT_EQ(0xfffffffe, static_cast<uint32_t>(flags1));
	}

	//-[ ] 左辺 enum class 系
	{
		flags1 = TestEnum1::V1;
		ASSERT_EQ(true, TestEnum1::V1 == Flags<TestEnum1>(TestEnum1::V1));
		ASSERT_EQ(false, TestEnum1::V1 != Flags<TestEnum1>(TestEnum1::V1));

		flags1 = TestEnum1::V1 & Flags<TestEnum1>(TestEnum1::V1);
		ASSERT_EQ(TestEnum1::V1, flags1);

		flags1 = TestEnum1::V1 | Flags<TestEnum1>(TestEnum1::V2);
		ASSERT_EQ(TestEnum1::V1V2, flags1);

		flags1 = TestEnum1::V1 ^ Flags<TestEnum1>(TestEnum1::V1V2);
		ASSERT_EQ(TestEnum1::V2, flags1);
	}

	//-[ ] 両辺 enum class 系
	{
		flags1 = TestEnum1::V1 & TestEnum1::V1;
		ASSERT_EQ(TestEnum1::V1, flags1);

		flags1 = TestEnum1::V1 | TestEnum1::V2;
		ASSERT_EQ(TestEnum1::V1V2, flags1);

		flags1 = TestEnum1::V1 ^ TestEnum1::V1V2;
		ASSERT_EQ(TestEnum1::V2, flags1);

		flags1 = ~TestEnum1::V1;
		ASSERT_EQ(0xfffffffe, (uint32_t)flags1);
	}
}

enum class TestEnum2 : ln::FlagsType
{
	V0 = 0x0000,
	V1 = 0x0001,
	V2 = 0x0002,
};
LN_FLAGS_OPERATORS(TestEnum2);

//------------------------------------------------------------------------------
//## メソッドによるフラグ操作のテスト
TEST_F(Test_Base_EnumClassBitFlags, SimpleAccessors)
{
	Flags<TestEnum2> flags1;

	//-[ ] hasFlag
	{
		bool b1 = (bool)flags1;
		ASSERT_EQ(false, b1);

		ASSERT_EQ(false, flags1.hasFlag(TestEnum2::V1));
		ASSERT_EQ(false, flags1.hasFlag(TestEnum2::V2));
	}

	//-[ ] set
	{
		flags1.set(TestEnum2::V1);
		ASSERT_EQ(true, flags1.hasFlag(TestEnum2::V1));
		ASSERT_EQ(false, flags1.hasFlag(TestEnum2::V2));

		flags1.set(TestEnum2::V2);
		ASSERT_EQ(true, flags1.hasFlag(TestEnum2::V1));
		ASSERT_EQ(true, flags1.hasFlag(TestEnum2::V2));

		bool b2 = static_cast<bool>(flags1);
		ASSERT_EQ(true, b2);
	}

	//-[ ] unset
	{
		flags1.unset(TestEnum2::V1);
		ASSERT_EQ(false, flags1.hasFlag(TestEnum2::V1));
		ASSERT_EQ(true, flags1.hasFlag(TestEnum2::V2));

		flags1.unset(TestEnum2::V2);
		ASSERT_EQ(false, flags1.hasFlag(TestEnum2::V1));
		ASSERT_EQ(false, flags1.hasFlag(TestEnum2::V2));
	}

}


