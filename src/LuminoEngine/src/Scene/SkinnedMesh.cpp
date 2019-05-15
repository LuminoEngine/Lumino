
#include "Internal.hpp"
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include <LuminoEngine/Visual/SkinnedMeshComponent.hpp>
#include <LuminoEngine/Scene/SkinnedMesh.hpp>
#include "../Mesh/MeshManager.hpp"

namespace ln {

//==============================================================================
// SkinnedMesh

Ref<SkinnedMesh> SkinnedMesh::create()
{
    return makeObject<SkinnedMesh>();
}

Ref<SkinnedMesh> SkinnedMesh::create(const StringRef& filePath, float scale)
{
    return makeObject<SkinnedMesh>(filePath, scale);
}

SkinnedMesh::SkinnedMesh()
{
}

SkinnedMesh::~SkinnedMesh()
{
}

void SkinnedMesh::init()
{
    VisualObject::init();
    m_component = makeObject<SkinnedMeshComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

void SkinnedMesh::init(const StringRef& filePath, float scale)
{
    init();
    m_component->setModel(detail::EngineDomain::meshManager()->createSkinnedMeshModel(filePath, scale));
}

SkinnedMeshComponent* SkinnedMesh::skinnedMeshComponent() const
{
    return m_component;
}

} // namespace ln

