
#pragma once
#include <vector>
#include <list>
#include <map>
#include <Lumino/Graphics/Shader.h>
#include "../Internal.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
//
//class SceneNode;
//class SceneShaderManager;
class MMETextureObject;

/// MME セマンティクス一覧
enum MMESemantic
{
    MME_SEMANTIC_NONE = 0,

    MME_SEMANTIC_WORLD,
    MME_SEMANTIC_VIEW,
    MME_SEMANTIC_PROJECTION,
    MME_SEMANTIC_WORLDVIEW,
    MME_SEMANTIC_VIEWPROJECTION,
    MME_SEMANTIC_WORLDVIEWPROJECTION,

    MME_SEMANTIC_WORLD_I,
    MME_SEMANTIC_VIEW_I,
    MME_SEMANTIC_PROJECTION_I,
    MME_SEMANTIC_WORLDVIEW_I,
    MME_SEMANTIC_VIEWPROJECTION_I,
    MME_SEMANTIC_WORLDVIEWPROJECTION_I,

    MME_SEMANTIC_WORLD_T,
    MME_SEMANTIC_VIEW_T,
    MME_SEMANTIC_PROJECTION_T,
    MME_SEMANTIC_WORLDVIEW_T,
    MME_SEMANTIC_VIEWPROJECTION_T,
    MME_SEMANTIC_WORLDVIEWPROJECTION_T,

    MME_SEMANTIC_WORLD_IT,
    MME_SEMANTIC_VIEW_IT,
    MME_SEMANTIC_PROJECTION_IT,
    MME_SEMANTIC_WORLDVIEW_IT,
    MME_SEMANTIC_VIEWPROJECTION_IT,
    MME_SEMANTIC_WORLDVIEWPROJECTION_IT,

    MME_SEMANTIC_DIFFUSE,
    MME_SEMANTIC_AMBIENT,
    MME_SEMANTIC_EMISSIVE,
    MME_SEMANTIC_SPECULAR,
    MME_SEMANTIC_SPECULARPOWER,
    MME_SEMANTIC_TOONCOLOR,
    MME_SEMANTIC_EDGECOLOR,

    MME_SEMANTIC_POSITION,          ///< ライトまたはカメラの、ワールド空間内における位置
    MME_SEMANTIC_DIRECTION,         ///< ライトまたはカメラの、ワールド空間内における向き

    MME_SEMANTIC_MATERIALTEXTURE,       ///< マテリアルに設定されているテクスチャ
    MME_SEMANTIC_MATERIALSPHEREMAP,     ///< マテリアルに設定されているスフィアマップテクスチャ

    MME_SEMANTIC_VIEWPORTPIXELSIZE,     ///< レンダリングターゲットのスクリーンのサイズ (Scene の描画先RTもしくはオフスクリーンRTのサイズ。Scriptsの"RenderColorTarget"コマンドで、RTを変更しても、この値は変化しない)

    MME_SEMANTIC_TIME,                  ///< 0フレーム目からの再生時間 (秒。モデル単位。アニメ再生時、または時間設定時に現在時間が代入される)
    MME_SEMANTIC_ELAPSEDTIME,           ///< 前回の描画からの経過時間 (秒)

    MME_SEMANTIC_MOUSEPOSITION,         ///< [float2] マウスの現在位置 (スクリーン中心が(0,0)、左下隅が(-1,-1)、右上隅が(1,1)になる)
    MME_SEMANTIC_LEFTMOUSEDOWN,         ///< [float4] 最後にボタンが押されたときのマウスの座標(x, y)、現在ボタンが押されているか（0 or 1）、最後にボタンが押された時点のTIME値(秒)
    MME_SEMANTIC_MIDDLEMOUSEDOWN,   
    MME_SEMANTIC_RIGHTMOUSEDOWN,

    MME_SEMANTIC_CONTROLOBJECT,         ///< 指定したオブジェクトの、座標やワールド変換行列を取得する(型によって情報の種類が異なる)

    MME_SEMANTIC_RENDERCOLORTARGET,
    MME_SEMANTIC_RENDERDEPTHSTENCILTARGET,
    MME_SEMANTIC_ANIMATEDTEXTURE,
    MME_SEMANTIC_OFFSCREENRENDERTARGET,
    MME_SEMANTIC_TEXTUREVALUE,

