// Copyright (c) 2018+ lriki. Distributed under the MIT license..
#pragma once

#include "Common.hpp"
#include "Plane.hpp"

namespace ln {
struct Matrix;
struct Box;

/** 交差判定の結果を表します。 */
enum class IntersectResult
{
    /** 領域の外側にある */
    Outside,

    /** 領域の内側にある */
    Inside,

    /** 領域の境界上にある (交差している) */
    Intersect,
};

/**
 * 3D の視錐台を定義します。
 * 
 * 視錐台は主に 3D 空間上のカメラの視野を表すために使用します。
 * カメラを表すビュープロジェクション行列から作成できます。
 * 
 * 視錐台の外側にあるオブジェクトは通常、レンダリングする必要はありません。
 * intersects() を使用して視錐台とオブジェクトの交差判定 (内側に存在するか) を行うことで、レンダリングが必要かをチェックできます。
 */
struct LN_API ViewFrustum
{
public:
    /** デフォルトコンストラクタ。作成された視錐台は大きさを持ちません。*/
    ViewFrustum();

    /**
     * 指定されたビュープロジェクション行列を使用して初期化します。
     * @param[in]   viewProjMatrix  : 視錐台の生成に使用するビュープロジェクション行列
     */
    ViewFrustum(const Matrix& viewProjMatrix);

public:
    /**
     * 指定されたビュープロジェクション行列を使用してこの視錐台を再構築します。
     * @param[in]   viewProjMatrix  : 視錐台の生成に使用するビュープロジェクション行列
     */
    void setViewProjMatrix(const Matrix& viewProjMatrix);

    /**
     * 指定した点がこの視錐台の内側にあるかを判定します。
     * @param[in]   point       : 判定する点
     * @return      点が内側にある場合は true、そうでない場合は false。
     */
    bool intersects(const Vector3& point) const;

    /**
     * 指定した球がこの視錐台と交差するかを判定します。
     * @param[in]   center      : 球の中心点
     * @param[in]   radius      : 球の半径
     * @return      交差する場合は true、そうでない場合は false。(接触している場合も true)
     */
    bool intersects(const Vector3& center, float radius) const;

    /**
     * 指定した Box がこの視錐台と交差するかを判定します。
     */
    IntersectResult intersects(const Box& box) const;

    /**
     * 錐台の各頂点を取得します。
     * @param[out]    points        : 頂点座標を格納する配列 (要素数は 8 であること)
     * 頂点の順番は、Near平面の左上、右上、右下、左下、Far平面の左上、右上、右下、左下です。
     */
    void getCornerPoints(Vector3* points) const;

    /**
     * 錐台の指定した面を取得します。
     * @param[in]   plane       : 取得する面
     */
    const Plane& plane(FrustumPlane plane) const { return m_planes[static_cast<int>(plane)]; }

private:
    static const int FrustumPlane_Max = 6;
    Plane m_planes[FrustumPlane_Max];
    Matrix m_viewProjMatrix;
};

} // namespace ln
