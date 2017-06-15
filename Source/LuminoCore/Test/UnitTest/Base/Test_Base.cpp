#include <vector>
#include <deque>
#include <list>
#include <random>
#include <TestConfig.h>
#include <Lumino/Base/ElapsedTimer.h>
#include <Lumino/Threading/Thread.h>

#ifdef LN_MEASURE_PREF
//==============================================================================
class Test_Measurement : public ::testing::Test
{
};

//------------------------------------------------------------------------------
TEST_F(Test_Measurement, Basic)
{
	static const int DATA_COUNT = 10000;
	std::random_device rd;
	std::mt19937 mt(rd());

	std::vector<intptr_t> vec;
	std::list<intptr_t> lis;
	std::deque<intptr_t> deq;

	//--------------------------------------------------------------------------
	std::cout << "push_back:" << '\n';
	{
		std::cout << "  vector:" << '\n';
		ElapsedTimer t;
		t.start();
		for (int i = 0; i < DATA_COUNT; ++i)
		{
			vec.push_back(mt());
		}
		std::cout << "    " << t.GetElapsed() << '\n';
	}
	{
		std::cout << "  list:" << '\n';
		ElapsedTimer t;
		t.start();
		for (int i = 0; i < DATA_COUNT; ++i)
		{
			lis.push_back(mt());
		}
		std::cout << "    " << t.GetElapsed() << '\n';
	}
	{
		std::cout << "  deque:" << '\n';
		ElapsedTimer t;
		t.start();
		for (int i = 0; i < DATA_COUNT; ++i)
		{
			deq.push_back(mt());
		}
		std::cout << "    " << t.GetElapsed() << '\n';
	}

	//--------------------------------------------------------------------------
	std::cout << "insert:" << '\n';
	{
		std::cout << "  vector:" << '\n';
		ElapsedTimer t;
		t.start();
		for (int i = 0; i < DATA_COUNT; ++i)
		{
			vec.insert(vec.begin() + i, mt());
		}
		std::cout << "    " << t.GetElapsed() << '\n';
	}
	{
		std::cout << "  list:" << '\n';
		ElapsedTimer t;
		t.start();
		for (int i = 0; i < DATA_COUNT; ++i)
		{
			lis.insert(lis.begin(), mt());
		}
		std::cout << "    " << t.GetElapsed() << '\n';
	}
	{
		std::cout << "  deque:" << '\n';
		ElapsedTimer t;
		t.start();
		for (int i = 0; i < DATA_COUNT; ++i)
		{
			deq.insert(deq.begin() + i, mt());
		}
		std::cout << "    " << t.GetElapsed() << '\n';
	}
	//--------------------------------------------------------------------------
	std::cout << "erase:" << '\n';
	{
		std::cout << "  vector:" << '\n';
		ElapsedTimer t;
		t.start();
		for (int i = 0; i < DATA_COUNT; ++i)
		{
			vec.erase(vec.begin() + i);
		}
		std::cout << "    " << t.GetElapsed() << '\n';
	}
	{
		std::cout << "  list:" << '\n';
		ElapsedTimer t;
		t.start();
		for (int i = 0; i < DATA_COUNT; ++i)
		{
			lis.erase(lis.begin());
		}
		std::cout << "    " << t.GetElapsed() << '\n';
	}
	{
		std::cout << "  deque:" << '\n';
		ElapsedTimer t;
		t.start();
		for (int i = 0; i < DATA_COUNT; ++i)
		{
			deq.erase(deq.begin() + i);
		}
		std::cout << "    " << t.GetElapsed() << '\n';
	}
}

#endif



//==============================================================================
class Test_Base_ElapsedTimer : public ::testing::Test
{
};

//------------------------------------------------------------------------------
TEST_F(Test_Base_ElapsedTimer, Basic)
{
	ElapsedTimer timer;
	timer.start();
	Thread::sleep(100);
	uint64_t t = timer.getElapsed();
	ASSERT_EQ(true, t >= 98 * 1000 * 1000);		// 微妙に早く終わることを考慮する
}
