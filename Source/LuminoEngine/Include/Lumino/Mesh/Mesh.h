
#pragma once
#include <Lumino/Graphics/Common.h>
#include "../Graphics/Vertex.h"
#include "../Graphics/Material.h"

LN_NAMESPACE_BEGIN
class VertexDeclaration;
class MeshResource;
class StaticMeshModel;
using MeshResourcePtr = Ref<MeshResource>;
using StaticMeshModelPtr = Ref<StaticMeshModel>;

/** メッシュ生成オプション */
LN_ENUM_FLAGS(MeshCreationFlags)
{
	None					= 0x0000,
	DynamicBuffers			= 0x0001,	/**< 頂点バッファ及びインデックスバッファの作成に ResourceUsage::Dynamic を使用する。*/
	reverseFaces			= 0x0002,	/**< 面方向を反転する。*/
};
LN_ENUM_FLAGS_DECLARE(MeshCreationFlags);

/// メッシュの属性
struct MeshAttribute	// TODO: Section
{
	int		MaterialIndex;  ///< 対応するマテリアル番号
	int		StartIndex;     ///< 開始インデックス
	int		PrimitiveNum;   ///< 描画プリミティブ数
	PrimitiveType	primitiveType = PrimitiveType_TriangleList;
};
typedef List<MeshAttribute>		MeshAttributeList;

enum class UnitMeshSide
{
	Outward,
	Inward,
};

/**
	@brief
*/
class MeshResource
	: public Object
{
	LN_OBJECT;
public:

	/** 頂点数とインデックス数を指定してメッシュを作成します。 */
	static Ref<MeshResource> create(int vertexCount, int indexCount, MeshCreationFlags flags = MeshCreationFlags::None);

	static MeshResourcePtr create();

	static Ref<MeshResource> getUnitSphere(UnitMeshSide side = UnitMeshSide::Outward);

public:
	/** 名前を設定します。*/
	void setName(const StringRef& name) { m_name = name; }

	/** 名前を取得します。*/
	const String& getName() const { return m_name; }

	int getVertexCount() const;
	int getIndexCount() const;
	int getTriangleCount() const;

	void setPosition(int index, const Vector3& position);
	void setNormal(int index, const Vector3& normal);
	void setUV(int index, const Vector2& uv);
	void setColor(int index, const Color& color);
	const Vector3& getPosition(int index);


	void setBlendWeight(int index, int blendIndex, float value);
	void setBlendWeights(int index, float v0, float v1, float v2, float v3);
	void getBlendWeights(int index, float* out0, float* out1, float* out2, float* out3);

	void setBlendIndex(int index, int blendIndex, float value);
	void setBlendIndices(int index, float v0, float v1, float v2, float v3);	// TODO: int
	void getBlendIndices(int index, int* out0, int* out1, int* out2, int* out3);

	void setIndex(int index, int vertexIndex);

	// PMX
	void setAdditionalUV(int index, int additionalUVIndex, const Vector4& uv);
	void setSdefC(int index, const Vector4& value);
	const Vector4& getSdefC(int index);
	void setSdefR0(int index, const Vector3& value);
	const Vector3& getSdefR0(int index);
	void setSdefR1(int index, const Vector3& value);
	const Vector3& getSdefR1(int index);
	void setEdgeWeight(int index, float weight);


	void addMeshSection(const MeshAttribute& section);

	// TODO
	void addSections(int count);
	MeshAttribute* getSection(int index);

	/*----------*/

	void clear();
	
	/**
		@brief
		@param[in]	v1	: 左上の頂点
		@param[in]	v2	: 左下の頂点
		@param[in]	v3	: 右下の頂点
		@param[in]	v4	: 右上の頂点
	*/
	void addSquare(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4);
	void addSquare(const Vertex* virtices);

	// TODO: obsolete, triangle only
	void addLine(const Vertex& v1, const Vertex& v2);

	void addPlane(const Vector2& size, int sliceH = 1, int sliceV = 1);	// TODO: name SquarePlane
	void addBox(const Vector3& size, const Color& color = Color::White, const Matrix& transform = Matrix::Identity);
	void addSphere(float radius, int slices = 16, int stacks = 16, const Matrix& transform = Matrix::Identity);
	void addTeapot(float size, int tessellation = 8);
	void addScreenPlane();

	void reverseFaces();

	IndexBuffer* getIndexBuffer() const;
	const Box& getBoundingBox() const { return m_boundingBox; }

LN_INTERNAL_ACCESS:
	enum VertexBufferType
	{
		VB_BasicVertices = 0,
		VB_BlendWeights,
		VB_AdditionalUVs,
		VB_SdefInfo,
		VB_MmdExtra,
		VB_Count,
	};

	struct VertexBufferInfo
	{
		Ref<VertexBuffer>	buffer = nullptr;
		void*					lockedBuffer = nullptr;
		bool					refresh = true;
	};
	
	struct IndexBufferInfo
	{
		//IndexBufferFormat		format = IndexBufferFormat_UInt16;
		Ref<IndexBuffer>		buffer = nullptr;
		void*					lockedBuffer = nullptr;
		bool					refresh = true;
	};

	struct BlendWeight
	{
		float		weights[4];
		float		indices[4];
	};

	struct AdditionalUVs
	{
		Vector4		uv[4];
	};

	struct SdefInfo
	{
		Vector4		sdefC;
		Vector3		sdefR0;
		Vector3		sdefR1;
	};

	struct MmdExtra
	{
		float		edgeWeight;
		float		index;
	};

	static const size_t vertexStrideTable[VB_Count];

	MeshResource();
	virtual ~MeshResource();
	void initialize(detail::GraphicsManager* manager, MeshCreationFlags flags);
	void reserve(int vertexCount, int indexCount);
	//void Resize(int vertexCount);



	// for PMX
	void resizeVertexBuffer(int vertexCount);
	void resizeIndexBuffer(int indexCount);
	//void resizeIndexBuffer(int indexCount, IndexBufferFormat format);

	//void SetIndexInternal(void* indexBuffer, int vertexIndex, int value);

	int getSubsetCount() const { return (m_attributes.isEmpty()) ? 1 : m_attributes.getCount(); }
	//int getIndexStride() const { return (m_indexBufferInfo.format == IndexBufferFormat_UInt16) ? 2 : 4; }

	//void* TryLockVertexBuffer(VertexBufferType type);
	//void* TryLockIndexBuffer();
	//void TryGlowVertexBuffers(int requestVertexCount);
	//void TryGlowIndexBuffer(int requestIndexCount);
	//void* requestVertexBuffer(int vertexCount, VertexBufferType type);
	//void* requestIndexBuffer(int indexCount);
	void* requestVertexBufferForAdditional(int additionalVertexCount, VertexBufferType type);
	uint16_t* requestIndexBufferForAdditional(int additionalIndexCount);

	VertexBuffer* getVertexBuffer(VertexBufferType type) const;
	VertexBuffer* requestVertexBuffer(VertexBufferType type);
	IndexBuffer* requestIndexBuffer();

	void getMeshAttribute(int subsetIndex, MeshAttribute* outAttr);
	void commitRenderData(VertexDeclaration** outDecl, VertexBuffer** outVBs, int* outVBCount, IndexBuffer** outIB);


private:
	//void PostGenerated(Vertex* vb, void* ib, MeshCreationFlags flags);

LN_INTERNAL_ACCESS:	// TODO:
	void setBoundingBox(const Box& box);

	detail::GraphicsManager*	m_manager;
	String						m_name;
	ResourceUsage				m_usage;

	//int							m_vertexCapacity;
	//int							m_vertexUsedCount;
	//int							m_indexCapacity;
	//int							m_indexUsedCount;
	Ref<VertexDeclaration>	m_vertexDeclaration;
	VertexBufferInfo			m_vertexBufferInfos[VB_Count];
	IndexBufferInfo				m_indexBufferInfo;

	//Ref<MaterialList>		m_materials;
	MeshAttributeList			m_attributes;
	bool						m_vertexDeclarationModified;

	Box                            m_boundingBox;
};


