/*
	"Section" FStaticMeshSection
*/

#include "../Internal.h"
#include "../Graphics/Device/ShaderVariableBase.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Mesh/Mesh.h>
#include <Lumino/Graphics/Utils.h>
#include "../Graphics/GraphicsManager.h"
#include "MeshFactory.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// MeshResourceBuilder
//==============================================================================


//==============================================================================
// MeshResource
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(MeshResource, Object);

//------------------------------------------------------------------------------
MeshResourcePtr MeshResource::Create()
{
	auto ptr = MeshResourcePtr::MakeRef();
	ptr->Initialize(detail::GraphicsManager::GetInstance(), MeshCreationFlags::DynamicBuffers);
	return ptr;
}

//------------------------------------------------------------------------------
MeshResource::MeshResource()
	: m_manager(nullptr)
	, m_usage(ResourceUsage::Static)
	, m_vertexCapacity(0)
	, m_vertexUsedCount(0)
	, m_indexCapacity(0)
	, m_indexUsedCount(0)
	, m_vertexDeclaration()
	, m_attributes()
	, m_vertexDeclarationModified(false)
{
}

//------------------------------------------------------------------------------
MeshResource::~MeshResource()
{
}

//------------------------------------------------------------------------------
void MeshResource::Initialize(detail::GraphicsManager* manager, MeshCreationFlags flags)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;
	m_usage = (flags.TestFlag(MeshCreationFlags::DynamicBuffers)) ? ResourceUsage::Dynamic : ResourceUsage::Static;

}

//------------------------------------------------------------------------------
void MeshResource::Reserve(int vertexCount, int indexCount)
{
	TryGlowVertexBuffers(vertexCount);
	TryGlowIndexBuffer(indexCount);
}

////------------------------------------------------------------------------------
//void MeshResource::Resize(int vertexCount)
//{
//	ResizeVertexBuffer(vertexCount);
//}

//------------------------------------------------------------------------------
void MeshResource::ResizeVertexBuffer(int vertexCount)
{
	TryGlowVertexBuffers(vertexCount);
	m_vertexUsedCount = vertexCount;
}

//------------------------------------------------------------------------------
void MeshResource::ResizeIndexBuffer(int indexCount)
{
	ResizeIndexBuffer(indexCount, Utils::GetIndexBufferFormat(indexCount));
}

//------------------------------------------------------------------------------
void MeshResource::ResizeIndexBuffer(int indexCount, IndexBufferFormat format)
{
	TryGlowIndexBuffer(indexCount);
	m_indexUsedCount = indexCount;
	m_indexBufferInfo.format = format;
}

//------------------------------------------------------------------------------
void MeshResource::SetIndexInternal(void* indexBuffer, int index, int vertexIndex)
{
	if (m_indexBufferInfo.format == IndexBufferFormat_UInt16)
	{
		uint16_t* i = (uint16_t*)indexBuffer;
		i[index] = vertexIndex;
	}
	else if (m_indexBufferInfo.format == IndexBufferFormat_UInt32)
	{
		uint32_t* i = (uint32_t*)indexBuffer;
		i[index] = vertexIndex;
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}
}

//------------------------------------------------------------------------------
void MeshResource::SetPosition(int index, const Vector3& position)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	Vertex* v = (Vertex*)TryLockVertexBuffer(VB_BasicVertices);
	v[index].position = position;
}

//------------------------------------------------------------------------------
void MeshResource::SetNormal(int index, const Vector3& normal)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	Vertex* v = (Vertex*)TryLockVertexBuffer(VB_BasicVertices);
	v[index].normal = normal;
}

//------------------------------------------------------------------------------
void MeshResource::SetUV(int index, const Vector2& uv)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	Vertex* v = (Vertex*)TryLockVertexBuffer(VB_BasicVertices);
	v[index].uv = uv;
}

//------------------------------------------------------------------------------
void MeshResource::SetColor(int index, const Color& color)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	Vertex* v = (Vertex*)TryLockVertexBuffer(VB_BasicVertices);
	v[index].color = color;
}

