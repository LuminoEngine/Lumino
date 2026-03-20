// Copyright (c) 2018+ lriki. Distributed under the MIT license...
#pragma once

#include "Common.hpp"

namespace ln {
struct Vector3;
struct Matrix;

/**
 * クォータニオンを定義します。
 * 
 * このクォータニオンクラスの乗算の順番は左から右です。
 * つまり、q1 * q2 は、先に q1、次に q2 の回転を適用する事と同じです。
 * 
 * この乗算順序は数学的な四元数の定義からは逆行している点に注意してください。
 */
LN_STRUCT()
struct LN_API Quaternion
{
public:
    /** X 要素 */
	LN_FIELD()
    float x;

    /** Y 要素 */
	LN_FIELD()
    float y;

    /** Z要素 */
	LN_FIELD()
    float z;

    /** W 要素 */
	LN_FIELD()
    float w;

    /** 単位クォータニオン (0, 0, 0, 1) */
    static const Quaternion Identity;

public:
    /**
     * 単位クォータニオンを設定してインスタンスを初期化します。
     */
    LN_METHOD(OverloadPostfix = "Zeros")
    Quaternion();

    /**
     * 指定した値を使用してインスタンスを初期化します。
     */
    LN_METHOD()
    Quaternion(float x, float y, float z, float w);

    /**
     * 回転軸と回転角度を指定してインスタンスを初期化します。
     * @param[in]   axis    : 回転軸
     * @param[in] 	r       : 回転角度 (ラジアン単位)
     * 
     * axis が単位ベクトルでなければ正規化してから計算を行います。
     */
    LN_METHOD(OverloadPostfix = "FromAxis")
    Quaternion(const Vector3& axis, float r);

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
     * このクォータニオンの長さを返します。
     */
    float length() const;

    /**
     * このクォータニオンの長さの 2 乗を返します。
     */
    float lengthSquared() const;

    /**
     * このクォータニオンを正規化します。
     */
    void normalize();

    /**
     * このクォータニオンが単位クォータニオンであるかを判定します。
     * @return  true の場合、単位クォータニオンである
     */
    bool isIdentity() const;

    /**
     * このクォータニオンに指定されたクォータニオンを乗算します。
     * @param[in]   qua     : 乗算するクォータニオン
     */
    void multiply(const Quaternion& qua);

    /**
     * このクォータニオンを X 軸で回転します。
     * @param[in]   angle   : 回転角度 (ラジアン単位)
     */
    void rotateX(float angle);

    /**
     * このクォータニオンを Y 軸で回転します。
     * @param[in] 	angle   : 回転角度 (ラジアン単位)
     */
    void rotateY(float angle);

    /**
     * このクォータニオンを Z 軸で回転します。
     * @param[in] 	angle   : 回転角度 (ラジアン単位)
     */
    void rotateZ(float angle);

    /**
     * このクォータニオンを回転軸と角度を指定して回転します。
     * @param[in] 	axis    : 回転軸
     * @param[in]   r       : 回転角度 (ラジアン単位)
     * axis が単位ベクトルでなければ正規化してから計算を行います。
     */
    void rotateAxis(const Vector3& axis, float r);

    /**
     * このクォータニオンを XYZ 各軸のオイラー角度に変換します。
     * @param[in]   order   : 回転順序
     * @param[in] 	locked  : ジンバルロックの発生有無を格納する変数へのポインタ (ジンバルロックが発生した場合 true)
     * @return      XYZ 各軸のオイラー角度 (ラジアン単位) を格納したベクトル
     * 
     * クォータニオンは order に指定された順序で乗算されたものでなければ本来の回転角度が取得できないことに注意してください。
     * 
     * 例えば RotationYawPitchRoll() は、回転軸 Z → Y → X の順に回転を行いますが、
     * これから元の角度を取り出すには RotationOrder_ZXY を指定します。
     */
    Vector3 toEulerAngles(RotationOrder order = RotationOrder::ZXY, bool* locked = nullptr) const;

    /**
     * このクォータニオンの回転軸と回転角度を取得します。
     * @param[in]   axis        : 回転軸を格納するベクトルのポインタ
     * @param[in] 	angle       : 回転角度を格納する変数のポインタ
     * 
     * axis には単位ベクトルを格納します。
     */
    void toAxisAngle(Vector3* axis, float* angle) const;

    /**
     * 要素のいずれかが NaN または Inf かを判別します。
     */
    bool isNaNOrInf() const;

public:
    /**
     * 指定クォータニオンを正規化したクォータニオンを返します。
     * @param[in] 	qua     : 処理の基になるクォータニオン
     * @return      正規化されたクォータニオン
     */
    static Quaternion normalize(const Quaternion& qua);

