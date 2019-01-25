
#pragma once

#include <vulkan/vulkan.h>
#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {
class VulkanSwapChain;
class VulkanIndexBuffer;
class VulkanRenderTargetTexture;
class VulkanShaderPass;
class VulkanShaderUniformBuffer;
class VulkanShaderUniform;
class VulkanLocalShaderSamplerBuffer;

class VulkanDeviceContext
    : public IGraphicsDeviceContext
{
public:
    struct Settings
    {
        PlatformWindow* mainWindow = nullptr;
    };

    VulkanDeviceContext();
    virtual ~VulkanDeviceContext();
    void init(const Settings& settings);
    virtual void dispose() override;

protected:
    virtual void onGetCaps(GraphicsDeviceCaps* outCaps) override;
    virtual void onEnterMainThread() override;
    virtual void onLeaveMainThread() override;
    virtual void onSaveExternalRenderState() override;
    virtual void onRestoreExternalRenderState() override;
    virtual Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
    virtual Ref<IVertexDeclaration> onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) override;
    virtual Ref<IVertexBuffer> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) override;
    virtual Ref<IIndexBuffer> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) override;
    virtual Ref<ITexture> onCreateTexture2D(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
    virtual Ref<ITexture> onCreateTexture3D(uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
    virtual Ref<ITexture> onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap) override;
    virtual Ref<IDepthBuffer> onCreateDepthBuffer(uint32_t width, uint32_t height) override;
    virtual Ref<ISamplerState> onCreateSamplerState(const SamplerStateData& desc) override;
    virtual Ref<IShaderPass> onCreateShaderPass(const byte_t* vsCode, int vsCodeLen, const byte_t* fsCodeLen, int psCodeLen, ShaderCompilationDiag* diag) override;
    virtual void onUpdatePipelineState(const BlendStateDesc& blendState, const RasterizerStateDesc& rasterizerState, const DepthStencilStateDesc& depthStencilState) override;
    virtual void onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, IDepthBuffer* depthBuffer) override;
    virtual void onUpdateRegionRects(const RectI& viewportRect, const RectI& scissorRect, const SizeI& targetSize) override;
    virtual void onUpdatePrimitiveData(IVertexDeclaration* decls, IVertexBuffer** vertexBuufers, int vertexBuffersCount, IIndexBuffer* indexBuffer) override;
    virtual void onUpdateShaderPass(IShaderPass* newPass) override;
    virtual void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;
    virtual void onDrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount) override;
    virtual void onDrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount) override;
    virtual void onPresent(ISwapChain* swapChain) override;
};

class VulkanSwapChain
    : public ISwapChain
{
public:
    VulkanSwapChain();
    virtual ~VulkanSwapChain();
    virtual void dispose() override;
    virtual ITexture* getColorBuffer() const override;

private:
};

class VulkanVertexDeclaration
    : public IVertexDeclaration
{
public:
    VulkanVertexDeclaration();
    virtual ~VulkanVertexDeclaration();
    void init(const VertexElement* elements, int elementsCount);
    virtual void dispose() override;

private:
};

class VulkanVertexBuffer
    : public IVertexBuffer
{
public:
    VulkanVertexBuffer();
    virtual ~VulkanVertexBuffer();
    void init(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData);
    virtual void dispose() override;
    virtual size_t getBytesSize() override;
    virtual GraphicsResourceUsage usage() const override;
    virtual void setSubData(size_t offset, const void* data, size_t length) override;
    virtual void* map() override;
    virtual void unmap() override;

private:
};

class VulkanIndexBuffer
    : public IIndexBuffer
{
public:
    VulkanIndexBuffer();
    virtual ~VulkanIndexBuffer();
    void init(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData);
    virtual void dispose() override;
    virtual size_t getBytesSize() override;
    virtual GraphicsResourceUsage usage() const override;
    virtual void setSubData(size_t offset, const void* data, size_t length) override;
    virtual void* map() override;
    virtual void unmap() override;

private:
};

class VulkanTextureBase
    : public ITexture
{
public:
    VulkanTextureBase();
    virtual ~VulkanTextureBase();
};

