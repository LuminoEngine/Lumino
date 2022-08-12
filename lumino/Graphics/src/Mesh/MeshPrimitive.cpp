/*
	MeshResource 注意点

	requestXXXX 系の関数は、ユーザーが要求したサイズなどの情報に応じて VertexBuffer と IndexBuffer の遅延構築を行う。
	m_vertexBuffers と m_indexBuffer に直接アクセスしてよいのは requestXXXX 系の関数だけ。
	それ以外でこれらのデータにアクセスしたい場合は必ず requestXXXX 系の関数でインスタンスを取得すること。
*/
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoGraphics/GPU/VertexBuffer.hpp>
#include <LuminoGraphics/GPU/IndexBuffer.hpp>
#include <LuminoGraphics/GPU/VertexLayout.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Mesh/MeshPrimitive.hpp>
#include <LuminoGraphics/Mesh/MeshModel.hpp>
#include <LuminoGraphics/Mesh/MeshModeEntity.hpp>
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
	m_manager = detail::MeshManager::instance();
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
		m_vertexBuffers[group] = ln::makeObject_deprecated<VertexBuffer>(m_vertexCount * VertexStrideTable[group], m_usage);
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
            m_indexBuffer = ln::makeObject_deprecated<IndexBuffer>(m_indexCount, GraphicsHelper::selectIndexBufferFormat(m_vertexCount), m_usage);
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

	return vertexBuffer->writableData();
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

	Vertex* data = (Vertex*)vertexBuffer->writableData();
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

void MeshPrimitive::VertexBufferEntry::reset()
{
	buffer = nullptr;
	mappedBuffer = nullptr;
}

void MeshPrimitive::IndexBufferEntry::reset()
{
	buffer = nullptr;
	mappedBuffer = nullptr;
}

MeshPrimitive::MeshPrimitive()
	: m_resourceUsage(GraphicsResourceUsage::Static)
{
}

MeshPrimitive::~MeshPrimitive()
{
}

void MeshPrimitive::init()
{
	Object::init();
}

void MeshPrimitive::init(int vertexCount, int indexCount)
{
	init();
	m_vertexCount = vertexCount;
	m_indexCount = indexCount;
	m_indexFormat = GraphicsHelper::selectIndexBufferFormat(m_vertexCount);
}

void MeshPrimitive::init(int vertexCount, int indexCount, IndexBufferFormat indexFormat, GraphicsResourceUsage resourceUsage)
{
	init();
	m_vertexCount = vertexCount;
	m_indexCount = indexCount;
	m_indexFormat = indexFormat;
	m_resourceUsage = resourceUsage;
}

void MeshPrimitive::resetVertexBuffer(int vertexCount)
{
	m_mainVertexBuffer.reset();
	m_skinningVertexBuffer.reset();
	m_additionalUVVertexBuffer.reset();
	m_extraVertexBuffers.clear();
	m_vertexCount = vertexCount;
}

void MeshPrimitive::resetIndexBuffer(int indexCount, IndexBufferFormat indexFormat)
{
	m_indexBuffer.reset();
	m_indexCount = indexCount;
	m_indexFormat = indexFormat;
}

void MeshPrimitive::setVertex(int index, const Vertex& value)
{
	if (LN_REQUIRE(0 <= index && index < m_vertexCount)) return;
	reinterpret_cast<Vertex*>(acquireMappedVertexBuffer(InterleavedVertexGroup::Main))[index] = value;
}

const Vertex& MeshPrimitive::vertex(int index)
{
	if (LN_REQUIRE(0 <= index && index < m_vertexCount)) return Vertex::Default;
	return reinterpret_cast<Vertex*>(acquireMappedVertexBuffer(InterleavedVertexGroup::Main))[index];
}

void MeshPrimitive::setIndex(int index, int value)
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

int MeshPrimitive::index(int index)
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

void MeshPrimitive::addSection(int startIndex, int primitiveCount, int materialIndex, PrimitiveTopology topology)
{
	MeshSection2 meshSection;
	meshSection.startIndex = startIndex;
	meshSection.primitiveCount = primitiveCount;
	meshSection.materialIndex = materialIndex;
	meshSection.topology = topology;
	m_sections.add(meshSection);
}

void MeshPrimitive::setSection(int sectionIndex, int startIndex, int primitiveCount, int materialIndex, PrimitiveTopology topology)
{
	MeshSection2& meshSection = m_sections[sectionIndex];
	meshSection.startIndex = startIndex;
	meshSection.primitiveCount = primitiveCount;
	meshSection.materialIndex = materialIndex;
	meshSection.topology = topology;
}

