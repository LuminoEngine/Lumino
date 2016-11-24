
#pragma once
#include <Lumino/Graphics/Common.h>
#include "Vertex.h"
#include "Material.h"

LN_NAMESPACE_BEGIN
class VertexDeclaration;
class StaticMeshModel;
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


	//void SetMaterialCount(int count);
	void AddMaterials(int count);
	Material* GetMaterial(int index) const;

	int GetVertexCount() const { return m_vertexCount; }
	int GetIndexCount() const { return m_indexCount; }

	void SetPosition(int index, const Vector3& position);
	void SetNormal(int index, const Vector3& normal);
	void SetUV(int index, const Vector2& uv);
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
		RefPtr<VertexBuffer>	buffer;
		void*					lockedBuffer;
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
	void Initialize(detail::GraphicsManager* manager);
	void Reserve(int vertexCount);
	void Resize(int vertexCount);

	// for PMX
	void ResizeVertexBuffer(int vertexCount);
	void ResizeIndexBuffer(int indexCount, IndexBufferFormat format);



	//void CreateVertexBuffer(int vertexCount);
	//void CreateIndexBuffer(int indexCount, IndexBufferFormat format);
	void BeginCreating(MeshCreationFlags flags);
	void EndCreating();

	void CreateBox(const Vector3& size);
	void CreateSphere(float radius, int slices, int stacks, MeshCreationFlags flags);
	void CreatePlane(const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags);
	void CreateSquarePlane(const Vector2& size, const Vector3& front, MeshCreationFlags flags);
	void CreateScreenPlane();

	int GetSubsetCount() const { return m_attributes.GetCount(); }

	void* TryLockVertexBuffer(VertexBufferType type, int stride);
	void* TryLockIndexBuffer();
	void CommitRenderData(VertexDeclaration** outDecl, VertexBuffer** outVBs, int* outVBCount, IndexBuffer** outIB);

private:
	void CreateBuffers(int vertexCount, int indexCount, MeshCreationFlags flags);
	void PostGenerated(Vertex* vb, void* ib, MeshCreationFlags flags);

LN_INTERNAL_ACCESS:	// TODO:
	detail::GraphicsManager*	m_manager;
	ResourceUsage				m_usage;
	int							m_vertexCount;
	int							m_indexCount;
	IndexBufferFormat			m_indexBufferFormat;

	RefPtr<VertexDeclaration>	m_vertexDeclaration;
	VertexBufferInfo			m_vertexBufferInfos[VB_Count];
	RefPtr<IndexBuffer>			m_indexBuffer;
	void*						m_lockedIndexBuffer;

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
