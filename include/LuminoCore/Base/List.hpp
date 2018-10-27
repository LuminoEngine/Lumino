// Copyright (c) 2018 lriki. Distributed under the MIT license.
#pragma once

#include <vector>
#include <algorithm>
#include "Assertion.hpp"
#include "StlHelper.hpp"
#include "RefObject.hpp"
#include "Optional.hpp"

namespace ln {
template<typename T> class List;

/**
 * Ref クラスから List クラスへ読み取り専用アクセスする際に使用するインターフェイスです。
 */
template<typename TItem>
class ReadOnlyList : public RefObject
{
protected:
	ReadOnlyList() {}

public:	// TODO: internal}
	typedef typename std::vector<TItem>::iterator iterator;
	typedef typename std::vector<TItem>::const_iterator const_iterator;

	List<TItem>& derived() { return *static_cast<List<TItem>*>(this); }
	const List<TItem>& derived() const { return *static_cast<const List<TItem> *>(this); }
	int size_internal() const LN_NOEXCEPT { return derived().size(); }
	TItem& at_internal(int index) { return derived().at(index); }
	const TItem& at_internal(int index) const { return derived().at(index); }
	iterator begin_internal() LN_NOEXCEPT { return derived().begin(); }
	const_iterator begin_internal() const LN_NOEXCEPT { return derived().begin(); }
	iterator end_internal() LN_NOEXCEPT { return derived().end(); }
	const_iterator end_internal() const LN_NOEXCEPT { return derived().end(); }
};

/**
 * 参照カウントを持つ可変長配列のコンテナテンプレートクラスです。
 * 
 * インデックスに基づく高速アクセスとインデックスベースの挿入と削除などの機能を提供します。
 * std::vector とおなじように使用できますが、ヒープに確保された List のインスタンスはベースクラスの RefObject が持つ参照カウントによっても寿命を管理されます。
 */
template<typename T>
class List : public ReadOnlyList<T>
{
public:
    typedef typename std::vector<T>::value_type value_type;
    typedef typename std::vector<T>::difference_type difference_type;
    typedef typename std::vector<T>::pointer pointer;
    typedef typename std::vector<T>::const_pointer const_pointer;
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;
    typedef typename std::vector<T>::reference reference;
    typedef typename std::vector<T>::const_reference const_reference;

    /** 空の配列で新しい配列を構築します。 */
    List();

    /** 配列をコピーして新しい配列を構築します。 */
    List(const List& ary);

    /** 別の配列からムーブして構築します。 */
    List(List&& ary);

    /** 初期化子リストから構築します。 */
    List(std::initializer_list<T> list);

    /** イテレータの範囲からリストを構築します。 */
    template<class TIter>
    List(TIter begin, TIter end);

    /** デストラクタ */
    ~List();

    /** 別の List をこの List に割り当てます。*/
    List& operator=(const List& ary);

    /** 別の List をこの List に割り当てます。*/
    List& operator=(List&& other);

    /** 指定したインデックスにある要素への参照を取得します。*/
    T& operator[](int index);

    /** 指定したインデックスにある要素への参照を取得します。*/
    const T& operator[](int index) const;

    /** 配列が空であるかを確認します。*/
    bool isEmpty() const LN_NOEXCEPT;

    /** 格納されている要素の数を取得します。*/
    int size() const LN_NOEXCEPT;

    /** 追加のメモリ割り当てを行わずに追加できる要素の最大数を取得します。*/
    int capacity() const LN_NOEXCEPT;

    /** 指定したインデックスにある要素への参照を取得します。*/
    reference at(int index);

    /** 指定したインデックスにある要素への参照を取得します。*/
    const_reference at(int index) const;

    /** 先頭要素の参照を返します。*/
    reference front();

    /** 先頭要素の参照を返します。*/
    const_reference front() const;

    /** 終端要素の参照を返します。*/
    reference back();

    /** 終端要素の参照を返します。*/
    const_reference back() const;

    /** メモリを指定したサイズで確保します。 */
    void reserve(int count);

    /** 要素数を変更します。 */
    void resize(int count);

    /** デフォルト値を指定して、要素数を変更します。 */
    void resize(int count, const T& value);

    /** メモリ領域を現在の要素数まで切り詰めます。 */
    void shrinkToFit();

    /** 先頭要素を指すイテレータを取得します。 */
    iterator begin() LN_NOEXCEPT { return m_data.begin(); }

    /** 先頭要素を指すイテレータを取得します。 */
    const_iterator begin() const LN_NOEXCEPT { return m_data.begin(); }

