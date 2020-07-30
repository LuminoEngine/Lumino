
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Scene/Component.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Scene/Level.hpp>
#include <LuminoEngine/Scene/World.hpp>

namespace ln {

//==============================================================================
// LevelRenderParameters

void LevelRenderParameters::serialize2(Serializer2& ar)
{
    ar & ln::makeNVP(u"fogStartDistance", m_fogStartDistance);
    ar & ln::makeNVP(u"fogColor", m_fogColor);
    ar & ln::makeNVP(u"fogDensity", m_fogDensity);
    ar & ln::makeNVP(u"fogHeightDensity", m_fogHeightDensity);
    ar & ln::makeNVP(u"fogLowerHeight", m_fogLowerHeight);
    ar & ln::makeNVP(u"fogUpperHeight", m_fogUpperHeight);
    
    ar & ln::makeNVP(u"skydomeSkyColor", m_skydomeSkyColor);
    ar & ln::makeNVP(u"skydomeHorizonColor", m_skydomeHorizonColor);
    ar & ln::makeNVP(u"skydomeCloudColor", m_skydomeCloudColor);
    ar & ln::makeNVP(u"skydomeOverlayColor", m_skydomeOverlayColor);
}

void LevelRenderParameters::mergeToRenderParams(detail::SceneGlobalRenderParams* params) const
{
    if (m_fogStartDistance) params->startDistance = m_fogStartDistance.value();
    if (m_fogColor) params->fogColor = m_fogColor.value();
    if (m_fogDensity) params->fogDensity = m_fogDensity.value();
    if (m_fogHeightDensity) params->heightFogDensity = m_fogHeightDensity.value();
    if (m_fogLowerHeight) params->lowerHeight = m_fogLowerHeight.value();
    if (m_fogUpperHeight) params->upperHeight = m_fogUpperHeight.value();

    if (m_skydomeSkyColor) params->skydomeSkyColor = m_skydomeSkyColor.value();
    if (m_skydomeHorizonColor) params->skydomeHorizonColor = m_skydomeHorizonColor.value();
    if (m_skydomeCloudColor) params->skydomeCloudColor = m_skydomeCloudColor.value();
    if (m_skydomeOverlayColor) params->skydomeOverlayColor = m_skydomeOverlayColor.value();
}

//==============================================================================
// Level

LN_OBJECT_IMPLEMENT(Level, AssetObject) {}

Level::Level()
    : m_ownerWorld(nullptr)
	, m_rootWorldObjectList(makeList<Ref<WorldObject>>())
    , m_updateMode(LevelUpdateMode::PauseWhenInactive)
	, m_initialUpdate(true)
{

}

Level::~Level()
{
}

bool Level::init()
{
    return AssetObject::init();
}

void Level::addSubLevel(Level* level)
{
    if (LN_REQUIRE(level)) return;
    if (LN_REQUIRE(!level->m_parentLevel)) return;
    acquireSubLevelManager()->addSubLevel(level);
    level->m_parentLevel = this;
}

void Level::removeSubLevel(Level* level)
{
    if (LN_REQUIRE(level)) return;
    if (LN_REQUIRE(level->m_parentLevel == this)) return;
    acquireSubLevelManager()->removeSubLevel(level);
    level->m_parentLevel = nullptr;
}

void Level::removeAllSubLevels()
{
    auto* manager = acquireSubLevelManager();
    for (auto& level : manager->subLevels()) {
        level->m_parentLevel = nullptr; 
    }
    manager->removeAllSubLevels();
}

// Note: このあたりの命名規則は Tkool からとっている。

void Level::onStart()
{
}

void Level::onStop()
{
}

void Level::onResume()
{
}

void Level::onPause()
{
}

void Level::onUpdate()
{
}

void Level::onPostUpdate(float elapsedSeconds)
{
    for (WorldObject* obj : m_destroyList) {
        //obj->removeFromWorld();

        obj->detachScene();
        m_rootWorldObjectList->remove(obj);
    }
    m_destroyList.clear();
}

void Level::update(float elapsedSeconds)
{
    if (m_subLevelManager) {
        for (auto& level : m_subLevelManager->subLevels()) {
            level->update(elapsedSeconds);
        }
    }

    // onUpdate 内で新しい WorldObject が作成され、m_rootWorldObjectList に add される場合に備えて
    // イテレータによる列挙は行わない。新しく追加されたものは、このループで
    // 今のフレーム中の最初の onUpdate が呼ばれる。
    for (int i = 0; i < m_rootWorldObjectList->size(); i++) {
        m_rootWorldObjectList[i]->updateFrame(elapsedSeconds);
    }

	onUpdate();
}


void Level::setup(const ln::Path& filePath)
{
	m_filePath = filePath;
}

void Level::save()
{
}

void Level::load()
{
}

void Level::clear()
{
	m_rootWorldObjectList->clear();
}

void Level::addObject(WorldObject* obj)
{
	if (LN_REQUIRE(obj)) return;
    if (obj->m_scene) {
        if (obj->m_scene == this) {
            return;
        }
        obj->m_scene->removeObject(obj);
    }

	m_rootWorldObjectList->add(obj);
    obj->attachScene(this);
}

void Level::removeObject(WorldObject* obj)
{
	m_rootWorldObjectList->remove(obj);
}

void Level::removeRootObject(WorldObject* obj)
{
    if (m_rootWorldObjectList->remove(obj)) {
        if (obj->destroyed()) {
            m_destroyList.remove(obj);
        }
        obj->detachScene();
    }
}

void Level::removeAllObjects()
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

void Level::mergeToRenderParams(detail::SceneGlobalRenderParams* params) const
{
    if (m_levelRenderParameters) {
        m_levelRenderParameters->mergeToRenderParams(params);
    }
}

LevelRenderParameters* Level::acquireRenderParameters()
{
    if (!m_levelRenderParameters) {
        m_levelRenderParameters = makeObject<LevelRenderParameters>();
    }
    return m_levelRenderParameters;
}

Level::SubLevelManager* Level::acquireSubLevelManager()
{
    if (!m_subLevelManager) {
        m_subLevelManager = std::make_unique<SubLevelManager>();
    }
    return m_subLevelManager.get();
}

// Multi-Lang 対応のため、テンプレートではなく基本は TypeInfo で検索する
WorldObject* Level::findObjectByComponentType(const TypeInfo* type) const
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

void Level::updateObjectsWorldMatrix() const
{
    for (auto& obj : m_rootWorldObjectList)
    {
        obj->updateWorldMatrixHierarchical();
    }
}

void Level::onPreUpdate(float elapsedSeconds)
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

void Level::collectRenderObjects(World* world, RenderingContext* context)
{
    if (m_subLevelManager) {
        m_subLevelManager->executeCommands();

        for (auto& level : m_subLevelManager->subLevels()) {
            level->collectRenderObjects(world, context);
        }
    }

    for (auto& obj : m_rootWorldObjectList) {
        // TODO: 不要なものははじいたり
        world->enqueueWorldRenderingElement(obj);

        for (auto& c : *(obj->m_components)) {
            c->onPrepareRender(context); // TODO: 全体の前にした方がいいかも
            // TODO: 不要なものははじいたり
            world->enqueueWorldRenderingElement(c);
        }
    }
}

void Level::renderGizmos(RenderingContext* context)
{
    for (auto& obj : m_rootWorldObjectList) {
        for (auto& c : *(obj->m_components)) {
            c->renderGizmo(context);
        }
    }
}

bool Level::traverse(detail::IWorldObjectVisitor* visitor) const
{
    for (auto& obj : m_rootWorldObjectList) {
        if (!obj->traverse(visitor)) {
            return false;
        }
    }
    return true;
}

void Level::serialize2(Serializer2& ar)
{
    AssetObject::serialize2(ar);

    if (ar.isLoading()) {
        removeAllObjects();
    }

    ar & ln::makeNVP(u"renderParams", m_levelRenderParameters);

    ar & ln::makeNVP(u"children", *m_rootWorldObjectList);

    if (ar.isLoading()) {
        for (auto& obj : m_rootWorldObjectList) {
            obj->attachScene(this);
        }
    }
}

//==============================================================================
// SubLevelManager

void Level::SubLevelManager::addSubLevel(Level* level)
{
    assert(level);
    m_commands.push_back({ CommandType::AddSubLevel, level });
}

void Level::SubLevelManager::removeSubLevel(Level* level)
{
    assert(level);
    m_commands.push_back({ CommandType::RemoveSubLevel, level });
}

void Level::SubLevelManager::removeAllSubLevels()
{
    m_commands.push_back({ CommandType::RemoveAllSubLevels, nullptr });
}

void Level::SubLevelManager::executeCommands()
{
    if (!m_commands.empty()) {
        for (const auto& command : m_commands) {
            switch (command.type) {
                case CommandType::AddSubLevel: {
                    m_subLevels.add(command.level);
                    command.level->onStart();
                    break;
                }
                case CommandType::RemoveSubLevel: {
                    command.level->onStop();
                    m_subLevels.remove(command.level);
                    break;
                }
                case CommandType::RemoveAllSubLevels: {
                    for (const auto& level : m_subLevels) {
                        command.level->onStop();
                    }
                    m_subLevels.clear();
                    break;
                }
            }
        }
        m_commands.clear();
    }
}

} // namespace ln

