// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once
#include <vector>
#include <algorithm>
#include "Assertion.hpp"
#include "StlHelper.hpp"
#include "Optional.hpp"

namespace ln {

/**
 * 動的配列
 * 
 * std::vector と同様、メモリの連続性が保証されています。
 * std::vector と似ていますが、いくつかの便利なメンバ関数やエラーを記録する機能が追加されています。
 * インデックスは冗長なコンパイル警告や逆順ループの記述ミスを防止するため、size_t ではなく int となっています。
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
    /** @defgroup  Element acces: */
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

private:
    container_type m_data;
};

} // namespace ln

#include "Array.ipp"
