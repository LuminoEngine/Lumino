
#include "../Internal.h"
#include <Lumino/World.h>
#include <Lumino/Scene/WorldObject.h>
#include <Lumino/UI/UIEventArgs.h>
#include "../Framework/GameSceneManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// LayerMask
//==============================================================================

//------------------------------------------------------------------------------
int LayerMask::GetLayer(BuiltinLayers builtinLayer)
{
	switch (builtinLayer)
	{
	case ln::BuiltinLayers::Default2D:
		return 0;
	case ln::BuiltinLayers::Default3D:
		return 1;
	case ln::BuiltinLayers::Background2D:
		return 2;
	case ln::BuiltinLayers::Background3D:
		return 3;
	case ln::BuiltinLayers::Layer5:
		return 4;
	case ln::BuiltinLayers::Layer6:
		return 5;
	case ln::BuiltinLayers::Layer7:
		return 6;
	default:
		LN_CHECK_ARG(0);
		break;
	}
	return 0;
}

//------------------------------------------------------------------------------
int LayerMask::GetRenderOrder(int layerId)
{
	const int sortingLayers[MaxLayers] =
	{
		3,//Default2D,
		2,//Default3D,
		1,//Background2D,
		0,//Background3D,
		//1,	// Background
		//0,
		//2, 3,
		4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		31,
	};
	return sortingLayers[layerId];
}

//------------------------------------------------------------------------------
bool LayerMask::filterComponent(Component* obj, uint32_t layerMask)
{
	uint32_t layerFlag = (1 << obj->getLayer());
	return (layerMask & layerFlag) != 0;
}

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
	, m_tag()
	, m_layer(0)
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

	// if alraady added, onAttachedWorld
	World* world = getWorld();
	if (world != nullptr)
	{
		component->onAttachedWorld(world);
	}
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
void WorldObject::onAttachedWorld(World* world)
{
	for (auto& c : m_components)
	{
		c->onAttachedWorld(world);
	}
	for (auto& c : m_children)
	{
		c->onAttachedWorld(world);
	}
}

//------------------------------------------------------------------------------
void WorldObject::onDetachedWorld(World* world)
{
	for (auto& c : m_children)
	{
		c->onDetachedWorld(world);
	}
	for (auto& c : m_components)
	{
		c->onDetachedWorld(world);
	}
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
