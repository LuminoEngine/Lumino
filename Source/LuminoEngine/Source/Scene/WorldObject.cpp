
#include "../Internal.h"
#include <Lumino/World.h>
#include <Lumino/Scene/WorldObject.h>
#include <Lumino/UI/UIEventArgs.h>
#include "../Framework/GameSceneManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// WorldObject
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(WorldObject, Object);

//------------------------------------------------------------------------------
WorldObjectPtr WorldObject::create()
{
	auto ptr = WorldObjectPtr::makeRef();
	//detail::GameSceneManager::getInstance()->getActiveScene()->addGameObject(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
WorldObject::WorldObject()
	: m_world(nullptr)
	, m_children()
	, m_parent(nullptr)
	//, m_combinedGlobalMatrix()
	, m_isAutoRelease(false)
	, m_isSpecialObject(false)
{
}

//------------------------------------------------------------------------------
WorldObject::~WorldObject()
{
}

//------------------------------------------------------------------------------
void WorldObject::initialize()
{
}

//------------------------------------------------------------------------------
World* WorldObject::getWorld() const
{
	// TODO: m_world は持たないようにする。上へさかのぼって検索する。
	return m_world;
}

//------------------------------------------------------------------------------
void WorldObject::addComponent(Component* component)
{
	if (LN_CHECK_ARG(component != nullptr)) return;
	if (LN_CHECK_ARG(component->m_owner == nullptr)) return;
	component->attach(this);
	m_components.add(component);
}

//------------------------------------------------------------------------------
void WorldObject::removeFromWorld()
{
	if (m_world != nullptr)
	{
		m_world->removeWorldObject(this);
	}
}

//------------------------------------------------------------------------------
//void WorldObject::onStart()
//{
//}

//------------------------------------------------------------------------------
void WorldObject::onUpdate()
{
	for (auto& c : m_components)
	{
		c->onUpdate();
	}
}

//------------------------------------------------------------------------------
void WorldObject::onRender(DrawList* context)
{

}

//------------------------------------------------------------------------------
void WorldObject::onDestroy()
{
}

//------------------------------------------------------------------------------
void WorldObject::onUIEvent(UIEventArgs* e)
{
	for (auto& c : m_components)
	{
		c->onUIEvent(e);
		if (e->handled) return;
	}
}

//------------------------------------------------------------------------------
void WorldObject::updateFrame()
{
	//Matrix localMatrix = transform.getTransformMatrix();

	//// グローバル行列結合
	//if (m_parent != nullptr)
	//{
	//	m_combinedGlobalMatrix = localMatrix * m_parent->m_combinedGlobalMatrix;
	//}
	//else
	//{
	//	m_combinedGlobalMatrix = localMatrix;
	//}


	onUpdate();

	transform.updateWorldMatrix();

	for (auto& c : m_components)
	{
		c->updateFrame();
	}

	// 子ノード更新
	int count = m_children.getCount();
	for (int i = 0; i < count; )
	{
		WorldObject* obj = m_children.getAt(i);
		obj->updateFrame();

		if (obj->m_isAutoRelease && obj->getReferenceCount() == 1)
		{
			m_children.removeAt(i);
			count = m_children.getCount();
		}
		else
		{
			++i;
		}
	}
}

//------------------------------------------------------------------------------
void WorldObject::render(DrawList* context)
{
	onRender(context);

	//for (auto& c : m_components)
	//{
	//	c->render(context);
	//}
}

//------------------------------------------------------------------------------
void WorldObject::releaseComponents()
{
	for (auto& c : m_components)
		c->detach();
	m_components.clear();
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
void WorldObject2D::initialize()
{
	WorldObject::initialize();
	detail::EngineDomain::getDefaultWorld2D()->addWorldObject(this, true);
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
void WorldObject3D::initialize()
{
	WorldObject::initialize();
	detail::EngineDomain::getDefaultWorld3D()->addWorldObject(this, true);
}

LN_NAMESPACE_END
