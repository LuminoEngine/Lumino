#pragma once
#include "../Graphics/Common.hpp"
#include "../Rendering/Vertex.hpp"

namespace ln {
class VertexLayout;
class VertexBuffer;
class IndexBuffer;
class Material;
class MeshContainer;
class StaticMeshModel;
class MeshGeometryBuilder;
namespace detail {
class MeshManager;


}


enum class InterleavedVertexGroup
{
	Main,			// struct Vertex
	Skinning,		// struct VertexBlendWeight
	AdditionalUV,	// struct VertexAdditionalUV
	Undefined,
};

struct MeshSection
{
	/** 開始インデックス */
	int startIndex;

	/** 描画プリミティブ数 */
	int	primitiveCount;

	/** 対応するマテリアル番号 */
	int	materialIndex;
};

// LOD
// Triangle
class MeshResource
	: public Object
{
public:
	/** 頂点の数を取得します。 */
	int vertexCount() const;

	/** インデックスの数を取得します。 */
	int indexCount() const;

	/** 面の数を取得します。 */
	int faceCount() const;

	/** 頂点の数を変更します。 */
	void resizeVertexBuffer(int vertexCount);

	/** インデックスの数を変更します。 */
	void resizeIndexBuffer(int indexCount);

	/** セクションの数を変更します。 */
	void resizeSections(int sectionCount);	// TODO: setSectionCount のほうがいいかな？Unity は mesh.submeshCount = 2; だし

	/** 頂点を設定します。 */
	void setVertex(int index, const Vertex& value);

    /** 頂点を取得します。 */
    const Vertex& vertex(int index);

	/** インデックス値を設定します。 */
	void setIndex(int index, int value);

    /** インデックス値を取得します。 */
    int index(int index);

	/** セクションの情報を設定します。 */
	void setSection(int sectionIndex, int startIndex, int primitiveCount, int materialIndex);

    /** セクションの情報を追加します。 */
    void addSection(int startIndex, int primitiveCount, int materialIndex);

    MeshContainer* ownerContainer() const { return m_ownerContainer; }

    // TODO: タイミング
    void setGraphicsResourceUsage(GraphicsResourceUsage value) { m_usage = value; }

	// TODO: internal
	void commitRenderData(int sectionIndex, MeshSection* outSection, VertexLayout** outDecl, VertexBuffer** outVBs, int* outVBCount, IndexBuffer** outIB);
	const List<MeshSection>& sections() const { return m_sections; }
    void getVertexBuffers(VertexBuffer** basic);
    void setVertexBuffers(VertexBuffer* basic);
    bool isInitialEmpty() const;

    // TODO: MMD
    void setBlendWeights(int index, float v0, float v1, float v2, float v3);
    void setBlendIndices(int index, float v0, float v1, float v2, float v3);	// TODO: int
    const VertexBlendWeight& vertexBlendWeight(int index);
    void setSdefInfo(int index, const Vector4& sdefC, const Vector3& sdefR0, const Vector3& sdefR1);
    void setMmdExtra(int index, float edgeWeight, float vertexIndex);


LN_CONSTRUCT_ACCESS:
	MeshResource();
	virtual ~MeshResource();

	/** MeshResource を作成します。 */
	void init();

public: // TODO: 
	enum VertexBufferGroup
	{
		VBG_Basic = 0,
		VBG_BlendWeights = 1,
		VBG_AdditionalUVs = 2,
		VBG_SdefInfo = 3,
		VBG_MmdExtra = 4,
		VBG_Count,
	};

	int realVertexCount(VertexBufferGroup group) const;
	int realIndexCount() const;
	void requestBuffers(VertexBufferGroup group, VertexBuffer** outVertexBuffer, IndexBuffer** outIndexBuffer);
	void* requestVertexData(VertexBufferGroup group);
	void* requestIndexData(IndexBufferFormat* outFormat);
	void* requestVertexDataForAdditional(int additionalVertexCount, VertexBufferGroup group);
	void* requestIndexDataForAdditional(int additionalIndexCount, IndexBufferFormat* outFormat);

	static const std::array<size_t, VBG_Count> VertexStrideTable;

	detail::MeshManager* m_manager;
    MeshContainer* m_ownerContainer;
	int m_vertexCount;
	int m_indexCount;
	GraphicsResourceUsage m_usage;
	std::array<Ref<VertexBuffer>, VBG_Count> m_vertexBuffers;
	Ref<IndexBuffer> m_indexBuffer;
	List<MeshSection> m_sections;

	friend class MeshContainer;
};



struct MeshSection2
{
	/** 開始インデックス */
	int startIndex;