    /** 末尾の次を指すイテレータを取得します。 */
    iterator end() LN_NOEXCEPT { return m_data.end(); }

    /** 末尾の次を指すイテレータを取得します。 */
    const_iterator end() const LN_NOEXCEPT { return m_data.end(); }

    /** 先頭要素を指す読み取り専用イテレータを取得します。 */
    const_iterator cbegin() const LN_NOEXCEPT { return m_data.cbegin(); }

    /** 末尾の次を指す読み取り専用イテレータを取得します。 */
    const_iterator cend() const LN_NOEXCEPT { return m_data.cend(); }

    /** 配列を初期化し、指定した範囲の要素をコピーします。*/
    template<typename TItr>
    void assign(TItr first, TItr last);

    /** 末尾に要素を追加します。*/
    void add(const value_type& item);

    /** 末尾に別の配列を連結します。*/
    void addRange(const List<T>& items);

    /** 指定したインデックスの位置に要素を挿入します。*/
    void insert(int index, const value_type& item);

    /** 指定したインデックスの位置に要素を挿入します。*/
    void insertRange(int index, const List<T>& items);

    /** 全ての要素を削除します。*/
    void clear();

    /** イテレータで示された位置の要素を削除し、次の要素を示すイテレータを返します。 */
    iterator erase(iterator pos);

    /** イテレータで示された位置の要素を削除し、次の要素を示すイテレータを返します。 */
    iterator erase(iterator begin, iterator end);

    /** item に一致する最初の要素を削除します。(正常に削除された場合は true を返す。要素が見つからなければ false を返す)*/
    bool remove(const value_type& item);

    /** 指定した条件に一致する最初の要素を削除します。(正常に削除された場合は true を返す。要素が見つからなければ false を返す)*/
    template<typename TPred>
    bool removeIf(TPred pred);

    /** item に一致する全ての要素を削除し、削除された要素数を返します。 */
    int removeAll(const value_type& item);

    /** 指定した条件に一致する全ての要素を削除し、削除された要素数を返します。 */
    template<typename TPred>
    int removeAllIf(TPred pred);

    /** 指定したインデックスにある要素を削除します。*/
    void removeAt(int index);

    /** 先頭要素を削除します。*/
    void removeFirst();

    /** 終端要素を削除します。*/
    void removeLast();

    /** 指定した要素がこの配列内に存在するかどうかを判断します。*/
    bool contains(const value_type& item) const;

    /** 指定した条件と一致する要素がこの配列内に存在するかどうかを判断します。*/
    template<typename TPred>
    bool containsIf(TPred pred) const;

    /**
     * 要素を指定した位置から検索し、最初に見つかったインデックスを返します。
     * @param[in]   item        : 検索する要素
     * @param[in]   startIndex  : 検索を開始するインデックス (省略した場合は先頭から)
     * @return      検索した要素が最初に現れた位置。見つからなかった場合は -1。
     */
    int indexOf(const value_type& item, int startIndex = 0) const;

    /**
     * 指定した条件と一致する最初のインデックスを返します。
     * @param[in]   pred        : 検索条件
     * @param[in]   startIndex  : 検索を開始するインデックス (省略した場合は先頭から)
     * @return      検索した要素が最初に現れた位置。見つからなかった場合は -1。
     */
    template<typename TPred>
    int indexOfIf(TPred pred, int startIndex = 0) const;

    /** 指定した要素と一致する最初の要素を検索し、その要素を指す Optional を返します。見つからなければ値を保持していません。 */
    Optional<T> find(const value_type& item) const;

    /** 指定した条件と一致する最初の要素を検索し、その要素を指す Optional を返します。見つからなければ値を保持していません。 */
    template<typename TPred>
    Optional<T> findIf(TPred pred) const;

