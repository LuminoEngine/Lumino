#pragma once

namespace ln {
namespace detail { class PlatformWindow; }
class ISwapChain;

class LN_API SwapChain
	: public Object
{
LN_CONSTRUCT_ACCESS:
	SwapChain();
	virtual ~SwapChain();
	void initialize(detail::PlatformWindow* window, const SizeI& backbufferSize);
	virtual void dispose();

private:
	Ref<ISwapChain> m_rhiObject;
};

} // namespace ln
