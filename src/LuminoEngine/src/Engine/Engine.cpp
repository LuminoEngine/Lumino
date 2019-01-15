
#include "Internal.hpp"
#include "EngineManager.hpp"
//#include "EngineDomain.hpp"
#include <LuminoEngine/Engine/Engine.hpp>
#include <LuminoEngine/Engine/EngineSettings.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/UIViewport.hpp>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
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

void EngineSettings::setEngineFeatures(Flags<EngineFeature> features)
{
    detail::EngineDomain::engineManager()->settings().features = features;
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
	detail::EngineDomain::engineManager()->initialize();
	beginFrame();
}

void Engine::finalize()
{
	endFrame();
	detail::EngineDomain::release();
}

bool Engine::update()
{
	endFrame();
	beginFrame();
	return !detail::EngineDomain::engineManager()->isExitRequested();
}

void Engine::quit()
{
    detail::EngineDomain::engineManager()->quit();
}

void Engine::resetFrameDelay()
{
    detail::EngineDomain::engineManager()->resetFrameDelay();
}

GraphicsContext* Engine::graphicsContext()
{
	return detail::EngineDomain::graphicsManager()->graphicsContext();
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

PhysicsWorld* Engine::mainPhysicsWorld()
{
    return detail::EngineDomain::engineManager()->mainPhysicsWorld();
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

