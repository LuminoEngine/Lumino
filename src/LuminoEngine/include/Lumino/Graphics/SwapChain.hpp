#pragma once

namespace ln {
namespace detail { class PlatformWindow; }
class ISwapChain;

class LN_API SwapChain
	: public Object
{
public:
	virtual void dispose();

LN_CONSTRUCT_ACCESS:
	SwapChain();
	virtual ~SwapChain();
	void initialize(detail::PlatformWindow* window, const SizeI& backbufferSize);

private:
	Ref<ISwapChain> m_rhiObject;
};

} // namespace ln
