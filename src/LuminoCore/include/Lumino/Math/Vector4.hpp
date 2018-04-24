// Copyright (c) 2018 lriki. Distributed under the MIT license.
#pragma once

#include "Common.hpp"

namespace ln {
struct Vector2;
struct Vector3;
struct Matrix;

/**
 *  4次元のベクトルを定義します。
 */
struct LN_API Vector4
{
public:
    /** X 要素 */
    float x;

    /** Y 要素 */
    float y;

    /** Z 要素 */
    float z;

    /** W 要素 */
    float w;

    /** Vector4(0, 0, 0, 0) */
    static const Vector4 Zero;

public:
    /**
     * すべての要素を 0.0 に設定してインスタンスを初期化します。
     */
    Vector4();

    /**
     * 指定した値を使用してインスタンスを初期化します。
     */
    Vector4(float x, float y, float z, float w);

    /**
     * Vector2 と z, w 値を指定してインスタンスを初期化します。
     */
    Vector4(const Vector2& vec, float z, float w);

    /**
     * Vector3 と w 値を指定してインスタンスを初期化します。
     */
    Vector4(const Vector3& vec, float w);

public:
    /**
     * 各要素に値を設定します。
     */
    void set(float x, float y, float z, float w);

	/** Returns pointer to the first element. */
	float* data() LN_NOEXCEPT { return &x; }

	/** Returns pointer to the first element. */
	const float* data() const LN_NOEXCEPT { return &x; }

    /**
     * このベクトルの X Y 要素を Vector2 として返します。
     */
    const Vector2& xy() const;

    /**
     * このベクトルの X Y Z 要素を Vector3 として返します。
     */
    const Vector3& xyz() const;

    /**
     * ベクトルの長さを返します。
     */
    float length() const;

    /**
     * ベクトルの長さの 2 乗を返します。
     */
    float lengthSquared() const;

    /**
     * このベクトルを指定された最大値と最小値の範囲にクランプします。
     */
    void clamp(float min_xyzw, float max_xyzw);

    /**
     * このベクトルを指定された最大値と最小値の範囲にクランプします。
     * @param[in]   minVec      : 最小値
     * @param[in]   maxVec      : 最大値
     */
    void clamp(const Vector4& minVec, const Vector4& maxVec);

    /**
     * 要素のいずれかが NaN または Inf かを判別します。
     */
    bool isNaNOrInf() const;

public:
    /**
     * 指定ベクトルを正規化したベクトルを返します。
     * @param[in]   vec         : 処理の基になるベクトル
     * @return      正規化されたベクトル
     */
    static Vector4 normalize(const Vector4& vec);

    /**
     * 2つのベクトルの内積を計算します。
     * @param[in]   vec1        : 処理の基になるベクトル
     * @param[in]   vec2        : 処理の基になるベクトル
     * @return      2つのベクトルの内積
     */
    static float dot(const Vector4& vec1, const Vector4& vec2);

    /**
     * 2つのベクトルの最小値で構成されるベクトルを返します。
     * @param[in]   vec1        : 処理の基になるベクトル
     * @param[in]   vec2        : 処理の基になるベクトル
     * @return      最小値から作成されたベクトル
     */
    static Vector4 min(const Vector4& vec1, const Vector4& vec2);

    /**
     * 2つのベクトルの最大値で構成されるベクトルを返します。
     * @param[in]   vec1       : 処理の基になるベクトル
     * @param[in]   vec2         : 処理の基になるベクトル
     * @return      最大値から作成されたベクトル
     */
    static Vector4 max(const Vector4& vec1, const Vector4& vec2);

    /**
     * 指定された行列を使用してベクトルを座標変換します。
     * @param[in]   vec        : 処理の基になるベクトル
     * @param[in]   mat        : 処理の基になる行列
     * @return      変換されたベクトル
     */
    static Vector4 transform(const Vector4& vec, const Matrix& mat);

    /**
     * 2 つのベクトル間の線形補間を行います。
     * @param[in]   start       : 開始ベクトル (t = 0.0 のときの値)
     * @param[in]   end         : 終了ベクトル (t = 1.0 のときの値)
     * @param[in]   t           : 加重係数
     * @return      補間結果のベクトル
     * t は通常、0.0～1.0 を指定します。
     */
    static Vector4 lerp(const Vector4& start, const Vector4& end, float t);

    /**
     * 指定されたベクトルを使用して エルミートスプライン補間を実行します。
     * @param[in]   v1      : 開始ベクトル
     * @param[in]   a1      : 開始ベクトルの接線ベクトル(速度)
     * @param[in]   v2      : 終了ベクトル
     * @param[in]   a2      : 終了ベクトルの接線ベクトル(速度)
     * @param[in]   t       : 加重係数
     * @return      補間結果の値
     * t は通常、0.0～1.0 を指定します。
     */
    static Vector4 hermite(const Vector4& v1, const Vector4& a1, const Vector4& v2, const Vector4& a2, float t);

    /**
     * 指定されたベクトルを使用して Catmull-Rom 補間を行います。
     * @param[in]   vec1    : 1番目の位置
     * @param[in]   vec2    : 2番目の位置 (t = 0.0 のときの値)
     * @param[in]   vec3    : 3番目の位置 (t = 1.0 のときの値)
     * @param[in]   vec4    : 4番目の位置
     * @param[in]   t       : 加重係数
     * @return      補間結果のベクトル
     * t は通常、0.0～1.0 を指定します。
     */
    static Vector4 catmullRom(const Vector4& vec1, const Vector4& vec2, const Vector4& vec3, const Vector4& vec4, float t);

public:
    Vector4& operator+=(const Vector4& v);
    Vector4& operator+=(float v);
    Vector4& operator-=(const Vector4& v);
    Vector4& operator-=(float v);
    Vector4& operator*=(const Vector4& v);
    Vector4& operator*=(float v);
    Vector4& operator/=(const Vector4& v);
    Vector4& operator/=(float v);

