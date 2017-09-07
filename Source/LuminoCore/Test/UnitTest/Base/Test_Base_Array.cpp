#include <TestConfig.h>
#include <Lumino/Base/Stack.h>
#include <map>
#include <set>
#include <list>
#include <vector>

class Test_Base_Array : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

template<typename T>
class StackArray
{
public:

	int add(const T& value)
	{
#if 0

		// 管理配列がすべて埋まっている場合は領域を足す
		if (m_indexStack.IsEmpty())
		{
			int count = (int)m_array.size() + 1;
			for (int i = 0; i < count; ++i)
			{
				m_indexStack.push((count - 1) + i);
				//m_array.push_back(T());
			}
			m_array.resize(m_array.size() + count);
		}

		// 空いている場所を取ってきてそこに格納
		int newIndex = m_indexStack.getTop();
		m_indexStack.pop();
		m_array.at(newIndex) = value;//[newIndex] = value;
		return newIndex;
#endif
		// 管理配列がすべて埋まっている場合は末尾に追加する
		if (m_indexStack.isEmpty())
		{
			int index = m_indexStack.getCount();
			m_array.push_back(value);//.Add(value);
			return index;
		}
		// 空き場所があればそこに格納する
		else
		{
			int newIndex = m_indexStack.getTop();
			m_indexStack.pop();	// TODO: これが重い気がする
			m_array.at(newIndex) = value;//[newIndex] = value;
			return newIndex;
		}
	}

	void removeAt(int index)
	{
		m_array.at(index) = T();//[index] = T();
		m_indexStack.push(index);
	}

	const T& getAt(int index) const
	{
		return m_array.at(index);//[index];
	}

	void clear()
	{
		m_indexStack.clear();
		int size = (int)m_array.size();
		for (int i = 0; i < size; ++i) {
			m_indexStack.push(i);
		}
	}

private:
	std::vector<T>	m_array;
	Stack<int>	m_indexStack;
};


