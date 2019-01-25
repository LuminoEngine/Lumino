
#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include "VulkanDeviceContext.hpp"

namespace ln {
namespace detail {

//=============================================================================
// VulkanDeviceContext

VulkanDeviceContext::VulkanDeviceContext()
{
}

VulkanDeviceContext::~VulkanDeviceContext()
{
}

void VulkanDeviceContext::init(const Settings& settings)
{
}

void VulkanDeviceContext::dispose()
{
    IGraphicsDeviceContext::dispose();
}

void VulkanDeviceContext::onGetCaps(GraphicsDeviceCaps* outCaps)
{
    LN_NOTIMPLEMENTED();
}

void VulkanDeviceContext::onEnterMainThread()
{
    LN_NOTIMPLEMENTED();
}

void VulkanDeviceContext::onLeaveMainThread()
{
    LN_NOTIMPLEMENTED();
}

void VulkanDeviceContext::onSaveExternalRenderState()
{
    LN_NOTIMPLEMENTED();
}

void VulkanDeviceContext::onRestoreExternalRenderState()
{
    LN_NOTIMPLEMENTED();
}

Ref<ISwapChain> VulkanDeviceContext::onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<IVertexDeclaration> VulkanDeviceContext::onCreateVertexDeclaration(const VertexElement* elements, int elementsCount)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<IVertexBuffer> VulkanDeviceContext::onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<IIndexBuffer> VulkanDeviceContext::onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<ITexture> VulkanDeviceContext::onCreateTexture2D(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<ITexture> VulkanDeviceContext::onCreateTexture3D(uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<ITexture> VulkanDeviceContext::onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<IDepthBuffer> VulkanDeviceContext::onCreateDepthBuffer(uint32_t width, uint32_t height)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<ISamplerState> VulkanDeviceContext::onCreateSamplerState(const SamplerStateData& desc)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<IShaderPass> VulkanDeviceContext::onCreateShaderPass(const byte_t* vsCode, int vsCodeLen, const byte_t* psCode, int psCodeLen, ShaderCompilationDiag* diag)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

void VulkanDeviceContext::onUpdatePipelineState(const BlendStateDesc& blendState, const RasterizerStateDesc& rasterizerState, const DepthStencilStateDesc& depthStencilState)
{
    LN_NOTIMPLEMENTED();
}

void VulkanDeviceContext::onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, IDepthBuffer* depthBuffer)
{
    LN_NOTIMPLEMENTED();
}

void VulkanDeviceContext::onUpdateRegionRects(const RectI& viewportRect, const RectI& scissorRect, const SizeI& targetSize)
{
    LN_NOTIMPLEMENTED();
}

void VulkanDeviceContext::onUpdatePrimitiveData(IVertexDeclaration* decls, IVertexBuffer** vertexBuufers, int vertexBuffersCount, IIndexBuffer* indexBuffer)
{
    LN_NOTIMPLEMENTED();
}

void VulkanDeviceContext::onUpdateShaderPass(IShaderPass* newPass)
{
    LN_NOTIMPLEMENTED();
}

void VulkanDeviceContext::onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
    LN_NOTIMPLEMENTED();
}

void VulkanDeviceContext::onDrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount)
{
    LN_NOTIMPLEMENTED();
}

void VulkanDeviceContext::onDrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount)
{
    LN_NOTIMPLEMENTED();
}

void VulkanDeviceContext::onPresent(ISwapChain* swapChain)
{
    LN_NOTIMPLEMENTED();
}

//=============================================================================
// VulkanSwapChain

VulkanSwapChain::VulkanSwapChain()
{
}

VulkanSwapChain::~VulkanSwapChain()
{
}

void VulkanSwapChain::dispose()
{
    ISwapChain::dispose();
}

ITexture* VulkanSwapChain::getColorBuffer() const
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}


//==============================================================================
// VulkanVertexDeclaration

VulkanVertexDeclaration::VulkanVertexDeclaration()
{
}

VulkanVertexDeclaration::~VulkanVertexDeclaration()
{
}

void VulkanVertexDeclaration::init(const VertexElement* elements, int elementsCount)
{
    LN_NOTIMPLEMENTED();
}

void VulkanVertexDeclaration::dispose()
{
    IVertexDeclaration::dispose();
}

//==============================================================================
// VulkanVertexBuffer

VulkanVertexBuffer::VulkanVertexBuffer()
{
}

VulkanVertexBuffer::~VulkanVertexBuffer()
{
}

void VulkanVertexBuffer::init(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
    LN_NOTIMPLEMENTED();
}

void VulkanVertexBuffer::dispose()
{
    IVertexBuffer::dispose();
}

size_t VulkanVertexBuffer::getBytesSize()
{
    LN_NOTIMPLEMENTED();
    return 0;
}

GraphicsResourceUsage VulkanVertexBuffer::usage() const
{
    LN_NOTIMPLEMENTED();
    return GraphicsResourceUsage::Static;
}

