#pragma once
#include <cstddef>
#include <cassert>
#include <type_traits>
#include <LuminoBase/Logger.hpp>

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
    static_assert(std::is_trivially_copyable_v<T>, "SmallVector の要素は trivially copyable な型のみです。");
    static_assert(Capacity > 0, "SmallVector の Capacity は 1 以上である必要があります。");

public:
    void push_back(const T& value) {
        assert(m_size < Capacity && "SmallVector capacity exceeded");
        if (m_size >= Capacity) {
            // assert は NDEBUG で消えるため、ここで弾かないと範囲外書き込みになる。
            LN_LOG_ERROR("SmallVector: 容量 (%zu) を超えるため要素を追加できません。", Capacity);
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
