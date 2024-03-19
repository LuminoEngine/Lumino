
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include <LuminoEngine/Scene/Mesh/MeshComponent.hpp>
#include <LuminoEngine/Scene/Mesh/StaticMesh.hpp>
#include "../../../../LuminoRuntime/src/Mesh/MeshManager.hpp"

namespace ln {

//==============================================================================
// StaticMesh

LN_OBJECT_IMPLEMENT(StaticMesh, VisualObject) {}

Ref<StaticMesh> StaticMesh::load(const StringView& filePath, MeshImportSettings* settings)
{
    return makeObject_deprecated<StaticMesh>(filePath, settings);
}

Ref<StaticMesh> StaticMesh::create()
{
    return makeObject_deprecated<StaticMesh>();
}

Ref<StaticMesh> StaticMesh::create(MeshModel* model)
{
    return makeObject_deprecated<StaticMesh>(model);
}

StaticMesh::StaticMesh()
{
}

StaticMesh::~StaticMesh()
{
}

bool StaticMesh::init()
{
    if (!VisualObject::init()) return false;
    m_component = makeObject_deprecated<MeshComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
    return true;
}

bool StaticMesh::init(MeshModel* model)
{
    if (!init()) return false;
    m_component->setModel(model);
    return true;
}

bool StaticMesh::init(const StringView& filePath, MeshImportSettings* settings)
{
    auto model = detail::MeshManager::instance()->createSkinnedMeshModel(
        filePath, settings ? settings : MeshImportSettings::defaultSettings());
    if (!init(model)) return false;
    return true;
}

MeshComponent* StaticMesh::staticMeshComponent() const
{
    return m_component;
}

MeshModel* StaticMesh::model() const
{
    return m_component->model();
}

void StaticMesh::makeCollisionBody(StringView meshContainerName)
{
    m_component->makeCollisionBody(meshContainerName);
}

void StaticMesh::serialize_deprecated(Serializer2_deprecated& ar)
{
    VisualObject::serialize_deprecated(ar);

    

    if (ar.isLoading()) {
        if (auto* c = findComponent<MeshComponent>()) {
            m_component = c;
            setMainVisualComponent(m_component);
        }
    }
}

//==============================================================================
// StaticMesh::BuilderDetails

void StaticMesh::BuilderDetails::apply(StaticMesh* p) const
{
    VisualObject::BuilderDetails::apply(p);
}

} // namespace ln