//------------------------------------------------------------------------------
const Vector3& MeshResource::GetPosition(int index)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	Vertex* v = (Vertex*)TryLockVertexBuffer(VB_BasicVertices);
	return v[index].position;
}

//------------------------------------------------------------------------------
void MeshResource::SetBlendWeight(int index, int blendIndex, float value)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	BlendWeight* v = (BlendWeight*)TryLockVertexBuffer(VB_BlendWeights);
	v[index].weights[blendIndex] = value;
}

//------------------------------------------------------------------------------
void MeshResource::SetBlendWeights(int index, float v0, float v1, float v2, float v3)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	BlendWeight* v = (BlendWeight*)TryLockVertexBuffer(VB_BlendWeights);
	v[index].weights[0] = v0;
	v[index].weights[1] = v1;
	v[index].weights[2] = v2;
	v[index].weights[3] = v3;
}

//------------------------------------------------------------------------------
void MeshResource::GetBlendWeights(int index, float* out0, float* out1, float* out2, float* out3)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	BlendWeight* v = (BlendWeight*)TryLockVertexBuffer(VB_BlendWeights);
	if (out0 != nullptr) *out0 = v[index].weights[0];
	if (out1 != nullptr) *out1 = v[index].weights[1];
	if (out2 != nullptr) *out2 = v[index].weights[2];
	if (out3 != nullptr) *out3 = v[index].weights[3];
}

//------------------------------------------------------------------------------
void MeshResource::SetBlendIndex(int index, int blendIndex, float value)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	BlendWeight* v = (BlendWeight*)TryLockVertexBuffer(VB_BlendWeights);
	v[index].indices[blendIndex] = value;
}

//------------------------------------------------------------------------------
void MeshResource::SetBlendIndices(int index, float v0, float v1, float v2, float v3)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	BlendWeight* v = (BlendWeight*)TryLockVertexBuffer(VB_BlendWeights);
	v[index].indices[0] = v0;
	v[index].indices[1] = v1;
	v[index].indices[2] = v2;
	v[index].indices[3] = v3;
}

//------------------------------------------------------------------------------
void MeshResource::GetBlendIndices(int index, int* out0, int* out1, int* out2, int* out3)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	BlendWeight* v = (BlendWeight*)TryLockVertexBuffer(VB_BlendWeights);
	if (out0 != nullptr) *out0 = (int)v[index].indices[0];
	if (out1 != nullptr) *out1 = (int)v[index].indices[1];
	if (out2 != nullptr) *out2 = (int)v[index].indices[2];
	if (out3 != nullptr) *out3 = (int)v[index].indices[3];
}

//------------------------------------------------------------------------------
void MeshResource::SetIndex(int index, int vertexIndex)
{
	SetIndexInternal(TryLockIndexBuffer(), index, vertexIndex);
}

//------------------------------------------------------------------------------
void MeshResource::SetAdditionalUV(int index, int additionalUVIndex, const Vector4& uv)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	AdditionalUVs* v = (AdditionalUVs*)TryLockVertexBuffer(VB_AdditionalUVs);
	v[index].uv[additionalUVIndex] = uv;
}

//------------------------------------------------------------------------------
void MeshResource::SetSdefC(int index, const Vector4& value)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	SdefInfo* v = (SdefInfo*)TryLockVertexBuffer(VB_SdefInfo);
	v[index].sdefC = value;
}

//------------------------------------------------------------------------------
const Vector4& MeshResource::GetSdefC(int index)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	SdefInfo* v = (SdefInfo*)TryLockVertexBuffer(VB_SdefInfo);
	return v[index].sdefC;
}

//------------------------------------------------------------------------------
void MeshResource::SetSdefR0(int index, const Vector3& value)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	SdefInfo* v = (SdefInfo*)TryLockVertexBuffer(VB_SdefInfo);
	v[index].sdefR0 = value;
}

