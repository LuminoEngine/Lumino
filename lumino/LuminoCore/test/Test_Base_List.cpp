#include "Common.hpp"
#include <LuminoCore/Base/List.hpp>

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


//template<>
//template <typename TItem>
//class Ref<List<TItem>>
//{
//public:
//};

//template<typename List<TItem>, typename S>
//class SomeRandomClass
//{
//	//put something here
//};
//
//template<typename List<TItem>>
//class MyTemplateClass
//{
//	void DoSomething(List<TItem> & t) {
//		//...something
//	}
//};
//
//template <>
//template <typename List<TItem>, typename S>
//class MyTemplateClass <SomeRandomClass<List<TItem>, S> >
//{
//	void DoSomething(SomeRandomClass<List<TItem>, S>& t);
//};
//
//template <>
//template <typename List<TItem>, typename S>
//void MyTemplateClass<SomeRandomClass<List<TItem>, S> >::DoSomething(SomeRandomClass<List<TItem>, S>& t)
//{
//	// something
//}


#if 0
/** RefObject 用 スマートポインタ */
template <typename TItem>
class Ref<const List<TItem>>
{
public:
	/** null 値を表します。 */
	static const Ref<const List<TItem>> Null;

	/** 参照を持たない空の Ref を構築します。 */
	LN_CONSTEXPR Ref() LN_NOEXCEPT;

	/** 参照を持たない空の Ref を構築します。 */
	LN_CONSTEXPR Ref(std::nullptr_t) LN_NOEXCEPT;

	/** 生ポインタの所有権を受け取ります。 */
	Ref(const List<TItem>* ptr);

	/** 生ポインタの所有権を受け取ります。retain が false の場合、参照カウントをインクリメントせずに参照します。 */
	Ref(const List<TItem>* ptr, bool retain);

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
	void reset(const List<TItem>* ptr, bool retain = true);

	/** 保持しているオブジェクトの参照を放棄します。 */
	void reset();

	/** 2つの Ref オブジェクトを入れ替えます。 */
	void swap(Ref<const List<TItem>>& other);

	/** 保持しているオブジェクトの参照を放棄します。参照カウントはデクリメントしません。 */
	const List<TItem>* detach();

	/** 保持しているオブジェクトへのポインタを取得します。 */
	const List<TItem>* get() const;

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
	const List<TItem>& operator*() const LN_NOEXCEPT;

	/** ポインタを通してオブジェクトにアクセスします。 */
	const List<TItem>* operator->() const LN_NOEXCEPT;

	/** 有効なポインタを保持しているかを確認します。 */
	explicit operator bool() const LN_NOEXCEPT { return (m_ptr != nullptr); }

	/** オブジェクトのポインタへの変換をサポートします。 */
	operator const List<TItem>*() const { return static_cast<const List<TItem>*>(m_ptr); } // ここでコンパイルエラーとなる場合、const List<TItem> の定義があるヘッダファイルを include しているか確認すること。

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

	const List<TItem>* m_ptr;

	template<class U> friend class Ref;
};

//template<class TItem>
//const Ref<const List<TItem>> Ref<const List<TItem>>::Null;
//
template<class TItem>
LN_CONSTEXPR Ref<const List<TItem>>::Ref() LN_NOEXCEPT
	: m_ptr(nullptr)
{
}

template<class TItem>
LN_CONSTEXPR Ref<const List<TItem>>::Ref(std::nullptr_t) LN_NOEXCEPT
	: m_ptr(nullptr)
{
}

template<class TItem>
Ref<const List<TItem>>::Ref(const List<TItem>* ptr)
	: Ref(ptr, true)
{
}

template<class TItem>
Ref<const List<TItem>>::Ref(const List<TItem>* ptr, bool retain)
	: m_ptr(ptr)
{
	if (retain) {
		LN_SAFE_RETAIN(m_ptr);
	}
}

template<class TItem>
Ref<const List<TItem>>::Ref(const Ref& ref) LN_NOEXCEPT
	: m_ptr(ref.m_ptr)
{
	LN_SAFE_RETAIN(m_ptr);
}

