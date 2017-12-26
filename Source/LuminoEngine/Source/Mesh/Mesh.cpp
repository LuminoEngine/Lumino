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
#include "ModelManager.h"
#include "MeshFactory.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// MeshResourceBuilder
//==============================================================================


//==============================================================================
// MeshResource
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(MeshResource, Object);

const size_t MeshResource::vertexStrideTable[VB_Count] =
{
	sizeof(Vertex),			//VB_BasicVertices,
	sizeof(BlendWeight),	//VB_BlendWeights,
	sizeof(AdditionalUVs),	//VB_AdditionalUVs,
	sizeof(SdefInfo),		//VB_SdefInfo,
	sizeof(MmdExtra),		//VB_MmdExtra,
};

Ref<MeshResource> MeshResource::create(int vertexCount, int indexCount, MeshCreationFlags flags)
{
	auto ptr = Ref<MeshResource>::makeRef();
	ptr->initialize(detail::GraphicsManager::getInstance(), flags);
	ptr->resizeVertexBuffer(vertexCount);
	ptr->resizeIndexBuffer(indexCount);
	return ptr;
}

//------------------------------------------------------------------------------
MeshResourcePtr MeshResource::create()
{
	auto ptr = MeshResourcePtr::makeRef();
	ptr->initialize(detail::GraphicsManager::getInstance(), MeshCreationFlags::DynamicBuffers);
	return ptr;
}

//------------------------------------------------------------------------------
Ref<MeshResource> MeshResource::getUnitSphere(UnitMeshSide side)
{
	return detail::ModelManager::getInstance()->getUnitSphereMeshResource(side == UnitMeshSide::Inward);
}

//------------------------------------------------------------------------------
MeshResource::MeshResource()
	: m_manager(nullptr)
	, m_usage(ResourceUsage::Static)
	//, m_vertexCapacity(0)
	//, m_vertexUsedCount(0)
	//, m_indexCapacity(0)
	//, m_indexUsedCount(0)
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
void MeshResource::initialize(detail::GraphicsManager* manager, MeshCreationFlags flags)
{
	if (LN_REQUIRE(manager != nullptr)) return;
	m_manager = manager;
	m_usage = (flags.TestFlag(MeshCreationFlags::DynamicBuffers)) ? ResourceUsage::Dynamic : ResourceUsage::Static;

}

//------------------------------------------------------------------------------
void MeshResource::reserve(int vertexCount, int indexCount)
{
	requestVertexBuffer(VB_BasicVertices)->reserve(vertexCount);	// まずは VB_BasicVertices だけ。他は必要になったとき、これのサイズに合わせて確保される。
	requestIndexBuffer()->reserve(indexCount);
}

////------------------------------------------------------------------------------
//void MeshResource::Resize(int vertexCount)
//{
//	resizeVertexBuffer(vertexCount);
//}

//------------------------------------------------------------------------------
void MeshResource::resizeVertexBuffer(int vertexCount)
{
	requestVertexBuffer(VB_BasicVertices);

	for (auto& info : m_vertexBufferInfos)
	{
		if (info.buffer != nullptr)
		{
			info.buffer->resize(vertexCount * vertexStrideTable[VB_BasicVertices]);
		}
	}
}

//------------------------------------------------------------------------------
void MeshResource::resizeIndexBuffer(int indexCount)
{
	requestIndexBuffer()->resize(indexCount);
}

//------------------------------------------------------------------------------
//void MeshResource::resizeIndexBuffer(int indexCount, IndexBufferFormat format)
//{
//	TryGlowIndexBuffer(indexCount);
//	m_indexUsedCount = indexCount;
//}

//------------------------------------------------------------------------------
int MeshResource::getVertexCount() const
{
	if (getVertexBuffer(VB_BasicVertices) != nullptr)
	{
		return getVertexBuffer(VB_BasicVertices)->getSize() / vertexStrideTable[VB_BasicVertices];
	}
	return 0;
}

//------------------------------------------------------------------------------
int MeshResource::getIndexCount() const
{
	// TODO: has check
	return getIndexBuffer()->getIndexCount();
}

//------------------------------------------------------------------------------
int MeshResource::getTriangleCount() const	// TODO: face count
{
	return getIndexCount() / 3;
}