    MME_SEMANTIC_STANDARDSGLOBAL,

	// MMM
	MME_SEMANTIC_LIGHTENABLES,
	MME_SEMANTIC_LIGHTWVPMATRICES,
	MME_SEMANTIC_LIGHTDIRECTIONS,
	MME_SEMANTIC_LIGHTPOSITIONS,
	MME_SEMANTIC_LIGHTZFARS,
	MME_SEMANTIC_LIGHTDIFFUSECOLORS,
	MME_SEMANTIC_LIGHTAMBIENTCOLORS,
	MME_SEMANTIC_LIGHTSPECULARCOLORS,

	// 以下は独自仕様
	//LN_SEMANTIC_ORTHOGRAPHIC,       ///< カメラビューサイズから作成した正射影行列 (動作未確認)
    //LN_SEMANTIC_PROJECTION2D,       ///< ※もう使わない。消しておく

    LN_SEMANTIC_OPACITY,            ///< 不透明度
    LN_SEMANTIC_COLOR_SCALE,        ///< 乗算する色
    LN_SEMANTIC_BLEND_COLOR,        ///< ブレンドする色
    LN_SEMANTIC_TONE,               ///< 色調
    LN_SEMANTIC_UVTRANSFORM,        ///< テクスチャ座標変換行列


    MME_MAX_SEMANTICS       ///< セマンティクスの最大数
};

/// MME アノテーション一覧
enum MMEAnnotation
{
    MME_ANNOTATION_NONE = 0,

    MME_ANNOTATION_Object,          ///< ジオメトリ変換、ライト、マテリアル、位置、向きの対象
    MME_ANNOTATION_SyncInEditMode,  ///< 時間情報のフレーム連動フラグ

    MME_ANNOTATION_Name,            ///< CONTROLOBJECT の対象オブジェクト名
    MME_ANNOTATION_Item,            ///< CONTROLOBJECT の特殊値名

    MME_ANNOTATION_ResourceType,    ///< テクスチャリソースの種類
    MME_ANNOTATION_ResourceName,    ///< テクスチャリソースのファイル名
    MME_ANNOTATION_Width,           ///< テクスチャの幅
    MME_ANNOTATION_Height,          ///< テクスチャの高さ
    MME_ANNOTATION_Depth,           ///< テクスチャの深さ
    MME_ANNOTATION_Dimensions,      ///< テクスチャのディメンジョン
    MME_ANNOTATION_ViewportRatio,   ///< テクスチャのビューに対する比率
    MME_ANNOTATION_Format,          ///< テクスチャのサーフェイスフォーマット
    MME_ANNOTATION_Miplevels,       ///< テクスチャのミップレベル
    MME_ANNOTATION_Levels,          ///< テクスチャのミップレベル

    MME_ANNOTATION_Offset,          ///< アニメーションテクスチャの開始時間のずらし量
    MME_ANNOTATION_Speed,           ///< アニメーションテクスチャの再生スピード
    MME_ANNOTATION_SeekVariable,    ///< アニメーションテクスチャのシーク制御方法

    MME_ANNOTATION_ClearColor,      ///< オフスクリーンRTのクリア色
    MME_ANNOTATION_ClearDepth,      ///< オフスクリーンRTの深度クリア値
    MME_ANNOTATION_AntiAlias,       ///< オフスクリーンRTのアンチエイリアス有効フラグ　
    MME_ANNOTATION_Description,     ///< オフスクリーンRTの説明文
    MME_ANNOTATION_DefaultEffect,   ///< オフスクリーンRTのデフォルトエフェクトファイル名

    MME_ANNOTATION_TextureName,     ///< TEXTUREVALUE の対象テクスチャ変数名

    MME_ANNOTATION_ScriptOutput,    ///< "color" で固定
    MME_ANNOTATION_ScriptClass,     ///< STANDARDSGLOBAL で指定されるエフェクトファイルの目的
    MME_ANNOTATION_ScriptOrder,     ///< STANDARDSGLOBAL で指定されるエフェクトファイルの実行タイミング
    
