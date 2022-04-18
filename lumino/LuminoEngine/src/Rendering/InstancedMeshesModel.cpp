
#include "Internal.hpp"
#include <LuminoGraphics/RHI/VertexLayout.hpp>
#include <LuminoGraphics/RHI/VertexBuffer.hpp>
#include <LuminoEngine/Mesh/MeshPrimitive.hpp>
#include <LuminoEngine/Rendering/InstancedMeshesModel.hpp>

namespace ln {
	
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

bool InstancedMeshList::init(MeshPrimitive* mesh, int sectionIndex)
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
	m_stagingData.transform0 = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	m_stagingData.transform1 = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	m_stagingData.transform2 = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	m_stagingData.transform3 = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_stagingData.uvOffset = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	m_stagingData.colorScale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

void InstancedMeshList::setTransform(const Matrix& transform)
{
	m_stagingData.transform0 = transform.row(0);
	m_stagingData.transform1 = transform.row(1);
	m_stagingData.transform2 = transform.row(2);
	m_stagingData.transform3 = transform.row(3);
}

void InstancedMeshList::setColorScale(const Color& value)
{
	m_stagingData.colorScale = value.toVector4();
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
	m_mesh->commitRenderData(m_sectionIndex, nullptr, outSection, outDecl, outVBs, outVBCount, outIB);
	
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
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Color, 1, VertexInputRate::Instance);
		}

		if (!m_instanceBuffer || (m_instanceBuffer->size() / sizeof(InstanceData)) < m_instanceCount) {
			m_instanceBuffer = makeObject<VertexBuffer>(m_instanceCount * sizeof(InstanceData), GraphicsResourceUsage::Dynamic);
		}

		void* data = m_instanceBuffer->writableData();
		memcpy(data, m_instanceData.data(), m_instanceCount * sizeof(InstanceData));

		m_dirty = false;
	}

	(*outDecl) = m_vertexLayout;
	(*outVBs)[*outVBCount] = m_instanceBuffer;
	(*outVBCount)++;
}


} // namespace ln

