#pragma once

namespace ln {
struct SizeI;
struct Color;

namespace detail {
class PlatformWindow;
class IShaderUniform;

class ISwapChain
	: public RefObject
{
public:

protected:
	virtual ~ISwapChain() = default;
};

class IShaderPass
	: public RefObject
{
public:
	virtual int getUniformCount() const = 0;
	virtual IShaderUniform* getUniform(int index) const = 0;

protected:
	virtual ~IShaderPass() = default;
};

class IShaderUniform
	: public RefObject
{
public:

protected:
	virtual ~IShaderUniform() = default;
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
	Ref<IShaderPass> createShaderPass(const byte_t* vsCode, int vsCodeLen, const byte_t* fsCodeLen, int psCodeLen, ShaderCompilationDiag* diag);

	void clearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil);

	void present(ISwapChain* swapChain);

protected:
	virtual void onEnterMainThread() = 0;
	virtual void onLeaveMainThread() = 0;
	virtual Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) = 0;
	virtual Ref<IShaderPass> onCreateShaderPass(const byte_t* vsCode, int vsCodeLen, const byte_t* fsCodeLen, int psCodeLen, ShaderCompilationDiag* diag) = 0;
	virtual void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) = 0;
	virtual void onPresent(ISwapChain* swapChain) = 0;
};


} // namespace detail
} // namespace ln

