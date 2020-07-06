
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Scene/Shapes/MeshPrimitiveComponent.hpp>
#include <LuminoEngine/Scene/Shapes/MeshPrimitives.hpp>
#include "../../Mesh/MeshManager.hpp"

namespace ln {

//==============================================================================
// BoxMesh

Ref<BoxMesh> BoxMesh::create()
{
    return makeObject<BoxMesh>();
}

//Ref<BoxMesh> BoxMesh::create(const StringRef& filePath, float scale)
//{
//    return makeObject<BoxMesh>(filePath, scale);
//}

BoxMesh::BoxMesh()
{
}

BoxMesh::~BoxMesh()
{
}

void BoxMesh::init()
{
    VisualObject::init();
    m_component = makeObject<BoxMeshComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

BoxMeshComponent* BoxMesh::boxMeshComponent() const
{
    return m_component;
}

//==============================================================================
// SphereMesh

Ref<SphereMesh> SphereMesh::create()
{
	return makeObject<SphereMesh>();
}

SphereMesh::SphereMesh()
{
}

void SphereMesh::init()
{
	VisualObject::init();
	m_component = makeObject<SphereMeshComponent>();
	addComponent(m_component);
	setMainVisualComponent(m_component);
}

SphereMeshComponent* SphereMesh::sphereMeshComponent() const
{
	return m_component;
}

//==============================================================================
// PlaneMesh

LN_OBJECT_IMPLEMENT(PlaneMesh, VisualObject) {}

Ref<PlaneMesh> PlaneMesh::create()
{
	return makeObject<PlaneMesh>();
}

Ref<PlaneMesh> PlaneMesh::create(Material* material)
{
	return makeObject<PlaneMesh>(material);
}

PlaneMesh::PlaneMesh()
{
}

bool PlaneMesh::init()
{
	if (!VisualObject::init()) return false;
	m_component = makeObject<PlaneMeshComponent>();
	addComponent(m_component);
	setMainVisualComponent(m_component);
	return true;
}

bool PlaneMesh::init(Material* material)
{
	if (!init()) return false;
	m_component->setMaterial(material);
	return true;
}

PlaneMeshComponent* PlaneMesh::planeMeshComponent() const
{
	return m_component;
}

void PlaneMesh::serialize2(Serializer2& ar)
{
	VisualObject::serialize2(ar);
	//ar & makeNVP(u"component", m_component);

	if (ar.isLoading()) {
		if (auto* c = findComponent<PlaneMeshComponent>()) {
			m_component = c;
			setMainVisualComponent(m_component);
		}
	}
}

} // namespace ln

