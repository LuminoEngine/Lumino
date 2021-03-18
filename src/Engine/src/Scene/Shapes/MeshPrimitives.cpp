﻿
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/MeshPrimitive.hpp>
#include <LuminoEngine/Scene/Shapes/MeshPrimitiveComponent.hpp>
#include <LuminoEngine/Scene/Shapes/MeshPrimitives.hpp>
#include "../../Mesh/MeshManager.hpp"

namespace ln {
	
//=============================================================================
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

Ref<BoxMesh> BoxMesh::create()
{
    return makeObject<BoxMesh>();
}

Ref<BoxMesh> BoxMesh::create(float width, float height, float depth)
{
	return makeObject<BoxMesh>(width, height, depth);
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

bool BoxMesh::init()
{
	return init(1.0f, 1.0f, 1.0f);
}

bool BoxMesh::init(float width, float height, float depth)
{
	if (!StaticMesh::init()) return false;
	m_component = makeObject<BoxMeshComponent>(Vector3(width, height, depth));
	addComponent(m_component);
	setMainVisualComponent(m_component);
	return true;
}

void BoxMesh::setSize(const Vector3& size)
{
	m_component->setSize(size);
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

void PlaneMesh::serialize(Serializer2& ar)
{
	VisualObject::serialize(ar);
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
{
}

void BoxMesh::BuilderDetails::apply(BoxMesh* p) const
{
	StaticMesh::BuilderDetails::apply(p);
	p->setSize(size);
}

//==============================================================================
// SphereMesh::BuilderDetailsB

void SphereMesh::BuilderDetails::apply(SphereMesh* p) const
{
	ShapeObject::BuilderDetails::apply(p);
}

} // namespace ln

