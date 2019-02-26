#pragma once
#include <LuminoEngine/Graphics/GeometryStructs.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>

namespace ln {

/** テキストの配置方法 */
enum class TextAlignment
{
    /** 左揃え */
    Left,
    
    /** 中央揃え */
    Center,

    /** 右揃え */
    Right,

    /** 両端揃え */
    Justify,
};

namespace detail {

class FontDesc
{
public:
    String Family;	// empty is default font
    int Size;
    bool isBold;
    bool isItalic;
    bool isAntiAlias;

    FontDesc();
    bool operator < (const FontDesc& right);
    uint64_t calcHash() const;
	bool equals(const FontDesc& other) const;
};

} // namespace detail
} // namespace ln
