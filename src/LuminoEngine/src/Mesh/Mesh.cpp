/*
	MeshResource 注意点

	requestXXXX 系の関数は、ユーザーが要求したサイズなどの情報に応じて VertexBuffer と IndexBuffer の遅延構築を行う。
	m_vertexBuffers と m_indexBuffer に直接アクセスしてよいのは requestXXXX 系の関数だけ。
	それ以外でこれらのデータにアクセスしたい場合は必ず requestXXXX 系の関数でインスタンスを取得すること。
*/
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include "MeshManager.hpp"

namespace ln {

//==============================================================================
// MeshResource

const std::array<size_t, MeshResource::VBG_Count> MeshResource::VertexStrideTable =
{
	sizeof(Vertex),
	sizeof(VertexBlendWeight),
	sizeof(VertexAdditionalUV),
	sizeof(VertexSdefInfo),
	sizeof(VertexMmdExtra),
};

MeshResource::MeshResource()
	: m_manager()
    , m_ownerContainer(nullptr)
	, m_vertexCount(0)
	, m_indexCount(0)
	, m_usage(GraphicsResourceUsage::Static)
	, m_vertexBuffers{}
	, m_indexBuffer(nullptr)
	, m_sections()
{
}

MeshResource::~MeshResource()
{
}

void MeshResource::init()
{
	Object::init();
	m_manager = detail::EngineDomain::meshManager();
}

int MeshResource::vertexCount() const
{
	return m_vertexCount;
}

int MeshResource::indexCount() const
{
	return m_indexCount;
}

int MeshResource::faceCount() const
{
	return indexCount() / 3;
}

void MeshResource::resizeVertexBuffer(int vertexCount)
{
	m_vertexCount = vertexCount;
}

void MeshResource::resizeIndexBuffer(int indexCount)
{
	m_indexCount = indexCount;
}

void MeshResource::resizeSections(int sectionCount)
{
	m_sections.resize(sectionCount);
}

void MeshResource::setVertex(int index, const Vertex& value)
{
	if (LN_REQUIRE_RANGE(index, 0, vertexCount())) return;

	Vertex* v = (Vertex*)requestVertexData(VBG_Basic);
	*(v + index) = value;
}

const Vertex& MeshResource::vertex(int index)
{
    if (LN_REQUIRE_RANGE(index, 0, vertexCount())) return Vertex::Default;

    Vertex* v = (Vertex*)requestVertexData(VBG_Basic);
    return *(v + index);
}

void MeshResource::setBlendWeights(int index, float v0, float v1, float v2, float v3)
{
    if (LN_REQUIRE_RANGE(index, 0, vertexCount())) return;
    VertexBlendWeight* v = ((VertexBlendWeight*)requestVertexData(VBG_BlendWeights) + index);
    v->weights[0] = v0;
    v->weights[1] = v1;
    v->weights[2] = v2;
    v->weights[3] = v3;
}

void MeshResource::setBlendIndices(int index, float v0, float v1, float v2, float v3)
{
    if (LN_REQUIRE_RANGE(index, 0, vertexCount())) return;
    VertexBlendWeight* v = ((VertexBlendWeight*)requestVertexData(VBG_BlendWeights) + index);
    v->indices[0] = v0;
    v->indices[1] = v1;
    v->indices[2] = v2;
    v->indices[3] = v3;
}

const VertexBlendWeight& MeshResource::vertexBlendWeight(int index)
{
    if (LN_REQUIRE_RANGE(index, 0, vertexCount())) return VertexBlendWeight::Default;
    return *((VertexBlendWeight*)requestVertexData(VBG_BlendWeights) + index);
}

void MeshResource::setSdefInfo(int index, const Vector4& sdefC, const Vector3& sdefR0, const Vector3& sdefR1)
{
    if (LN_REQUIRE_RANGE(index, 0, vertexCount())) return;
    VertexSdefInfo* v = ((VertexSdefInfo*)requestVertexData(VBG_SdefInfo) + index);
    v->sdefC = sdefC;
    v->sdefR0 = sdefR0;
    v->sdefR1 = sdefR1;
}

void MeshResource::setMmdExtra(int index, float edgeWeight, float vertexIndex)
{
    if (LN_REQUIRE_RANGE(index, 0, vertexCount())) return;
    VertexMmdExtra* v = ((VertexMmdExtra*)requestVertexData(VBG_MmdExtra) + index);
    v->edgeWeight = edgeWeight;
    v->index = vertexIndex;
}

void MeshResource::setIndex(int index, int value)
{
	if (LN_REQUIRE_RANGE(index, 0, indexCount())) return;

	IndexBuffer* indexBuffer;
	requestBuffers(VBG_Basic, nullptr, &indexBuffer);
	indexBuffer->setIndex(index, value);
}

int MeshResource::index(int index)
{
    if (LN_REQUIRE_RANGE(index, 0, indexCount())) return 0;

    IndexBuffer* indexBuffer;
    requestBuffers(VBG_Basic, nullptr, &indexBuffer);
    return indexBuffer->index(index);
}

void MeshResource::setSection(int sectionIndex, int startIndex, int primitiveCount, int materialIndex)
{
	MeshSection& section = m_sections[sectionIndex];
	section.startIndex = startIndex;
	section.primitiveCount = primitiveCount;
	section.materialIndex = materialIndex;
}

void MeshResource::addSection(int startIndex, int primitiveCount, int materialIndex)
{
    MeshSection section;
    section.startIndex = startIndex;
    section.primitiveCount = primitiveCount;
    section.materialIndex = materialIndex;
    m_sections.add(section);
}

int MeshResource::realVertexCount(VertexBufferGroup group) const
{
	if (m_vertexBuffers[group]) {
		return m_vertexBuffers[group]->size() / sizeof(Vertex);
	}
	return 0;
}

int MeshResource::realIndexCount() const
{
	if (m_indexBuffer) {
		return m_indexBuffer->size();
	}
	return 0;
}

void MeshResource::requestBuffers(VertexBufferGroup group, VertexBuffer** outVertexBuffer, IndexBuffer** outIndexBuffer)
{
	// prepare vertex buffers
	if (!m_vertexBuffers[group]) {
		m_vertexBuffers[group] = ln::makeObject<VertexBuffer>(m_vertexCount * VertexStrideTable[group], m_usage);
	}

	// sync vertex buffers size
	for (int i = 0; i < m_vertexBuffers.size(); i++) {
		if (m_vertexBuffers[i] && realVertexCount((VertexBufferGroup)i) != m_vertexCount) {
			m_vertexBuffers[i]->resize(m_vertexCount * VertexStrideTable[i]);
		}
	}

	if (outVertexBuffer) {
		*outVertexBuffer = m_vertexBuffers[group];
	}

    // Create index buffer if requested
	if (outIndexBuffer) {
        // prepare index buffer
        if (!m_indexBuffer) {
            m_indexBuffer = ln::makeObject<IndexBuffer>(m_indexCount, detail::GraphicsResourceInternal::selectIndexBufferFormat(m_vertexCount), m_usage);
        }
        else if (realIndexCount() != m_indexCount) {
            m_indexBuffer->resize(m_indexCount);
        }

        // check index buffer format
        if (m_indexBuffer->format() == IndexBufferFormat::UInt16) {
            if (m_vertexCount > 0xFFFF) {
                m_indexBuffer->setFormat(IndexBufferFormat::UInt32);
            }
        }
		*outIndexBuffer = m_indexBuffer;
	}
}

void* MeshResource::requestVertexData(VertexBufferGroup group)
{
	VertexBuffer* vertexBuffer;
	requestBuffers(group, &vertexBuffer, nullptr);

	return vertexBuffer->map(MapMode::Write);
}

void* MeshResource::requestIndexData(IndexBufferFormat* outFormat)
{
	IndexBuffer* indexBuffer;
	requestBuffers(VBG_Basic, nullptr, &indexBuffer);

	if (outFormat) {
		*outFormat = indexBuffer->format();
	}
	return indexBuffer->map(MapMode::Write);
}

void* MeshResource::requestVertexDataForAdditional(int additionalVertexCount, VertexBufferGroup group)
{
	LN_CHECK(additionalVertexCount > 0);

	int begin = realVertexCount(group);
	int newCount = begin + additionalVertexCount;

	// resize all vertex buffers
	resizeVertexBuffer(newCount);

	VertexBuffer* vertexBuffer;
	requestBuffers(group, &vertexBuffer, nullptr);

	Vertex* data = (Vertex*)vertexBuffer->map(MapMode::Write);
	return data + begin;
}

void* MeshResource::requestIndexDataForAdditional(int additionalIndexCount, IndexBufferFormat* outFormat)
{
	int begin = realIndexCount();
	int newCount = begin + additionalIndexCount;

	// resize all vertex buffers
	resizeIndexBuffer(newCount);

	IndexBuffer* indexBuffer;
	requestBuffers(VBG_Basic, nullptr, &indexBuffer);

	if (outFormat) {
		*outFormat = indexBuffer->format();
	}

	void* data = indexBuffer->map(MapMode::Write);
	if (indexBuffer->format() == IndexBufferFormat::UInt16)
		return ((uint16_t*)data) + begin;
	else
		return ((uint32_t*)data) + begin;
}

void MeshResource::commitRenderData(int sectionIndex, MeshSection* outSection, VertexLayout** outDecl, VertexBuffer** outVBs, int* outVBCount, IndexBuffer** outIB)
{
	// Section
	*outSection = m_sections[sectionIndex];

	// VertexLayout
	{
		uint32_t flags = 0;
		if (m_vertexBuffers[VBG_Basic]) flags |= detail::PredefinedVertexLayoutFlags_Geometry;
		if (m_vertexBuffers[VBG_BlendWeights]) flags |= detail::PredefinedVertexLayoutFlags_BlendWeights;
		if (m_vertexBuffers[VBG_AdditionalUVs]) flags |= detail::PredefinedVertexLayoutFlags_AdditionalUV;
		if (m_vertexBuffers[VBG_SdefInfo]) flags |= detail::PredefinedVertexLayoutFlags_SdefInfo;
		if (m_vertexBuffers[VBG_MmdExtra]) flags |= detail::PredefinedVertexLayoutFlags_MmdExtra;
        if (LN_REQUIRE(flags != 0)) return;
		*outDecl = m_manager->getPredefinedVertexLayout((detail::PredefinedVertexLayoutFlags)flags);
	}

	// VertexBuffer
	int vbCount = 0;
	for (int i = 0; i < VBG_Count; ++i)
	{
		if (m_vertexBuffers[i])
		{
			outVBs[vbCount] = m_vertexBuffers[i];
			++vbCount;
		}
	}
	*outVBCount = vbCount;
	
	// IndexBuffer
	*outIB = m_indexBuffer;
}

bool MeshResource::isInitialEmpty() const
{
    for (auto& buf : m_vertexBuffers) {
        if (buf) {
            return false;
        }
    }
    return true;
}

//==============================================================================
// Mesh

Mesh::Mesh()
	: m_resourceUsage(GraphicsResourceUsage::Static)
{
}

Mesh::~Mesh()
{
}

void Mesh::init()
{
	Object::init();
}

void Mesh::init(int vertexCount, int indexCount)
{
	init();
	m_vertexCount = vertexCount;
	m_indexCount = indexCount;
	m_indexFormat = detail::GraphicsResourceInternal::selectIndexBufferFormat(m_vertexCount);
}

void Mesh::init(int vertexCount, int indexCount, IndexBufferFormat indexFormat, GraphicsResourceUsage resourceUsage)
{
	init();
	m_vertexCount = vertexCount;
	m_indexCount = indexCount;
	m_indexFormat = indexFormat;
	m_resourceUsage = resourceUsage;
}

void Mesh::setVertex(int index, const Vertex& value)
{
	if (LN_REQUIRE(0 <= index && index < m_vertexCount)) return;
	reinterpret_cast<Vertex*>(acquireMappedVertexBuffer(InterleavedVertexGroup::Main))[index] = value;
}

const Vertex& Mesh::vertex(int index)
{
	if (LN_REQUIRE(0 <= index && index < m_vertexCount)) return Vertex::Default;
	return reinterpret_cast<Vertex*>(acquireMappedVertexBuffer(InterleavedVertexGroup::Main))[index];
}

void Mesh::setIndex(int index, int value)
{
	if (LN_REQUIRE(0 <= index && index < m_indexCount)) return;
	
	switch (m_indexFormat)
	{
	case IndexBufferFormat::UInt16:
		reinterpret_cast<uint16_t*>(acquireMappedIndexBuffer())[index] = value;
		break;
	case IndexBufferFormat::UInt32:
		reinterpret_cast<uint32_t*>(acquireMappedIndexBuffer())[index] = value;
		break;
	default:
		LN_UNREACHABLE();
		break;
	}
}

int Mesh::index(int index)
{
	if (LN_REQUIRE(0 <= index && index < m_indexCount)) return 0;

	switch (m_indexFormat)
	{
	case IndexBufferFormat::UInt16:
		return reinterpret_cast<uint16_t*>(acquireMappedIndexBuffer())[index];
	case IndexBufferFormat::UInt32:
		return reinterpret_cast<uint32_t*>(acquireMappedIndexBuffer())[index];
	default:
		LN_UNREACHABLE();
		return 0;
	}
}

void Mesh::addSection(int startIndex, int primitiveCount, int materialIndex, PrimitiveTopology topology)
{
	MeshSection2 meshSection;
	meshSection.startIndex = startIndex;
	meshSection.primitiveCount = primitiveCount;
	meshSection.materialIndex = materialIndex;
	meshSection.topology = topology;
	m_sections.add(meshSection);
}

void Mesh::setSection(int sectionIndex, int startIndex, int primitiveCount, int materialIndex, PrimitiveTopology topology)
{
	MeshSection2& meshSection = m_sections[sectionIndex];
	meshSection.startIndex = startIndex;
	meshSection.primitiveCount = primitiveCount;
	meshSection.materialIndex = materialIndex;
	meshSection.topology = topology;
}

void Mesh::commitRenderData(int sectionIndex, MeshSection2* outSection, VertexLayout** outDecl, std::array<VertexBuffer*, 16>* outVBs, int* outVBCount, IndexBuffer** outIB)
{
	// unmap
	{
		if (m_mainVertexBuffer.mappedBuffer) {
			m_mainVertexBuffer.buffer->unmap();
			m_mainVertexBuffer.mappedBuffer = nullptr;
		}
		if (m_tangentsVertexBuffer.mappedBuffer) {
			m_tangentsVertexBuffer.buffer->unmap();
			m_tangentsVertexBuffer.mappedBuffer = nullptr;
		}
		if (m_skinningVertexBuffer.mappedBuffer) {
			m_skinningVertexBuffer.buffer->unmap();
			m_skinningVertexBuffer.mappedBuffer = nullptr;
		}
		for (auto& v : m_extraVertexBuffers) {
			if (v.entry.mappedBuffer) {
				v.entry.buffer->unmap();
				v.entry.mappedBuffer = nullptr;
			}
		}
		if (m_indexBuffer.mappedBuffer) {
			m_indexBuffer.buffer->unmap();
			m_indexBuffer.mappedBuffer = nullptr;
		}
	}

	attemptResetVertexLayout();

    *outSection = m_sections[sectionIndex];
    *outDecl = m_vertexLayout;
    *outIB = m_indexBuffer.buffer;

	int count = 0;

	if (m_mainVertexBuffer.buffer) {
		(*outVBs)[count] = m_mainVertexBuffer.buffer;
		count++;
	}

	if (m_tangentsVertexBuffer.buffer) {
		(*outVBs)[count] = m_tangentsVertexBuffer.buffer;
		count++;
	}

	if (m_skinningVertexBuffer.buffer) {
		(*outVBs)[count] = m_skinningVertexBuffer.buffer;
		count++;
	}

    for (int i = 0; i < m_extraVertexBuffers.size(); i++) {
        (*outVBs)[count] = m_extraVertexBuffers[i].entry.buffer;
		count++;
    }

	*outVBCount = count;
}

InterleavedVertexGroup Mesh::getStandardElement(VertexElementUsage usage, int usageIndex) const
{
	if (usageIndex == 0) {
		if (usage == VertexElementUsage::Position ||
			usage == VertexElementUsage::Normal ||
			usage == VertexElementUsage::Color ||
			usage == VertexElementUsage::TexCoord) {
			return InterleavedVertexGroup::Main;
		}
		if (usage == VertexElementUsage::Tangent ||
			usage == VertexElementUsage::Binormal) {
			return InterleavedVertexGroup::Tangents;
		}
		if (usage == VertexElementUsage::BlendWeight ||
			usage == VertexElementUsage::BlendIndices) {
			return InterleavedVertexGroup::Skinning;
		}
	}
	else if (1 <= usageIndex && usageIndex <= 3) {
		if (usage == VertexElementUsage::TexCoord) {
			return InterleavedVertexGroup::AdditionalUV;
		}
	}
	return InterleavedVertexGroup::Undefined;
}

void* Mesh::acquireMappedVertexBuffer(InterleavedVertexGroup group)
{
	switch (group)
	{
	case InterleavedVertexGroup::Undefined:
	{
		LN_UNREACHABLE();
		return nullptr;
	}
	case InterleavedVertexGroup::Main:
		if (!m_mainVertexBuffer.buffer) {
			m_mainVertexBuffer.buffer = makeObject<VertexBuffer>(sizeof(Vertex) * m_vertexCount, m_resourceUsage);

			// set default
			auto* buf = static_cast<Vertex*>(m_mainVertexBuffer.buffer->map(MapMode::Write));
			for (int i = 0; i < m_vertexCount; i++) {
				buf[i].normal = Vector3(0, 0, 1);
				buf[i].color = Color::White;
			}
		}

		if (!m_mainVertexBuffer.mappedBuffer) {
			m_mainVertexBuffer.mappedBuffer = m_mainVertexBuffer.buffer->map(MapMode::Write);
		}
		return m_mainVertexBuffer.mappedBuffer;

	case InterleavedVertexGroup::Tangents:
		if (!m_tangentsVertexBuffer.buffer) {
			m_tangentsVertexBuffer.buffer = makeObject<VertexBuffer>(sizeof(VertexTangents) * m_vertexCount, m_resourceUsage);

			// set default
			auto* buf = static_cast<VertexTangents*>(m_mainVertexBuffer.buffer->map(MapMode::Write));
			for (int i = 0; i < m_vertexCount; i++) {
				buf[i] = VertexTangents::Default;
			}
		}

		if (!m_tangentsVertexBuffer.mappedBuffer) {
			m_tangentsVertexBuffer.mappedBuffer = m_tangentsVertexBuffer.buffer->map(MapMode::Write);
		}
		return m_tangentsVertexBuffer.mappedBuffer;

	case InterleavedVertexGroup::Skinning:
		if (!m_skinningVertexBuffer.buffer) {
			m_skinningVertexBuffer.buffer = makeObject<VertexBuffer>(sizeof(VertexBlendWeight) * m_vertexCount, m_resourceUsage);
		}

		if (!m_skinningVertexBuffer.mappedBuffer) {
			m_skinningVertexBuffer.mappedBuffer = m_skinningVertexBuffer.buffer->map(MapMode::Write);
		}
		return m_skinningVertexBuffer.mappedBuffer;

	case InterleavedVertexGroup::AdditionalUV:
		if (!m_additionalUVVertexBuffer.buffer) {
			m_additionalUVVertexBuffer.buffer = makeObject<VertexBuffer>(sizeof(VertexAdditionalUV) * m_vertexCount, m_resourceUsage);
		}

		if (!m_additionalUVVertexBuffer.mappedBuffer) {
			m_additionalUVVertexBuffer.mappedBuffer = m_additionalUVVertexBuffer.buffer->map(MapMode::Write);
		}
		return m_additionalUVVertexBuffer.mappedBuffer;

	default:
		LN_UNREACHABLE();
		return nullptr;
	}
}

void* Mesh::acquireMappedVertexBuffer(VertexElementType type, VertexElementUsage usage, int usageIndex)
{
	auto group = getStandardElement(usage, usageIndex);
	if (group == InterleavedVertexGroup::Undefined) {
		VertexBuffer* vb = nullptr;
		auto r = m_extraVertexBuffers.findIf([&](auto& x) { return x.usage == usage && x.usageIndex == usageIndex; });
		if (!r) {
			if (r->type != type) {
				LN_ERROR();
				return nullptr;
			}
			auto vb = makeObject<VertexBuffer>(GraphicsHelper::getVertexElementTypeSize(type) * m_vertexCount, m_resourceUsage);
			
			VertexBufferEntry entry;
			entry.buffer = vb;
			entry.mappedBuffer = vb->map(MapMode::Write);

			m_extraVertexBuffers.add({ type, usage, usageIndex, entry });
			m_vertexLayout = nullptr;	// dirty layout

			return entry.mappedBuffer;
		}
		else {
			if (!r->entry.mappedBuffer) {
				r->entry.mappedBuffer = r->entry.buffer->map(MapMode::Write);
			}
			return r->entry.mappedBuffer;
		}
	}
	else {
		return acquireMappedVertexBuffer(group);
	}
}

void* Mesh::acquireMappedIndexBuffer()
{
	if (!m_indexBuffer.buffer) {
		m_indexBuffer.buffer = makeObject<IndexBuffer>(m_indexCount, m_indexFormat, m_resourceUsage);
	}

	//detail::GraphicsResourceInternal::selectIndexBufferFormat(indexCount)

	if (!m_indexBuffer.mappedBuffer) {
		m_indexBuffer.mappedBuffer = m_indexBuffer.buffer->map(MapMode::Write);
	}

	return m_indexBuffer.mappedBuffer;
}

VertexElementType Mesh::findVertexElementType(VertexElementUsage usage, int usageIndex) const
{
	// Find predefined vertex data.
	if (usageIndex == 0) {
		if (usage == VertexElementUsage::Position) return VertexElementType::Float3;
		if (usage == VertexElementUsage::Normal) return VertexElementType::Float3;
		if (usage == VertexElementUsage::TexCoord) return VertexElementType::Float2;
		if (usage == VertexElementUsage::Color) return VertexElementType::Float4;
		if (usage == VertexElementUsage::Tangent) return VertexElementType::Float3;
		if (usage == VertexElementUsage::Binormal) return VertexElementType::Float3;
		if (usage == VertexElementUsage::BlendWeight) return VertexElementType::Float4;
		if (usage == VertexElementUsage::BlendIndices) return VertexElementType::Float4;
	}
	if (usage == VertexElementUsage::TexCoord) {
		if (1 <= usageIndex && usageIndex <= 3) {
			return VertexElementType::Float4;
		}
	}

	// Find extra data.
	auto r = m_extraVertexBuffers.findIf([&](auto& x) { return x.usage == usage && x.usageIndex == usageIndex; });
	if (!r) return r->type;

	return VertexElementType::Unknown;
}

void Mesh::attemptResetVertexLayout()
{
	if (!m_vertexLayout) {
		m_vertexLayout = makeObject<VertexLayout>();

		int streamIndex = 0;

		if (m_mainVertexBuffer.buffer) {
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float3, VertexElementUsage::Position, 0);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float3, VertexElementUsage::Normal, 0);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float2, VertexElementUsage::TexCoord, 0);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Color, 0);
			streamIndex++;
		}

		if (m_tangentsVertexBuffer.buffer) {
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float3, VertexElementUsage::Tangent, 0);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float3, VertexElementUsage::Binormal, 0);
			streamIndex++;
		}

		if (m_skinningVertexBuffer.buffer) {
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::BlendIndices, 0);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::BlendWeight, 0);
			streamIndex++;
		}

		for (int i = 0; i < m_extraVertexBuffers.size(); i++) {
			const auto& attr = m_extraVertexBuffers[i];
			m_vertexLayout->addElement(streamIndex, attr.type, attr.usage, attr.usageIndex);
			streamIndex++;
		}
	}
}

