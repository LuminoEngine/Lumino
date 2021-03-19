
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Mesh/MeshModelFactory.hpp>
#include "../Scene/SceneManager.hpp"
#include "MeshGenerater.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// MeshModelFactory

Ref<MeshModel> MeshModelFactory::createBox(const Vector3& size, Material* material)
{
	RegularBoxMeshFactory factory;
	factory.setColor(Color::White);
	factory.m_size = size;
	return createMesh(&factory, material);

	//Ref<MeshPrimitive> mesh = makeObject<MeshPrimitive>(factory.vertexCount(), factory.indexCount());

	//MeshGeneraterBuffer buffer(nullptr);
	//buffer.setBuffer(static_cast<Vertex*>(mesh->acquireMappedVertexBuffer(InterleavedVertexGroup::Main)), mesh->acquireMappedIndexBuffer(), mesh->indexBufferFormat(), 0);
	//buffer.generate(&factory);

	//mesh->addSection(0, factory.indexCount() / 3, 0, factory.primitiveType());

	//Ref<MeshModel> model = makeObject<MeshModel>();
	//MeshNode* node = model->addMeshContainerNode(mesh);
	//model->addMaterial(material ? material : detail::EngineDomain::sceneManager()->primitiveMeshDefaultMaterial());

	//return model;
}

Ref<MeshModel> MeshModelFactory::createCone(float radius, float height, float segments, Material* material)
{
	ConeMeshFactory factory;
	factory.init(radius, height, segments);
	factory.setColor(Color::White);
	return createMesh(&factory, material);
}

Ref<MeshModel> MeshModelFactory::createCylinder(float radiusTop, float radiusBottom, float height, float segments, Material* material)
{
	CylinderMeshFactory factory;
	factory.init(radiusTop, radiusBottom, height, segments, 1);
	factory.setColor(Color::White);
	return createMesh(&factory, material);
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