    MME_ANNOTATION_Script,          ///< 各スクリプト情報

    MME_MAX_ANNOTATIONS
};

/// シェーダ変数がセマンティクス・アノテーションによって要求しているものの種類
enum MMEVariableRequest
{
    MME_VARREQ_NONE = 0,

    MME_VARREQ_MATRIX_World,                ///< [ object ] オブジェクトのワールド変換行列
    MME_VARREQ_MATRIX_CAMERA_WorldView,     ///< [ object ] オブジェクトのワールド変換行列とカメラのビュー変換行列
    MME_VARREQ_MATRIX_CAMERA_WorldViewProj, ///< [ object ] オブジェクトのワールド変換行列とカメラのビュー、プロジェクション変換行列
    MME_VARREQ_MATRIX_LIGHT_WorldView,      ///< [ object ] オブジェクトのワールド変換行列とデフォルトライトのビュー変換行列
    MME_VARREQ_MATRIX_LIGHT_WorldViewProj,  ///< [ object ] オブジェクトのワールド変換行列とデフォルトライトのビュー、プロジェクション変換行列

    MME_VARREQ_MATRIX_CAMERA_View,          ///< [ camera scene ] カメラのビュー変換行列
    MME_VARREQ_MATRIX_CAMERA_Proj,          ///< [ camera scene ] カメラのプロジェクション変換行列
    MME_VARREQ_MATRIX_CAMERA_ViewProj,      ///< [ camera scene ] カメラのビュー×プロジェクション変換行列

    MME_VARREQ_MATRIX_LIGHT_View,           ///< [ scene ] デフォルトライトのビュー変換行列
    MME_VARREQ_MATRIX_LIGHT_Proj,           ///< [ scene ] デフォルトライトのプロジェクション変換行列
    MME_VARREQ_MATRIX_LIGHT_ViewProj,       ///< [ scene ] デフォルトライトのビュー×プロジェクション変換行列

	// 逆行列
    MME_VARREQ_MATRIX_World_I,
    MME_VARREQ_MATRIX_CAMERA_WorldView_I,
    MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I,
    MME_VARREQ_MATRIX_LIGHT_WorldView_I,
    MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I,
    MME_VARREQ_MATRIX_CAMERA_View_I,
    MME_VARREQ_MATRIX_CAMERA_Proj_I,
    MME_VARREQ_MATRIX_CAMERA_ViewProj_I,
    MME_VARREQ_MATRIX_LIGHT_View_I,
    MME_VARREQ_MATRIX_LIGHT_Proj_I,
    MME_VARREQ_MATRIX_LIGHT_ViewProj_I,

	// 転置行列
    MME_VARREQ_MATRIX_World_T,
    MME_VARREQ_MATRIX_CAMERA_WorldView_T,
    MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T,
    MME_VARREQ_MATRIX_LIGHT_WorldView_T,
    MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T,
    MME_VARREQ_MATRIX_CAMERA_View_T,
    MME_VARREQ_MATRIX_CAMERA_Proj_T,
    MME_VARREQ_MATRIX_CAMERA_ViewProj_T,
    MME_VARREQ_MATRIX_LIGHT_View_T,
    MME_VARREQ_MATRIX_LIGHT_Proj_T,
    MME_VARREQ_MATRIX_LIGHT_ViewProj_T,

	// 逆行列×転置行列
    MME_VARREQ_MATRIX_World_IT,
    MME_VARREQ_MATRIX_CAMERA_WorldView_IT,
    MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT,
    MME_VARREQ_MATRIX_LIGHT_WorldView_IT,
    MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT,
    MME_VARREQ_MATRIX_CAMERA_View_IT,
    MME_VARREQ_MATRIX_CAMERA_Proj_IT,
    MME_VARREQ_MATRIX_CAMERA_ViewProj_IT,
    MME_VARREQ_MATRIX_LIGHT_View_IT,
    MME_VARREQ_MATRIX_LIGHT_Proj_IT,
    MME_VARREQ_MATRIX_LIGHT_ViewProj_IT,