//==============================================================================
// MeshContainer

MeshContainer::MeshContainer()
	: m_name()
	//, m_lodResources()
{
    m_lodMesh.resize(1);
}

MeshContainer::~MeshContainer()
{
}

void MeshContainer::init()
{
	Object::init();
	//m_lodResources.resize(1);
}
//
//void MeshContainer::setMeshResource(MeshResource* mesh)
//{
//    if (LN_REQUIRE(!mesh->m_ownerContainer)) return;
//    m_lodResources[0] = mesh;
//    mesh->m_ownerContainer = this;
//}
//
//MeshResource* MeshContainer::meshResource() const
//{
//	return m_lodResources[0];
//}
//
//MeshResource* MeshContainer::selectLODResource(float distance) const
//{
//	// TODO:
//	return m_lodResources[0];
//}
//
void MeshContainer::calculateBounds()
{
#if 1
	m_boundingBox = Box();
#else
	MeshResource* mesh = meshResource();
	if (mesh)
	{
		VertexBuffer* vertexBuffer;
		mesh->requestBuffers(MeshResource::VBG_Basic, &vertexBuffer, nullptr);
		Vertex* data = (Vertex*)vertexBuffer->map(MapMode::Read);

		int count = mesh->vertexCount();
		if (count > 0)
		{
			Vector3 min = data[0].position;
			Vector3 max = data[0].position;
			for (int i = 1; i < count; i++)
			{
				if (data[i].position.x < min.x)
					min.x = data[i].position.x;

				if (data[i].position.y < min.y)
					min.y = data[i].position.y;

				if (data[i].position.z < min.z)
					min.z = data[i].position.z;

				if (data[i].position.x > max.x)
					max.x = data[i].position.x;

				if (data[i].position.y > max.y)
					max.y = data[i].position.y;

				if (data[i].position.z > max.z)
					max.z = data[i].position.z;
			}

			m_boundingBox = Box(min, max);
		}
		else
		{
			m_boundingBox = Box();
		}
	}
	else
	{
		m_boundingBox = Box();
	}
#endif
}

