#pragma once

#include <cstdint>
#include <cstddef>

namespace lumino {

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using f32 = float;
using f64 = double;

/// Non-owning view of a contiguous range.
template <typename T>
struct Span {
    T* data_ = nullptr;
    size_t size_ = 0;

    constexpr Span() = default;
    constexpr Span(T* d, size_t s) : data_(d), size_(s) {}

    template <size_t N>
    constexpr Span(T (&arr)[N]) : data_(arr), size_(N) {}

    constexpr T* data() const { return data_; }
    constexpr size_t size() const { return size_; }
    constexpr bool empty() const { return size_ == 0; }
    constexpr T& operator[](size_t i) const { return data_[i]; }
    constexpr T* begin() const { return data_; }
    constexpr T* end() const { return data_ + size_; }
};

} // namespace lumino
