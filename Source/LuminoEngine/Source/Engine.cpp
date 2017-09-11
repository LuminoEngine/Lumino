
#include "Internal.h"
#include <Lumino/IO/Console.h>
#include <Lumino/Profiler.h>
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include <Lumino/Engine.h>
#include <Lumino/World.h>
#include "Graphics/CoreGraphicsRenderFeature.h"
#include "Graphics/ProfilerRenderer.h"
#include "Graphics/RenderingThread.h"
#include "Scene/SceneGraphManager.h"
#include <Lumino/UI/UIFrameWindow.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Camera.h>
#include "Effect/EffectManager.h"
#include "UI/UIManager.h"
#include "EngineManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Engine
//==============================================================================

//------------------------------------------------------------------------------
void Engine::initialize()
{
	if (LN_REQUIRE(EngineManager::Instance == nullptr)) return;
	EngineManager::Instance = EngineManager::create(detail::EngineSettings::instance);
	EngineManager::Instance->initialize();
}

//------------------------------------------------------------------------------
void Engine::terminate()
{
	LN_SAFE_RELEASE(EngineManager::Instance);
}

//------------------------------------------------------------------------------
bool Engine::update()
{
	if (LN_REQUIRE(EngineManager::Instance != nullptr)) return false;
	return EngineManager::Instance->updateUnitily();
}

//------------------------------------------------------------------------------
void Engine::updateFrame()
{
	if (LN_REQUIRE(EngineManager::Instance != nullptr)) return;
	EngineManager::Instance->updateFrame();
}

//------------------------------------------------------------------------------
//bool Engine::BeginRendering()
//{
//	return EngineManager::Instance->BeginRendering();
//}

//------------------------------------------------------------------------------
void Engine::renderFrame()
{
	EngineManager::Instance->renderFrame();
}

//------------------------------------------------------------------------------
void Engine::presentFrame()
{
	EngineManager::Instance->presentFrame();
}

//------------------------------------------------------------------------------
bool Engine::isEndRequested()
{
	return EngineManager::Instance->isEndRequested();
}

//------------------------------------------------------------------------------
void Engine::exit()
{
	EngineManager::Instance->exit();
}

//------------------------------------------------------------------------------
void Engine::setFrameUpdateMode(FrameUpdateMode mode)
{
	EngineManager::Instance->setFrameUpdateMode(mode);
}

//------------------------------------------------------------------------------
void Engine::resetFrameDelay()
{
	EngineManager::Instance->resetFrameDelay();
}

//------------------------------------------------------------------------------
World2D* Engine::getWorld2D()
{
	return EngineManager::Instance->getDefaultWorld2D();
}

//------------------------------------------------------------------------------
World3D* Engine::getWorld3D()
{
	return EngineManager::Instance->getDefaultWorld3D();
}

//------------------------------------------------------------------------------
Camera* Engine::getCamera3D()
{
	return EngineManager::Instance->getDefaultWorld3D()->getMainCamera();
}

//------------------------------------------------------------------------------
UIViewport* Engine::getMainViewport()
{
	return EngineManager::Instance->getUIManager()->getMainWindow()->getViewport();
}

//------------------------------------------------------------------------------
UIViewportLayer* Engine::getDefault2DLayer()
{
	return EngineManager::Instance->getUIManager()->getMainWindow()->getDefaultCameraViewportLayer2D();
}

//------------------------------------------------------------------------------
UIViewportLayer* Engine::getDefault3DLayer()
{
	return EngineManager::Instance->getUIManager()->getMainWindow()->getDefaultCameraViewportLayer3D();
}

//------------------------------------------------------------------------------
UILayoutLayer* Engine::getDefaultUILayer()
{
	return EngineManager::Instance->getUIManager()->getMainWindow()->getDefaultUILayer();
}

////------------------------------------------------------------------------------
//SceneGraph2D* Engine::getDefaultSceneGraph2D()
//{
//	return EngineManager::Instance->getDefaultWorld2D()->getSceneGraph2D();
//}
//
////------------------------------------------------------------------------------
//SceneGraph3D* Engine::getDefaultSceneGraph3D()
//{
//	return EngineManager::Instance->getDefaultWorld3D()->getSceneGraph3D();
//}

//------------------------------------------------------------------------------
//LightComponent* Engine::GetMainLight3D()
//{
//	return EngineManager::Instance->getDefaultWorld3D()->getSceneGraph3D()->GetMainLight();
//}

LN_NAMESPACE_END
