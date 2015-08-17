
#pragma once
#include <Lumino/Base/Array.h>

namespace Lumino
{
namespace Scene
{
class SceneGraphManager;
class Layer;
class DrawingLayer;
class RenderingPass;
class SceneNode;
class VisualNode;
class Camera;
class Light;

typedef ArrayList<Layer*>			LayerList;
typedef ArrayList<RenderingPass*>	RenderingPassList;
typedef ArrayList<Light*>			LightNodeList;
typedef ArrayList<SceneNode*>		SceneNodeList;

/// ノード種別
enum SceneNodeType
{
	SceneNodeType_BaseNode = 0,
	SceneNodeType_VisualNode,
};

/// ビルボードの計算方法
enum BillboardType
{
	BillboardType_None = 0,		///< ビルボードの計算を行わない
	BillboardType_Front,		///< カメラ (ビュー行列) に対して正面を向く
	BillboardType_RotY,			///< Y 軸回転のみ行う

	BillboardType_Max,			///< (Terminator)
};

/// 反転方法
enum FlipMode
{
	FlipMode_None = 0,		///< 反転無し
	FlipMode_H,				///< 左右反転
	FlipMode_V,				///< 上下反転
	FlipMode_HV,			///< 左右上下反転
};

} // namespace Scene
} // namespace Lumino

