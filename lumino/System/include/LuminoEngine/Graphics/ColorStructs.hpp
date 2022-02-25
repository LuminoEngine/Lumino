// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

namespace ln {
struct Vector3;
struct Vector4;
struct Color;
struct ColorI;
class Serializer2;

/**
 * 各要素を 0.0～1.0 の範囲で表す RGBA カラーを定義します。
 */
LN_STRUCT()
struct Color
{
public:
    /** 赤成分 (0.0～1.0) */
    LN_FIELD()
    float r;

    /** 緑成分 (0.0～1.0) */
    LN_FIELD()
    float g;

    /** 青成分 (0.0～1.0) */
    LN_FIELD()
    float b;

    /** アルファ成分 (0.0～1.0) */
    LN_FIELD()
    float a;

public:
    static const Color Zero;                 /**< #FF0000 (0.0, 0.0, 0.0, 0.0) */
    static const Color Transparency;         /**< #FF0000 (透明) (0.0, 0.0, 0.0, 0.0) */
    static const Color TransparencyWhite;    /**< #FF0000 (白透明) (1.0, 1.0, 1.0, 0.0) */
    static const Color Red;                  /**< #FF0000 (1, 0, 0) */
    static const Color Green;                /**< #00FF00 (0, 1, 0) */
    static const Color Blue;                 /**< #0000FF (0, 0, 1) */
    static const Color AliceBlue;            /**< #F0F8FF (0.941176, 0.972549, 1) */
    static const Color AntiqueWhite;         /**< #FAEBD7 (0.980392, 0.921569, 0.843137) */
    static const Color Aqua;                 /**< #00FFFF (0, 1, 1) */
    static const Color Aquamarine;           /**< #7FFFD4 (0.498039, 1, 0.831373) */
    static const Color Azure;                /**< #F0FFFF (0.941176, 1, 1) */
    static const Color Beige;                /**< #F5F5DC (0.960784, 0.960784, 0.862745) */
    static const Color Bisque;               /**< #FFE4C4 (1, 0.894118, 0.768627) */
    static const Color Black;                /**< #000000 (0, 0, 0) */
    static const Color BlanchedAlmond;       /**< #FFEBCD (1, 0.921569, 0.803922) */
    static const Color BlueViolet;           /**< #8A2BE2 (0.541176, 0.168627, 0.886275) */
    static const Color Brown;                /**< #A52A2A (0.647059, 0.164706, 0.164706) */
    static const Color BurlyWood;            /**< #DEB887 (0.870588, 0.721569, 0.529412) */
    static const Color CadetBlue;            /**< #5F9EA0 (0.372549, 0.619608, 0.627451) */
    static const Color Chartreuse;           /**< #7FFF00 (0.498039, 1, 0) */
    static const Color Chocolate;            /**< #D2691E (0.823529, 0.411765, 0.117647) */
    static const Color Coral;                /**< #FF7F50 (1, 0.498039, 0.313725) */
    static const Color CornflowerBlue;       /**< #6495ED (0.392157, 0.584314, 0.929412) */
    static const Color Cornsilk;             /**< #FFF8DC (1, 0.972549, 0.862745) */
    static const Color Crimson;              /**< #DC143C (0.862745, 0.0784314, 0.235294) */
    static const Color Cyan;                 /**< #00FFFF (0, 1, 1) */
    static const Color DarkBlue;             /**< #00008B (0, 0, 0.545098) */
    static const Color DarkCyan;             /**< #008B8B (0, 0.545098, 0.545098) */
    static const Color DarkGoldenrod;        /**< #B8860B (0.721569, 0.52549, 0.0431373) */
    static const Color DarkGray;             /**< #A9A9A9 (0.662745, 0.662745, 0.662745) */
    static const Color DarkGreen;            /**< #006400 (0, 0.392157, 0) */
    static const Color DarkKhaki;            /**< #BDB76B (0.741176, 0.717647, 0.419608) */
    static const Color DarkMagenta;          /**< #8B008B (0.545098, 0, 0.545098) */
    static const Color DarkOliveGreen;       /**< #556B2F (0.333333, 0.419608, 0.184314) */
    static const Color DarkOrange;           /**< #FF8C00 (1, 0.54902, 0) */
    static const Color DarkOrchid;           /**< #9932CC (0.6, 0.196078, 0.8) */
    static const Color DarkRed;              /**< #8B0000 (0.545098, 0, 0) */
    static const Color DarkSalmon;           /**< #E9967A (0.913725, 0.588235, 0.478431) */
    static const Color DarkSeaGreen;         /**< #8FBC8F (0.560784, 0.737255, 0.560784) */
    static const Color DarkSlateBlue;        /**< #483D8B (0.282353, 0.239216, 0.545098) */
    static const Color DarkSlateGray;        /**< #2F4F4F (0.184314, 0.309804, 0.309804) */
    static const Color DarkTurquoise;        /**< #00CED1 (0, 0.807843, 0.819608) */
    static const Color DarkViolet;           /**< #9400D3 (0.580392, 0, 0.827451) */
    static const Color DeepPink;             /**< #FF1493 (1, 0.0784314, 0.576471) */
    static const Color DeepSkyBlue;          /**< #00BFFF (0, 0.74902, 1) */
    static const Color DimGray;              /**< #696969 (0.411765, 0.411765, 0.411765) */
    static const Color DodgerBlue;           /**< #1E90FF (0.117647, 0.564706, 1) */
    static const Color Firebrick;            /**< #B22222 (0.698039, 0.133333, 0.133333) */
    static const Color FloralWhite;          /**< #FFFAF0 (1, 0.980392, 0.941176) */
    static const Color ForestGreen;          /**< #228B22 (0.133333, 0.545098, 0.133333) */
    static const Color Fuchsia;              /**< #FF00FF (1, 0, 1) */
    static const Color Gainsboro;            /**< #DCDCDC (0.862745, 0.862745, 0.862745) */
    static const Color GhostWhite;           /**< #F8F8FF (0.972549, 0.972549, 1) */
    static const Color Gold;                 /**< #FFD700 (1, 0.843137, 0) */
    static const Color Goldenrod;            /**< #DAA520 (0.854902, 0.647059, 0.12549) */
    static const Color Gray;                 /**< #808080 (0.5, 0.5, 0.5) */
    static const Color GreenYellow;          /**< #ADFF2F (0.678431, 1, 0.184314) */
    static const Color Honeydew;             /**< #F0FFF0 (0.941176, 1, 0.941176) */
    static const Color HotPink;              /**< #FF69B4 (1, 0.411765, 0.705882) */
    static const Color IndianRed;            /**< #CD5C5C (0.803922, 0.360784, 0.360784) */
    static const Color Indigo;               /**< #4B0082 (0.294118, 0, 0.509804) */
    static const Color Ivory;                /**< #FFFFF0 (1, 1, 0.941176) */
    static const Color Khaki;                /**< #F0E68C (0.941176, 0.901961, 0.54902) */
    static const Color Lavender;             /**< #E6E6FA (0.901961, 0.901961, 0.980392) */
    static const Color LavenderBlush;        /**< #FFF0F5 (1, 0.941176, 0.960784) */
    static const Color LawnGreen;            /**< #7CFC00 (0.486275, 0.988235, 0) */
    static const Color LemonChiffon;         /**< #FFFACD (1, 0.980392, 0.803922) */
    static const Color LightBlue;            /**< #ADD8E6 (0.678431, 0.847059, 0.901961) */
    static const Color LightCoral;           /**< #F08080 (0.941176, 0.5, 0.5) */
    static const Color LightCyan;            /**< #E0FFFF (0.878431, 1, 1) */
    static const Color LightGoldenrodYellow; /**< #FAFAD2 (0.980392, 0.980392, 0.823529) */
    static const Color LightGray;            /**< #D3D3D3 (0.827451, 0.827451, 0.827451) */
    static const Color LightGreen;           /**< #90EE90 (0.564706, 0.933333, 0.564706) */
    static const Color LightPink;            /**< #FFB6C1 (1, 0.713725, 0.756863) */
    static const Color LightSalmon;          /**< #FFA07A (1, 0.627451, 0.478431) */
    static const Color LightSeaGreen;        /**< #20B2AA (0.12549, 0.698039, 0.666667) */
    static const Color LightSkyBlue;         /**< #87CEFA (0.529412, 0.807843, 0.980392) */
    static const Color LightSlateGray;       /**< #778899 (0.466667, 0.533333, 0.6) */
    static const Color LightSteelBlue;       /**< #B0C4DE (0.690196, 0.768627, 0.870588) */
    static const Color LightYellow;          /**< #FFFFE0 (1, 1, 0.878431) */
    static const Color Lime;                 /**< #00FF00 (0, 1, 0) */
    static const Color LimeGreen;            /**< #32CD32 (0.196078, 0.803922, 0.196078) */
    static const Color Linen;                /**< #FAF0E6 (0.980392, 0.941176, 0.901961) */
    static const Color Magenta;              /**< #FF00FF (1, 0, 1) */
    static const Color Maroon;               /**< #800000 (0.5, 0, 0) */
    static const Color MediumAquamarine;     /**< #66CDAA (0.4, 0.803922, 0.666667) */
    static const Color MediumBlue;           /**< #0000CD (0, 0, 0.803922) */
    static const Color MediumOrchid;         /**< #BA55D3 (0.729412, 0.333333, 0.827451) */
    static const Color MediumPurple;         /**< #9370DB (0.576471, 0.439216, 0.858824) */
    static const Color MediumSeaGreen;       /**< #3CB371 (0.235294, 0.701961, 0.443137) */
    static const Color MediumSlateBlue;      /**< #7B68EE (0.482353, 0.407843, 0.933333) */
    static const Color MediumSpringGreen;    /**< #00FA9A (0, 0.980392, 0.603922) */
    static const Color MediumTurquoise;      /**< #48D1CC (0.282353, 0.819608, 0.8) */
    static const Color MediumVioletRed;      /**< #C71585 (0.780392, 0.0823529, 0.521569) */
    static const Color MidnightBlue;         /**< #191970 (0.0980392, 0.0980392, 0.439216) */
    static const Color MintCream;            /**< #F5FFFA (0.960784, 1, 0.980392) */
    static const Color MistyRose;            /**< #FFE4E1 (1, 0.894118, 0.882353) */
    static const Color Moccasin;             /**< #FFE4B5 (1, 0.894118, 0.709804) */
    static const Color NavajoWhite;          /**< #FFDEAD (1, 0.870588, 0.678431) */
    static const Color Navy;                 /**< #000080 (0, 0, 0.5) */
    static const Color OldLace;              /**< #FDF5E6 (0.992157, 0.960784, 0.901961) */
    static const Color Olive;                /**< #808000 (0.5, 0.5, 0) */
    static const Color OliveDrab;            /**< #6B8E23 (0.419608, 0.556863, 0.137255) */
    static const Color Orange;               /**< #FFA500 (1, 0.647059, 0) */
    static const Color OrangeRed;            /**< #FF4500 (1, 0.270588, 0) */
    static const Color Orchid;               /**< #DA70D6 (0.854902, 0.439216, 0.839216) */
    static const Color PaleGoldenrod;        /**< #EEE8AA (0.933333, 0.909804, 0.666667) */
    static const Color PaleGreen;            /**< #98FB98 (0.596078, 0.984314, 0.596078) */
    static const Color PaleTurquoise;        /**< #AFEEEE (0.686275, 0.933333, 0.933333) */
    static const Color PaleVioletRed;        /**< #DB7093 (0.858824, 0.439216, 0.576471) */
    static const Color PapayaWhip;           /**< #FFEFD5 (1, 0.937255, 0.835294) */
    static const Color PeachPuff;            /**< #FFDAB9 (1, 0.854902, 0.72549) */
    static const Color Peru;                 /**< #CD853F (0.803922, 0.521569, 0.247059) */
    static const Color Pink;                 /**< #FFC0CB (1, 0.752941, 0.796078) */
    static const Color Plum;                 /**< #DDA0DD (0.866667, 0.627451, 0.866667) */
    static const Color PowderBlue;           /**< #B0E0E6 (0.690196, 0.878431, 0.901961) */
    static const Color Purple;               /**< #800080 (0.5, 0, 0.5) */
    static const Color RosyBrown;            /**< #BC8F8F (0.737255, 0.560784, 0.560784) */
    static const Color RoyalBlue;            /**< #4169E1 (0.254902, 0.411765, 0.882353) */
    static const Color SaddleBrown;          /**< #8B4513 (0.545098, 0.270588, 0.0745098) */
    static const Color Salmon;               /**< #FA8072 (0.980392, 0.5, 0.447059) */
    static const Color SandyBrown;           /**< #F4A460 (0.956863, 0.643137, 0.376471) */
    static const Color SeaGreen;             /**< #2E8B57 (0.180392, 0.545098, 0.341176) */
    static const Color SeaShell;             /**< #FFF5EE (1, 0.960784, 0.933333) */
    static const Color Sienna;               /**< #A0522D (0.627451, 0.321569, 0.176471) */
    static const Color Silver;               /**< #C0C0C0 (0.752941, 0.752941, 0.752941) */
    static const Color SkyBlue;              /**< #87CEEB (0.529412, 0.807843, 0.921569) */
    static const Color SlateBlue;            /**< #6A5ACD (0.415686, 0.352941, 0.803922) */
    static const Color SlateGray;            /**< #708090 (0.439216, 0.5, 0.564706) */
    static const Color Snow;                 /**< #FFFAFA (1, 0.980392, 0.980392) */
    static const Color SpringGreen;          /**< #00FF7F (0, 1, 0.498039) */
    static const Color SteelBlue;            /**< #4682B4 (0.27451, 0.509804, 0.705882) */
    static const Color Tan;                  /**< #D2B48C (0.823529, 0.705882, 0.54902) */
    static const Color Teal;                 /**< #008080 (0, 0.5, 0.5) */
    static const Color Thistle;              /**< #D8BFD8 (0.847059, 0.74902, 0.847059) */
    static const Color Tomato;               /**< #FF6347 (1, 0.388235, 0.278431) */
    static const Color Transparent;          /**< #00FFFFFF (1, 1, 1) */
    static const Color Turquoise;            /**< #40E0D0 (0.25098, 0.878431, 0.815686) */
    static const Color Violet;               /**< #EE82EE (0.933333, 0.509804, 0.933333) */
    static const Color Wheat;                /**< #F5DEB3 (0.960784, 0.870588, 0.701961) */
    static const Color White;                /**< #FFFFFF (1, 1, 1) */
    static const Color WhiteSmoke;           /**< #F5F5F5 (0.960784, 0.960784, 0.960784) */
    static const Color Yellow;               /**< #FFFF00 (1, 1, 0) */
    static const Color YellowGreen;          /**< #9ACD32 (0.603922, 0.803922, 0.196078) */

public:
    /** すべての要素を 0.0 で初期化します。 */
    LN_METHOD(OverloadPostfix = "Zeros")
    constexpr Color() noexcept
        : r(0.0f)
        , g(0.0f)
        , b(0.0f)
        , a(0.0f)
    {
    }