//------------------------------------------------------------------------------
const Vector3& MeshResource::GetSdefR0(int index)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	SdefInfo* v = (SdefInfo*)TryLockVertexBuffer(VB_SdefInfo);
	return v[index].sdefR0;
}

//------------------------------------------------------------------------------
void MeshResource::SetSdefR1(int index, const Vector3& value)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	SdefInfo* v = (SdefInfo*)TryLockVertexBuffer(VB_SdefInfo);
	v[index].sdefR1 = value;
}

//------------------------------------------------------------------------------
const Vector3& MeshResource::GetSdefR1(int index)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	SdefInfo* v = (SdefInfo*)TryLockVertexBuffer(VB_SdefInfo);
	return v[index].sdefR1;
}

//------------------------------------------------------------------------------
void MeshResource::SetEdgeWeight(int index, float weight)
{
	LN_CHECK_RANGE(index, 0, m_vertexUsedCount);
	MmdExtra* v = (MmdExtra*)TryLockVertexBuffer(VB_MmdExtra);
	v[index].edgeWeight = weight;
}

//------------------------------------------------------------------------------
void MeshResource::AddMeshSection(const MeshAttribute& section)
{
	m_attributes.Add(section);
}

//------------------------------------------------------------------------------
void MeshResource::AddSections(int count)
{
	m_attributes.Resize(m_attributes.GetCount() + count);
}

//------------------------------------------------------------------------------
MeshAttribute* MeshResource::GetSection(int index)
{
	return &m_attributes[index];
}

//------------------------------------------------------------------------------
void MeshResource::Clear()
{
	m_vertexUsedCount = 0;
	m_indexUsedCount = 0;
}

//------------------------------------------------------------------------------
void MeshResource::AddSquare(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4)
{
	int beginIndex = GetVertexCount();
	Vertex* v = (Vertex*)RequestVertexBufferForAdditional(4, VB_BasicVertices);
	v[0] = v1;
	v[1] = v2;
	v[2] = v3;
	v[3] = v4;

	uint16_t* i = RequestIndexBufferForAdditional(6);
	i[0] = beginIndex + 0;
	i[1] = beginIndex + 1;
	i[2] = beginIndex + 3;
	i[3] = beginIndex + 3;
	i[4] = beginIndex + 1;
	i[5] = beginIndex + 2;
}

//------------------------------------------------------------------------------
void MeshResource::AddSquare(const Vertex* virtices)
{
	AddSquare(virtices[0], virtices[1], virtices[2], virtices[3]);
}

//------------------------------------------------------------------------------
void MeshResource::AddLine(const Vertex& v1, const Vertex& v2)
{
	LN_FAIL_CHECK_STATE(!m_attributes.IsEmpty()) return;
	LN_FAIL_CHECK_STATE(m_attributes.GetLast().primitiveType == PrimitiveType_LineList) return;

	int beginIndex = GetVertexCount();
	Vertex* v = (Vertex*)RequestVertexBufferForAdditional(2, VB_BasicVertices);
	v[0] = v1;
	v[1] = v2;

	uint16_t* i = RequestIndexBufferForAdditional(2);
	i[0] = beginIndex + 0;
	i[1] = beginIndex + 1;

	m_attributes.GetLast().PrimitiveNum += 1;
}

//------------------------------------------------------------------------------
void MeshResource::AddPlane(const Vector2& size, int sliceH, int sliceV)
{
	int startIndex = GetVertexCount();
	LN_VERIFY_STATE(startIndex <= UINT16_MAX);

	// setup factory
	detail::PlaneMeshFactory3 factory;
	factory.Initialize(size, sliceH, sliceV, Color::White, Matrix::Identity);

	// alloc buffers, generate mesh
	Vertex* vb = (Vertex*)RequestVertexBufferForAdditional(factory.GetVertexCount(), VB_BasicVertices);
	uint16_t* ib = RequestIndexBufferForAdditional(factory.GetIndexCount());
	factory.Generate(vb, ib, (uint16_t)startIndex);
}

