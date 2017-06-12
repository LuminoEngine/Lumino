#include <TestConfig.h>
#include <Lumino/Threading/Thread.h>
#include <Lumino/Threading/Task.h>
#include <Lumino/Threading/TaskScheduler.h>
using namespace ln::tr;

class IntegrateTest_Threading_Task : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

static volatile int g_value = 0;

//---------------------------------------------------------------------
TEST_F(IntegrateTest_Threading_Task, Basic)
{
	g_value = 0;
	ConditionFlag flag(false);
	auto func = [&flag](){ g_value++; flag.Wait(); };

	int level = TaskScheduler::getDefault()->GetMaxConcurrencyLevel();

	// 同時実行可能な数+1だけ Task を作る
	List<TaskPtr> tasks;
	for (int i = 0; i < level + 1; ++i)
	{
		tasks.add(Task::Run(Delegate<void()>(func)));
	}

	// 実行されているのは TaskScheduler::GetDefault()->GetMaxConcurrencyLevel() だけ
	//Threading::Thread::Sleep(200);
	while (g_value != level);
	ASSERT_EQ(level, g_value);
	ASSERT_EQ(TaskStatus::Running, tasks[0]->GetStatus());	// まだ lag.Wait(); で待機している

	flag.SetTrue();
	//Threading::Thread::Sleep(200);
	while (g_value != level + 1);
	ASSERT_EQ(level + 1, g_value);
	Thread::Sleep(200);
	for (int i = 0; i < level + 1; ++i)
	{
		ASSERT_EQ(true, tasks[i]->IsCompleted());
	}
}

//---------------------------------------------------------------------
TEST_F(IntegrateTest_Threading_Task, Basic2)
{
	for (int i = 0; i < 10; i++)
	{
		List<int> ary;
		int sum;
		auto funcAdd = [&ary]() { ary.add(1); };
		auto funcItr = [&ary, &sum]() { for (int v : ary) { sum += v; } };

		auto task1 = Task::create(Delegate<void()>(funcAdd));
		auto task2 = Task::create(Delegate<void()>(funcItr));

		for (int j = 0; j < 1000; j++)
		{
			sum = 0;
			task1->start();
			task1->Wait();
			task2->start();
			task2->Wait();
			ASSERT_EQ((j + 1), sum);
		}
	}

}
