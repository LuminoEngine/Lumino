
#ifdef _WIN32
#include <Windows.h>	// TODO: PlatformSupport.h の要求。必要ないようにしたい
#endif
#include "../Internal.h"
#include <Lumino/Engine.h>
#include <Lumino/Platform/PlatformSupport.h>

extern void Main();

namespace ln
{

//==============================================================================
// EntryPoint
//==============================================================================

//------------------------------------------------------------------------------
int EntryPoint()
{
	try
	{
		Main();
	}
	catch (Exception& e)
	{
		PlatformSupport::ShowAlertMessageBox(e.GetMessage());
	}
	Engine::Terminate();
	return 0;
}

}