    /** 各要素を指定して初期化します。 */
    LN_METHOD()
    constexpr Color(float r_, float g_, float b_, float a_ = 1.0f) noexcept
        : r(r_)
        , g(g_)
        , b(b_)
        , a(a_)
    {
    }

    /** グレースケール値とアルファ値を指定して初期化します。 */
    constexpr Color(float rgb, float a_ = 1.0f) noexcept
        : r(rgb)
        , g(rgb)
        , b(rgb)
        , a(a_)
    {
    }

    /** 指定した Color をコピーして初期化します。 */
    constexpr Color(const Color& other) noexcept
        : r(other.r)
        , g(other.g)
        , b(other.b)
        , a(other.a)
    {
    }

    /** 指定した Color とアルファ値で初期化します。color のアルファ値は無視され、a_ で指定されたアルファ値を設定します。 */
    constexpr Color(const Color& color, float a_) noexcept
        : r(color.r)
        , g(color.g)
        , b(color.b)
        , a(a_)
    {
    }

    /** 指定した Vector4 をコピーして初期化します。x y z w 成分がそれぞれ r g b a 値となります。 */
    constexpr Color(const Vector4& vec) noexcept;

    /** 指定した Vector3 とアルファ値をで初期化します。 x y z 成分がそれぞれ r g b 値となります。 */
    constexpr explicit Color(const Vector3& vec, float a = 1.0f) noexcept;

