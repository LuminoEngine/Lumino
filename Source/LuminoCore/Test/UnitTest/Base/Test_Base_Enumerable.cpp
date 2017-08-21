#include <TestConfig.h>
#include <Lumino/Base/Enumerable.h>

class Test_Base_Enumerable : public ::testing::Test
{
};

//------------------------------------------------------------------------------
TEST_F(Test_Base_Enumerable, Basic)
{
	//{
	//	List<int> list = {1, 2, 3};
	//	tr::Enumerator<int> e(list.begin(), list.end());
	//	for (auto i : e)
	//	{
	//		printf("%d\n", i);
	//	}
	//	printf("e1\n");
	//}
	//printf("e\n");


	//{
	//	List<int> list = { 1, 2, 3, 4, 5 };
	//	tr::Enumerator<int> e(list.begin(), list.end());
	//	e.Where([](int v) { return v % 2 == 0; });
	//	for (auto i : e)
	//	{
	//		printf("%d\n", i);
	//	}
	//	printf("e1\n");
	//}
	//printf("e\n");

	{
		List<int> list = { 1, 2, 3, 4, 5 };
		auto e = tr::MakeEnumerator::from(list.begin(), list.end());
		for (auto i : e)
		{
			//printf("%d\n", i);
		}
	}
	// <Test> 前置デクリメント
	{
		int sum = 0;
		List<int> list = { 1, 2, 3, 4, 5 };
		auto e = tr::MakeEnumerator::from(list.begin(), list.end());
		for (auto itr = e.begin(); itr != e.end(); ++itr) sum += *itr;
		ASSERT_EQ(15, sum);
	}
	// <Test> 後置デクリメント
	{
		int sum = 0;
		List<int> list = { 1, 2, 3, 4, 5 };
		auto e = tr::MakeEnumerator::from(list.begin(), list.end());
		for (auto itr = e.begin(); itr != e.end(); itr++) sum += *itr;
		ASSERT_EQ(15, sum);
	}

	{
		List<int> list = { 1, 2, 3, 4, 5 };
		auto e = tr::MakeEnumerator::from(list.begin(), list.end());
		e = e.Where([](int v) { return v % 2 == 0; });
		for (auto i : e)
		{
			//printf("%d\n", i);
		}
	}
	{
		List<int> list1 = { 1, 2, 3, 4, 5 };
		List<int> list2 = { 6, 7, 8, 9, 0 };
		//auto e1 = tr::MakeEnumerator::from(list1.begin(), list1.end());
		//auto e2 = ;
		auto e = tr::MakeEnumerator::from(list1.begin(), list1.end());
		e = e.Join(tr::MakeEnumerator::from(list2.begin(), list2.end()));
		for (auto i : e)
		{
			//printf("%d\n", i);
		}
	}
	//{
	//	List<int> list = { 1, 2, 3, 4, 5 };
	//	auto e = tr::MakeEnumerator::from(list.begin(), list.end());
	//	auto e2 = e.Select([](int v) { return v % 2 == 0; });
	//	for (auto i : e2)
	//	{
	//		printf("%d\n", i);
	//	}
	//}

	/*
		Where (フィルタ)
		Join
		Select (map・変換)
	*/
}
