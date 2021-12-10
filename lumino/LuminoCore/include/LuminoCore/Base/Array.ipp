// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once

namespace ln {

template<class T>
inline Array<T>::Array(const container_type& other)
    : m_data(other)
{}

template<class T>
inline Array<T>::Array(container_type&& other)
    : m_data(std::move(other))
{}

template<class T>
inline Array<T>::Array(std::initializer_list<T> list)
    : m_data(list)
{
}

template<class T>
template<class TIter>
Array<T>::Array(TIter begin, TIter end)
    : m_data(begin, end)
{}

template<class T>
Array<T>& Array<T>::operator=(const container_type& other)
{
    m_data = other;
    return *this;
}

template<class T>
Array<T>& Array<T>::operator=(container_type&& other)
{
    m_data = std::move(other);
    return *this;
}

template<class T>
Array<T>& Array<T>::operator=(std::initializer_list<value_type> list)
{
    m_data = list;
    return *this;
}



template<class T>
inline typename Array<T>::iterator Array<T>::begin() noexcept
{
    return m_data.begin();
}

template<class T>
inline typename Array<T>::iterator Array<T>::end() noexcept
{
    return m_data.end();
}

template<class T>
inline typename Array<T>::const_iterator Array<T>::begin() const noexcept
{
    return m_data.begin();
}

template<class T>
inline typename Array<T>::const_iterator Array<T>::end() const noexcept
{
    return m_data.end();
}

template<class T>
inline typename Array<T>::const_iterator Array<T>::cbegin() const noexcept
{
    return m_data.cbegin();
}

template<class T>
inline typename Array<T>::const_iterator Array<T>::cend() const noexcept
{
    return m_data.cend();
}



template<class T>
inline int Array<T>::size() const noexcept
{
    return static_cast<int>(m_data.size());
}

template<class T>
inline size_t Array<T>::max_size() const noexcept
{
    return std::numeric_limits<int>::max();
}

template<class T>
inline size_t Array<T>::capacity() const noexcept
{
    return m_data.capacity();
}

template<class T>
inline void Array<T>::resize(size_type count)
{
    m_data.resize(count);
}

template<class T>
inline void Array<T>::resize(size_type count, const value_type& value)
{
    m_data.resize(count, value);
}

template<class T>
inline void Array<T>::reserve(size_type new_capacity)
{
    m_data.reserve(new_capacity);
}

template<class T>
inline void Array<T>::shrink_to_fit()
{
    m_data.shrink_to_fit();
}

template<class T>
inline bool Array<T>::empty() const noexcept
{
    return m_data.empty();
}

template<class T>
inline bool Array<T>::isEmpty() const noexcept
{
    return m_data.empty();
}

template<class T>
inline Array<T>::operator bool() const noexcept
{
    return !m_data.empty();
}




template<class T>
inline typename T& Array<T>::at(int index)
{
    return m_data.at(index);
}

template<class T>
inline const T& Array<T>::at(int index) const
{
    return m_data.at(index);
}

template<class T>
inline T& Array<T>::operator[](int index)
{
    return m_data[index];
}

template<class T>
inline const T& Array<T>::operator[](int index) const
{
    return m_data[index];
}

template<class T>
inline typename Array<T>::reference Array<T>::front()
{
    return m_data.front();
}

template<class T>
inline typename Array<T>::const_reference Array<T>::front() const
{
    return m_data.front();
}

template<class T>
inline typename Array<T>::reference Array<T>::back()
{
    return m_data.back();
}

template<class T>
inline typename Array<T>::const_reference Array<T>::back() const
{
    return m_data.back();
}

template<class T>
inline T* Array<T>::data() noexcept
{
    return m_data.data();
}

template<class T>
inline const T* Array<T>::data() const noexcept
{
    return m_data.data();
}

template<class T>
inline void Array<T>::assign(const int count, const T& value)
{
    m_data.assign(count, value);
}

template<class T>
template<class TIterator>
inline void Array<T>::assign(TIterator first, TIterator last)
{
    m_data.assign(first, last);
}

template<class T>
inline void Array<T>::assign(std::initializer_list<T> ilist)
{
    m_data.assign(ilist);
}

template<class T>
inline void Array<T>::push_back(const T& value)
{
    m_data.push_back(value);
}

template<class T>
inline void Array<T>::push_back(T&& value)
{
    m_data.push_back(std::move(value));
}

template<class T>
inline void Array<T>::push(const T& value)
{
    m_data.push_back(value);
}

template<class T>
inline void Array<T>::push(T&& value)
{
    m_data.push_back(std::move(value));
}

template<class T>
inline void Array<T>::pop_back()
{
    m_data.pop_back();
}

template<class T>
inline typename Array<T>::iterator Array<T>::insert(const_iterator pos, const T& value)
{
    return m_data.insert(pos, value);
}

template<class T>
inline typename Array<T>::iterator Array<T>::insert(const_iterator pos, T&& value)
{
    return m_data.insert(pos, std::move(value));
}

template<class T>
inline typename Array<T>::iterator Array<T>::insert(const_iterator pos, int count, const T& value)
{
    return m_data.insert(pos, count, value);
}

template<class T>
template <class Iterator>
inline typename Array<T>::iterator Array<T>::insert(const_iterator pos, Iterator first, Iterator last)
{
    return m_data.insert(pos, first, last);
}

template<class T>
inline typename Array<T>::iterator Array<T>::insert(const_iterator pos, std::initializer_list<T> ilist)
{
    return m_data.insert(pos, ilist);
}

template<class T>
inline typename Array<T>::iterator Array<T>::erase(const_iterator pos)
{
    return m_data.erase(pos);
}

template<class T>
inline typename Array<T>::iterator Array<T>::erase(const_iterator first, const_iterator last)
{
    return m_data.erase(first, last);
}

template<class T>
inline void Array<T>::swap(Array& other) noexcept
{
    m_data.swap(other.m_data);
}

template<class T>
inline void Array<T>::clear() noexcept
{
    return m_data.clear();
}

template<class T>
template<class... Args>
inline typename Array<T>::iterator Array<T>::emplace(const_iterator pos, Args&&... args)
{
    return m_data.emplace(pos, std::forward<Args>(args)...);
}

template<class T>
template<class... Args>
inline typename Array<T>::reference Array<T>::emplace_back(Args&&... args)
{
    return m_data.emplace_back(std::forward<Args>(args)...);
}

template<typename T>
template<typename TPred>
int Array<T>::indexOfIf(TPred pred, int startIndex) const
{
    if (m_data.empty())
        return -1;

    // index based loop, for std::unique_ptr
    const int count = m_data.size();
    for (int i = startIndex; i < count; i++) {
        if (pred(m_data[i])) {
            return i;
        }
    }
    return -1;
}

template<typename T>
bool Array<T>::contains(const value_type& item) const
{
    return std::find(m_data.begin(), m_data.end(), item) != m_data.end();
}

template<typename T>
template<typename TPred>
bool Array<T>::containsIf(TPred pred) const
{
    return std::find_if(m_data.begin(), m_data.end(), pred) != m_data.end();
}

template<class T>
inline typename Array<T>::iterator Array<T>::insert(int pos, const T& value)
{
    return m_data.insert(begin() + pos, value);
}

template<class T>
inline typename Array<T>::iterator Array<T>::insert(int pos, T&& value)
{
    return m_data.insert(begin() + pos, std::move(value));
}

template<class T>
inline typename Array<T>::iterator Array<T>::insert(int pos, int count, const T& value)
{
    return m_data.insert(begin() + pos, count, value);
}

template<class T>
template <class Iterator>
inline typename Array<T>::iterator Array<T>::insert(int pos, Iterator first, Iterator last)
{
    return m_data.insert(begin() + pos, first, last);
}

template<class T>
inline typename Array<T>::iterator Array<T>::insert(int pos, std::initializer_list<T> ilist)
{
    return m_data.insert(begin() + pos, ilist);
}

template<class T>
inline bool operator ==(const Array<T>& a, const Array<T>& b)
{
    return ((a.size() == b.size()) && std::equal(a.begin(), a.end(), b.begin()));
}

template<class T>
inline bool operator !=(const Array<T>& a, const Array<T>& b)
{
    return ((a.size() != b.size()) || !std::equal(a.begin(), a.end(), b.begin()));
}

template<class T>
inline bool operator <(const Array<T>& a, const Array<T>& b)
{
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

template<class T>
inline bool operator >(const Array<T>& a, const Array<T>& b)
{
    return (b < a);
}

template<class T>
inline bool operator <=(const Array<T>& a, const Array<T>& b)
{
    return !(b < a);
}

template<class T>
inline bool operator >=(const Array<T>& a, const Array<T>& b)
{
    return !(a < b);
}

template<class T>
inline void swap(Array<T>& a, Array<T>& b) noexcept
{
    a.swap(b);
}

} // namespace ln

#include "Array.ipp"
