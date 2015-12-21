
#pragma once
#include <Lumino/Graphics/Material.h>

LN_NAMESPACE_BEGIN

class ModelCore;
class ModelBoneCore;
class ModelIKCore;
class ModelMorphCore;
class ModelRigidBodyCore;
class ModelJointCore;

typedef Array<Material>				MaterialList;
typedef Array<ModelBoneCore*>		ModelBoneCoreList;
typedef Array<ModelIKCore*>			ModelIKCoreList;
typedef Array<ModelMorphCore*>		ModelMorphCoreList;
typedef Array<ModelRigidBodyCore*>	ModelRigidBodyCoreList;
typedef Array<ModelJointCore*>		ModelJointCoreList;

class ModelBone;
typedef Array<ModelBone*>			ModelBoneList;


/// モデルファイルのフォーマット
enum ModelFormat
{
	ModelFormat_Unknown = 0,
	ModelFormat_PMD,
	ModelFormat_PMX,
};

/// モデル生成オプション
LN_ENUM_FLAGS(ModelCreationFlag)
{
	None = 0x0000,
	IgnoreTextureNotFound = 0x04,		///< マテリアルに含まれるテクスチャファイルが見つからなくても無視する
	//ModelCreateFlag_CopyMaterials = 0x08,		///< マテリアルを共有せずにコピーする
};
LN_ENUM_FLAGS_DECLARE(ModelCreationFlag)

/// メッシュの属性
struct MeshAttribute
{
	int		MaterialIndex;  ///< 対応するマテリアル番号
	int		StartIndex;     ///< 開始インデックス
	int		PrimitiveNum;   ///< 描画プリミティブ数 (三角形の数)
};
typedef Array<MeshAttribute>		MeshAttributeList;

/// 材質の描画オプション
enum MMDDrawingFlags
{
	MMDDrawingFlag_CullingDouble = 0x01,	///< 両面描画
	MMDDrawingFlag_GroundShadow = 0x02,		///< 地面影
	MMDDrawingFlag_SelfShadowMap = 0x04,	///< セルフシャドウマップへの描画
	MMDDrawingFlag_SelfShadow = 0x08,		///< セルフシャドウの描画
	MMDDrawingFlag_Edge = 0x10,				///< エッジ描画
};

/// ボーンフラグ 接続先(PMD子ボーン指定)表示方法
enum BoneConnectType
{
	BoneConnectType_PositionOffset = 0,		///< 座標オフセットで指定
	BoneConnectType_Bone,					///< ボーンで指定
};

/// ボーンフラグ ローカル付与 
enum LocalProvideType
{
	LocalProvideType_UserTransformValue = 0,	///< ユーザー変形値／IKリンク／多重付与
	LocalProvideType_ParentLocalTransformValue,	///< 親のローカル変形量
};

/// モーフ種別
enum ModelMorphType
{
	ModelMorphType_Vertex = 0,
	ModelMorphType_UV,
	ModelMorphType_AdditionalUV1,
	ModelMorphType_AdditionalUV2,
	ModelMorphType_AdditionalUV3,
	ModelMorphType_AdditionalUV4,
	ModelMorphType_Bone,
	ModelMorphType_Matrial,
	ModelMorphType_Group,
	ModelMorphType_Flip,
	ModelMorphType_Impulse,
};

/// 剛体の形状
enum CollisionShapeType
{
	CollisionShapeType_Sphere = 0,		///< 球
	CollisionShapeType_Box,				///< 箱
	CollisionShapeType_Capsule,			///< カプセル    
};

/// 剛体の演算種別
enum RigidBodyType
{
	RigidBodyType_ControlledByBone = 0,	///< Bone追従
	RigidBodyType_Physics,				///< 物理演算
	RigidBodyType_PhysicsAlignment,		///< 物理演算(Bone位置合せ)
};

// PMX モデルの頂点情報
struct PMX_Vertex
{
public:

	Vector3	Position;			///< 位置
	float	BlendWeights[4];	///< ボーンブレンド率
	float	BlendIndices[4];	///< ボーンインデックス
	Vector3	Normal;				///< 法線
	ColorF	Color;				///< 頂点カラー (ライブラリ独自仕様)
	Vector2	TexUV;				///< テクスチャUV

	Vector4	AdditionalUV[4];	///< 追加UV
	Vector4	SdefC;				///< Sdef - C値
	Vector3	SdefR0;				///< Sdef - R0値
	Vector3	SdefR1;				///< Sdef - R1値

	float	EdgeWeight;			///< エッジウェイト
	float	Index;				///< 頂点インデックス値

	/// 頂点レイアウト
	static VertexElement* Elements()
	{
		static VertexElement elements[] = {
			{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
			{ 0, VertexElementType_Float4, VertexElementUsage_BlendWeight, 0 },
			{ 0, VertexElementType_Float4, VertexElementUsage_BlendIndices, 0 },
			{ 0, VertexElementType_Float3, VertexElementUsage_Normal, 0 },
			{ 0, VertexElementType_Float4, VertexElementUsage_Color, 0 },
			{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0 },

			{ 0, VertexElementType_Float4, VertexElementUsage_TexCoord, 1 },
			{ 0, VertexElementType_Float4, VertexElementUsage_TexCoord, 2 },
			{ 0, VertexElementType_Float4, VertexElementUsage_TexCoord, 3 },
			{ 0, VertexElementType_Float4, VertexElementUsage_TexCoord, 4 },
			{ 0, VertexElementType_Float4, VertexElementUsage_TexCoord, 5 },
			{ 0, VertexElementType_Float3, VertexElementUsage_TexCoord, 6 },
			{ 0, VertexElementType_Float3, VertexElementUsage_TexCoord, 7 },

			{ 0, VertexElementType_Float1, VertexElementUsage_TexCoord, 8 },
			{ 0, VertexElementType_Float1, VertexElementUsage_PointSize, 15 },
		};
		return elements;
	}
	static const int ElementCount = 15;
};

namespace detail
{
	class ModelManager;
}
LN_NAMESPACE_END

