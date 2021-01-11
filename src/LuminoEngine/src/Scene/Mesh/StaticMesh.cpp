
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Scene/Mesh/MeshComponent.hpp>
#include <LuminoEngine/Scene/Mesh/StaticMesh.hpp>
#include "../../Mesh/MeshManager.hpp"

namespace ln {

//==============================================================================
// Mesh

LN_OBJECT_IMPLEMENT(Mesh, VisualObject) {}

Ref<Mesh> Mesh::load(const StringRef& filePath)
{
    return makeObject<Mesh>(filePath, 1.0f);
}

Ref<Mesh> Mesh::create()
{
    return makeObject<Mesh>();
}

Ref<Mesh> Mesh::create(MeshModel* model)
{
    return makeObject<Mesh>(model);
}

Ref<Mesh> Mesh::create(const StringRef& filePath, float scale)
{
    return makeObject<Mesh>(filePath, scale);
}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::init()
{
    VisualObject::init();
    m_component = makeObject<MeshComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

void Mesh::init(MeshModel* model)
{
    init();
    m_component->setModel(model);
}

void Mesh::init(const StringRef& filePath, float scale)
{
    //auto model = makeObject<MeshModel>();
    //detail::EngineDomain::meshManager()->loadStaticMeshModel(model, filePath, scale);
    auto model = detail::EngineDomain::meshManager()->acquireStaticMeshModel(filePath, scale);
    init(model);
}

MeshComponent* Mesh::staticMeshComponent() const
{
    return m_component;
}

MeshModel* Mesh::model() const
{
    return m_component->model();
}

void Mesh::makeCollisionBody(StringRef meshContainerName)
{
    m_component->makeCollisionBody(meshContainerName);
}

void Mesh::serialize(Serializer2& ar)
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