//------------------------------------------------------------------------------
void MeshResource::AddBox(const Vector3& size)
{
	int startIndex = GetVertexCount();
	LN_VERIFY_STATE(startIndex <= UINT16_MAX);

	// setup factory
	detail::RegularBoxMeshFactory factory;
	factory.Initialize(size, Color::White, Matrix::Identity);

	// alloc buffers, generate mesh
	Vertex* vb = (Vertex*)RequestVertexBufferForAdditional(factory.GetVertexCount(), VB_BasicVertices);
	uint16_t* ib = RequestIndexBufferForAdditional(factory.GetIndexCount());
	factory.Generate(vb, ib, (uint16_t)startIndex);
}

//------------------------------------------------------------------------------
void MeshResource::AddSphere(float radius, int slices, int stacks)
{
	int startIndex = GetVertexCount();
	LN_VERIFY_STATE(startIndex <= UINT16_MAX);

	// setup factory
	detail::RegularSphereMeshFactory factory;
	factory.Initialize(radius, slices, stacks, Color::White, Matrix::Identity);

	// alloc buffers, generate mesh
	Vertex* vb = (Vertex*)RequestVertexBufferForAdditional(factory.GetVertexCount(), VB_BasicVertices);
	uint16_t* ib = RequestIndexBufferForAdditional(factory.GetIndexCount());
	factory.Generate(vb, ib, (uint16_t)startIndex);
}

//------------------------------------------------------------------------------
void MeshResource::AddScreenPlane()
{
	int startIndex = GetVertexCount();
	LN_VERIFY_STATE(startIndex <= UINT16_MAX);

	// setup factory
	detail::PlaneMeshFactory factory(Vector2(2.0f, 2.0f));

	// alloc buffers, generate mesh
	Vertex* vb = (Vertex*)RequestVertexBufferForAdditional(factory.GetVertexCount(), VB_BasicVertices);
	uint16_t* ib = RequestIndexBufferForAdditional(factory.GetIndexCount());
	factory.Generate(vb, ib, (uint16_t)startIndex);
}

//------------------------------------------------------------------------------
void MeshResource::ReverseFaces()
{
	if (m_indexBufferInfo.buffer->GetIndexStride() == 2)
	{
		Vertex* vb = (Vertex*)TryLockVertexBuffer(VB_BasicVertices);
		uint16_t* ib = (uint16_t*)TryLockIndexBuffer();

		for (int i = 0; i < m_vertexUsedCount; ++i)
		{
			vb[i].normal *= -1.0f;
		}
		
		uint16_t* indices = (uint16_t*)ib;
		for (int i = 0; i < m_indexUsedCount; i += 3)
		{
			std::swap(indices[i + 1], indices[i + 2]);
		}
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}
}

//------------------------------------------------------------------------------
void MeshResource::AddTeapot(float size, int tessellation)
{
	int startIndex = GetVertexCount();
	LN_VERIFY_STATE(startIndex <= UINT16_MAX);

	// setup factory
	detail::TeapotMeshFactory factory;
	factory.Initialize(size, tessellation, Color::White, Matrix::Identity);

	// alloc buffers, generate mesh
	Vertex* vb = (Vertex*)RequestVertexBufferForAdditional(factory.GetVertexCount(), VB_BasicVertices);
	uint16_t* ib = RequestIndexBufferForAdditional(factory.GetIndexCount());
	factory.Generate(vb, ib, (uint16_t)startIndex);
}

