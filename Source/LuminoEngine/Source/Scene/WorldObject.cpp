
#include "../Internal.h"
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
	detail::GameSceneManager::GetInstance()->GetActiveScene()->AddGameObject(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
WorldObject::WorldObject()
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
void WorldObject::OnDestroy()
{
}

//------------------------------------------------------------------------------
void WorldObject::OnRender(DrawList* context)
{

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

LN_NAMESPACE_END
