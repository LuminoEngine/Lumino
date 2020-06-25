
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

void MeshGeometryBuilder::setTransform(const AttitudeTransform& value)
{
	m_transform = value;
}

void MeshGeometryBuilder::setPosition(const Vector3& value)
{
	m_transform.translation = value;
}

void MeshGeometryBuilder::setRotation(const Vector3& value)
{
	m_transform.rotation = Quaternion::makeFromEulerAngles(value);
}

void MeshGeometryBuilder::setScale(const Vector3& value)
{
	m_transform.scale = value;
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

void MeshGeometryBuilder::addSphere(float radius, int sliceH, int sliceV)
{
	auto* g = newMeshGenerater<detail::RegularSphereMeshFactory>();
	g->m_radius = radius;
	g->m_slices = sliceH;
	g->m_stacks = sliceV;
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
	int indexStride = GraphicsHelper::getIndexStride(mesh->indexBufferFormat());
	for (const auto& section : m_currentMeshSections) {
		int sectionIndexOffset = indexOffset;

		for (int i = section.startGenerator; i < section.startGenerator + section.generatorCount; i++) {
			const auto& g = m_generators[i];
			//int vertexCount = g.generator->vertexCount();
			detail::MeshGeneraterBuffer genBuffer(m_allocator);
			genBuffer.setBuffer(
				static_cast<Vertex*>(mappedVB) + vertexOffset,
				static_cast<byte_t*>(mappedIB) + indexOffset * indexStride,
				mesh->indexBufferFormat(), vertexOffset);
			genBuffer.generate(g.generator);
			//genBuffer.transform(g.generator, vertexCount);

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

