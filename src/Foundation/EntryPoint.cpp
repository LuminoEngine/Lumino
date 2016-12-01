
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