template<class TItem>
template<class Y>
Ref<const List<TItem>>::Ref(const Ref<Y>& ref) LN_NOEXCEPT
	: m_ptr(static_cast<const List<TItem>*>(ref.get()))
{
	LN_SAFE_RETAIN(m_ptr);
}

template<class TItem>
Ref<const List<TItem>>::Ref(Ref&& ref) LN_NOEXCEPT
{
	m_ptr = ref.m_ptr;
	ref.m_ptr = nullptr;
}

template<class TItem>
template<class Y>
Ref<const List<TItem>>::Ref(Ref<Y>&& ref) LN_NOEXCEPT
{
	m_ptr = static_cast<const List<TItem>*>(ref.get());
	ref.m_ptr = nullptr;
}

template<class TItem>
Ref<const List<TItem>>::~Ref()
{
	LN_SAFE_RELEASE(m_ptr);
}

template<class TItem>
void Ref<const List<TItem>>::reset(const List<TItem>* ptr, bool retain)
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
void Ref<const List<TItem>>::reset()
{
	safeRelease();
}

template<class TItem>
void Ref<const List<TItem>>::swap(Ref<const List<TItem>>& other)
{
	if (&other != this) {
		const List<TItem>* t = m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = t;
	}
}

template<class TItem>
const List<TItem>* Ref<const List<TItem>>::detach()
{
	RefObject* ptr = m_ptr;
	m_ptr = nullptr;
	return static_cast<const List<TItem>*>(ptr);
}

template<class TItem>
const List<TItem>* Ref<const List<TItem>>::get() const
{
	return static_cast<const List<TItem>*>(m_ptr);
}

template<class TItem>
Ref<const List<TItem>>& Ref<const List<TItem>>::operator=(const Ref<const List<TItem>>& ref) LN_NOEXCEPT
{
	LN_REFOBJ_SET(m_ptr, ref.m_ptr);
	return *this;
}

template<class TItem>
template<class Y>
Ref<const List<TItem>>& Ref<const List<TItem>>::operator=(const Ref<Y>& ref) LN_NOEXCEPT
{
	LN_REFOBJ_SET(m_ptr, static_cast<const List<TItem>*>(ref.get()));
	return *this;
}

template<class TItem>
Ref<const List<TItem>>& Ref<const List<TItem>>::operator=(Ref&& ref) LN_NOEXCEPT
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
Ref<const List<TItem>>& Ref<const List<TItem>>::operator=(Ref<Y>&& ref) LN_NOEXCEPT
{
	LN_SAFE_RELEASE(m_ptr);
	m_ptr = static_cast<const List<TItem>*>(ref.get());
	ref.m_ptr = nullptr;
	return *this;
}

template<class TItem>
const List<TItem>& Ref<const List<TItem>>::operator*() const LN_NOEXCEPT
{
	assert(m_ptr != nullptr);
	return *static_cast<const List<TItem>*>(m_ptr);
}

template<class TItem>
const List<TItem>* Ref<const List<TItem>>::operator->() const LN_NOEXCEPT
{
	assert(m_ptr != nullptr);
	return static_cast<const List<TItem>*>(m_ptr);
}
#endif

TEST_F(Test_Base_List, RefList)
{
	Ref<List<int>> list1 = makeList({1, 2, 3});
	list1[0] = 10;
	list1[1] = 20;
	ASSERT_EQ(10, list1[0]);
	ASSERT_EQ(20, list1[1]);

	Ref<ReadOnlyList<int>> list2 = list1;
	ASSERT_EQ(3, list2->size_internal());
	ASSERT_EQ(10, list2->at_internal(0));


    Ref<ReadOnlyList<int>> rlist1 = list1;

    ASSERT_EQ(10, rlist1->front());
    ASSERT_EQ(10, rlist1[0]);
    List<int> tmp;
    for (int v : rlist1) {
        tmp.add(v);
    }
    ASSERT_EQ(10, tmp[0]);
    ASSERT_EQ(20, tmp[1]);
    ASSERT_EQ(3, tmp[2]);



    //rlist1->front();

}

