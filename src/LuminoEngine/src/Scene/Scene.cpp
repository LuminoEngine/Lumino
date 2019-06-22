/*
 Note: Scene なんて必要なの？
    主な目的は、
    1. オブジェクト間の相互作用ルールを記述する
    2. コールツリーを利用して、アクティブなルールを制御する

    1 については RPG ツクールの BattleScene のイメージ。
    コマンド入力中、実行中、など。
    Unity とかだと基本的にコンポーネントの実装の中で頑張ってルールを書くことになる。
    コンポーネントの実装は一般的な MVC の考えだと V が近い。ルールは C。
    V の中に状態に応じた switch とかたくさん書く必要が出てしまうのでよくない。

    2 については、バトル画面のサブとしてメニュー画面を出したりする仕組み。
*/
#include "Internal.hpp"
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Scene/Scene.hpp>
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// Scene

LN_OBJECT_IMPLEMENT(Scene, Object) {}

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


void Scene::setup(const ln::Path& filePath)
{
	m_filePath = filePath;
}

void Scene::save()
{
}

void Scene::load()
{
}

void Scene::clear()
{
	m_rootWorldObjects.clear();
}

void Scene::addObject(WorldObject* obj)
{
	if (LN_REQUIRE(obj)) return;
	if (LN_REQUIRE(m_rootWorldObjects.contains(obj))) return;
	m_rootWorldObjects.add(obj);
}

void Scene::removeObject(WorldObject* obj)
{
	m_rootWorldObjects.remove(obj);
}

void Scene::serialize(Archive& ar)
{
}




//==============================================================================
// SceneAsset

namespace ed {

LN_OBJECT_IMPLEMENT(SceneAsset, AssetModel) {}

SceneAsset::SceneAsset()
{
}

void SceneAsset::init()
{
    AssetModel::init();
}

void SceneAsset::setup(const ln::Path& filePath)
{
	m_filePath = filePath;
}

void SceneAsset::clear()
{
}

void SceneAsset::save()
{
	auto json = ln::JsonSerializer::serialize(*this);
	ln::FileSystem::writeAllText(m_filePath, json);
}

void SceneAsset::load()
{
}

void SceneAsset::serialize(Archive& ar)
{
	ar & makeNVP(u"objects", m_rootWorldObjects);
}

void SceneAsset::addNewWorldObject()
{
	auto obj = makeObject<WorldObjectAsset>();
	m_rootWorldObjects.add(obj);
}

} // namespace ed
} // namespace ln