    /** ColorI を変換して初期化します。 */
    explicit Color(const ColorI& color) noexcept;

public:
    /** 各要素を設定します。 */
    void set(float r_, float g_, float b_, float a_ = 1.0f) noexcept
    {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }

    Color withAlpha(float alpha) const { return Color(r, g, b, alpha); }

    /** 2 つの色を加算します。結果を 0.0～1.0 にクランプします。 */
    static constexpr Color addClamp(const Color& c1, const Color& c2) noexcept;

    /** 2 つの色を乗算します。結果を 0.0～1.0 にクランプします。 */
    static constexpr Color multiplyClamp(const Color& c1, const Color& c2) noexcept;

    /** 2 つの色を線形補間します。 */
    static constexpr Color lerp(const Color& c1, const Color& c2, float t) noexcept;

    /**
     * HTML カラー表現の文字列を、Color に変換します。
     *
     * 書式は次の通りです。
     * - #RRGGBB
     * - #AARRGGBB
     * - #RGB
     * - #ARGB
     */
    static Color parse(const StringView& str);

	static bool nearEqual(const Color& value1, const Color& value2) { return Vector4::nearEqual(value1.toVector4(), value2.toVector4()); }

    /** Vector4 へ変換します。 */
    const Vector4& toVector4() const { return *reinterpret_cast<const Vector4*>(this); }

