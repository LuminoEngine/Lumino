

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
		PlatformSupport::ShowAlertMessageBox(e.getMessage());
	}
	Engine::terminate();
	return 0;
}

}
