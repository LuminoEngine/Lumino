
#pragma once
#include <Lumino/Base/Array.h>

#define LN_NAMESPACE_SCENE_BEGIN	//namespace Scene {
#define LN_NAMESPACE_SCENE_END		//}

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN
class SceneGraphManager;
class ViewPane;
class Layer;
class DrawingLayer;
class RenderingPass;
class RenderingParams;
class SceneNode;
class VisualNode;
class Camera;
class Light;
class MMEShader;
class MMEShaderTechnique;

typedef Array<RenderingPass*>	RenderingPassList;
typedef Array<Light*>			LightNodeList;
typedef Array<SceneNode*>		SceneNodeList;
typedef Array<MMEShader*>		SceneShaderList;

/// ノード種別
enum SceneNodeType
{
	SceneNodeType_BaseNode = 0,
	SceneNodeType_VisualNode,
};

/// カメラのプロジェクション
enum CameraProjection
{
	CameraProjection_2D = 0,	///< 2D モード
	CameraProjection_3D,		///< 3D モード

	CameraProjection_Max,		///< (Terminator)
};

/// ライトの種類
enum LightType
{
	LightType_Directional = 0,	///< ディレクショナルライト
	LightType_Point,			///< ポイントライト
	LightType_Spot,				///< スポットライト

	LightType_Max,				///< (Terminator)
};

/// ビルボードの計算方法
enum BillboardType
{
	BillboardType_None = 0,		///< ビルボードの計算を行わない
	BillboardType_Front,		///< カメラ (ビュー行列) に対して正面を向く
	BillboardType_RotY,			///< Y 軸回転のみ行う

	BillboardType_Max,			///< (Terminator)
};

enum SpriteCoord
{
	SpriteCoord_X = 0,			///< X+ 方向 (右向き)
	SpriteCoord_Y,				///< Y+ 方向 (上向き)
	SpriteCoord_Z,				///< Z+ 方向 (奥向き)
	SpriteCoord_RX,				///< X- 方向 (左向き)
	SpriteCoord_RY,				///< Y- 方向 (下向き)
	SpriteCoord_RZ,				///< Z- 方向 (手前向き)
	SpriteCoord_2D,				///< 2D 空間用。原点を左上、Y+ 方向を下向きとする。

	SpriteCoord_Max,			///< (Terminator)
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

/// カメラ単位の設定項目 (1フレームに1度だけ設定すればいいパラメータ)
//struct CameraSceneParams
//{
//	//const Matrix*	Mat_CameraView;
//	//const Matrix*	Mat_CameraProj;
//	//const Matrix*	Mat_CameraViewProj;    ///< カメラのビューとプロジェクション行列の積
//
//	//Vector4			CameraPosition;	    ///< カメラの位置
//	//Vector4			CameraDirection;	    ///< カメラの向き
//	//Vector4         ViewPixelSize;         ///< 描画先RTサイズ
//
//	//// 以下は calcMatrices() で設定する
//	//Matrix  Mat_CameraView_Inverse;
//	//Matrix  Mat_CameraProj_Inverse;
//	//Matrix  Mat_CameraViewProj_Inverse;
//	//Matrix  Mat_CameraView_Transpose;
//	//Matrix  Mat_CameraProj_Transpose;
//	//Matrix  Mat_CameraViewProj_Transpose;
//	//Matrix  Mat_CameraView_InverseTranspose;
//	//Matrix  Mat_CameraProj_InverseTranspose;
//	//Matrix  Mat_CameraViewProj_InverseTranspose;
//
//	//void CalcMatrices()
//	//{
//	//	Mat_CameraView_Inverse = Matrix::Inverse(*Mat_CameraView);
//	//	Mat_CameraProj_Inverse = Matrix::Inverse(*Mat_CameraProj);
//	//	Mat_CameraViewProj_Inverse = Matrix::Inverse(*Mat_CameraViewProj);
//	//	Mat_CameraView_Transpose = Matrix::Transpose(*Mat_CameraView);
//	//	Mat_CameraProj_Transpose = Matrix::Transpose(*Mat_CameraProj);
//	//	Mat_CameraViewProj_Transpose = Matrix::Transpose(*Mat_CameraViewProj);
//	//	Mat_CameraView_InverseTranspose = Mat_CameraView_Inverse * Mat_CameraView_Transpose;
//	//	Mat_CameraProj_InverseTranspose = Mat_CameraProj_Inverse * Mat_CameraProj_Transpose;
//	//	Mat_CameraViewProj_InverseTranspose = Mat_CameraViewProj_Inverse * Mat_CameraViewProj_Transpose;
//	//}
//};

namespace Internal
{
static const int MaxRenderingPass = 32;
struct VisualNodeSubsetParams;
class VisualNodeParams;
class SceneHelper;

struct RenderingPassClientData
{
	RenderingPass*	OwnerPass;				///< 
	int				PriorityShaderIndex;	///< RenderingPass::m_priorityShaderList のインデックス (-1 は一致なし)
};

};

LN_NAMESPACE_SCENE_END
} // namespace Lumino