    Vector3 rgb() const LN_NOEXCEPT { return Vector3(r, g, b); }

    void serialize(Serializer2& ar);

#define LN_OP_TYPE Color
#include <LuminoCore/Math/Vector4OpDeclareTemplate.inl>
#undef LN_OP_TYPE
};

constexpr Color::Color(const Vector3& vec, float a_) noexcept
    : r(vec.x)
    , g(vec.y)
    , b(vec.z)
    , a(a_)
{
}

constexpr Color::Color(const Vector4& vec) noexcept
    : r(vec.x)
    , g(vec.y)
    , b(vec.z)
    , a(vec.w)
{
}

constexpr Color Color::addClamp(const Color& c1, const Color& c2) noexcept
{
    return Color(
        Math::clamp01(c1.r + c2.r),
        Math::clamp01(c1.g + c2.g),
        Math::clamp01(c1.b + c2.b),
        Math::clamp01(c1.a + c2.a));
}

constexpr Color Color::multiplyClamp(const Color& c1, const Color& c2) noexcept
{
    return Color(
        Math::clamp01(c1.r * c2.r),
        Math::clamp01(c1.g * c2.g),
        Math::clamp01(c1.b * c2.b),
        Math::clamp01(c1.a * c2.a));
}

constexpr Color Color::lerp(const Color& color1, const Color& color2, float t) noexcept
{
    return Color(
        Math::lerp(color1.r, color2.r, t),
        Math::lerp(color1.g, color2.g, t),
        Math::lerp(color1.b, color2.b, t),
        Math::lerp(color1.a, color2.a, t));
}

#define LN_OP_TYPE Color
#define LN_E0 r
#define LN_E1 g
#define LN_E2 b
#define LN_E3 a
#include <LuminoCore/Math/Vector4OpImplementTemplate.inl>
#undef LN_OP_TYPE
#undef LN_E0
#undef LN_E1
#undef LN_E2
#undef LN_E3

/** 色調を定義します。 */
LN_STRUCT()
struct ColorTone
{
public:
    /** (0, 0, 0, 0) */
    static const ColorTone Zero;

public:
    /** 赤成分のカラーバランス (-1.0～1.0) */
    LN_FIELD()
    float r;

