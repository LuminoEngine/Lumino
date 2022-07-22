
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Mesh/MeshPrimitive.hpp>
#include <LuminoGraphics/Mesh/MeshModel.hpp>
#include <LuminoGraphics/Mesh/MeshModelFactory.hpp>
#include <LuminoEngine/Scene/Shapes/MeshPrimitiveComponent.hpp>
#include <LuminoEngine/Scene/Shapes/MeshPrimitives.hpp>
#include "../../Graphics/src/Mesh/MeshManager.hpp"
#include "../SceneManager.hpp"

namespace ln {
	
//==============================================================================
// ShapeComponent

LN_OBJECT_IMPLEMENT(ShapeObject, VisualObject) {}

ShapeObject::ShapeObject()
{
}

bool ShapeObject::init()
{
    if (!VisualObject::init()) return false;
    return true;
}

void ShapeObject::setMaterial(Material* material)
{
	static_cast<ShapeComponent*>(mainVisualComponent().get())->setMaterial(material);
}

Material* ShapeObject::material() const
{
    return static_cast<ShapeComponent*>(mainVisualComponent().get())->material();
}

//==============================================================================
// BoxMesh

BoxMesh::BoxMesh()
{
}

BoxMesh::~BoxMesh()
{
}

bool BoxMesh::init(const Vector3& size, Material* material)
{
	auto model = detail::MeshModelFactory::createBox(size, material);
	if (!StaticMesh::init(model)) return false;
	return true;
}

bool BoxMesh::init()
{
	return init(1.0f, 1.0f, 1.0f);
}

bool BoxMesh::init(const Vector3& size)
{
	auto model = detail::MeshModelFactory::createBox(size);
	if (!StaticMesh::init(model)) return false;



	//m_component = makeObject<BoxMeshComponent>(size);
	//addComponent(m_component);
	//setMainVisualComponent(m_component);
	return true;
}

void BoxMesh::setSize(const Vector3& size)
{
	LN_NOTIMPLEMENTED();
	//m_component->setSize(size);
}
//
//BoxMeshComponent* BoxMesh::boxMeshComponent() const
//{
//    return m_component;
//}

//==============================================================================
// SphereMesh

Ref<SphereMesh> SphereMesh::create()
{
	return makeObject<SphereMesh>();
}

SphereMesh::SphereMesh()
{
}

bool SphereMesh::init()
{
	if (!ShapeObject::init()) return false;
	m_component = makeObject<SphereMeshComponent>();
	addComponent(m_component);
	setMainVisualComponent(m_component);
	return true;
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

void PlaneMesh::serialize_deprecated(Serializer2_deprecated& ar)
{
    VisualObject::serialize_deprecated(ar);
	//ar & makeNVP(u"component", m_component);

	if (ar.isLoading()) {
		if (auto* c = findComponent<PlaneMeshComponent>()) {
			m_component = c;
			setMainVisualComponent(m_component);
		}
	}
}

//==============================================================================
// PlaneMesh::BuilderDetails

void ShapeObject::BuilderDetails::apply(ShapeObject* p) const
{
	VisualObject::BuilderDetails::apply(p);
	p->setMaterial(material);
}

//==============================================================================
// PlaneMesh::BuilderDetails

void PlaneMesh::BuilderDetails::apply(PlaneMesh* p) const
{
	ShapeObject::BuilderDetails::apply(p);
}

//==============================================================================
// BoxMesh::BuilderDetails

BoxMesh::BuilderDetails::BuilderDetails()
	: size(1, 1, 1)
	, material(nullptr)
{
}

Ref<Object> BoxMesh::BuilderDetails::create() const
{
	auto p = makeObject<BoxMesh>(size, material);
	apply(p);
	return p;
}

//==============================================================================
// SphereMesh::BuilderDetailsB

void SphereMesh::BuilderDetails::apply(SphereMesh* p) const
{
	ShapeObject::BuilderDetails::apply(p);
}

} // namespace ln