void MeshContainer::setMesh(Mesh* mesh)
{
	//if (LN_REQUIRE(mesh)) return;
	//if (LN_REQUIRE(!mesh->m_owner)) return;
    m_lodMesh[0] = mesh;
	//mesh->m_owner = this;
}

Mesh* MeshContainer::mesh() const
{
    return m_lodMesh[0];
}
//==============================================================================
// MeshNode

MeshNode::MeshNode()
    : m_index(-1)
    , m_meshContainerIndex(-1)
    , m_children()
    , m_initialLocalTransform(Matrix::Identity)
{
}

void MeshNode::setPosition(const Vector3& value)
{
	m_localTransform.translation = value;
}

void MeshNode::setRotation(const Quaternion& value)
{
	m_localTransform.rotation = value;
}

void MeshNode::setScale(const Vector3& value)
{
	m_localTransform.scale = value;
}

void MeshNode::setMeshContainerIndex(int value)
{
    m_meshContainerIndex = value;
}

void MeshNode::addChildIndex(int value)
{
    m_children.add(value);
}

void MeshNode::setInitialLocalTransform(const Matrix& value)
{
	m_initialLocalTransform = value;
}

//==============================================================================
// StaticMeshModel

Ref<StaticMeshModel> StaticMeshModel::load(const StringRef& filePath, float scale)
{
    return detail::EngineDomain::meshManager()->createStaticMeshModel(filePath, scale);
}