void MeshPrimitive::commitRenderData(int sectionIndex, detail::MorphInstance* morph, MeshSection2* outSection, VertexLayout** outDecl, std::array<VertexBuffer*, 16>* outVBs, int* outVBCount, IndexBuffer** outIB)
{
	// unmap
	{
		if (m_mainVertexBuffer.mappedBuffer) {
			//m_mainVertexBuffer.buffer->unmap();
			m_mainVertexBuffer.mappedBuffer = nullptr;
		}
		if (m_skinningVertexBuffer.mappedBuffer) {
			//m_skinningVertexBuffer.buffer->unmap();
			m_skinningVertexBuffer.mappedBuffer = nullptr;
		}
		for (auto& v : m_extraVertexBuffers) {
			if (v.entry.mappedBuffer) {
				//v.entry.buffer->unmap();
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

	int streamIndex = 0;

	if (m_mainVertexBuffer.buffer) {
		(*outVBs)[streamIndex] = m_mainVertexBuffer.buffer;
		streamIndex++;
	}

	if (m_skinningVertexBuffer.buffer) {
		(*outVBs)[streamIndex] = m_skinningVertexBuffer.buffer;
		streamIndex++;
	}

#if 0
	if (morph) {
		for (int i = 0; i < MaxRenderMorphTargets; i++) {
			int targetIndex = morph->priorityTargets()[i];
			if (targetIndex >= 0) {
				const MorphVertexBuffers& b = m_morphVertexBuffers[targetIndex];
				if (b.positionVertexBuffer.buffer) {
					(*outVBs)[streamIndex] = b.positionVertexBuffer.buffer;
					streamIndex++;
				}
				if (b.normalVertexBuffer.buffer) {
					(*outVBs)[streamIndex] = b.normalVertexBuffer.buffer;
					streamIndex++;
				}
				if (b.tangentVertexBuffer.buffer) {
					(*outVBs)[streamIndex] = b.tangentVertexBuffer.buffer;
					streamIndex++;
				}
			}
		}
	}
#endif

	//for (int i = 0; i < m_morphVertexBuffers.size(); i++) {
	//	const MorphVertexBuffers& b = m_morphVertexBuffers[i];
	//	if (b.positionVertexBuffer.buffer) {
	//		(*outVBs)[streamIndex] = b.positionVertexBuffer.buffer;
	//		streamIndex++;
	//	}
	//	if (b.normalVertexBuffer.buffer) {
	//		(*outVBs)[streamIndex] = b.normalVertexBuffer.buffer;
	//		streamIndex++;
	//	}
	//	if (b.tangentVertexBuffer.buffer) {
	//		(*outVBs)[streamIndex] = b.tangentVertexBuffer.buffer;
	//		streamIndex++;
	//	}
	//}

    for (int i = 0; i < m_extraVertexBuffers.size(); i++) {
        (*outVBs)[streamIndex] = m_extraVertexBuffers[i].entry.buffer;
		streamIndex++;
    }

	*outVBCount = streamIndex;
}

void MeshPrimitive::commitMorphTargets(detail::MorphInstance* morph, std::array<VertexBuffer*, MaxRenderMorphTargets>* outTargets)
{
	//int index = 0;
	for (int i = 0; i < MaxRenderMorphTargets; i++) {
		(*outTargets)[i] = nullptr;

		if (i < m_morphVertexBuffers.size()) {
			int targetIndex = morph->priorityTargets()[i];
			if (targetIndex >= 0) {
				const VertexBufferEntry& e = m_morphVertexBuffers[i];
				if (e.buffer) {
					(*outTargets)[i] = e.buffer;
				}
			}
		}
	}
}

InterleavedVertexGroup MeshPrimitive::getStandardElement(VertexElementUsage usage, int usageIndex) const
{
	if (usageIndex == 0) {
		if (usage == VertexElementUsage::Position ||
			usage == VertexElementUsage::Normal ||
			usage == VertexElementUsage::Color ||
			usage == VertexElementUsage::TexCoord ||
			usage == VertexElementUsage::Tangent) {
			return InterleavedVertexGroup::Main;
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

void* MeshPrimitive::acquireMappedVertexBuffer(InterleavedVertexGroup group)
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
			m_mainVertexBuffer.buffer = makeObject_deprecated<VertexBuffer>(sizeof(Vertex) * m_vertexCount, m_resourceUsage);

			// set default
			auto* buf = static_cast<Vertex*>(m_mainVertexBuffer.buffer->writableData());
			for (int i = 0; i < m_vertexCount; i++) {
				buf[i].setNormal(Vector3(0, 0, 1));
				buf[i].color = Color::White;
			}
		}

		if (!m_mainVertexBuffer.mappedBuffer) {
			m_mainVertexBuffer.mappedBuffer = m_mainVertexBuffer.buffer->writableData();
		}
		return m_mainVertexBuffer.mappedBuffer;

	case InterleavedVertexGroup::Skinning:
		if (!m_skinningVertexBuffer.buffer) {
			m_skinningVertexBuffer.buffer = makeObject_deprecated<VertexBuffer>(sizeof(VertexBlendWeight) * m_vertexCount, m_resourceUsage);
		}

		if (!m_skinningVertexBuffer.mappedBuffer) {
			m_skinningVertexBuffer.mappedBuffer = m_skinningVertexBuffer.buffer->writableData();
		}
		return m_skinningVertexBuffer.mappedBuffer;

	case InterleavedVertexGroup::AdditionalUV:
		if (!m_additionalUVVertexBuffer.buffer) {
			m_additionalUVVertexBuffer.buffer = makeObject_deprecated<VertexBuffer>(sizeof(VertexAdditionalUV) * m_vertexCount, m_resourceUsage);
		}

		if (!m_additionalUVVertexBuffer.mappedBuffer) {
			m_additionalUVVertexBuffer.mappedBuffer = m_additionalUVVertexBuffer.buffer->writableData();
		}
		return m_additionalUVVertexBuffer.mappedBuffer;

	default:
		LN_UNREACHABLE();
		return nullptr;
	}
}

void* MeshPrimitive::acquireMappedVertexBuffer(VertexElementType type, VertexElementUsage usage, int usageIndex)
{
	auto group = getStandardElement(usage, usageIndex);
	if (group == InterleavedVertexGroup::Undefined) {
		VertexBuffer* vb = nullptr;
		auto r = m_extraVertexBuffers.findIf([&](auto& x) { return x.usage == usage && x.usageIndex == usageIndex; });
		if (!r) {
			auto vb = makeObject_deprecated<VertexBuffer>(GraphicsHelper::getVertexElementTypeSize(type) * m_vertexCount, m_resourceUsage);
			
			VertexBufferEntry entry;
			entry.buffer = vb;
			entry.mappedBuffer = vb->writableData();

			m_extraVertexBuffers.add({ type, usage, usageIndex, entry });
			m_vertexLayout = nullptr;	// dirty layout

			return entry.mappedBuffer;
		}
		else {
			if (r->type != type) {
				LN_ERROR();
				return nullptr;
			}

			if (!r->entry.mappedBuffer) {
				r->entry.mappedBuffer = r->entry.buffer->writableData();
			}
			return r->entry.mappedBuffer;
		}
	}
	else {
		return acquireMappedVertexBuffer(group);
	}
}

void* MeshPrimitive::acquireMappedIndexBuffer()
{
	if (!m_indexBuffer.buffer) {
		m_indexBuffer.buffer = makeObject_deprecated<IndexBuffer>(m_indexCount, m_indexFormat, m_resourceUsage);
	}

	//detail::GraphicsResourceInternal::selectIndexBufferFormat(indexCount)

	if (!m_indexBuffer.mappedBuffer) {
		m_indexBuffer.mappedBuffer = m_indexBuffer.buffer->map(MapMode::Write);
	}

	return m_indexBuffer.mappedBuffer;
}

VertexElementType MeshPrimitive::findVertexElementType(VertexElementUsage usage, int usageIndex) const
{
	// Find predefined vertex data.
	if (usageIndex == 0) {
		if (usage == VertexElementUsage::Position) return VertexElementType::Float3;
		if (usage == VertexElementUsage::Normal) return VertexElementType::Float3;
		if (usage == VertexElementUsage::TexCoord) return VertexElementType::Float2;
		if (usage == VertexElementUsage::Color) return VertexElementType::Float4;
		if (usage == VertexElementUsage::Tangent) return VertexElementType::Float4;
		//if (usage == VertexElementUsage::Binormal) return VertexElementType::Float3;
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
	if (r) return r->type;

	return VertexElementType::Unknown;
}

template<typename TIndex>
static void calculateTangentsInternal(int indexCount, const TIndex* indices, Vertex* vertices)
{
	// v0 について計算する
	auto calculateTangent = [](
		const Vector3& p0, const Vector2& uv0,
		const Vector3& p1, const Vector2& uv1,
		const Vector3& p2, const Vector2& uv2)
	{
		Vector3 deltaPos1 = p1 - p0;
		Vector3 deltaPos2 = p2 - p0;
		Vector2 deltaUV1 = uv1 - uv0;
		Vector2 deltaUV2 = uv2 - uv0;

		if (Vector3::nearEqual(deltaPos1, Vector3::Zero))
			return Vector3::UnitX;
		if (Vector3::nearEqual(deltaPos2, Vector3::Zero))
			return Vector3::UnitX;
		if (Vector2::nearEqual(deltaUV1, Vector2::Zero))
			return Vector3::UnitX;
		if (Vector2::nearEqual(deltaUV2, Vector2::Zero))
			return Vector3::UnitX;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		return Vector3::normalize((deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r);
	};

	for (int i = 0; i < indexCount; i += 3) {
		int i0 = indices[i + 0];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];
		const auto& v0 = vertices[i0];
		const auto& v1 = vertices[i1];
		const auto& v2 = vertices[i2];

#if 1
		{
			auto v = v0;
			v.tangent = Vector4(calculateTangent(v0.position.xyz(), v0.uv.xy(), v1.position.xyz(), v1.uv.xy(), v2.position.xyz(), v2.uv.xy()), 1.0f);
			vertices[i0] = v;
		}
		{
			auto v = v1;
			v.tangent = Vector4(calculateTangent(v1.position.xyz(), v1.uv.xy(), v2.position.xyz(), v2.uv.xy(), v0.position.xyz(), v0.uv.xy()), 1.0f);
			vertices[i1] = v;
		}
		{
			auto v = v2;
			v.tangent = Vector4(calculateTangent(v2.position.xyz(), v2.uv.xy(), v0.position.xyz(), v0.uv.xy(), v1.position.xyz(), v1.uv.xy()), 1.0f);
			vertices[i2] = v;
		}
#elif 0
		{
			auto v = v0;
			const auto t = calculateTangent(v0.position, v0.uv, v1.position, v1.uv, v2.position, v2.uv);
			v.tangent = Vector4((v.tangent.xyz() + t) / 2, 1.0f);
			//v.tangent = Vector4(1, 0, 0, 1);
			coreMesh->setVertex(i0, v);
		}
		{
			auto v = v1;
			const auto t = calculateTangent(v1.position, v1.uv, v2.position, v2.uv, v0.position, v0.uv);
			v.tangent = Vector4((v.tangent.xyz() + t) / 2, 1.0f);
			//v.tangent = Vector4(1, 0, 0, 1);
			coreMesh->setVertex(i1, v);
		}
		{
			auto v = v2;
			const auto t = calculateTangent(v2.position, v2.uv, v0.position, v0.uv, v1.position, v1.uv);
			v.tangent = Vector4((v.tangent.xyz() + t) / 2, 1.0f);
			//v.tangent = Vector4(1, 0, 0, 1);
			coreMesh->setVertex(i2, v);
		}

#else
		if (v0.tangent.xyz().lengthSquared() < 0.000001f) {
			auto v = v0;
			v.tangent = Vector4(calculateTangent(v0.position, v0.uv, v1.position, v1.uv, v2.position, v2.uv), 1.0f);
			coreMesh->setVertex(i0, v);
		}
		if (v1.tangent.xyz().lengthSquared() < 0.000001f) {
			auto v = v1;
			v.tangent = Vector4(calculateTangent(v1.position, v1.uv, v2.position, v2.uv, v0.position, v0.uv), 1.0f);
			coreMesh->setVertex(i1, v);
		}
		if (v2.tangent.xyz().lengthSquared() < 0.000001f) {
			auto v = v2;
			v.tangent = Vector4(calculateTangent(v2.position, v2.uv, v0.position, v0.uv, v1.position, v1.uv), 1.0f);
			coreMesh->setVertex(i2, v);
		}
#endif

		/*
		Vector3 deltaPos1 = v1.position - v0.position;
		Vector3 deltaPos2 = v2.position - v0.position;
		Vector2 deltaUV1 = v1.uv - v0.uv;
		Vector2 deltaUV2 = v2.uv - v0.uv;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		Vector3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		//glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
		*/

		//v0.tangent = Vector4(Vector3::normalize(tangent), 1.0f);
		//coreMesh->setVertex(i0, v0);


		//v.position.y = 0;
		//coreMesh->setVertex(vi, v);
		//printf("");
	}
}

// TODO: 未テスト。Blender が接線をエクスポートしない問題の暫定対策として用意しているが、
// ちゃんと公開する前にはテストすること。
// https://github.com/KhronosGroup/glTF-Blender-IO/issues/172
void MeshPrimitive::calculateTangents()
{
	if (LN_REQUIRE(isAllTriangleLists())) return;

	Vertex* vertices = static_cast<Vertex*>(acquireMappedVertexBuffer(InterleavedVertexGroup::Main));
	if (indexBufferFormat() == IndexBufferFormat::UInt16) {
		uint16_t* indices = static_cast<uint16_t*>(acquireMappedIndexBuffer());
		calculateTangentsInternal(indexCount(), indices, vertices);
	}
	else if (indexBufferFormat() == IndexBufferFormat::UInt32) {
		uint32_t* indices = static_cast<uint32_t*>(acquireMappedIndexBuffer());
		calculateTangentsInternal(indexCount(), indices, vertices);
	}
	else {
		LN_NOTIMPLEMENTED();
		return;
	}
}

void MeshPrimitive::calculateBoundingBox()
{
	m_boundingBox = AABB();
	const Vertex* vertices = static_cast<const Vertex*>(m_mainVertexBuffer.buffer->data());
	for (int i = 0; i < m_vertexCount; i++) {
		m_boundingBox.attemptInfrate(vertices[i].position.xyz());
	}
}

VertexBuffer* MeshPrimitive::vertexBuffer(InterleavedVertexGroup group) const
{
	switch (group)
	{
	case ln::InterleavedVertexGroup::Main:
		return m_mainVertexBuffer.buffer;
	case ln::InterleavedVertexGroup::Skinning:
		return m_skinningVertexBuffer.buffer;
	case ln::InterleavedVertexGroup::AdditionalUV:
		return m_additionalUVVertexBuffer.buffer;
	default:
		LN_UNREACHABLE();
		return nullptr;
	}
}

IndexBuffer* MeshPrimitive::indexBuffer() const
{
	return m_indexBuffer.buffer;
}

// NOTE: MorphVertexBuffers で１つのターゲットメッシュを表すイメージ。
// VRM モデルだと表情の数だけ作られる。
void* MeshPrimitive::acquireMappedMorphVertexBuffer(int morphTargetIndex/*, VertexElementUsage usage*/)
{
	// Grow
	if (morphTargetIndex >= m_morphVertexBuffers.size()) {
		m_morphVertexBuffers.resize(morphTargetIndex + 1);
	}

	//MorphVertexBuffers& b = m_morphVertexBuffers[morphTargetIndex];
	//VertexBufferEntry* e;
	//if (usage == VertexElementUsage::Position)
	//	e = &b.positionVertexBuffer;
	//else if (usage == VertexElementUsage::Normal)
	//	e = &b.normalVertexBuffer;
	//else if (usage == VertexElementUsage::Tangent)
	//	e = &b.tangentVertexBuffer;
	//else {
	//	LN_ERROR();
	//	return nullptr;
	//}
		
	VertexBufferEntry* e = &m_morphVertexBuffers[morphTargetIndex];
	if (!e->buffer) {
		e->buffer = makeObject_deprecated<VertexBuffer>(sizeof(Vertex) * m_vertexCount, m_resourceUsage);
	}

	if (!e->mappedBuffer) {
		e->mappedBuffer = e->buffer->writableData();
	}

	return static_cast<Vertex*>(e->mappedBuffer);
}

void MeshPrimitive::attemptResetVertexLayout()
{
	if (!m_vertexLayout) {
		m_vertexLayout = makeObject_deprecated<VertexLayout>();

		int streamIndex = 0;

		if (m_mainVertexBuffer.buffer) {
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Position, 0);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Normal, 0);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::TexCoord, 0);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Color, 0);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Tangent, 0);
			streamIndex++;
		}

		if (m_skinningVertexBuffer.buffer) {
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::BlendIndices, 0);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::BlendWeight, 0);
			streamIndex++;
		}

		// NOTE: DX12 は Shader 側で要求されている Attribute の分はかならず VertexBuffer を Stream へセットしなければならない (NULL 禁止) ため、
		// もし頂点シェーダでモーフィングしたければ、モーフターゲットが 0個, 1個, 2個… というように個別にコンパイルしなければならなくなる。
		// 管理が非常に大変になるため却下。
