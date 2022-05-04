#pragma once
#include <LuminoGraphics/Common.hpp>
#include <LuminoGraphics/Rendering/Vertex.hpp>
#include "../Rendering/Common.hpp"

namespace ln {
class VertexLayout;
class VertexBuffer;
class IndexBuffer;
class Material;
class MeshContainer;
class MeshModel;
class MeshGeometryBuilder;
namespace detail {
class MeshManager;
class MeshImporter;
class MorphInstance;
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
class MeshPrimitive
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

	const AABB& boundingBox() const { return m_boundingBox; }

	/** セクションの情報を追加します。 */
	void addSection(int startIndex, int primitiveCount, int materialIndex, PrimitiveTopology topology);
	void setSection(int sectionIndex, int startIndex, int primitiveCount, int materialIndex, PrimitiveTopology topology);

	// TODO: internal
	void commitRenderData(int sectionIndex, detail::MorphInstance* morph, MeshSection2* outSection, VertexLayout** outDecl, std::array<VertexBuffer*, 16>* outVBs, int* outVBCount, IndexBuffer** outIB);
	void commitMorphTargets(detail::MorphInstance* morph, std::array<VertexBuffer*, MaxRenderMorphTargets>* outTargets);
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
	void calculateBoundingBox();
	bool isAllTriangleLists() const { return !m_sections.containsIf([](const MeshSection2& x) { return x.topology != PrimitiveTopology::TriangleList; }); }

	VertexBuffer* vertexBuffer(InterleavedVertexGroup group) const;
	IndexBuffer* indexBuffer() const;


	void* acquireMappedMorphVertexBuffer(int morphTargetIndex/*, VertexElementUsage usage*/);
	int morphTargetCount() const { return m_morphVertexBuffers.size(); }

LN_CONSTRUCT_ACCESS:
	MeshPrimitive();
	virtual ~MeshPrimitive();
	void init();
	void init(int vertexCount, int indexCount);
	void init(int vertexCount, int indexCount, IndexBufferFormat indexFormat, GraphicsResourceUsage resourceUsage);
	// TODO: section 1作る init ユーティリティも欲しい。addSection は本当に忘れやすい

private:
	void attemptResetVertexLayout();

	struct VertexBufferEntry
	{
		Ref<VertexBuffer> buffer;
		void* mappedBuffer = nullptr;	// TODO: 不要かも

		void reset();
	};

	struct IndexBufferEntry
	{
		Ref<IndexBuffer> buffer;
		void* mappedBuffer = nullptr;	// TODO: 不要かも

		void reset();
	};

	//struct MorphVertexBuffers
	//{
	//	VertexBufferEntry positionVertexBuffer;
	//	VertexBufferEntry normalVertexBuffer;
	//	VertexBufferEntry tangentVertexBuffer;
	//};

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
	//List<MorphVertexBuffers> m_morphVertexBuffers;
	List<VertexBufferEntry> m_morphVertexBuffers;
	List<VertexBufferAttribute> m_extraVertexBuffers;
	IndexBufferEntry m_indexBuffer;
    Ref<VertexLayout> m_vertexLayout;
	List<MeshSection2> m_sections;

	int m_vertexCount;
	int m_indexCount;
	IndexBufferFormat m_indexFormat;
	GraphicsResourceUsage m_resourceUsage;
	AABB m_boundingBox;



	friend class MeshGeometryBuilder;
	//friend class MeshContainer
};



// ひとつのメッシュモデルデータ内にいくつかのメッシュノードが含まれているとき、それを名前検索するために使用する。
// 例えば、フィールドのモデルに ビジュアル用のメッシュとコリジョン用のメッシュが含まれている場合、名前検索でコリジョンを取り出して Phyiscs モジュールに渡したりする。
// また、LOD の管理も行う。
// glTF などリソース効率のため、複数の Node から参照されることがある点に注意。
/** MeshContainer */
class MeshContainer
	: public Object
{
public:
	/** 名前を取得します。 */
	const ln::String& name() const { return m_name; }

	/** 名前を設定します。 */
	void setName(const StringView& name) { m_name = name; }

	///** メインの MeshResource を設定します。 */
	//void setMeshResource(MeshResource* mesh);

	///** メインの MeshResource を取得します。 */
	//MeshResource* meshResource() const;

 //   void addMeshResource(MeshResource* mesh);

	/** メッシュの境界ボックスを取得します。 */
	//const Box& bounds() const { return m_boundingBox; }

	// TODO: internal
	//MeshResource* selectLODResource(float distance) const;

	//void calculateBounds();


    //void setMesh(MeshPrimitive* mesh);
    //MeshPrimitive* mesh() const;
	void addMeshPrimitive(MeshPrimitive* mesh);
	const List<Ref<MeshPrimitive>>& meshPrimitives() const { return m_lodMesh; }
	MeshPrimitive* meshPrimitive() const { return m_lodMesh[0]; }

	// TODO: ↓ Node に持たせたので消しておく
	
	/** 可視状態を設定します。false の場合、このメッシュの描画は行われません。(default: true) */
	void setVisible(bool value) { m_visible = value; }

	/** 可視状態を取得します。*/
	bool isVisible() const { return m_visible; }

LN_CONSTRUCT_ACCESS:
	MeshContainer();
	virtual ~MeshContainer();

	/** MeshContainer を作成します。 */
	void init();

private:
	int m_index = -1;
	ln::String m_name;
	//Box m_boundingBox;
	//List<Ref<MeshResource>> m_lodResources; // TODO: :obsolete
    List<Ref<MeshPrimitive>> m_lodMesh;
	bool m_visible;
	AABB m_boundingBox;

	friend class MeshModel;
};

// Bone and Container
/** MeshNode */
LN_CLASS()
class MeshNode : public Object {
    LN_OBJECT;
public:
	int parentNodeIndex() const { return m_parent; }

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

    const List<int>& childrenIndices() const { return m_children; }

    void setMeshContainerIndex(int value);
    int meshContainerIndex() const { return m_meshContainerIndex; }
	bool hasMeshContainer() const { return m_meshContainerIndex >= 0; }

	int skeletonIndex = -1;
	bool m_boneNode = false;	// いずれかの Bone から参照されているか

    void addChildIndex(int value);
	const List<int>& children() const { return m_children; }

    void setInitialLocalTransform(const Matrix& value);
	void resetLocalTransform();

    const Matrix& initialLocalTransform() const { return m_initialLocalTransform; }
    
	const Matrix& globalMatrix() const;
	void setGlobalTransform(const Matrix& value);

	void updateGlobalTransform(bool hierarchical);

	

	// TODO: ↓ このあたりは dynamic data.
	
	/** 可視状態を設定します。false の場合、このメッシュの描画は行われません。(default: true) */
	LN_METHOD(Property)
	void setVisible(bool value) { m_visible = value; }

	/** 可視状態を取得します。*/
	LN_METHOD(Property)
	bool isVisible() const { return m_visible; }

LN_CONSTRUCT_ACCESS:
	MeshNode();
    virtual ~MeshNode() = default;

private:
	MeshModel* m_model;
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

	bool m_visible = true;

    friend class MeshModel;
};

namespace detail {

	// TODO: deprecated?
enum class InternalMeshModelType
{
    StaticMesh,
    SkinnedMesh,
};
}

} // namespace ln

