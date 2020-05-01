
#include "Internal.hpp"
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Scene/Mesh/StaticMeshComponent.hpp>
#include <LuminoEngine/Scene/Mesh/StaticMesh.hpp>
#include "../../Mesh/MeshManager.hpp"

namespace ln {

//==============================================================================
// StaticMesh

Ref<StaticMesh> StaticMesh::create()
{
    return makeObject<StaticMesh>();
}

Ref<StaticMesh> StaticMesh::create(const StringRef& filePath, float scale)
{
    return makeObject<StaticMesh>(filePath, scale);
}

StaticMesh::StaticMesh()
{
}

StaticMesh::~StaticMesh()
{
}

void StaticMesh::init()
{
    VisualObject::init();
    m_component = makeObject<StaticMeshComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

void StaticMesh::init(const StringRef& filePath, float scale)
{
    init();
    m_component->setModel(detail::EngineDomain::meshManager()->createStaticMeshModel(filePath, scale));
}

StaticMeshComponent* StaticMesh::staticMeshComponent() const
{
    return m_component;
}

} // namespace ln