StaticMeshModel::StaticMeshModel()
    : m_type(detail::InternalMeshModelType::StaticMesh)
{
}

StaticMeshModel::StaticMeshModel(detail::InternalMeshModelType type)
    : m_type(type)
{
}

MeshNode* StaticMeshModel::findNode(StringRef name) const
{
	return m_nodes.findIf([&](const auto& x) { return x->name() == name; }).valueOr(nullptr);
}

void StaticMeshModel::addMeshContainer(MeshContainer* meshContainer)
{
	if (LN_REQUIRE(meshContainer)) return;
	m_meshContainers.add(meshContainer);
}

void StaticMeshModel::addNode(MeshNode* node)
{
    if (LN_REQUIRE(node)) return;
    node->m_index = m_nodes.size();
    m_nodes.add(node);
}

void StaticMeshModel::addMaterial(Material* material)
{
	m_materials.add(material);
}

void StaticMeshModel::addRootNode(int index)
{
    m_rootNodes.add(index);
}

void StaticMeshModel::updateNodeTransforms()
{
    m_nodeGlobalTransforms.resize(m_nodes.size());
    for (int index : m_rootNodes) {
        updateNodeTransformsHierarchical(index, Matrix::Identity);
    }
}

void StaticMeshModel::updateNodeTransformsHierarchical(int nodeIndex, const Matrix& parentTransform)
{
    auto node = m_nodes[nodeIndex];

	Matrix local = Matrix::makeScaling(node->m_localTransform.scale);
	local.rotateQuaternion(node->m_localTransform.rotation);
	local.translate(node->m_localTransform.translation);

    m_nodeGlobalTransforms[nodeIndex] = node->initialLocalTransform() * local * parentTransform;   // NOTE: glTF はこの順である必要がある。

	// glview.cc と比べて Node の Transform の差分は無し。
	float* m = m_nodeGlobalTransforms[nodeIndex].data();
	for (int i = 0; i < 16; i++) std::cout << m[i] << ", ";

    for (int child : node->m_children) {
        updateNodeTransformsHierarchical(child, m_nodeGlobalTransforms[nodeIndex]);
    }
}

