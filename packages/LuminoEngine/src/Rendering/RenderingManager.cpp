#include "Internal.hpp"
#include <LuminoEngine/Graphics/Bitmap/Bitmap.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/GraphicsDeviceContext.hpp>
#include <LuminoEngine/Graphics/GPU/VertexLayout.hpp>
#include <LuminoEngine/Graphics/GPU/Shader.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoCore/Base/LinearAllocator.hpp>
#include "DrawElementListBuilder.hpp"
#include "RenderFeature/BlitRenderFeature.hpp"
#include "RenderFeature/MeshRenderFeature.hpp"
#include <LuminoEngine/Rendering/detail/RenderingProfiler.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Rendering/DebugPrint.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/ShaderCompiler.hpp>

#include <LuminoEngine/Rendering/Kanata/RenderFeature/KPrimitiveMeshRenderer.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KScreenRectangleRenderFeature.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KMeshRenderFeature.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KShapesRenderFeature.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KSpriteTextRenderFeature.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KFrameRectRenderFeature.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/BatchInstructionEncoder.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/BatchRenderer.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/PrimitiveMeshRenderer.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/SpriteTextRenderer.hpp>

namespace ln {

const Vertex Vertex::Default;
const VertexBlendWeight VertexBlendWeight::Default{ { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

namespace detail {

//==============================================================================
// RenderingManager

RenderingManager* RenderingManager::s_instance = nullptr;
RenderingManager* RenderingManager::instance() {
    return s_instance;
}

RenderingManager::RenderingManager()
    : m_graphicsManager(nullptr)
    , m_fontManager(nullptr)
    , m_standardVertexDeclaration(nullptr)
    , m_stageDataPageManager(nullptr) {
    s_instance = this;
}

RenderingManager::~RenderingManager() {
}

MaybeResult RenderingManager::init(const Options& options) {
    LN_LOG_DEBUG("RenderingManager Initialization started.");
    auto* context = RuntimeContext::current();

    m_graphicsManager = options.graphicsManager;
    m_fontManager = options.fontManager;

    static VertexElement elements[] = {
        { 0, VertexElementType::Float4, VertexElementUsage::Position, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::Normal, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::TexCoord, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::Color, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::Tangent, 0 },
    };
    m_standardVertexDeclaration = makeObject_deprecated<VertexLayout>(elements, LN_ARRAY_SIZE_OF(elements));

    // CopyScreen
    {
        static const unsigned char data[] = {
#include "Resource/CopyScreen.lcsh.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        auto result = Shader::createFromCompiledShader(data, size, "CopyScreen");
        if (!result) {
            return LN_TO_ERROR(result);
        }
        m_builtinShaders[(int)BuiltinShader::CopyScreen] = result.value();
    }
    // Sprite
    {
        static const unsigned char data[] = {
#include "Resource/Sprite.lcsh.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        auto result = Shader::createFromCompiledShader(data, size, "Sprite");
        if (!result) {
            return LN_TO_ERROR(result);
        }
        m_builtinShaders[(int)BuiltinShader::Sprite] = result.value();
    }

//#ifdef LN_USE_SLANG // テスト用
//    const auto dir = Path(String::fromCString(__FILE__)).parent() / U"Resource";
//    auto result = ln::kokage::ShaderCompiler::create();
//    auto result2 = result->get()->build(
//        "C:/Proj/Lumino/packages/LuminoEngine/src/Rendering/Resource/Sprite.slang");
//        //"E:/Proj/Lumino/packages/LuminoEngine/src/Rendering/Resource/Sprite.slang");
//    //if (!result2) return;
//    m_builtinShaders[(int)BuiltinShader::Sprite]->setupShader2(result->get()->shader());
//#endif

    m_screenRectangleRenderFeature = makeURef<kanata::ScreenRectangleRenderFeature>(this);
    m_screenRectangleRenderFeature->init();
    m_primitiveRenderer = makeURef<kanata::PrimitiveMeshRenderer>(this);
    m_meshRenderFeature = makeURef<kanata::MeshRenderFeature>(this);
    m_shapesRenderFeature = makeURef<kanata::ShapesRenderFeature>(this);
    m_spriteTextRenderFeature = makeURef<kanata::SpriteTextRenderFeature>(this);
    m_frameRectRenderFeature = makeURef<kanata::FrameRectRenderFeature>(this);

    m_batchInstructionDispatcher = makeURef<BatchInstructionEncoder>(this);
    m_spriteRenderer = Ref<BatchRenderer>(LN_NEW BatchRenderer(), false);
    if (!m_spriteRenderer->init()) {
        return LN_MAKE_ERROR();
    }
    m_primitiveMeshRenderer = Ref<PrimitiveMeshRenderer>(LN_NEW PrimitiveMeshRenderer(), false);
    if (!m_primitiveMeshRenderer->init()) {
        return LN_MAKE_ERROR();
    }
    m_spriteTextRenderer = Ref<SpriteTextRenderer>(LN_NEW SpriteTextRenderer(), false);
    if (!m_spriteTextRenderer->init()) {
        return LN_MAKE_ERROR();
	}

    m_profiler = std::make_unique<RenderingProfiler>();

    m_stageDataPageManager = makeRef<LinearAllocatorPageManager>();

    m_defaultMaterial = Material::create();

    {
        m_builtinMaterials[(int)BuiltinMaterial::Default] = Material::create();
    }
    {
        auto material = Material::create();
        material->setShadingModel(ShadingModel::Unlit);
        m_builtinMaterials[(int)BuiltinMaterial::Unlit] = material;
    }

    m_primitiveMeshDefaultMaterial = Material::create();
    m_primitiveMeshDefaultMaterial->setColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
    m_primitiveMeshDefaultMaterial->setRoughness(0.5f);
    m_primitiveMeshDefaultMaterial->setMetallic(0.0f);

    // DebugPrint
    {
        m_debugPrint = makeURef<DebugPrint>(this, m_spriteRenderer.get());
        auto result = m_debugPrint->init();
        if (!result) {
            return result;
        }
    }

    LN_LOG_DEBUG("RenderingManager Initialization finished.");
    return LN_MAKE_SUCCESS();
}

void RenderingManager::dispose() {
    LN_LOG_DEBUG("RenderingManager dispose started.");

    for (int i = 0; i < m_builtinShaders.size(); i++) {
        m_builtinShaders[i] = nullptr;
    }
    m_stageDataPageManager = nullptr;
    //m_shapesRenderFeature = nullptr;
    //m_frameRectRenderFeature = nullptr;
    //m_spriteTextRenderFeature = nullptr;
    //m_meshGeneraterRenderFeature = nullptr;
    //m_meshRenderFeature = nullptr;
    //m_spriteRenderFeature2 = nullptr;
    //m_blitRenderFeature = nullptr;
    //m_standardVertexDeclarationRHI = nullptr;

    if (m_debugPrint) {
        m_debugPrint->dispose();
        m_debugPrint = nullptr;
    }

    m_standardVertexDeclaration = nullptr;
    //m_renderFeatures.clear();

    LN_LOG_DEBUG("RenderingManager dispose finished.");
}

} // namespace detail
} // namespace ln
