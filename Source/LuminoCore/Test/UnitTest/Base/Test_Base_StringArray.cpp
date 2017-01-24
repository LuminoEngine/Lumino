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
		ASSERT_EQ(2, ary.GetCount());
		ASSERT_EQ(_T("a"), ary[0]);
		ASSERT_EQ(_T("b"), ary[1]);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringArray, Unit_Contains)
{
	StringArray ary;
	ASSERT_EQ(false, ary.Contains((const TCHAR*)NULL));
	ASSERT_EQ(false, ary.Contains(_T("")));

	ary.Add(_T("a"));
	ASSERT_EQ(false, ary.Contains(_T("")));
	ASSERT_EQ(false, ary.Contains(String()));
	ASSERT_EQ(true, ary.Contains(_T("a")));
	ASSERT_EQ(true, ary.Contains(String("a")));

	ary.Add(_T(" "));
	ASSERT_EQ(true, ary.Contains(String("a")));
	ASSERT_EQ(true, ary.Contains(String(" ")));
}
