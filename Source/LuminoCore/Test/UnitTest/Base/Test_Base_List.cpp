#include <TestConfig.h>
#include <Lumino/Base/SharedList.h>







class Test_Base_List : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST_F(Test_Base_List, CopyFramework)
{
	// <Test> copy
	{
		SharedList<int> a1 = { 1, 2, 3 };
		SharedList<int> a2 = a1;
		SharedList<int> a3;
		a3 = a1;
		ASSERT_EQ(3, a1.size());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(2, a1[1]);
		ASSERT_EQ(3, a1[2]);
		ASSERT_EQ(3, a2.size());
		ASSERT_EQ(1, a2[0]);
		ASSERT_EQ(2, a2[1]);
		ASSERT_EQ(3, a2[2]);
		ASSERT_EQ(3, a3.size());
		ASSERT_EQ(1, a3[0]);
		ASSERT_EQ(2, a3[1]);
		ASSERT_EQ(3, a3[2]);

		// 共有なので全部影響する
		a1[1] = 100;
		ASSERT_EQ(100, a1[1]);
		ASSERT_EQ(100, a2[1]);
		ASSERT_EQ(100, a3[1]);
	}
	// <Test> copy (this)
	{
		SharedList<int> a1 = { 1, 2, 3 };
		a1 = a1;
		ASSERT_EQ(3, a1.size());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(2, a1[1]);
		ASSERT_EQ(3, a1[2]);
	}
	// <Test> move
	{
		SharedList<int> a1 = { 1, 2, 3 };
		SharedList<int> a2 = std::move(a1);
		ASSERT_EQ(nullptr, a1);
		ASSERT_EQ(3, a2.size());
		ASSERT_EQ(1, a2[0]);
		ASSERT_EQ(2, a2[1]);
		ASSERT_EQ(3, a2[2]);
	}
}

TEST_F(Test_Base_List, ArrayAccess)
{
	// <Test> non const
	{
		SharedList<int> a1 = { 1, 2, 3 };
		ASSERT_EQ(3, a1.size());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(2, a1[1]);
		ASSERT_EQ(3, a1[2]);
	}
	// <Test> const
	{
		const SharedList<int> a1 = { 1, 2, 3 };
		ASSERT_EQ(3, a1.size());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(2, a1[1]);
		ASSERT_EQ(3, a1[2]);
	}
}

TEST_F(Test_Base_List, Iteraotr)
{
	// <Test> non const
	{
		SharedList<int> a1 = { 1, 2, 3 };
		int t = 0;
		for (auto i : a1) t += i;
		ASSERT_EQ(6, t);
	}
	// <Test> const
	{
		const SharedList<int> a1 = { 1, 2, 3 };
		int t = 0;
		for (auto i : a1) t += i;
		ASSERT_EQ(6, t);
	}
}





template<typename TValue>
class Ref<List<TValue>>
{
public:
};




TEST_F(Test_Base_List, CopyFramework)
{
	// <Test> copy
	{
		Ref<List<int>> a1 = { 1, 2, 3 };
		Ref<List<int>> a2 = a1;
		Ref<List<int>> a3;
		a3 = a1;
		ASSERT_EQ(3, a1.size());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(2, a1[1]);
		ASSERT_EQ(3, a1[2]);
		ASSERT_EQ(3, a2.size());
		ASSERT_EQ(1, a2[0]);
		ASSERT_EQ(2, a2[1]);
		ASSERT_EQ(3, a2[2]);
		ASSERT_EQ(3, a3.size());
		ASSERT_EQ(1, a3[0]);
		ASSERT_EQ(2, a3[1]);
		ASSERT_EQ(3, a3[2]);

		// 共有なので全部影響する
		a1[1] = 100;
		ASSERT_EQ(100, a1[1]);
		ASSERT_EQ(100, a2[1]);
		ASSERT_EQ(100, a3[1]);
	}
	// <Test> copy (this)
	{
		Ref<List<int>> a1 = { 1, 2, 3 };
		a1 = a1;
		ASSERT_EQ(3, a1.size());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(2, a1[1]);
		ASSERT_EQ(3, a1[2]);
	}
	// <Test> move
	{
		Ref<List<int>> a1 = { 1, 2, 3 };
		Ref<List<int>> a2 = std::move(a1);
		ASSERT_EQ(nullptr, a1);
		ASSERT_EQ(3, a2.size());
		ASSERT_EQ(1, a2[0]);
		ASSERT_EQ(2, a2[1]);
		ASSERT_EQ(3, a2[2]);
	}
}
