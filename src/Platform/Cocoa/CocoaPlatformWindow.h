
#pragma once
#if defined(__OBJC__)
#import <Cocoa/Cocoa.h>
#else
#include <ApplicationServices/ApplicationServices.h>
typedef void* id;
#endif
#include <Lumino/Platform/PlatformWindow.h>

LN_NAMESPACE_BEGIN
class CocoaPlatformWindowManager;

class CocoaPlatformWindow
	: public PlatformWindow
{
public:
	CocoaPlatformWindow(CocoaPlatformWindowManager* manager);
	virtual ~CocoaPlatformWindow();
	void Initialize(CocoaPlatformWindowManager* windowManager, String windowTitle, int width, int height, bool fullscreen, bool resizable);
    id GetView() const { return m_view; }
    
public:
	// PlatformWindow interface
	virtual void SetVisible(bool visible) override;
	virtual void SetFullScreenEnabled(bool enabled) override;
	virtual bool IsFullScreenEnabled() const override;

private:
	id		m_window;
    id      m_view;
	id		m_delegate;
};

LN_NAMESPACE_END