    friend Vector4 operator+(const Vector4& v1, const Vector4& v2);
    friend Vector4 operator+(const Vector4& v1, float v2);
    friend Vector4 operator+(float v1, const Vector4& v2);
    friend Vector4 operator-(const Vector4& v1, const Vector4& v2);
    friend Vector4 operator-(const Vector4& v1, float v2);
    friend Vector4 operator-(float v1, const Vector4& v2);
    friend Vector4 operator*(const Vector4& v1, const Vector4& v2);
    friend Vector4 operator*(const Vector4& v1, float v2);
    friend Vector4 operator*(float v1, const Vector4& v2);
    friend Vector4 operator/(const Vector4& v1, const Vector4& v2);
    friend Vector4 operator/(const Vector4& v1, float v2);
    friend Vector4 operator/(float v1, const Vector4& v2);

    friend Vector4 operator-(const Vector4& v1);

    bool operator==(const Vector4& v) const;
    bool operator!=(const Vector4& v) const;
};

inline Vector4::Vector4()
{
    x = y = z = w = 0.0f;
}

inline Vector4::Vector4(float x_, float y_, float z_, float w_)
{
    x = x_;
    y = y_;
    z = z_;
    w = w_;
}

inline void Vector4::set(float x_, float y_, float z_, float w_)
{
    x = x_;
    y = y_;
    z = z_;
    w = w_;
}

inline const Vector2& Vector4::xy() const
{
    return *reinterpret_cast<const Vector2*>(this);
}

inline const Vector3& Vector4::xyz() const
{
    return *reinterpret_cast<const Vector3*>(this);
}

inline Vector4& Vector4::operator+=(const Vector4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return (*this);
}

inline Vector4& Vector4::operator+=(float v)
{
    x += v;
    y += v;
    z += v;
    w += v;
    return (*this);
}

inline Vector4& Vector4::operator-=(const Vector4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return (*this);
}

inline Vector4& Vector4::operator-=(float v)
{
    x -= v;
    y -= v;
    z -= v;
    w -= v;
    return (*this);
}

inline Vector4& Vector4::operator*=(const Vector4& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return (*this);
}

inline Vector4& Vector4::operator*=(float v)
{
    x *= v;
    y *= v;
    z *= v;
    w *= v;
    return (*this);
}

inline Vector4& Vector4::operator/=(const Vector4& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return (*this);
}

inline Vector4& Vector4::operator/=(float v)
{
    v = 1.0f / v;
    x *= v;
    y *= v;
    z *= v;
    w *= v;
    return (*this);
}

inline Vector4 operator+(const Vector4& v1, const Vector4& v2)
{
    return Vector4(
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z,
        v1.w + v2.w);
}

inline Vector4 operator+(const Vector4& v1, float v2)
{
    return Vector4(
        v1.x + v2,
        v1.y + v2,
        v1.z + v2,
        v1.w + v2);
}

inline Vector4 operator+(float v1, const Vector4& v2)
{
    return Vector4(
        v1 + v2.x,
        v1 + v2.y,
        v1 + v2.z,
        v1 + v2.w);
}

inline Vector4 operator-(const Vector4& v1, const Vector4& v2)
{
    return Vector4(
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z,
        v1.w - v2.w);
}

inline Vector4 operator-(const Vector4& v1, float v2)
{
    return Vector4(
        v1.x - v2,
        v1.y - v2,
        v1.z - v2,
        v1.w - v2);
}

inline Vector4 operator-(float v1, const Vector4& v2)
{
    return Vector4(
        v1 - v2.x,
        v1 - v2.y,
        v1 - v2.z,
        v1 - v2.w);
}

inline Vector4 operator*(const Vector4& v1, const Vector4& v2)
{
    return Vector4(
        v1.x * v2.x,
        v1.y * v2.y,
        v1.z * v2.z,
        v1.w * v2.w);
}

inline Vector4 operator*(const Vector4& v1, float v2)
{
    return Vector4(
        v1.x * v2,
        v1.y * v2,
        v1.z * v2,
        v1.w * v2);
}

inline Vector4 operator*(float v1, const Vector4& v2)
{
    return Vector4(
        v1 * v2.x,
        v1 * v2.y,
        v1 * v2.z,
        v1 * v2.w);
}

inline Vector4 operator/(const Vector4& v1, const Vector4& v2)
{
    return Vector4(
        v1.x / v2.x,
        v1.y / v2.y,
        v1.z / v2.z,
        v1.w / v2.w);
}

inline Vector4 operator/(const Vector4& v1, float v2)
{
    v2 = 1.0f / v2;
    return Vector4(
        v1.x * v2,
        v1.y * v2,
        v1.z * v2,
        v1.w * v2);
}

inline Vector4 operator/(float v1, const Vector4& v2)
{
    return Vector4(
        v1 / v2.x,
        v1 / v2.y,
        v1 / v2.z,
        v1 / v2.w);
}

inline Vector4 operator-(const Vector4& v1)
{
    return Vector4(
        -v1.x,
        -v1.y,
        -v1.z,
        -v1.w);
}

inline bool Vector4::operator==(const Vector4& v) const
{
    return (x == v.x && y == v.y && z == v.z && w == v.w);
}

inline bool Vector4::operator!=(const Vector4& v) const
{
    return (x != v.x || y != v.y || z != v.z || w != v.w);
}

inline std::ostream& operator<<(std::ostream& s, const Vector4& v)
{
	s << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w;
	return s;
}

} // namespace ln
