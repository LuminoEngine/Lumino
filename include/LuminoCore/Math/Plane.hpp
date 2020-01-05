// Copyright (c) 2018+ lriki. Distributed under the MIT license..
#pragma once

#include "Common.hpp"
#include "Vector3.hpp"

namespace ln {
struct Ray;

/** 平面を定義します。 */
struct LN_API Plane
{
public:
    /** Plane の法線 ベクトル */
    Vector3 normal;

    /** 原点からの法線に沿った Plane の距離 */
    float distance;

public:
    /** 各要素に 0.0 を設定してインスタンスを初期化します。 */
    Plane();

    /**
     * 各要素を指定してインスタンスを初期化します。
     * @param[in]   a   : 法線の X 成分
     * @param[in]   b   : 法線の Y 成分
     * @param[in]   c   : 法線の Z 成分
     * @param[in]   d   : 原点からの法線に沿った Plane の距離
     */
    Plane(float a, float b, float c, float d);

    Plane(const Vector3& normal, float d);

    Plane(const Vector3& normal);

    /**
     * 平面上の1点と法線を指定してインスタンスを初期化します。
     * @param[in]   point   : 平面上の点
     * @param[in]   normal  : 法線 (正規化済みであること)
     */
    Plane(const Vector3& point, const Vector3& normal);

    /**
     * 平面上の3点を指定してインスタンスを初期化します。
     * @param[in]   point1  : 平面上の点1
     * @param[in]   point2  : 平面上の点2
     * @param[in]   point3  : 平面上の点3
     */
    Plane(const Vector3& point1, const Vector3& point2, const Vector3& point3);

public:
    /**
     * この平面を正規化します。
     * 
     * 面法線 Normal を正規化し、それに使用した係数を distance にも適用します。
     */
    void normalize();

    /**
     * 点がこの平面の裏側にあるかを判定します。点が面上にある場合は表側と判定します。
     * @param[in]   point   : 判定する点の座標
     * @return      true の場合、裏側にある

     * 面は法線が伸びている方が表側です。
     */
    bool checkInside(const Vector3& point) const;

    /**
     * 点がこの平面の裏側にあるかを判定します。点が面上にある場合も裏側と判定します。
     * @param[in]   point   : 判定する点の座標
     * @return      true の場合、裏側にある
     */
    bool checkInsideLower(const Vector3& point) const;

    /**
     * 球がこの平面の裏側にあるかを判定します。
     * @param[in]   point   : 球の中心座標
     * @param[in]   radius  : 球の半径
     * @return      true の場合、裏側にある (接触している場合も true)
     *
     * 面は法線が伸びている方が表側です。
     */
    bool checkInside(const Vector3& point, float radius) const;

    /**
     * この平面と線分が交差するかを判定します。
     * @param[in]   start   : 線分の始点
     * @param[in]   end     : 線分の終点
     * @param[in]   point   : 交点の座標を格納するベクトルのポインタ
     * @return      交差する場合は true、平行な場合は false
     */
    bool intersects(const Vector3& start, const Vector3& end, Vector3* point = nullptr) const;

    /**
     * この平面とレイが交差するかを判定します。
     * @param[in]   ray     : レイ
     * @param[in]   point   : 交点の座標を格納するベクトルのポインタ
     * @return      交差する場合は true、平行な場合は false
     */
    bool intersects(const Ray& ray, Vector3* point = nullptr) const;

    /**
     * 指定された行列を使用してこの平面を座標変換します。
     * @param[in]   mat     : 処理の基になる行列
     */
    void transform(const Matrix& mat);

    /**
     * 指定した点と平面の距離を計算します。
     * 
     * 戻り値の符号で点と平面の位置関係を表します。負値の場合、点は平面の裏側にあります。
     * 単純に距離を知りたい場合は戻り値の絶対値を取ってください。
     */
    float getDistanceToPoint(const Vector3& point) const { return Vector3::dot(normal, point) + distance; }

public:
    /**
     * 正規化した平面を返します。
     * @param[in]   plane   : 処理の基になる平面
     * @return      正規化された平面
     * 
     * 面法線 Normal を正規化し、それに使用した係数を distance にも適用します。
     */
    static Plane normalize(const Plane& plane);

    /**
     * 平面と 4D ベクトルの内積を計算します。
     * @param[in]   plane   : 処理の基になる平面
     * @param[in]   vec     : 処理の基になるベクトル
     */
    static float dot(const Plane& plane, const Vector4& vec);

    /**
     * 平面と 3D ベクトルの内積を計算します。(ベクトルの W を 1.0 として計算する)
     * @param[in]   plane   : 処理の基になる平面
     * @param[in]   vec     : 処理の基になるベクトル
     * 
     * 戻り値が正の値である場合、点は平面の表側になります。
     */
    static float dotCoord(const Plane& plane, const Vector3& vec);

    /**
     * 平面と 3D ベクトルの内積を計算します。(ベクトルの W を 0.0 として計算する)
     * @param[in]   plane   : 処理の基になる平面
     * @param[in]   vec     : 処理の基になるベクトル
     */
    static float dotNormal(const Plane& plane, const Vector3& vec);

    /**
     * 指定された行列を使用して平面を座標変換します。
     * @param[in]   plane   : 処理の基になる平面
     * @param[in]   mat     : 処理の基になる行列
     * @return      変換された平面
     */
    static Plane transform(const Plane& plane, const Matrix& mat);
};

inline bool Plane::checkInside(const Vector3& point) const
{
    return (Plane::dotCoord(*this, point) < 0.0f);
}

inline bool Plane::checkInsideLower(const Vector3& point) const
{
    return (Plane::dotCoord(*this, point) <= 0.0f);
}

inline bool Plane::checkInside(const Vector3& point, float radius) const
{
    return (Plane::dotCoord(*this, point) - radius < 0.0f);
}

inline std::ostream& operator<<(std::ostream& s, const Plane& v)
{
    s << v.normal.x << ' ' << v.normal.y << ' ' << v.normal.z << ' ' << v.distance;
    return s;
}

} // namespace ln
