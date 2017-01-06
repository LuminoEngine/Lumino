
#pragma once
#include <Lumino/Graphics/Common.h>
#include "../Vertex.h"
#include "../Material.h"

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
	int		PrimitiveNum;   ///< 描画プリミティブ数 (三角形の数)
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

	//void SetMaterialCount(int count);
	// TODO: MeshResource としては Material は持たない・・ような気がする
	void AddMaterials(int count);
	Material* GetMaterial(int index) const;

	int GetVertexCount() const { return m_vertexUsedCount; }
	int GetIndexCount() const { return m_indexUsedCount; }

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
	void Initialize(detail::GraphicsManager* manager, ResourceUsage usage);
	void Reserve(int vertexCount, int indexCount);
	//void Resize(int vertexCount);

	// for PMX
	void ResizeVertexBuffer(int vertexCount);
	void ResizeIndexBuffer(int indexCount);
	void ResizeIndexBuffer(int indexCount, IndexBufferFormat format);

	void SetIndexInternal(void* indexBuffer, int vertexIndex, int value);

	void BeginCreating(MeshCreationFlags flags);
	void EndCreating();

	void CreateBox(const Vector3& size);
	void CreateSphere(float radius, int slices, int stacks, MeshCreationFlags flags);
	void CreatePlane(const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags);
	void CreateSquarePlane(const Vector2& size, const Vector3& front, MeshCreationFlags flags);
	void CreateScreenPlane();

	int GetSubsetCount() const { return m_attributes.GetCount(); }

	void* TryLockVertexBuffer(VertexBufferType type);
	void* TryLockIndexBuffer();
	void TryGlowVertexBuffers(int requestVertexCount);
	void TryGlowIndexBuffer(int requestIndexCount);

	void GetMeshAttribute(int subsetIndex, MeshAttribute* outAttr);
	void CommitRenderData(VertexDeclaration** outDecl, VertexBuffer** outVBs, int* outVBCount, IndexBuffer** outIB);


private:
	void CreateBuffers(int vertexCount, int indexCount, MeshCreationFlags flags);
	void PostGenerated(Vertex* vb, void* ib, MeshCreationFlags flags);

LN_INTERNAL_ACCESS:	// TODO:
	detail::GraphicsManager*	m_manager;
	ResourceUsage				m_usage;

	int							m_vertexCapacity;
	int							m_vertexUsedCount;
	int							m_indexCapacity;
	int							m_indexUsedCount;
	RefPtr<VertexDeclaration>	m_vertexDeclaration;
	VertexBufferInfo			m_vertexBufferInfos[VB_Count];
	IndexBufferInfo				m_indexBufferInfo;

	RefPtr<MaterialList>		m_materials;
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

	MeshResource* GetMeshResource() const { return m_meshResource; }

	int GetSubsetCount() const;
	//Material* GetMaterial(int index) const;

	//void SetPosition(int index, const Vector3& position);
	//void SetUV(int index, const Vector2& uv);
	//const Vector3& GetPosition(int index);

LN_INTERNAL_ACCESS:
	StaticMeshModel();
	virtual ~StaticMeshModel();
	void Initialize(detail::GraphicsManager* manager, MeshResource* sharingMesh);
	void InitializeBox(detail::GraphicsManager* manager, const Vector3& size);
	void InitializeSphere(detail::GraphicsManager* manager, float radius, int slices, int stacks, MeshCreationFlags flags);
	void InitializePlane(detail::GraphicsManager* manager, const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags);
	void InitializeSquarePlane(detail::GraphicsManager* manager, const Vector2& size, const Vector3& front, MeshCreationFlags flags);
	void InitializeScreenPlane(detail::GraphicsManager* manager);

LN_INTERNAL_ACCESS:	// TODO:
	RefPtr<MeshResource>	m_meshResource;
	RefPtr<MaterialList>	m_materials;
};

LN_NAMESPACE_END
