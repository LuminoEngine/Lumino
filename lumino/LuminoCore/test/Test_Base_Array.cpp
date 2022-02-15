#include "Common.hpp"
#include <LuminoCore/Base/Array.hpp>

class Test_Base_Array : public ::testing::Test {
protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(Test_Base_Array, Construct) {
    Array<int> list1;
    ASSERT_EQ(0, list1.size());

    Array<int> list2 = { 1, 2, 3 };
    ASSERT_EQ(3, list2.size());
    ASSERT_EQ(1, list2[0]);
    ASSERT_EQ(2, list2[1]);
    ASSERT_EQ(3, list2[2]);

    Array<int> list3 = list2;
    ASSERT_EQ(3, list3.size());
    ASSERT_EQ(1, list3[0]);
    ASSERT_EQ(2, list3[1]);
    ASSERT_EQ(3, list3[2]);

    Array<int> list4 = std::move(list2);
    ASSERT_EQ(0, list2.size());
    ASSERT_EQ(3, list4.size());
    ASSERT_EQ(1, list4[0]);
    ASSERT_EQ(2, list4[1]);
    ASSERT_EQ(3, list4[2]);

    std::vector<int> vec1 = { 1, 2, 3 };
    Array<int> list5(vec1);
    ASSERT_EQ(3, list5.size());
    ASSERT_EQ(1, list5[0]);
    ASSERT_EQ(2, list5[1]);
    ASSERT_EQ(3, list5[2]);

    Array<int> list6(std::move(vec1));
    ASSERT_EQ(0, vec1.size());
    ASSERT_EQ(3, list6.size());
    ASSERT_EQ(1, list6[0]);
    ASSERT_EQ(2, list6[1]);
    ASSERT_EQ(3, list6[2]);

    Array<int> list7(list4.begin(), list4.end());
    ASSERT_EQ(3, list7.size());
    ASSERT_EQ(1, list7[0]);
    ASSERT_EQ(2, list7[1]);
    ASSERT_EQ(3, list7[2]);
}

TEST_F(Test_Base_Array, AssignOperator) {
    Array<int> list1 = { 1, 2 };
    Array<int> list2, list3;

    list2 = list1;
    ASSERT_EQ(2, list2.size());
    ASSERT_EQ(1, list2[0]);
    ASSERT_EQ(2, list2[1]);

    list3 = std::move(list1);
    ASSERT_EQ(0, list1.size());
    ASSERT_EQ(2, list2.size());
    ASSERT_EQ(1, list2[0]);
    ASSERT_EQ(2, list2[1]);

    std::vector<int> vec1 = { 1, 2 };
    Array<int> list4, list5, list6;

    list2 = vec1;
    ASSERT_EQ(2, list2.size());
    ASSERT_EQ(1, list2[0]);
    ASSERT_EQ(2, list2[1]);

    list3 = std::move(vec1);
    ASSERT_EQ(0, vec1.size());
    ASSERT_EQ(2, list3.size());
    ASSERT_EQ(1, list3[0]);
    ASSERT_EQ(2, list3[1]);

    list6 = { 5, 6 };
    ASSERT_EQ(2, list6.size());
    ASSERT_EQ(5, list6[0]);
    ASSERT_EQ(6, list6[1]);
}

TEST_F(Test_Base_Array, Access) {
    Array<int> list1 = { 1, 2, 3, 4 };
    list1.at(1) = 20;
    list1[2] = 30;
    list1.front() = 10;
    list1.back() = 40;

    ASSERT_EQ(20, list1.at(1));
    ASSERT_EQ(30, list1[2]);
    ASSERT_EQ(10, list1.front());
    ASSERT_EQ(40, list1.back());
    ASSERT_EQ(30, list1.data()[2]);

    const Array<int> list2 = { 1, 2, 3, 4 };
    ASSERT_EQ(2, list2.at(1));
    ASSERT_EQ(3, list2[2]);
    ASSERT_EQ(1, list2.front());
    ASSERT_EQ(4, list2.back());
    ASSERT_EQ(3, list2.data()[2]);
}

TEST_F(Test_Base_Array, Capacity) {
    Array<int> list1;
    ASSERT_EQ(std::numeric_limits<int>::max(), list1.max_size());
    ASSERT_EQ(true, list1.empty());
    ASSERT_EQ(true, list1.isEmpty());
    ASSERT_EQ(false, static_cast<bool>(list1));

    list1.reserve(100);
    ASSERT_EQ(100, list1.capacity());

    list1.resize(1);
    ASSERT_EQ(1, list1.size());
    ASSERT_EQ(0, list1[0]);

    list1.resize(2, 5);
    ASSERT_EQ(2, list1.size());
    ASSERT_EQ(0, list1[0]);
    ASSERT_EQ(5, list1[1]);

    list1.shrink_to_fit();
    ASSERT_EQ(2, list1.capacity());
    ASSERT_EQ(false, list1.empty());
    ASSERT_EQ(false, list1.isEmpty());
    ASSERT_EQ(true, static_cast<bool>(list1));
}

TEST_F(Test_Base_Array, Iterator) {
    Array<int> list1 = { 1, 2, 3 };
    const Array<int> list2 = { 1, 2, 3 };
    int s = 0;

    s = 0;
    for (auto it = list1.begin(); it != list1.end(); it++)
        s += *it;
    ASSERT_EQ(6, s);

    s = 0;
    for (auto it = list2.begin(); it != list2.end(); it++)
        s += *it;
    ASSERT_EQ(6, s);

    s = 0;
    for (auto it = list1.cbegin(); it != list1.cend(); it++)
        s += *it;
    ASSERT_EQ(6, s);

    s = 0;
    for (auto i : list1)
        s += i;
    ASSERT_EQ(6, s);

    s = 0;
    for (auto i : list2)
        s += i;
    ASSERT_EQ(6, s);
}

TEST_F(Test_Base_Array, Modifiers) {
    Array<int> list1;
    Array<int> list2 = { 1, 2 };

    list1.assign(list2.begin(), list2.end());
    ASSERT_EQ(2, list1.size());
    ASSERT_EQ(1, list1[0]);
    ASSERT_EQ(2, list1[1]);

    list1.assign(2, 1);
    ASSERT_EQ(2, list1.size());
    ASSERT_EQ(1, list1[0]);
    ASSERT_EQ(1, list1[1]);

    list1.assign({ 5, 6 });
    ASSERT_EQ(2, list1.size());
    ASSERT_EQ(5, list1[0]);
    ASSERT_EQ(6, list1[1]);

    Array<int> list3;
    list3.push_back(1);
    ASSERT_EQ(1, list3.size());
    ASSERT_EQ(1, list3[0]);

    list3.assign(list1.begin(), list1.end());
    ASSERT_EQ(2, list3.size());
    ASSERT_EQ(5, list3[0]);
    ASSERT_EQ(6, list3[1]);

    list3.pop_back();
    ASSERT_EQ(1, list3.size());
    ASSERT_EQ(5, list3[0]);

    list2.insert(list2.begin() + 1, 10);
    ASSERT_EQ(3, list2.size());
    ASSERT_EQ(1, list2[0]);
    ASSERT_EQ(10, list2[1]);
    ASSERT_EQ(2, list2[2]);

    list2.insert(list2.begin() + 1, 2, 20);
    ASSERT_EQ(5, list2.size());
    ASSERT_EQ(1, list2[0]);
    ASSERT_EQ(20, list2[1]);
    ASSERT_EQ(20, list2[2]);
    ASSERT_EQ(10, list2[3]);
    ASSERT_EQ(2, list2[4]);

    list1 = { 1, 2 };
    list2 = { 3, 4 };
    list1.insert(list1.begin() + 1, list2.begin(), list2.end());
    ASSERT_EQ(4, list1.size());
    ASSERT_EQ(1, list1[0]);
    ASSERT_EQ(3, list1[1]);
    ASSERT_EQ(4, list1[2]);
    ASSERT_EQ(2, list1[3]);

    list1 = { 1, 2 };
    list1.insert(list1.begin() + 1, { 5, 6 });
    ASSERT_EQ(4, list1.size());
    ASSERT_EQ(1, list1[0]);
    ASSERT_EQ(5, list1[1]);
    ASSERT_EQ(6, list1[2]);
    ASSERT_EQ(2, list1[3]);

    list1.clear();
    ASSERT_EQ(0, list1.size());

    // erase
    {
        list1 = { 1, 2, 3 };
        auto itr1 = list1.erase(list1.begin());
        ASSERT_EQ(2, list1.size());
        ASSERT_EQ(2, list1[0]);
        ASSERT_EQ(3, list1[1]);

        list1.erase(itr1);
        ASSERT_EQ(1, list1.size());
        ASSERT_EQ(3, list1[0]);

        list1 = { 1, 2, 3 };
        list1.erase(list1.begin() + 1, list1.end());
        ASSERT_EQ(1, list1.size());
        ASSERT_EQ(1, list1[0]);
    }

    // swap
    {
        list1 = { 1, 2 };
        list2 = { 3, 4 };

        list1.swap(list2);
        ASSERT_EQ(2, list1.size());
        ASSERT_EQ(3, list1[0]);
        ASSERT_EQ(4, list1[1]);
        ASSERT_EQ(2, list2.size());
        ASSERT_EQ(1, list2[0]);
        ASSERT_EQ(2, list2[1]);

        swap(list1, list2);
        ASSERT_EQ(2, list1.size());
        ASSERT_EQ(1, list1[0]);
        ASSERT_EQ(2, list1[1]);
        ASSERT_EQ(2, list2.size());
        ASSERT_EQ(3, list2[0]);
        ASSERT_EQ(4, list2[1]);
    }

    // emplace
    {
        struct Test1 {
            std::string str;
            Test1(int n, char c)
                : str(n, c) {}
        };
        Array<Test1> list5;
        list5.emplace_back(2, 'a');
        list5.emplace(list5.begin(), 3, 'b');
        ASSERT_EQ(2, list5.size());
        ASSERT_EQ("bbb", list5[0].str);
        ASSERT_EQ("aa", list5[1].str);
    }
}

TEST_F(Test_Base_Array, Operators) {
    Array<int> list0 = { 0 };
    Array<int> list1 = { 1 };
    Array<int> list2 = { 1 };
    Array<int> list3 = { 2 };

    ASSERT_EQ(true, list1 == list2);
    ASSERT_EQ(false, list1 == list3);

    ASSERT_EQ(false, list1 != list2);
    ASSERT_EQ(true, list1 != list3);

    ASSERT_EQ(false, list1 < list2);
    ASSERT_EQ(true, list1 < list3);

    ASSERT_EQ(false, list1 <= list0);
    ASSERT_EQ(true, list1 <= list2);
    ASSERT_EQ(true, list1 <= list3);

    ASSERT_EQ(true, list1 > list0);
    ASSERT_EQ(false, list1 > list2);

    ASSERT_EQ(true, list1 >= list0);
    ASSERT_EQ(true, list1 >= list2);
    ASSERT_EQ(false, list1 >= list3);
}

TEST_F(Test_Base_Array, findIf) {
    struct Test2 {
        int v;
    };

    Array<Test2> list2 = { Test2{ 1 }, Test2{ 2 } };
    auto r = list2.findIf([](const Test2& x) { return x.v == 2; });
    ASSERT_EQ(true, r.hasValue());
    ASSERT_EQ(2, r.value().v);

    r.value().v = 10;
    ASSERT_EQ(10, list2[1].v);

    std::optional<int> a;
}

TEST_F(Test_Base_Array, contains) {
    Array<int> list1 = { 1, 2, 3 };
    ASSERT_EQ(true, list1.contains(2));
    ASSERT_EQ(false, list1.contains(5));
}

TEST_F(Test_Base_Array, containsIf) {
    Array<int> list1 = { 1, 2, 3 };
    ASSERT_EQ(true, list1.containsIf([](int i) { return i == 2; }));
    ASSERT_EQ(false, list1.containsIf([](int i) { return i == 5; }));
}

TEST_F(Test_Base_Array, remove) {

    //* [ ] remove
    {
        Array<int> list1 = { 1, 2, 3, 2, 1 };
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
        Array<int> list1 = { 1, 2, 3, 2, 1 };
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
        Array<int> list1 = { 1, 2, 3, 2, 1 };
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
        Array<int> list1 = { 1, 2, 3, 2, 1 };
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
        Array<int> list1 = { 1, 2, 3 };
        list1.removeAt(1);
        ASSERT_EQ(2, list1.size());
        ASSERT_EQ(1, list1[0]);
        ASSERT_EQ(3, list1[1]);
    }
}

TEST_F(Test_Base_Array, map) {
    struct Test1 {
        int v1;
    };
    struct Test2 : public Test1 {
        int v2;
    };

    Array<Test2> ary1 = { Test2{} };
    Array<Test2*> ary2 = ary1.map([](Test2& x) { return &x; });
    Array<Test1*> ary3 = ary1.map([](Test2& x) { return static_cast<Test1*>(&x); });

    const Array<Test2> ary4 = { Test2{} };
    const Array<const Test2*> ary5 = ary4.map([](const Test2& x) { return &x; });
}

TEST_F(Test_Base_Array, reduce) {
    struct Test1 {
        int v1;
    };

    const Array<Test1> ary1 = { Test1{ 1 }, Test1{ 2 }, Test1{ 3 } };
    int value = ary1.reduce(10, [](auto r, auto& x) { return r + x.v1; });
    ASSERT_EQ(16, value);
}

#if 0



TEST_F(Test_Base_Array, Modifiers)
{

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

TEST_F(Test_Base_Array, Algorithm)
{
	//* [ ] contains
	{
		Array<int> list1 = { 1, 2, 3 };
		ASSERT_EQ(true, list1.contains(2));
		ASSERT_EQ(false, list1.contains(5));
	}
	//* [ ] containsIf
	{
		Array<int> list1 = { 1, 2, 3 };
		ASSERT_EQ(true, list1.containsIf([](int i) { return i == 2; }));
		ASSERT_EQ(false, list1.containsIf([](int i) { return i == 5; }));
	}
	//* [ ] indexOf
	{
		Array<int> list1 = { 1, 2, 1, 3 };
		ASSERT_EQ(0, list1.indexOf(1));
		ASSERT_EQ(2, list1.indexOf(1, 1));
		ASSERT_EQ(-1, list1.indexOf(5));
		ASSERT_EQ(-1, list1.indexOf(1, 3));
	}
	//* [ ] indexOfIf
	{
		Array<int> list1 = { 1, 2, 1, 3 };
		ASSERT_EQ(0, list1.indexOfIf([](int i) { return i == 1; }));
		ASSERT_EQ(2, list1.indexOfIf([](int i) { return i == 1; }, 1));
		ASSERT_EQ(-1, list1.indexOfIf([](int i) { return i == 5; }));
		ASSERT_EQ(-1, list1.indexOfIf([](int i) { return i == 1; }, 3));
	}

	//* [ ] indexOf
	{
		Array<int> list1 = { 1, 2, 3 };
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
		Array<int> list1 = { 1, 2, 3 };
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


//template<>
//template <typename TItem>
//class Ref<Array<TItem>>
//{
//public:
//};

//template<typename Array<TItem>, typename S>
//class SomeRandomClass
//{
//	//put something here
//};
//
//template<typename Array<TItem>>
//class MyTemplateClass
//{
//	void DoSomething(Array<TItem> & t) {
//		//...something
//	}
//};
//
//template <>
//template <typename Array<TItem>, typename S>
//class MyTemplateClass <SomeRandomClass<Array<TItem>, S> >
//{
//	void DoSomething(SomeRandomClass<Array<TItem>, S>& t);
//};
//
//template <>
//template <typename Array<TItem>, typename S>
//void MyTemplateClass<SomeRandomClass<Array<TItem>, S> >::DoSomething(SomeRandomClass<Array<TItem>, S>& t)
//{
//	// something
//}

#if 0
/** RefObject 用 スマートポインタ */
template <typename TItem>
class Ref<const Array<TItem>>
{
public:
	/** null 値を表します。 */
	static const Ref<const Array<TItem>> Null;

	/** 参照を持たない空の Ref を構築します。 */
	LN_CONSTEXPR Ref() LN_NOEXCEPT;

	/** 参照を持たない空の Ref を構築します。 */
	LN_CONSTEXPR Ref(std::nullptr_t) LN_NOEXCEPT;

	/** 生ポインタの所有権を受け取ります。 */
	Ref(const Array<TItem>* ptr);

	/** 生ポインタの所有権を受け取ります。retain が false の場合、参照カウントをインクリメントせずに参照します。 */
	Ref(const Array<TItem>* ptr, bool retain);

	/** 他の Ref と、参照を共有します。(コピーコンストラクタ) */
	Ref(const Ref& ref) LN_NOEXCEPT;

	/** 他の Ref と、参照を共有します。 */
	template<class Y>
	Ref(const Ref<Y>& ref) LN_NOEXCEPT;

	/** 他の Ref から参照をムーブします。 */
	Ref(Ref&& ref) LN_NOEXCEPT;

	/** 他の Ref から参照をムーブします。 */
	template<class Y>
	Ref(Ref<Y>&& ref) LN_NOEXCEPT;

	/** 保持しているオブジェクトの参照を開放します。 */
	~Ref();

	/**
	 * 保持しているオブジェクトの所有権を放棄し、指定されたオブジェクトの参照を設定します。
	 *
	 * @param[in]  ptr    : 管理対象としてセットするオブジェクト
	 * @param[in]  retain : 参照カウントをインクリメントするかどうか。false の場合、参照カウントをインクリメントせずに参照します。
	 */
	void reset(const Array<TItem>* ptr, bool retain = true);

	/** 保持しているオブジェクトの参照を放棄します。 */
	void reset();

	/** 2つの Ref オブジェクトを入れ替えます。 */
	void swap(Ref<const Array<TItem>>& other);

	/** 保持しているオブジェクトの参照を放棄します。参照カウントはデクリメントしません。 */
	const Array<TItem>* detach();

	/** 保持しているオブジェクトへのポインタを取得します。 */
	const Array<TItem>* get() const;

	/** 保持しているオブジェクトの所有権を放棄し、他の Ref が持つ参照を共有します。 */
	Ref& operator=(const Ref& ref) LN_NOEXCEPT;

	/** 保持しているオブジェクトの所有権を放棄し、他の Ref が持つ参照を共有します。 */
	template<class Y>
	Ref& operator=(const Ref<Y>& ref) LN_NOEXCEPT;

	/** 保持しているオブジェクトの所有権を放棄し、他の Ref が持つをムーブします。 */
	Ref& operator=(Ref&& ref) LN_NOEXCEPT;

	/** 保持しているオブジェクトの所有権を放棄し、他の Ref が持つをムーブします。 */
	template<class Y>
	Ref& operator=(Ref<Y>&& ref) LN_NOEXCEPT;

	/** ポインタを間接参照します。 */
	const Array<TItem>& operator*() const LN_NOEXCEPT;

	/** ポインタを通してオブジェクトにアクセスします。 */
	const Array<TItem>* operator->() const LN_NOEXCEPT;

	/** 有効なポインタを保持しているかを確認します。 */
	explicit operator bool() const LN_NOEXCEPT { return (m_ptr != nullptr); }

	/** オブジェクトのポインタへの変換をサポートします。 */
	operator const Array<TItem>*() const { return static_cast<const Array<TItem>*>(m_ptr); } // ここでコンパイルエラーとなる場合、const Array<TItem> の定義があるヘッダファイルを include しているか確認すること。

	RefObject* basePointer() const { return m_ptr; }



	/** 指定したインデックスにある要素への参照を取得します。*/
	TItem& operator[](int index) { return m_ptr->operator[](index); }

	/** 指定したインデックスにある要素への参照を取得します。*/
	const TItem& operator[](int index) const { return m_ptr->operator[](index); }

protected:
	void safeAddRef()
	{
		LN_SAFE_RETAIN(m_ptr);
	}

	void safeRelease()
	{
		LN_SAFE_RELEASE(m_ptr);
	}

	const Array<TItem>* m_ptr;

	template<class U> friend class Ref;
};

//template<class TItem>
//const Ref<const Array<TItem>> Ref<const Array<TItem>>::Null;
//
template<class TItem>
LN_CONSTEXPR Ref<const Array<TItem>>::Ref() LN_NOEXCEPT
	: m_ptr(nullptr)
{
}

template<class TItem>
LN_CONSTEXPR Ref<const Array<TItem>>::Ref(std::nullptr_t) LN_NOEXCEPT
	: m_ptr(nullptr)
{
}

template<class TItem>
Ref<const Array<TItem>>::Ref(const Array<TItem>* ptr)
	: Ref(ptr, true)
{
}

template<class TItem>
Ref<const Array<TItem>>::Ref(const Array<TItem>* ptr, bool retain)
	: m_ptr(ptr)
{
	if (retain) {
		LN_SAFE_RETAIN(m_ptr);
	}
}

template<class TItem>
Ref<const Array<TItem>>::Ref(const Ref& ref) LN_NOEXCEPT
	: m_ptr(ref.m_ptr)
{
	LN_SAFE_RETAIN(m_ptr);
}

template<class TItem>
template<class Y>
Ref<const Array<TItem>>::Ref(const Ref<Y>& ref) LN_NOEXCEPT
	: m_ptr(static_cast<const Array<TItem>*>(ref.get()))
{
	LN_SAFE_RETAIN(m_ptr);
}

template<class TItem>
Ref<const Array<TItem>>::Ref(Ref&& ref) LN_NOEXCEPT
{
	m_ptr = ref.m_ptr;
	ref.m_ptr = nullptr;
}

template<class TItem>
template<class Y>
Ref<const Array<TItem>>::Ref(Ref<Y>&& ref) LN_NOEXCEPT
{
	m_ptr = static_cast<const Array<TItem>*>(ref.get());
	ref.m_ptr = nullptr;
}

template<class TItem>
Ref<const Array<TItem>>::~Ref()
{
	LN_SAFE_RELEASE(m_ptr);
}

template<class TItem>
void Ref<const Array<TItem>>::reset(const Array<TItem>* ptr, bool retain)
{
	if (ptr != m_ptr) {
		safeRelease();
		m_ptr = ptr;
		if (retain) {
			safeAddRef();
		}
	}
}

template<class TItem>
void Ref<const Array<TItem>>::reset()
{
	safeRelease();
}

template<class TItem>
void Ref<const Array<TItem>>::swap(Ref<const Array<TItem>>& other)
{
	if (&other != this) {
		const Array<TItem>* t = m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = t;
	}
}

template<class TItem>
const Array<TItem>* Ref<const Array<TItem>>::detach()
{
	RefObject* ptr = m_ptr;
	m_ptr = nullptr;
	return static_cast<const Array<TItem>*>(ptr);
}

template<class TItem>
const Array<TItem>* Ref<const Array<TItem>>::get() const
{
	return static_cast<const Array<TItem>*>(m_ptr);
}

template<class TItem>
Ref<const Array<TItem>>& Ref<const Array<TItem>>::operator=(const Ref<const Array<TItem>>& ref) LN_NOEXCEPT
{
	LN_REFOBJ_SET(m_ptr, ref.m_ptr);
	return *this;
}

template<class TItem>
template<class Y>
Ref<const Array<TItem>>& Ref<const Array<TItem>>::operator=(const Ref<Y>& ref) LN_NOEXCEPT
{
	LN_REFOBJ_SET(m_ptr, static_cast<const Array<TItem>*>(ref.get()));
	return *this;
}

template<class TItem>
Ref<const Array<TItem>>& Ref<const Array<TItem>>::operator=(Ref&& ref) LN_NOEXCEPT
{
	if (&ref != this) {
		LN_SAFE_RELEASE(m_ptr);
		m_ptr = ref.m_ptr;
		ref.m_ptr = nullptr;
	}
	return *this;
}

template<class TItem>
template<class Y>
Ref<const Array<TItem>>& Ref<const Array<TItem>>::operator=(Ref<Y>&& ref) LN_NOEXCEPT
{
	LN_SAFE_RELEASE(m_ptr);
	m_ptr = static_cast<const Array<TItem>*>(ref.get());
	ref.m_ptr = nullptr;
	return *this;
}

template<class TItem>
const Array<TItem>& Ref<const Array<TItem>>::operator*() const LN_NOEXCEPT
{
	assert(m_ptr != nullptr);
	return *static_cast<const Array<TItem>*>(m_ptr);
}

template<class TItem>
const Array<TItem>* Ref<const Array<TItem>>::operator->() const LN_NOEXCEPT
{
	assert(m_ptr != nullptr);
	return static_cast<const Array<TItem>*>(m_ptr);
}
#endif

TEST_F(Test_Base_Array, RefList)
{
	Ref<Array<int>> list1 = makeList({1, 2, 3});
	list1[0] = 10;
	list1[1] = 20;
	ASSERT_EQ(10, list1[0]);
	ASSERT_EQ(20, list1[1]);

	Ref<ReadOnlyArray<int>> list2 = list1;
	ASSERT_EQ(3, list2->size_internal());
	ASSERT_EQ(10, list2->at_internal(0));


    Ref<ReadOnlyArray<int>> rlist1 = list1;

    ASSERT_EQ(10, rlist1->front());
    ASSERT_EQ(10, rlist1[0]);
    Array<int> tmp;
    for (int v : rlist1) {
        tmp.add(v);
    }
    ASSERT_EQ(10, tmp[0]);
    ASSERT_EQ(20, tmp[1]);
    ASSERT_EQ(3, tmp[2]);



    //rlist1->front();

}

#endif