    MME_VARREQ_OBJECT_DIFFUSE,              ///< [ object ] ディフューズ色（拡散光）
    MME_VARREQ_OBJECT_AMBIENT,              ///< [ object ] アンビエント色（環境光）
    MME_VARREQ_OBJECT_EMISSIVE,             ///< [ object ] エミッション色（放射光）
    MME_VARREQ_OBJECT_SPECULAR,             ///< [ object ] スペキュラ色（鏡面光）
    MME_VARREQ_OBJECT_SPECULARPOWER,        ///< [ object ] スペキュラの強度
    MME_VARREQ_OBJECT_TOONCOLOR,            ///< [ object ] トゥーン色
    MME_VARREQ_OBJECT_EDGECOLOR,            ///< [ object ] 輪郭色

    MME_VARREQ_LIGHT_DIFFUSE,               ///< [ scene ] デフォルトライトのディフューズ色（拡散光）
    MME_VARREQ_LIGHT_AMBIENT,               ///< [ scene ] デフォルトライトのアンビエント色（環境光）
    MME_VARREQ_LIGHT_SPECULAR,              ///< [ scene ] デフォルトライトのスペキュラ色（鏡面光）

    MME_VARREQ_CAMERA_POSITION,             ///< [ camera scene ] カメラの位置
    MME_VARREQ_CAMERA_DIRECTION,            ///< [ camera scene ] カメラの向き

    MME_VARREQ_LIGHT_POSITION,              ///< [ scene ] デフォルトライトの位置
    MME_VARREQ_LIGHT_DIRECTION,             ///< [ scene ] デフォルトライトの向き

    MME_VARREQ_OBJECT_MATERIALTEXTURE,      ///< [ object ] マテリアルに設定されているテクスチャ
    MME_VARREQ_OBJECT_MATERIALSPHEREMAP,    ///< [ object ] マテリアルに設定されている、スフィアマップテクスチャ

    MME_VARREQ_VIEWPORTPIXELSIZE,           ///< [ camera scene / offscr scene ] スクリーンもしくはオフスクリーンレンダリングターゲットのサイズ (Scriptsの"RenderColorTarget"コマンドで、レンダリングターゲットが変更しても、この値は変化しない)

    MME_VARREQ_TIME,                        ///< [ scene ] 経過時間 (秒)
    MME_VARREQ_ELAPSEDTIME,                 ///< [ scene ] 前回のフレームからの経過時間 (秒)

    MME_VARREQ_MOUSEPOSITION,               ///< マウス座標 (x, y)
    MME_VARREQ_LEFTMOUSEDOWN,               ///< マウスの左ボタン情報
    MME_VARREQ_MIDDLEMOUSEDOWN,             ///< マウスの中ボタン情報
    MME_VARREQ_RIGHTMOUSEDOWN,              ///< マウスの右ボタン情報


    MME_VARREQ_CONTROLOBJECT_Visible,          ///< 指定したオブジェクトが表示されているか否か (bool)
    MME_VARREQ_CONTROLOBJECT_Scale,         ///< 指定したオブジェクトのスケーリング値 (float)
    MME_VARREQ_CONTROLOBJECT_Position,      ///< 指定したオブジェクトの座標（オフセット）(float3 float4)
    MME_VARREQ_CONTROLOBJECT_World,         ///< 指定したオブジェクトのワールド変換行列 (float4x4)

    MME_VARREQ_CONTROLOBJECT_BoneOffset,    ///< 指定ボーンの座標 (float3,float4)           [ボーン名]
    MME_VARREQ_CONTROLOBJECT_BoneMatrix,    ///< 指定ボーンのワールド変換行列 (float4x4)    [ボーン名]
    MME_VARREQ_CONTROLOBJECT_MorphBlend,    ///< 指定した表情の値 (float)                   [表情名]

