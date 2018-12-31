
#include "Internal.hpp"
#include <LuminoEngine/Visual/StaticMeshComponent.hpp>
#include <LuminoEngine/Scene/StaticMesh.hpp>

namespace ln {

//==============================================================================
// StaticMesh

Ref<StaticMesh> StaticMesh::create()
{
    return newObject<StaticMesh>();
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

StaticMeshComponent* StaticMesh::staticMeshComponent() const
{
    return m_component;
}

} // namespace ln

