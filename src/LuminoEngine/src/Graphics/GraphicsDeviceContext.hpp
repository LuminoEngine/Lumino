﻿#pragma once

namespace ln {
namespace detail {
class PlatformWindow;

class ISwapChain
	: public RefObject
{
public:

protected:
	virtual ~ISwapChain() = default;
};

class IGraphicsDeviceContext
	: public RefObject
{
public:
	IGraphicsDeviceContext();
	virtual ~IGraphicsDeviceContext() = default;

	void initialize();
	virtual void dispose();
	void enterMainThread();
	void leaveMainThread();
	//void enterRenderingThread();
	//void leaveRenderingThread();

	Ref<ISwapChain> createSwapChain(PlatformWindow* window, const SizeI& backbufferSize);

	void clearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil);

	void present(ISwapChain* swapChain);

protected:
	virtual void onEnterMainThread() = 0;
	virtual void onLeaveMainThread() = 0;
	virtual Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) = 0;
	virtual void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) = 0;
	virtual void onPresent(ISwapChain* swapChain) = 0;
};


} // namespace detail
} // namespace ln