    MME_VARREQ_CONTROLOBJECT_X,             ///< アクセサリの位置X(アクセサリパネルのX)。型はfloat。
    MME_VARREQ_CONTROLOBJECT_Y,             ///< アクセサリの位置Y(アクセサリパネルのY)。型はfloat。
    MME_VARREQ_CONTROLOBJECT_Z,             ///< アクセサリの位置Z(アクセサリパネルのZ)。型はfloat。
    MME_VARREQ_CONTROLOBJECT_XYZ,           ///< アクセサリの位置(アクセサリパネルのX,Y,Z)。型はfloat3。
    MME_VARREQ_CONTROLOBJECT_Rx,            ///< アクセサリの回転X(アクセサリパネルのRx)。型はfloat。
    MME_VARREQ_CONTROLOBJECT_Ry,            ///< アクセサリの回転Y(アクセサリパネルのRy)。型はfloat。
    MME_VARREQ_CONTROLOBJECT_Rz,            ///< アクセサリの回転Z(アクセサリパネルのRz)。型はfloat。
    MME_VARREQ_CONTROLOBJECT_Rxyz,          ///< アクセサリの回転(アクセサリパネルのRx,Ry,Rz)。型はfloat3。
    MME_VARREQ_CONTROLOBJECT_Si,            ///< アクセサリのサイズ(アクセサリパネルのSi)。型はfloat。
    MME_VARREQ_CONTROLOBJECT_Tr,            ///< アクセサリの透明度(アクセサリパネルのTr)。型はfloat。



    
	// リソースを作成するもの
    MME_VARREQ_TEXTURE,                     ///< MME の仕様ではないけど、テクスチャ型識別用に用意 (texture, texture2D, texture3D, textureCUBEのうちいずれか)
    MME_VARREQ_RENDERCOLORTARGET,
    MME_VARREQ_RENDERDEPTHSTENCILTARGET,
    MME_VARREQ_ANIMATEDTEXTURE,
    MME_VARREQ_OFFSCREENRENDERTARGET,

    MME_VARREQ_TEXTUREVALUE,

    MME_VARREQ_STANDARDSGLOBAL,



	// MMM
	MME_VARREQ_LIGHTENABLES,
	MME_VARREQ_LIGHTWVPMATRICES,
	MME_VARREQ_LIGHTDIRECTIONS,
	MME_VARREQ_LIGHTPOSITIONS,
	MME_VARREQ_LIGHTZFARS,
	MME_VARREQ_LIGHTDIFFUSECOLORS,
	MME_VARREQ_LIGHTAMBIENTCOLORS,
	MME_VARREQ_LIGHTSPECULARCOLORS,


	// 以下は独自仕様
	//LN_VARREQ_MATRIX_CAMERA_ORTHOGRAPHIC,
	//LN_VARREQ_MATRIX_LIGHT_ORTHOGRAPHIC,
    //LN_VARREQ_MATRIX_PROJECTION2D,

    LN_VARREQ_OPACITY,            ///< 不透明度 (TODO: 廃止)
    LN_VARREQ_COLOR_SCALE,        ///< 乗算する色
    LN_VARREQ_BLEND_COLOR,        ///< ブレンドする色
    LN_VARREQ_TONE,               ///< 色調
    LN_VARREQ_UVTRANSFORM,        ///< テクスチャ座標変換行列


	// 少しでも条件判定の回数を減らすための策
    MME_VARREQ_GEOMETRY_MATRIX_BEGIN    = MME_VARREQ_MATRIX_World,
    MME_VARREQ_GEOMETRY_MATRIX_END      = MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT,
	
	MME_VARREQ_CONTROLOBJECT_BEGIN		= MME_VARREQ_CONTROLOBJECT_Visible,
	MME_VARREQ_CONTROLOBJECT_END		= MME_VARREQ_CONTROLOBJECT_Tr,
};

/// 必要のない行列計算をマスクするためのフラグ
enum MMEWorldMatrixCalcFlags
{
    MME_WMCF_NONE = 0,

	//MME_WMCF_WORLD                      = 0x00000000,
    MME_WMCF_WORLD_VIEW_CAMERA          = 0x00000001,   ///< ワールド行列 * カメラのビュー行列
    MME_WMCF_WORLD_VIEW_PROJ_CAMERA     = 0x00000004,   ///< ワールド行列 * カメラのビュー行列 * カメラのプロジェクション行列
    MME_WMCF_WORLD_VIEW_LIGHT           = 0x00000008,   ///< ワールド行列 * ライトのビュー行列
    MME_WMCF_WORLD_VIEW_PROJ_LIGHT      = 0x00000020,   ///< ワールド行列 * ライトのビュー行列 * ライトのプロジェクション行列

