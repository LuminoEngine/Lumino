#pragma once
#include "Common.hpp"
#include "Logger.hpp"

namespace ln {

/**
 * スタック上に固定サイズのバッファを持つ配列。
 *
 * Capacity 個までの要素をヒープアロケーションなしで保持できます。
 *
 * @example
 * ```
 * SmallVector<int, 4> vec;
 * vec.push_back(1);
 * vec.push_back(2);
 * int x = vec[0]; // => 1
 * ```
 */
template<typename T, std::size_t Capacity>
class SmallVector {
    static_assert(std::is_trivially_copyable_v<T>, "SmallVector element type must be trivially copyable.");
    static_assert(Capacity > 0, "SmallVector Capacity must be 1 or greater.");

public:
    void push_back(const T& value) {
        assert(m_size < Capacity && "SmallVector capacity exceeded");
        if (m_size >= Capacity) {
            LN_LOG_ERROR("SmallVector: cannot push_back, capacity (%zu) exceeded.", Capacity);
            return;
        }
        m_data[m_size] = value;
        ++m_size;
    }

    T& operator[](std::size_t index) {
        assert(index < m_size && "SmallVector index out of range");
        return m_data[index];
    }

    const T& operator[](std::size_t index) const {
        assert(index < m_size && "SmallVector index out of range");
        return m_data[index];
    }

    T* begin() { return m_data; }
    const T* begin() const { return m_data; }
    T* end() { return m_data + m_size; }
    const T* end() const { return m_data + m_size; }

    std::size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }

    T* data() { return m_data; }
    const T* data() const { return m_data; }

    bool operator==(const SmallVector& other) const {
        if (m_size != other.m_size) return false;
        for (std::size_t i = 0; i < m_size; ++i) {
            if (!(m_data[i] == other.m_data[i])) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const SmallVector& other) const { return !(*this == other); }

private:
    T m_data[Capacity];
    std::size_t m_size = 0;
};

} // namespace ln
