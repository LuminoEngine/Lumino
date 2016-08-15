
#pragma once
#include <Lumino/Graphics/Mesh.h>

LN_NAMESPACE_BEGIN
class PmxSkinnedMeshResource;

/// モデルファイルのフォーマット
enum ModelFormat
{
	ModelFormat_Unknown = 0,
	ModelFormat_PMD,
	ModelFormat_PMX,
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

/// 共有マテリアルデータ
class PmxMaterialResource
	: public RefObject
{
public:

	/// 描画オプション
	enum DrawingFlags
	{
		DrawingFlag_CullingDouble = 0x01,	///< 両面描画
		DrawingFlag_GroundShadow = 0x02,	///< 地面影
		DrawingFlag_SelfShadowMap = 0x04,	///< セルフシャドウマップへの描画
		DrawingFlag_SelfShadow = 0x08,		///< セルフシャドウの描画
		DrawingFlag_Edge = 0x10,			///< エッジ描画
	};

	/// スフィアテクスチャの合成モード
	enum SphereMode
	{
		SphereMode_Disable = 0,				///< 無効
		SphereMode_Mul = 1,					///< 乗算(sph)
		SphereMode_Add = 2,					///< 加算(spa)
		SphereMode_SubTexture = 3,			///< サブテクスチャ(追加UV1のx,yをUV参照して通常テクスチャ描画を行う)
	};

public:
	// TODO: 材質名 なども

	Color						Diffuse;			///< 物体の色
	Color						Ambient;			///< 環境光
	Color						Specular;			///< 光沢
	Color						Emissive;			///< 物体の発光色 ( 光源の影響を受けない色 )
	float						Power;				///< 光沢の強さ

	RefPtr<Shader>				Shader;				///< シェーダ
	RefPtr<ln::Texture>			Texture;			///< テクスチャ
	RefPtr<ln::Texture>			ToonTexture;		///< [PMD] トゥーンテクスチャ
	RefPtr<ln::Texture>			SphereTexture;		///< [PMD] スフィアテクスチャ

	Color						ToonColor;			///< [PMD] トゥーンカラー
	Color						EdgeColor;			///< [PMX] エッジカラー
	float						EdgeSize;			///< [PMX] エッジサイズ
	Color						TextureCoe;			///< [PMX] テクスチャ係数
	Color						SphereTextureCoe;	///< [PMX] スフィアテクスチャ係数
	Color						ToonTextureCoe;		///< [PMX] Toonテクスチャ係数
	uint32_t					DrawingFlags;		///< [PMX] 描画オプション (MMDDrawingFlags の組み合わせ)
	SphereMode					SphereMode;			///< [PMX] スフィアテクスチャの合成モード

public:

	PmxMaterialResource()
	{
		Diffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
		Ambient.Set(0.0f, 0.0f, 0.0f, 0.0f);
		Specular.Set(0.5f, 0.5f, 0.5f, 0.5f);
		Emissive.Set(0.0f, 0.0f, 0.0f, 0.0f);
		Power = 50.0f;
		ToonColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
		EdgeColor.Set(0.0f, 0.0f, 0.0f, 1.0f);
		EdgeSize = 0.0f;
		SphereMode = SphereMode_Disable;
		DrawingFlags = 0;
	}

	RefPtr<Material> MakeCommonMaterial() const;
};

/// 共有ボーンデータ
class PmxBoneResource
	: public RefObject
{
public:
	PmxBoneResource(PmxSkinnedMeshResource* owner, int boneIndex);
	void RefreshInitialValues();
	const Vector3& GetOffsetFromParent() const { return m_offsetFromParent; }
	const Matrix& GetInitialTranstormInv() const { return m_initialTranstormInv; }

public:
	String				Name;						///< ボーン名
	//String			EnglishName;				///< ボーン英名
	Vector3				OrgPosition;				///< モデル原点からの位置
	int					ParentBoneIndex;			///< 親ボーンのインデックス (-1 は無し)
	int					TransformLevel;				///< 変形階層  TOOD: IKボーン及びそのIKを親としている系列の変形階層を 0→1、回転影響下以下の変形階層を 0→2 (SortVal の考え方な気がする。MMM では使ってないみたい)

	// ↓ボーンフラグ
	BoneConnectType		BoneConnect;				///< 接続先(PMD子ボーン指定)表示方法 -> 0:座標オフセットで指定 1:ボーンで指定
	bool				CanRotate;					///< 回転可能
	bool				CanMove;					///< 移動可能
	bool				IsVisible;					///< 表示
	bool				CanOperate;					///< 操作可
	bool				IsIK;						///< IK
	LocalProvideType	LocalProvide;				///< ローカル付与 | 付与対象 0:ユーザー変形値／IKリンク／多重付与 1:親のローカル変形量
	bool				IsRotateProvided;			///< 回転付与
	bool				IsMoveProvided;				///< 移動付与
	bool				IsFixAxis;					///< 軸固定
	bool				IsLocalAxis;				///< ローカル軸
	bool				TransformAfterPhysics;		///< 物理後変形
	bool				ParentTransform;			///< 外部親変形
	// ↑ボーンフラグ

	// ↓ボーンフラグにより使用するデータ
	Vector3				PositionOffset;				///< [接続先:0 の場合] 座標オフセット, ボーン位置からの相対分
	int					ConnectedBoneIndex;			///< [接続先:1 の場合] 接続先ボーンのボーンIndex
	int					ProvidedParentBoneIndex;	///< [回転付与:1 または 移動付与:1 の場合] 付与親ボーンのボーンIndex
	float				ProvidedRatio;				///< [回転付与:1 または 移動付与:1 の場合] 付与率
	Vector3				AxisDirectionVector;		///< [軸固定:1 の場合] 軸の方向ベクトル
	Vector3				DimentionXDirectionVector;	///< [ローカル軸:1 の場合] X軸の方向ベクトル
	Vector3				DimentionZDirectionVector;	///< [ローカル軸:1 の場合] Z軸の方向ベクトル
	int					KeyValue;					///< [外部親変形:1 の場合] Key値

private:
	PmxSkinnedMeshResource*	m_owner;
	int						m_boneIndex;
	Vector3					m_offsetFromParent;			///< (RefreshInitialValues() で設定される) 親ボーンのからの相対位置 (親OrgPosition - OrgPosition)
	Matrix					m_initialTranstormInv;		///< (RefreshInitialValues() で設定される) モデル座標系内の初期姿勢の逆行列
};


/// 共有IKデータ
class PmxIKResource
	: public RefObject
{
public:
	struct IKLink
	{
		int			LinkBoneIndex;		///< IK構成ボーン番号
		bool		IsRotateLimit;		///< 回転制限をするか
		Vector3		MinLimit;			///< 下限
		Vector3		MaxLimit;			///< 上限
	};

public:
	/* ① - ② - ③    ④
	*
	*   ①② : IK影響下ボーン
	*   ③	: IKTargetBoneIndex (PMX 仕様書では「IKターゲットボーン」、MMDX 等では「エフェクタ」と呼ばれる)
	*   ④	: IKBone (PMX 仕様書 では「IKボーン」、MMDX 等では「ターゲットボーン」と呼ばれる。かかと等)
	*/

	int			IKBoneIndex;			///< IKボーン (PMX では、この IK 情報を持つボーンを指す)
	int			IKTargetBoneIndex;		///< IKターゲットボーン
	int			LoopCount;				///< 演算回数
	float		IKRotateLimit;			///< IKループ計算時の1回あたりの制限角度 -> ラジアン角 | PMDのIK値とは4倍異なるので注意

	Array<IKLink> IKLinks;			///< IK影響ボーンと制限のリスト

	/* PMD の場合の IKRotateLimit は以下の計算結果を格納する
	*		PI * Fact * (iLink + 1)
	*/
};


/// 共有モーフデータ
class PmxMorphResource
	: public RefObject
{
public:
	//ModelMorphCore();
	//virtual ~ModelMorphCore();

public:

	String		Name;						///< モーフ名
	//String	EnglishName;				///< モーフ英名

	int				OperationPanel;			///< 操作パネル (PMD:カテゴリ) 1:眉(左下) 2:目(左上) 3:口(右上) 4:その他(右下)  | 0:システム予約
	ModelMorphType	MorphType;

	union MorphOffset
	{
		struct
		{
			int			MorphIndex;			///< モーフIndex
			float		MorphRatio;			///< モーフ率 : グループモーフのモーフ値 * モーフ率 = 対象モーフのモーフ値
		} GroupMorphOffset;
		struct
		{
			int			VertexIndex;		///< 頂点Index
			float		PositionOffset[3];	///< 座標オフセット量(x,y,z)
		} VertexMorphOffset;
		struct
		{
			int			BoneIndex;			///< ボーンIndex
			float		Moving[3];			///< 移動量(x,y,z)
			float		Rotating[4];		///< 回転量-クォータニオン(x,y,z,w)
		} BoneMorphOffset;
		struct
		{
			int			VertexIndex;		///< 頂点Index
			float		UVOffset[4];		///< UVオフセット量(x,y,z,w) ※通常UVはz,wが不要項目になるがモーフとしてのデータ値は記録しておく
		} UVMorphOffset;
		struct
		{
			int			MaterialIndex;		///< 材質Index -> -1:全材質対象
			uint8_t		OffsetCalcType;		///< オフセット演算形式 | 0:乗算, 1:加算 - 詳細は後述
			float		Diffuse[4];			///< Diffuse (R,G,B,A) - 乗算:1.0／加算:0.0 が初期値となる(同以下)
			float		Specular[3];		///< Specular (R,G,B)
			float		SpecularCoe;		///< Specular係数
			float		Ambient[3];			///< Ambient (R,G,B)
			float		EdgeColor[4];		///< エッジ色 (R,G,B,A)
			float		EdgeSize;			///< エッジサイズ
			float		TextureCoe[4];		///< テクスチャ係数 (R,G,B,A)
			float		SphereTextureCoe[4];///< スフィアテクスチャ係数 (R,G,B,A)
			float		ToonTextureCoe[4];	///< Toonテクスチャ係数 (R,G,B,A)
		} MaterialMorphOffset;
		struct
		{
			int			MorphIndex;			///< モーフIndex
			float		MorphValue;
		} FlipMorphOffset;
		struct
		{
			int			RigidIndex;
			uint8_t		LocalFlag;
			float		Moving[3];
			float		Rotating[3];
		} ImpulseMorphOffset;
	};

	Array<MorphOffset>	MorphOffsets;
};

/// 共有剛体データ
class PmxRigidBodyResource
	: public RefObject
{
public:

	/// 形状データ
	struct CollisionShapeData
	{
		CollisionShapeType Type;
		union
		{
			struct
			{
				float Radius;
			} Sphere;

			struct
			{
				float Width;
				float Height;
				float Depth;
			} Box;

			struct
			{
				float Radius;
				float Height;
			} Capsule;
		};
	};

public:
	String				Name;				///< 剛体名
	//String			EnglishName;		///< 剛体英名
	int					RelatedBoneIndex;	///< 関連ボーン
	uint16_t			Group;				///< 衝突グループ
	uint16_t			GroupMask;			///< 衝突グループマスク
	RigidBodyType		RigidBodyType;		///< 形状
	CollisionShapeData	ColShapeData;		///< 形状サイズ
	Matrix				InitialTransform;	///< モデル座標空間内での初期姿勢 (そのまま剛体の初期姿勢になる)



	//Matrix				BoneOffset;			///< ボーン行列 → 剛体行列変換用 (グローバル行列ではない)
	//Matrix				InvBoneOffset;		///< 剛体行列 → ボーン行列変換用


	float				Mass;				///< 質量
	float				LinearDamping;		///< 移動減
	float				AngularDamping;		///< 回転減
	float				Restitution;		///< 反発力
	float				Friction;			///< 摩擦力

};

/// 共有ジョイントデータ
class PmxJointResource
	: public RefObject
{
public:
	String				Name;						///< ジョイント名
	//String			EnglishName;				///< ジョイント英名
	int					RigidBodyAIndex;			///< 結合剛体 A
	int					RigidBodyBIndex;			///< 結合剛体 B
	Vector3				Position;					///< 位置(モデル原点中心)
	Vector3				Rotation;					///< 回転(radian)
	Vector3				PositionLimitLower;			///< 移動制限1
	Vector3				PositionLimitUpper;			///< 移動制限2
	Vector3				RotationLimitLower;			///< 回転制限1
	Vector3				RotationLimitUpper;			///< 回転制限2
	Vector3				SpringPositionStiffness;	///< ばね移動
	Vector3				SpringRotationStiffness;	///< ばね回転
};

/// モデルの共有データクラス
class PmxSkinnedMeshResource
	: public MeshResource
	, public ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	virtual ~PmxSkinnedMeshResource();
	void RefreshInitialValues();

public:
	ModelFormat				Format;

	String					Name;				///< モデル名
	//String				EnglishName;		///< モデル英名
	//String				Comment;			///< コメント
	//String				EnglishComment;		///< コメント英

	Array<RefPtr<PmxMaterialResource>>	materials;	// マテリアルリスト
	Array<RefPtr<PmxBoneResource>>		bones;		// ボーンリスト
	Array<RefPtr<PmxIKResource>>		iks;		// IK リスト
	Array<RefPtr<PmxMorphResource>>		morphs;		// モーフリスト
	Array<RefPtr<PmxRigidBodyResource>>	rigidBodys;	// 剛体リスト
	Array<RefPtr<PmxJointResource>>		joints;		// ジョイントリスト
};

LN_NAMESPACE_END

