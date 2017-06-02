
#pragma once
#include <Lumino/Graphics/Common.h>
#include "../Graphics/Vertex.h"
#include "../Graphics/Material.h"

LN_NAMESPACE_BEGIN
class VertexDeclaration;
class MeshResource;
class StaticMeshModel;
using MeshResourcePtr = RefPtr<MeshResource>;
using StaticMeshModelPtr = RefPtr<StaticMeshModel>;

/** メッシュ生成オプション */
LN_ENUM_FLAGS(MeshCreationFlags)
{
	None					= 0x0000,
	DynamicBuffers			= 0x0001,	/**< 頂点バッファ及びインデックスバッファの作成に ResourceUsage::Dynamic を使用する。*/
	ReverseFaces			= 0x0002,	/**< 面方向を反転する。*/
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

/**
	@brief
*/
class MeshResource
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static MeshResourcePtr Create();

public:
	/** 名前を設定します。*/
	void SetName(const StringRef& name) { m_name = name; }

	/** 名前を取得します。*/
	const String& GetName() const { return m_name; }

	int GetVertexCount() const { return m_vertexUsedCount; }
	int GetIndexCount() const { return m_indexUsedCount; }
	int GetTriangleCount() const { return m_indexUsedCount / 3; }

	void SetPosition(int index, const Vector3& position);
	void SetNormal(int index, const Vector3& normal);
	void SetUV(int index, const Vector2& uv);
	void SetColor(int index, const Color& color);
	const Vector3& GetPosition(int index);


	void SetBlendWeight(int index, int blendIndex, float value);
	void SetBlendWeights(int index, float v0, float v1, float v2, float v3);
	void GetBlendWeights(int index, float* out0, float* out1, float* out2, float* out3);

	void SetBlendIndex(int index, int blendIndex, float value);
	void SetBlendIndices(int index, float v0, float v1, float v2, float v3);	// TODO: int
	void GetBlendIndices(int index, int* out0, int* out1, int* out2, int* out3);

	void SetIndex(int index, int vertexIndex);

	// PMX
	void SetAdditionalUV(int index, int additionalUVIndex, const Vector4& uv);
	void SetSdefC(int index, const Vector4& value);
	const Vector4& GetSdefC(int index);
	void SetSdefR0(int index, const Vector3& value);
	const Vector3& GetSdefR0(int index);
	void SetSdefR1(int index, const Vector3& value);
	const Vector3& GetSdefR1(int index);
	void SetEdgeWeight(int index, float weight);


	void AddMeshSection(const MeshAttribute& section);

	// TODO
	void AddSections(int count);
	MeshAttribute* GetSection(int index);

	/*----------*/

	void Clear();
	
	/**
		@brief
		@param[in]	v1	: 左上の頂点
		@param[in]	v2	: 左下の頂点
		@param[in]	v3	: 右下の頂点
		@param[in]	v4	: 右上の頂点
	*/
	void AddSquare(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4);
	void AddSquare(const Vertex* virtices);


	void AddLine(const Vertex& v1, const Vertex& v2);

	void AddPlane(const Vector2& size, int sliceH = 1, int sliceV = 1);	// TODO: name SquarePlane
	void AddBox(const Vector3& size);
	void AddSphere(float radius, int slices = 16, int stacks = 16);
	void AddTeapot(float size, int tessellation = 8);
	void AddScreenPlane();

	void ReverseFaces();

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
		RefPtr<VertexBuffer>	buffer = nullptr;
		void*					lockedBuffer = nullptr;
		bool					refresh = true;
	};
	
	struct IndexBufferInfo
	{
		IndexBufferFormat		format = IndexBufferFormat_UInt16;
		RefPtr<IndexBuffer>		buffer = nullptr;
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

	MeshResource();
	virtual ~MeshResource();
	void Initialize(detail::GraphicsManager* manager, MeshCreationFlags flags);
	void Reserve(int vertexCount, int indexCount);
	//void Resize(int vertexCount);

	// for PMX
	void ResizeVertexBuffer(int vertexCount);
	void ResizeIndexBuffer(int indexCount);
	void ResizeIndexBuffer(int indexCount, IndexBufferFormat format);

	void SetIndexInternal(void* indexBuffer, int vertexIndex, int value);

	int GetSubsetCount() const { return (m_attributes.IsEmpty()) ? 1 : m_attributes.GetCount(); }
	int GetIndexStride() const { return (m_indexBufferInfo.format == IndexBufferFormat_UInt16) ? 2 : 4; }

	void* TryLockVertexBuffer(VertexBufferType type);
	void* TryLockIndexBuffer();
	void TryGlowVertexBuffers(int requestVertexCount);
	void TryGlowIndexBuffer(int requestIndexCount);
	//void* RequestVertexBuffer(int vertexCount, VertexBufferType type);
	//void* RequestIndexBuffer(int indexCount);
	void* RequestVertexBufferForAdditional(int additionalVertexCount, VertexBufferType type);
	uint16_t* RequestIndexBufferForAdditional(int additionalIndexCount);

	void GetMeshAttribute(int subsetIndex, MeshAttribute* outAttr);
	void CommitRenderData(VertexDeclaration** outDecl, VertexBuffer** outVBs, int* outVBCount, IndexBuffer** outIB);


private:
	//void PostGenerated(Vertex* vb, void* ib, MeshCreationFlags flags);

LN_INTERNAL_ACCESS:	// TODO:
	detail::GraphicsManager*	m_manager;
	String						m_name;
	ResourceUsage				m_usage;

	int							m_vertexCapacity;
	int							m_vertexUsedCount;
	int							m_indexCapacity;
	int							m_indexUsedCount;
	RefPtr<VertexDeclaration>	m_vertexDeclaration;
	VertexBufferInfo			m_vertexBufferInfos[VB_Count];
	IndexBufferInfo				m_indexBufferInfo;

	//RefPtr<MaterialList>		m_materials;
	MeshAttributeList			m_attributes;
	bool						m_vertexDeclarationModified;
};


/**
	@brief
*/
class StaticMeshModel
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	//void SetMeshResource(MeshResource* meshResource) { m_meshResource = meshResource; }
	void AddMeshResource(MeshResource* meshResource) { m_meshResources.Add(meshResource); }
	MeshResource* GetMeshResource(int index) const { return m_meshResources[index]; }
	int GetMeshResourceCount() const { return m_meshResources.GetCount(); }

	MeshResource* FindMesh(const StringRef& name);

	//int GetSubsetCount() const;

	void AddMaterials(int count);
	void AddMaterial(Material* material);
	Material* GetMaterial(int index) const;

LN_INTERNAL_ACCESS:
	StaticMeshModel();
	virtual ~StaticMeshModel();
	void Initialize(detail::GraphicsManager* manager);
	void Initialize(detail::GraphicsManager* manager, MeshResource* sharingMesh);
	void InitializeBox(detail::GraphicsManager* manager, const Vector3& size, MeshCreationFlags flags);
	void InitializeSphere(detail::GraphicsManager* manager, float radius, int slices, int stacks, MeshCreationFlags flags);
	void InitializePlane(detail::GraphicsManager* manager, const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags);
	void InitializeScreenPlane(detail::GraphicsManager* manager, MeshCreationFlags flags);
	void InitializeTeapot(detail::GraphicsManager* manager, float size, int tessellation, MeshCreationFlags flags);
	
LN_INTERNAL_ACCESS:	// TODO:
	List<RefPtr<MeshResource>>	m_meshResources;
	RefPtr<MaterialList>		m_materials;
};

LN_NAMESPACE_END
