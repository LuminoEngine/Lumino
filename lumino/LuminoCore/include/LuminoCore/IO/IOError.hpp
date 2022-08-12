#pragma once
#include "../Base/String.hpp"
#include "../Base/Result.hpp"

namespace ln {

class IOError {
public:
    IOError()
        : m_errno(0)
        , m_dwError(0) {}

    IOError(int v)
        : m_errno(v)
        , m_dwError(0)
    {}
    int m_errno;
    uint32_t m_dwError;
};

template<>
inline String toString<IOError>(const IOError& e) {
    if (e.m_errno != 0) {
#ifdef LN_MSVC
        char buf[256];
        strerror_s(buf, sizeof(buf), e.m_errno);
        return String::fromCString(buf);
#else
        return String::fromCString(strerror(e.m_errno));
#endif
    }
    return U"IOError"; // TODO:
}


template<class T = void>
using IOResult = BasicResult<T, IOError>;

} // namespace ln
