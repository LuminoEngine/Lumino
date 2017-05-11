
#include "../Internal.h"
#include <Lumino/Graphics/Rendering.h>
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
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), staticMeshModel);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::Create(const StringRef& filePath)
{
	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	auto mesh = SceneGraphManager::Instance->GetModelManager()->CreateStaticMeshModel(filePath);
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreatePlane(const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags)
{
	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializePlane(SceneGraphManager::Instance->GetGraphicsManager(), size, sliceH, sliceV, flags);
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateScreenPlane()
{
	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeScreenPlane(SceneGraphManager::Instance->GetGraphicsManager(), MeshCreationFlags::None);
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateBox()
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager());
	mesh->SetMeshResource(detail::ModelManager::GetInstance()->GetUnitBoxMeshResource(MeshCreationFlags::None));
	mesh->AddMaterial(detail::ModelManager::GetInstance()->GetDefaultMaterial());

	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateBox(const Vector3& size)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeBox(SceneGraphManager::Instance->GetGraphicsManager(), size, MeshCreationFlags::None);

	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
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
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager());
	mesh->SetMeshResource(detail::ModelManager::GetInstance()->GetUnitSphereMeshResource(MeshCreationFlags::None));
	mesh->AddMaterial(detail::ModelManager::GetInstance()->GetDefaultMaterial());

	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateSphere(float radius, int tessellation)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeSphere(SceneGraphManager::Instance->GetGraphicsManager(), radius, tessellation, tessellation, MeshCreationFlags::None);

	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateTeapot()
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager());
	mesh->SetMeshResource(detail::ModelManager::GetInstance()->GetUnitTeapotMeshResource());
	mesh->AddMaterial(detail::ModelManager::GetInstance()->GetDefaultMaterial());

	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::CreateTeapot(float size, int tessellation)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeTeapot(SceneGraphManager::Instance->GetGraphicsManager(), size, tessellation, MeshCreationFlags::None);

	auto ptr = RefPtr<StaticMeshComponent>::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
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
void StaticMeshComponent::Initialize(SceneGraph* owner, StaticMeshModel* meshModel)
{
	if (LN_CHECK_ARG(meshModel != nullptr)) return;
	m_mesh = meshModel;

	VisualComponent::Initialize(owner, m_mesh->GetSubsetCount());

	m_materialList->CopyShared(m_mesh->m_materials, true);


	owner->GetRootNode()->AddChild(this);
	SetAutoRemove(true);
}

//------------------------------------------------------------------------------
void StaticMeshComponent::OnRender2(DrawList* renderer)
{
	renderer->SetTransform(m_combinedGlobalMatrix);

	MeshResource* m = m_mesh->GetMeshResource();
	for (int i = 0; i < m->GetSubsetCount(); i++)
	{
		renderer->DrawMesh(m, i, GetMaterials()->GetAt(i));
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
void Rectangle::Initialize(const Rect& rect)
{
	VisualComponent::Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), 1);
	detail::EngineDomain::GetDefaultSceneGraph3D()->GetRootNode()->AddChild(this);
	SetAutoRemove(true);

	m_rect = rect;
}

//------------------------------------------------------------------------------
void Rectangle::OnRender2(DrawList* renderer)
{
	//renderer->SetShader(GetPrimaryShader());
	renderer->DrawSquarePrimitive(
		Vector3(m_rect.GetTopLeft(), 0), Vector2(0, 0), Color::White,
		Vector3(m_rect.GetTopRight(), 0), Vector2(1, 0), Color::White,
		Vector3(m_rect.GetBottomLeft(), 0), Vector2(0, 1), Color::White,
		Vector3(m_rect.GetBottomRight(), 0), Vector2(1, 1), Color::White);
}

LN_NAMESPACE_END
