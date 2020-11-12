// Copyright (c) 2018+ lriki. Distributed under the MIT license...
#pragma once

#include <stdio.h>
#include "Common.hpp"

namespace ln {
struct Vector2;
struct Vector4;
struct Quaternion;
struct Matrix;

/**
 * 3次元のベクトルを定義します。
 */
LN_STRUCT()
struct LN_API Vector3
{
public:
    /** X 要素 */
    LN_FIELD()
    float x;

    /** Y 要素 */
    LN_FIELD()
    float y;

    /** Z 要素 */
    LN_FIELD()
    float z;

    /** Vector3(0, 0, 0) */
    static const Vector3 Zero;

    /** Vector3(1, 0, 0) */
    static const Vector3 UnitX;

    /** Vector3(0, 1, 0) */
    static const Vector3 UnitY;

    /** Vector3(0, 0, 1) */
    static const Vector3 UnitZ;

    /** Vector3(1, 1, 1) */
    static const Vector3 Ones;

    /** Vector3(FLT_MIN, FLT_MIN, FLT_MIN) */
    static const Vector3 Minimum;

    /** Vector3(FLT_MAX, FLT_MAX, FLT_MAX) */
    static const Vector3 Maximum;

public:
    /** すべての要素を 0.0 に設定してインスタンスを初期化します。 */
    LN_METHOD(OverloadPostfix = "Zeros")
    Vector3();

    /** 指定した値を使用してインスタンスを初期化します。 */
    LN_METHOD()
    Vector3(float x, float y, float z);

    /** Vector2 と z 値を指定してインスタンスを初期化します。 */
    Vector3(const Vector2& vec, float z);

public:
    /**
     * 各要素に値を設定します。
     */
    void set(float x, float y, float z);

    /** get */
    LN_METHOD()
    void get(float* outX, float* outY, float* outZ) const { *outX = x; *outY = y; *outZ = z; }

	/** Returns pointer to the first element. */
	float* data() LN_NOEXCEPT { return &x; }

	/** Returns pointer to the first element. */
	const float* data() const LN_NOEXCEPT { return &x; }

    /**
     * このベクトルの X Y 要素を Vector2 として返します。
     */
    const Vector2& xy() const;

    /**
     * ベクトルの長さを返します。
     */
    LN_METHOD()
    float length() const;

    /**
     * ベクトルの長さの 2 乗を返します。
     */
    LN_METHOD()
    float lengthSquared() const;

	bool isZero() const { return !x && !y && !z; }

    /**
     * このベクトルを正規化します。
     * 
     * ベクトルの長さが 0 の場合は正規化を行いません。
     */
    LN_METHOD()
    void mutatingNormalize();

    /**
     * このベクトルを指定された最大値と最小値の範囲にクランプします。
     * @param[in]   minVec  : 最小値
     * @param[in]   maxVec  : 最大値
     */
    void clamp(const Vector3& minVec, const Vector3& maxVec);

    /**
     * 指定された行列を使用してこのベクトルを座標変換します。
     * @param[in]   mat     : 処理の基になる行列
     * 
     * ベクトルを (X, Y, Z, 1.0) として座標変換を行い、結果を w で除算します。
     */
    void transformCoord(const Matrix& mat);

    void transformDirection(const Matrix& mat);

    /**
     * 要素のいずれかが NaN または Inf かを判別します。
     */
    bool isNaNOrInf() const;

    /**
     * デバッグ用に文字列を標準出力します。
     * @param[in]   format  : 書式指定文字列
     * @param[in]   stream  : 出力先ストリーム
     * 
     * format が NULL の場合、書式は "%f, %f, %f\n" を使用します。
     */
    void print(const char* format = NULL, FILE* stream = NULL) const;

public:
    /**
     * 指定ベクトルを正規化したベクトルを返します。
     * @param[in]   x       : 処理の基になるベクトルの X 要素
     * @param[in]   y       : 処理の基になるベクトルの Y 要素
     * @param[in]   z       : 処理の基になるベクトルの Z 要素
     * @return      正規化されたベクトル
     */
    LN_METHOD(OverloadPostfix = "XYZ")
    static Vector3 normalize(float x, float y, float z);

