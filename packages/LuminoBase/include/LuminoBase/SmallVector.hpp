#pragma once
#include <cstddef>
#include <cassert>
#include <utility>
#include <new>

namespace ln {

/**
 * スタック上に固定サイズのバッファを持つ配列。
 * Capacity 個までの要素をヒープアロケーションなしで保持できる。
 * Capacity を超える要素を追加しようとした場合は assert で停止する。
 * 
 * @tparam T 要素の型
 * @tparam Capacity 最大要素数
 * 
 * @example
 * SmallVector<int, 4> vec;
 * vec.push_back(1);
 * vec.push_back(2);
 * int x = vec[0]; // => 1
 */
template<typename T, std::size_t Capacity>
class SmallVector {
public:
    SmallVector() : m_size(0) {}

    ~SmallVector() {
        for (std::size_t i = 0; i < m_size; ++i) {
            data()[i].~T();
        }
    }

    SmallVector(const SmallVector& other) : m_size(0) {
        for (std::size_t i = 0; i < other.m_size; ++i) {
            push_back(other.data()[i]);
        }
    }

    SmallVector(SmallVector&& other) noexcept : m_size(0) {
        for (std::size_t i = 0; i < other.m_size; ++i) {
            new (data() + i) T(std::move(other.data()[i]));
            other.data()[i].~T();
        }
        m_size = other.m_size;
        other.m_size = 0;
    }

    SmallVector& operator=(const SmallVector& other) {
        if (this != &other) {
            clear();
            for (std::size_t i = 0; i < other.m_size; ++i) {
                push_back(other.data()[i]);
            }
        }
        return *this;
    }

    void push_back(const T& value) {
        assert(m_size < Capacity && "SmallVector capacity exceeded");
        new (data() + m_size) T(value);
        ++m_size;
    }

    void push_back(T&& value) {
        assert(m_size < Capacity && "SmallVector capacity exceeded");
        new (data() + m_size) T(std::move(value));
        ++m_size;
    }

    template<typename... Args>
    T& emplace_back(Args&&... args) {
        assert(m_size < Capacity && "SmallVector capacity exceeded");
        T* ptr = new (data() + m_size) T(std::forward<Args>(args)...);
        ++m_size;
        return *ptr;
    }

    void pop_back() {
        assert(m_size > 0 && "SmallVector is empty");
        --m_size;
        data()[m_size].~T();
    }

    void clear() {
        for (std::size_t i = 0; i < m_size; ++i) {
            data()[i].~T();
        }
        m_size = 0;
    }

    T& operator[](std::size_t index) {
        assert(index < m_size && "SmallVector index out of range");
        return data()[index];
    }

    const T& operator[](std::size_t index) const {
        assert(index < m_size && "SmallVector index out of range");
        return data()[index];
    }

    T& front() { assert(m_size > 0); return data()[0]; }
    const T& front() const { assert(m_size > 0); return data()[0]; }

    T& back() { assert(m_size > 0); return data()[m_size - 1]; }
    const T& back() const { assert(m_size > 0); return data()[m_size - 1]; }

    T* begin() { return data(); }
    const T* begin() const { return data(); }
    T* end() { return data() + m_size; }
    const T* end() const { return data() + m_size; }

    std::size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }
    static constexpr std::size_t capacity() { return Capacity; }

    T* data() { return reinterpret_cast<T*>(m_storage); }
    const T* data() const { return reinterpret_cast<const T*>(m_storage); }

    bool operator==(const SmallVector& other) const {
        if (m_size != other.m_size) return false;
        for (std::size_t i = 0; i < m_size; ++i) {
            if (!(data()[i] == other.data()[i])) return false;
        }
        return true;
    }
    bool operator!=(const SmallVector& other) const { return !(*this == other); }

private:
    alignas(T) unsigned char m_storage[sizeof(T) * Capacity];
    std::size_t m_size;
};

} // namespace ln
