
#include "Internal.hpp"
#include "EngineManager.hpp"
//#include "EngineDomain.hpp"
#include <LuminoEngine/Engine/Engine.hpp>
#include <LuminoEngine/Engine/EngineSettings.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/UI/UIContext.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/UIViewport.hpp>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Physics/PhysicsWorld2D.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Camera.hpp>
#include <LuminoEngine/Scene/Light.hpp>
#include "../Graphics/GraphicsManager.hpp"

namespace ln {

//==============================================================================
// Engine

/** メインウィンドウのクライアント領域の幅と高さを設定します。(default: 640x480) */
void EngineSettings::setMainWindowSize(int width, int height)
{
	detail::EngineDomain::engineManager()->settings().mainWindowSize.set(width, height);
}

void EngineSettings::setMainBackBufferSize(int width, int height)
{
	detail::EngineDomain::engineManager()->settings().mainBackBufferSize.set(width, height);
}

void EngineSettings::setMainWindowTitle(const StringRef& title)
{
	detail::EngineDomain::engineManager()->settings().mainWindowTitle = title;
}

void EngineSettings::addAssetDirectory(const ln::StringRef& path)
{
	detail::EngineDomain::engineManager()->settings().assetDirectories.add(path);
}

void EngineSettings::addAssetArchive(const ln::StringRef& fileFullPath, const ln::StringRef& password)
{
	detail::EngineDomain::engineManager()->settings().assetArchives.add({ fileFullPath, password });
}

void EngineSettings::setAssetStorageAccessPriority(AssetStorageAccessPriority value)
{
	detail::EngineDomain::engineManager()->settings().assetStorageAccessPriority = value;
}

void EngineSettings::setGraphicsAPI(GraphicsAPI value)
{
	detail::EngineDomain::engineManager()->settings().graphicsAPI = value;
}

void EngineSettings::setUserMainWindow(intptr_t value)
{
    detail::EngineDomain::engineManager()->settings().userMainWindow = value;
}

void EngineSettings::setStandaloneFpsControl(bool enabled)
{
    detail::EngineDomain::engineManager()->settings().standaloneFpsControl = enabled;
}

void EngineSettings::setEngineFeatures(Flags<EngineFeature> features)
{
    detail::EngineDomain::engineManager()->settings().features = features;
}

void EngineSettings::setDefaultObjectsCreation(bool value)
{
    detail::EngineDomain::engineManager()->settings().defaultObjectsCreation = value;
}

void EngineSettings::setUseGLFWWindowSystem(bool value)
{
    detail::EngineDomain::engineManager()->settings().useGLFWWindowSystem = value;
}

void EngineSettings::setGraphicsContextManagement(bool value)
{
    detail::EngineDomain::engineManager()->settings().graphicsContextManagement = value;
}

void EngineSettings::setExternalMainLoop(bool value)
{
    detail::EngineDomain::engineManager()->settings().externalMainLoop = value;
}

void EngineSettings::setExternalRenderingManagement(bool value)
{
    detail::EngineDomain::engineManager()->settings().externalRenderingManagement = value;
}

//==============================================================================
// Engine

static void beginFrame()
{
	detail::EngineDomain::engineManager()->updateFrame();
	detail::EngineDomain::engineManager()->renderFrame();
}

static void endFrame()
{
	detail::EngineDomain::engineManager()->presentFrame();
}

void Engine::initialize()
{
    detail::EngineManager* manager = detail::EngineDomain::engineManager();
    manager->init();
    if (manager->settings().externalMainLoop) {
        beginFrame();
    }
}

void Engine::finalize()
{
    detail::EngineManager* manager = detail::EngineDomain::engineManager();
    if (manager->settings().externalMainLoop) {
        endFrame();
    }
	detail::EngineDomain::release();
}

bool Engine::update()
{
    detail::EngineManager* manager = detail::EngineDomain::engineManager();
    if (manager->settings().externalMainLoop) {
        endFrame();
        beginFrame();
    }
    else {
        beginFrame();
        endFrame();
    }
	return !manager->isExitRequested();
}

void Engine::quit()
{
    detail::EngineDomain::engineManager()->quit();
}

void Engine::resetFrameDelay()
{
    detail::EngineDomain::engineManager()->resetFrameDelay();
}

double Engine::totalTime()
{
    return detail::EngineDomain::engineManager()->fpsController().getTotalGameTime();
}

void Engine::setTimeScale(float value)
{
    detail::EngineDomain::engineManager()->setTimeScale(value);
}

const Path& Engine::persistentDataPath()
{
	return detail::EngineDomain::engineManager()->persistentDataPath();
}

//void Engine::setShowDebugFpsEnabled(bool enabled)
//{
//    return detail::EngineDomain::engineManager()->setShowDebugFpsEnabled(enabled);
//}

GraphicsContext* Engine::graphicsContext()
{
	return detail::EngineDomain::engineManager()->mainWindow()->m_renderingGraphicsContext;
	//return detail::EngineDomain::graphicsManager()->mainWindowGraphicsContext();
}

UIContext* Engine::mainUIContext()
{
    return detail::EngineDomain::engineManager()->mainUIContext();
}

UIFrameWindow* Engine::mainWindow()
{
	return detail::EngineDomain::engineManager()->mainWindow();
}

UIViewport* Engine::mainViewport()
{
    return detail::EngineDomain::engineManager()->mainViewport();
}

UIContainerElement* Engine::mainUIRoot()
{
	return detail::EngineDomain::engineManager()->mainUIRoot();
}

Size Engine::mainViewSize()
{
    return mainViewport()->actualViewboxSize();
}

World* Engine::mainWorld()
{
    return detail::EngineDomain::engineManager()->mainWorld();
}

Camera* Engine::mainCamera()
{
    return detail::EngineDomain::engineManager()->mainCamera();
}

AmbientLight* Engine::mainAmbientLight()
{
    return detail::EngineDomain::engineManager()->mainAmbientLight();
}

DirectionalLight* Engine::mainDirectionalLight()
{
    return detail::EngineDomain::engineManager()->mainDirectionalLight();
}

WorldRenderView* Engine::mainRenderView()
{
    return detail::EngineDomain::engineManager()->mainRenderView();
}

PhysicsWorld* Engine::mainPhysicsWorld()
{
    return detail::EngineDomain::engineManager()->mainPhysicsWorld();
}

PhysicsWorld2D* Engine::mainPhysicsWorld2D()
{
    return detail::EngineDomain::engineManager()->mainPhysicsWorld2D();
}


// TODO: time
//Engine::elapsedSeconds()
//Engine::totalRealTime()
//Engine::frameCount()
//
//World::elapsedSeconds()
//World::totalTime()
//Engine::frameCount()
//World::timeScale


} // namespace ln