//------------------------------------------------------------------------------
void* MeshResource::TryLockVertexBuffer(VertexBufferType type)
{
	LN_FAIL_CHECK_STATE(m_vertexUsedCount > 0) return nullptr;

	const size_t strideTable[VB_Count] =
	{
		sizeof(Vertex),			//VB_BasicVertices,
		sizeof(BlendWeight),	//VB_BlendWeights,
		sizeof(AdditionalUVs),	//VB_AdditionalUVs,
		sizeof(SdefInfo),		//VB_SdefInfo,
		sizeof(MmdExtra),		//VB_MmdExtra,
	};
	size_t requestedSize = strideTable[type] * m_vertexCapacity;

	//if (m_usage == ResourceUsage::Dynamic)
	{
		if (m_vertexBufferInfos[type].buffer != nullptr &&
			m_vertexBufferInfos[type].buffer->GetBufferSize() != requestedSize)
		{
			//// Unlock
			//if (m_vertexBufferInfos[type].lockedBuffer != nullptr)
			//{
			//	m_vertexBufferInfos[type].lockedBuffer = nullptr;
			//	m_vertexBufferInfos[type].buffer->Unlock();
			//}

			// Resize
			m_vertexBufferInfos[type].buffer->Resize(requestedSize);
		}
	}

	if (m_vertexBufferInfos[type].buffer == nullptr)
	{
		m_vertexBufferInfos[type].buffer = RefPtr<VertexBuffer>::MakeRef();
		m_vertexBufferInfos[type].buffer->Initialize(m_manager, requestedSize, nullptr, m_usage);
		m_vertexDeclarationModified = true;
	}

	//if (m_vertexBufferInfos[type].lockedBuffer == nullptr)
	{
		ByteBuffer* buf = m_vertexBufferInfos[type].buffer->GetMappedData();
		m_vertexBufferInfos[type].lockedBuffer = buf->GetData();
	}
	return m_vertexBufferInfos[type].lockedBuffer;
}

//------------------------------------------------------------------------------
void* MeshResource::TryLockIndexBuffer()
{
	LN_FAIL_CHECK_STATE(m_indexUsedCount > 0) return nullptr;

	//if (m_usage == ResourceUsage::Dynamic)
	{
		if (m_indexBufferInfo.buffer != nullptr &&
			(m_indexBufferInfo.buffer->GetIndexCount() != m_indexCapacity || m_indexBufferInfo.buffer->GetIndexFormat() != m_indexBufferInfo.format))
		{
			// Unlock
			//if (m_indexBufferInfo.lockedBuffer != nullptr)
			//{
			//	m_indexBufferInfo.lockedBuffer = nullptr;
			//	m_indexBufferInfo.buffer->Unlock();
			//}

			// Resize
			m_indexBufferInfo.buffer->Resize(m_indexCapacity, m_indexBufferInfo.format);
		}
	}

	if (m_indexBufferInfo.buffer == nullptr)
	{
		m_indexBufferInfo.buffer = RefPtr<IndexBuffer>::MakeRef();
		m_indexBufferInfo.buffer->Initialize(m_manager, m_indexCapacity, nullptr, m_indexBufferInfo.format, m_usage);
		m_indexBufferInfo.refresh = false;
	}
	//else if (m_indexBufferInfo.refresh)
	//{
	//	m_indexBufferInfo.buffer->Resize();
	//	m_indexBufferInfo.refresh = false;
	//}

	//if (m_indexBufferInfo.lockedBuffer == nullptr)
	{
		ByteBuffer* buf = m_indexBufferInfo.buffer->GetMappedData();
		m_indexBufferInfo.lockedBuffer = buf->GetData();
	}
	return m_indexBufferInfo.lockedBuffer;
}

//------------------------------------------------------------------------------
void MeshResource::TryGlowVertexBuffers(int requestVertexCount)
{
	if (m_vertexUsedCount + requestVertexCount > m_vertexCapacity)
	{
		m_vertexCapacity += std::max(m_vertexCapacity, requestVertexCount);
		for (int i = 0; i < VB_Count; ++i)
		{
			m_vertexBufferInfos[i].refresh = true;	// 次の TryLock で Resize してほしい
		}
	}
}

//------------------------------------------------------------------------------
void MeshResource::TryGlowIndexBuffer(int requestIndexCount)
{
	if (m_indexUsedCount + requestIndexCount > m_indexCapacity)
	{
		m_indexCapacity += std::max(m_indexCapacity, requestIndexCount);
		m_indexBufferInfo.refresh = true;	// 次の TryLock で Resize してほしい
	}
}

