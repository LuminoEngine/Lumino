#include <TestConfig.h>
#include <Lumino/Threading/AtomicCounter.h>
#include <Lumino/Threading/Thread.h>
#include <atomic>

class Test_Threading_Atomic : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Threading_Atomic, Basic)
{
	//std::atomic<int> a;

	class IncrThread : public Thread
	{
	public:
		Atomic* value;
		Atomic* valueIncr;
		virtual void execute()
		{
			for (int i = 0; i < 1000; i++)
			{
				value->Increment();
				valueIncr->Increment();
			}
		}
	};

	class DecrThread : public Thread
	{
	public:
		Atomic* value;
		Atomic* valueDecr;
		virtual void execute()
		{
			for (int i = 0; i < 1000; i++)
			{
				value->Decrement();
				valueDecr->Decrement();
			}
		}
	};

	Atomic value;		// ++ と -- を繰り返す
	Atomic valueIncr;	// ++ だけ
	Atomic valueDecr;	// -- だけ
	value.set(10);			// 初期値

	// ++ と -- スレッドを 10 個ずつ走らせる
	const int ThreadCount = 10;
	IncrThread incrThreads[ThreadCount];
	DecrThread decrThreads[ThreadCount];
	for (int i = 0; i < ThreadCount; i++) 
	{
		incrThreads[i].value = &value;
		incrThreads[i].valueIncr = &valueIncr;
		incrThreads[i].start();
		decrThreads[i].value = &value;
		decrThreads[i].valueDecr = &valueDecr;
		decrThreads[i].start();
	}
	// 終了待機
	for (int i = 0; i < ThreadCount; i++)
	{
		incrThreads[i].Wait();
		decrThreads[i].Wait();
	}

	ASSERT_EQ(10, value.get());
	ASSERT_EQ(ThreadCount * 1000, valueIncr.get());
	ASSERT_EQ(-ThreadCount * 1000, valueDecr.get());
}

