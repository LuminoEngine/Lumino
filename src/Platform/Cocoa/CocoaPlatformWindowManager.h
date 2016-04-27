
#pragma once
#include "../PlatformWindowManagerBase.h"
#include "CocoaPlatformWindow.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// CocoaPlatformWindowManager
//=============================================================================
class CocoaPlatformWindowManager
    : public WindowManagerBase
{
public:
    CocoaPlatformWindowManager();
	virtual ~CocoaPlatformWindowManager();
	void Initialize();

public:
	// WindowManagerBase interface
	virtual void CreateMainWindow(const WindowCreationSettings& settings) override;
	virtual PlatformWindow* GetMainWindow() override;
	virtual PlatformWindow* CreateSubWindow(const WindowCreationSettings& settings) override;
	virtual void DoEvents() override;
	virtual void Finalize() override;

private:
	CocoaPlatformWindow*	m_mainWindow;
	id						m_appDelegate;
};

LN_NAMESPACE_END
