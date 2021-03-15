
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include <LuminoEngine/Scene/Mesh/MeshComponent.hpp>
#include <LuminoEngine/Scene/Mesh/StaticMesh.hpp>
#include "../../Mesh/MeshManager.hpp"

namespace ln {

//==============================================================================
// StaticMesh

LN_OBJECT_IMPLEMENT(StaticMesh, VisualObject) {}

Ref<StaticMesh> StaticMesh::load(const StringRef& filePath, MeshImportSettings* settings)
{
    return makeObject<StaticMesh>(filePath, settings);
}

Ref<StaticMesh> StaticMesh::create()
{
    return makeObject<StaticMesh>();
}

Ref<StaticMesh> StaticMesh::create(MeshModel* model)
{
    return makeObject<StaticMesh>(model);
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
    m_component = makeObject<MeshComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

void StaticMesh::init(MeshModel* model)
{
    init();
    m_component->setModel(model);
}

void StaticMesh::init(const StringRef& filePath, MeshImportSettings* settings)
{
    auto model = detail::EngineDomain::meshManager()->createSkinnedMeshModel(
        filePath, settings ? settings : MeshImportSettings::defaultSettings());
    init(model);
}

MeshComponent* StaticMesh::staticMeshComponent() const
{
    return m_component;
}

MeshModel* StaticMesh::model() const
{
    return m_component->model();
}

void StaticMesh::makeCollisionBody(StringRef meshContainerName)
{
    m_component->makeCollisionBody(meshContainerName);
}

void StaticMesh::serialize(Serializer2& ar)
{
    VisualObject::serialize(ar);

    

    if (ar.isLoading()) {
        if (auto* c = findComponent<MeshComponent>()) {
            m_component = c;
            setMainVisualComponent(m_component);
        }
    }
}

} // namespace ln

