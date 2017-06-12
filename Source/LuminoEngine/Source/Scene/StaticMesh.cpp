
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
RefPtr<StaticMeshComponent> StaticMeshComponent::Create(StaticMeshModel* staticMeshModel)
{
	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	ptr->initialize(staticMeshModel);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::Create(const StringRef& filePath)
{
	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	auto mesh = SceneGraphManager::Instance->GetModelManager()->CreateStaticMeshModel(filePath);
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreatePlane(const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags)
{
	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializePlane(SceneGraphManager::Instance->GetGraphicsManager(), size, sliceH, sliceV, flags);
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateScreenPlane()
{
	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeScreenPlane(SceneGraphManager::Instance->GetGraphicsManager(), MeshCreationFlags::None);
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateBox()
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->initialize(SceneGraphManager::Instance->GetGraphicsManager());
	mesh->AddMeshResource(detail::ModelManager::GetInstance()->GetUnitBoxMeshResource(MeshCreationFlags::None));
	mesh->AddMaterial(detail::ModelManager::GetInstance()->GetDefaultMaterial());

	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateBox(const Vector3& size)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeBox(SceneGraphManager::Instance->GetGraphicsManager(), size, MeshCreationFlags::None);

	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
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
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->initialize(SceneGraphManager::Instance->GetGraphicsManager());
	mesh->AddMeshResource(detail::ModelManager::GetInstance()->GetUnitSphereMeshResource(MeshCreationFlags::None));
	mesh->AddMaterial(detail::ModelManager::GetInstance()->GetDefaultMaterial());

	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateSphere(float radius, int tessellation)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeSphere(SceneGraphManager::Instance->GetGraphicsManager(), radius, tessellation, tessellation, MeshCreationFlags::None);

	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateTeapot()
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->initialize(SceneGraphManager::Instance->GetGraphicsManager());
	mesh->AddMeshResource(detail::ModelManager::GetInstance()->GetUnitTeapotMeshResource());
	mesh->AddMaterial(detail::ModelManager::GetInstance()->GetDefaultMaterial());

	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateTeapot(float size, int tessellation)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeTeapot(SceneGraphManager::Instance->GetGraphicsManager(), size, tessellation, MeshCreationFlags::None);

	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
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

	//m_materialList->CopyShared(m_mesh->m_materials, true);


	//owner->GetRootNode()->AddChild(this);
	SetAutoRemove(true);
}

//------------------------------------------------------------------------------
void StaticMeshComponent::OnRender2(DrawList* renderer)
{
	renderer->SetTransform(GetOwnerObject()->transform.GetWorldMatrix());

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
RefPtr<Rectangle> Rectangle::Create(const Rect& rect)
{
	return NewObject<Rectangle>(rect);
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
	detail::EngineDomain::GetDefaultSceneGraph3D()->GetRootNode()->AddChild(this);
	SetAutoRemove(true);

	m_rect = rect;
}

//------------------------------------------------------------------------------
void Rectangle::OnRender2(DrawList* renderer)
{
	renderer->DrawSquarePrimitive(
		Vector3(m_rect.GetTopLeft(), 0), Vector2(0, 0), Color::White,
		Vector3(m_rect.GetTopRight(), 0), Vector2(1, 0), Color::White,
		Vector3(m_rect.GetBottomLeft(), 0), Vector2(0, 1), Color::White,
		Vector3(m_rect.GetBottomRight(), 0), Vector2(1, 1), Color::White);
}

LN_NAMESPACE_END
