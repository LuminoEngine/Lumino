
#include "Internal.hpp"
#include <LuminoGraphics/detail/GraphicsManager.hpp>
#include <LuminoGraphics/GPU/Texture.hpp>
#include <LuminoGraphics/GPU/SwapChain.hpp>
#include <LuminoGraphics/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoGraphics/Rendering/RenderView.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoEngine/Effect/ParticleEffectModel2.hpp>
//#include "../../../Engine/src/Asset/AssetManager.hpp"
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>
#include "EffekseerEffect.hpp"
#include "EffectManager.hpp"

#if LN_EFFEKSEER_ENABLED
#include <Effekseer.h>
#endif

#if LN_EFFEKSEER_ENABLED
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
#include <LuminoEngine/Graphics/GraphicsExtension.hpp>
#include <LuminoEngine/Graphics/GraphicsExtensionVulkan.hpp>
#endif
namespace ln {
namespace detail {

template<class T, class TKey = uint64_t>
class LifeCountedObjectCache
{
public:
    LifeCountedObjectCache()
        : m_entryMap()
        , m_holdLifeCount(60)
    {}

    void clear()
    {
        m_entryMap.clear();
    }

    std::shared_ptr<T> find(const TKey& key)
    {
        std::shared_ptr<T> result = nullptr;

        auto itr = m_entryMap.find(key);
        if (itr != m_entryMap.end())
        {
            itr->second.lifeCount = 0;
            result = itr->second.obj;
        }

        gcObjects();
        return result;
    }

    void insert(const TKey& key, const std::shared_ptr<T>& obj)
    {
        m_entryMap.insert({ key, { 0, obj } });
    }

    void release(const TKey& key)
    {
        auto itr = m_entryMap.find(key);
        if (itr != m_entryMap.end())
        {
            itr->second.lifeCount = m_holdLifeCount;
            return;
        }

        LN_UNREACHABLE();
    }

private:
    void gcObjects()
    {
        for (auto itr = m_entryMap.begin(); itr != m_entryMap.end();) {
            if (itr->second.lifeCount > 0) {
                itr->second.lifeCount--;
                if (itr->second.lifeCount <= 0) {
                    itr = m_entryMap.erase(itr);
                }
            }
            else {
                ++itr;
            }
        }
    }

    struct Entry
    {
        int lifeCount;  // 0 is alive.
        std::shared_ptr<T> obj;
    };

    std::unordered_map<TKey, Entry>	m_entryMap;
    int m_holdLifeCount;
};

#if LN_EFFEKSEER_ENABLED
static int g_window_width = 800;
static int g_window_height = 600;
//static ::EffekseerSound::Sound*			g_sound = NULL;
static ::Effekseer::Effect*				g_effect = NULL;
static ::Effekseer::Handle				g_handle = -1;
static ::Effekseer::Vector3D			g_position;

class LLGINativeGraphicsExtension : public INativeGraphicsExtension
{
public:
    EffectManager* m_manager = nullptr;
	
    ::EffekseerRendererLLGI::RendererImplemented* m_renderer = nullptr;
    ::LLGI::SingleFrameMemoryPool* m_singleFrameMemoryPool = nullptr;
    LLGI::GraphicsVulkan* m_llgiGraphics = nullptr;
    LLGI::CommandListVulkan* m_llgiCommandList = nullptr;
    EffekseerRendererLLGI::CommandList* m_efkCommandList = nullptr;

    // LLGI::RenderPass はネイティブの RenderTarget と DepthBuffer をキーに作成する。
    // RenderTarget と DepthBuffer は常に一定というわけではなく、
    // Swapchain は通常複数のバックバッファを持つため、少なくとも毎フレーム違う RenderPass が必要になる。
    // また、ポストエフェクトなどで1フレーム中に異なる RenderPass が必要になることもある。
    // 一方で、RenderTarget と DepthBuffer をトリガーすることは現状できないので、LLGI::RenderPass は適切な削除のタイミングを知ることができない。
    // そのため、しばらくの間全く利用されない場合に削除するようにする。
    LifeCountedObjectCache<LLGI::RenderPass> m_llgiRenderPassCache;

