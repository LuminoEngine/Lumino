#pragma once
#include <array>

namespace ln {

template<typename T, size_t N>
struct FixedArray {
public:
    size_t size() const { return m_size; }

    void resize(size_t nsize) {
        assert(nsize <= m_data.size());
        m_size = nsize;
    }

    T& at(size_t n) {
        assert(n < m_size);
        return m_data.at(n);
    }

    const T& at(size_t n) const {
        assert(n < m_size);
        return m_data.at(n);
    }

    const T* data() const { return m_data.data(); }

    /** Add element to the end. */
    void push(const T& value) {
        assert(m_size < m_data.size());
        m_data[m_size] = value;
        m_size++;
    }

    bool equals(FixedArray<T, N> const& other) const {
        if (m_size != other.m_size) {
            return false;
        }
        for (size_t i = 0; i < m_size; i++) {
            if (m_data[i] != other.m_data[i]) {
                return false;
            }
        }
        return true;
    }

private:
    std::array<T, N> m_data;
    size_t m_size = 0;
};

} // namespace ln