/**
	@brief
*/
class StaticMeshModel
	: public Object
{
	LN_OBJECT;
public:

	//void SetMeshResource(MeshResource* meshResource) { m_meshResource = meshResource; }
	void addMeshResource(MeshResource* meshResource) { m_meshResources.add(meshResource); }
	MeshResource* getMeshResource(int index) const { return m_meshResources[index]; }
	int getMeshResourceCount() const { return m_meshResources.getCount(); }

	MeshResource* FindMesh(const StringRef& name);

	//int getSubsetCount() const;

	void addMaterials(int count);
	void addMaterial(CommonMaterial* material);
	CommonMaterial* getMaterial(int index) const;

LN_CONSTRUCT_ACCESS:
public:	// TODO
	StaticMeshModel();
	virtual ~StaticMeshModel();
	void initialize(detail::GraphicsManager* manager);
	void initialize(MeshResource* sharingMesh);
	void initializeBox(const Vector3& size, MeshCreationFlags flags);
	void initializeSphere(float radius, int slices, int stacks, MeshCreationFlags flags);
	void initializePlane(const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags);
	void initializeScreenPlane(MeshCreationFlags flags);
	void initializeTeapot(float size, int tessellation, MeshCreationFlags flags);
	
LN_INTERNAL_ACCESS:	// TODO:
	List<Ref<MeshResource>>	m_meshResources;
	Ref<MaterialList>		m_materials;
};

LN_NAMESPACE_END