    /**
     * 指定クォータニオンの共役を返します。
     * @param[in]   qua     : 処理の基になるクォータニオン
     * @return      共役クォータニオン
     */
    static Quaternion conjugate(const Quaternion& qua);

    /**
     * 2つのクォータニオンの内積を計算します。
     * @param[in] 	qua1    : 処理の基になるクォータニオン
     * @param[in] 	qua2    : 処理の基になるクォータニオン
     * @return      2つのクォータニオンの内積
     */
    static float dot(const Quaternion& qua1, const Quaternion& qua2);

    /**
     * 2つのクォータニオンの積を計算します。
     * @param[in] 	qua1    : 処理の基になるクォータニオン
     * @param[in] 	qua2    : 処理の基になるクォータニオン
     * 
     * Multiply(qua1, qua2) は、先に qua1、次に qua2 の回転を行うクォータニオンを作成します。
     */
    static Quaternion multiply(const Quaternion& qua1, const Quaternion& qua2);

    /**
     * クォータニオンを共役し、再正規化します。
     * @param[in] 	qua     : 処理の基になるクォータニオン
     * @return      逆数クォータニオン
     *
     * 逆数クォータニオンは逆行列のように、元の回転を打ち消すクォータニオンです。
     * 次のようにすると、回転を打ち消し合い Identity の値と一致します。
     * ```
     * q *= Quaternion::makeInverse(q);
     * ```
     */
    static Quaternion makeInverse(const Quaternion& qua);

    /**
     * 回転軸と角度を指定してクォータニオンを作成します。
     * @param[in] 	axis    : 回転軸
     * @param[in] 	r       : 回転角度 (ラジアン単位)
     * @return      新しく作成されたクォータニオン
     * 
     * axis が単位ベクトルでなければ正規化してから計算を行います。
     */
    static Quaternion makeFromRotationAxis(const Vector3& axis, float r);

    /**
     * 回転行列からクォータニオンを作成します。
     * @param[in] 	mat     : 処理の基になる行列
     * @return      新しく作成されたクォータニオン
     */
    static Quaternion makeFromRotationMatrix(const Matrix& mat);

    /**
     * ヨー、ピッチ、ロールを指定してクォータニオンを作成します。
     * @param[in]   yaw     : y 軸を中心とするヨー (ラジアン単位)
     * @param[in] 	pitch   : x 軸を中心とするピッチ (ラジアン単位)
     * @param[in] 	roll    : z 軸を中心とするロール (ラジアン単位)
     * @return		新しく作成されたクォータニオン
	 * 
     * 回転順序は Z → X → Y です。引数の並びとは逆順であることに注意してください。
     * これは  D3DXQuaternionRotationYawPitchRoll の回転順序と同じで、
     * RotationEulerAngles に RotationOrder_ZXY を指定して計算した結果とも同じです。
     */
    static Quaternion makeFromYawPitchRoll(float yaw, float pitch, float roll);

    /**
     * XYZ 各軸のオイラー角からクォータニオンを作成します。
     * @param[in] 	angles  : XYZ 各軸の回転角度 (ラジアン単位)
     * @param[in] 	order   : 回転順序
     * @return      演算結果のクォータニオン
     */
    static Quaternion makeFromEulerAngles(const Vector3& angles, RotationOrder order = RotationOrder::ZXY);
    static Quaternion makeFromEulerAngles(float x, float y, float z, RotationOrder order = RotationOrder::ZXY) { return makeFromEulerAngles(Vector3(x, y, z), order); }

    /**
     * 球面線形補間を使用して、2 つのクオータニオン間を補間します。
     * @param[in] 	qua1    : 処理の基になるクオータニオン (t = 0.0 のときの値)
     * @param[in] 	qua2    : 処理の基になるクオータニオン (t = 1.0 のときの値)
     * @param[in]	t       : 加重係数
     * @return      補間結果のクォータニオン
     * 
     * t は通常、0.0～1.0 を指定します。
     */
    static Quaternion slerp(const Quaternion& qua1, const Quaternion& qua2, float t);

    /**
     * 指定した方向を向くように回転するクオータニオンを作成します。
     * @param[in] 	forward : 向かせたい方向
     * @param[in] 	up      : 上方向
     * @return      演算結果のクォータニオン
     */
    static Quaternion lookRotation(const Vector3& forward, const Vector3& up = Vector3::UnitY);

public:
    Quaternion& operator+=(const Quaternion& v);
    Quaternion& operator+=(float v);
    Quaternion& operator-=(const Quaternion& v);
    Quaternion& operator-=(float v);
    Quaternion& operator*=(const Quaternion& v); ///< this->Multiply(v); と同じです。
    Quaternion& operator*=(float v);
    Quaternion& operator/=(float v);

