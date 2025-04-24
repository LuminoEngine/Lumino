// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "SwapChain.hpp"

namespace ln {

#ifdef LN_USE_DIRECTX12
/**
 */
class DirectX12GraphicsContext : public GraphicsContext {
public:
    struct Settings {
        PlatformWindow* mainWindow = nullptr;
        bool debugMode = false;
        ln::String priorityAdapterName;
    };
    
    static Ref<DirectX12GraphicsContext> create(const Settings& settings);

    detail::IGraphicsDevice* rhiDevice() const override;

private:
    DirectX12GraphicsContext();
    ~DirectX12GraphicsContext() override;
    bool init(const Settings& settings);
    void onDispose(bool explicitDisposing) override;

    Ref<detail::DX12Device> m_device;
};
#endif // LN_USE_DIRECTX12

#ifdef LN_USE_VULKAN
/**
 */
class VulkanGraphicsContext : public GraphicsContext {
public:
    struct Settings {
        PlatformWindow* mainWindow = nullptr;
        bool debugMode = false;
    };

    static Ref<VulkanGraphicsContext> create(const Settings& settings);

    detail::IGraphicsDevice* rhiDevice() const override;

private:
    VulkanGraphicsContext();
    ~VulkanGraphicsContext() override;
    bool init(const Settings& settings);
    void onDispose(bool explicitDisposing) override;

    Ref<detail::VulkanDevice> m_device;
};
#ifdef VULKAN_CORE_H_
class VulkanIntegration {
public:
    static void getImageInfo(
        GraphicsCommandList* graphicsContext,
        RenderTargetTexture* texture,
        VkImage* outImage,
        VkImageView* outImageView,
        VkFormat* outFormat,
        int* outWidth,
        int* outHeight);
    static void getImageInfo(
        GraphicsCommandList* graphicsContext,
        DepthBuffer* texture,
        VkImage* outImage,
        VkImageView* outImageView,
        VkFormat* outFormat,
        int* outWidth,
        int* outHeight);
};
#endif
#endif // LN_USE_VULKAN

#ifdef LN_USE_WEBGPU
class WebGPUGraphicsContext : public GraphicsContext {
public:
    struct Settings {
        // PlatformWindow または selector が必要
        PlatformWindow* mainWindowOrNull = nullptr;
        const char* selectorOrNull = nullptr;
        bool debugMode = false;
    };

    static Ref<WebGPUGraphicsContext> create(const Settings& settings);
    //static Ref<WebGPUGraphicsContext> createFromSelector(const char* selector);

    detail::IGraphicsDevice* rhiDevice() const override;

private:
    WebGPUGraphicsContext();
    ~WebGPUGraphicsContext() override;
    bool init(const Settings& settings);
    void onDispose(bool explicitDisposing) override;

    Ref<detail::IGraphicsDevice> m_device;
};
#endif // LN_USE_WEBGPU

#ifdef LN_USE_OPENGL
/**
 */
class OpenGLGraphicsContext : public GraphicsContext {
public:
    struct Settings {
        PlatformWindow* window = nullptr; // nullptr の場合、OpenGL Context の生成・管理を内部で行わない
        uint32_t defaultFramebuffer = 0;
        int32_t width = 0;
        int32_t height = 0;
    };

    static Ref<OpenGLGraphicsContext> create(const Settings& settings);

    detail::IGraphicsDevice* rhiDevice() const override;

    /** Lumino 外部で作成された OpenGL Context を使用しているかどうかを取得します。 */
    bool isUseExternalGLContext() const;

private:
    OpenGLGraphicsContext();
    ~OpenGLGraphicsContext() override;
    bool init(const Settings& settings);
    void onDispose(bool explicitDisposing) override;
    void onCreateRHIObjects() override;

    Ref<detail::OpenGLDevice> m_device;
    PlatformWindow* m_window;
    Ref<RenderTargetTexture> m_wrapedRenderTarget;
    uint32_t m_externalDefaultFBO;
    int32_t m_externalWidth;
    int32_t m_externalHeight;
};
#endif // LN_USE_OPENGL

} // namespace ln
