#pragma once
#include <LuminoEngine/Graphics/GeometryStructs.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>

namespace ln {
namespace detail {

class FontDesc
{
public:
    String Family;
    int Size;
    bool isBold;
    bool isItalic;
    bool isAntiAlias;

    FontDesc();
    bool operator < (const FontDesc& right);
    uint64_t calcHash() const;
};

} // namespace detail
} // namespace ln
