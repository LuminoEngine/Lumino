
#include "Internal.hpp"
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Scene/Shapes/MeshPrimitiveComponent.hpp>
#include <LuminoEngine/Scene/Shapes/MeshPrimitives.hpp>
#include "../../Mesh/MeshManager.hpp"

namespace ln {

//==============================================================================
// BoxMesh

Ref<BoxMesh> BoxMesh::create()
{
    return makeObject<BoxMesh>();
}

//Ref<BoxMesh> BoxMesh::create(const StringRef& filePath, float scale)
//{
//    return makeObject<BoxMesh>(filePath, scale);
//}

BoxMesh::BoxMesh()
{
}

BoxMesh::~BoxMesh()
{
}

void BoxMesh::init()
{
    VisualObject::init();
    m_component = makeObject<BoxMeshComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

BoxMeshComponent* BoxMesh::boxMeshComponent() const
{
    return m_component;
}

//==============================================================================
// SphereMesh

Ref<SphereMesh> SphereMesh::create()
{
	return makeObject<SphereMesh>();
}

SphereMesh::SphereMesh()
{
}

void SphereMesh::init()
{
	VisualObject::init();
	m_component = makeObject<SphereMeshComponent>();
	addComponent(m_component);
	setMainVisualComponent(m_component);
}

SphereMeshComponent* SphereMesh::sphereMeshComponent() const
{
	return m_component;
}

} // namespace ln