    /** 指定したインデックスがこの配列の境界の範囲外かを確認します。*/
    bool isOutOfRange(int index) const;

private:
    std::vector<T> m_data;
};

template<typename T>
List<T>::List()
    : m_data()
{
}
template<typename T>
List<T>::List(const List& ary)
    : m_data(ary.m_data)
{
}

template<typename T>
List<T>::List(List&& ary)
    : m_data(std::move(ary.m_data))
{
}

template<typename T>
List<T>::List(std::initializer_list<T> list)
    : m_data(list)
{
}

template<typename T>
template<class TIter>
List<T>::List(TIter begin, TIter end)
    : m_data(begin, end)
{
}

template<typename T>
List<T>::~List()
{
}

template<typename T>
List<T>& List<T>::operator=(const List& ary)
{
    m_data = ary.m_data;
    return *this;
}

template<typename T>
List<T>& List<T>::operator=(List&& other)
{
    m_data = std::move(other.m_data);
    return *this;
}

template<typename T>
T& List<T>::operator[](int index)
{
    LN_FATAL(!isOutOfRange(index));
    return m_data[index];
}

template<typename T>
const T& List<T>::operator[](int index) const
{
    LN_FATAL(!isOutOfRange(index));
    return m_data[index];
}

template<typename T>
bool List<T>::isEmpty() const LN_NOEXCEPT
{
    return m_data.empty();
}

template<typename T>
int List<T>::size() const LN_NOEXCEPT
{
    return static_cast<int>(m_data.size());
}

template<typename T>
int List<T>::capacity() const LN_NOEXCEPT
{
    return m_data.capacity();
}

template<typename T>
typename List<T>::reference List<T>::at(int index)
{
    LN_FATAL(!isOutOfRange(index));
    return m_data.at(index);
}

template<typename T>
typename List<T>::const_reference List<T>::at(int index) const
{
    LN_FATAL(!isOutOfRange(index));
    return m_data.at(index);
}

template<typename T>
typename List<T>::reference List<T>::front()
{
    LN_FATAL(!isEmpty());
    return m_data.front();
}

template<typename T>
typename List<T>::const_reference List<T>::front() const
{
    LN_FATAL(!isEmpty());
    return m_data.front();
}

template<typename T>
typename List<T>::reference List<T>::back()
{
    LN_FATAL(!isEmpty());
    return m_data.back();
}

template<typename T>
typename List<T>::const_reference List<T>::back() const
{
    LN_FATAL(!isEmpty());
    return m_data.back();
}

template<typename T>
void List<T>::reserve(int count)
{
    m_data.reserve(count);
}

template<typename T>
void List<T>::resize(int count)
{
    m_data.resize(count);
}

template<typename T>
void List<T>::resize(int count, const T& value)
{
    m_data.resize(count, value);
}

template<typename T>
void List<T>::shrinkToFit()
{
    m_data.shrink_to_fit();
}

template<typename T>
template<typename TItr>
void List<T>::assign(TItr first, TItr last)
{
    m_data.assign(first, last);
}

template<typename T>
void List<T>::add(const value_type& item)
{
    m_data.push_back(item);
}

template<typename T>
void List<T>::addRange(const List<T>& items)
{
    m_data.insert(m_data.end(), items.m_data.begin(), items.m_data.end());
}

template<typename T>
void List<T>::insert(int index, const value_type& item)
{
    if (LN_REQUIRE(0 <= index && index <= size()))
        return;
    m_data.insert(m_data.begin() + index, item);
}

template<typename T>
void List<T>::insertRange(int index, const List<T>& items)
{
    if (LN_REQUIRE(0 <= index && index <= size()))
        return;
    m_data.insert(m_data.begin() + index, items.m_data.begin(), items.m_data.end());
}

template<typename T>
void List<T>::clear()
{
    m_data.clear();
}

template<typename T>
typename List<T>::iterator List<T>::erase(iterator pos)
{
    return m_data.erase(pos);
}

template<typename T>
typename List<T>::iterator List<T>::erase(iterator begin, iterator end)
{
    return m_data.erase(begin, end);
}

template<typename T>
bool List<T>::remove(const value_type& item)
{
    return detail::StlHelper::remove(m_data, item);
}

template<typename T>
template<typename TPred>
bool List<T>::removeIf(TPred pred)
{
    auto itr = m_data.begin();
    auto end = m_data.end();
    for (; itr != end; ++itr) {
        if (pred((*itr))) {
            m_data.erase(itr);
            return true;
        }
    }
    return false;
}

template<typename T>
int List<T>::removeAll(const value_type& item)
{
    return static_cast<int>(detail::StlHelper::removeAll(m_data, item));
}

template<typename T>
template<typename TPred>
int List<T>::removeAllIf(TPred pred)
{
    return static_cast<int>(detail::StlHelper::removeAll(m_data, pred));
}

template<typename T>
void List<T>::removeAt(int index)
{
    if (LN_REQUIRE(!isOutOfRange(index)))
        return;
    m_data.erase(m_data.begin() + index);
}

template<typename T>
void List<T>::removeFirst()
{
    if (LN_REQUIRE(!isEmpty()))
        return;
    removeAt(0);
}

template<typename T>
void List<T>::removeLast()
{
    if (LN_REQUIRE(!isEmpty()))
        return;
    removeAt(size() - 1);
}

template<typename T>
bool List<T>::contains(const value_type& item) const
{
    return std::find(m_data.begin(), m_data.end(), item) != m_data.end();
}

template<typename T>
template<typename TPred>
bool List<T>::containsIf(TPred pred) const
{
    return std::find_if(m_data.begin(), m_data.end(), pred) != m_data.end();
}

template<typename T>
int List<T>::indexOf(const value_type& item, int startIndex) const
{
    if (isEmpty())
        return -1;

    const_iterator itr = std::find(m_data.begin() + startIndex, m_data.end(), item);
    if (itr != m_data.end()) {
        return itr - m_data.begin();
    }
    return -1;
}

template<typename T>
template<typename TPred>
int List<T>::indexOfIf(TPred pred, int startIndex) const
{
    if (isEmpty())
        return -1;

    const_iterator itr = std::find_if(m_data.begin() + startIndex, m_data.end(), pred);
    if (itr != m_data.end()) {
        return itr - m_data.begin();
    }
    return -1;
}

template<typename T>
Optional<T> List<T>::find(const value_type& item) const
{
    auto itr = std::find(m_data.begin(), m_data.end(), item);
    if (itr != end()) {
        return *itr;
    }
    return nullptr;
}

template<typename T>
template<typename TPred>
Optional<T> List<T>::findIf(TPred pred) const
{
    auto itr = std::find_if(m_data.begin(), m_data.end(), pred);
    if (itr != end()) {
        return *itr;
    }
    return nullptr;
}

template<typename T>
bool List<T>::isOutOfRange(int index) const
{
    return (index < 0 || size() <= index);
}



//================================================================================

#if 1
/** RefObject 用 スマートポインタ */
template <typename TItem>
class Ref<List<TItem>>
{
public:
    /** null 値を表します。 */
    static const Ref<List<TItem>> Null;

