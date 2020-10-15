#pragma once
#include "Mesh.hpp"

namespace ln {

/** StaticMeshModel */
LN_CLASS()
class StaticMeshModel
	: public Object
{
public:
	/** load */
	LN_METHOD()
    static Ref<StaticMeshModel> load(const StringRef& filePath);

	MeshNode* findNode(StringRef name) const;
	//MeshContainer* findMeshContainer(StringRef name) const;
	int findNodeIndex(StringRef name) const;

	MeshNode* addNode();
	MeshContainer* addMeshContainer(Mesh* mesh);
	MeshNode* addMeshContainerNode(Mesh* mesh);

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

    void addRootNode(int index);

	/** 全ノードの Local Transform をリセットします。(アニメーション適用前の、デフォルトの姿勢に戻します) */
	void resetNodeLocalTransforms();

	/** 現在の Local Transform をもとに、全ノードの Global Transform を更新します。 */
	void updateNodeTransforms();

    // TODO: internal
    detail::InternalMeshModelType meshModelType() const { return m_type; }
    //Matrix* nodeGlobalTransformPtr(int nodeIndex) { return &m_nodeGlobalTransforms[nodeIndex]; }
    const Matrix& nodeGlobalTransform(int nodeIndex) { return m_nodeGlobalTransforms[nodeIndex]; }

protected:
	void serialize(Serializer2& ar) override;

LN_CONSTRUCT_ACCESS:
    StaticMeshModel();
    StaticMeshModel(detail::InternalMeshModelType type);

public:	// TODO:
	void clear();
    void updateNodeTransformsHierarchical(int nodeIndex, const Matrix& parentTransform, bool hierarchical);

	detail::AssetPath m_filePath;
	float m_scale;

    detail::InternalMeshModelType m_type;
	List<Ref<MeshContainer>> m_meshContainers;
    List<Ref<MeshNode>> m_nodes;
	List<Ref<Material>> m_materials;
    List<int> m_rootNodes;

    // 静的データである localTransform に対する動的データ。
    // といっても StaticMesh はリアルタイム更新はしない。
    // ロード後にまとめて構築するだけ。
    // でも Node は SkinndMesh と共用なので、Node 側に GlobalTransform を持たせるのは
    // データが無駄になったりする。
    List<Matrix> m_nodeGlobalTransforms;

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


	Ref<Mesh> mesh() const { return m_mesh; }
	int sectionIndex() const { return m_sectionIndex; }
	int instanceCount() const { return m_instanceCount; }
	void commitRenderData(MeshSection2* outSection, VertexLayout** outDecl, std::array<VertexBuffer*, 16>* outVBs, int* outVBCount, IndexBuffer** outIB);

LN_CONSTRUCT_ACCESS:
	InstancedMeshList();
	virtual ~InstancedMeshList();
	bool init(Mesh* mesh, int sectionIndex);

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

	Ref<Mesh> m_mesh;
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

class MeshImportSettings
	: public Object
{
public:
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
	bool init();

private:
	Optional<bool> m_applyBoneTransformationsEnabled;
	//Optional<bool> m_flipZCoordinate;
};

} // namespace ln