    /**
     * 指定ベクトルを正規化したベクトルを返します。
     * @param[in]   vec     : 処理の基になるベクトル
     * @return      正規化されたベクトル
     */
    LN_METHOD()
    static Vector3 normalize(const Vector3& vec);

    static Vector3 safeNormalize(const Vector3& vec);
    static Vector3 safeNormalize(const Vector3& vec, const Vector3& alt);

    /** 指定した値をすべての要素に設定します。 */
    static Vector3 replicate(float value);

    /**
     * 2つのベクトル間の距離を計算します。
     * @param[in]   vec1    : 処理の基になるベクトル
     * @param[in]   vec2    : 処理の基になるベクトル
     * @return      2つのベクトル間の距離
     */
    static float distance(const Vector3& vec1, const Vector3& vec2);

    /**
     * 2つのベクトル間の距離の2乗を計算します。
     * @param[in]   vec1    : 処理の基になるベクトル
     * @param[in]   vec2    : 処理の基になるベクトル
     * @return      2つのベクトル間の距離の2乗
     */
    static float distanceSquared(const Vector3& vec1, const Vector3& vec2);

    /**
     * 2つのベクトルの内積を計算します。
     * @param[in]   vec1    : 処理の基になるベクトル
     * @param[in]   vec2    : 処理の基になるベクトル
     * @return      2つのベクトルの内積
     * @note
     *   正規化された方向ベクトルの内積は、2つのベクトルのコサインを表します。
     *   例えば、向きが完全に一致している場合は 1 を、反対の場合は -1 を返します。
     */
    static float dot(const Vector3& vec1, const Vector3& vec2);

    /**
     * 2つのベクトルの外積を計算します。
     * @param[in]   vec1    : 処理の基になるベクトル
     * @param[in]   vec2    : 処理の基になるベクトル
     * @return      2つのベクトルの外積
     */
    static Vector3 cross(const Vector3& vec1, const Vector3& vec2);

    /**
     * 2つのベクトルの最小値で構成されるベクトルを返します。
     * @param[in]   vec1    : 処理の基になるベクトル
     * @param[in]   vec2    : 処理の基になるベクトル
     * @return      最小値から作成されたベクトル
     */
    static Vector3 min(const Vector3& vec1, const Vector3& vec2);

    /**
     * 指定された複数のベクトルの最小値で構成されるベクトルを返します。
     * @param[in]   vectors : 処理の基になるベクトルの配列
     * @param[in]   count   : 処理の基になるベクトルの数
     * @return      最小値から作成されたベクトル
     */
    static Vector3 min(const Vector3* vectors, int count);

    /**
     * 2つのベクトルの最大値で構成されるベクトルを返します。
     * @param[in]   vec1    : 処理の基になるベクトル
     * @param[in]   vec2    : 処理の基になるベクトル
     * @return      最大値から作成されたベクトル
     */
    static Vector3 max(const Vector3& vec1, const Vector3& vec2);

    /**
     * 指定された複数のベクトルの最大値で構成されるベクトルを返します。
     * @param[in]   vectors : 処理の基になるベクトルの配列
     * @param[in]   count   : 処理の基になるベクトルの数
     * @return      最大値から作成されたベクトル
     */
    static Vector3 max(const Vector3* vectors, int count);

    /**
     * 入射ベクトルと法線ベクトルから反射ベクトルを計算します。
     * @param[in]   vec     : 入射ベクトル
     * @param[in]   normal  : 法線ベクトル
     * @return      反射ベクトル
     */
    static Vector3 reflect(const Vector3& vec, const Vector3& normal);

    /**
     * 入射ベクトルと法線ベクトルから滑りベクトルを計算します。
     * @param[in]   vec     : 入射ベクトル
     * @param[in]   normal  : 法線ベクトル
     * @return      滑りベクトル
     */
    static Vector3 slide(const Vector3& vec, const Vector3& normal);

    /**
     * 指定されたクォータニオンを使用してベクトルを座標変換します。
     * @param[in]   vec     : 処理の基になるベクトル
     * @param[in]   qua     : 処理の基になるクォータニオン
     * @return      変換されたベクトル
     */
    static Vector3 transform(const Vector3& vec, const Quaternion& qua);

