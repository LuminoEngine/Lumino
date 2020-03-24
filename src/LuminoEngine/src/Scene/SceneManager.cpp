
#include "Internal.hpp"
#include "SceneManager.hpp"
#include <LuminoEngine/Scene/Raycaster.hpp>

// for registerType
#include <LuminoEngine/Asset/Assets.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Sprite.hpp>
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Tilemap/Tileset.hpp>
#include <LuminoEngine/Tilemap/TilemapLayer.hpp>
#include <LuminoEngine/Tilemap/TilemapModel.hpp>
#include <LuminoEngine/Tilemap/TilemapComponent.hpp>
#include <LuminoEngine/Tilemap/Tilemap.hpp>

namespace ln {
namespace detail {

//==============================================================================
// SceneManager

SceneManager::SceneManager()
    : m_activeWorld(nullptr)
	, m_raycasterCache(makeObject<Raycaster>())
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::init()
{
    LN_LOG_DEBUG << "SceneManager Initialization started.";


	// ClusteredShadingDefault.lcfx.h
	{
		static const unsigned char data[] =
		{
#include "Resource/SkyFromAtmosphere.lcfx.inl"
		};
		static const size_t size = LN_ARRAY_SIZE_OF(data);
		MemoryStream stream(data, size);
		m_atmosphereShader = makeObject<Shader>(u"SkyFromAtmosphere", &stream);
	}

#if 0
    m_atmosphereShader = Shader::create(u"D:/Proj/LN/Lumino/src/LuminoEngine/src/Scene/Resource/SkyFromAtmosphere.fx");
#endif

	m_primitiveMeshDefaultMaterial = makeObject<Material>();
	m_primitiveMeshDefaultMaterial->setColor(Color(0.8f, 0.8f, 0.8f, 1.0));
	m_primitiveMeshDefaultMaterial->setRoughness(0.5f);
	m_primitiveMeshDefaultMaterial->setMetallic(0.0f);

    EngineDomain::registerType<World>();
    EngineDomain::registerType<Scene>();
    EngineDomain::registerType<Sprite>();

    EngineDomain::registerType<Tileset>();
    EngineDomain::registerType<TilemapLayer>();
    EngineDomain::registerType<TilemapModel>();
    EngineDomain::registerType<TilemapComponent>();
    EngineDomain::registerType<Tilemap>();

    LN_LOG_DEBUG << "SceneManager Initialization ended.";
}

void SceneManager::dispose()
{
	releaseAndTerminateAllRunningScenes();
}

Scene* SceneManager::loadScene(const StringRef& sceneAssetFilePath)
{
    auto scene = dynamic_pointer_cast<Scene>(Assets::loadAsset(sceneAssetFilePath));
    activeWorld()->addScene(scene);
    return nullptr;
}

void SceneManager::unloadScene(Scene* scene)
{
    activeWorld()->m_sceneList->remove(scene);
}

#if 0
void SceneManager::gotoScene(Scene* scene)
{
	if (LN_REQUIRE(scene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Goto;
	c.scene = scene;
	m_eventQueue.push_back(c);
}

void SceneManager::callScene(Scene* scene)
{
	if (LN_REQUIRE(scene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Call;
	c.scene = scene;
	m_eventQueue.push_back(c);
}

void SceneManager::returnScene()
{
	if (LN_REQUIRE(m_activeScene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Return;
	c.scene = nullptr;
	m_eventQueue.push_back(c);
}
#endif

void SceneManager::updateFrame()
{
	executeCommands();
	//if (m_activeScene != nullptr)
	//{
	//	m_activeScene->update();
	//}
}

void SceneManager::executeCommands()
{
	while (!m_eventQueue.empty())
	{
		const EventCommsnd& cmd = m_eventQueue.front();

		switch (cmd.type)
		{
			/////////////// 直接遷移
			case EventType::Goto:
			{
				// 現在の全てのシーンを解放 (onTerminate() 呼び出し)
				releaseAndTerminateAllRunningScenes();

				m_activeScene = cmd.scene;
				if (m_activeScene != nullptr)
				{
					m_activeScene->onStart();
                    m_activeScene->onActivated();
				}
				break;
			}
			/////////////// 呼び出し
			case EventType::Call:
			{
				m_sceneStack.push(m_activeScene);
				m_activeScene = cmd.scene;
				m_activeScene->onStart();
                m_activeScene->onActivated();
				break;
			}
			/////////////// 呼び出し元へ戻る
			case EventType::Return:
			{
				Ref<Scene> oldScene = m_activeScene;
				m_activeScene = m_sceneStack.top();
                oldScene->onDeactivated();
				oldScene->onClosed();
				break;
			}
		}

		m_eventQueue.pop_front();
	}
}

void SceneManager::releaseAndTerminateAllRunningScenes()
{
	if (m_activeScene != nullptr)
	{
		m_activeScene->onClosed();
		m_activeScene = nullptr;
	}

	while (!m_sceneStack.empty())
	{
		m_sceneStack.top()->onClosed();
		m_sceneStack.pop();
	}
}

} // namespace detail
} // namespace ln