    /** 緑成分のカラーバランス (-1.0～1.0) */
    LN_FIELD()
    float g;

    /** 青成分のカラーバランス (-1.0～1.0) */
    LN_FIELD()
    float b;

    /** グレースケールフィルタの強さ (0.0 ～ 1.0) (saturation) */
    LN_FIELD()
    float s;

public:
    /** すべての要素を 0.0 で初期化します。 */
    LN_METHOD(OverloadPostfix = "Zeros")
    constexpr ColorTone() noexcept
        : r(0.0f)
        , g(0.0f)
        , b(0.0f)
        , s(0.0f)
    {
    }

    /** 各要素を指定して初期化します。 */
    LN_METHOD()
    constexpr ColorTone(float r_, float g_, float b_, float s_) noexcept
        : r(r_)
        , g(g_)
        , b(b_)
        , s(s_)
    {
    }

    /** 指定した Vector4 をコピーして初期化します。 */
    constexpr ColorTone(const Vector4& vec) noexcept
        : r(vec.x)
        , g(vec.y)
        , b(vec.z)
        , s(vec.w)
    {
    }

public:
    /** 各要素を設定します。 */
    void set(float r_, float g_, float b_, float s_)
    {
        r = r_;
        g = g_;
        b = b_;
        s = s_;
    }

    /** この色調に指定した色調を加算します。0.0～1.0 を超える場合はクランプします。 */
    void addClamp(const ColorTone& tone);