    /**
     * 指定された行列を使用してベクトルを座標変換します。
     * @param[in]   vec     : 処理の基になるベクトル
     * @param[in]   mat     : 処理の基になる行列
     * @return      変換されたベクトル
     */
    static Vector4 transform(const Vector3& vec, const Matrix& mat);

    /**
     * 指定された行列を使用してベクトルを座標変換します。
     * @param[in]   vec     : 処理の基になるベクトル
     * @param[in]   mat     : 処理の基になる行列
     * @return      変換されたベクトル
     * 
     * ベクトルを (X, Y, Z, 1.0) として座標変換を行い、結果を w で除算します。
     */
    static Vector3 transformCoord(const Vector3& vec, const Matrix& mat);



    /**
     * 2 つのベクトル間の線形補間を行います。
     * @param[in]   start   : 開始ベクトル (t = 0.0 のときの値)
     * @param[in]   end     : 終了ベクトル (t = 1.0 のときの値)
     * @param[in]   t       : 加重係数
     * @return      補間結果のベクトル
     * 
     * t は通常、0.0～1.0 を指定します。
     */
    static Vector3 lerp(const Vector3& start, const Vector3& end, float t);

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
    static Vector3 hermite(const Vector3& v1, const Vector3& a1, const Vector3& v2, const Vector3& a2, float t);

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
    static Vector3 catmullRom(const Vector3& vec1, const Vector3& vec2, const Vector3& vec3, const Vector3& vec4, float t);

    /**
     * オブジェクト空間(3D空間)のベクトルをスクリーン空間(2D空間)のベクトルに変換する。
     * @param[in]   point           : オブジェクト空間上の座標
     * @param[in]   worldViewProj   : 結合済みの ワールド - ビュー - プロジェクション行列
     * @param[in]   x               : ビューポートの左上 X 座標
     * @param[in]   y               : ビューポートの左上 Y 座標
     * @param[in]   width           : ビューポートの幅
     * @param[in]   height          : ビューポートの高さ
     * @param[in]   minZ            : ビューポートの最小深度
     * @param[in]   maxZ            : ビューポートの最大深度
     */
    static Vector3 project(const Vector3& point, const Matrix& worldViewProj, float x, float y, float width, float height, float minZ = 0.0f, float maxZ = 1.0f);

    /**
     * スクリーン空間(2D空間)のベクトルをオブジェクト空間(3D空間)のベクトルに変換する。
     * @param[in]   point           : スクリーン空間上の座標
     * @param[in]   worldViewProj   : 結合済みの ワールド - ビュー - プロジェクション行列
     * @param[in]   x               : ビューポートの左上 X 座標
     * @param[in]   y               : ビューポートの左上 Y 座標
     * @param[in]   width           : ビューポートの幅
     * @param[in]   height          : ビューポートの高さ
     * @param[in]   minZ            : ビューポートの最小深度
     * @param[in]   maxZ            : ビューポートの最大深度
     */
    static Vector3 unproject(const Vector3& point, const Matrix& worldViewProj, float x, float y, float width, float height, float minZ = 0.0f, float maxZ = 1.0f);

    static bool nearEqual(const Vector3& value1, const Vector3& value2);

    static Vector3 sign(const Vector3& value);

public:
    Vector3& operator+=(const Vector3& v);
    Vector3& operator+=(float v);
    Vector3& operator-=(const Vector3& v);
    Vector3& operator-=(float v);
    Vector3& operator*=(const Vector3& v);
    Vector3& operator*=(float v);
    Vector3& operator/=(const Vector3& v);
    Vector3& operator/=(float v);

    friend Vector3 operator+(const Vector3& v1, const Vector3& v2);
    friend Vector3 operator+(const Vector3& v1, float v2);
    friend Vector3 operator+(float v1, const Vector3& v2);
    friend Vector3 operator-(const Vector3& v1, const Vector3& v2);
    friend Vector3 operator-(const Vector3& v1, float v2);
    friend Vector3 operator-(float v1, const Vector3& v2);
    friend Vector3 operator*(const Vector3& v1, const Vector3& v2);
    friend Vector3 operator*(const Vector3& v1, float v2);
    friend Vector3 operator*(float v1, const Vector3& v2);
    friend Vector3 operator/(const Vector3& v1, const Vector3& v2);
    friend Vector3 operator/(const Vector3& v1, float v2);
    friend Vector3 operator/(float v1, const Vector3& v2);

