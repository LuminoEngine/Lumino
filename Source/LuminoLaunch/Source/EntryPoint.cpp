

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