//------------------------------------------------------------------------------
void MeshResource::setPosition(int index, const Vector3& position)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return;
	Vertex* v = (Vertex*)requestVertexBuffer(VB_BasicVertices)->getMappedData();
	v[index].position = position;
}

//------------------------------------------------------------------------------
void MeshResource::setNormal(int index, const Vector3& normal)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return;
	Vertex* v = (Vertex*)requestVertexBuffer(VB_BasicVertices)->getMappedData();
	v[index].normal = normal;
}

//------------------------------------------------------------------------------
void MeshResource::setUV(int index, const Vector2& uv)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return;
	Vertex* v = (Vertex*)requestVertexBuffer(VB_BasicVertices)->getMappedData();
	v[index].uv = uv;
}

//------------------------------------------------------------------------------
void MeshResource::setColor(int index, const Color& color)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return;
	Vertex* v = (Vertex*)requestVertexBuffer(VB_BasicVertices)->getMappedData();
	v[index].color = color;
}

//------------------------------------------------------------------------------
const Vector3& MeshResource::getPosition(int index)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return Vector3::Zero;
	Vertex* v = (Vertex*)requestVertexBuffer(VB_BasicVertices)->getMappedData();
	return v[index].position;
}

//------------------------------------------------------------------------------
void MeshResource::setBlendWeight(int index, int blendIndex, float value)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return;
	BlendWeight* v = (BlendWeight*)requestVertexBuffer(VB_BlendWeights)->getMappedData();
	v[index].weights[blendIndex] = value;
}

//------------------------------------------------------------------------------
void MeshResource::setBlendWeights(int index, float v0, float v1, float v2, float v3)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return;
	BlendWeight* v = (BlendWeight*)requestVertexBuffer(VB_BlendWeights)->getMappedData();
	v[index].weights[0] = v0;
	v[index].weights[1] = v1;
	v[index].weights[2] = v2;
	v[index].weights[3] = v3;
}

//------------------------------------------------------------------------------
void MeshResource::getBlendWeights(int index, float* out0, float* out1, float* out2, float* out3)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return;
	BlendWeight* v = (BlendWeight*)requestVertexBuffer(VB_BlendWeights)->getMappedData();
	if (out0 != nullptr) *out0 = v[index].weights[0];
	if (out1 != nullptr) *out1 = v[index].weights[1];
	if (out2 != nullptr) *out2 = v[index].weights[2];
	if (out3 != nullptr) *out3 = v[index].weights[3];
}

//------------------------------------------------------------------------------
void MeshResource::setBlendIndex(int index, int blendIndex, float value)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return;
	BlendWeight* v = (BlendWeight*)requestVertexBuffer(VB_BlendWeights)->getMappedData();
	v[index].indices[blendIndex] = value;
}

//------------------------------------------------------------------------------
void MeshResource::setBlendIndices(int index, float v0, float v1, float v2, float v3)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return;
	BlendWeight* v = (BlendWeight*)requestVertexBuffer(VB_BlendWeights)->getMappedData();
	v[index].indices[0] = v0;
	v[index].indices[1] = v1;
	v[index].indices[2] = v2;
	v[index].indices[3] = v3;
}

//------------------------------------------------------------------------------
void MeshResource::getBlendIndices(int index, int* out0, int* out1, int* out2, int* out3)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return;
	BlendWeight* v = (BlendWeight*)requestVertexBuffer(VB_BlendWeights)->getMappedData();
	if (out0 != nullptr) *out0 = (int)v[index].indices[0];
	if (out1 != nullptr) *out1 = (int)v[index].indices[1];
	if (out2 != nullptr) *out2 = (int)v[index].indices[2];
	if (out3 != nullptr) *out3 = (int)v[index].indices[3];
}

//------------------------------------------------------------------------------
void MeshResource::setIndex(int index, int vertexIndex)
{
	m_indexBufferInfo.buffer->setIndex(index, vertexIndex);
}

//------------------------------------------------------------------------------
void MeshResource::setAdditionalUV(int index, int additionalUVIndex, const Vector4& uv)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return;
	AdditionalUVs* v = (AdditionalUVs*)requestVertexBuffer(VB_AdditionalUVs)->getMappedData();
	v[index].uv[additionalUVIndex] = uv;
}

