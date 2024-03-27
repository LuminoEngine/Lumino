// Copyright (c) 2018+ lriki. Distributed under the MIT license.
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
public:
    typedef typename std::vector<TItem>::iterator iterator;
    typedef typename std::vector<TItem>::const_iterator const_iterator;
    typedef typename std::vector<TItem>::reference reference;
    typedef typename std::vector<TItem>::const_reference const_reference;

	/** 配列が空であるかを確認します。*/
	bool isEmpty() const LN_NOEXCEPT { return size_internal() == 0; }

	/** 格納されている要素の数を取得します。*/
	int size() const LN_NOEXCEPT { return size_internal(); }

    /** 先頭要素の参照を返します。*/
    reference front() { return at_internal(0); }

    /** 先頭要素の参照を返します。*/
    const_reference front() const { return at_internal(0); }

    /** 終端要素の参照を返します。*/
    reference back() { return at_internal(size_internal() - 1); }

    /** 終端要素の参照を返します。*/
    const_reference back() const { return at_internal(size_internal() - 1); }

    /** 指定したインデックスにある要素への参照を取得します。*/
    TItem& operator[](int index) { return at_internal(index); }

    /** 指定したインデックスにある要素への参照を取得します。*/
    const TItem& operator[](int index) const { return at_internal(index); }

    /** 先頭要素を指すイテレータを取得します。 */
    iterator begin() LN_NOEXCEPT { return begin_internal(); }

    /** 先頭要素を指すイテレータを取得します。 */
    const_iterator begin() const LN_NOEXCEPT { return begin_internal(); }

    /** 末尾の次を指すイテレータを取得します。 */
    iterator end() LN_NOEXCEPT { return end_internal(); }

    /** 末尾の次を指すイテレータを取得します。 */
    const_iterator end() const LN_NOEXCEPT { return end_internal(); }

protected:
	ReadOnlyList() {}

public:	// TODO: internal}

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

    template<typename TPred>
    int lastIndexOfIf(TPred pred) const;

    /** 指定した要素と一致する最初の要素を検索し、その要素を指す Optional を返します。見つからなければ値を保持していません。 */
    Optional_deprecated<T> find(const value_type& item) const;

    /** 指定した条件と一致する最初の要素を検索し、その要素を指す Optional を返します。見つからなければ値を保持していません。 */
    template<typename TPred>
    Optional_deprecated<T> findIf(TPred pred) const;

    /** 指定したインデックスがこの配列の境界の範囲外かを確認します。*/
    bool isOutOfRange(int index) const;

    /** 指定したインデックスがこの配列の境界の範囲内かを確認します。*/
    bool isValidRange(int index) const;

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
	LN_CHECK(!isOutOfRange(index));
    return m_data[index];
}

template<typename T>
const T& List<T>::operator[](int index) const
{
	LN_CHECK(!isOutOfRange(index));
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
    return static_cast<int>(m_data.capacity());
}

template<typename T>
typename List<T>::reference List<T>::at(int index)
{
	LN_CHECK(!isOutOfRange(index));
    return m_data.at(index);
}

template<typename T>
typename List<T>::const_reference List<T>::at(int index) const
{
	LN_CHECK(!isOutOfRange(index));
    return m_data.at(index);
}

template<typename T>
typename List<T>::reference List<T>::front()
{
	LN_CHECK(!isEmpty());
    return m_data.front();
}

template<typename T>
typename List<T>::const_reference List<T>::front() const
{
	LN_CHECK(!isEmpty());
    return m_data.front();
}

template<typename T>
typename List<T>::reference List<T>::back()
{
	LN_CHECK(!isEmpty());
    return m_data.back();
}

template<typename T>
typename List<T>::const_reference List<T>::back() const
{
	LN_CHECK(!isEmpty());
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
        return static_cast<int>(itr - m_data.begin());
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
        return static_cast<int>(itr - m_data.begin());
    }
    return -1;
}

template<typename T>
template<typename TPred>
int List<T>::lastIndexOfIf(TPred pred) const
{
    if (isEmpty()) return -1;

    for (int i = size() - 1; i >= 0; i--) {
        if (pred(m_data[i])) return i;
    }
    return -1;
}

template<typename T>
Optional_deprecated<T> List<T>::find(const value_type& item) const
{
    auto itr = std::find(m_data.begin(), m_data.end(), item);
    if (itr != end()) {
        return *itr;
    }
    return std::nullopt;
}

template<typename T>
template<typename TPred>
Optional_deprecated<T> List<T>::findIf(TPred pred) const
{
    auto itr = std::find_if(m_data.begin(), m_data.end(), pred);
    if (itr != end()) {
        return *itr;
    }
    return std::nullopt;
}

template<typename T>
bool List<T>::isOutOfRange(int index) const
{
    return (index < 0 || size() <= index);
}

template<typename T>
bool List<T>::isValidRange(int index) const
{
    return (0 <= index && index < size());
}


//================================================================================

#define LN_SPECIALIZED_TYPE List<TItem>
#include <LuminoCore/Base/Ref.inl>
#undef LN_SPECIALIZED_TYPE

#define LN_SPECIALIZED_TYPE ReadOnlyList<TItem>
#include <LuminoCore/Base/Ref.inl>
#undef LN_SPECIALIZED_TYPE

/** Ref<List> を構築します。 */
template<class TItem>
inline Ref<List<TItem>> makeList()
{
    return Ref<List<TItem>>(LN_NEW List<TItem>(), false);
}

/** Ref<List> を構築します。受け取った引数リストを要素としてオブジェクトを構築します。 */
template<class TItem>
inline Ref<List<TItem>> makeList(std::initializer_list<TItem> list)
{
    return Ref<List<TItem>>(LN_NEW List<TItem>(std::forward<std::initializer_list<TItem>>(list)), false);
}

} // namespace ln