//------------------------------------------------------------------------------
void* MeshResource::RequestVertexBufferForAdditional(int additionalVertexCount, VertexBufferType type)
{
	int begin = GetVertexCount();
	int newCount = begin + additionalVertexCount;
	TryGlowVertexBuffers(newCount);
	m_vertexUsedCount = newCount;

	Vertex* vb = (Vertex*)TryLockVertexBuffer(type);
	return vb + begin;
}

//------------------------------------------------------------------------------
uint16_t* MeshResource::RequestIndexBufferForAdditional(int additionalIndexCount)
{
	int begin = GetIndexCount();
	int newCount = begin + additionalIndexCount;

	LN_FAIL_CHECK_STATE(m_indexBufferInfo.buffer == nullptr || m_indexBufferInfo.buffer->GetIndexStride() == 2) return nullptr;
	LN_FAIL_CHECK_STATE(newCount <= UINT16_MAX) return nullptr;

	TryGlowIndexBuffer(newCount);
	m_indexUsedCount = newCount;

	uint16_t* ib = (uint16_t*)TryLockIndexBuffer();
	return ib + begin;
}

//------------------------------------------------------------------------------
void MeshResource::GetMeshAttribute(int subsetIndex, MeshAttribute* outAttr)
{
	if (m_attributes.IsEmpty())
	{
		outAttr->MaterialIndex = 0;
		outAttr->StartIndex = 0;
		outAttr->PrimitiveNum = m_indexUsedCount / 3;	// triangle only
		outAttr->primitiveType = PrimitiveType_TriangleList;
	}
	else
	{
		*outAttr = m_attributes[subsetIndex];
	}
}

