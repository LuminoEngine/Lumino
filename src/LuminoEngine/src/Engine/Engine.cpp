
#include "Internal.hpp"
#include "EngineManager.hpp"
//#include "EngineDomain.hpp"
#include <LuminoEngine/Engine/Engine.hpp>
#include <LuminoEngine/Engine/EngineSettings.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/UIViewport.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Camera.hpp>
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

void Engine::terminate()
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

World* Engine::mainWorld()
{
    return detail::EngineDomain::engineManager()->mainWorld();
}

Camera* Engine::mainCamera()
{
    return detail::EngineDomain::engineManager()->mainCamera();
}

} // namespace ln

