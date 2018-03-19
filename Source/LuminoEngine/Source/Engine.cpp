
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
	detail::EngineDomain::getEngineManager()->initialize(detail::EngineSettings::instance);
}

//------------------------------------------------------------------------------
void Engine::terminate()
{
	detail::EngineDomain::release();
}

//------------------------------------------------------------------------------
bool Engine::update()
{
	return detail::EngineDomain::getEngineManager()->updateUnitily();
}

//------------------------------------------------------------------------------
void Engine::updateFrame()
{
	detail::EngineDomain::getEngineManager()->updateFrame();
}

//------------------------------------------------------------------------------
//bool Engine::BeginRendering()
//{
//	return detail::EngineDomain::getEngineManager()->BeginRendering();
//}

//------------------------------------------------------------------------------
void Engine::renderFrame()
{
	detail::EngineDomain::getEngineManager()->renderFrame();
}

//------------------------------------------------------------------------------
void Engine::presentFrame()
{
	detail::EngineDomain::getEngineManager()->presentFrame();
}

//------------------------------------------------------------------------------
bool Engine::isEndRequested()
{
	return detail::EngineDomain::getEngineManager()->isEndRequested();
}

//------------------------------------------------------------------------------
void Engine::exit()
{
	detail::EngineDomain::getEngineManager()->exit();
}

//------------------------------------------------------------------------------
void Engine::setFrameUpdateMode(FrameUpdateMode mode)
{
	detail::EngineDomain::getEngineManager()->setFrameUpdateMode(mode);
}

//------------------------------------------------------------------------------
void Engine::resetFrameDelay()
{
	detail::EngineDomain::getEngineManager()->resetFrameDelay();
}

//------------------------------------------------------------------------------
World2D* Engine::defaultWorld2D()
{
	return detail::EngineDomain::getEngineManager()->defaultWorld2D();
}

//------------------------------------------------------------------------------
World3D* Engine::defaultWorld3D()
{
	return detail::EngineDomain::getEngineManager()->defaultWorld3D();
}

World2D* Engine::activeWorld2D()
{
	return detail::EngineDomain::getEngineManager()->activeWorld2D();
}

World3D* Engine::activeWorld3D()
{
	return detail::EngineDomain::getEngineManager()->activeWorld3D();
}

void Engine::setActiveWorld2D(World2D* world)
{
	return detail::EngineDomain::getEngineManager()->setActiveWorld2D(world);
}

void Engine::setActiveWorld3D(World3D* world)
{
	return detail::EngineDomain::getEngineManager()->setActiveWorld3D(world);
}

//------------------------------------------------------------------------------
Camera* Engine::getCamera3D()
{
	return detail::EngineDomain::getEngineManager()->defaultWorld3D()->getMainCamera();
}

UIFrameWindow* Engine::getMainWindow()
{
	return detail::EngineDomain::getEngineManager()->getUIManager()->getMainWindow();
}

//------------------------------------------------------------------------------
UIViewport* Engine::getMainViewport()
{
	return detail::EngineDomain::getEngineManager()->getUIManager()->getMainWindow()->getViewport();
}

//------------------------------------------------------------------------------
RenderView* Engine::getDefault2DLayer()
{
	return detail::EngineDomain::getEngineManager()->getUIManager()->getMainWindow()->getDefaultCameraViewportLayer2D();
}

//------------------------------------------------------------------------------
RenderView* Engine::getDefault3DLayer()
{
	return detail::EngineDomain::getEngineManager()->getUIManager()->getMainWindow()->getDefaultCameraViewportLayer3D();
}

//------------------------------------------------------------------------------
UILayoutLayer* Engine::getDefaultUILayer()
{
	return detail::EngineDomain::getEngineManager()->getUIManager()->getMainWindow()->getDefaultUILayer();
}

LN_NAMESPACE_END
