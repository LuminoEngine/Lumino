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
	virtual void getFramebufferSize(int* width, int* height) override { *width = 0; *height = 0; }
	virtual PointI pointFromScreen(const PointI& screenPoint) override { return screenPoint; }
	virtual PointI pointToScreen(const PointI& clientPoint) override { return clientPoint; }

private:
};

class EmptyPlatformWindowManager
	: public PlatformWindowManager
{
public:
	EmptyPlatformWindowManager();
	virtual ~EmptyPlatformWindowManager();

    Result init();
	virtual void dispose() override;
	virtual Ref<PlatformWindow> createWindow(const WindowCreationSettings& settings) override;
	virtual void destroyWindow(PlatformWindow* window) override;
	virtual void processSystemEventQueue() override;
};

} // namespace detail
} // namespace ln
