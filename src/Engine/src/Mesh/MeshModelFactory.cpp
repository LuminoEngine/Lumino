
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Mesh/MeshModelFactory.hpp>
#include "../Scene/SceneManager.hpp"
#include "MeshGeneraters/MeshGenerater.hpp"
#include "MeshGeneraters/TeapotMeshGenerater.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// MeshModelFactory

Ref<MeshModel> MeshModelFactory::createBox(const Vector3& size, Material* material)
{
	RegularBoxMeshFactory g;
	g.setColor(Color::White);
	g.m_size = size;
	return createMesh(&g, material);
}

Ref<MeshModel> MeshModelFactory::createCone(float radius, float height, float segments, Material* material)
{
	ConeMeshFactory g;
	g.init(radius, height, segments);
	g.setColor(Color::White);
	return createMesh(&g, material);
}

Ref<MeshModel> MeshModelFactory::createCylinder(float radiusTop, float radiusBottom, float height, float segments, Material* material)
{
	CylinderMeshFactory g;
	g.init(radiusTop, radiusBottom, height, segments, 1);
	g.setColor(Color::White);
	return createMesh(&g, material);
}

Ref<MeshModel> MeshModelFactory::createTeapot(Material* material)
{
	TeapotMeshGenerater g;
	g.m_size = 1.0f;
	g.m_tessellation = 8;
	g.setColor(Color::White);
	return createMesh(&g, material);
}

Ref<MeshModel> MeshModelFactory::createMesh(MeshGenerater* factory, Material* material)
{
	Ref<MeshPrimitive> mesh = makeObject<MeshPrimitive>(factory->vertexCount(), factory->indexCount());

	MeshGeneraterBuffer buffer(nullptr);
	buffer.setBuffer(static_cast<Vertex*>(mesh->acquireMappedVertexBuffer(InterleavedVertexGroup::Main)), mesh->acquireMappedIndexBuffer(), mesh->indexBufferFormat(), 0);
	buffer.generate(factory);

	mesh->addSection(0, factory->indexCount() / 3, 0, factory->primitiveType());	// TODO: primitiveCount by toporogy

	Ref<MeshModel> model = makeObject<MeshModel>();
	MeshNode* node = model->addMeshContainerNode(mesh);
	model->addMaterial(material ? material : detail::EngineDomain::sceneManager()->primitiveMeshDefaultMaterial());

	model->calculateBoundingBox();

	return model;
}

} // namespace detail
} // namespace ln

