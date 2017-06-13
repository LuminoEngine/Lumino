
#include "../Internal.h"
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Scene/StaticMesh.h>
#include <Lumino/Scene/SceneGraph.h>
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// StaticMeshComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(StaticMeshComponent, VisualComponent);

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::create(StaticMeshModel* staticMeshModel)
{
	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	ptr->initialize(staticMeshModel);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::create(const StringRef& filePath)
{
	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	auto mesh = SceneGraphManager::Instance->getModelManager()->CreateStaticMeshModel(filePath);
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreatePlane(const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags)
{
	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->InitializePlane(SceneGraphManager::Instance->getGraphicsManager(), size, sliceH, sliceV, flags);
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateScreenPlane()
{
	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->InitializeScreenPlane(SceneGraphManager::Instance->getGraphicsManager(), MeshCreationFlags::None);
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateBox()
{
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->initialize(SceneGraphManager::Instance->getGraphicsManager());
	mesh->AddMeshResource(detail::ModelManager::getInstance()->GetUnitBoxMeshResource(MeshCreationFlags::None));
	mesh->AddMaterial(detail::ModelManager::getInstance()->GetDefaultMaterial());

	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateBox(const Vector3& size)
{
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->InitializeBox(SceneGraphManager::Instance->getGraphicsManager(), size, MeshCreationFlags::None);

	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateBox(float width, float height, float depth)
{
	return CreateBox(Vector3(width, height, depth));
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateSphere()
{
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->initialize(SceneGraphManager::Instance->getGraphicsManager());
	mesh->AddMeshResource(detail::ModelManager::getInstance()->GetUnitSphereMeshResource(MeshCreationFlags::None));
	mesh->AddMaterial(detail::ModelManager::getInstance()->GetDefaultMaterial());

	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateSphere(float radius, int tessellation)
{
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->InitializeSphere(SceneGraphManager::Instance->getGraphicsManager(), radius, tessellation, tessellation, MeshCreationFlags::None);

	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateTeapot()
{
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->initialize(SceneGraphManager::Instance->getGraphicsManager());
	mesh->AddMeshResource(detail::ModelManager::getInstance()->GetUnitTeapotMeshResource());
	mesh->AddMaterial(detail::ModelManager::getInstance()->GetDefaultMaterial());

	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateTeapot(float size, int tessellation)
{
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->InitializeTeapot(SceneGraphManager::Instance->getGraphicsManager(), size, tessellation, MeshCreationFlags::None);

	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMeshComponent::StaticMeshComponent()
	: m_mesh()
{
}

//------------------------------------------------------------------------------
StaticMeshComponent::~StaticMeshComponent()
{
}

//------------------------------------------------------------------------------
void StaticMeshComponent::initialize(StaticMeshModel* meshModel)
{
	if (LN_CHECK_ARG(meshModel != nullptr)) return;
	m_mesh = meshModel;

	VisualComponent::initialize();

	//m_materialList->copyShared(m_mesh->m_materials, true);


	//owner->GetRootNode()->AddChild(this);
	setAutoRemove(true);
}

//------------------------------------------------------------------------------
void StaticMeshComponent::OnRender2(DrawList* renderer)
{
	renderer->SetTransform(getOwnerObject()->transform.getWorldMatrix());

	for (int iMesh = 0; iMesh < m_mesh->GetMeshResourceCount(); iMesh++)
	{
		MeshResource* m = m_mesh->GetMeshResource(iMesh);
		for (int i = 0; i < m->GetSubsetCount(); i++)
		{
			MeshAttribute attr;
			m->GetMeshAttribute(i, &attr);
			renderer->DrawMesh(m, i, m_mesh->GetMaterial(attr.MaterialIndex));
			//renderer->DrawMesh(m, i, GetMaterials()->GetAt(i));
		}
	}

}

//==============================================================================
// Rectangle
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Rectangle, VisualComponent);

//------------------------------------------------------------------------------
RefPtr<Rectangle> Rectangle::create(const Rect& rect)
{
	return newObject<Rectangle>(rect);
}

//------------------------------------------------------------------------------
Rectangle::Rectangle()
{
}

//------------------------------------------------------------------------------
Rectangle::~Rectangle()
{
}

//------------------------------------------------------------------------------
void Rectangle::initialize(const Rect& rect)
{
	VisualComponent::initialize();
	detail::EngineDomain::getDefaultSceneGraph3D()->GetRootNode()->AddChild(this);
	setAutoRemove(true);

	m_rect = rect;
}

//------------------------------------------------------------------------------
void Rectangle::OnRender2(DrawList* renderer)
{
	renderer->DrawSquarePrimitive(
		Vector3(m_rect.getTopLeft(), 0), Vector2(0, 0), Color::White,
		Vector3(m_rect.getTopRight(), 0), Vector2(1, 0), Color::White,
		Vector3(m_rect.getBottomLeft(), 0), Vector2(0, 1), Color::White,
		Vector3(m_rect.getBottomRight(), 0), Vector2(1, 1), Color::White);
}

LN_NAMESPACE_END
