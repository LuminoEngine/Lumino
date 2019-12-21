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
{
}

Mesh::~Mesh()
{
}

void Mesh::init()
{
	Object::init();
}

void Mesh::init(const MeshView& meshView)
{
	init();

    m_vertexLayout = makeObject<VertexLayout>();

    // Validation and count vertices
    int vertexCount = 0;
	for (auto& section : meshView.sectionViews) {
		int count = section.vertexBufferViews[0].count;
		for (size_t i = 0; i < section.vertexBufferViews.size(); i++) {
            auto& view = section.vertexBufferViews[i];
			if (count != view.count) {
				// ひとつの section 内ではすべての頂点数が一致していることが前提
				LN_ERROR();
				return;
			}

            auto r = m_vertexBuffers.findIf([&](auto& x) { return x.type == view.type && x.usage == view.usage; });
            if (!r) {
                m_vertexBuffers.add({ view.type, view.usage, view.usageIndex, nullptr });
            }
		}
        vertexCount += count;
	}

    // Allocate vertex buffers
    for (int i = 0; i < m_vertexBuffers.size(); i++) {
        auto& attr = m_vertexBuffers[i];
        attr.buffer = makeObject<VertexBuffer>(vertexCount * GraphicsHelper::getVertexElementTypeSize(attr.type), GraphicsResourceUsage::Static);
    }

    int vertexOffset = 0;
    for (auto& section : meshView.sectionViews) {
        int vertexCountInSection = section.vertexBufferViews[0].count;

        for (auto& vbView : section.vertexBufferViews) {
            auto attr = m_vertexBuffers.findIf([&](auto& x) { return x.type == vbView.type && x.usage == vbView.usage; });
            auto* buf = static_cast<byte_t*>(attr->buffer->map(MapMode::Write));
            auto* src = static_cast<const byte_t*>(vbView.data);// +vbView.byteOffset;

            int size = GraphicsHelper::getVertexElementTypeSize(vbView.type);
            for (int i = 0; i < vertexCountInSection; i++) {
                memcpy(&buf[(vertexOffset + i) * size], src + (vbView.byteStride * i), size);
                //memcpy(&buf[(vertexOffset + i) * size], src + (vbView.byteStride * i), size);
            }

            // TODO: unmap 無いとめんどい以前に怖い
        }

        vertexOffset += vertexCountInSection;
    }

    // 
    {
        auto r = m_vertexBuffers.findIf([&](auto& x) { return x.usage == VertexElementUsage::Color; });
        if (!r) {
            VertexBufferAttribute attr;
            attr.type = VertexElementType::Float4;
            attr.usage = VertexElementUsage::Color;
            attr.usageIndex = 0;
            attr.buffer = makeObject<VertexBuffer>(vertexCount * sizeof(Vector4), GraphicsResourceUsage::Static);
            m_vertexBuffers.add(attr);
            
            auto* buf = static_cast<Vector4*>(attr.buffer->map(MapMode::Write));
            for (int i = 0; i < vertexCount; i++) {
                buf[i] = Vector4(1, 1, 1, 1);
            }
        }
    }

    //VertexBufferAttribute posa = m_vertexBuffers[1];
    //m_vertexBuffers.removeAt(1);
    //m_vertexBuffers.insert(0, posa);


    for (int i = 0; i < m_vertexBuffers.size(); i++) {
        auto& attr = m_vertexBuffers[i];
        m_vertexLayout->addElement(i, attr.type, attr.usage, attr.usageIndex);
    }

    // Index buffer.
    //   Lumino の MeshSection は glTF の Primitive と対応させているが、glTF の Primitive は様々な Index buffer を参照することができる。
    //   MeshSection で扱うにはそれらすべてをひとつの Index buffer に統合する必要がある。
    {
        // 全ての index buffer を調べて、一番大きい Format でバッファ確保
        int indexElementSize = 0;
        int indexCount = 0;
        for (auto& section : meshView.sectionViews) {
            indexElementSize = std::max(indexElementSize, section.indexElementSize);
            indexCount += section.indexCount;
        }
        if (indexElementSize == 1 || indexElementSize == 2) {
            m_indexBuffer = makeObject<IndexBuffer>(indexCount, IndexBufferFormat::UInt16, GraphicsResourceUsage::Static);
            indexElementSize = 2;
        }
        else if (indexElementSize == 4) {
            m_indexBuffer = makeObject<IndexBuffer>(indexCount, IndexBufferFormat::UInt32, GraphicsResourceUsage::Static);
        }
        else {
            LN_NOTIMPLEMENTED();
        }

        void* buf = m_indexBuffer->map(MapMode::Write);

        int beginVertexIndex = 0;
        int indexOffset = 0;
        for (auto& section : meshView.sectionViews) {
            int vertexCountInSection = section.vertexBufferViews[0].count;

            if (indexElementSize == 2) {
                if (section.indexElementSize == 1) {
                    auto* b = static_cast<uint16_t*>(buf) + indexOffset;
                    auto* s = static_cast<const uint8_t*>(section.indexData);
                    for (int i = 0; i < section.indexCount; i++) {
                        b[i] = beginVertexIndex + s[i];
                        assert(b[i] < vertexCount);
                    }
                }
                else if (section.indexElementSize == 2) {
                    auto* b = static_cast<uint16_t*>(buf) + indexOffset;
                    auto* s = static_cast<const uint16_t*>(section.indexData);
                    for (int i = 0; i < section.indexCount; i++) {
                        b[i] = beginVertexIndex + s[i];
                        assert(b[i] < vertexCount);
                    }
                }
                else if (section.indexElementSize == 4) {
                }
                else {
                    LN_NOTIMPLEMENTED();
                }
            }
            else if (indexElementSize == 4) {
                LN_NOTIMPLEMENTED();
            }
            else {
                LN_NOTIMPLEMENTED();
            }

            assert(section.topology == PrimitiveTopology::TriangleList);

            // make mesh section
            MeshSection2 meshSection;
            meshSection.startIndex = indexOffset;
            meshSection.primitiveCount = section.indexCount / 3;    // TODO: tri only
            meshSection.materialIndex = section.materialIndex;
            meshSection.topology = section.topology;
            m_sections.add(meshSection);

            // next
            indexOffset += section.indexCount;
            beginVertexIndex += vertexCountInSection;
        }

        // TODO: unmap 無いとめんどい以前に怖い

    }
    

}

