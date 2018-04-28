#pragma once

#include "Common.hpp"
#include "GeometryStructs.hpp"
#include "ColorStructs.hpp"

namespace ln {
namespace detail {
	class PlatformWindow;
	class GraphicsManager;
	class IGraphicsDeviceContext;
	class ISwapChain;
}


class LN_API SwapChain
	: public Object
{
public:
	virtual void dispose();

LN_CONSTRUCT_ACCESS:
	SwapChain();
	virtual ~SwapChain();
	void initialize(detail::PlatformWindow* window, const SizeI& backbufferSize);

LN_INTERNAL_ACCESS:
	detail::ISwapChain* resolveRHIObject() const;

private:
	Ref<detail::ISwapChain> m_rhiObject;
};

class LN_API GraphicsContext
	: public Object
{
public:

	void clear(ClearFlags flags, const Color& color, float z, uint8_t stencil);

	void present(SwapChain* swapChain);


LN_CONSTRUCT_ACCESS:
	GraphicsContext();
	virtual ~GraphicsContext();
	void initialize(detail::IGraphicsDeviceContext* device);

LN_INTERNAL_ACCESS:
	virtual void dispose();

private:
	detail::IGraphicsDeviceContext* m_rhiObject;
};

} // namespace ln
