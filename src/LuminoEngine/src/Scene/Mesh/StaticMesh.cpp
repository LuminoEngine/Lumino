﻿
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Scene/Mesh/MeshComponent.hpp>
#include <LuminoEngine/Scene/Mesh/StaticMesh.hpp>
#include "../../Mesh/MeshManager.hpp"

namespace ln {

//==============================================================================
// StaticMesh

LN_OBJECT_IMPLEMENT(StaticMesh, VisualObject) {}

Ref<StaticMesh> StaticMesh::load(const StringRef& filePath)
{
    return makeObject<StaticMesh>(filePath, 1.0f);
}

Ref<StaticMesh> StaticMesh::create()
{
    return makeObject<StaticMesh>();
}

Ref<StaticMesh> StaticMesh::create(MeshModel* model)
{
    return makeObject<StaticMesh>(model);
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
    m_component = makeObject<MeshComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

void StaticMesh::init(MeshModel* model)
{
    init();
    m_component->setModel(model);
}

void StaticMesh::init(const StringRef& filePath, float scale)
{
    //auto model = makeObject<MeshModel>();
    //detail::EngineDomain::meshManager()->loadStaticMeshModel(model, filePath, scale);
    auto model = detail::EngineDomain::meshManager()->acquireStaticMeshModel(filePath, scale);
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

