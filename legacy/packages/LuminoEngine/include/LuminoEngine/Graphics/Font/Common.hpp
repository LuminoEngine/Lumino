#pragma once
#include <LuminoEngine/Graphics/ColorStructs.hpp>

namespace ln {
class Font;

/** テキストの配置方法 */
enum class TextAlignment {
    /** 左揃え・上揃え */
    Forward,

    /** 中央揃え */
    Center,

    /** 右揃え・下揃え */
    Backward,

    /** 両端揃え */
    Justify,
};

enum class TextCrossAlignment {
    /** 左揃え・上揃え */
    Forward,

    /** 中央揃え */
    Center,

    /** 右揃え・下揃え */
    Backward,

    /** 両端揃え */
    Justify,

};

/** テキストの配置方法 */
enum class TextDrawMode {
    /** 塗りつぶし文字を描画します。 */
    Fill = 0x01,
    /** 文字の輪郭を描画します。 */
    Stroke = 0x02,
};

namespace detail {

class FontDesc {
public:
    String Family; // empty is default font
    int Size;      // 1/72inch   Web Base.
    bool isBold;
    bool isItalic;
    bool isAntiAlias;

    FontDesc();
    bool operator<(const FontDesc& right);
    uint64_t calcHash() const;
    bool equals(const FontDesc& other) const;
};

} // namespace detail
} // namespace ln