void VulkanVertexBuffer::setSubData(size_t offset, const void* data, size_t length)
{
    LN_NOTIMPLEMENTED();
}

void* VulkanVertexBuffer::map()
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

void VulkanVertexBuffer::unmap()
{
    LN_NOTIMPLEMENTED();
}

//==============================================================================
// VulkanIndexBuffer

VulkanIndexBuffer::VulkanIndexBuffer()
{
}

VulkanIndexBuffer::~VulkanIndexBuffer()
{
}

void VulkanIndexBuffer::init(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
    LN_NOTIMPLEMENTED();
}

void VulkanIndexBuffer::dispose()
{
    LN_NOTIMPLEMENTED();
    IIndexBuffer::dispose();
}

size_t VulkanIndexBuffer::getBytesSize()
{
    LN_NOTIMPLEMENTED();
    return 0;
}

GraphicsResourceUsage VulkanIndexBuffer::usage() const
{
    LN_NOTIMPLEMENTED();
    return GraphicsResourceUsage::Static;
}

void VulkanIndexBuffer::setSubData(size_t offset, const void* data, size_t length)
{
    LN_NOTIMPLEMENTED();
}

void* VulkanIndexBuffer::map()
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

void VulkanIndexBuffer::unmap()
{
    LN_NOTIMPLEMENTED();
}

//=============================================================================
// VulkanTextureBase

VulkanTextureBase::VulkanTextureBase()
{
}

VulkanTextureBase::~VulkanTextureBase()
{
}

//=============================================================================
// VulkanTexture2D

VulkanTexture2D::VulkanTexture2D()
{
}

VulkanTexture2D::~VulkanTexture2D()
{
}

void VulkanTexture2D::init(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
    LN_NOTIMPLEMENTED();
}

void VulkanTexture2D::dispose()
{
    LN_NOTIMPLEMENTED();
    VulkanTextureBase::dispose();
}

DeviceTextureType VulkanTexture2D::type() const
{
    return DeviceTextureType::Texture2D;
}

void VulkanTexture2D::readData(void* outData)
{
    LN_UNREACHABLE();
}

const SizeI& VulkanTexture2D::realSize()
{
    LN_NOTIMPLEMENTED();
    return SizeI::Zero;
}

TextureFormat VulkanTexture2D::getTextureFormat() const
{
    LN_NOTIMPLEMENTED();
    return TextureFormat::Unknown;
}

void VulkanTexture2D::setSubData(int x, int y, int width, int height, const void* data, size_t dataSize)
{
    LN_NOTIMPLEMENTED();
}

void VulkanTexture2D::setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
    LN_UNREACHABLE();
}

//=============================================================================
// VulkanTexture3D

VulkanTexture3D::VulkanTexture3D()
{
}

VulkanTexture3D::~VulkanTexture3D()
{
}

void VulkanTexture3D::init(uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
    LN_NOTIMPLEMENTED();
}

void VulkanTexture3D::dispose()
{
    LN_NOTIMPLEMENTED();
    VulkanTextureBase::dispose();
}

DeviceTextureType VulkanTexture3D::type() const
{
    return DeviceTextureType::Texture3D;
}

void VulkanTexture3D::readData(void* outData)
{
    LN_UNREACHABLE();
}

const SizeI& VulkanTexture3D::realSize()
{
    LN_NOTIMPLEMENTED();
    return SizeI::Zero;
}

TextureFormat VulkanTexture3D::getTextureFormat() const
{
    LN_NOTIMPLEMENTED();
    return TextureFormat::Unknown;
}

void VulkanTexture3D::setSubData(int x, int y, int width, int height, const void* data, size_t dataSize)
{
    LN_UNREACHABLE();
}

void VulkanTexture3D::setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
    LN_NOTIMPLEMENTED();
}

//=============================================================================
// VulkanRenderTargetTexture

VulkanRenderTargetTexture::VulkanRenderTargetTexture()
{
}

VulkanRenderTargetTexture::~VulkanRenderTargetTexture()
{
}

void VulkanRenderTargetTexture::init(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
    LN_NOTIMPLEMENTED();
}

void VulkanRenderTargetTexture::dispose()
{
    LN_NOTIMPLEMENTED();
    VulkanTextureBase::dispose();
}

DeviceTextureType VulkanRenderTargetTexture::type() const
{
    return DeviceTextureType::RenderTarget;
}

void VulkanRenderTargetTexture::readData(void* outData)
{
    LN_NOTIMPLEMENTED();
}

const SizeI& VulkanRenderTargetTexture::realSize()
{
    LN_NOTIMPLEMENTED();
    return SizeI::Zero;
}

TextureFormat VulkanRenderTargetTexture::getTextureFormat() const
{
    LN_NOTIMPLEMENTED();
    return TextureFormat::Unknown;
}

void VulkanRenderTargetTexture::setSubData(int x, int y, int width, int height, const void* data, size_t dataSize)
{
    LN_UNREACHABLE();
}

