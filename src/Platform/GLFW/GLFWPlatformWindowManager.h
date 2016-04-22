
#pragma once
#include "../PlatformWindowManagerBase.h"
#include "GLFWPlatformWindow.h"

LN_NAMESPACE_BEGIN

class GLFWPlatformWindowManager
    : public WindowManagerBase
{
public:
    GLFWPlatformWindowManager();
	virtual ~GLFWPlatformWindowManager();
	void Initialize();

public:
	// WindowManagerBase interface
	virtual void CreateMainWindow(const WindowCreationSettings& settings) override;
	virtual PlatformWindow* GetMainWindow() override;
	virtual PlatformWindow* CreateSubWindow(const WindowCreationSettings& settings) override;
	virtual void DoEvents() override;
	virtual void Finalize() override;

private:
	RefPtr<GLFWPlatformWindow>	m_mainWindow;
};

LN_NAMESPACE_END
