
#include "../Internal.h"
#include <Lumino/Platform/PlatformSupport.h>
#include <Lumino/Platform/Win32/Win32PlatformWindow.h>
#include "../PlatformManager.h"

LN_NAMESPACE_BEGIN

//------------------------------------------------------------------------------
void PlatformSupport::ShowAlertMessageBox(const TCHAR* message) LN_NOEXCEPT
{
	HWND owner = NULL;
	PlatformManager* manager = PlatformManager::getInstance();
	if (manager != nullptr)
	{
		try
		{
			owner = GetWindowHandle(manager->getMainWindow());
		}
		catch (...)
		{
		}
	}

	::MessageBox(owner, message, NULL, MB_ICONERROR);
}

//------------------------------------------------------------------------------
HWND PlatformSupport::GetWindowHandle(PlatformWindow* window)
{
	Win32PlatformWindow* w = dynamic_cast<Win32PlatformWindow*>(window);
	LN_THROW(w, ArgumentException);
	return w->GetWindowHandle();
}

LN_NAMESPACE_END
