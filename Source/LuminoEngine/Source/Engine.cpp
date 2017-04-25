
#include "Internal.h"
#include <Lumino/IO/Console.h>
#include <Lumino/Profiler.h>
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include <Lumino/Engine.h>
#include <Lumino/World.h>
#include "Graphics/RendererImpl.h"
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
void Engine::Initialize()
{
	if (LN_CHECK_STATE(EngineManager::Instance == nullptr)) return;
	EngineManager::Instance = EngineManager::Create(detail::EngineSettings::instance);
	EngineManager::Instance->Initialize();
}

//------------------------------------------------------------------------------
void Engine::Terminate()
{
	LN_SAFE_RELEASE(EngineManager::Instance);
}

//------------------------------------------------------------------------------
bool Engine::Update()
{
	if (LN_CHECK_STATE(EngineManager::Instance != nullptr)) return false;
	return EngineManager::Instance->UpdateUnitily();
}

//------------------------------------------------------------------------------
void Engine::UpdateFrame()
{
	if (LN_CHECK_STATE(EngineManager::Instance != nullptr)) return;
	EngineManager::Instance->UpdateFrame();
}

//------------------------------------------------------------------------------
//bool Engine::BeginRendering()
//{
//	return EngineManager::Instance->BeginRendering();
//}

//------------------------------------------------------------------------------
void Engine::RenderFrame()
{
	EngineManager::Instance->RenderFrame();
}

//------------------------------------------------------------------------------
void Engine::PresentFrame()
{
	EngineManager::Instance->PresentFrame();
}

//------------------------------------------------------------------------------
bool Engine::IsEndRequested()
{
	return EngineManager::Instance->IsEndRequested();
}

//------------------------------------------------------------------------------
void Engine::Exit()
{
	EngineManager::Instance->Exit();
}

//------------------------------------------------------------------------------
void Engine::SetFrameUpdateMode(FrameUpdateMode mode)
{
	EngineManager::Instance->SetFrameUpdateMode(mode);
}

//------------------------------------------------------------------------------
void Engine::ResetFrameDelay()
{
	EngineManager::Instance->ResetFrameDelay();
}

//------------------------------------------------------------------------------
World* Engine::GetWorld2D()
{
	return EngineManager::Instance->GetDefaultWorld2D();
}

//------------------------------------------------------------------------------
World* Engine::GetWorld3D()
{
	return EngineManager::Instance->GetDefaultWorld3D();
}

//------------------------------------------------------------------------------
UIMainWindow* Engine::GetMainWindow()
{
	return EngineManager::Instance->GetUIManager()->GetMainWindow();
}

//------------------------------------------------------------------------------
UIViewport* Engine::GetMainViewport()
{
	return EngineManager::Instance->GetUIManager()->GetMainWindow()->GetViewport();
}

////------------------------------------------------------------------------------
//ViewportLayer* Engine::GetDefault2DLayer()
//{
//	return EngineManager::Instance->GetUIManager()->GetMainWindow()->GetDefault2DCameraViewportLayer();
//}
//
////------------------------------------------------------------------------------
//ViewportLayer* Engine::GetDefault3DLayer()
//{
//	return EngineManager::Instance->GetUIManager()->GetMainWindow()->GetDefault3DCameraViewportLayer();
//}
//
//------------------------------------------------------------------------------
SceneGraph2D* Engine::GetDefaultSceneGraph2D()
{
	return EngineManager::Instance->GetDefaultWorld2D()->GetSceneGraph2D();
}

//------------------------------------------------------------------------------
SceneGraph3D* Engine::GetDefaultSceneGraph3D()
{
	return EngineManager::Instance->GetDefaultWorld3D()->GetSceneGraph3D();
}

//------------------------------------------------------------------------------
Light* Engine::GetMainLight3D()
{
	return EngineManager::Instance->GetDefaultWorld3D()->GetSceneGraph3D()->GetMainLight();
}

LN_NAMESPACE_END
