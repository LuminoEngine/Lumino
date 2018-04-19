#include "Common.hpp"
#include <Lumino/Base/List.hpp>

class Test_Base_List : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST_F(Test_Base_List, Construct)
{
	List<int> list1;
	List<int> list2 = { 1, 2, 3 };
	ASSERT_EQ(0, list1.size());
	ASSERT_EQ(3, list2.size());

	List<int> list3 = list2;
	List<int> list4 = std::move(list2);
	ASSERT_EQ(3, list3.size());
	ASSERT_EQ(3, list4.size());
	ASSERT_EQ(0, list2.size());

	list2 = list3;
	list4 = std::move(list3);
	ASSERT_EQ(3, list2.size());
	ASSERT_EQ(3, list4.size());
	ASSERT_EQ(0, list3.size());

	List<int> list5(list4.begin(), list4.end());
	ASSERT_EQ(3, list5.size());

	list5 = list5;
	ASSERT_EQ(3, list5.size());

	list5 = std::move(list5);
	ASSERT_EQ(3, list5.size());
}

TEST_F(Test_Base_List, Access)
{
	List<int> list2 = { 1, 2, 3, 4 };
	list2.at(1) = 20;
	list2[2] = 30;
	list2.front() = 10;
	list2.back() = 40;

	ASSERT_EQ(20, list2.at(1));
	ASSERT_EQ(30, list2[2]);
	ASSERT_EQ(10, list2.front());
	ASSERT_EQ(40, list2.back());
}

TEST_F(Test_Base_List, Capacity)
{
	List<int> list1;

	list1.reserve(100);
	ASSERT_EQ(100, list1.capacity());

	list1.resize(1);
	ASSERT_EQ(1, list1.size());
	ASSERT_EQ(0, list1[0]);

	list1.resize(2, 5);
	ASSERT_EQ(2, list1.size());
	ASSERT_EQ(0, list1[0]);
	ASSERT_EQ(5, list1[1]);

	list1.shrinkToFit();
	ASSERT_EQ(2, list1.capacity());
}

TEST_F(Test_Base_List, Iterator)
{
	List<int> list1 = { 1, 2, 3 };
	const List<int> list2 = { 1, 2, 3 };
	int s = 0;

	s = 0;
	for (auto i : list1) s += i;
	ASSERT_EQ(6, s);

	s = 0;
	for (auto i : list2) s += i;
	ASSERT_EQ(6, s);
}