//------------------------------------------------------------------------------
void MeshResource::setSdefC(int index, const Vector4& value)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return;
	SdefInfo* v = (SdefInfo*)requestVertexBuffer(VB_SdefInfo)->getMappedData();
	v[index].sdefC = value;
}

//------------------------------------------------------------------------------
const Vector4& MeshResource::getSdefC(int index)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return Vector4::Zero;
	SdefInfo* v = (SdefInfo*)requestVertexBuffer(VB_SdefInfo)->getMappedData();
	return v[index].sdefC;
}

//------------------------------------------------------------------------------
void MeshResource::setSdefR0(int index, const Vector3& value)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return;
	SdefInfo* v = (SdefInfo*)requestVertexBuffer(VB_SdefInfo)->getMappedData();
	v[index].sdefR0 = value;
}

//------------------------------------------------------------------------------
const Vector3& MeshResource::getSdefR0(int index)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return Vector3::Zero;
	SdefInfo* v = (SdefInfo*)requestVertexBuffer(VB_SdefInfo)->getMappedData();
	return v[index].sdefR0;
}

//------------------------------------------------------------------------------
void MeshResource::setSdefR1(int index, const Vector3& value)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return;
	SdefInfo* v = (SdefInfo*)requestVertexBuffer(VB_SdefInfo)->getMappedData();
	v[index].sdefR1 = value;
}

//------------------------------------------------------------------------------
const Vector3& MeshResource::getSdefR1(int index)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return Vector3::Zero;
	SdefInfo* v = (SdefInfo*)requestVertexBuffer(VB_SdefInfo)->getMappedData();
	return v[index].sdefR1;
}

//------------------------------------------------------------------------------
void MeshResource::setEdgeWeight(int index, float weight)
{
	if (LN_REQUIRE_RANGE(index, 0, getVertexCount())) return;
	MmdExtra* v = (MmdExtra*)requestVertexBuffer(VB_MmdExtra)->getMappedData();
	v[index].edgeWeight = weight;
}

//------------------------------------------------------------------------------
void MeshResource::addMeshSection(const MeshAttribute& section)
{
	m_attributes.add(section);
}

//------------------------------------------------------------------------------
void MeshResource::addSections(int count)
{
	m_attributes.resize(m_attributes.getCount() + count);
}

//------------------------------------------------------------------------------
MeshAttribute* MeshResource::getSection(int index)
{
	return &m_attributes[index];
}

//------------------------------------------------------------------------------
void MeshResource::clear()
{
	for (auto& info : m_vertexBufferInfos)
	{
		if (info.buffer != nullptr)
		{
			info.buffer->clear();
		}
	}
	if (getIndexBuffer() != nullptr) getIndexBuffer()->clear();
}

//------------------------------------------------------------------------------
void MeshResource::addSquare(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4)
{
	int beginIndex = getVertexCount();
	Vertex* v = (Vertex*)requestVertexBufferForAdditional(4, VB_BasicVertices);
	v[0] = v1;
	v[1] = v2;
	v[2] = v3;
	v[3] = v4;

	uint16_t* i = requestIndexBufferForAdditional(6);
	i[0] = beginIndex + 0;
	i[1] = beginIndex + 1;
	i[2] = beginIndex + 3;
	i[3] = beginIndex + 3;
	i[4] = beginIndex + 1;
	i[5] = beginIndex + 2;
}

//------------------------------------------------------------------------------
void MeshResource::addSquare(const Vertex* virtices)
{
	addSquare(virtices[0], virtices[1], virtices[2], virtices[3]);
}

//------------------------------------------------------------------------------
void MeshResource::addLine(const Vertex& v1, const Vertex& v2)
{
	if (LN_REQUIRE(!m_attributes.isEmpty())) return;
	if (LN_REQUIRE(m_attributes.getLast().primitiveType == PrimitiveType_LineList)) return;

	int beginIndex = getVertexCount();
	Vertex* v = (Vertex*)requestVertexBufferForAdditional(2, VB_BasicVertices);
	v[0] = v1;
	v[1] = v2;

	uint16_t* i = requestIndexBufferForAdditional(2);
	i[0] = beginIndex + 0;
	i[1] = beginIndex + 1;

	m_attributes.getLast().PrimitiveNum += 1;
}

