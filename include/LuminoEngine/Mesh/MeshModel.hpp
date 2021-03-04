﻿#pragma once
#include "Common.hpp"
#include "MeshPrimitive.hpp"

namespace ln {
class MeshSkeleton;
class MeshModel;
class AnimationController;
namespace detail {
class MeshModelInstance;
}
	
// MeshNode を参照するためのデータ構造。
// Node と Bone は似ているが異なるものなので注意。頂点の BLEND_INDICES から参照されるのはこのインスタンス。
// ひとつの Node を複数の Bone が参照する。
// 正直、Bone という名前はやめた方がいい気がする。
// SkinnedMesh でのみ使用する。
class MeshBone
	: public Object
{
public:
	//const String& name() const;

	MeshNode* node() const;
	int nodeIndex() const { return m_node; }

	//const AttitudeTransform& localTransform() const;

	//const Matrix& globalMatrix() const;

public:	// TODO
	MeshSkeleton* m_skeleton;
	int m_node = -1;

	// ボーンの初期姿勢を打ち消して、原点に戻す行列。
	// モデル空間内の絶対座標系となっている。(親ボーンからの相対座標系ではない点に注意)
	Matrix m_inverseInitialMatrix;

	friend class MeshSkeleton;
};

// Bone をまとめるデータ構造。
// BoneTexture を生成する単位。
// ひとつの Model に対して複数インスタンスを持つことがあるが、MeshContainer がどの Skeleton を使って描画するのかを参照する。
class MeshSkeleton
	: public Object
{
public:
	int boneCount() const { return m_bones.size(); }
	MeshBone* bone(int index) const;


	const List<MeshBone*>& rootBones() const { return m_rootBones; }

public:
	// TODO: internal
	void addBone(int linkNode, const Matrix& inverseInitialMatrix);

LN_CONSTRUCT_ACCESS:
	MeshSkeleton();
	bool init(MeshModel* model);

public:	// TODO:
	MeshModel* m_model = nullptr;
	List<Ref<MeshBone>> m_bones;
	List<MeshBone*> m_rootBones;

};

class MeshBoneIKChain
	: public Object
{
public:

	int LinkBoneIndex;        // IK構成ボーン番号
	bool IsRotateLimit;        // 回転制限をするか
	Vector3 MinLimit;            // 下限
	Vector3 MaxLimit;            // 上限
};

class MeshBoneIK
	: public Object
{
public:
	int            IKBoneIndex;            // IKボーン (PMX では、この IK 情報を持つボーンを指す)
	int            IKTargetBoneIndex;        // IKターゲットボーン
	int            LoopCount;                // 演算回数
	float        IKRotateLimit;            // IKループ計算時の1回あたりの制限角度 -> ラジアン角 | PMDのIK値とは4倍異なるので注意

	List<Ref<MeshBoneIKChain>> IKLinks;            // IK影響ボーンと制限のリスト
};

/** MeshModel */
LN_CLASS()
class MeshModel
	: public Object
{
public:
	/** load */
	LN_METHOD()
    static Ref<MeshModel> load(const StringRef& filePath, MeshImportSettings* settings = nullptr);
	//static Ref<MeshModel> loadSkinned(const StringRef& filePath);	// TODO: load 統一でいいかも

	/** findNode */
	LN_METHOD()
	MeshNode* findNode(StringRef name) const;
	
	/** findMaterial */
	LN_METHOD()
	Material* findMaterial(StringRef name) const;

	//MeshContainer* findMeshContainer(StringRef name) const;
	int findNodeIndex(StringRef name) const;

	MeshNode* addNode();
	MeshContainer* addMeshContainer(MeshPrimitive* mesh);
	MeshNode* addMeshContainerNode(MeshPrimitive* mesh);

	[[deprecated]]
	void addMeshContainer(MeshContainer* meshContainer);
	[[deprecated]]
    void addNode(MeshNode* node);
	void addMaterial(Material* material);


	const List<Ref<MeshContainer>>& meshContainers() const { return m_meshContainers; }
    const List<Ref<MeshNode>>& meshNodes() const { return m_nodes; }
    const List<Ref<Material>>& materials() const { return m_materials; }

	
	/** materialCount */
	LN_METHOD()
    int materialCount() const { return m_materials.size(); }
	
	/** load */
	LN_METHOD()
	Material* material(int index);
	
	/** animationController */
	LN_METHOD(Property)
	AnimationController* animationController() const;

	MeshNode* findHumanoidBone(HumanoidBones boneKind) const;


	AABB boundingBox() const { return m_boundingBox; }


    void addRootNode(int index);

	/** 全ノードの Local Transform をリセットします。(アニメーション適用前の、デフォルトの姿勢に戻します) */
	void resetNodeLocalTransforms();

	/** 現在の Local Transform をもとに、全ノードの Global Transform を更新します。 */
	void updateNodeTransforms();

    // TODO: internal
    detail::InternalMeshModelType meshModelType() const { return m_type; }
    //Matrix* nodeGlobalTransformPtr(int nodeIndex) { return &m_nodeGlobalTransforms[nodeIndex]; }
    const Matrix& nodeGlobalTransform(int nodeIndex) { return m_nodeGlobalTransforms[nodeIndex]; }
	
	// TODO: internal
	void addSkeleton(MeshSkeleton* skeleton);
	const Ref<MeshSkeleton>& skeleton(int index) const { return m_skeletons[index]; }
	const List<Ref<MeshSkeleton>>& skeletons() const { return m_skeletons; }

	std::array<int, 56> m_humanoidBoneNodeIndices;	// Index of m_bones
	void setHumanoidBoneIndex(HumanoidBones kind, int boneIndex) { m_humanoidBoneNodeIndices[static_cast<int>(kind)] = boneIndex; }
	int humanoidBoneIndex(HumanoidBones kind) const { return m_humanoidBoneNodeIndices[static_cast<int>(kind)]; }

	// TODO: internal (Skinning)
	void beginUpdate();
	void preUpdate();
	void postUpdate();
	void updateBoneTransformHierarchy();
	void updateIK();
	void writeSkinningMatrices(Matrix* matrixesBuffer, Quaternion* localQuaternionsBuffer);
	void verifyHumanoidBones();
	Ref<detail::MeshModelInstance> createMeshModelInstance();
	bool isStaticMeshModel() const { return m_animationController == nullptr; }
	void calculateBoundingBox();

protected:
	void serialize(Serializer2& ar) override;

LN_CONSTRUCT_ACCESS:
    MeshModel();
    MeshModel(detail::InternalMeshModelType type);

public:	// TODO:
	void clear();
    void updateNodeTransformsHierarchical(int nodeIndex, const Matrix& parentTransform, bool hierarchical);

	detail::AssetPath m_filePath;
	String m_name;
	float m_scale;

    detail::InternalMeshModelType m_type;
	List<Ref<MeshContainer>> m_meshContainers;
    List<Ref<MeshNode>> m_nodes;
	List<Ref<Material>> m_materials;
    List<int> m_rootNodes;

    // 静的データである localTransform に対する動的データ。
    // といっても Mesh はリアルタイム更新はしない。
    // ロード後にまとめて構築するだけ。
    // でも Node は SkinndMesh と共用なので、Node 側に GlobalTransform を持たせるのは
    // データが無駄になったりする。
    List<Matrix> m_nodeGlobalTransforms;

	AABB m_boundingBox;

	List<Ref<MeshBoneIK>> m_iks;
	Ref<AnimationController> m_animationController;

private:
	List<Ref<MeshSkeleton>> m_skeletons;

	friend class detail::MeshManager;
};


class InstancedMeshList
	: public Object
{
public:
	void reset();
	void setTransform(const Matrix& transform);
	void setUVOffset(const Vector4& value);
	void setColorScale(const Color& value);
	void drawMesh();


	Ref<MeshPrimitive> mesh() const { return m_mesh; }
	int sectionIndex() const { return m_sectionIndex; }
	int instanceCount() const { return m_instanceCount; }
	void commitRenderData(MeshSection2* outSection, VertexLayout** outDecl, std::array<VertexBuffer*, 16>* outVBs, int* outVBCount, IndexBuffer** outIB);

LN_CONSTRUCT_ACCESS:
	InstancedMeshList();
	virtual ~InstancedMeshList();
	bool init(MeshPrimitive* mesh, int sectionIndex);

private:
	struct InstanceData
	{
		Vector4 transform0;
		Vector4 transform1;
		Vector4 transform2;
		Vector4 transform3;
		Vector4 uvOffset;
		Vector4 colorScale;
	};

	Ref<MeshPrimitive> m_mesh;
	int m_sectionIndex;
	InstanceData m_stagingData;
	std::vector<InstanceData> m_instanceData;
	int m_instanceCount;
	Ref<VertexLayout> m_vertexLayout;
	Ref<VertexBuffer> m_instanceBuffer;
	VertexLayout* m_sourceVertexLayout;
	bool m_dirty;
};

class MeshHelper
{
public:
	static Box makeAABB(const Vertex* vertices, uint32_t vertexCount);
	static std::array<const Char*, 1> CandidateExtensions_MeshModel;
};



//namespace detail {
//struct ActualMeshImportSettings
//{
//	bool applyBoneTransformationsEnabled;
//};
//}

/** MeshImportSettings */
LN_CLASS()
class MeshImportSettings
	: public Object
{
public:
	static MeshImportSettings* defaultSettings();

	/** (default: true) */
	void setSkeletonImport(bool value);

	bool skeletonImport() const { return m_skeletonImport; }


	/** */
	void setCharacterModelFormat(Optional<bool> value) { m_isCharacterModelFormat = value; }

	Optional<bool> isCharacterModelFormat() const { return m_isCharacterModelFormat; }

	
	/**
	 * Bone 姿勢を平行移動成分へ適用するかどうかを設定します。(default: true)
	 *
	 * Bone 姿勢に含まれている回転とスケーリングを、平行移動成分へ適用し、リセットします。
	 * これによって、すべての Bone は回転とスケーリングがリセットされ、平行移動のみで初期姿勢を表すようになります。
	 * 
	 * このオプションは、インポートしたモデルに共有モーションデータをアタッチする際に必須となります。
	 *
	 * @note
	 * Blender からエクスポートした glTF などは Bone に回転情報がついていることがある。
	 * 一方 BVH モーションデータはこうした初期姿勢としての回転情報は考慮されない。
	 * このためアタッチしようとすると、Bone の初期姿勢回転の分だけ余計に回転されることになり、モーションが崩れて見える。
	 *
	 * また、IK などプログラム上から回転させるときも、計算後に初期姿勢分の回転を減算するような処理が必要となり、複雑になってしまう。
	 * IK のようなエンジンの内部処理ならいいが、ユーザープログラムから Bone を操作したい場合に原因の分かりづらい問題になりやすい。
	 */
	void setApplyBoneTransformationsEnabled(Optional<bool> value);

	const Optional<bool>& applyBoneTransformationsEnabled() const { return m_applyBoneTransformationsEnabled; }

	/**
	 * 右手座標系と左手座標系の変換を行うかどうかを設定します。(default: true)
	 *
	 * 変換されたモデルは Z 軸が反転します。
	 */
	//void setFlipZCoordinate(Optional<bool> value);

	//const Optional<bool>& flipZCoordinate() const { return m_flipZCoordinate; }

	bool flipZ = false;
	bool flipX = false;

LN_CONSTRUCT_ACCESS:
	MeshImportSettings();

	/** init */
	LN_METHOD()
	bool init();

private:
	bool m_skeletonImport;
	Optional<bool> m_isCharacterModelFormat;
	Optional<bool> m_applyBoneTransformationsEnabled;

	//Optional<bool> m_flipZCoordinate;
};


class MeshDiag
{
public:
	static void printNodes(const MeshModel* model);
	static void clearBoneInitialRotations(MeshModel* model);
};

} // namespace ln

