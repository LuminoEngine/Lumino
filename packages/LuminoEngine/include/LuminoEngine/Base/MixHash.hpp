
#pragma once
#include <LuminoCore/Base/CRCHash.hpp>

namespace ln {
namespace detail {

#define LN_MIXHASH_M 0x5bd1e995
#define LN_MIXHASH_R 24
#define LN_MIXHASH(_h, _k)        \
    {                             \
        _k *= LN_MIXHASH_M;       \
        _k ^= _k >> LN_MIXHASH_R; \
        _k *= LN_MIXHASH_M;       \
        _h *= LN_MIXHASH_M;       \
        _h ^= _k;                 \
    }

class MixHash {
public:
    MixHash()
        : m_value(0) {
    }

    void add(const void* data, size_t len) {
        uint32_t k = computeSingle(data, len);
        LN_MIXHASH(m_value, k);
    }

    template<typename T>
    void add(const T& data) {
        add(&data, sizeof(T));
    }

    // Add computed hash.
    void addHash(uint32_t hash) {
        LN_MIXHASH(m_value, hash);
    }

    uint32_t value() const { return m_value; }

    static constexpr uint32_t computeSingle(const void* data, size_t len) {
        return CRCHash::calcCrcHash(static_cast<const uint8_t*>(data), len);
    }

    template<size_t N>
    static constexpr uint32_t computeSingle(const char (&data)[N]) {
        return CRCHash::calcCrcHash(data, N - 1);   // -1 is null terminator.
    }

private:
    uint32_t m_value;
};

inline uint64_t hash_combine(uint64_t seed, uint64_t v) {
    return seed ^ ((v) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

} // namespace detail
} // namespace ln
