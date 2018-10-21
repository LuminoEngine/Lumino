// Copyright (c) 2018 lriki. Distributed under the MIT license.
#pragma once

#include "Common.hpp"
#include "Vector3.hpp"

namespace ln {

/** レイを定義します。 */
struct LN_API Ray
{
public:
    /** レイの始点 */
    Vector3 origin;

    /** レイの方向 */
    Vector3 direction;

public:
    /** 各要素に 0.0 を設定してインスタンスを初期化します。 */
    Ray();

    /**
        各要素を指定してインスタンスを初期化します。
        @param[in]  origin      : レイの始点
        @param[in]  direction   : レイの方向
    */
    Ray(const Vector3& origin, const Vector3& direction);
};

/** 直方体を定義します。 */
struct LN_API Box
{
public:
    /** 中心座標 */
    Vector3 center;

    /** X 軸の範囲 */
    float width;

    /** Y 軸の範囲 */
    float height;

    /** Z 軸の範囲 */
    float depth;

public:
    /** 各要素に 0.0 を設定してインスタンスを初期化します。 */
    Box();

    /** 各軸のサイズを指定して正方形を作成します。*/
    Box(float sizeXYZ);

    /** 最小値と最大値を指定してボックスを作成します。*/
    Box(const Vector3& min, const Vector3& max);

    /** 境界の最小値と最大値を取得します。*/
    void getMinMax(Vector3* outMin, Vector3* outMax) const;
};

/** 向きを持つボックスを定義します。 */
struct LN_API OrientedBox
{
public:
    /** ボックスの中心座標 */
    Vector3 center;

    /** ボックスの X 軸 (単位ベクトルであること) */
    Vector3 axisX;

    /** ボックスの Y 軸 (単位ベクトルであること) */
    Vector3 axisY;

    /** ボックスの Z 軸 (単位ベクトルであること) */
    Vector3 axisZ;

    /** X 軸の範囲 */
    float extentX;

    /** Y 軸の範囲 */
    float extentY;
    
    /** Z 軸の範囲 */
    float extentZ;

public:
    /** サイズを 0 に設定してインスタンスを初期化します。*/
    OrientedBox();

    /** 各軸のサイズを指定して正方形を作成します。*/
    OrientedBox(float sizeXYZ);
};

} // namespace ln
