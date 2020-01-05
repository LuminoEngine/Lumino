// Copyright (c) 2018+ lriki. Distributed under the MIT license...
#pragma once

#include <stdio.h>
#include "Common.hpp"

namespace ln {
struct Vector4;
struct Matrix;

/** 
 * 2次元のベクトルを定義します。
 */
struct LN_API Vector2
{
public:
    /** X 要素 */
    float x;

    /** Y 要素 */
    float y;

    /** Vector2(0, 0) */
    static const Vector2 Zero;

    /** Vector2(1, 0) */
    static const Vector2 UnitX;

    /** Vector2(0, 1) */
    static const Vector2 UnitY;

    /** Vector2(1, 1) */
    static const Vector2 Ones;

	/** Vector2(nan, nan) */
	static const Vector2 NaN;

public:
    /**
     * すべての要素を 0.0 に設定してインスタンスを初期化します。
     */
    Vector2();

    /**
     * 指定した値を使用してインスタンスを初期化します。
     */
    Vector2(float x, float y);

public:
    /**
     * 各要素に値を設定します。
     */
    void set(float x, float y);

    /** Returns pointer to the first element. */
    float* data() LN_NOEXCEPT { return &x; }

    /** Returns pointer to the first element. */
    const float* data() const LN_NOEXCEPT { return &x; }

    /**
     * このベクトルの長さを返します。
     */
    float length() const;

    /**
     * このベクトルの長さの 2 乗を返します。
     */
    float lengthSquared() const;

    /**
     * このベクトルを正規化します。
     *     ベクトルの長さが 0 の場合は正規化を行いません。
     */
    void normalize();

    /**
     * 指定された行列を使用してこのベクトルを座標変換します。
     * @param[in]   mat     : 座標変換行列
     * 
     * ベクトルを (X, Y, 0.0, 1.0) として座標変換を行い、結果を w で除算します。
     */
    void transformCoord(const Matrix& mat);

    /**
     * 要素のいずれかが NaN または Inf かを判別します。
     */
    bool isNaNOrInf() const;

    /**
     * デバッグ用に文字列を標準出力します。
     * @param[in]   format  : 書式指定文字列
     * @param[in]   stream  : 出力先ストリーム
     * 
     * format が NULL の場合、書式は "%f, %f\n" を使用します。
     */
    void print(const char* format = NULL, FILE* stream = NULL) const;

public:
    /**
     * 指定ベクトルを正規化したベクトルを返します。
     * @param[in]   vec     : 処理の基になるベクトル
     * @return      正規化されたベクトル
     */
    static Vector2 normalize(const Vector2& vec);

	static Vector2 safeNormalize(const Vector2& vec, const Vector2& alt);

    /**
     * 2つのベクトルの内積を計算します。
     * @param[in]   vec1    : 処理の基になるベクトル
     * @param[in]   vec2    : 処理の基になるベクトル
     * @return      2つのベクトルの内積
     */
    static float dot(const Vector2& vec1, const Vector2& vec2);

    /**
     * 2つのベクトルの最小値で構成されるベクトルを返します。
     * @param[in]   vec1    : 処理の基になるベクトル
     * @param[in]   vec2    : 処理の基になるベクトル
     * @return      最小値から作成されたベクトル
     */
    static Vector2 min(const Vector2& vec1, const Vector2& vec2);

    /**
     * 2つのベクトルの最大値で構成されるベクトルを返します。
     * @param[in]   vec1    : 処理の基になるベクトル
     * @param[in]   vec2    : 処理の基になるベクトル
     * @return      最大値から作成されたベクトル
     */
    static Vector2 max(const Vector2& vec1, const Vector2& vec2);

    /**
     * 指定された行列を使用してベクトルを座標変換します。
     * @param[in]   vec        : 処理の基になるベクトル
     * @param[in]   mat        : 処理の基になる行列
     * @return      変換されたベクトル
     */
    static Vector4 transform(const Vector2& vec, const Matrix& mat);

    /**
     * 指定された行列を使用してベクトルを座標変換します。
     * @param[in]   vec        : 処理の基になるベクトル
     * @param[in]   mat        : 処理の基になる行列
     * @return      変換されたベクトル
     * 
     * ベクトルを (X, Y, 0.0, 1.0) として座標変換を行い、結果を w で除算します。
     */
    static Vector2 transformCoord(const Vector2& vec, const Matrix& mat);

    /**
     * 2 つのベクトル間の線形補間を行います。
     * @param[in]   start   : 開始ベクトル (t = 0.0 のときの値)
     * @param[in]   end     : 終了ベクトル (t = 1.0 のときの値)
     * @param[in]   t       : 加重係数
     * @return      補間結果のベクトル
     * 
     * t は通常、0.0～1.0 を指定します。
     */
    static Vector2 lerp(const Vector2& start, const Vector2& end, float t);

    /**
     * 指定されたベクトルを使用して エルミートスプライン補間を実行します。
     * @param[in]   v1      : 開始ベクトル
     * @param[in]   a1      : 開始ベクトルの接線ベクトル(速度)
     * @param[in]   v2      : 終了ベクトル
     * @param[in]   a2      : 終了ベクトルの接線ベクトル(速度)
     * @param[in]   t       : 加重係数
     * @return      補間結果の値
     * 
     * t は通常、0.0～1.0 を指定します。
     */
    static Vector2 hermite(const Vector2& v1, const Vector2& a1, const Vector2& v2, const Vector2& a2, float t);

