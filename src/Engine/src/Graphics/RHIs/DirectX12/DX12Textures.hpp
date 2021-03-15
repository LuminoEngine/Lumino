
#pragma once
#include "DX12DeviceContext.hpp"

namespace ln {
namespace detail {

class DX12Image
    : public RHIRefObject
{
public:
    DX12Image();
    bool init(DX12Device* device, uint32_t width, uint32_t height, uint32_t mipLevels, bool msaa, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flags, D3D12_RESOURCE_STATES initialState);
    bool init(DX12Device* device, const ComPtr<ID3D12Resource>& dxRenderTarget, D3D12_RESOURCE_STATES state);
    void dispose();

    const RHISizeI& size() const { return m_size; }
    DXGI_FORMAT dxFormat() const { return m_dxFormat; }
    UINT mipLevels() const { return m_mipLevels; }
    ID3D12Resource* dxResource() const { return m_dxResource.Get(); }
    UINT64 uploadBufferSize() const { return m_uploadBufferSize; }
    const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& dxFootprint() const { return m_footprint; }
    void resourceBarrior(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES newState);

private:
    ComPtr<ID3D12Resource> m_dxResource;
    RHISizeI m_size;
    UINT m_mipLevels;
    DXGI_FORMAT m_dxFormat;
    D3D12_RESOURCE_STATES m_currentState;
    UINT64 m_uploadBufferSize;
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT m_footprint;
};

class DX12Texture
    : public RHIResource
{
public:
    DX12Texture();
    //virtual const DX12Image* image() const = 0;
    virtual void setSubData(DX12GraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) = 0;
    virtual void setSubData3D(DX12GraphicsContext* graphicsContext, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) = 0;

    //UINT mipLevels() const { return m_mipLevels; }
    DX12Image* image() const { return m_image.get(); }
    DXGI_FORMAT dxFormat() const { return m_image->dxFormat(); }
    UINT mipLevels() const { return m_image->mipLevels(); }
    ID3D12Resource* dxResource() const { return m_image->dxResource(); }

protected:
    RHIRef<DX12Image> m_image;
};

class DX12Texture2D
	: public DX12Texture
{
public:
	DX12Texture2D();
	Result init(DX12Device* device, GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat format, bool mipmap, const void* initialData);
    void dispose() override;
	//DeviceTextureType type() const override { return DeviceTextureType::Texture2D; }
    RHIRef<RHIBitmap> readData() override { LN_UNREACHABLE(); return nullptr; }
    void setSubData(DX12GraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) override;
	void setSubData3D(DX12GraphicsContext* graphicsContext, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override { LN_UNREACHABLE(); }

    //virtual const DX12Image* image() const override { return &m_image; }

    //ID3D12Resource* dxResource() const override { LN_NOTIMPLEMENTED(); return nullptr; }
    //void resourceBarrior(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES newState);

private:
    bool generateMips();

	DX12Device* m_device;
	//GraphicsResourceUsage m_usage;
 //   RHISizeI m_size;
};

class DX12RenderTarget
    : public DX12Texture
{
public:
    DX12RenderTarget();
    bool init(DX12Device* deviceContext, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa);
    bool init(DX12Device* deviceContext, const ComPtr<ID3D12Resource>& dxRenderTarget);
    void dispose() override;
    RHIRef<RHIBitmap> readData() override;
    void setSubData(DX12GraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) override {}
    void setSubData3D(DX12GraphicsContext* graphicsContext, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override {}

    //virtual const DX12Image* image() const override { return m_image.get(); }

    //ID3D12Resource* dxResource() const override { return m_dxRenderTarget.Get(); }
    //bool isMultisample() const override { return m_multisampleBuffer != nullptr; }
    DX12Image* multisampleBuffer() const { return m_multisampleBuffer.get(); }


protected:
    DX12Device* m_device;
    //RHISizeI m_size;
    //TextureFormat m_format;
    RHIRef<DX12Image> m_multisampleBuffer;
};

class DX12DepthBuffer
    : public IDepthBuffer
{
public:
    DX12DepthBuffer();
    Result init(DX12Device* device, uint32_t width, uint32_t height);
    void dispose();
    const SizeI& size() const { return m_size; }

    DX12Image* image() const { return m_image.get(); }
    DXGI_FORMAT dxFormat() const { return m_image->dxFormat(); }
    ID3D12Resource* dxResource() const { return m_image->dxResource(); }

    bool isMultisample() const override { return m_multisampleBuffer != nullptr; }
    DX12Image* multisampleBuffer() const { return m_multisampleBuffer.get(); }

private:
    DX12Device* m_deviceContext;
    SizeI m_size;
    RHIRef<DX12Image> m_image;
    RHIRef<DX12Image> m_multisampleBuffer;
};

} // namespace detail
} // namespace ln
