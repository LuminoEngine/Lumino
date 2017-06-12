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
	// <Test> 初期化子リスト
	{
		StringArray ary = { _T("a"), _T("b") };
		ASSERT_EQ(2, ary.getCount());
		ASSERT_EQ(_T("a"), ary[0]);
		ASSERT_EQ(_T("b"), ary[1]);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringArray, Unit_Contains)
{
	StringArray ary;
	ASSERT_EQ(false, ary.contains((const TCHAR*)NULL));
	ASSERT_EQ(false, ary.contains(_T("")));

	ary.add(_T("a"));
	ASSERT_EQ(false, ary.contains(_T("")));
	ASSERT_EQ(false, ary.contains(String()));
	ASSERT_EQ(true, ary.contains(_T("a")));
	ASSERT_EQ(true, ary.contains(String("a")));

	ary.add(_T(" "));
	ASSERT_EQ(true, ary.contains(String("a")));
	ASSERT_EQ(true, ary.contains(String(" ")));
}
