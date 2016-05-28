
#include "Internal.h"
#include <Lumino/IO/Console.h>
#include <Lumino/Profiler.h>
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include <Lumino/Engine.h>
#include "Graphics/RendererImpl.h"
#include "Graphics/ProfilerRenderer.h"
#include "Graphics/RenderingThread.h"
#include "Scene/SceneGraphManager.h"
#include <Lumino/Scene/SceneGraph.h>
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
	LN_CHECK_STATE(EngineManager::Instance == nullptr);
	EngineManager::Instance = EngineManager::Create(detail::EngineSettings::instance);
	EngineManager::Instance->Initialize();
}

//------------------------------------------------------------------------------
void Engine::Terminate()
{
	LN_SAFE_RELEASE(EngineManager::Instance);
}

//------------------------------------------------------------------------------
bool Engine::UpdateFrame()
{
	LN_CHECK_STATE(EngineManager::Instance != NULL);
	return EngineManager::Instance->UpdateFrame();
}

//------------------------------------------------------------------------------
bool Engine::BeginRendering()
{
	return EngineManager::Instance->BeginRendering();
}

//------------------------------------------------------------------------------
void Engine::EndRendering()
{
	EngineManager::Instance->EndRendering();
}

//------------------------------------------------------------------------------
void Engine::Render()
{
	EngineManager::Instance->Render();
}

//------------------------------------------------------------------------------
void Engine::Exit()
{
	EngineManager::Instance->Exit();
}

//------------------------------------------------------------------------------
void Engine::SetFixedDeltaTime(float deltaTime)
{
	EngineManager::Instance->SetFixedDeltaTime(deltaTime);
}

//------------------------------------------------------------------------------
void Engine::ResetFrameDelay()
{
	EngineManager::Instance->ResetFrameDelay();
}

LN_NAMESPACE_END