    MME_WMCF_WORLD_I                    = 0x00000002,   ///< (逆行列) ワールド行列
    MME_WMCF_WORLD_VIEW_CAMERA_I        = 0x00000040,   ///< (逆行列) ワールド行列 * カメラのビュー行列
    MME_WMCF_WORLD_VIEW_PROJ_CAMERA_I   = 0x00000100,   ///< (逆行列) ワールド行列 * カメラのビュー行列 * カメラのプロジェクション行列
    MME_WMCF_WORLD_VIEW_LIGHT_I         = 0x00000200,   ///< (逆行列) ワールド行列 * ライトのビュー行列    
    MME_WMCF_WORLD_VIEW_PROJ_LIGHT_I    = 0x00000800,   ///< (逆行列) ワールド行列 * ライトのビュー行列 * ライトのプロジェクション行列

    MME_WMCF_WORLD_T                    = 0x00002000,   ///< (転置行列) ワールド行列
    MME_WMCF_WORLD_VIEW_CAMERA_T        = 0x00001000,   ///< (転置行列) ワールド行列 * カメラのビュー行列
    MME_WMCF_WORLD_VIEW_PROJ_CAMERA_T   = 0x00004000,   ///< (転置行列) ワールド行列 * カメラのビュー行列 * カメラのプロジェクション行列
    MME_WMCF_WORLD_VIEW_LIGHT_T         = 0x00008000,   ///< (転置行列) ワールド行列 * ライトのビュー行列
    MME_WMCF_WORLD_VIEW_PROJ_LIGHT_T    = 0x00020000,   ///< (転置行列) ワールド行列 * ライトのビュー行列 * ライトのプロジェクション行列

    MME_WMCF_WORLD_IT                   = 0x00400000,   ///< (逆行列 * 転置行列) ワールド行列
    MME_WMCF_WORLD_VIEW_CAMERA_IT       = 0x00040000,   ///< (逆行列 * 転置行列) ワールド行列 * カメラのビュー行列
    MME_WMCF_WORLD_VIEW_PROJ_CAMERA_IT  = 0x00100000,   ///< (逆行列 * 転置行列) ワールド行列 * カメラのビュー行列 * カメラのプロジェクション行列
    MME_WMCF_WORLD_VIEW_LIGHT_IT        = 0x00200000,   ///< (逆行列 * 転置行列) ワールド行列 * ライトのビュー行列
    MME_WMCF_WORLD_VIEW_PROJ_LIGHT_IT   = 0x00800000,   ///< (逆行列 * 転置行列) ワールド行列 * ライトのビュー行列 * ライトのプロジェクション行列

	// ここで指定するのはワールド行列にかかわるものだけ。
	// ライトやカメラはすべてのシェーダで共有するので、あらかじめ全部計算しておく。

	// 以下はよく見たら必要なかったもの

	//MME_WMCF_WORLD_PROJ_CAMERA          = 0x00000002,   ///< ワールド行列 * カメラのプロジェクション行列
	//MME_WMCF_WORLD_PROJ_LIGHT           = 0x00000010,   ///< ワールド行列 * ライトのプロジェクション行列

	//MME_WMCF_WORLD_PROJ_CAMERA_I        = 0x00000080,   ///< (逆行列) ワールド行列 * カメラのプロジェクション行列
	//MME_WMCF_WORLD_PROJ_LIGHT_I         = 0x00000400,   ///< (逆行列) ワールド行列 * ライトのプロジェクション行列

	//MME_WMCF_WORLD_PROJ_CAMERA_T        = 0x00002000,   ///< (転置行列) ワールド行列 * カメラのプロジェクション行列
	//MME_WMCF_WORLD_PROJ_LIGHT_T         = 0x00010000,   ///< (転置行列) ワールド行列 * ライトのプロジェクション行列

	//MME_WMCF_WORLD_PROJ_CAMERA_IT       = 0x00080000,   ///< (逆行列 * 転置行列) ワールド行列 * カメラのプロジェクション行列
	//MME_WMCF_WORLD_PROJ_LIGHT_IT        = 0x00400000,   ///< (逆行列 * 転置行列) ワールド行列 * ライトのプロジェクション行列

};

