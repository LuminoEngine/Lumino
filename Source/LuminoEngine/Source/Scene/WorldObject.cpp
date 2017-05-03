
#include "../Internal.h"
#include <Lumino/World.h>
#include <Lumino/Scene/WorldObject.h>
#include "../Framework/GameSceneManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// WorldObject
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(WorldObject, Object);

//------------------------------------------------------------------------------
WorldObjectPtr WorldObject::Create()
{
	auto ptr = WorldObjectPtr::MakeRef();
	//detail::GameSceneManager::GetInstance()->GetActiveScene()->AddGameObject(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
WorldObject::WorldObject()
	: m_children()
	, m_parent(nullptr)
	, m_combinedGlobalMatrix()
	, m_isAutoRelease(false)
{
}

//------------------------------------------------------------------------------
WorldObject::~WorldObject()
{
}

//------------------------------------------------------------------------------
void WorldObject::Initialize()
{
}

//------------------------------------------------------------------------------
void WorldObject::AddComponent(Component* component)
{
	if (LN_CHECK_ARG(component != nullptr)) return;
	if (LN_CHECK_ARG(component->m_owner == nullptr)) return;
	component->Attach(this);
	m_components.Add(component);
}

//------------------------------------------------------------------------------
//void WorldObject::OnStart()
//{
//}

//------------------------------------------------------------------------------
void WorldObject::OnUpdate()
{
	for (auto& c : m_components)
	{
		c->OnUpdate();
	}
}

//------------------------------------------------------------------------------
void WorldObject::OnRender(DrawList* context)
{

}

//------------------------------------------------------------------------------
void WorldObject::OnDestroy()
{
}

//------------------------------------------------------------------------------
void WorldObject::UpdateFrame()
{
	Matrix localMatrix = transform.GetTransformMatrix();

	// グローバル行列結合
	if (m_parent != nullptr)
	{
		m_combinedGlobalMatrix = localMatrix * m_parent->m_combinedGlobalMatrix;
	}
	else
	{
		m_combinedGlobalMatrix = localMatrix;
	}

	OnUpdate();

	for (auto& c : m_components)
	{
		c->UpdateFrame();
	}

	// 子ノード更新
	int count = m_children.GetCount();
	for (int i = 0; i < count; )
	{
		WorldObject* obj = m_children.GetAt(i);
		obj->UpdateFrame();

		if (obj->m_isAutoRelease && obj->GetReferenceCount() == 1)
		{
			m_children.RemoveAt(i);
			count = m_children.GetCount();
		}
		else
		{
			++i;
		}
	}
}

//------------------------------------------------------------------------------
void WorldObject::Render(DrawList* context)
{
	OnRender(context);

	for (auto& c : m_components)
	{
		c->Render(context);
	}
}

//------------------------------------------------------------------------------
void WorldObject::ReleaseComponents()
{
	for (auto& c : m_components)
		c->Detach();
	m_components.Clear();
}


//==============================================================================
// WorldObject2D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(WorldObject2D, WorldObject);

//------------------------------------------------------------------------------
WorldObject2D::WorldObject2D()
{
}

//------------------------------------------------------------------------------
WorldObject2D::~WorldObject2D()
{
}

//------------------------------------------------------------------------------
void WorldObject2D::Initialize()
{
	WorldObject::Initialize();
	detail::EngineDomain::GetDefaultWorld2D()->AddWorldObject(this, true);
}


//==============================================================================
// WorldObject3D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(WorldObject3D, WorldObject);

//------------------------------------------------------------------------------
WorldObject3D::WorldObject3D()
{
}

//------------------------------------------------------------------------------
WorldObject3D::~WorldObject3D()
{
}

//------------------------------------------------------------------------------
void WorldObject3D::Initialize()
{
	WorldObject::Initialize();
	detail::EngineDomain::GetDefaultWorld3D()->AddWorldObject(this, true);
}

LN_NAMESPACE_END