    friend Vector3 operator-(const Vector3& v1);

    bool operator==(const Vector3& v) const;
    bool operator!=(const Vector3& v) const;

#ifdef LN_MATH_VECTOR3_EXTENSION
    LN_MATH_VECTOR3_EXTENSION
#endif
};

inline Vector3::Vector3()
{
    x = y = z = 0.0f;
}

inline Vector3::Vector3(float x_, float y_, float z_)
{
    x = x_;
    y = y_;
    z = z_;
}

inline void Vector3::set(float x_, float y_, float z_)
{
    x = x_;
    y = y_;
    z = z_;
}

inline const Vector2& Vector3::xy() const
{
    return *reinterpret_cast<const Vector2*>(this);
}

inline float Vector3::lengthSquared() const
{
    return x * x + y * y + z * z;
}

inline Vector3& Vector3::operator+=(const Vector3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return (*this);
}

inline Vector3& Vector3::operator+=(float v)
{
    x += v;
    y += v;
    z += v;
    return (*this);
}

inline Vector3& Vector3::operator-=(const Vector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return (*this);
}

inline Vector3& Vector3::operator-=(float v)
{
    x -= v;
    y -= v;
    z -= v;
    return (*this);
}

inline Vector3& Vector3::operator*=(const Vector3& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return (*this);
}

inline Vector3& Vector3::operator*=(float v)
{
    x *= v;
    y *= v;
    z *= v;
    return (*this);
}

inline Vector3& Vector3::operator/=(const Vector3& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return (*this);
}

inline Vector3& Vector3::operator/=(float v)
{
    v = 1.0f / v;
    x *= v;
    y *= v;
    z *= v;
    return (*this);
}

inline Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
    return Vector3(
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z);
}

inline Vector3 operator+(const Vector3& v1, float v2)
{
    return Vector3(
        v1.x + v2,
        v1.y + v2,
        v1.z + v2);
}

inline Vector3 operator+(float v1, const Vector3& v2)
{
    return Vector3(
        v1 + v2.x,
        v1 + v2.y,
        v1 + v2.z);
}

inline Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
    return Vector3(
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z);
}

inline Vector3 operator-(const Vector3& v1, float v2)
{
    return Vector3(
        v1.x - v2,
        v1.y - v2,
        v1.z - v2);
}

inline Vector3 operator-(float v1, const Vector3& v2)
{
    return Vector3(
        v1 - v2.x,
        v1 - v2.y,
        v1 - v2.z);
}

inline Vector3 operator*(const Vector3& v1, const Vector3& v2)
{
    return Vector3(
        v1.x * v2.x,
        v1.y * v2.y,
        v1.z * v2.z);
}

inline Vector3 operator*(const Vector3& v1, float v2)
{
    return Vector3(
        v1.x * v2,
        v1.y * v2,
        v1.z * v2);
}

inline Vector3 operator*(float v1, const Vector3& v2)
{
    return Vector3(
        v1 * v2.x,
        v1 * v2.y,
        v1 * v2.z);
}

inline Vector3 operator/(const Vector3& v1, const Vector3& v2)
{
    return Vector3(
        v1.x / v2.x,
        v1.y / v2.y,
        v1.z / v2.z);
}

inline Vector3 operator/(const Vector3& v1, float v2)
{
    v2 = 1.0f / v2;
    return Vector3(
        v1.x * v2,
        v1.y * v2,
        v1.z * v2);
}

inline Vector3 operator/(float v1, const Vector3& v2)
{
    return Vector3(
        v1 / v2.x,
        v1 / v2.y,
        v1 / v2.z);
}

inline Vector3 operator-(const Vector3& v1)
{
    return Vector3(
        -v1.x,
        -v1.y,
        -v1.z);
}

inline bool Vector3::operator==(const Vector3& v) const
{
    return (x == v.x && y == v.y && z == v.z);
}

inline bool Vector3::operator!=(const Vector3& v) const
{
    return (x != v.x || y != v.y || z != v.z);
}

inline std::ostream& operator<<(std::ostream& s, const Vector3& v)
{
	s << v.x << ' ' << v.y << ' ' << v.z;
	return s;
}

} // namespace ln
