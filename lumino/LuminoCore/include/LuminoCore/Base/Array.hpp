// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once
#include <vector>
#include <algorithm>
#include <optional>
#include "Assertion.hpp"
#include "StlHelper.hpp"
#include "Optional.hpp"
#include "Optional2.hpp"

namespace ln {

/**
 * 動的配列
 * 
 * std::vector と同様、メモリの連続性が保証されています。
 * std::vector と似ていますが、いくつかの便利なメンバ関数やエラーを記録する機能が追加されています。
 * インデックスは冗長なコンパイル警告や逆順ループの記述ミスの対策のため、size_t ではなく int となっています。
 */
template<typename T>
class Array
{
public:
    using container_type = std::vector<T>;
    using value_type = typename container_type::value_type;
    using pointer = typename container_type::pointer;
    using const_pointer = typename container_type::const_pointer;
    using reference = typename container_type::reference;
    using const_reference = typename container_type::const_reference;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;
    using reverse_iterator = typename container_type::reverse_iterator;
    using const_reverse_iterator = typename container_type::const_reverse_iterator;
    using size_type = int;
    using index_type = int32_t;
    using difference_type = typename container_type::difference_type;
    using allocator_type = typename container_type::allocator_type;


    /** Construct empty array. */
    Array() = default;

    /** Copy the array and Constructs a new array. */
    Array(const Array& ary) = default;

    /** Copy the array and Constructs a new array. */
    Array(const container_type& ary);

    /** Constructor with move. */
    Array(Array&& ary) = default;

    /** Constructor with move. */
    Array(container_type&& ary);

    /** Constructor with initializer_list. */
    Array(std::initializer_list<T> list);

    /** Range constructor. */
    template<class TIter>
    Array(TIter begin, TIter end);

    /** Range constructor. */
    template<class TRange>
    Array(TRange&& range);

    /** Destructor.  */
    ~Array() = default;

    /** Assign content. */
    Array& operator=(const Array&) = default;

    /** Assign content. */
    Array& operator=(Array&&) = default;

    /** Assign content. */
    Array& operator=(const container_type& other);

    /** Assign content. */
    Array& operator=(container_type&& other);

    /** Assign content. */
    Array& operator=(std::initializer_list<value_type> list);

    /** @defgroup Iterators: */
    /** @{ */

    [[nodiscard]]
    iterator begin() noexcept;

    [[nodiscard]]
    iterator end() noexcept;

    [[nodiscard]]
    const_iterator begin() const noexcept;

    [[nodiscard]]
    const_iterator end() const noexcept;

    [[nodiscard]]
    const_iterator cbegin() const noexcept;

    [[nodiscard]]
    const_iterator cend() const noexcept;

    /** @} */
    /** @defgroup Capacity */
    /** @{ */

    /** Return size. */
    [[nodiscard]]
    int size() const noexcept;

    /** Return maximum size. */
    [[nodiscard]]
    size_t max_size() const noexcept;

    /** Return size of allocated storage capacity. */
    [[nodiscard]]
    size_t capacity() const noexcept;

    /** Change size. */
    void resize(size_type count);

    /** Change size. */
    void resize(size_type count, const value_type& value);

    /** Request a change in capacity. */
    void reserve(size_type new_capacity);

    /** Truncate the area to the size of the container. */
    void shrink_to_fit();

    /** Test whether vector is empty. */
    [[nodiscard]]
    bool empty() const noexcept;

    /** Test whether vector is empty. */
    [[nodiscard]]
    bool isEmpty() const noexcept;

    /** Test whether vector is empty. */
    [[nodiscard]]
    explicit operator bool() const noexcept;

    /** @} */
    /** @defgroup Element access */
    /** @{ */

    /** Access the element at the specified index. */
    [[nodiscard]]
    T& at(int index);

    /** Access the element at the specified index. */
    [[nodiscard]]
    const T& at(int index) const;

    /** Access the element at the specified index. */
    [[nodiscard]]
    T& operator[](int index);

    /** Access the element at the specified index. */
    [[nodiscard]]
    const T& operator[](int index) const;

    /** Access first element. */
    [[nodiscard]]
    reference front();

    /** Access first element. */
    [[nodiscard]]
    const_reference front() const;

    /** Access last element. */
    [[nodiscard]]
    reference back();

    /** Access last element. */
    [[nodiscard]]
    const_reference back() const;

    /** Access raw-data. */
    [[nodiscard]]
    T* data() noexcept;

    /** Access raw-data. */
    [[nodiscard]]
    const T* data() const noexcept;

    /** @} */
    /** @defgroup Modifiers */
    /** @{ */

    /** Assign array content. */
    void assign(int count, const T& value);

    /** Assign array content. */
    template <class TIterator>
    void assign(TIterator first, TIterator last);

    /** Assign array content. */
    void assign(std::initializer_list<T> ilist);

    /** Add element to the end. */
    void push_back(const T& value);

    /** Add element to the end. */
    void push_back(T&& value);

    /** Add element to the end. */
    void push(const T& value);

    /** Add element to the end. */
    void push(T&& value);

    /** Remove the last element. */
    void pop_back();

    /** Insert a new element at specified position. */
    iterator insert(const_iterator pos, const T& value);

    /** Insert a new element at specified position. */
    iterator insert(const_iterator pos, T&& value);

    /** Insert a new element at specified position. */
    iterator insert(const_iterator pos, int count, const T& value);

