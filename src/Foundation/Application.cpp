
#include "../Internal.h"
#include <Lumino/Engine.h>
#include <Lumino/Foundation/Application.h>

LN_NAMESPACE_BEGIN

//=============================================================================
// GameApplication
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GameApplication::GameApplication()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GameApplication::~GameApplication()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameApplication::OnConfigure(EngineSettings* settings)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameApplication::Run()
{
	try
	{
		EngineSettings data;
		data.GraphicsAPI = GraphicsAPI::DirectX9;
		data.RenderingType = RenderingType::Immediate;

		OnConfigure(&data);

		Engine::Initialize(data);

		do
		{

		} while (Engine::UpdateFrame());
	}
	catch (...)
	{
		Engine::Finalize();
		throw;
	}
	Engine::Finalize();
}

LN_NAMESPACE_END
