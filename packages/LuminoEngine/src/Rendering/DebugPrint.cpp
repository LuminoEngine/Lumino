#include "Internal.hpp"
#include <LuminoEngine/Graphics/GPU/Texture.hpp>
#include <LuminoEngine/Graphics/GPU/RenderPass.hpp>
#include <LuminoEngine/Graphics/GPU/SwapChain.hpp>
#include <LuminoEngine/Rendering/SurfaceContext.hpp>
#include <LuminoEngine/Rendering/RenderViewPoint.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/BatchRenderer.hpp>
#include <LuminoEngine/Rendering/Kanata/KDrawEvent.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxyCollector.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/DebugPrint.hpp>

namespace ln {

DebugPrint::DebugPrint(detail::RenderingManager* manager, BatchRenderer* batchRenderer)
    : m_manager(manager)
    , m_batchRenderer(batchRenderer)
    , m_fontTexture(nullptr)
    , m_material(nullptr)
    , m_textBuffer{}
    , m_textBufferUsed(0)
    , m_renderPass(nullptr)
    , m_viewPoint(nullptr) {
}

DebugPrint::~DebugPrint() {
}

MaybeResult DebugPrint::init() {
    // CopyScreen
    {
        static const unsigned char data[] = {
#include "Resource/text_16.png.inl"
        };
        Result<Ref<Texture2D>> texture = Texture2D::createFromImageFileData(data, LN_ARRAY_SIZE_OF(data));
        if (!texture) {
            return LN_TO_ERROR(texture);
        }
        m_fontTexture = texture.value();
    }

    m_material = Material::create(m_fontTexture);

    m_renderPass = makeRef<kanata::DrawEventsEncoder>(m_manager);

    m_viewPoint = makeRef<RenderViewPoint>();

    return LN_MAKE_SUCCESS();
}

void DebugPrint::dispose() {
    m_fontTexture = nullptr;
}

void DebugPrint::print(const std::string_view& text) {
    if (m_textBufferUsed + (text.size() + 1) > m_textBuffer.size()) {
        return;
    }
    for (char c : text) {
        m_textBuffer[m_textBufferUsed++] = c;
    }
    m_textBuffer[m_textBufferUsed++] = '\n';
}

MaybeResult DebugPrint::render(SurfaceContext* surfaceContext, CommandList* commandList) {
    GraphicsContext* graphicsContext = surfaceContext->context();
    GraphicsCommandList* graphicsCommandList = surfaceContext->commandList();
    RenderTargetTexture* colorBuffer = graphicsContext->currentBackbuffer();
    DepthBuffer* depthBuffer = graphicsContext->currentDepthBuffer();
    RenderPass* renderPass = RenderPass::get(
        colorBuffer,
        nullptr,//depthBuffer,
        ln::ClearFlags::None, Color::Red);
    //graphicsCommandList->beginRenderPass(renderPass);

    Size viewSize(colorBuffer->width(), colorBuffer->height());
    m_viewPoint->resetPerspective2DLH(Vector3(0, 0, 0), viewSize, 0.0f, 100.0f);

    commandList->clearCommandsAndState(m_viewPoint);

    const float paddingLeft = 8;
    const float paddingTop = 8;
    const float frameWidth = 16;
    const float frameHeight = 20;
    const int imageCols = 16;
    const float textureWidth = static_cast<float>(m_fontTexture->width());
    const float textureHeight = static_cast<float>(m_fontTexture->height());
    m_batchRenderer->begin(commandList, m_material);

    float textX = 0;
    float textY = 0;
    for (size_t i = 0; i < m_textBufferUsed; i++) {
        char codePoint = m_textBuffer[i];
        if (codePoint == '\n') {
            textX = 0;
            textY += frameHeight;
            continue;
        }
        if (codePoint < 0 || 127 < codePoint) {
            codePoint = 63; // ?
        }
        const float x = (codePoint % imageCols) * frameWidth;
        const float y = (codePoint / imageCols) * frameHeight;
        const Rect srcUVRect(
            x / textureWidth,
            y / textureHeight,
            frameWidth / textureWidth,
            frameHeight / textureHeight);
        Matrix pos = Matrix::makeTranslation(Vector3(paddingLeft + textX, paddingTop + textY, 0));
        SpriteData data;
        data.size = Size(frameWidth, frameHeight);
        data.anchorRatio = Vector2::Zero;
        data.srcUVRect = srcUVRect;
        data.color = Color::White;
        data.baseDirection = SpriteBaseDirection::Basic2D;
        data.billboardType = BillboardType::None;
        data.flipFlags = SpriteFlipFlags::None;
        m_batchRenderer->drawSprite(pos, data);
        textX += frameWidth;
    }

    m_batchRenderer->end();
    //graphicsCommandList->endRenderPass();

    

    // Render commands
    {
        kanata::DrawEventList* drawEventList = surfaceContext->drawEventList();
        kanata::BatchCollector* batchList = commandList->batchCollector();
        batchList->clear(m_viewPoint);


        auto& batchProxyCollector = commandList->batchProxyCollector();
        batchProxyCollector->resolveSingleFrameBatchProxies(batchList);

        drawEventList->clear();
        //ln::ElapsedTimer t1;

        detail::SceneInfo sceneInfo;
        detail::RenderViewInfo renderViewInfo;
        m_viewPoint->makeCameraInfo(&renderViewInfo.cameraInfo);

        m_renderPass->buildDrawEvents(
            batchList,
            graphicsCommandList,
            renderPass,
            renderViewInfo,
            sceneInfo,
            drawEventList);
        //std::cout << t1.elapsedMilliseconds() << "[ms] buildDrawCommands" << std::endl;
        ln::ElapsedTimer t2;
        drawEventList->submitDrawEvents(graphicsCommandList);
        //std::cout << t2.elapsedMilliseconds() << "[ms] xx" << std::endl;
    }

    m_textBufferUsed = 0;

    return LN_MAKE_SUCCESS();
}

} // namespace ln