	virtual NativeGraphicsExtensionRenderPassPreCondition getRenderPassPreCondition() const override
    {
        // 歪み描画のため、onRender() までの RenderPass は一度 end して、カレントの RenderTarget 使って描画できるようにしたいので Outside
        return NativeGraphicsExtensionRenderPassPreCondition::EnsureOutside;
    }

	virtual void onLoaded(INativeGraphicsInterface* nativeInterface) override
	{
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

        m_llgiCommandList = new LLGI::CommandListVulkan();
        if (!m_llgiCommandList->Initialize(m_llgiGraphics, maxDrawcall, LLGI::CommandListPreCondition::Standalone)) {
            return;
        }

        m_efkCommandList = new EffekseerRendererLLGI::CommandList(m_llgiGraphics, m_llgiCommandList, m_singleFrameMemoryPool);

	}

	virtual void onUnloaded(INativeGraphicsInterface* nativeInterface) override
	{
        m_llgiRenderPassCache.clear();

        if (m_efkCommandList) {
            m_efkCommandList->Release();
            m_efkCommandList = nullptr;
        }

        if (m_llgiCommandList) {
            m_llgiCommandList->Release();
            m_llgiCommandList = nullptr;
        }

        if (m_renderer) {
            m_renderer->Destroy();
            m_renderer = nullptr;
        }

        if (m_singleFrameMemoryPool) {
            m_singleFrameMemoryPool->Release();
            m_singleFrameMemoryPool = nullptr;
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

        VkImage renderTargetImage, depthBufferImage;
        VkImageView renderTargetImageView, depthBufferImageView;
        VkFormat renderTargetFormat, depthBufferFormat;
        int renderTargetWidth, renderTargetHeight, depthBufferWidth, depthBufferHeight;
        VulkanIntegration::getImageInfo(this->graphicsContext, this->renderTarget, &renderTargetImage, &renderTargetImageView, &renderTargetFormat, &renderTargetWidth, &renderTargetHeight);
        VulkanIntegration::getImageInfo(this->graphicsContext, this->depthBuffer, &depthBufferImage, &depthBufferImageView, &depthBufferFormat, &depthBufferWidth, &depthBufferHeight);


        // find or create LLGI::RenderPass
        auto renderPassKey = std::hash<VkImage>()(renderTargetImage) + std::hash<VkImage>()(depthBufferImage);
        auto renderPass = m_llgiRenderPassCache.find(renderPassKey);
        if (!renderPass) {
            auto textureType = (this->renderTarget->isBackbuffer()) ? LLGI::TextureType::Screen : LLGI::TextureType::Render;

            auto llgiRenderTarget = new LLGI::TextureVulkan();
            llgiRenderTarget->InitializeFromExternal(textureType, renderTargetImage, renderTargetImageView, renderTargetFormat, LLGI::Vec2I(renderTargetWidth, renderTargetHeight));
            auto llgiDepthBuffer = new LLGI::TextureVulkan();
            llgiDepthBuffer->InitializeFromExternal(LLGI::TextureType::Depth, depthBufferImage, depthBufferImageView, depthBufferFormat, LLGI::Vec2I(depthBufferWidth, depthBufferHeight));

            LLGI::Texture* llgiRenderTargets[] = { llgiRenderTarget };
            LLGI::RenderPass* llgiRenderPass = m_llgiGraphics->CreateRenderPass((const LLGI::Texture**)llgiRenderTargets, 1, llgiDepthBuffer);
            llgiRenderPass->SetIsColorCleared(false);
            llgiRenderPass->SetIsDepthCleared(false);

            renderPass = std::shared_ptr<LLGI::RenderPass>(llgiRenderPass, [](LLGI::RenderPass* ptr) { ptr->Release(); });
            m_llgiRenderPassCache.insert(renderPassKey, renderPass);

            llgiRenderTarget->Release();
            llgiDepthBuffer->Release();
        }

        LLGI::RenderPassPipelineState* llgtRenderPassPipelineState = m_llgiGraphics->CreateRenderPassPipelineState(renderPass.get());
        m_renderer->SetRenderPassPipelineState(llgtRenderPassPipelineState);

        m_singleFrameMemoryPool->NewFrame();
        m_llgiCommandList->BeginExternal(m_nativeInterface->getRecordingCommandBuffer());
        m_renderer->SetCommandList(m_efkCommandList);
		m_renderer->BeginRendering();
        m_llgiCommandList->BeginRenderPass(renderPass.get());
		m_manager->effekseerManager()->Draw();
		m_renderer->EndRendering();
        m_llgiCommandList->EndRenderPass();
        m_llgiCommandList->EndExternal();

        llgtRenderPassPipelineState->Release();
	}

private:
    IVulkanNativeGraphicsInterface* m_nativeInterface = nullptr;
    VkCommandPool m_commandPool = VK_NULL_HANDLE;
};

class FileInterface : public ::Effekseer::FileInterface
{
public:
    EffectManager* m_manager = nullptr;
    ln::Path m_effectDir;

