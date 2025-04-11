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
#include "RenderFeature/PathRenderFeature.hpp"
#include "RenderFeature/ExtensionRenderFeature.hpp"
#include <LuminoEngine/Rendering/detail/RenderingProfiler.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
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

RenderingManager* RenderingManager::initialize(const Settings& settings) {
    if (s_instance) return s_instance;

    auto m = Ref<RenderingManager>(LN_NEW detail::RenderingManager(), false);
    s_instance = m;
    if (!m->init(settings)) return nullptr;

    EngineInstance::instance()->registerModule(m);
    return m;
}

void RenderingManager::terminate() {
    if (s_instance) {
        s_instance->dispose();
        EngineInstance::instance()->unregisterModule(s_instance);
        s_instance = nullptr;
    }
}

RenderingManager::RenderingManager()
    : m_graphicsManager(nullptr)
    , m_fontManager(nullptr)
    , m_standardVertexDeclaration(nullptr)
    //, m_spriteRenderFeature(nullptr)
    //, m_meshRenderFeature(nullptr)
    //, m_meshGeneraterRenderFeature(nullptr)
    , m_stageDataPageManager(nullptr) {
    assert(s_instance == nullptr);
}

RenderingManager::~RenderingManager() {
}

bool RenderingManager::init(const Settings& settings) {
    LN_LOG_DEBUG("RenderingManager Initialization started.");
    auto* context = RuntimeContext::current();

    m_graphicsManager = settings.graphicsManager;
    m_fontManager = settings.fontManager;

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
#include "Resource/CopyScreen.lcfx.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        MemoryStream stream(data, size);
        m_builtinShaders[(int)BuiltinShader::CopyScreen] = makeObject_deprecated<Shader>(_TT("CopyScreen"), &stream);
    }
    // Sprite
    {
        static const unsigned char data[] = {
#include "Resource/Sprite.lcfx.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        MemoryStream stream(data, size);
        m_builtinShaders[(int)BuiltinShader::Sprite] = makeObject_deprecated<Shader>(_TT("Sprite"), &stream);
    }

#ifdef LN_USE_SLANG // テスト用
    const auto dir = Path(String::fromCString(__FILE__)).parent() / U"Resource";
    auto result = ln::kokage::ShaderCompiler::create();
    auto result2 = result->get()->build(
        //"C:/Proj/LN/Lumino/packages/LuminoEngine/src/Rendering/Resource/Sprite.slang");
        "E:/Proj/Lumino/packages/LuminoEngine/src/Rendering/Resource/Sprite.slang");
    //if (!result2) return;
    m_builtinShaders[(int)BuiltinShader::Sprite]->setupShader2(result->get()->shader());
#endif

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
        return false;
    }
    m_primitiveMeshRenderer = Ref<PrimitiveMeshRenderer>(LN_NEW PrimitiveMeshRenderer(), false);
    if (!m_primitiveMeshRenderer->init()) {
        return false;
    }
    m_spriteTextRenderer = Ref<SpriteTextRenderer>(LN_NEW SpriteTextRenderer(), false);
    if (!m_spriteTextRenderer->init()) {
		return false;
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

    LN_LOG_DEBUG("RenderingManager Initialization finished.");
    return true;
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
    m_standardVertexDeclaration = nullptr;
    //m_renderFeatures.clear();

    LN_LOG_DEBUG("RenderingManager dispose finished.");
}

void RenderingManager::createBuiltinShader(BuiltinShader index, const Char* name, const void* data, int dataLen) {
    MemoryStream stream(data, dataLen);
    m_builtinShaders[static_cast<int>(index)] = makeObject_deprecated<Shader>(name, &stream);
}

} // namespace detail
} // namespace ln