	/** 描画プリミティブ数 */
	int	primitiveCount;

	/** 対応するマテリアル番号 */
	int	materialIndex;

	PrimitiveTopology topology;
};

// LOD
// 方針：サイズは固定。内容は変更可。
/*
	頂点レイアウトは次のようになっている。
		Stream.0:
			Float3 Position.0
			Float3 Normal.0
			Float2 TexCoord.0
			Float4 Color.0
		Stream.1:
			Float3 BlendWeight.0
			Float4 BlendIndices.0
		Stream.X:
			...

	Stream 0, 1, 2 は予約済み。
	0 は必須。
	1, 2 は要求された場合のみ作られる。

	3~ はユーザー定義できる。
	今は主に glTF 形式のモデルを読むときのカスタムフォーマットのために用意されている。
*/
// MeshLOD は複数の Container で共有可能とする。
// 特にデータサイズが大きくなるオブジェクトなので、一般的な形状では共有したいケースが多い。
// そのため owner などのフィールドは持たない。
class Mesh
	: public Object
{
public:
	/** 頂点の数を取得します。 */
	int vertexCount() const { return m_vertexCount; }

	/** インデックスの数を取得します。 */
	int indexCount() const { return m_indexCount; }

	/** 頂点を設定します。 */
	void setVertex(int index, const Vertex& value);

	/** 頂点を取得します。 */
	const Vertex& vertex(int index);

	/** インデックス値を設定します。 */
	void setIndex(int index, int value);

	/** インデックス値を取得します。 */
	int index(int index);


	/** セクションの情報を追加します。 */
	void addSection(int startIndex, int primitiveCount, int materialIndex, PrimitiveTopology topology);
	void setSection(int sectionIndex, int startIndex, int primitiveCount, int materialIndex, PrimitiveTopology topology);

	// TODO: internal
	void commitRenderData(int sectionIndex, MeshSection2* outSection, VertexLayout** outDecl, std::array<VertexBuffer*, 16>* outVBs, int* outVBCount, IndexBuffer** outIB);
    const List<MeshSection2>& sections() const { return m_sections; }

	void resetVertexBuffer(int vertexCount);
	void resetIndexBuffer(int indexCount, IndexBufferFormat indexFormat);
	InterleavedVertexGroup getStandardElement(VertexElementUsage usage, int usageIndex) const;
	void* acquireMappedVertexBuffer(InterleavedVertexGroup group);
	void* acquireMappedVertexBuffer(VertexElementType type, VertexElementUsage usage, int usageIndex);
	void* acquireMappedIndexBuffer();
	VertexElementType findVertexElementType(VertexElementUsage usage, int usageIndex) const;
	IndexBufferFormat indexBufferFormat() const { return m_indexFormat; }
	void calculateTangents();
	bool isAllTriangleLists() const { return !m_sections.containsIf([](const MeshSection2& x) { return x.topology != PrimitiveTopology::TriangleList; }); }

LN_CONSTRUCT_ACCESS:
	Mesh();
	virtual ~Mesh();
	void init();
	void init(int vertexCount, int indexCount);
	void init(int vertexCount, int indexCount, IndexBufferFormat indexFormat, GraphicsResourceUsage resourceUsage);
	// TODO: section 1作る init ユーティリティも欲しい。addSection は本当に忘れやすい

private:
	void attemptResetVertexLayout();

	struct VertexBufferEntry
	{
		Ref<VertexBuffer> buffer;
		void* mappedBuffer = nullptr;

		void reset();
	};

	struct IndexBufferEntry
	{
		Ref<IndexBuffer> buffer;
		void* mappedBuffer = nullptr;

		void reset();
	};

	struct VertexBufferAttribute
	{
        VertexElementType type;
		VertexElementUsage usage;
        int usageIndex;
		VertexBufferEntry entry;
	};

	//MeshContainer* m_owner = nullptr;
	VertexBufferEntry m_mainVertexBuffer;		// struct Vertex. (Pos0, Normal0, UV0, Color0)
	VertexBufferEntry m_skinningVertexBuffer;	// BlendWeignt0, BlendIndex0
	VertexBufferEntry m_additionalUVVertexBuffer;	// TEXCOORD_1, TEXCOORD_2,TEXCOORD_3
	List<VertexBufferAttribute> m_extraVertexBuffers;
	IndexBufferEntry m_indexBuffer;
    Ref<VertexLayout> m_vertexLayout;
	List<MeshSection2> m_sections;

	int m_vertexCount;
	int m_indexCount;
	IndexBufferFormat m_indexFormat;
	GraphicsResourceUsage m_resourceUsage;

	friend class MeshGeometryBuilder;
	//friend class MeshContainer
};



// ひとつのメッシュモデルデータ内にいくつかのメッシュノードが含まれているとき、それを名前検索するために使用する。
// 例えば、フィールドのモデルに ビジュアル用のメッシュとコリジョン用のメッシュが含まれている場合、名前検索でコリジョンを取り出して Phyiscs モジュールに渡したりする。
// また、LOD の管理も行う。
class MeshContainer
	: public Object
{
public:
	/** 名前を取得します。 */
	const ln::String& name() const { return m_name; }

	/** 名前を設定します。 */
	void setName(const StringRef& name) { m_name = name; }

	///** メインの MeshResource を設定します。 */
	//void setMeshResource(MeshResource* mesh);

	///** メインの MeshResource を取得します。 */
	//MeshResource* meshResource() const;

 //   void addMeshResource(MeshResource* mesh);

	/** メッシュの境界ボックスを取得します。 */
	const Box& bounds() const { return m_boundingBox; }

	// TODO: internal
	//MeshResource* selectLODResource(float distance) const;

	void calculateBounds();


    void setMesh(Mesh* mesh);
    Mesh* mesh() const;


LN_CONSTRUCT_ACCESS:
	MeshContainer();
	virtual ~MeshContainer();

	/** MeshContainer を作成します。 */
	void init();

private:
	int m_index = -1;
	ln::String m_name;
	Box m_boundingBox;
	//List<Ref<MeshResource>> m_lodResources; // TODO: :obsolete
    List<Ref<Mesh>> m_lodMesh;

	friend class StaticMeshModel;
};

// Bone and Container
class MeshNode : public Object
{
public:
	void setPosition(const Vector3& value);
	void setPosition(float x, float y, float z) { setPosition(Vector3(x, y, z)); }