class VulkanTexture2D
    : public VulkanTextureBase
{
public:
    VulkanTexture2D();
    virtual ~VulkanTexture2D();
    void init(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData);
    virtual void dispose() override;

    virtual DeviceTextureType type() const override;
    virtual void readData(void* outData) override;
    virtual const SizeI& realSize() override;
    virtual TextureFormat getTextureFormat() const override;
    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;
    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;

private:
};

class VulkanTexture3D
    : public VulkanTextureBase
{
public:
    VulkanTexture3D();
    virtual ~VulkanTexture3D();
    void init(uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData);
    virtual void dispose() override;

    virtual DeviceTextureType type() const override;
    virtual void readData(void* outData) override;
    virtual const SizeI& realSize() override;
    virtual TextureFormat getTextureFormat() const override;
    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;
    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;

private:
};

class VulkanRenderTargetTexture
    : public VulkanTextureBase
{
public:
    VulkanRenderTargetTexture();
    virtual ~VulkanRenderTargetTexture();
    void init(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap);
    virtual void dispose() override;

    virtual DeviceTextureType type() const override;
    virtual void readData(void* outData) override;
    virtual const SizeI& realSize() override;
    virtual TextureFormat getTextureFormat() const override;
    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;
    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;

private:
};

class VulkanDepthBuffer
    : public IDepthBuffer
{
public:
    VulkanDepthBuffer();
    virtual ~VulkanDepthBuffer();
    void init(uint32_t width, uint32_t height);
    virtual void dispose() override;

private:
};

class VulkanSamplerState
    : public ISamplerState
{
public:
    VulkanSamplerState();
    virtual ~VulkanSamplerState();
    void init(const SamplerStateData& desc);
    virtual void dispose() override;

private:
};

class VulkanShaderPass
    : public IShaderPass
{
public:
    VulkanShaderPass();
    virtual ~VulkanShaderPass();
    void init(VulkanDeviceContext* context, const byte_t* vsCode, int vsCodeLen, const byte_t* fsCodeLen, int psCodeLen, ShaderCompilationDiag* diag);
    virtual void dispose() override;

    virtual int getUniformCount() const override;
    virtual IShaderUniform* getUniform(int index) const override;
    virtual void setUniformValue(int index, const void* data, size_t size) override;
    virtual int getUniformBufferCount() const override;
    virtual IShaderUniformBuffer* getUniformBuffer(int index) const override;
    virtual IShaderSamplerBuffer* samplerBuffer() const override;

private:
};

class VulkanShaderUniformBuffer
    : public IShaderUniformBuffer
{
public:
    VulkanShaderUniformBuffer();
    virtual ~VulkanShaderUniformBuffer();
    void init();
    virtual void dispose() override;

    virtual const std::string& name() const override;
    virtual int getUniformCount() const override;
    virtual IShaderUniform* getUniform(int index) const override;
    virtual size_t bufferSize() const override;
    virtual void setData(const void* data, size_t size) override;

private:
    std::string m_name;
};

class VulkanShaderUniform
    : public IShaderUniform
{
public:
    VulkanShaderUniform();
    virtual ~VulkanShaderUniform();
    void init();
    virtual void dispose() override;
    virtual const ShaderUniformTypeDesc& desc() const override { return m_desc; }
    virtual const std::string& name() const override { return m_name; }

private:
    ShaderUniformTypeDesc m_desc;
    std::string m_name;
};

class VulkanLocalShaderSamplerBuffer
    : public IShaderSamplerBuffer
{
public:
    VulkanLocalShaderSamplerBuffer();
    virtual ~VulkanLocalShaderSamplerBuffer();
    void init();
    virtual void dispose() override;

    virtual int registerCount() const override;
    virtual const std::string& getTextureRegisterName(int registerIndex) const override;
    virtual const std::string& getSamplerRegisterName(int registerIndex) const override;
    virtual void setTexture(int registerIndex, ITexture* texture) override;
    virtual void setSamplerState(int registerIndex, ISamplerState* state) override;

private:
};

} // namespace detail
} // namespace ln

