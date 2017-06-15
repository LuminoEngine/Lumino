#include <TestConfig.h>
#include <Lumino/Threading/Thread.h>

class Test_Threading_EventFlag : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

int gValue;

//---------------------------------------------------------------------
TEST_F(Test_Threading_EventFlag, Basic)
{
	struct TestThread : public Thread
	{
	public:
		TestThread()
			: mInited(false)
			, mRunning(false)
		{
		}

		virtual void execute()
		{
			gValue++;
			mInited.setTrue();

			do
			{
				gValue++;
				Thread::sleep(10);
			} while (mRunning.isTrue());
		}

		ConditionFlag mInited;
		ConditionFlag mRunning;
	};

	gValue = 0;

	TestThread t;
	t.execute();

	t.mInited.wait();	// 初期化完了待ち
	ASSERT_TRUE(gValue >= 2);


	t.mRunning.setFalse();	// 終了要求
	t.wait();
	ASSERT_TRUE(gValue >= 2);
}

//---------------------------------------------------------------------
TEST_F(Test_Threading_EventFlag, Basic2)
{
	ConditionFlag f1;
	ConditionFlag f2(true);
	ConditionFlag f3(false);
	ASSERT_FALSE(f1.isTrue());
	ASSERT_TRUE(f2.isTrue());
	ASSERT_FALSE(f3.isTrue());

	f2.wait();	// true なので止まらずに進む
}