	void setRotation(const Quaternion& value);
	void setRotation(float x, float y, float z) { setRotation(Quaternion::makeFromEulerAngles(Vector3(x, y, z))); }

	void setScale(const Vector3& value);
	void setScale(float x, float y, float z) { setScale(Vector3(x, y, z)); }
	void setScale(float xyz) { setScale(Vector3(xyz, xyz, xyz)); }

	void setTransform(const AttitudeTransform& value);
	const AttitudeTransform& localTransform() const { return m_localTransform; }

	void setName(const String& value) { m_name = value; }
	const String& name() const { return m_name; }

    int index() const { return m_index; }

    void setMeshContainerIndex(int value);
    int meshContainerIndex() const { return m_meshContainerIndex; }

	int skeletonIndex = -1;

    void addChildIndex(int value);

    void setInitialLocalTransform(const Matrix& value);
	void resetLocalTransform();

    const Matrix& initialLocalTransform() const { return m_initialLocalTransform; }
    
	const Matrix& globalMatrix() const;

	void updateGlobalTransform(bool hierarchical);

LN_CONSTRUCT_ACCESS:
	MeshNode();
    virtual ~MeshNode() = default;

private:
	StaticMeshModel* m_model;
	String m_name;
    int m_index;
    int m_meshContainerIndex;
	int m_parent;
    List<int> m_children;

	// デフォルトの、親ノードからの相対姿勢。
	// SkinnedMesh でもボーンの初期姿勢として参照するが、この値が実行中に変わることは無い。
    Matrix m_initialLocalTransform;

	// アニメーションを書き込むのはここ
	AttitudeTransform m_localTransform;

    friend class StaticMeshModel;
};

namespace detail {
enum class InternalMeshModelType
{
    StaticMesh,
    SkinnedMesh,
};
}




class StaticMeshModel
	: public Object
{
public:
    static Ref<StaticMeshModel> load(const StringRef& filePath, float scale = 1.0f);

	MeshNode* findNode(StringRef name) const;
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

    void addRootNode(int index);


    // TODO: internal
    detail::InternalMeshModelType meshModelType() const { return m_type; }
    //Matrix* nodeGlobalTransformPtr(int nodeIndex) { return &m_nodeGlobalTransforms[nodeIndex]; }
    const Matrix& nodeGlobalTransform(int nodeIndex) { return m_nodeGlobalTransforms[nodeIndex]; }
    void updateNodeTransforms();

protected:
	void serialize2(Serializer2& ar) override;

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

} // namespace ln