//------------------------------------------------------------------------------
void MeshResource::addPlane(const Vector2& size, int sliceH, int sliceV)
{
	int startIndex = getVertexCount();
	if (LN_ENSURE(startIndex <= UINT16_MAX)) return;

	// setup factory
	detail::PlaneMeshFactory3 factory;
	factory.initialize(size, sliceH, sliceV, Color::White, Matrix::Identity);

	// alloc buffers, generate mesh
	Vertex* vb = (Vertex*)requestVertexBufferForAdditional(factory.getVertexCount(), VB_BasicVertices);
	uint16_t* ib = requestIndexBufferForAdditional(factory.getIndexCount());
	factory.setBuffers(vb, ib, IndexBufferFormat_UInt16, (uint16_t)startIndex);
	factory.generate();
}

//------------------------------------------------------------------------------
void MeshResource::addBox(const Vector3& size, const Color& color, const Matrix& transform)
{
	int startIndex = getVertexCount();
	if (LN_ENSURE(startIndex <= UINT16_MAX)) return;

	// setup factory
	detail::RegularBoxMeshFactory factory;
	factory.initialize(size, color, transform);

	// alloc buffers, generate mesh
	Vertex* vb = (Vertex*)requestVertexBufferForAdditional(factory.getVertexCount(), VB_BasicVertices);
	uint16_t* ib = requestIndexBufferForAdditional(factory.getIndexCount());
	factory.setBuffers(vb, ib, IndexBufferFormat_UInt16, (uint16_t)startIndex);
	factory.generate();
}

//------------------------------------------------------------------------------
void MeshResource::addSphere(float radius, int slices, int stacks, const Matrix& transform)
{
	int startIndex = getVertexCount();
	if (LN_ENSURE(startIndex <= UINT16_MAX)) return;

	// setup factory
	detail::RegularSphereMeshFactory factory;
	factory.initialize(radius, slices, stacks, Color::White, transform);

	// alloc buffers, generate mesh
	Vertex* vb = (Vertex*)requestVertexBufferForAdditional(factory.getVertexCount(), VB_BasicVertices);
	uint16_t* ib = requestIndexBufferForAdditional(factory.getIndexCount());
	factory.setBuffers(vb, ib, IndexBufferFormat_UInt16, (uint16_t)startIndex);
	factory.generate();
}

//------------------------------------------------------------------------------
void MeshResource::addScreenPlane()
{
	int startIndex = getVertexCount();
	if (LN_ENSURE(startIndex <= UINT16_MAX)) return;

	// setup factory
	detail::PlaneMeshFactory factory(Vector2(2.0f, 2.0f));

	// alloc buffers, generate mesh
	Vertex* vb = (Vertex*)requestVertexBufferForAdditional(factory.getVertexCount(), VB_BasicVertices);
	uint16_t* ib = requestIndexBufferForAdditional(factory.getIndexCount());
	factory.setBuffers(vb, ib, IndexBufferFormat_UInt16, (uint16_t)startIndex);
	factory.generate();
}

