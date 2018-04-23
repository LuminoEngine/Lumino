#pragma onc

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

class GraphicsDeviceContext
	: public RefObject
{
public:
	GraphicsDeviceContext();
	virtual ~GraphicsDeviceContext() = default;

	void initialize();
	virtual void dispose();

	Ref<ISwapChain> createSwapChain(PlatformWindow* window, const SizeI& backbufferSize);

	void clearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil);

protected:
	virtual Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) = 0;
	virtual void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) = 0;
};

} // namespace detail
} // namespace ln