    friend Quaternion operator+(const Quaternion& v1, const Quaternion& v2);
    friend Quaternion operator+(const Quaternion& v1, float v2);
    friend Quaternion operator+(float v1, const Quaternion& v2);
    friend Quaternion operator-(const Quaternion& v1, const Quaternion& v2);
    friend Quaternion operator-(const Quaternion& v1, float v2);
    friend Quaternion operator-(float v1, const Quaternion& v2);
    friend Quaternion operator*(const Quaternion& v1, const Quaternion& v2); ///< Quaternion::Multiply(v1, v2); と同じです。
    friend Quaternion operator*(const Quaternion& v1, float v2);
    friend Quaternion operator*(float v1, const Quaternion& v2);
    friend Quaternion operator/(const Quaternion& v1, float v2);

    friend Quaternion operator-(const Quaternion& v1);

    bool operator==(const Quaternion& v) const;
    bool operator!=(const Quaternion& v) const;
};

inline Quaternion::Quaternion()
{
    x = y = z = 0.0f;
    w = 1.0f;
}

inline Quaternion::Quaternion(float x_, float y_, float z_, float w_)
{
    x = x_;
    y = y_;
    z = z_;
    w = w_;
}

inline void Quaternion::set(float x_, float y_, float z_, float w_)
{
    x = x_;
    y = y_;
    z = z_;
    w = w_;
}

inline bool Quaternion::isIdentity() const
{
    return (x == 0.0f && y == 0.0f && z == 0.0f && w == 1.0f);
}

inline Quaternion& Quaternion::operator+=(const Quaternion& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return (*this);
}

inline Quaternion& Quaternion::operator+=(float v)
{
    x += v;
    y += v;
    z += v;
    w += v;
    return (*this);
}

inline Quaternion& Quaternion::operator-=(const Quaternion& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return (*this);
}

inline Quaternion& Quaternion::operator-=(float v)
{
    x -= v;
    y -= v;
    z -= v;
    w -= v;
    return (*this);
}

inline Quaternion& Quaternion::operator*=(const Quaternion& v)
{
    this->multiply(v);
    return (*this);
}

inline Quaternion& Quaternion::operator*=(float v)
{
    x *= v;
    y *= v;
    z *= v;
    w *= v;
    return (*this);
}

inline Quaternion& Quaternion::operator/=(float v)
{
    v = 1.0f / v;
    x *= v;
    y *= v;
    z *= v;
    w *= v;
    return (*this);
}

inline Quaternion operator+(const Quaternion& v1, const Quaternion& v2)
{
    return Quaternion(
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z,
        v1.w + v2.w);
}

inline Quaternion operator+(const Quaternion& v1, float v2)
{
    return Quaternion(
        v1.x + v2,
        v1.y + v2,
        v1.z + v2,
        v1.w + v2);
}

inline Quaternion operator+(float v1, const Quaternion& v2)
{
    return Quaternion(
        v1 + v2.x,
        v1 + v2.y,
        v1 + v2.z,
        v1 + v2.w);
}

inline Quaternion operator-(const Quaternion& v1, const Quaternion& v2)
{
    return Quaternion(
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z,
        v1.w - v2.w);
}

inline Quaternion operator-(const Quaternion& v1, float v2)
{
    return Quaternion(
        v1.x - v2,
        v1.y - v2,
        v1.z - v2,
        v1.w - v2);
}

inline Quaternion operator-(float v1, const Quaternion& v2)
{
    return Quaternion(
        v1 - v2.x,
        v1 - v2.y,
        v1 - v2.z,
        v1 - v2.w);
}

inline Quaternion operator*(const Quaternion& v1, const Quaternion& v2)
{
    return Quaternion::multiply(v1, v2);
}

inline Quaternion operator*(const Quaternion& v1, float v2)
{
    return Quaternion(
        v1.x * v2,
        v1.y * v2,
        v1.z * v2,
        v1.w * v2);
}

inline Quaternion operator*(float v1, const Quaternion& v2)
{
    return Quaternion(
        v1 * v2.x,
        v1 * v2.y,
        v1 * v2.z,
        v1 * v2.w);
}

inline Quaternion operator/(const Quaternion& v1, float v2)
{
    v2 = 1.0f / v2;
    return Quaternion(
        v1.x * v2,
        v1.y * v2,
        v1.z * v2,
        v1.w * v2);
}

inline Quaternion operator-(const Quaternion& v1)
{
    return Quaternion(
        -v1.x,
        -v1.y,
        -v1.z,
        -v1.w);
}

inline bool Quaternion::operator==(const Quaternion& v) const
{
    return (x == v.x && y == v.y && z == v.z && w == v.w);
}

inline bool Quaternion::operator!=(const Quaternion& v) const
{
    return (x != v.x || y != v.y || z != v.z || w != v.w);
}

inline std::ostream& operator<<(std::ostream& s, const Quaternion& v)
{
	s << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w;
	return s;
}

} // namespace ln