    /** 参照を持たない空の Ref を構築します。 */
    LN_CONSTEXPR Ref() LN_NOEXCEPT;

    /** 参照を持たない空の Ref を構築します。 */
    LN_CONSTEXPR Ref(std::nullptr_t) LN_NOEXCEPT;

    /** 生ポインタの所有権を受け取ります。 */
    Ref(List<TItem>* ptr);

    /** 生ポインタの所有権を受け取ります。retain が false の場合、参照カウントをインクリメントせずに参照します。 */
    Ref(List<TItem>* ptr, bool retain);

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
    void reset(List<TItem>* ptr, bool retain = true);

    /** 保持しているオブジェクトの参照を放棄します。 */
    void reset();

    /** 2つの Ref オブジェクトを入れ替えます。 */
    void swap(Ref<List<TItem>>& other);

    /** 保持しているオブジェクトの参照を放棄します。参照カウントはデクリメントしません。 */
    List<TItem>* detach();

    /** 保持しているオブジェクトへのポインタを取得します。 */
    List<TItem>* get() const;

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
    List<TItem>& operator*() const LN_NOEXCEPT;

    /** ポインタを通してオブジェクトにアクセスします。 */
    List<TItem>* operator->() const LN_NOEXCEPT;

    /** 有効なポインタを保持しているかを確認します。 */
    explicit operator bool() const LN_NOEXCEPT { return (m_ptr != nullptr); }

    /** オブジェクトのポインタへの変換をサポートします。 */
    operator List<TItem>*() const { return static_cast<List<TItem>*>(m_ptr); } // ここでコンパイルエラーとなる場合、List<TItem> の定義があるヘッダファイルを include しているか確認すること。

    RefObject* basePointer() const { return m_ptr; }






    /** 指定したインデックスにある要素への参照を取得します。*/
    TItem& operator[](int index) { return m_ptr->operator[](index); }

    /** 指定したインデックスにある要素への参照を取得します。*/
    const TItem& operator[](int index) const { return m_ptr->operator[](index); }

    typedef typename std::vector<TItem>::iterator iterator;
    typedef typename std::vector<TItem>::const_iterator const_iterator;

    /** 先頭要素を指すイテレータを取得します。 */
    iterator begin() LN_NOEXCEPT { return m_ptr->begin(); }

    /** 先頭要素を指すイテレータを取得します。 */
    const_iterator begin() const LN_NOEXCEPT { return m_ptr->begin(); }

    /** 末尾の次を指すイテレータを取得します。 */
    iterator end() LN_NOEXCEPT { return m_ptr->end(); }

    /** 末尾の次を指すイテレータを取得します。 */
    const_iterator end() const LN_NOEXCEPT { return m_ptr->end(); }

protected:
    void safeAddRef()
    {
        LN_SAFE_RETAIN(m_ptr);
    }

    void safeRelease()
    {
        LN_SAFE_RELEASE(m_ptr);
    }

    List<TItem>* m_ptr;

