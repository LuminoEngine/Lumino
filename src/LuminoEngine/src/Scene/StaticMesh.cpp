
#include "Internal.hpp"
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Visual/StaticMeshComponent.hpp>
#include <LuminoEngine/Scene/StaticMesh.hpp>
#include "../Mesh/MeshManager.hpp"

namespace ln {

//==============================================================================
// StaticMesh

Ref<StaticMesh> StaticMesh::create()
{
    return newObject<StaticMesh>();
}

Ref<StaticMesh> StaticMesh::create(const StringRef& filePath, float scale)
{
    return newObject<StaticMesh>(filePath, scale);
}

StaticMesh::StaticMesh()
{
}

StaticMesh::~StaticMesh()
{
}

void StaticMesh::initialize()
{
    VisualObject::initialize();
    m_component = newObject<StaticMeshComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

void StaticMesh::initialize(const StringRef& filePath, float scale)
{
    initialize();
    m_component->setModel(detail::EngineDomain::meshManager()->createStaticMeshModel(filePath, scale));
}

StaticMeshComponent* StaticMesh::staticMeshComponent() const
{
    return m_component;
}

} // namespace ln

