
#include "Internal.hpp"
#include <LuminoGraphics/Mesh/SkinnedMeshModel.hpp>
#include <LuminoEngine/Scene/Mesh/MeshComponent.hpp>
#include <LuminoEngine/Scene/Mesh/SkinnedMesh.hpp>
#include "../../Graphics/src/Mesh/MeshManager.hpp"

namespace ln {

//==============================================================================
// SkinnedMesh

Ref<SkinnedMesh> SkinnedMesh::create()
{
    return makeObject_deprecated<SkinnedMesh>();
}

Ref<SkinnedMesh> SkinnedMesh::load(const StringView& filePath, float scale)
{
    return makeObject_deprecated<SkinnedMesh>(filePath, scale);
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
    m_component = makeObject_deprecated<MeshComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

void SkinnedMesh::init(const StringView& filePath, float scale)
{
    init();
    m_component->setModel(MeshModel::load(filePath));
}

MeshComponent* SkinnedMesh::meshComponent() const
{
    return m_component;
}

} // namespace ln