TEST_F(Test_Base_List, Modifiers)
{
	List<int> list1;
	List<int> list2 = { 1, 2, 3 };

	list1.assign(list2.begin(), list2.end());
	ASSERT_EQ(3, list1.size());
	ASSERT_EQ(1, list1[0]);
	ASSERT_EQ(2, list1[1]);
	ASSERT_EQ(3, list1[2]);

	list1.add(4);
	ASSERT_EQ(4, list1.size());
	ASSERT_EQ(4, list1[3]);

	list1.addRange(list2);
	ASSERT_EQ(7, list1.size());
	ASSERT_EQ(1, list1[4]);
	ASSERT_EQ(2, list1[5]);
	ASSERT_EQ(3, list1[6]);

	list1.insert(1, 10);
	ASSERT_EQ(8, list1.size());
	ASSERT_EQ(10, list1[1]);

	list1.insertRange(1, list2);
	ASSERT_EQ(11, list1.size());
	ASSERT_EQ(1, list1[1]);
	ASSERT_EQ(2, list1[2]);
	ASSERT_EQ(3, list1[3]);

	list1.clear();
	ASSERT_EQ(0, list1.size());


	//* [ ] erase
	{
		list1 = list2;
		auto itr1 = list1.erase(list1.begin());
		ASSERT_EQ(2, list1.size());
		ASSERT_EQ(2, list1[0]);
		ASSERT_EQ(3, list1[1]);

		list1.erase(itr1);
		ASSERT_EQ(1, list1.size());
		ASSERT_EQ(3, list1[0]);
	}

	//* [ ] remove
	{
		list1 = { 1, 2, 3, 2, 1 };
		bool r = list1.remove(2);
		ASSERT_EQ(true, r);
		ASSERT_EQ(4, list1.size());
		ASSERT_EQ(1, list1[0]);
		ASSERT_EQ(3, list1[1]);
		ASSERT_EQ(2, list1[2]);
		ASSERT_EQ(1, list1[3]);

		r = list1.remove(20);
		ASSERT_EQ(false, r);
		ASSERT_EQ(4, list1.size());
	}

	//* [ ] removeIf
	{
		list1 = { 1, 2, 3, 2, 1 };
		bool r = list1.removeIf([](int i) { return i == 2; });
		ASSERT_EQ(true, r);
		ASSERT_EQ(4, list1.size());
		ASSERT_EQ(1, list1[0]);
		ASSERT_EQ(3, list1[1]);
		ASSERT_EQ(2, list1[2]);
		ASSERT_EQ(1, list1[3]);

		r = list1.removeIf([](int i) { return i == 20; });
		ASSERT_EQ(false, r);
		ASSERT_EQ(4, list1.size());
	}

	//* [ ] removeAll
	{
		list1 = { 1, 2, 3, 2, 1 };
		int r = list1.removeAll(2);
		ASSERT_EQ(2, r);
		ASSERT_EQ(3, list1.size());
		ASSERT_EQ(1, list1[0]);
		ASSERT_EQ(3, list1[1]);
		ASSERT_EQ(1, list1[2]);

		r = list1.removeAll(20);
		ASSERT_EQ(0, r);
		ASSERT_EQ(3, list1.size());
	}

	//* [ ] removeAllIf
	{
		list1 = { 1, 2, 3, 2, 1 };
		int r = list1.removeAllIf([](int i) { return i == 2; });
		ASSERT_EQ(2, r);
		ASSERT_EQ(3, list1.size());
		ASSERT_EQ(1, list1[0]);
		ASSERT_EQ(3, list1[1]);
		ASSERT_EQ(1, list1[2]);

		r = list1.removeAllIf([](int i) { return i == 20; });
		ASSERT_EQ(0, r);
		ASSERT_EQ(3, list1.size());
	}

	//* [ ] removeAt
	{
		list1 = { 1, 2, 3 };
		list1.removeAt(1);
		ASSERT_EQ(2, list1.size());
		ASSERT_EQ(1, list1[0]);
		ASSERT_EQ(3, list1[1]);
	}

	//* [ ] removeFirst
	//* [ ] removeLast
	{
		list1 = { 1, 2, 3 };
		list1.removeFirst();
		ASSERT_EQ(2, list1.size());
		ASSERT_EQ(2, list1[0]);
		ASSERT_EQ(3, list1[1]);

		list1.removeLast();
		ASSERT_EQ(1, list1.size());
		ASSERT_EQ(2, list1[0]);
	}
}

TEST_F(Test_Base_List, Algorithm)
{
	//* [ ] contains
	{
		List<int> list1 = { 1, 2, 3 };
		ASSERT_EQ(true, list1.contains(2));
		ASSERT_EQ(false, list1.contains(5));
	}
	//* [ ] containsIf
	{
		List<int> list1 = { 1, 2, 3 };
		ASSERT_EQ(true, list1.containsIf([](int i) { return i == 2; }));
		ASSERT_EQ(false, list1.containsIf([](int i) { return i == 5; }));
	}
	//* [ ] indexOf
	{
		List<int> list1 = { 1, 2, 1, 3 };
		ASSERT_EQ(0, list1.indexOf(1));
		ASSERT_EQ(2, list1.indexOf(1, 1));
		ASSERT_EQ(-1, list1.indexOf(5));
		ASSERT_EQ(-1, list1.indexOf(1, 3));
	}
	//* [ ] indexOfIf
	{
		List<int> list1 = { 1, 2, 1, 3 };
		ASSERT_EQ(0, list1.indexOfIf([](int i) { return i == 1; }));
		ASSERT_EQ(2, list1.indexOfIf([](int i) { return i == 1; }, 1));
		ASSERT_EQ(-1, list1.indexOfIf([](int i) { return i == 5; }));
		ASSERT_EQ(-1, list1.indexOfIf([](int i) { return i == 1; }, 3));
	}

	//* [ ] indexOf
	{
		List<int> list1 = { 1, 2, 3 };
		auto v = list1.find(1);
		if (v) {
			ASSERT_EQ(1, v.value());
		}
		else {
			FAIL();
		}

		ASSERT_EQ(false, list1.find(10).hasValue());
	}
	//* [ ] indexOfIf
	{
		List<int> list1 = { 1, 2, 3 };
		auto v = list1.findIf([](int i) { return i == 1; });
		if (v) {
			ASSERT_EQ(1, v.value());
		}
		else {
			FAIL();
		}

		ASSERT_EQ(false, list1.findIf([](int i) { return i == 10; }).hasValue());
	}
}
