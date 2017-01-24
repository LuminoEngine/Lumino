#include <TestConfig.h>
#include <Lumino/Base/Uuid.h>

//==============================================================================
class Test_Base_Uuid : public ::testing::Test
{
};

//------------------------------------------------------------------------------
TEST_F(Test_Base_Uuid, Basic)
{
	Uuid empty1, empty2;
	Uuid id1(_T("{8DF0E5DD-0717-4F29-8F2F-A47DEDA55041}"));
	Uuid id2(_T("8DF0E5DD-0717-4F29-8F2F-A47DEDA55041"));
	Uuid id3(_T("{6003FC8B-E954-4E2A-931D-BC1427A7BF31}"));
	
	// <Test> 空UUIDの比較
	{
		ASSERT_EQ(empty1, empty2);
	}
	
	// <Test> {} の有無はどちらでも受け入れる
	{
		ASSERT_EQ(id1, id2);
	}
	
	// <Test> 比較
	{
		ASSERT_NE(id1, id3);
		ASSERT_NE(id3, id1);
		ASSERT_NE(id2, id3);
		ASSERT_EQ(id3, id3);
	}

	// <Test> 新しい UUID を生成する
	{
		Uuid newId1 = Uuid::Generate();
		Uuid newId2 = Uuid::Generate();
		ASSERT_NE(newId1, newId2);
		ASSERT_NE(empty1, newId1);
		ASSERT_NE(empty1, newId2);
	}

	// <Test> 文字列への変換
	{
		ASSERT_EQ(_T("{00000000-0000-0000-0000-000000000000}"), empty1.ToString());
		ASSERT_EQ(_T("{8DF0E5DD-0717-4F29-8F2F-A47DEDA55041}"), id1.ToString());
		ASSERT_EQ(_T("{8DF0E5DD-0717-4F29-8F2F-A47DEDA55041}"), id2.ToString());
		ASSERT_EQ(_T("{6003FC8B-E954-4E2A-931D-BC1427A7BF31}"), id3.ToString());
	}
}