    class FileReader : public ::Effekseer::FileReader
    {
        Ref<Stream> m_stream;
    public:
        FileReader(Ref<Stream> stream)
            : m_stream(stream)
        {}

        virtual size_t Read(void* buffer, size_t size) override { return m_stream->read(buffer, size); }
        virtual void Seek(int position) override { m_stream->seek(position, SeekOrigin::Begin); }
        virtual int GetPosition() override { return m_stream->position(); }
        virtual size_t GetLength() override { return m_stream->length(); }
    };

    virtual ::Effekseer::FileReader* OpenRead(const EFK_CHAR* path) override
    {
        auto stream = m_manager->assetManager()->openFileStream(ln::Path(m_effectDir, path));
        if (stream) {
            return new FileReader(stream);
        }
        else {
            return nullptr;
        }
    }

    virtual ::Effekseer::FileWriter* OpenWrite(const EFK_CHAR* path) override
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
};

#else
class LLGINativeGraphicsExtension {};
class FileInterface {};
#endif

//==============================================================================
// EffectManager

EffectManager::EffectManager()
    : m_efkManager(nullptr)
{
    m_efkEffectCache.init(64);
}

void EffectManager::init(const Settings& settings)
{
    LN_LOG_DEBUG("EffectManager Initialization started.");
    m_graphicsManager = settings.graphicsManager;
    m_assetManager = settings.assetManager;

#if LN_EFFEKSEER_ENABLED

    m_nativeGraphicsExtension = std::make_unique<LLGINativeGraphicsExtension>();
    m_nativeGraphicsExtension->m_manager = this;
    m_graphicsManager->registerExtension(m_nativeGraphicsExtension.get());

    m_fileInterface = std::make_unique<FileInterface>();
    m_fileInterface->m_manager = this;

    m_efkManager = ::Effekseer::Manager::Create(2000);

    // 描画用インスタンスから描画機能を設定
    m_efkManager->SetSpriteRenderer(m_nativeGraphicsExtension->m_renderer->CreateSpriteRenderer());
    m_efkManager->SetRibbonRenderer(m_nativeGraphicsExtension->m_renderer->CreateRibbonRenderer());
    m_efkManager->SetRingRenderer(m_nativeGraphicsExtension->m_renderer->CreateRingRenderer());
    m_efkManager->SetModelRenderer(m_nativeGraphicsExtension->m_renderer->CreateModelRenderer());

    // 描画用インスタンスからテクスチャの読込機能を設定
    // 独自拡張可能、現在はファイルから読み込んでいる。
    m_efkManager->SetTextureLoader(m_nativeGraphicsExtension->m_renderer->CreateTextureLoader(m_fileInterface.get()));
    m_efkManager->SetModelLoader(m_nativeGraphicsExtension->m_renderer->CreateModelLoader(m_fileInterface.get()));

    // 音再生用インスタンスの生成
    //g_sound = EffekseerSound::Sound::Create(32);

    // 音再生用インスタンスから再生機能を指定
    //g_manager->SetSoundPlayer(g_sound->CreateSoundPlayer());

    // 音再生用インスタンスからサウンドデータの読込機能を設定
    // 独自拡張可能、現在はファイルから読み込んでいる。
    //g_manager->SetSoundLoader(g_sound->CreateSoundLoader());
#endif

    m_defaultSpriteParticleGeometry = makeObject_deprecated<SpriteParticleGeometry>();
    auto material = makeObject_deprecated<Material>();
    material->setMainTexture(settings.graphicsManager->whiteTexture());
    material->setShader(settings.renderingManager->builtinShader(BuiltinShader::Sprite));
    m_defaultSpriteParticleGeometry->setMaterial(material);

    LN_LOG_DEBUG("EffectManager Initialization ended.");
}

void EffectManager::dispose()
{
#if LN_EFFEKSEER_ENABLED

    m_efkEffectCache.dispose();

    m_graphicsManager->unregisterExtension(m_nativeGraphicsExtension.get());

    m_efkManager->Destroy();
    m_efkManager = nullptr;

#endif
}

Ref<ParticleModel> EffectManager::loadParticleModel(const StringView& filePath)
{
    // TODO: find cache

    auto obj = makeObject_deprecated<ParticleModel>();
    LN_NOTIMPLEMENTED();
    //detail::AssetObjectInternal::setAssetPath(obj, filePath);
    //detail::AssetObjectInternal::reload(obj);

    return obj;
}

Ref<ParticleModelPromise> EffectManager::loadParticleModelAsync(const StringView& filePath)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

void EffectManager::testDraw(RenderingContext* renderingContext)
{
#if LN_EFFEKSEER_ENABLED

    // 視点位置を確定
    //g_position = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

    auto viewPoint = renderingContext->viewPoint();

    // 投影行列を設定
    //Matrix t = viewPoint->projMatrix;
    //t.transpose();
    m_nativeGraphicsExtension->m_renderer->SetProjectionMatrix(
        //(::Effekseer::Matrix44&)t);
        //::Effekseer::Matrix44().PerspectiveFovRH_OpenGL(90.0f / 180.0f * 3.14f, (float)g_window_width / (float)g_window_height, 1.0f, 50.0f));
        ::Effekseer::Matrix44().PerspectiveFovLH(viewPoint->fovY, (float)viewPoint->viewPixelSize.width / (float)viewPoint->viewPixelSize.height, viewPoint->nearClip, viewPoint->farClip));

    // カメラ行列を設定
    m_nativeGraphicsExtension->m_renderer->SetCameraMatrix(
        //(::Effekseer::Matrix44&)viewPoint->viewMatrix);
        //::Effekseer::Matrix44().LookAtRH(g_position, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
        ::Effekseer::Matrix44().LookAtLH((::Effekseer::Vector3D&)viewPoint->viewPosition, (::Effekseer::Vector3D&)(viewPoint->viewPosition + viewPoint->viewDirection), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
    m_efkManager->Update();

    renderingContext->invokeExtensionRendering(m_nativeGraphicsExtension.get());

#endif
}

Ref<EffectEmitter> EffectManager::createEmitterFromFile(const Path& filePath)
{
#if LN_EFFEKSEER_ENABLED
    auto efkEffect = getOrCreateEffekseerEffect(filePath);
    auto emitter = ln::makeObject_deprecated<EffekseerEffectEmitter>(this, efkEffect);
    return emitter;
#endif
    return nullptr;
}

#if LN_EFFEKSEER_ENABLED
::Effekseer::Manager* EffectManager::effekseerManager() const
{
    return m_efkManager;
}

Ref<EffekseerEffect> EffectManager::getOrCreateEffekseerEffect(const Path& filePath)
{
    const Char* exts[] = { u".efk" };
    auto assetPath = m_assetManager->findAssetPath(filePath, exts, LN_ARRAY_SIZE_OF(exts));
    if (assetPath) {
        auto effect = m_efkEffectCache.findObject(*assetPath);
        if (effect) {
            return effect;
        }
        else {
            m_fileInterface->m_effectDir = filePath.parent();

            auto stream = m_assetManager->openStreamFromAssetPath(*assetPath);
            auto data = stream->readToEnd();
            auto efkEffect = ::Effekseer::Effect::Create(effekseerManager(), data.data(), data.size(), 1.0f, nullptr);

            effect = ln::makeRef<EffekseerEffect>(efkEffect);
            m_efkEffectCache.registerObject(*assetPath, effect);

            return effect;
        }
    }
    else {
        LN_ERROR(u"File not found: " + (*assetPath));
        return nullptr;
    }
}

void EffectManager::releaseEffekseerEffect(EffekseerEffect* effect)
{
    m_efkEffectCache.releaseObject(effect);
}

#endif

} // namespace detail
} // namespace ln