//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, isEmpty)
{
	StackArray<int> stackArray;
	std::vector<int> vec1;
	std::list<int> list1;
	std::set<int> set1;
	//const int testCount = 1000000;

#if 0
	// 要素の追加
	printf("要素の追加 ----\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.start();
		for (int i = 0; i < testCount; ++i) { stackArray.Add(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.start();
		for (int i = 0; i < testCount; ++i) { vec1.push_back(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.start();
		for (int i = 0; i < testCount; ++i) { list1.push_back(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.start();
		for (int i = 0; i < testCount; ++i) { set1.insert(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");


	stackArray.Clear();
	vec1.clear();
	list1.clear();
	set1.clear();

	// Reserve からの要素の追加
	printf("Reserve からの要素の追加 ----\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.start();
		for (int i = 0; i < testCount; ++i) { stackArray.Add(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.start();
		for (int i = 0; i < testCount; ++i) { vec1.push_back(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.start();
		for (int i = 0; i < testCount; ++i) { list1.push_back(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.start();
		for (int i = 0; i < testCount; ++i) { set1.insert(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");

	// 指定要素へのアクセス
	printf("指定要素へのアクセス ----\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.start();
		int sum = 0;
		for (int i = 0; i < testCount; ++i) { sum += stackArray.GetAt(i); }
		printf("%7llu", timer.GetElapsedTime(), sum);
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.start();
		int sum = 0;
		for (int i = 0; i < testCount; ++i) { sum += vec1.at(i); }
		printf("%7llu", timer.GetElapsedTime(), sum);
	}
	printf("\n");
	//for (int j = 0; j < 10; j++)	// list は論外
	//{
	//	ElapsedTimer timer;
	//	timer.Start();
	//	for (int i = 0; i < testCount; ++i) { list1.push_back(i); }
	//	printf("%7llu", timer.getElapsedTime());
	//}
	//printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.start();
		int sum = 0;
		for (int i = 0; i < testCount; ++i) { sum += *(set1.find(i)); }
		printf("%7llu", timer.GetElapsedTime(), sum);
	}
	printf("\n");

	// 要素の部分削除
	printf("要素の部分削除 ----\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.start();
		for (int i = 0; i < testCount / 2; ++i) { stackArray.RemoveAt(i * 2); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 2; j++)
	{
		ElapsedTimer timer;
		timer.start();
		for (int i = 0; i < testCount / 2; ++i) { vec1.erase(vec1.begin() + 1); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.start();
		for (int i = 0; i < testCount / 2; ++i) { list1.erase(list1.begin()++); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.start();
		for (int i = 0; i < testCount / 2; ++i) { set1.erase(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");

	return;
#endif


	// <Test> 配列が空かを確認する。
	{
		List<int> a1;
		ASSERT_TRUE(a1.isEmpty());
		a1.add(1);
		ASSERT_FALSE(a1.isEmpty());
		a1.clear();
		ASSERT_TRUE(a1.isEmpty());
	}
	

#if 0
	String str = "";
	List<String> ary1;
	List<String> ary2;

	printf("----\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.start();

		for (int i = 0; i < 1000000; ++i)
		{
			ary1 = StringTraits::Split(str, _T(","));
			List<String> a11 = ary1;
		}
		printf("t:%llu\n", timer.GetElapsedTime());
	}
	printf("----\n");
	//for (int j = 0; j < 10; j++)
	//{
	//	ElapsedTimer timer;
	//	timer.Start();

	//	for (int i = 0; i < 1000000; ++i)
	//	{
	//		ary2 = StringTraits::Split2(str, _T(","));
	//		List<String> a22 = ary2;
	//	}
	//	printf("t:%llu\n", timer.getElapsedTime());
	//}
#endif
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, getCount)
{
	// <Test> 要素数を取得する。
	{
		List<int> a1;
		ASSERT_EQ(0, a1.getCount());
		a1.add(1);
		ASSERT_EQ(1, a1.getCount());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, assign)
{
	// <Test> 
	{
		std::vector<int> a = { 1, 2, 3 };
		std::vector<int> b = { 4, 5 };
		a.assign(b.begin(), b.end());
		printf("");
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, add)
{
	// <Test> 要素を追加する。
	{
		List<int> a1;
		a1.add(1);
		a1.add(2);
		ASSERT_EQ(2, a1.getCount());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(2, a1[1]);
	}
	// <Test> 配列を末尾に結合する。
	{
		List<String> a1, a2, a3;
		a2.add(_LT("1")); a2.add(_LT("2"));
		a3.add(_LT("3")); a3.add(_LT("4"));
		a1.addRange(a2); a1.addRange(a3);
		ASSERT_EQ(4, a1.getCount());
		ASSERT_EQ(_LT("1"), a1[0]);
		ASSERT_EQ(_LT("2"), a1[1]);
		ASSERT_EQ(_LT("3"), a1[2]);
		ASSERT_EQ(_LT("4"), a1[3]);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, insert)
{
	// <Test> 要素を挿入する。
	{
		List<int> a1;
		a1.add(1);
		a1.add(2);
		a1.insert(0, 3);
		a1.insert(2, 4);
		a1.insert(4, 5);
		ASSERT_EQ(3, a1[0]);
		ASSERT_EQ(1, a1[1]);
		ASSERT_EQ(4, a1[2]);
		ASSERT_EQ(2, a1[3]);
		ASSERT_EQ(5, a1[4]);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, remove)
{
	// ついでにコピーで元の配列が壊れないかも見てみる
	List<int> ary;
	ary.add(1); ary.add(2); ary.add(3); ary.add(2);

	// <Test> Remove
	{
		List<int> a1 = ary;
		a1.remove(2);
		ASSERT_EQ(3, a1.getCount());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(3, a1[1]);
		ASSERT_EQ(2, a1[2]);
	}
	// <Unit> ラムダ式で条件指定できること
	{
		List<int> a1 = ary;
		a1.removeIf([](int v){ return v == 2; });
		ASSERT_EQ(3, a1.getCount());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(3, a1[1]);
		ASSERT_EQ(2, a1[2]);
	}
	// <Test> RemoveAt
	{
		List<int> a1 = ary;
		a1.removeAt(2);
		ASSERT_EQ(3, a1.getCount());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(2, a1[1]);
		ASSERT_EQ(2, a1[2]);
	}
	// <Test> RemoveAll
	{
		List<int> a1 = ary;
		a1.removeAll(2);
		ASSERT_EQ(2, a1.getCount());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(3, a1[1]);
	}
	// <Test> RemoveAll (ラムダ式)
	{
		List<int> a1 = ary;
        auto exp = [](int v) { return v == 2; };
		a1.removeAll(exp);
		ASSERT_EQ(2, a1.getCount());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(3, a1[1]);
	}
	// <Test> RemoveLast
	{
		List<int> a1 = ary;
		a1.removeLast();
		ASSERT_EQ(3, a1.getCount());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(2, a1[1]);
		ASSERT_EQ(3, a1[2]);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, reserve)
{
	// <Test> Reserve
	{
		List<int> a1;
		a1.reserve(2);
		ASSERT_EQ(2, a1.getCapacity());
		a1.add(1); a1.add(2);
		ASSERT_EQ(2, a1.getCapacity());
		a1.add(3);
		ASSERT_LT(2, a1.getCapacity());	// 2 < cap
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, resize)
{
	// <Test> Resize
	{
		List<int> a1;
		a1.add(1);
		a1.resize(2);
		ASSERT_EQ(2, a1.getCount());
		ASSERT_EQ(0, a1[1]);
		a1[1] = 10;
		ASSERT_EQ(10, a1[1]);
		a1.resize(1);
		ASSERT_EQ(1, a1.getCount());
		ASSERT_EQ(1, a1[0]);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, contains)
{
	// <Test> Contains
	{
		PathName p1(_LT("LuminoCore/include/Lumino/Base/String.h"));
		PathName p2(_LT("LuminoCore/include/Lumino/Base/String.h"));
		PathName p3(_LT("LuminoCore/include/Lumino/Base/String.c"));
		List<PathName> pathes;
		pathes.add(p1);
		ASSERT_TRUE(pathes.contains(p2));
		ASSERT_FALSE(pathes.contains(p3));
	}

	// <Test> Contains
	{
		struct St
		{
			int a;
			int b;
		};
		St s1 = { 10, 20 };
		St s2 = { 30, 40 };
		List<St> ary = { s1, s2, { 50, 60 } };
		ASSERT_TRUE(ary.containsIf([](const St& s) { return s.a == 30; }));
		ASSERT_TRUE(ary.containsIf([](const St& s) { return s.b == 60; }));
		ASSERT_FALSE(ary.containsIf([](const St& s) { return s.a == 0; }));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, indexOf)
{
	// <Test> IndexOf
	{
		List<int> ary;
		ary.add(1); ary.add(2); ary.add(3); ary.add(2);
		ASSERT_EQ(1, ary.indexOf(2));
		ASSERT_EQ(1, ary.indexOf(2, 1));
		ASSERT_EQ(3, ary.indexOf(2, 2));
		ASSERT_EQ(-1, ary.indexOf(10));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, find)
{
	{
		List<int> ary1 = { 1, 2, 3 };
		ASSERT_TRUE(ary1.find(1) != nullptr);
		ASSERT_TRUE(ary1.find(1) != NULL);
		ASSERT_EQ(2, *ary1.find(2));
		ASSERT_EQ(NULL, ary1.find(4));
	}
	{
		struct St { int a; int b; };
		List<St> ary1 = { { 1, 2 }, { 3, 4 } };
		ASSERT_TRUE(ary1.find([](const St& st) { return st.a == 1; }) != nullptr);
		ASSERT_EQ(4, ary1.find([](const St& st) { return st.a == 3; })->b);
		ASSERT_EQ(nullptr, ary1.find([](const St& st) { return st.a == 5; }));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, Front_Last)
{
	// <Test> GetFront, GetLast
	{
		List<int> ary;
		ary.add(1); ary.add(2); ary.add(3); ary.add(4);
		ASSERT_EQ(1, ary.getFront());
		ASSERT_EQ(4, ary.getLast());

		const List<int>& ary2 = ary;
		ASSERT_EQ(1, ary2.getFront());
		ASSERT_EQ(4, ary2.getLast());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, Iterate)
{
	// <Test> 範囲 for で使用できる。
	{
		List<int> a1;
		a1.add(1); a1.add(10); a1.add(100);
		int r = 0;
		for (auto v : a1) {
			r += v;
		}
		ASSERT_EQ(r, 111);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, Issue)
{
	List<int> a1;
	List<int>::iterator itr = a1.begin();
	List<int>::iterator end = a1.end();
	for (; itr != end; ++itr) {
		(*itr);
	}
}
