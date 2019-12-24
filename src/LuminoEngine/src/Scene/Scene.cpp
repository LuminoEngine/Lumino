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
#include <LuminoEngine/Scene/Component.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Scene/Scene.hpp>
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// Scene

LN_OBJECT_IMPLEMENT(Scene, Object)
{
    context->registerType<Scene>({});
}

Scene::Scene()
    : m_ownerWorld(nullptr)
	, m_rootWorldObjectList(makeList<Ref<WorldObject>>())
	, m_initialUpdate(true)
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
}

void Scene::onPostUpdate(float elapsedSeconds)
{
    for (WorldObject* obj : m_destroyList) {
        //obj->removeFromWorld();

        obj->detachScene();
        m_rootWorldObjectList->remove(obj);
    }
    m_destroyList.clear();
}

void Scene::update(float elapsedSeconds)
{
    // onUpdate 内で新しい WorldObject が作成され、m_rootWorldObjectList に add される場合に備えて
    // イテレータによる列挙は行わない。新しく追加されたものは、このループで
    // 今のフレーム中の最初の onUpdate が呼ばれる。
    for (int i = 0; i < m_rootWorldObjectList->size(); i++) {
        m_rootWorldObjectList[i]->updateFrame(elapsedSeconds);
    }

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
	m_rootWorldObjectList->clear();
}

void Scene::addObject(WorldObject* obj)
{
	if (LN_REQUIRE(obj)) return;
	if (LN_REQUIRE(!obj->m_scene)) return;
	m_rootWorldObjectList->add(obj);
    obj->attachScene(this);
}

void Scene::removeObject(WorldObject* obj)
{
	m_rootWorldObjectList->remove(obj);
}

void Scene::removeRootObject(WorldObject* obj)
{
    if (m_rootWorldObjectList->remove(obj)) {
        if (obj->destroyed()) {
            m_destroyList.remove(obj);
        }
        obj->detachScene();
    }
}

void Scene::removeAllObjects()
{
    for (int i = m_rootWorldObjectList->size() - 1; i >= 0; i--)
    {
        auto& obj = m_rootWorldObjectList[i];
        if (!obj->isSpecialObject())
        {
            if (obj->destroyed()) {
                m_destroyList.remove(obj);
            }

            obj->detachScene();
            m_rootWorldObjectList->removeAt(i);
        }
    }
}

// Multi-Lang 対応のため、テンプレートではなく基本は TypeInfo で検索する
WorldObject* Scene::findObjectByComponentType(const TypeInfo* type) const
{
    class LocalVisitor : public detail::IWorldObjectVisitor
    {
    public:
        const TypeInfo* type;
        WorldObject* result = nullptr;
        virtual bool visit(WorldObject* obj)
        {
            for (auto& component : *(obj->m_components)) {
                if (TypeInfo::getTypeInfo(component) == type) {
                    result = obj;
                    return false;
                }
            }
            return true;
        }
    } visitor;
    visitor.type = type;

    for (auto& obj : m_rootWorldObjectList) {
        if (!obj->traverse(&visitor)) {
            break;
        }
    }

    return visitor.result;
}

void Scene::updateObjectsWorldMatrix()
{
    for (auto& obj : m_rootWorldObjectList)
    {
        obj->updateWorldMatrixHierarchical();
    }
}

void Scene::onPreUpdate(float elapsedSeconds)
{
	if (m_initialUpdate) {
		for (auto& obj : m_rootWorldObjectList) {
			obj->start();
		}
		m_initialUpdate = false;
	}


    for (auto& obj : m_rootWorldObjectList)
    {
        obj->onPreUpdate();
    }
}

void Scene::renderObjects(RenderingContext* context)
{
    for (auto& obj : m_rootWorldObjectList)
    {
        obj->render(context);

        for (auto& c : *(obj->m_components))
        {
            c->onPrepareRender(context); // TODO: 全体の前にした方がいいかも
            c->render(context);
        }
    }
}

void Scene::serialize(Archive& ar)
{
    Object::serialize(ar);

    ar & ln::makeNVP(u"children", *m_rootWorldObjectList);

    if (ar.isLoading()) {
        for (auto& obj : m_rootWorldObjectList) {
            obj->attachScene(this);
        }
    }
}




////==============================================================================
//// SceneAsset
//
//namespace ed {
//
//LN_OBJECT_IMPLEMENT(SceneAsset, AssetModel) {}
//
//SceneAsset::SceneAsset()
//{
//}
//
//void SceneAsset::init()
//{
//    AssetModel::init();
//}
//
//void SceneAsset::setup(const ln::Path& filePath)
//{
//	m_filePath = filePath;
//}
//
//void SceneAsset::clear()
//{
//}
//
//void SceneAsset::save()
//{
//	auto json = ln::JsonSerializer::serialize(*this);
//	ln::FileSystem::writeAllText(m_filePath, json);
//}
//
//void SceneAsset::load()
//{
//}
//
//void SceneAsset::serialize(Archive& ar)
//{
//	ar & makeNVP(u"objects", m_rootWorldObjectList);
//}
//
//void SceneAsset::addNewWorldObject()
//{
//	auto obj = makeObject<WorldObjectAsset>();
//	m_rootWorldObjectList.add(obj);
//}

//} // namespace ed
} // namespace ln