    /** Insert a new element at specified position. */
    template <class Iterator>
    iterator insert(const_iterator pos, Iterator first, Iterator last);

    /** Insert a new element at specified position. */
    iterator insert(const_iterator pos, std::initializer_list<T> list);

    /** Remove an element at the specified position. */
    iterator erase(const_iterator pos);

    /** Remove an element at the specified position. */
    iterator erase(const_iterator first, const_iterator last);

    /** Swap containers. */
    void swap(Array& other) noexcept;

    /** Removes all elements. */
    void clear() noexcept;

    /** Construct and insert element. */
    template<class... Args>
    iterator emplace(const_iterator pos, Args&&... args);

    /** Construct and insert element at the end. */
    template <class... Args>
    reference emplace_back(Args&&... args);

    /** @} */
    /** @defgroup Extensions */
    /** @{ */

    /** 指定した条件と一致する最初の要素を検索し、その要素を指す Optional を返します。見つからなければ値を保持していません。 */
    template<typename TPred>
    Optional2<const T&> findIf(TPred pred) const
    {
        auto itr = std::find_if(m_data.begin(), m_data.end(), pred);
        if (itr != end()) {
            return Optional2<const T&>(*itr);
        }
        return Optional2<const T&>();
    }
    template<typename TPred>
    Optional2<T&> findIf(TPred pred)
    {
        auto itr = std::find_if(m_data.begin(), m_data.end(), pred);
        if (itr != end()) {
            return Optional2<T&>(*itr);
        }
        return Optional2<T&>();
    }


    /**
     * 指定した条件と一致する最初のインデックスを返します。
     * @param[in]   pred        : 検索条件
     * @param[in]   startIndex  : 検索を開始するインデックス (省略した場合は先頭から)
     * @return      検索した要素が最初に現れた位置。見つからなかった場合は -1。
     */
    template<typename TPred>
    int indexOfIf(TPred pred, int startIndex = 0) const;

    /** 指定した要素がこの配列内に存在するかどうかを判断します。*/
    bool contains(const value_type& item) const;

    /** 指定した条件と一致する要素がこの配列内に存在するかどうかを判断します。*/
    template<typename TPred>
    bool containsIf(TPred pred) const;

    /** Insert a new element at specified position. */
    iterator insert(int pos, const T& value);

    /** Insert a new element at specified position. */
    iterator insert(int pos, T&& value);

    /** Insert a new element at specified position. */
    iterator insert(int pos, int count, const T& value);

    /** Insert a new element at specified position. */
    template <class Iterator>
    iterator insert(int pos, Iterator first, Iterator last);

    /** Insert a new element at specified position. */
    iterator insert(int pos, std::initializer_list<T> list);

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

    /** @} */

    
    template<class TCallback>
    auto map(TCallback callback) -> Array<decltype(callback(*reinterpret_cast<T*>(0)))> {
        Array<decltype(callback(*reinterpret_cast<T*>(0)))> result = {};
        for (size_t i = 0; i < m_data.size(); i++) {
            result.push(callback(m_data[i]));
        }
        return result;
    }

    template<class TCallback>
    auto map(TCallback callback) const -> Array<decltype(callback(*reinterpret_cast<T*>(0)))>
    {
        Array<decltype(callback(*reinterpret_cast<T*>(0)))> result = {};
        for (size_t i = 0; i < m_data.size(); i++) {
            result.push(callback(m_data[i]));
        }
        return result;
    }

    /** 重複を削除した新しい配列を返します。 */
    template<typename TPred>
    Array<T> distinct(TPred predicate) const {
        ln::Array<T> result = *this;
        std::sort(result.begin(), result.end());
        result.erase(std::unique(result.begin(), result.end(), predicate), result.end());
        return result;
    }

    /** この配列と指定した配列を連結した新しい配列を返します。 */
    Array<T> concat(const Array<T>& ary) const {
        ln::Array<T> result = *this;
        for (const auto& i : ary) {
            result.push(i);
        }
        return result;
    }

    /** 配列のそれぞれの要素に対してコールバック関数を適用します。その際、直前の要素における計算結果の返値を渡し、最後の結果を返します。 */
    template<class T, class TFunc>
    T fold(T initialValue, TFunc func) const {
        for (const auto& i : *this) {
            initialValue = func(std::move(initialValue), i);
        }
        return initialValue;
    }

    /** 指定したインデックスがこの配列の境界の範囲外かを確認します。*/
    bool isOutOfRange(int index) const;

    /** 指定したインデックスがこの配列の境界の範囲内かを確認します。*/
    bool isValidRange(int index) const;

private:
    container_type m_data;
    template<class T>
    friend class Array;
};

template<class T>
[[nodiscard]]
inline bool operator==(const Array<T>& a, const Array<T>& b);

template<class T>
[[nodiscard]]
inline bool operator!=(const Array<T>& a, const Array<T>& b);

template<class T>
[[nodiscard]]
inline bool operator<(const Array<T>& a, const Array<T>& b);

template<class T>
[[nodiscard]]
inline bool operator<=(const Array<T>& a, const Array<T>& b);

template<class T>
[[nodiscard]]
inline bool operator>(const Array<T>& a, const Array<T>& b);

template<class T>
[[nodiscard]]
inline bool operator>=(const Array<T>& a, const Array<T>& b);

template<class T>
inline void swap(Array<T>& a, Array<T>& b) noexcept;

} // namespace ln

#include "Array.ipp"
