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

	int Add(const T& value)
	{
#if 0

		// 管理配列がすべて埋まっている場合は領域を足す
		if (m_indexStack.IsEmpty())
		{
			int count = (int)m_array.size() + 1;
			for (int i = 0; i < count; ++i)
			{
				m_indexStack.Push((count - 1) + i);
				//m_array.push_back(T());
			}
			m_array.resize(m_array.size() + count);
		}

		// 空いている場所を取ってきてそこに格納
		int newIndex = m_indexStack.GetTop();
		m_indexStack.Pop();
		m_array.at(newIndex) = value;//[newIndex] = value;
		return newIndex;
#endif
		// 管理配列がすべて埋まっている場合は末尾に追加する
		if (m_indexStack.IsEmpty())
		{
			int index = m_indexStack.GetCount();
			m_array.push_back(value);//.Add(value);
			return index;
		}
		// 空き場所があればそこに格納する
		else
		{
			int newIndex = m_indexStack.GetTop();
			m_indexStack.Pop();	// TODO: これが重い気がする
			m_array.at(newIndex) = value;//[newIndex] = value;
			return newIndex;
		}
	}

	void RemoveAt(int index)
	{
		m_array.at(index) = T();//[index] = T();
		m_indexStack.Push(index);
	}

	const T& GetAt(int index) const
	{
		return m_array.at(index);//[index];
	}

	void Clear()
	{
		m_indexStack.Clear();
		int size = (int)m_array.size();
		for (int i = 0; i < size; ++i) {
			m_indexStack.Push(i);
		}
	}

private:
	std::vector<T>	m_array;
	Stack<int>	m_indexStack;
};


