#pragma once
#include <LuminoEngine/Graphics/GeometryStructs.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>

namespace ln {
class Font;

/** テキストの配置方法 */
enum class TextAlignment
{
    /** 左揃え・上揃え */
	Forward,
    
    /** 中央揃え */
    Center,

    /** 右揃え・下揃え */
	Backward,

    /** 両端揃え */
    Justify,


};

enum class TextCrossAlignment
{
	/** 左揃え・上揃え */
	Forward,

	/** 中央揃え */
	Center,

	/** 右揃え・下揃え */
	Backward,

	/** 両端揃え */
	Justify,

};

///** 描画領域に対するテキストの配置方法 */
//enum class TextAnchor
//{
//	/** 左上 */
//	UpperLeft,
//
//	/** 上 */
//	UpperCenter,
//
//	/** 右上 */
//	UpperRight,
//
//	/** 左 */
//	MiddleLeft,
//
//	/** 中央 */
//	MiddleCenter,
//
//	/** 右 */
//	MiddleRight,
//
//	/** 左下 */
//	LowerLeft,
//
//	/** 下 */
//	LowerCenter,
//
//	/** 右下 */
//	LowerRight,	
//};
//
namespace detail {

class FontDesc
{
public:
    String Family;	// empty is default font
    int Size;   // 1/72inch   Web Base.
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