    /**
     * 指定されたベクトルを使用して Catmull-Rom 補間を行います。
     * @param[in]   vec1    : 1番目の位置
     * @param[in]   vec2    : 2番目の位置 (t = 0.0 のときの値)
     * @param[in]   vec3    : 3番目の位置 (t = 1.0 のときの値)
     * @param[in]   vec4    : 4番目の位置
     * @param[in]   t       : 加重係数
     * @return      補間結果のベクトル
     * 
     * t は通常、0.0～1.0 を指定します。
     */
    static Vector2 catmullRom(const Vector2& vec1, const Vector2& vec2, const Vector2& vec3, const Vector2& vec4, float t);

	static bool nearEqual(const Vector2& value1, const Vector2& value2);

public:
    Vector2& operator+=(const Vector2& v);
    Vector2& operator+=(float v);
    Vector2& operator-=(const Vector2& v);
    Vector2& operator-=(float v);
    Vector2& operator*=(const Vector2& v);
    Vector2& operator*=(float v);
    Vector2& operator/=(const Vector2& v);
    Vector2& operator/=(float v);

    friend Vector2 operator+(const Vector2& v1, const Vector2& v2);
    friend Vector2 operator+(const Vector2& v1, float v2);
    friend Vector2 operator+(float v1, const Vector2& v2);
    friend Vector2 operator-(const Vector2& v1, const Vector2& v2);
    friend Vector2 operator-(const Vector2& v1, float v2);
    friend Vector2 operator-(float v1, const Vector2& v2);
    friend Vector2 operator*(const Vector2& v1, const Vector2& v2);
    friend Vector2 operator*(const Vector2& v1, float v2);
    friend Vector2 operator*(float v1, const Vector2& v2);
    friend Vector2 operator/(const Vector2& v1, const Vector2& v2);
    friend Vector2 operator/(const Vector2& v1, float v2);
    friend Vector2 operator/(float v1, const Vector2& v2);

    friend Vector2 operator-(const Vector2& v1);

    bool operator==(const Vector2& v) const;
    bool operator!=(const Vector2& v) const;
};

inline Vector2::Vector2()
{
    x = y = 0.0f;
}

inline Vector2::Vector2(float x_, float y_)
{
    x = x_;
    y = y_;
}

inline void Vector2::set(float x_, float y_)
{
    x = x_;
    y = y_;
}

inline Vector2& Vector2::operator+=(const Vector2& v)
{
    x += v.x;
    y += v.y;
    return (*this);
}

inline Vector2& Vector2::operator+=(float v)
{
    x += v;
    y += v;
    return (*this);
}

inline Vector2& Vector2::operator-=(const Vector2& v)
{
    x -= v.x;
    y -= v.y;
    return (*this);
}

inline Vector2& Vector2::operator-=(float v)
{
    x -= v;
    y -= v;
    return (*this);
}

inline Vector2& Vector2::operator*=(const Vector2& v)
{
    x *= v.x;
    y *= v.y;
    return (*this);
}

inline Vector2& Vector2::operator*=(float v)
{
    x *= v;
    y *= v;
    return (*this);
}

inline Vector2& Vector2::operator/=(const Vector2& v)
{
    x /= v.x;
    y /= v.y;
    return (*this);
}

inline Vector2& Vector2::operator/=(float v)
{
    v = 1.0f / v;
    x *= v;
    y *= v;
    return (*this);
}

inline Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return Vector2(
        v1.x + v2.x,
        v1.y + v2.y);
}

inline Vector2 operator+(const Vector2& v1, float v2)
{
    return Vector2(
        v1.x + v2,
        v1.y + v2);
}

inline Vector2 operator+(float v1, const Vector2& v2)
{
    return Vector2(
        v1 + v2.x,
        v1 + v2.y);
}

inline Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
    return Vector2(
        v1.x - v2.x,
        v1.y - v2.y);
}

inline Vector2 operator-(const Vector2& v1, float v2)
{
    return Vector2(
        v1.x - v2,
        v1.y - v2);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector2 operator-(float v1, const Vector2& v2)
{
    return Vector2(
        v1 - v2.x,
        v1 - v2.y);
}

inline Vector2 operator*(const Vector2& v1, const Vector2& v2)
{
    return Vector2(
        v1.x * v2.x,
        v1.y * v2.y);
}

inline Vector2 operator*(const Vector2& v1, float v2)
{
    return Vector2(
        v1.x * v2,
        v1.y * v2);
}

inline Vector2 operator*(float v1, const Vector2& v2)
{
    return Vector2(
        v1 * v2.x,
        v1 * v2.y);
}

inline Vector2 operator/(const Vector2& v1, const Vector2& v2)
{
    return Vector2(
        v1.x / v2.x,
        v1.y / v2.y);
}

inline Vector2 operator/(const Vector2& v1, float v2)
{
    v2 = 1.0f / v2;
    return Vector2(
        v1.x * v2,
        v1.y * v2);
}

inline Vector2 operator/(float v1, const Vector2& v2)
{
    return Vector2(
        v1 / v2.x,
        v1 / v2.y);
}

inline Vector2 operator-(const Vector2& v1)
{
    return Vector2(
        -v1.x,
        -v1.y);
}

inline bool Vector2::operator==(const Vector2& v) const
{
    return (x == v.x && y == v.y);
}

inline bool Vector2::operator!=(const Vector2& v) const
{
    return (x != v.x || y != v.y);
}

inline std::ostream& operator<<(std::ostream& s, const Vector2& v)
{
    s << v.x << ' ' << v.y;
    return s;
}

} // namespace ln
