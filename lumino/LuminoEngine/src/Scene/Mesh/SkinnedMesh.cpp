
#include "../Internal.hpp"
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include <LuminoEngine/Scene/Mesh/MeshComponent.hpp>
#include <LuminoEngine/Scene/Mesh/SkinnedMesh.hpp>
#include "../../Mesh/MeshManager.hpp"

namespace ln {

//==============================================================================
// SkinnedMesh

Ref<SkinnedMesh> SkinnedMesh::create()
{
    return makeObject<SkinnedMesh>();
}

Ref<SkinnedMesh> SkinnedMesh::load(const StringRef& filePath, float scale)
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
    m_component = makeObject<MeshComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

void SkinnedMesh::init(const StringRef& filePath, float scale)
{
    init();
    m_component->setModel(MeshModel::load(filePath));
}

MeshComponent* SkinnedMesh::meshComponent() const
{
    return m_component;
}

} // namespace ln