    bool isZero() const { return r == 0.0f && g == 0.0f && b == 0.0f && s == 0.0f; }

	static bool nearEqual(const ColorTone& value1, const ColorTone& value2) { return Vector4::nearEqual(value1.toVector4(), value2.toVector4()); }

    /** Vector4 へ変換します。 */
    const Vector4& toVector4() const { return *reinterpret_cast<const Vector4*>(this); }

#define LN_OP_TYPE ColorTone
#include <LuminoCore/Math/Vector4OpDeclareTemplate.inl>
#undef LN_OP_TYPE
};

#define LN_OP_TYPE ColorTone
#define LN_E0 r
#define LN_E1 g
#define LN_E2 b
#define LN_E3 s
#include <LuminoCore/Math/Vector4OpImplementTemplate.inl>
#undef LN_OP_TYPE
#undef LN_E0
#undef LN_E1
#undef LN_E2
#undef LN_E3

/** 各要素を 0～255 の範囲で表す RGBA カラーを定義します。 */
struct ColorI
{
public:
    /** (0, 0, 0, 0) */
    static const ColorI Zero;

    /** (255, 255, 255, 255) */
    static const ColorI White;

public:
    /** 赤成分 (0～255) */
    uint8_t r;

    /** 緑成分 (0～255) */
    uint8_t g;

    /** 青成分 (0～255) */
    uint8_t b;

    /** アルファ成分 (0～255) */
    uint8_t a;

public:
    /** すべての要素を 0 で初期化します。*/
    ColorI()
        : r(0)
        , g(0)
        , b(0)
        , a(0)
    {
    }

    /** 各要素を指定して初期化します。*/
    ColorI(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255)
    {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }

    /** 各要素を設定します。 */
    void set(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255)
    {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }

    static ColorI fromLinearColor(const Color& color)
    {
        return ColorI(
            static_cast<uint8_t>(color.r * 255),
            static_cast<uint8_t>(color.g * 255),
            static_cast<uint8_t>(color.b * 255),
            static_cast<uint8_t>(color.a * 255));
    }

    static ColorI parse(const StringView& str);

public:
    bool operator==(const ColorI& color) const { return (memcmp(this, &color, sizeof(ColorI)) == 0); }
    bool operator!=(const ColorI& obj) const { return !operator==(obj); }
};

class HSVColor
{
public:
    float h;    ///< 色相 Hue Saturation (0.0 ~ 360.0)
    float s;    ///< 彩度 Saturation (0.0 ~ 1.0)
    float v;    ///< 明度 Value (0.0 ~ 1.0)
    float a;    ///< 不透明度 (0.0 ~ 1.0)

public:
    HSVColor() : h(0.0f), s(0.0f), v(0.0f), a(1.0f) { }
    HSVColor(float h_, float s_, float v_, float a_) : h(h_), s(s_), v(v_), a(a_) { }

    Color toColor() const;

    static HSVColor fromColor(const Color& color);


public:
};


#if 0
/**
    @brief    各要素を整数値で表す HSV カラーを定義します。
*/
class HSVColorI
{
public:
    uint32_t    H;    ///< 色相 0～360
    uint32_t    S;    ///< 彩度 0～255
    uint32_t    V;    ///< 明度 0～255
    uint32_t    a;    ///< 不透明度 0～255

public:

    /**
        @brief    すべての要素を 0 で初期化します。
    */
    HSVColor() { H = 0; S = 0; V = 0; a = 255; }

    /**
        @brief    各要素を指定して初期化します。
    */
    HSVColor(uint32_t h, uint32_t s, uint32_t v, uint32_t a_) { H = h; S = s; V = v; a = a_; }
    
    /**
        @brief    ColorF を変換して設定します。
    */
    //ColorI(const ColorF& colorF);

public:

    /**
        @brief    各要素を設定します。
    */
    void set(uint32_t h, uint32_t s, uint32_t v, uint32_t a_) { H = h; S = s; V = v; a = a_; }

    /**
        @brief    ColorI 型に変換します。
    */
    ColorI toColor() const;
    
    /**
        @brief    ColorF 型に変換します。
    */
    Color toColorF() const;

public:
    bool operator == (const ColorI& color) const { return (memcmp(this, &color, sizeof(ColorI)) == 0); }
    bool operator != (const ColorI& obj) const { return !operator==(obj); }
};
#endif

} // namespace ln
