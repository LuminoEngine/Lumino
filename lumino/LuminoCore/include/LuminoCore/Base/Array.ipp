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

} // namespace ln

#include "Array.ipp"