//------------------------------------------------------------------------------
void MeshResource::CommitRenderData(VertexDeclaration** outDecl, VertexBuffer** outVBs, int* outVBCount, IndexBuffer** outIB)
{
	LN_ASSERT(outDecl != nullptr);
	LN_ASSERT(outVBs != nullptr);
	LN_ASSERT(outVBCount != nullptr);
	LN_ASSERT(outIB != nullptr);

	// VertexDeclaration
	if (m_vertexDeclaration == nullptr || m_vertexDeclarationModified)
	{
		m_vertexDeclaration = RefPtr<VertexDeclaration>::MakeRef();
		m_vertexDeclaration->Initialize(m_manager);
		int stream = 0;

		// BasicVertices
		if (m_vertexBufferInfos[VB_BasicVertices].buffer != nullptr)
		{
			m_vertexDeclaration->AddVertexElement(stream, VertexElementType_Float3, VertexElementUsage_Position, 0);
			m_vertexDeclaration->AddVertexElement(stream, VertexElementType_Float2, VertexElementUsage_TexCoord, 0);
			m_vertexDeclaration->AddVertexElement(stream, VertexElementType_Float3, VertexElementUsage_Normal, 0);
			m_vertexDeclaration->AddVertexElement(stream, VertexElementType_Float4, VertexElementUsage_Color, 0);
			++stream;
		}

		// BlendWeights
		if (m_vertexBufferInfos[VB_BlendWeights].buffer != nullptr)
		{
			m_vertexDeclaration->AddVertexElement(stream, VertexElementType_Float4, VertexElementUsage_BlendWeight, 0);
			m_vertexDeclaration->AddVertexElement(stream, VertexElementType_Float4, VertexElementUsage_BlendIndices, 0);
			++stream;
		}

		// AdditionalUVs
		if (m_vertexBufferInfos[VB_AdditionalUVs].buffer != nullptr)
		{
			m_vertexDeclaration->AddVertexElement(stream, VertexElementType_Float4, VertexElementUsage_TexCoord, 1);
			m_vertexDeclaration->AddVertexElement(stream, VertexElementType_Float4, VertexElementUsage_TexCoord, 2);
			m_vertexDeclaration->AddVertexElement(stream, VertexElementType_Float4, VertexElementUsage_TexCoord, 3);
			m_vertexDeclaration->AddVertexElement(stream, VertexElementType_Float4, VertexElementUsage_TexCoord, 4);
			++stream;
		}

		// SdefInfo
		if (m_vertexBufferInfos[VB_SdefInfo].buffer != nullptr)
		{
			m_vertexDeclaration->AddVertexElement(stream, VertexElementType_Float4, VertexElementUsage_TexCoord, 5);
			m_vertexDeclaration->AddVertexElement(stream, VertexElementType_Float3, VertexElementUsage_TexCoord, 6);
			m_vertexDeclaration->AddVertexElement(stream, VertexElementType_Float3, VertexElementUsage_TexCoord, 7);
			++stream;
		}

		// MmdExtra
		if (m_vertexBufferInfos[VB_MmdExtra].buffer != nullptr)
		{
			m_vertexDeclaration->AddVertexElement(stream, VertexElementType_Float1, VertexElementUsage_TexCoord, 8);
			m_vertexDeclaration->AddVertexElement(stream, VertexElementType_Float1, VertexElementUsage_PointSize, 15);
			++stream;
		}
	}
	*outDecl = m_vertexDeclaration;

	// VertexBuffer
	int vbCount = 0;
	for (int i = 0; i < VB_Count; ++i)
	{
		//if (m_vertexBufferInfos[i].lockedBuffer != nullptr)
		//{
		//	m_vertexBufferInfos[i].buffer->Unlock();
			m_vertexBufferInfos[i].lockedBuffer = nullptr;
		//}

		if (m_vertexBufferInfos[i].buffer != nullptr)
		{
			outVBs[vbCount] = m_vertexBufferInfos[i].buffer;
			++vbCount;
		}
	}
	*outVBCount = vbCount;

	// IndexBuffer
	//if (m_indexBufferInfo.lockedBuffer != nullptr)
	//{
	//	m_indexBufferInfo.buffer->Unlock();
		m_indexBufferInfo.lockedBuffer = nullptr;
	//}
	*outIB = m_indexBufferInfo.buffer;
}

//------------------------------------------------------------------------------
//void MeshResource::PostGenerated(Vertex* vb, void* ib, MeshCreationFlags flags)
//{
//	for (int i = 0; i < m_vertexUsedCount; ++i)
//	{
//		vb[i].color = Color::White;
//	}
//
//	if (flags.TestFlag(MeshCreationFlags::ReverseFaces))
//	{
//		if (m_indexBufferInfo.buffer->GetIndexStride() == 2)
//		{
//			for (int i = 0; i < m_vertexUsedCount; ++i)
//			{
//				vb[i].normal *= -1.0f;
//			}
//
//			uint16_t* indices = (uint16_t*)ib;
//			for (int i = 0; i < m_indexUsedCount; i += 3)
//			{
//				std::swap(indices[i + 1], indices[i + 2]);
//			}
//		}
//		else
//		{
//			LN_NOTIMPLEMENTED();
//		}
//	}
//}

//==============================================================================
// StaticMeshModel
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(StaticMeshModel, Object);

//------------------------------------------------------------------------------
StaticMeshModel::StaticMeshModel()
	: m_materials()
{
}

//------------------------------------------------------------------------------
StaticMeshModel::~StaticMeshModel()
{
}

//------------------------------------------------------------------------------
void StaticMeshModel::Initialize(detail::GraphicsManager* manager)
{
	LN_VERIFY_ARG(manager != nullptr);
	m_materials = RefPtr<MaterialList>::MakeRef();
}

