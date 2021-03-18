#include "Internal.hpp"
#include <LuminoCore/IO/Stream.hpp>

namespace ln {

Stream::Stream()
{
}

Stream::~Stream()
{
}

std::vector<uint8_t> Stream::readToEnd()
{
    std::vector<uint8_t> data;
    if (LN_REQUIRE(canRead())) return data;
    data.resize(length());
    read(data.data(), data.size());
    return data;
}

} // namespace ln
