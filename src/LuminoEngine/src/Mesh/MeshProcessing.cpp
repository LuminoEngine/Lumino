
#include "Internal.hpp"
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Mesh/MeshProcessing.hpp>
#include "MeshManager.hpp"
#include "MeshGenerater.hpp"

namespace ln {

//==============================================================================
// MeshGeometryBuilder

MeshGeometryBuilder::MeshGeometryBuilder()
	//: m_transform(nullptr)
{
}

MeshGeometryBuilder::~MeshGeometryBuilder()
{
	if (m_allocator) {
		m_allocator->cleanup();
		m_allocator = nullptr;
	}
}

void MeshGeometryBuilder::init()
{
	Object::init();
	m_allocator = makeRef<detail::LinearAllocator>(detail::EngineDomain::meshManager()->linearAllocatorPageManager());
	//allocateNextTransform();
	m_color = Color::White;
}

void MeshGeometryBuilder::setTransform(const Matrix& value)
{
	m_transform = value;
}

void MeshGeometryBuilder::setColor(const Color& value)
{
	m_color = value;
}

void MeshGeometryBuilder::beginSection()
{
	m_currentMeshSections.add({ m_generators.size(), 0 });
}

void MeshGeometryBuilder::addPlane(const Vector2& size, int sliceH, int sliceV)
{
	auto* g = newMeshGenerater<detail::PlaneMeshGenerater2>();
	g->size = size;
	g->sliceH = sliceH;
	g->sliceV = sliceV;
}

void MeshGeometryBuilder::addBox(const Vector3& sizes)
{
	auto* g = newMeshGenerater<detail::RegularBoxMeshFactory>();
	g->m_size = sizes;
}

void MeshGeometryBuilder::endSection()
{
	Section& s = m_currentMeshSections.back();
	s.generatorCount = m_generators.size() - s.startGenerator;
}

Ref<Mesh> MeshGeometryBuilder::buildMesh()
{
	int vertexCount = 0;
	int indexCount = 0;
	for (const auto& section : m_currentMeshSections) {
		//const Section& s = m_currentMeshSections.back();
		for (int i = section.startGenerator; i < section.startGenerator + section.generatorCount; i++) {
			const auto& g = m_generators[i];
			vertexCount += g.generator->vertexCount();
			indexCount += g.generator->indexCount();
		}
	}

	auto mesh = makeObject<Mesh>(vertexCount, indexCount);

	auto* mappedVB = mesh->acquireMappedVertexBuffer(InterleavedVertexGroup::Main);
	auto* mappedIB = mesh->acquireMappedIndexBuffer();

	int vertexOffset = 0;
	int indexOffset = 0;
	int sectionCount = 0;
	for (const auto& section : m_currentMeshSections) {
		int sectionIndexOffset = indexOffset;

		for (int i = section.startGenerator; i < section.startGenerator + section.generatorCount; i++) {
			const auto& g = m_generators[i];
			detail::MeshGeneraterBuffer genBuffer(nullptr);	// TODO: allocator
			genBuffer.setBuffer(static_cast<Vertex*>(mappedVB) + vertexOffset, mappedIB, mesh->indexBufferFormat(), indexOffset);
			genBuffer.generate(g.generator);

			//int indexCount = g.generator->indexCount();

			vertexOffset += g.generator->vertexCount();
			indexOffset += g.generator->indexCount();

			if (g.generator->primitiveType() != PrimitiveTopology::TriangleList) {
				LN_NOTIMPLEMENTED();
				return nullptr;
			}
		}


		mesh->addSection(sectionIndexOffset, (indexOffset - sectionIndexOffset) / 3, sectionCount, PrimitiveTopology::TriangleList);
		sectionCount++;
	}

	return mesh;
}
//
//void MeshGeometryBuilder::allocateNextTransform()
//{
//	m_transform = static_cast<Matrix*>(m_allocator->allocate(sizeof(Matrix)));
//}
//
} // namespace ln