//==============================================================================
// InstancedMeshList

InstancedMeshList::InstancedMeshList()
	: m_sourceVertexLayout(nullptr)
	, m_dirty(true)
{
}

InstancedMeshList::~InstancedMeshList()
{
}

bool InstancedMeshList::init(Mesh* mesh, int sectionIndex)
{
	if (!Object::init()) return false;
	m_mesh = mesh;
	m_sectionIndex = sectionIndex;
	reset();
	return true;
}

void InstancedMeshList::reset()
{
	m_instanceData.clear();
	m_instanceCount = 0;
	m_dirty = true;
}

void InstancedMeshList::setTransform(const Matrix& transform)
{
	m_stagingData.transform0 = transform.row(0);
	m_stagingData.transform1 = transform.row(1);
	m_stagingData.transform2 = transform.row(2);
	m_stagingData.transform3 = transform.row(3);
}

void InstancedMeshList::setUVOffset(const Vector4& value)
{
	m_stagingData.uvOffset = value;
}

void InstancedMeshList::drawMesh()
{
	m_instanceData.push_back(m_stagingData);
	m_instanceCount++;
	m_dirty = true;
}

void InstancedMeshList::commitRenderData(MeshSection2* outSection, VertexLayout** outDecl, std::array<VertexBuffer*, 16>* outVBs, int* outVBCount, IndexBuffer** outIB)
{
	m_mesh->commitRenderData(m_sectionIndex, outSection, outDecl, outVBs, outVBCount, outIB);
	
	if (m_dirty) {
		if (m_sourceVertexLayout != *outDecl) {
			m_sourceVertexLayout = *outDecl;
			m_vertexLayout = makeObject<VertexLayout>();
			for (auto& e : m_sourceVertexLayout->vertexElements()) {
				m_vertexLayout->addElement(e);
			}

			int streamIndex = *outVBCount;
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Position, 1, VertexInputRate::Instance);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Position, 2, VertexInputRate::Instance);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Position, 3, VertexInputRate::Instance);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Position, 4, VertexInputRate::Instance);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::TexCoord, 2, VertexInputRate::Instance);
		}

		if (!m_instanceBuffer || (m_instanceBuffer->size() / sizeof(InstanceData)) < m_instanceCount) {
			m_instanceBuffer = makeObject<VertexBuffer>(m_instanceCount * sizeof(InstanceData), GraphicsResourceUsage::Dynamic);
		}

		void* data = m_instanceBuffer->map(MapMode::Write);
		memcpy(data, m_instanceData.data(), m_instanceCount * sizeof(InstanceData));
		m_instanceBuffer->unmap();

		m_dirty = false;
	}

	(*outDecl) = m_vertexLayout;
	(*outVBs)[*outVBCount] = m_instanceBuffer;
	(*outVBCount)++;
}


namespace detail {


} // namespace detail
} // namespace ln