/// (固定値)
enum MMEScriptOutput
{
    MME_SCROUT_color = 0,          ///< デフォルト値
};

/// エフェクトファイルの目的 (何を描画するエフェクトか)
enum MMEScriptClass
{
    MME_SCRCLS_object = 0,         ///< オブジェクトを描画する。（デフォルト）
    MME_SCRCLS_scene,              ///< スクリーンバッファを描画する。
    MME_SCRCLS_sceneorobject,      ///< 上記の両方。
};

/// エフェクトファイルの実行タイミング
enum MMEScriptOrder
{
    MME_SCRORDER_standard = 0,      ///< オブジェクトを描画する。（デフォルト）
    MME_SCRORDER_preprocess,        ///< オブジェクトの描画よりも先に描画する。プリエフェクト用。
    MME_SCRORDER_postprocess,       ///< オブジェクトの描画の後で描画する。ポストエフェクト用。
};

/// MMD の描画パス
enum MMDPass
{
    MMD_PASS_object = 0,			///< オブジェクト本体（セルフシャドウOFF）
    MMD_PASS_zplot,					///< セルフシャドウ用Z値プロット
    MMD_PASS_object_ss,				///< オブジェクト本体（セルフシャドウON）
    MMD_PASS_shadow,				///< 影（セルフシャドウではない単純な影）※ 使用しない
    MMD_PASS_edge,					///< 輪郭（PMDモデルのみ）※使用しない
	MMD_PASS_Infomation,			///< 物理演算のデバッグ用形状描画など

	MMD_PASS_Max,					///< (Terminator)
};

/// テクニックのアノテーションで指定されている bool 値
//enum MMETechEnableFlag
//{
//    MME_TECHSTATE_Disable       = 0x00,
//
//    MME_TECHSTATE_UseTexture    = 0x01,     ///< テクスチャ使用の有無
//    MME_TECHSTATE_UseSphereMap  = 0x02,     ///< スフィアマップ使用の有無
//    MME_TECHSTATE_UseToon       = 0x04,     ///< トゥーンレンダリング使用の有無
//
//    MME_TECHSTATE_OMITTED_Texture    = 0x10, ///< UseTexture フラグの有無 (省略されている)
//    MME_TECHSTATE_OMITTED_SphereMap  = 0x20, ///< UseSphereMap フラグの有無 (省略されている)
//    MME_TECHSTATE_OMITTED_Toon       = 0x40, ///< EnableToon フラグの有無 (省略されている)
//
//	//MME_TECHSTATE_ALL =                     ///< すべてのフラグの組み合わせ
//	//    MME_TECHSTATE_UseTexture ||
//	//    MME_TECHSTATE_UseSphereMap ||
//	//    MME_TECHSTATE_UseToon
//};

/// あるコマンドがテクニックのスクリプト内に存在しているかを示すフラグ
enum MMEScriptCommandExistFlag
{
    MME_SCEXIST_ScriptExternal_Color = 0x01,    ///< ScriptExternal=Color
    MME_SCEXIST_Draw_Geometry        = 0x02,    ///< Draw=Geometry
    MME_SCEXIST_Draw_Buffer          = 0x04,    ///< Draw=Buffer
};

