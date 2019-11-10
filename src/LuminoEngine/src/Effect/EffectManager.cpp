
#include "Internal.hpp"
#include <LuminoEngine/Graphics/SwapChain.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Effect/EffectContext.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "EffectManager.hpp"

//#define EFK_TEST

#ifdef EFK_TEST
#include <Effekseer.h>
//#include <EffekseerRendererGL.h>
#include <EffekseerRenderer/EffekseerRendererVulkan.Renderer.h>
#include <EffekseerRendererLLGI.Renderer.h>
#include <EffekseerRendererLLGI.RendererImplemented.h>
//#include <EffekseerSoundAL.h>
#include <Utils/LLGI.CommandListPool.h>
#include <Vulkan/LLGI.GraphicsVulkan.h>
#include <Vulkan/LLGI.CommandListVulkan.h>
#include <Vulkan/LLGI.TextureVulkan.h>
#include <LLGI.Platform.h>
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "C:/VulkanSDK/1.1.101.0/Lib32/vulkan-1.lib")
#include <LuminoEngine/Graphics/GraphicsExtension.hpp>
#include <LuminoEngine/Graphics/GraphicsExtensionVulkan.hpp>
#endif
namespace ln {
namespace detail {

#ifdef EFK_TEST
static int g_window_width = 800;
static int g_window_height = 600;
//static ::Effekseer::Manager*			g_manager = NULL;
//static ::EffekseerRenderer::Renderer*	g_renderer = NULL;
//static ::EffekseerSound::Sound*			g_sound = NULL;
static ::Effekseer::Effect*				g_effect = NULL;
static ::Effekseer::Handle				g_handle = -1;
static ::Effekseer::Vector3D			g_position;
//static LLGI::Platform* g_platform = nullptr;
//static LLGI::Graphics* g_graphics = nullptr;

class LLGINativeGraphicsExtension : public INativeGraphicsExtension
{
public:
	::Effekseer::Manager* m_manager = nullptr;
	//::EffekseerRenderer::Renderer* m_renderer = nullptr;
    ::EffekseerRendererLLGI::RendererImplemented* m_renderer = nullptr;
    ::LLGI::SingleFrameMemoryPool* m_singleFrameMemoryPool = nullptr;
    //std::shared_ptr<LLGI::CommandListPool> m_commandListPool = nullptr;
    LLGI::GraphicsVulkan* m_llgiGraphics = nullptr;
    LLGI::CommandListVulkan* m_llgiCommandList = nullptr;
    EffekseerRendererLLGI::CommandList* m_efkCommandList = nullptr;

	// 歪み描画のため、onRender() までの RenderPass は一度 end して、カレントの RenderTarget 使って描画できるようにしたいので Outside
	virtual NativeGraphicsExtensionRenderPassPreCondition getRenderPassPreCondition() const override { return NativeGraphicsExtensionRenderPassPreCondition::EnsureOutside; }

	virtual void onLoaded(INativeGraphicsInterface* nativeInterface) override
	{
        // FIXME: RenderTarget,Depthbuffer の VkImage 参照は RenderPassVulkan が Framebuffer として持っている。

        int swapBufferCount = 3;
        int maxDrawcall = 128;
        m_nativeInterface = static_cast<IVulkanNativeGraphicsInterface*>(nativeInterface);

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = m_nativeInterface->getGraphicsQueueFamilyIndex();
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        if (LN_ENSURE(vkCreateCommandPool(m_nativeInterface->getDevice(), &poolInfo, nullptr, &m_commandPool) == VK_SUCCESS)) {
            return;
        }

        auto renderer = ::EffekseerRendererVulkan::Create(
            m_nativeInterface->getPhysicalDevice(),
            m_nativeInterface->getDevice(),
            m_nativeInterface->getGraphicsQueue(),
            m_commandPool,
            swapBufferCount,
            1000);
        m_renderer = dynamic_cast<::EffekseerRendererLLGI::RendererImplemented*>(renderer);
        m_llgiGraphics = dynamic_cast<LLGI::GraphicsVulkan*>(m_renderer->GetGraphics());

        m_singleFrameMemoryPool = m_llgiGraphics->CreateSingleFrameMemoryPool(1024 * 1024, maxDrawcall);
        //m_commandListPool = std::make_shared<LLGI::CommandListPool>(m_renderer->GetGraphics(), m_singleFrameMemoryPool, swapBufferCount);

        m_llgiCommandList = new LLGI::CommandListVulkan();
        if (!m_llgiCommandList->Initialize(m_llgiGraphics, maxDrawcall, LLGI::CommandListPreCondition::Standalone)) {
            return;
        }

        m_efkCommandList = new EffekseerRendererLLGI::CommandList(m_llgiGraphics, m_llgiCommandList, m_singleFrameMemoryPool);

	}