//------------------------------------------------------------------------------
void MeshResource::reverseFaces()
{
	if (m_indexBufferInfo.buffer->getIndexStride() == 2)
	{
		Vertex* vb = (Vertex*)getVertexBuffer(VB_BasicVertices)->getMappedData();
		uint16_t* ib = (uint16_t*)m_indexBufferInfo.buffer->getMappedData();

		int vertexCount = getVertexCount();
		for (int i = 0; i < vertexCount; ++i)
		{
			vb[i].normal *= -1.0f;
		}
		
		uint16_t* indices = (uint16_t*)ib;
		int indexCount = getIndexBuffer()->getIndexCount();
		for (int i = 0; i < indexCount; i += 3)
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
IndexBuffer* MeshResource::getIndexBuffer() const
{
	return m_indexBufferInfo.buffer;
}

//------------------------------------------------------------------------------
void MeshResource::addTeapot(float size, int tessellation)
{
	int startIndex = getVertexCount();
	if (LN_ENSURE(startIndex <= UINT16_MAX)) return;

	// setup factory
	detail::TeapotMeshFactory factory;
	factory.initialize(size, tessellation, Color::White, Matrix::Identity);

	// alloc buffers, generate mesh
	Vertex* vb = (Vertex*)requestVertexBufferForAdditional(factory.getVertexCount(), VB_BasicVertices);
	uint16_t* ib = requestIndexBufferForAdditional(factory.getIndexCount());
	factory.setBuffers(vb, ib, IndexBufferFormat_UInt16, (uint16_t)startIndex);
	factory.generate();
}

////------------------------------------------------------------------------------
//void* MeshResource::TryLockVertexBuffer(VertexBufferType type)
//{
//	if (LN_REQUIRE(m_vertexUsedCount > 0)) return nullptr;
//
//	const size_t strideTable[VB_Count] =
//	{
//		sizeof(Vertex),			//VB_BasicVertices,
//		sizeof(BlendWeight),	//VB_BlendWeights,
//		sizeof(AdditionalUVs),	//VB_AdditionalUVs,
//		sizeof(SdefInfo),		//VB_SdefInfo,
//		sizeof(MmdExtra),		//VB_MmdExtra,
//	};
//	size_t requestedSize = strideTable[type] * m_vertexCapacity;
//
//	//if (m_usage == ResourceUsage::Dynamic)
//	{
//		if (m_vertexBufferInfos[type].buffer != nullptr &&
//			m_vertexBufferInfos[type].buffer->GetSize() != requestedSize)
//		{
//			//// unlock
//			//if (m_vertexBufferInfos[type].lockedBuffer != nullptr)
//			//{
//			//	m_vertexBufferInfos[type].lockedBuffer = nullptr;
//			//	m_vertexBufferInfos[type].buffer->unlock();
//			//}
//
//			// Resize
//			m_vertexBufferInfos[type].buffer->Resize(requestedSize);
//		}
//	}
//
//	if (m_vertexBufferInfos[type].buffer == nullptr)
//	{
//		m_vertexBufferInfos[type].buffer = Ref<VertexBuffer>::MakeRef();
//		m_vertexBufferInfos[type].buffer->initialize(m_manager, requestedSize, nullptr, m_usage, false);
//		m_vertexDeclarationModified = true;
//	}
//
//	//if (m_vertexBufferInfos[type].lockedBuffer == nullptr)
//	{
//		//ByteBuffer* buf = m_vertexBufferInfos[type].buffer->getMappedData();
//		//m_vertexBufferInfos[type].lockedBuffer = buf->GetData();
//		m_vertexBufferInfos[type].lockedBuffer = m_vertexBufferInfos[type].buffer->getMappedData();
//	}
//	return m_vertexBufferInfos[type].lockedBuffer;
//}

////------------------------------------------------------------------------------
//void* MeshResource::TryLockIndexBuffer()
//{
//	if (LN_REQUIRE(m_indexUsedCount > 0)) return nullptr;
//
//	//if (m_usage == ResourceUsage::Dynamic)
//	{
//		if (m_indexBufferInfo.buffer != nullptr &&
//			(m_indexBufferInfo.buffer->getIndexCount() != m_indexCapacity/* || m_indexBufferInfo.buffer->getIndexFormat() != m_indexBufferInfo.format*/))
//		{
//			// unlock
//			//if (m_indexBufferInfo.lockedBuffer != nullptr)
//			//{
//			//	m_indexBufferInfo.lockedBuffer = nullptr;
//			//	m_indexBufferInfo.buffer->unlock();
//			//}
//
//			// Resize
//			m_indexBufferInfo.buffer->Resize(m_indexCapacity/*, m_indexBufferInfo.format*/);
//		}
//	}
//
//	if (m_indexBufferInfo.buffer == nullptr)
//	{
//		m_indexBufferInfo.buffer = Ref<IndexBuffer>::MakeRef();
//		m_indexBufferInfo.buffer->initialize(m_manager, m_indexCapacity, nullptr, m_indexBufferInfo.format, m_usage, false);
//		m_indexBufferInfo.refresh = false;
//	}
//	//else if (m_indexBufferInfo.refresh)
//	//{
//	//	m_indexBufferInfo.buffer->Resize();
//	//	m_indexBufferInfo.refresh = false;
//	//}
//
//	//if (m_indexBufferInfo.lockedBuffer == nullptr)
//	{
//		m_indexBufferInfo.lockedBuffer = m_indexBufferInfo.buffer->getMappedData();
//	}
//	return m_indexBufferInfo.lockedBuffer;
//}

//------------------------------------------------------------------------------
//void MeshResource::TryGlowVertexBuffers(int requestVertexCount)
//{
//	if (m_vertexUsedCount + requestVertexCount > m_vertexCapacity)
//	{
//		m_vertexCapacity += std::max(m_vertexCapacity, requestVertexCount);
//		for (int i = 0; i < VB_Count; ++i)
//		{
//			m_vertexBufferInfos[i].refresh = true;	// 次の tryLock で Resize してほしい
//		}
//	}
//}

//------------------------------------------------------------------------------
//void MeshResource::TryGlowIndexBuffer(int requestIndexCount)
//{
//	if (m_indexUsedCount + requestIndexCount > m_indexCapacity)
//	{
//		m_indexCapacity += std::max(m_indexCapacity, requestIndexCount);
//		m_indexBufferInfo.refresh = true;	// 次の tryLock で Resize してほしい
//	}
//}
//
//------------------------------------------------------------------------------
void* MeshResource::requestVertexBufferForAdditional(int additionalVertexCount, VertexBufferType type)
{
	int begin = getVertexCount();
	int newCount = begin + additionalVertexCount;
	//TryGlowVertexBuffers(newCount);
	//m_vertexUsedCount = newCount;

	VertexBuffer* vertexBuffer = requestVertexBuffer(type);
	Vertex* vb = (Vertex*)vertexBuffer->requestMappedData(newCount * vertexStrideTable[type]);
	return vb + begin;
}

//------------------------------------------------------------------------------
uint16_t* MeshResource::requestIndexBufferForAdditional(int additionalIndexCount)
{
	int begin = (getIndexBuffer() != nullptr) ? getIndexBuffer()->getIndexCount() : 0;
	int newCount = begin + additionalIndexCount;

	if (LN_REQUIRE(m_indexBufferInfo.buffer == nullptr || m_indexBufferInfo.buffer->getIndexStride() == 2)) return nullptr;
	if (LN_REQUIRE(newCount <= UINT16_MAX)) return nullptr;

	//TryGlowIndexBuffer(newCount);
	//m_indexUsedCount = newCount;
	
	IndexBuffer* indexBuffer = requestIndexBuffer();
	uint16_t* ib = (uint16_t*)indexBuffer->requestMappedData(newCount);
	return ib + begin;
}

//------------------------------------------------------------------------------
VertexBuffer* MeshResource::getVertexBuffer(VertexBufferType type) const
{
	return m_vertexBufferInfos[type].buffer;
}

//------------------------------------------------------------------------------
VertexBuffer* MeshResource::requestVertexBuffer(VertexBufferType type)
{

	if (m_vertexBufferInfos[type].buffer == nullptr)
	{
		int size = 0;
		if (type != VB_BasicVertices)
		{
			size = m_vertexBufferInfos[VB_BasicVertices].buffer->getSize();
		}

		m_vertexBufferInfos[type].buffer = ln::newObject<VertexBuffer>(m_manager, size, nullptr, m_usage, false);
		m_vertexDeclarationModified = true;
	}
	return m_vertexBufferInfos[type].buffer;
}

//------------------------------------------------------------------------------
IndexBuffer* MeshResource::requestIndexBuffer()
{
	if (m_indexBufferInfo.buffer == nullptr)
	{
		// TODO: sizeConst マーク
		m_indexBufferInfo.buffer = ln::newObject<IndexBuffer>(m_manager, 0, nullptr, IndexBufferFormat_UInt16, m_usage, false);
	}
	return m_indexBufferInfo.buffer;
}

//------------------------------------------------------------------------------
void MeshResource::getMeshAttribute(int subsetIndex, MeshAttribute* outAttr)
{
	if (m_attributes.isEmpty())
	{
		outAttr->MaterialIndex = 0;
		outAttr->StartIndex = 0;
		outAttr->PrimitiveNum = getIndexBuffer()->getIndexCount() / 3;	// triangle only
		outAttr->primitiveType = PrimitiveType_TriangleList;
	}
	else
	{
		*outAttr = m_attributes[subsetIndex];
	}
}

//------------------------------------------------------------------------------
void MeshResource::commitRenderData(VertexDeclaration** outDecl, VertexBuffer** outVBs, int* outVBCount, IndexBuffer** outIB)
{
	LN_ASSERT(outDecl != nullptr);
	LN_ASSERT(outVBs != nullptr);
	LN_ASSERT(outVBCount != nullptr);
	LN_ASSERT(outIB != nullptr);

	// VertexDeclaration
	if (m_vertexDeclaration == nullptr || m_vertexDeclarationModified)
	{
		m_vertexDeclaration = Ref<VertexDeclaration>::makeRef();
		m_vertexDeclaration->initialize(m_manager);
		int stream = 0;

		// BasicVertices
		if (m_vertexBufferInfos[VB_BasicVertices].buffer != nullptr)
		{
			m_vertexDeclaration->addVertexElement(stream, VertexElementType_Float3, VertexElementUsage_Position, 0);
			m_vertexDeclaration->addVertexElement(stream, VertexElementType_Float2, VertexElementUsage_TexCoord, 0);
			m_vertexDeclaration->addVertexElement(stream, VertexElementType_Float3, VertexElementUsage_Normal, 0);
			m_vertexDeclaration->addVertexElement(stream, VertexElementType_Float4, VertexElementUsage_Color, 0);
			++stream;
		}

		// BlendWeights
		if (m_vertexBufferInfos[VB_BlendWeights].buffer != nullptr)
		{
			m_vertexDeclaration->addVertexElement(stream, VertexElementType_Float4, VertexElementUsage_BlendWeight, 0);
			m_vertexDeclaration->addVertexElement(stream, VertexElementType_Float4, VertexElementUsage_BlendIndices, 0);
			++stream;
		}

		// AdditionalUVs
		if (m_vertexBufferInfos[VB_AdditionalUVs].buffer != nullptr)
		{
			m_vertexDeclaration->addVertexElement(stream, VertexElementType_Float4, VertexElementUsage_TexCoord, 1);
			m_vertexDeclaration->addVertexElement(stream, VertexElementType_Float4, VertexElementUsage_TexCoord, 2);
			m_vertexDeclaration->addVertexElement(stream, VertexElementType_Float4, VertexElementUsage_TexCoord, 3);
			m_vertexDeclaration->addVertexElement(stream, VertexElementType_Float4, VertexElementUsage_TexCoord, 4);
			++stream;
		}

		// SdefInfo
		if (m_vertexBufferInfos[VB_SdefInfo].buffer != nullptr)
		{
			m_vertexDeclaration->addVertexElement(stream, VertexElementType_Float4, VertexElementUsage_TexCoord, 5);
			m_vertexDeclaration->addVertexElement(stream, VertexElementType_Float3, VertexElementUsage_TexCoord, 6);
			m_vertexDeclaration->addVertexElement(stream, VertexElementType_Float3, VertexElementUsage_TexCoord, 7);
			++stream;
		}

		// MmdExtra
		if (m_vertexBufferInfos[VB_MmdExtra].buffer != nullptr)
		{
			m_vertexDeclaration->addVertexElement(stream, VertexElementType_Float1, VertexElementUsage_TexCoord, 8);
			m_vertexDeclaration->addVertexElement(stream, VertexElementType_Float1, VertexElementUsage_PointSize, 15);
			++stream;
		}
		m_vertexDeclarationModified = false;
	}
	*outDecl = m_vertexDeclaration;

	// VertexBuffer
	int vbCount = 0;
	for (int i = 0; i < VB_Count; ++i)
	{
		//if (m_vertexBufferInfos[i].lockedBuffer != nullptr)
		//{
		//	m_vertexBufferInfos[i].buffer->unlock();
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
	//	m_indexBufferInfo.buffer->unlock();
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
//	if (flags.TestFlag(MeshCreationFlags::reverseFaces))
//	{
//		if (m_indexBufferInfo.buffer->getIndexStride() == 2)
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

void MeshResource::setBoundingBox(const Box& box)
{
	m_boundingBox = box;
}

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
void StaticMeshModel::initialize(detail::GraphicsManager* manager)
{
	if (LN_REQUIRE(manager != nullptr)) return;
	m_materials = Ref<MaterialList>::makeRef();
}

//------------------------------------------------------------------------------
void StaticMeshModel::initialize(MeshResource* sharingMesh)
{
	if (LN_REQUIRE(sharingMesh != nullptr)) return;

	// メッシュ(バッファ類)は共有する
	m_meshResources.add(sharingMesh);

	m_materials = Ref<MaterialList>::makeRef();

	// マテリアルはコピーする
	// TODO: コピー有無のフラグがあったほうがいいかも？
	//int count = m_meshResource->m_materials->GetCount();
	//m_materials = Ref<MaterialList>::MakeRef();
	//m_materials->Resize(count);
	//for (int i = 0; i < count; ++i)
	//{
	//	m_materials->SetAt(i, m_meshResource->m_materials->GetAt(i)->copyShared());
	//}
}

//------------------------------------------------------------------------------
void StaticMeshModel::initializeBox(const Vector3& size, MeshCreationFlags flags)
{
	auto res = Ref<MeshResource>::makeRef();
	res->initialize(detail::GraphicsManager::getInstance(), flags);
	res->addBox(size);
	if (flags.TestFlag(MeshCreationFlags::reverseFaces)) res->reverseFaces();
	initialize(res);
	addMaterials(1);
}

//------------------------------------------------------------------------------
void StaticMeshModel::initializeSphere(float radius, int slices, int stacks, MeshCreationFlags flags)
{
	auto res = Ref<MeshResource>::makeRef();
	res->initialize(detail::GraphicsManager::getInstance(), flags);
	res->addSphere(radius, slices, stacks);
	if (flags.TestFlag(MeshCreationFlags::reverseFaces)) res->reverseFaces();
	initialize(res);
	addMaterials(1);
}

//------------------------------------------------------------------------------
void StaticMeshModel::initializePlane(const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags)
{
	auto res = Ref<MeshResource>::makeRef();
	res->initialize(detail::GraphicsManager::getInstance(), flags);
	res->addPlane(size, sliceH, sliceV);
	if (flags.TestFlag(MeshCreationFlags::reverseFaces)) res->reverseFaces();
	initialize(res);
	addMaterials(1);
}

//------------------------------------------------------------------------------
void StaticMeshModel::initializeScreenPlane(MeshCreationFlags flags)
{
	auto res = Ref<MeshResource>::makeRef();
	res->initialize(detail::GraphicsManager::getInstance(), flags);
	res->addScreenPlane();
	if (flags.TestFlag(MeshCreationFlags::reverseFaces)) res->reverseFaces();
	initialize(res);
	addMaterials(1);
}

//------------------------------------------------------------------------------
void StaticMeshModel::initializeTeapot(float size, int tessellation, MeshCreationFlags flags)
{
	auto res = Ref<MeshResource>::makeRef();
	res->initialize(detail::GraphicsManager::getInstance(), flags);
	res->addTeapot(size, tessellation);
	if (flags.TestFlag(MeshCreationFlags::reverseFaces)) res->reverseFaces();
	initialize(res);
	addMaterials(1);
}

//------------------------------------------------------------------------------
//int StaticMeshModel::getSubsetCount() const
//{
//	return m_meshResource->getSubsetCount();
//}

//------------------------------------------------------------------------------
MeshResource* StaticMeshModel::FindMesh(const StringRef& name)
{
	auto mesh = m_meshResources.find([name](MeshResource* mesh) { return mesh->getName() == name; });
	return (mesh != nullptr) ? *mesh : nullptr;
}

//------------------------------------------------------------------------------
void StaticMeshModel::addMaterials(int count)
{
	int oldCount = m_materials->getCount();
	int newCount = oldCount + count;
	m_materials->resize(newCount);
	if (oldCount < newCount)
	{
		for (int i = oldCount; i < newCount; ++i)
		{
			auto m = newObject<CommonMaterial>();
			m_materials->getAt(i, m);
		}
	}
}

//------------------------------------------------------------------------------
void StaticMeshModel::addMaterial(CommonMaterial* material)
{
	if (LN_ENSURE(m_materials)) return;
	m_materials->add(material);
}

//------------------------------------------------------------------------------
CommonMaterial* StaticMeshModel::getMaterial(int index) const
{
	return m_materials->getAt(index);
}

LN_NAMESPACE_END

