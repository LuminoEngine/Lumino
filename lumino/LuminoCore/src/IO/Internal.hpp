#pragma once
#include "../Internal.hpp"

namespace ln {
namespace detail {
static const int LocalPathBaseLength = 255;

// 文字列が短ければスタックに、長ければヒープに確保する
template<typename TChar>
class GenericStaticallyLocalPath {
public:
    GenericStaticallyLocalPath();
    GenericStaticallyLocalPath(const char* path, size_t len);
    GenericStaticallyLocalPath(const wchar_t* path, size_t len);
    GenericStaticallyLocalPath(const char16_t* path, size_t len);
    GenericStaticallyLocalPath(const char32_t* path, size_t len);

    const TChar* c_str() const { return m_path.empty() ? m_static : m_path.c_str(); }
    const std::basic_string<TChar>& getPath() const { return m_path; }
    size_t getLength() const { return m_length; }
    size_t length() const { return m_length; }
    bool isEmpty() const { return m_length == 0; }

    bool isStatic() const { return m_path.empty(); }

private:
    TChar m_static[LocalPathBaseLength + 1];
    size_t m_length;
    std::basic_string<TChar> m_path;
};

typedef GenericStaticallyLocalPath<char> StaticallyLocalPathA;
typedef GenericStaticallyLocalPath<wchar_t> StaticallyLocalPathW;

} // namespace detail
} // namespace ln