    template<class U> friend class Ref;
};

//template<class TItem>
//const Ref<List<TItem>> Ref<List<TItem>>::Null;
//
template<class TItem>
LN_CONSTEXPR Ref<List<TItem>>::Ref() LN_NOEXCEPT
    : m_ptr(nullptr)
{
}

template<class TItem>
LN_CONSTEXPR Ref<List<TItem>>::Ref(std::nullptr_t) LN_NOEXCEPT
    : m_ptr(nullptr)
{
}

template<class TItem>
Ref<List<TItem>>::Ref(List<TItem>* ptr)
    : Ref(ptr, true)
{
}

template<class TItem>
Ref<List<TItem>>::Ref(List<TItem>* ptr, bool retain)
    : m_ptr(ptr)
{
    if (retain) {
        LN_SAFE_RETAIN(m_ptr);
    }
}

template<class TItem>
Ref<List<TItem>>::Ref(const Ref& ref) LN_NOEXCEPT
    : m_ptr(ref.m_ptr)
{
    LN_SAFE_RETAIN(m_ptr);
}

template<class TItem>
template<class Y>
Ref<List<TItem>>::Ref(const Ref<Y>& ref) LN_NOEXCEPT
    : m_ptr(static_cast<List<TItem>*>(ref.get()))
{
    LN_SAFE_RETAIN(m_ptr);
}

template<class TItem>
Ref<List<TItem>>::Ref(Ref&& ref) LN_NOEXCEPT
{
    m_ptr = ref.m_ptr;
    ref.m_ptr = nullptr;
}

template<class TItem>
template<class Y>
Ref<List<TItem>>::Ref(Ref<Y>&& ref) LN_NOEXCEPT
{
    m_ptr = static_cast<List<TItem>*>(ref.get());
    ref.m_ptr = nullptr;
}

template<class TItem>
Ref<List<TItem>>::~Ref()
{
    LN_SAFE_RELEASE(m_ptr);
}

template<class TItem>
void Ref<List<TItem>>::reset(List<TItem>* ptr, bool retain)
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
void Ref<List<TItem>>::reset()
{
    safeRelease();
}

template<class TItem>
void Ref<List<TItem>>::swap(Ref<List<TItem>>& other)
{
    if (&other != this) {
        List<TItem>* t = m_ptr;
        m_ptr = other.m_ptr;
        other.m_ptr = t;
    }
}

template<class TItem>
List<TItem>* Ref<List<TItem>>::detach()
{
    RefObject* ptr = m_ptr;
    m_ptr = nullptr;
    return static_cast<List<TItem>*>(ptr);
}

template<class TItem>
List<TItem>* Ref<List<TItem>>::get() const
{
    return static_cast<List<TItem>*>(m_ptr);
}

template<class TItem>
Ref<List<TItem>>& Ref<List<TItem>>::operator=(const Ref<List<TItem>>& ref) LN_NOEXCEPT
{
    LN_REFOBJ_SET(m_ptr, ref.m_ptr);
    return *this;
}

template<class TItem>
template<class Y>
Ref<List<TItem>>& Ref<List<TItem>>::operator=(const Ref<Y>& ref) LN_NOEXCEPT
{
    LN_REFOBJ_SET(m_ptr, static_cast<List<TItem>*>(ref.get()));
    return *this;
}

template<class TItem>
Ref<List<TItem>>& Ref<List<TItem>>::operator=(Ref&& ref) LN_NOEXCEPT
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
Ref<List<TItem>>& Ref<List<TItem>>::operator=(Ref<Y>&& ref) LN_NOEXCEPT
{
    LN_SAFE_RELEASE(m_ptr);
    m_ptr = static_cast<List<TItem>*>(ref.get());
    ref.m_ptr = nullptr;
    return *this;
}

template<class TItem>
List<TItem>& Ref<List<TItem>>::operator*() const LN_NOEXCEPT
{
    assert(m_ptr != nullptr);
    return *static_cast<List<TItem>*>(m_ptr);
}

template<class TItem>
List<TItem>* Ref<List<TItem>>::operator->() const LN_NOEXCEPT
{
    assert(m_ptr != nullptr);
    return static_cast<List<TItem>*>(m_ptr);
}

/** Ref を構築します。受け取った引数リストを型 T のコンストラクタへ渡してオブジェクトを構築します。 */
template<class TItem>
inline Ref<List<TItem>> makeList(std::initializer_list<TItem> list)
{
    return Ref<List<TItem>>(LN_NEW List<TItem>(std::forward<std::initializer_list<TItem>>(list)), false);
}

#endif



} // namespace ln