/// シェーダプログラム内の変数ひとつ分を表す構造体
class MMEShaderVariable
{
public:
    ShaderVariable*				Variable;			///< シェーダ変数
    MMEVariableRequest			Request;			///< 変数が要求しているものの種類
	//SceneNode*				ControlObject;      ///< CONTROLOBJECT の参照対象 (新しいオブジェクトが Scene に追加されたときに再取得する)
    String						ObjectName;			///< CONTROLOBJECT の name アノテーションの値
    String						ItemName;			///< CONTROLOBJECT の item アノテーションの値
    MMETextureObject*			TextureObject;		///< OFFSCREENRENDERTARGET 等で作成されたテクスチャ (セマンティクスがなくても ResourceName アノテーションがあれば作る)
    int							LightNum;			///< 必要なライト数
	bool						LightParamIsArray;	///< ライト要素を格納する変数が配列の場合
    bool						LightParamIsMatrix;	///< ライト情報が行列の場合は true。false の場合は Vector4 (setMatrix() をつかうか、setVector() を使うかの指定)
};
//
//typedef std::map< String, MMESemantics >			MMESemanticsMap;
//typedef std::pair< String, MMESemantics >			MMESemanticsPair;
//typedef std::map< String, MMEAnnotation >			MMEAnnotationMap;
//typedef std::pair< String, MMEAnnotation >			MMEAnnotationPair;
//typedef std::map< String, MMEShaderVariable* >		MMEShaderVariableMap;
//typedef std::pair< String, MMEShaderVariable* >	MMEShaderVariablePair;
//typedef std::vector< MMEShaderVariable* >				MMEShaderVariableArray;
//typedef std::vector< MMETextureObject* >				MMETextureObjectArray;
//

//-------------------------------------------------------------------------
//

/// Scene 単位のシェーダ設定パラメータ (1フレームに1度だけ設定すればいいパラメータ)
struct MMESceneParams	// TOOD: 昇格
{
 //   const Matrix*	Mat_LightView;         ///< デフォルトライトのビュー行列
 //   const Matrix*	Mat_LightProj;         ///< デフォルトライトのプロジェクション行列
 //   const Matrix*	Mat_LightViewProj;     ///< デフォルトライトのビュー×プロジェクション行列
	////const Matrix*   Mat_LightOrtho;        ///< (未実装)
 //   Vector4			LightPosition;			///< デフォルトライトの位置
 //   Vector4			LightDirection;			///< デフォルトライトの向き
 //   const Vector4*	LightDiffuse;		    ///< デフォルトライトのディフューズ色（拡散光）
	//const Vector4*	LightAmbient;			///< デフォルトライトのアンビエント色（環境光）
	//const Vector4*	LightSpecular;          ///< デフォルトライトのスペキュラ色（鏡面光）

    float			Time;                   ///< 経過時間 (秒) ←ゲーム開始からずっと加算とかだとヤバいので何か対応しておく
	float			ElapsedTime;            ///< 前回のフレームからの経過時間 (秒)
    Vector4			MousePosition;          ///< マウス座標 (x, y)
    Vector4			LeftMouseDown;          ///< マウスの左ボタン情報
    Vector4			MiddleMouseDown;        ///< マウスの中ボタン情報
    Vector4			RightMouseDown;         ///< マウスの右ボタン情報

	//// 以下は calcMatrices() で設定する
 //   Matrix  Mat_LightView_Inverse;
 //   Matrix  Mat_LightProj_Inverse;
 //   Matrix  Mat_LightViewProj_Inverse;
 //   Matrix  Mat_LightView_Transpose;
 //   Matrix  Mat_LightProj_Transpose;
 //   Matrix  Mat_LightViewProj_Transpose;
 //   Matrix  Mat_LightView_InverseTranspose;
 //   Matrix  Mat_LightProj_InverseTranspose;
 //   Matrix  Mat_LightViewProj_InverseTranspose;

 //   void calcMatrices()
 //   {
 //       Mat_LightView_Inverse = Matrix::Inverse(*Mat_LightView );
	//	Mat_LightProj_Inverse = Matrix::Inverse(*Mat_LightProj);
	//	Mat_LightViewProj_Inverse = Matrix::Inverse(*Mat_LightViewProj);
	//	Mat_LightView_Transpose = Matrix::Transpose(*Mat_LightView);
	//	Mat_LightProj_Transpose = Matrix::Transpose(*Mat_LightProj);
	//	Mat_LightViewProj_Transpose = Matrix::Transpose(*Mat_LightViewProj);
 //       Mat_LightView_InverseTranspose = Mat_LightView_Inverse * Mat_LightView_Transpose;
 //       Mat_LightProj_InverseTranspose = Mat_LightProj_Inverse * Mat_LightProj_Transpose;
 //       Mat_LightViewProj_InverseTranspose = Mat_LightViewProj_Inverse * Mat_LightViewProj_Transpose;
 //   }
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
