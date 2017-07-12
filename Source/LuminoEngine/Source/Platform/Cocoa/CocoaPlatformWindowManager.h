
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
	virtual void createMainWindow(const WindowCreationSettings& settings) override;
	virtual PlatformWindow* getMainWindow() override;
	virtual PlatformWindow* createSubWindow(const WindowCreationSettings& settings) override;
	virtual void doEvents() override;
	virtual void Finalize() override;

private:
	CocoaPlatformWindow*	m_mainWindow;
	id						m_appDelegate;
};

LN_NAMESPACE_END
