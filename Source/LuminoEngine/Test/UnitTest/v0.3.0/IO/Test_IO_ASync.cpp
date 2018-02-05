#include <TestConfig.h>

class Test_IO_ASync : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

static int g_count = 0;

class ASyncTest1
	: public ASyncIOObject
{
public:
	virtual void onASyncIOProc()
	{
		++g_count;
	}
};

//------------------------------------------------------------------------------
TEST_F(Test_IO_ASync, Basic)
{
#if 0
	g_count = 0;
	ASyncTest1 test1;
	FileManager::GetInstance().RequestASyncTask(&test1);
	FileManager::GetInstance().WaitForAllASyncTask();

	ASSERT_EQ(ASyncIOState_Completed, test1.GetASyncIOState());
	ASSERT_EQ(NULL, test1.GetASyncIOException());
	ASSERT_EQ(1, g_count);
#endif
}