void VulkanRenderTargetTexture::setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
    LN_UNREACHABLE();
}

//=============================================================================
// VulkanDepthBuffer

VulkanDepthBuffer::VulkanDepthBuffer()
{
}

VulkanDepthBuffer::~VulkanDepthBuffer()
{
}

void VulkanDepthBuffer::init(uint32_t width, uint32_t height)
{
    LN_NOTIMPLEMENTED();
}

void VulkanDepthBuffer::dispose()
{
    IDepthBuffer::dispose();
}

//=============================================================================
// VulkanSamplerState

VulkanSamplerState::VulkanSamplerState()
{
}

VulkanSamplerState::~VulkanSamplerState()
{
}

void VulkanSamplerState::init(const SamplerStateData& desc)
{
    LN_NOTIMPLEMENTED();
}

void VulkanSamplerState::dispose()
{
    LN_NOTIMPLEMENTED();
    ISamplerState::dispose();
}

//=============================================================================
// VulkanShaderPass

VulkanShaderPass::VulkanShaderPass()
{
}

VulkanShaderPass::~VulkanShaderPass()
{
}

void VulkanShaderPass::init(VulkanDeviceContext* context, const byte_t* vsCode, int vsCodeLen, const byte_t* fsCode, int fsCodeLen, ShaderCompilationDiag* diag)
{
    LN_NOTIMPLEMENTED();
}

void VulkanShaderPass::dispose()
{
    IShaderPass::dispose();
}

int VulkanShaderPass::getUniformCount() const
{
    LN_NOTIMPLEMENTED();
    return 0;
}

IShaderUniform* VulkanShaderPass::getUniform(int index) const
{
    // TODO: 必要？
    LN_NOTIMPLEMENTED();
    return nullptr;
}

void VulkanShaderPass::setUniformValue(int index, const void* data, size_t size)
{
    LN_NOTIMPLEMENTED();
}

int VulkanShaderPass::getUniformBufferCount() const
{
    LN_NOTIMPLEMENTED();
    return 0;
}

IShaderUniformBuffer* VulkanShaderPass::getUniformBuffer(int index) const
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

IShaderSamplerBuffer* VulkanShaderPass::samplerBuffer() const
{
    LN_NOTIMPLEMENTED();
    return 0;
}

//=============================================================================
// VulkanShaderUniformBuffer

VulkanShaderUniformBuffer::VulkanShaderUniformBuffer()
{
}

VulkanShaderUniformBuffer::~VulkanShaderUniformBuffer()
{
}

void VulkanShaderUniformBuffer::init()
{
    LN_NOTIMPLEMENTED();
}

void VulkanShaderUniformBuffer::dispose()
{
    IShaderUniformBuffer::dispose();
}

const std::string& VulkanShaderUniformBuffer::name() const
{
    LN_NOTIMPLEMENTED();
    return m_name;
}

int VulkanShaderUniformBuffer::getUniformCount() const
{
    LN_NOTIMPLEMENTED();
    return 0;
}

IShaderUniform* VulkanShaderUniformBuffer::getUniform(int index) const
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

size_t VulkanShaderUniformBuffer::bufferSize() const
{
    LN_NOTIMPLEMENTED();
    return 0;
}

void VulkanShaderUniformBuffer::setData(const void* data, size_t size)
{
    LN_NOTIMPLEMENTED();
}


//=============================================================================
// VulkanShaderUniform

VulkanShaderUniform::VulkanShaderUniform()
{
}

VulkanShaderUniform::~VulkanShaderUniform()
{
}

void VulkanShaderUniform::init()
{
    LN_NOTIMPLEMENTED();
}

void VulkanShaderUniform::dispose()
{
    IShaderUniform::dispose();
}

//=============================================================================
// VulkanLocalShaderSamplerBuffer

VulkanLocalShaderSamplerBuffer::VulkanLocalShaderSamplerBuffer()
{
}

VulkanLocalShaderSamplerBuffer::~VulkanLocalShaderSamplerBuffer()
{
}

void VulkanLocalShaderSamplerBuffer::init()
{
}

void VulkanLocalShaderSamplerBuffer::dispose()
{
}

int VulkanLocalShaderSamplerBuffer::registerCount() const
{
    LN_NOTIMPLEMENTED();
    return 0;
}

const std::string& VulkanLocalShaderSamplerBuffer::getTextureRegisterName(int registerIndex) const
{
    LN_NOTIMPLEMENTED();
    return std::string();
}

const std::string& VulkanLocalShaderSamplerBuffer::getSamplerRegisterName(int registerIndex) const
{
    LN_NOTIMPLEMENTED();
    return std::string();
}

void VulkanLocalShaderSamplerBuffer::setTexture(int registerIndex, ITexture* texture)
{
    LN_NOTIMPLEMENTED();
}

void VulkanLocalShaderSamplerBuffer::setSamplerState(int registerIndex, ISamplerState* state)
{
    LN_NOTIMPLEMENTED();
}

} // namespace detail
} // namespace ln

