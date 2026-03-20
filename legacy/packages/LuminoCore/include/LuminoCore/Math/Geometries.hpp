// Copyright (c) 2018+ lriki. Distributed under the MIT license..
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

	/** レイの長さ (default: Inf) */
	float distance;

public:
    /** 各要素に 0.0 を設定してインスタンスを初期化します。 */
    Ray();

    /**
        各要素を指定してインスタンスを初期化します。
        @param[in]  origin      : レイの始点
        @param[in]  direction   : レイの方向
    */
    Ray(const Vector3& origin, const Vector3& direction);

	Ray(const Vector3& origin, const Vector3& direction, float distance);

	Vector3 endPoint() const { return origin + (direction * distance); }
};

/** 直方体を定義します。(axis-aligned bounding box) */
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

    /** 中心座標と各軸のサイズを指定して正方形を作成します。*/
    Box(const Vector3& center, float sizeXYZ);

    /** 中心座標と各軸のサイズを指定して正方形を作成します。*/
    Box(const Vector3& center, float width, float height, float depth);

    /** 最小値と最大値を指定してボックスを作成します。*/
    Box(const Vector3& min, const Vector3& max);

    /** サイズが 0 以下であるかを判定します。*/
    bool isEmpty() const { return width <= 0.0f || height <= 0.0f || depth <= 0.0f; }

    /** 境界の最小値を取得します。*/
    Vector3 minPoint() const;

    /** 境界と最大値を取得します。*/
    Vector3 maxPoint() const;

    /** 境界の最小値と最大値を取得します。*/
    void getMinMax(Vector3* outMin, Vector3* outMax) const;

	static Box transform(const Box& box, const Matrix& mat);
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