//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, IsEmpty)
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
		timer.Start();
		for (int i = 0; i < testCount; ++i) { stackArray.Add(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.Start();
		for (int i = 0; i < testCount; ++i) { vec1.push_back(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.Start();
		for (int i = 0; i < testCount; ++i) { list1.push_back(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.Start();
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
		timer.Start();
		for (int i = 0; i < testCount; ++i) { stackArray.Add(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.Start();
		for (int i = 0; i < testCount; ++i) { vec1.push_back(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.Start();
		for (int i = 0; i < testCount; ++i) { list1.push_back(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.Start();
		for (int i = 0; i < testCount; ++i) { set1.insert(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");

	// 指定要素へのアクセス
	printf("指定要素へのアクセス ----\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.Start();
		int sum = 0;
		for (int i = 0; i < testCount; ++i) { sum += stackArray.GetAt(i); }
		printf("%7llu", timer.GetElapsedTime(), sum);
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.Start();
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
	//	printf("%7llu", timer.GetElapsedTime());
	//}
	//printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.Start();
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
		timer.Start();
		for (int i = 0; i < testCount / 2; ++i) { stackArray.RemoveAt(i * 2); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 2; j++)
	{
		ElapsedTimer timer;
		timer.Start();
		for (int i = 0; i < testCount / 2; ++i) { vec1.erase(vec1.begin() + 1); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.Start();
		for (int i = 0; i < testCount / 2; ++i) { list1.erase(list1.begin()++); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.Start();
		for (int i = 0; i < testCount / 2; ++i) { set1.erase(i); }
		printf("%7llu", timer.GetElapsedTime());
	}
	printf("\n");

	return;
#endif


	// <Test> 配列が空かを確認する。
	{
		List<int> a1;
		ASSERT_TRUE(a1.IsEmpty());
		a1.Add(1);
		ASSERT_FALSE(a1.IsEmpty());
		a1.Clear();
		ASSERT_TRUE(a1.IsEmpty());
	}
	

#if 0
	String str = "";
	List<String> ary1;
	List<String> ary2;

	printf("----\n");
	for (int j = 0; j < 10; j++)
	{
		ElapsedTimer timer;
		timer.Start();

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
	//	printf("t:%llu\n", timer.GetElapsedTime());
	//}
#endif
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, GetCount)
{
	// <Test> 要素数を取得する。
	{
		List<int> a1;
		ASSERT_EQ(0, a1.GetCount());
		a1.Add(1);
		ASSERT_EQ(1, a1.GetCount());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, Assign)
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
TEST_F(Test_Base_Array, Add)
{
	// <Test> 要素を追加する。
	{
		List<int> a1;
		a1.Add(1);
		a1.Add(2);
		ASSERT_EQ(2, a1.GetCount());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(2, a1[1]);
	}
	// <Test> 配列を末尾に結合する。
	{
		List<String> a1, a2, a3;
		a2.Add(_T("1")); a2.Add(_T("2"));
		a3.Add(_T("3")); a3.Add(_T("4"));
		a1.AddRange(a2); a1.AddRange(a3);
		ASSERT_EQ(4, a1.GetCount());
		ASSERT_EQ(_T("1"), a1[0]);
		ASSERT_EQ(_T("2"), a1[1]);
		ASSERT_EQ(_T("3"), a1[2]);
		ASSERT_EQ(_T("4"), a1[3]);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, Insert)
{
	// <Test> 要素を挿入する。
	{
		List<int> a1;
		a1.Add(1);
		a1.Add(2);
		a1.Insert(0, 3);
		a1.Insert(2, 4);
		a1.Insert(4, 5);
		ASSERT_EQ(3, a1[0]);
		ASSERT_EQ(1, a1[1]);
		ASSERT_EQ(4, a1[2]);
		ASSERT_EQ(2, a1[3]);
		ASSERT_EQ(5, a1[4]);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, Remove)
{
	// ついでにコピーで元の配列が壊れないかも見てみる
	List<int> ary;
	ary.Add(1); ary.Add(2); ary.Add(3); ary.Add(2);

	// <Test> Remove
	{
		List<int> a1 = ary;
		a1.Remove(2);
		ASSERT_EQ(3, a1.GetCount());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(3, a1[1]);
		ASSERT_EQ(2, a1[2]);
	}
	// <Unit> ラムダ式で条件指定できること
	{
		List<int> a1 = ary;
		a1.RemoveIf([](int v){ return v == 2; });
		ASSERT_EQ(3, a1.GetCount());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(3, a1[1]);
		ASSERT_EQ(2, a1[2]);
	}
	// <Test> RemoveAt
	{
		List<int> a1 = ary;
		a1.RemoveAt(2);
		ASSERT_EQ(3, a1.GetCount());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(2, a1[1]);
		ASSERT_EQ(2, a1[2]);
	}
	// <Test> RemoveAll
	{
		List<int> a1 = ary;
		a1.RemoveAll(2);
		ASSERT_EQ(2, a1.GetCount());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(3, a1[1]);
	}
	// <Test> RemoveAll (ラムダ式)
	{
		List<int> a1 = ary;
        auto exp = [](int v) { return v == 2; };
		a1.RemoveAll(exp);
		ASSERT_EQ(2, a1.GetCount());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(3, a1[1]);
	}
	// <Test> RemoveLast
	{
		List<int> a1 = ary;
		a1.RemoveLast();
		ASSERT_EQ(3, a1.GetCount());
		ASSERT_EQ(1, a1[0]);
		ASSERT_EQ(2, a1[1]);
		ASSERT_EQ(3, a1[2]);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, Reserve)
{
	// <Test> Reserve
	{
		List<int> a1;
		a1.Reserve(2);
		ASSERT_EQ(2, a1.GetCapacity());
		a1.Add(1); a1.Add(2);
		ASSERT_EQ(2, a1.GetCapacity());
		a1.Add(3);
		ASSERT_LT(2, a1.GetCapacity());	// 2 < cap
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, Resize)
{
	// <Test> Resize
	{
		List<int> a1;
		a1.Add(1);
		a1.Resize(2);
		ASSERT_EQ(2, a1.GetCount());
		ASSERT_EQ(0, a1[1]);
		a1[1] = 10;
		ASSERT_EQ(10, a1[1]);
		a1.Resize(1);
		ASSERT_EQ(1, a1.GetCount());
		ASSERT_EQ(1, a1[0]);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, Contains)
{
	// <Test> Contains
	{
		PathNameA p1("LuminoCore/include/Lumino/Base/String.h");
		PathNameA p2("LuminoCore/include/Lumino/Base/String.h");
		PathNameA p3("LuminoCore/include/Lumino/Base/String.c");
		List<PathNameA> pathes;
		pathes.Add(p1);
		ASSERT_TRUE(pathes.Contains(p2));
		ASSERT_FALSE(pathes.Contains(p3));
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
		ASSERT_TRUE(ary.Contains([](const St& s) { return s.a == 30; }));
		ASSERT_TRUE(ary.Contains([](const St& s) { return s.b == 60; }));
		ASSERT_FALSE(ary.Contains([](const St& s) { return s.a == 0; }));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, IndexOf)
{
	// <Test> IndexOf
	{
		List<int> ary;
		ary.Add(1); ary.Add(2); ary.Add(3); ary.Add(2);
		ASSERT_EQ(1, ary.IndexOf(2));
		ASSERT_EQ(1, ary.IndexOf(2, 1));
		ASSERT_EQ(3, ary.IndexOf(2, 2));
		ASSERT_EQ(-1, ary.IndexOf(10));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, Find)
{
	{
		List<int> ary1 = { 1, 2, 3 };
		ASSERT_TRUE(ary1.Find(1) != nullptr);
		ASSERT_TRUE(ary1.Find(1) != NULL);
		ASSERT_EQ(2, *ary1.Find(2));
		ASSERT_EQ(NULL, ary1.Find(4));
	}
	{
		struct St { int a; int b; };
		List<St> ary1 = { { 1, 2 }, { 3, 4 } };
		ASSERT_TRUE(ary1.Find([](const St& st) { return st.a == 1; }) != nullptr);
		ASSERT_EQ(4, ary1.Find([](const St& st) { return st.a == 3; })->b);
		ASSERT_EQ(nullptr, ary1.Find([](const St& st) { return st.a == 5; }));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, Front_Last)
{
	// <Test> GetFront, GetLast
	{
		List<int> ary;
		ary.Add(1); ary.Add(2); ary.Add(3); ary.Add(4);
		ASSERT_EQ(1, ary.GetFront());
		ASSERT_EQ(4, ary.GetLast());

		const List<int>& ary2 = ary;
		ASSERT_EQ(1, ary2.GetFront());
		ASSERT_EQ(4, ary2.GetLast());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Array, Iterate)
{
	// <Test> 範囲 for で使用できる。
	{
		List<int> a1;
		a1.Add(1); a1.Add(10); a1.Add(100);
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
