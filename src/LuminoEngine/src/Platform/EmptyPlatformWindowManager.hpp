#pragma once

#include "PlatformWindowManager.hpp"

namespace ln {
namespace detail {

class EmptyPlatformWindow
	: public PlatformWindow
{
public:
	EmptyPlatformWindow();
    Result init(const WindowCreationSettings& settings);
    virtual void setWindowTitle(const String& title) override {}
	virtual void getSize(SizeI* size) override { *size = SizeI(); }
	virtual void setSize(const SizeI& size) override { LN_NOTIMPLEMENTED(); }
	virtual void getFramebufferSize(int* width, int* height) override { *width = 0; *height = 0; }
	virtual void setAllowDragDrop(bool value) override;
	virtual bool isAllowDragDrop() const override;
	virtual PointI pointFromScreen(const PointI& screenPoint) override { return screenPoint; }
	virtual PointI pointToScreen(const PointI& clientPoint) override { return clientPoint; }
	virtual void grabCursor() override {}
	virtual void releaseCursor() override {}

private:
};

class EmptyPlatformWindowManager
	: public PlatformWindowManager
{
public:
	EmptyPlatformWindowManager(PlatformManager* manager);
	virtual ~EmptyPlatformWindowManager();

    Result init();
	virtual void dispose() override;
	virtual Ref<PlatformWindow> createMainWindow(const WindowCreationSettings& settings) override;
	virtual Ref<PlatformWindow> createSubWindow(const WindowCreationSettings& settings) override;
	virtual void destroyWindow(PlatformWindow* window) override;
	virtual void processSystemEventQueue(EventProcessingMode mode) override;
	virtual OpenGLContext* getOpenGLContext() const override;
};

} // namespace detail
} // namespace ln
