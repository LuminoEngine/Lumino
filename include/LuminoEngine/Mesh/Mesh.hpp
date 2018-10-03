#pragma once
#include "../Graphics/Common.hpp"
#include "../Rendering/Vertex.hpp"

namespace ln {
class VertexBuffer;
class IndexBuffer;
class AbstractMaterial;
class MeshContainer;

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

	/** 頂点情報を設定します。 */
	void setVertex(int index, const Vertex& value);

	/** インデックス値を設定します。 */
	void setIndex(int index, int value);

	/** セクションの情報を設定します。 */
	void setSection(int sectionIndex, int startIndex, int primitiveCount, int materialIndex);

LN_CONSTRUCT_ACCESS:
	MeshResource();
	virtual ~MeshResource();

	/** MeshResource を作成します。 */
	void initialize();

private:
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

	int m_vertexCount;
	int m_indexCount;
	GraphicsResourceUsage m_usage;
	std::array<Ref<VertexBuffer>, VBG_Count> m_vertexBuffers;
	Ref<IndexBuffer> m_indexBuffer;
	List<MeshSection> m_sections;

	friend class MeshContainer;
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

	/** メインの MeshResource を設定します。 */
	void setMeshResource(MeshResource* mesh);

	/** メインの MeshResource を取得します。 */
	MeshResource* meshResource() const;

	/** メッシュの境界ボックスを取得します。 */
	const Box& bounds() const { return m_boundingBox; }

	// TODO: internal
	MeshResource* selectLODResource(float distance) const;

	void calculateBounds();

LN_CONSTRUCT_ACCESS:
	MeshContainer();
	virtual ~MeshContainer();

	/** MeshContainer を作成します。 */
	void initialize();

private:
	ln::String m_name;
	Box m_boundingBox;
	List<Ref<MeshResource>> m_lodResources;
};

class MeshModel
	: public Object
{
public:
	void addMeshContainer(MeshContainer* meshContainer);
	void addMaterial(AbstractMaterial* material);

private:
	List<Ref<MeshContainer>> m_meshContainers;
	List<Ref<AbstractMaterial>> m_materials;
};

} // namespace ln