#if 0
		// MorphTarget は最大 MaxRenderMorphTargets 個までを詰め込む
		for (int i = 0; i < MaxRenderMorphTargets; i++) {
			if (m_morphVertexBuffers.isValidRange(i)) {
				const MorphVertexBuffers& b = m_morphVertexBuffers[i];
				if (b.positionVertexBuffer.buffer) {
					m_vertexLayout->addElement(streamIndex, VertexElementType::Float3, VertexElementUsage::Position, 1 + i);
					streamIndex++;
				}
				if (b.normalVertexBuffer.buffer) {
					m_vertexLayout->addElement(streamIndex, VertexElementType::Float3, VertexElementUsage::Normal, 1 + i);
					streamIndex++;
				}
				if (b.tangentVertexBuffer.buffer) {
					m_vertexLayout->addElement(streamIndex, VertexElementType::Float3, VertexElementUsage::Tangent, 1 + i);
					streamIndex++;
				}
			}
		}
#endif

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
	, m_visible(true)
{
    //m_lodMesh.resize(1);
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
//void MeshContainer::calculateBounds()
//{
//#if 1
//	m_boundingBox = Box();
//#else
//	MeshResource* mesh = meshResource();
//	if (mesh)
//	{
//		VertexBuffer* vertexBuffer;
//		mesh->requestBuffers(MeshResource::VBG_Basic, &vertexBuffer, nullptr);
//		Vertex* data = (Vertex*)vertexBuffer->map(MapMode::Read);
//
//		int count = mesh->vertexCount();
//		if (count > 0)
//		{
//			Vector3 min = data[0].position;
//			Vector3 max = data[0].position;
//			for (int i = 1; i < count; i++)
//			{
//				if (data[i].position.x < min.x)
//					min.x = data[i].position.x;
//
//				if (data[i].position.y < min.y)
//					min.y = data[i].position.y;
//
//				if (data[i].position.z < min.z)
//					min.z = data[i].position.z;
//
//				if (data[i].position.x > max.x)
//					max.x = data[i].position.x;
//
//				if (data[i].position.y > max.y)
//					max.y = data[i].position.y;
//
//				if (data[i].position.z > max.z)
//					max.z = data[i].position.z;
//			}
//
//			m_boundingBox = Box(min, max);
//		}
//		else
//		{
//			m_boundingBox = Box();
//		}
//	}
//	else
//	{
//		m_boundingBox = Box();
//	}
//#endif
//}

void MeshContainer::addMeshPrimitive(MeshPrimitive* mesh)
{
	m_lodMesh.add(mesh);
}

//void MeshContainer::setMesh(MeshPrimitive* mesh)
//{
//	//if (LN_REQUIRE(mesh)) return;
//	//if (LN_REQUIRE(!mesh->m_owner)) return;
//    m_lodMesh[0] = mesh;
//	//mesh->m_owner = this;
//}
//
//MeshPrimitive* MeshContainer::mesh() const
//{
//    return m_lodMesh[0];
//}
//==============================================================================
// MeshNode

LN_OBJECT_IMPLEMENT(MeshNode, Object) {}

MeshNode::MeshNode()
    : m_model(nullptr)
	, m_index(-1)
    , m_meshContainerIndex(-1)
	, m_parent(-1)
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

void MeshNode::setTransform(const AttitudeTransform& value)
{
	m_localTransform = value;
}

void MeshNode::setMeshContainerIndex(int value)
{
    m_meshContainerIndex = value;
}

void MeshNode::addChildIndex(int value)
{
    m_children.add(value);
	m_model->m_nodes[value]->m_parent = m_index;
}

void MeshNode::setInitialLocalTransform(const Matrix& value)
{
	m_initialLocalTransform = value;
}

void MeshNode::resetLocalTransform()
{
	m_localTransform = AttitudeTransform::Identity;
}

const Matrix& MeshNode::globalMatrix() const
{
	return m_model->m_nodeGlobalTransforms[m_index];
}

void MeshNode::setGlobalTransform(const Matrix& value)
{
	m_model->m_nodeGlobalTransforms[m_index] = value;
}

void MeshNode::updateGlobalTransform(bool hierarchical)
{
	m_model->updateNodeTransformsHierarchical(m_index, m_model->m_nodes[m_parent]->globalMatrix(), hierarchical);
}

} // namespace ln