void Mesh::commitRenderData(int sectionIndex, MeshSection2* outSection, VertexLayout** outDecl, std::array<VertexBuffer*, 16>* outVBs, int* outVBCount, IndexBuffer** outIB)
{
    *outSection = m_sections[sectionIndex];
    *outDecl = m_vertexLayout;
    *outIB = m_indexBuffer;
    *outVBCount = m_vertexBuffers.size();

    for (int i = 0; i < m_vertexBuffers.size(); i++) {
        (*outVBs)[i] = m_vertexBuffers[i].buffer;
    }
}

//==============================================================================
// MeshContainer

MeshContainer::MeshContainer()
	: m_name()
	, m_lodResources()
{
    m_lodMesh.resize(1);
}

MeshContainer::~MeshContainer()
{
}

void MeshContainer::init()
{
	Object::init();
	m_lodResources.resize(1);
}

void MeshContainer::setMeshResource(MeshResource* mesh)
{
    if (LN_REQUIRE(!mesh->m_ownerContainer)) return;
    m_lodResources[0] = mesh;
    mesh->m_ownerContainer = this;
}

MeshResource* MeshContainer::meshResource() const
{
	return m_lodResources[0];
}

MeshResource* MeshContainer::selectLODResource(float distance) const
{
	// TODO:
	return m_lodResources[0];
}

void MeshContainer::calculateBounds()
{
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
}

void MeshContainer::setMesh(Mesh* mesh)
{
    m_lodMesh[0] = mesh;
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
    , m_localTransform(Matrix::Identity)
{
}

void MeshNode::setMeshContainerIndex(int value)
{
    m_meshContainerIndex = value;
}

void MeshNode::addChildIndex(int value)
{
    m_children.add(value);
}

void MeshNode::setLocalTransform(const Matrix& value)
{
    m_localTransform = value;
}

//==============================================================================
// StaticMeshModel

StaticMeshModel::StaticMeshModel()
    : m_type(detail::InternalMeshModelType::StaticMesh)
{
}

StaticMeshModel::StaticMeshModel(detail::InternalMeshModelType type)
    : m_type(type)
{
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

void StaticMeshModel::addMaterial(AbstractMaterial* material)
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
    m_nodeGlobalTransforms[nodeIndex] = node->localTransform() * parentTransform;   // NOTE: glTF はこの順である必要がある。

    for (int child : node->m_children) {
        updateNodeTransformsHierarchical(child, m_nodeGlobalTransforms[nodeIndex]);
    }
}

namespace detail {


} // namespace detail
} // namespace ln

