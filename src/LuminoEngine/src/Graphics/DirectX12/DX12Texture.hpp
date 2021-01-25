
#pragma once
#include "DX12DeviceContext.hpp"

namespace ln {
namespace detail {
	
class DX12RenderTarget
    : public DX12Texture
{
public:
    DX12RenderTarget();
    bool init(DX12Device* deviceContext, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap);
    bool init(DX12Device* deviceContext, const ComPtr<ID3D12Resource>& dxRenderTarget);
    void dispose() override;
    DeviceTextureType type() const override { return DeviceTextureType::RenderTarget; }
    SizeI realSize() override { return m_size; }
    TextureFormat getTextureFormat() const { return TextureFormat::RGBA8; }
    GraphicsResourceUsage usage() const override { return GraphicsResourceUsage::Static; }
    void readData(void* outData) override;
    void setSubData(DX12GraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) override {}
    void setSubData3D(DX12GraphicsContext* graphicsContext, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override {}

    //virtual const DX12Image* image() const override { return m_image.get(); }

protected:
    DX12Device* m_deviceContext;
    SizeI m_size;
    TextureFormat m_format;
    ComPtr<ID3D12Resource> m_dxRenderTarget;
};

class DX12DepthBuffer
    : public IDepthBuffer
{
public:
    DX12DepthBuffer();
    Result init(DX12Device* deviceContext, uint32_t width, uint32_t height);
    void dispose();
    const SizeI& size() const { return m_size; }
    //const DX12Image* image() const { return &m_image; }

private:
    DX12Device* m_deviceContext;
    SizeI m_size;
    ComPtr<ID3D12Resource> m_dxDepthBuffer;
};

} // namespace detail
} // namespace ln