	virtual void onUnloaded(INativeGraphicsInterface* nativeInterface) override
	{
        if (m_efkCommandList) {
            m_efkCommandList->Release();
            m_efkCommandList = nullptr;
        }

        if (m_llgiCommandList) {
            m_llgiCommandList->Release();
            m_llgiCommandList = nullptr;
        }

        //if (m_commandListPool) {
        //    m_commandListPool = nullptr;
        //}

        if (m_singleFrameMemoryPool) {
            m_singleFrameMemoryPool->Release();
            m_singleFrameMemoryPool = nullptr;
        }

        if (m_renderer) {
            m_renderer->Destroy();
            m_renderer = nullptr;
        }

        if (m_commandPool) {
            vkDestroyCommandPool(m_nativeInterface->getDevice(), m_commandPool, nullptr);
            m_commandPool = VK_NULL_HANDLE;
        }
	}

	virtual void onRender(INativeGraphicsInterface* nativeInterface) override
	{
        // FIXME: renderPassPipelineState の管理は EffekseerRendererVulkan の中で握られていて、
        // これが RendererImplemented::Initialize でセットされる唯一オブジェクト。
        // なので現状、RenderTarget が変わったら 全部作り直さなければならないし、遅延初期化せざるを得ない。
        // もう少しスマートにするには、RendererImplemented::SetRenderPassPipelineState とか用意して、後からセットできるようにすること。
        //if (!m_renderer) {


        //}

        printf("==========\n");


        VkImage renderTargetImage, depthBufferImage;
        VkImageView renderTargetImageView, depthBufferImageView;
        VkFormat renderTargetFormat, depthBufferFormat;
        int renderTargetWidth, renderTargetHeight, depthBufferWidth, depthBufferHeight;
        VulkanIntegration::getImageInfo(this->graphicsContext, this->renderTarget, &renderTargetImage, &renderTargetImageView, &renderTargetFormat, &renderTargetWidth, &renderTargetHeight);
        VulkanIntegration::getImageInfo(this->graphicsContext, this->depthBuffer, &depthBufferImage, &depthBufferImageView, &depthBufferFormat, &depthBufferWidth, &depthBufferHeight);

        auto llgiRenderTarget = new LLGI::TextureVulkan();
        llgiRenderTarget->InitializeFromExternal(LLGI::TextureType::Render, renderTargetImage, renderTargetImageView, renderTargetFormat, LLGI::Vec2I(renderTargetWidth, renderTargetHeight));
        auto llgiDepthBuffer = new LLGI::TextureVulkan();
        llgiDepthBuffer->InitializeFromExternal(LLGI::TextureType::Depth, depthBufferImage, depthBufferImageView, depthBufferFormat, LLGI::Vec2I(depthBufferWidth, depthBufferHeight));

        LLGI::Texture* llgiRenderTargets[] = { llgiRenderTarget };
        LLGI::RenderPass* llgiRenderPass = m_llgiGraphics->CreateRenderPass((const LLGI::Texture**)llgiRenderTargets, 1, llgiDepthBuffer);
        llgiRenderPass->SetIsColorCleared(false);
        llgiRenderPass->SetIsDepthCleared(false);
        LLGI::RenderPassPipelineState* llgtRenderPassPipelineState = m_llgiGraphics->CreateRenderPassPipelineState(llgiRenderPass);


        m_renderer->SetRenderPassPipelineState(llgtRenderPassPipelineState);


        m_singleFrameMemoryPool->NewFrame();
        m_llgiCommandList->BeginExternal(m_nativeInterface->getRecordingCommandBuffer());
        //m_commandListPool->Get();
        m_renderer->SetCommandList(m_efkCommandList);
		m_renderer->BeginRendering();
        m_llgiCommandList->BeginRenderPass(llgiRenderPass);
		m_manager->Draw();
		m_renderer->EndRendering();
        m_llgiCommandList->EndRenderPass();
        m_llgiCommandList->EndExternal();

        printf("-----\n");
	}

private:
    IVulkanNativeGraphicsInterface* m_nativeInterface = nullptr;
    VkCommandPool m_commandPool = VK_NULL_HANDLE;
};
#else
class LLGINativeGraphicsExtension {};
#endif

//==============================================================================
// EffectManager

EffectManager::EffectManager()
{
}

void EffectManager::init(const Settings& settings)
{
    LN_LOG_DEBUG << "EffectManager Initialization started.";
    m_graphicsManager = settings.graphicsManager;

#ifdef EFK_TEST
	//return;
    //g_platform = LLGI::CreatePlatform(LLGI::DeviceType::Vulkan);
    //g_graphics = g_platform->CreateGraphics();

    m_nativeGraphicsExtension = std::make_unique<LLGINativeGraphicsExtension>();
    m_graphicsManager->registerExtension(m_nativeGraphicsExtension.get());

 //   ::EffekseerRendererLLGI::FixedShader fixedShaders;
	//::EffekseerRendererLLGI::Renderer::CreateFixedShaderForVulkan(&fixedShaders);

    // エフェクト管理用インスタンスの生成
    m_nativeGraphicsExtension->m_manager = ::Effekseer::Manager::Create(2000);

    // 描画用インスタンスから描画機能を設定
    m_nativeGraphicsExtension->m_manager->SetSpriteRenderer(m_nativeGraphicsExtension->m_renderer->CreateSpriteRenderer());
    m_nativeGraphicsExtension->m_manager->SetRibbonRenderer(m_nativeGraphicsExtension->m_renderer->CreateRibbonRenderer());
    m_nativeGraphicsExtension->m_manager->SetRingRenderer(m_nativeGraphicsExtension->m_renderer->CreateRingRenderer());
    m_nativeGraphicsExtension->m_manager->SetModelRenderer(m_nativeGraphicsExtension->m_renderer->CreateModelRenderer());

    // 描画用インスタンスからテクスチャの読込機能を設定
    // 独自拡張可能、現在はファイルから読み込んでいる。
    m_nativeGraphicsExtension->m_manager->SetTextureLoader(m_nativeGraphicsExtension->m_renderer->CreateTextureLoader());
    m_nativeGraphicsExtension->m_manager->SetModelLoader(m_nativeGraphicsExtension->m_renderer->CreateModelLoader());

    // 音再生用インスタンスの生成
    //g_sound = EffekseerSound::Sound::Create(32);

    // 音再生用インスタンスから再生機能を指定
    //g_manager->SetSoundPlayer(g_sound->CreateSoundPlayer());

    // 音再生用インスタンスからサウンドデータの読込機能を設定
    // 独自拡張可能、現在はファイルから読み込んでいる。
    //g_manager->SetSoundLoader(g_sound->CreateSoundLoader());

    // 視点位置を確定
    g_position = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

    // 投影行列を設定
    m_nativeGraphicsExtension->m_renderer->SetProjectionMatrix(
        ::Effekseer::Matrix44().PerspectiveFovRH_OpenGL(90.0f / 180.0f * 3.14f, (float)g_window_width / (float)g_window_height, 1.0f, 50.0f));

    // カメラ行列を設定
    m_nativeGraphicsExtension->m_renderer->SetCameraMatrix(
        ::Effekseer::Matrix44().LookAtRH(g_position, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

    // エフェクトの読込
    g_effect = Effekseer::Effect::Create(m_nativeGraphicsExtension->m_manager, (const EFK_CHAR*)L"D:/LocalProj/Effekseer/EffekseerRuntime143b/RuntimeSample/release/test.efk");

    // エフェクトの再生
    g_handle = m_nativeGraphicsExtension->m_manager->Play(g_effect, 0, 0, 0);
#endif

    LN_LOG_DEBUG << "EffectManager Initialization ended.";
}

void EffectManager::dispose()
{
#ifdef EFK_TEST
	//return;
    // エフェクトの停止
    m_nativeGraphicsExtension->m_manager->StopEffect(g_handle);

    // エフェクトの破棄
    ES_SAFE_RELEASE(g_effect);

    // 先にエフェクト管理用インスタンスを破棄
    m_nativeGraphicsExtension->m_manager->Destroy();

    // 次に音再生用インスタンスを破棄
    //g_sound->Destroy();

    // 次に描画用インスタンスを破棄
    //g_renderer->Destroy();

    m_graphicsManager->unregisterExtension(m_nativeGraphicsExtension.get());
#endif
}

void EffectManager::testDraw(RenderingContext* renderingContext)
{
#ifdef EFK_TEST
	//return;

    //if (1) {

    //    if (!g_platform->NewFrame())
    //        return;

    //    g_graphics->NewFrame();

    //}


    // エフェクトの移動処理を行う
    m_nativeGraphicsExtension->m_manager->AddLocation(g_handle, ::Effekseer::Vector3D(0.2f, 0.0f, 0.0f));

    // エフェクトの更新処理を行う
    m_nativeGraphicsExtension->m_manager->Update();


   // graphicsContext->drawExtension(m_nativeGraphicsExtension.get());
    renderingContext->invokeExtensionRendering(m_nativeGraphicsExtension.get());

    //// エフェクトの描画開始処理を行う。
    //g_renderer->BeginRendering();

    //// エフェクトの描画を行う。
    //g_manager->Draw();

    //// エフェクトの描画終了処理を行う。
    //g_renderer->EndRendering();


    //if (1) {
    //    g_platform->Present();
    //}
#endif
}

void EffectManager::testDraw2(GraphicsContext* graphicsContext)
{
#ifdef EFK_TEST
    m_nativeGraphicsExtension->m_manager->AddLocation(g_handle, ::Effekseer::Vector3D(0.2f, 0.0f, 0.0f));
    m_nativeGraphicsExtension->m_manager->Update();
    graphicsContext->drawExtension(m_nativeGraphicsExtension.get());
#endif
}

} // namespace detail
} // namespace ln

