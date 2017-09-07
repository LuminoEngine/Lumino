#include <TestConfig.h>

class Test_Base_StringArray : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringArray, Constructor)
{
	//// <Test> TODO: 初期化子リスト
	//{
	//	StringArray ary = { _LT("a"), _LT("b") };
	//	ASSERT_EQ(2, ary.getCount());
	//	ASSERT_EQ(_LT("a"), ary[0]);
	//	ASSERT_EQ(_LT("b"), ary[1]);
	//}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringArray, Unit_Contains)
{
	StringArray ary;
	ASSERT_EQ(false, ary.contains((const Char*)NULL));
	ASSERT_EQ(false, ary.contains(_LT("")));

	ary.add(_LT("a"));
	ASSERT_EQ(false, ary.contains(_LT("")));
	ASSERT_EQ(false, ary.contains(String()));
	ASSERT_EQ(true, ary.contains(_LT("a")));
	ASSERT_EQ(true, ary.contains(String("a")));

	ary.add(_LT(" "));
	ASSERT_EQ(true, ary.contains(String("a")));
	ASSERT_EQ(true, ary.contains(String(" ")));
}
