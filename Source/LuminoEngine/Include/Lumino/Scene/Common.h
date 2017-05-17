
#pragma once
#include <Lumino/Base/List.h>

#define LN_NAMESPACE_SCENE_BEGIN	//namespace Scene {
#define LN_NAMESPACE_SCENE_END		//}

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
class SceneGraphManager;

class SceneGraph;
class Layer;
class DrawingLayer;
class SceneNode;
class VisualComponent;
class CameraComponent;
class LightComponent;
class EffectBatchRendererNode;

class TileMap;
typedef RefPtr<TileMap>		TileMapPtr;

class CameraBehavior;

typedef List<LightComponent*>			LightNodeList;

/// ノード種別
enum SceneNodeType
{
	SceneNodeType_BaseNode = 0,
	SceneNodeType_VisualNode,
};

/// 
enum class SceneNodeRenderingMode
{
	Invisible,
	Visible,
	NonShaderVisible,
};

/// カメラのプロジェクション
enum CameraProjection
{
	CameraProjection_2D = 0,	///< 2D モード
	CameraProjection_3D,		///< 3D モード

	CameraProjection_Max,		///< (Terminator)
};

/** カメラの向きの指定方法 */
enum class CameraDirection
{
	LookAt,			/**< SetLookAt() により注視点を指定する */
	Transform,		/**< 姿勢の正面方向を向く */
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

/// 描画パス
enum InternalRenderingPass
{
	InternalRenderingPass_object = 65536,	///< オブジェクト本体（セルフシャドウOFF）
	InternalRenderingPass_zplot,			///< セルフシャドウ用Z値プロット
	InternalRenderingPass_object_ss,		///< オブジェクト本体（セルフシャドウON）
	InternalRenderingPass_shadow,			///< 影（セルフシャドウではない単純な影）
	InternalRenderingPass_edge,				///< 輪郭（PMDモデルのみ）
	InternalRenderingPass_Infomation,		///< デバッグ情報等
};


namespace detail
{
static const int MaxRenderingPass = 32;

//struct RenderingPassClientData
//{
//	RenderingPass*	OwnerPass;				///< 
//	int				PriorityShaderIndex;	///< RenderingPass::m_priorityShaderList のインデックス (-1 は一致なし)
//};

enum SceneNodeDefaultShaderClass
{
	SceneNodeDefaultShaderClass_StaticMesh,
	SceneNodeDefaultShaderClass_SkinnedMesh,
	SceneNodeDefaultShaderClass_Count,
};

}
LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

