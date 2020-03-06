
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
}

void MeshGeometryBuilder::beginSection()
{
	m_currentMeshSections.add({ m_generators.size(), 0 });
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
	const Section& s = m_currentMeshSections.back();
	int vertexCount = 0;
	int indexCount = 0;
	for (int i = s.startGenerator; i < s.startGenerator + s.generatorCount; i++) {
		auto* g = m_generators[i];
		vertexCount += g->vertexCount();
		indexCount += g->indexCount();
	}

	auto mesh = makeObject<Mesh>(vertexCount, indexCount);

	auto* mappedVB = mesh->acquireMappedVertexBuffer(InterleavedVertexGroup::Main);
	auto* mappedIB = mesh->acquireMappedIndexBuffer();

	int vertexOffset = 0;
	int indexOffset = 0;
	for (int i = s.startGenerator; i < s.startGenerator + s.generatorCount; i++) {
		auto* g = m_generators[i];
		detail::MeshGeneraterBuffer genBuffer(nullptr);	// TODO: allocator
		genBuffer.setBuffer(static_cast<Vertex*>(mappedVB) + vertexOffset, mappedIB, mesh->indexBufferFormat(), indexOffset);
		genBuffer.generate(g);
		vertexOffset += g->vertexCount();
		indexOffset += g->indexCount();
	}

	return mesh;
}

} // namespace ln

