
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
    virtual void setVisible(bool visible) override;
    virtual void setTitleText(const StringRef& title) override;
    virtual const String& getTitleText() const override;
	virtual void setFullScreenEnabled(bool enabled) override;
	virtual bool isFullScreenEnabled() const override;

private:
	id		m_window;
    id      m_view;
	id		m_delegate;
    String  m_title;
};

LN_NAMESPACE_END
