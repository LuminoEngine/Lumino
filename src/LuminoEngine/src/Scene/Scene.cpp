
#include "Internal.hpp"
#include <LuminoEngine/Scene/Scene.hpp>
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// Scene

LN_OBJECT_IMPLEMENT(Scene, Object);

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::init()
{

}

void Scene::onCreated()
{
}

void Scene::onClosed()
{
}

void Scene::onActivated()
{
}

void Scene::onDeactivated()
{
}

void Scene::onUpdate()
{
	//for (auto& ptr : m_gameObjectList)
	//{
	//	ptr->onUpdate();
	//}
}

void Scene::update()
{
	onUpdate();
}

} // namespace ln