//------------------------------------------------------------------------------
void StaticMeshModel::Initialize(detail::GraphicsManager* manager, MeshResource* sharingMesh)
{
	LN_CHECK_ARG(manager != nullptr);
	LN_CHECK_ARG(sharingMesh != nullptr);

	// メッシュ(バッファ類)は共有する
	m_meshResource = sharingMesh;

	m_materials = RefPtr<MaterialList>::MakeRef();

	// マテリアルはコピーする
	// TODO: コピー有無のフラグがあったほうがいいかも？
	//int count = m_meshResource->m_materials->GetCount();
	//m_materials = RefPtr<MaterialList>::MakeRef();
	//m_materials->Resize(count);
	//for (int i = 0; i < count; ++i)
	//{
	//	m_materials->SetAt(i, m_meshResource->m_materials->GetAt(i)->CopyShared());
	//}
}

//------------------------------------------------------------------------------
void StaticMeshModel::InitializeBox(detail::GraphicsManager* manager, const Vector3& size, MeshCreationFlags flags)
{
	auto res = RefPtr<MeshResource>::MakeRef();
	res->Initialize(manager, flags);
	res->AddBox(size);
	if (flags.TestFlag(MeshCreationFlags::ReverseFaces)) res->ReverseFaces();
	Initialize(manager, res);
	AddMaterials(1);
}

//------------------------------------------------------------------------------
void StaticMeshModel::InitializeSphere(detail::GraphicsManager* manager, float radius, int slices, int stacks, MeshCreationFlags flags)
{
	auto res = RefPtr<MeshResource>::MakeRef();
	res->Initialize(manager, flags);
	res->AddSphere(radius, slices, stacks);
	if (flags.TestFlag(MeshCreationFlags::ReverseFaces)) res->ReverseFaces();
	Initialize(manager, res);
	AddMaterials(1);
}

//------------------------------------------------------------------------------
void StaticMeshModel::InitializePlane(detail::GraphicsManager* manager, const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags)
{
	auto res = RefPtr<MeshResource>::MakeRef();
	res->Initialize(manager, flags);
	res->AddPlane(size, sliceH, sliceV);
	if (flags.TestFlag(MeshCreationFlags::ReverseFaces)) res->ReverseFaces();
	Initialize(manager, res);
	AddMaterials(1);
}

//------------------------------------------------------------------------------
void StaticMeshModel::InitializeScreenPlane(detail::GraphicsManager* manager, MeshCreationFlags flags)
{
	auto res = RefPtr<MeshResource>::MakeRef();
	res->Initialize(manager, flags);
	res->AddScreenPlane();
	if (flags.TestFlag(MeshCreationFlags::ReverseFaces)) res->ReverseFaces();
	Initialize(manager, res);
	AddMaterials(1);
}

//------------------------------------------------------------------------------
void StaticMeshModel::InitializeTeapot(detail::GraphicsManager* manager, float size, int tessellation, MeshCreationFlags flags)
{
	auto res = RefPtr<MeshResource>::MakeRef();
	res->Initialize(manager, flags);
	res->AddTeapot(size, tessellation);
	if (flags.TestFlag(MeshCreationFlags::ReverseFaces)) res->ReverseFaces();
	Initialize(manager, res);
	AddMaterials(1);
}

//------------------------------------------------------------------------------
int StaticMeshModel::GetSubsetCount() const
{
	return m_meshResource->GetSubsetCount();
}

//------------------------------------------------------------------------------
void StaticMeshModel::AddMaterials(int count)
{
	int oldCount = m_materials->GetCount();
	int newCount = oldCount + count;
	m_materials->Resize(newCount);
	if (oldCount < newCount)
	{
		for (int i = oldCount; i < newCount; ++i)
		{
			auto m = RefPtr<Material>::MakeRef();
			m->Initialize();
			m_materials->SetAt(i, m);
		}
	}
}

//------------------------------------------------------------------------------
void StaticMeshModel::AddMaterial(Material* material)
{
	LN_VERIFY_STATE(m_materials != nullptr);
	m_materials->Add(material);
}

//------------------------------------------------------------------------------
Material* StaticMeshModel::GetMaterial(int index) const
{
	return m_materials->GetAt(index);
}

LN_NAMESPACE_END

