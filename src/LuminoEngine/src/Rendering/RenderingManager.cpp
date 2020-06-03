
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include "../Engine/LinearAllocator.hpp"
#include "DrawElementListBuilder.hpp"
#include "RenderFeature/BlitRenderFeature.hpp"
#include "RenderFeature/SpriteRenderFeature.hpp"
#include "RenderFeature/MeshRenderFeature.hpp"
#include "RenderFeature/ExtensionRenderFeature.hpp"
#include "RenderingManager.hpp"

namespace ln {

const Vertex Vertex::Default = {
	{ 0, 0, 0 },
	{ 0, 0, 1 },
	{ 0, 0 },
	{ 1, 1, 1, 1 },
	{ 1, 0, 0, 1 },
};
const VertexBlendWeight VertexBlendWeight::Default{ {0, 0, 0, 0}, {0, 0, 0, 0} };

namespace detail {

//==============================================================================
// RenderingManager
	
RenderingManager::RenderingManager()
	: m_graphicsManager(nullptr)
	, m_fontManager(nullptr)
    , m_standardVertexDeclaration(nullptr)
	//, m_spriteRenderFeature(nullptr)
	, m_meshRenderFeature(nullptr)
    , m_meshGeneraterRenderFeature(nullptr)
	, m_stageDataPageManager(nullptr)
{
}

void RenderingManager::init(const Settings& settings)
{
    LN_LOG_DEBUG << "RenderingManager Initialization started.";
    EngineDomain::registerType<Material>();

    m_graphicsManager = settings.graphicsManager;
    m_fontManager = settings.fontManager;

    static VertexElement elements[] =
    {
        { 0, VertexElementType::Float3, VertexElementUsage::Position, 0 },
        { 0, VertexElementType::Float3, VertexElementUsage::Normal, 0 },
        { 0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::Color, 0 },
		{ 0, VertexElementType::Float4, VertexElementUsage::Tangent, 0 },
    };
    m_standardVertexDeclaration = makeObject<VertexLayout>(elements, LN_ARRAY_SIZE_OF(elements));
    m_standardVertexDeclarationRHI = detail::GraphicsResourceInternal::resolveRHIObject<detail::IVertexDeclaration>(nullptr, m_standardVertexDeclaration, nullptr);
    //m_renderStageListBuilder = makeRef<DrawElementListBuilder>();

	m_clearRenderFeature = makeObject<ClearRenderFeature>();
	m_renderFeatures.add(m_clearRenderFeature);

    m_blitRenderFeature = makeObject<BlitRenderFeature>(this);
	m_renderFeatures.add(m_blitRenderFeature);

	m_spriteRenderFeature2 = makeObject<SpriteRenderFeature2>(this);
	m_renderFeatures.add(m_spriteRenderFeature2);

    m_meshRenderFeature = makeObject<MeshRenderFeature>(this);
	m_renderFeatures.add(m_meshRenderFeature);

    m_meshGeneraterRenderFeature = makeObject<MeshGeneraterRenderFeature>(this);
	m_renderFeatures.add(m_meshGeneraterRenderFeature);

	m_primitiveRenderFeature = makeObject<PrimitiveRenderFeature>();
	m_renderFeatures.add(m_primitiveRenderFeature);

    m_spriteTextRenderFeature = makeObject<SpriteTextRenderFeature>(this);
	m_renderFeatures.add(m_spriteTextRenderFeature);

	m_frameRectRenderFeature = makeObject<FrameRectRenderFeature>(this);
	m_renderFeatures.add(m_frameRectRenderFeature);

#ifdef LN_BOX_ELEMENT_RENDER_FEATURE_TEST
	m_shapesRenderFeature = makeObject<ShapesRenderFeature2>(this);
#else
	m_shapesRenderFeature = makeObject<ShapesRenderFeature>(this);
#endif
	m_renderFeatures.add(m_shapesRenderFeature);

    m_extensionRenderFeature = makeObject<ExtensionRenderFeature>(this);
    m_renderFeatures.add(m_extensionRenderFeature);


    m_stageDataPageManager = makeRef<LinearAllocatorPageManager>();

	// Sprite
	{
		static const unsigned char data[] =
		{
#include "Resource/Sprite.lcfx.inl"
		};
		static const size_t size = LN_ARRAY_SIZE_OF(data);
		MemoryStream stream(data, size);
		m_builtinShaders[(int)BuiltinShader::Sprite] = makeObject<Shader>(u"Sprite", &stream);
	}
    // ClusteredShadingDefault
    {
        static const unsigned char data[] =
        {
#include "Resource/ClusteredShadingDefault.lcfx.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        MemoryStream stream(data, size);
        m_builtinShaders[(int)BuiltinShader::ClusteredShadingDefault] = makeObject<Shader>(u"ClusteredShadingDefault", &stream);
    }
    // DepthPrepass
    {
        static const unsigned char data[] =
        {
#include "Resource/DepthPrepass.lcfx.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        MemoryStream stream(data, size);
        m_builtinShaders[(int)BuiltinShader::DepthPrepass] = makeObject<Shader>(u"DepthPrepass", &stream);
    }
	// BlackShader
	{
		static const unsigned char data[] =
		{
#include "Resource/BlackShader.lcfx.inl"
		};
		MemoryStream stream(data, LN_ARRAY_SIZE_OF(data));
		m_builtinShaders[(int)BuiltinShader::BlackShader] = makeObject<Shader>(u"BlackShader", &stream);
	}
	// SkyLowAltitudeOptimized
	{
		static const unsigned char data[] =
		{
#include "../Scene/Resource/SkyLowAltitudeOptimized.lcfx.inl"
		};
		MemoryStream stream(data, LN_ARRAY_SIZE_OF(data));
		m_builtinShaders[(int)BuiltinShader::SkyLowAltitudeOptimized] = makeObject<Shader>(u"SkyLowAltitudeOptimized", &stream);
	}
	

	// LuminosityHighPassShader
	{
		const unsigned char data[] =
		{
#include "../ImageEffect/Resource/LuminosityHighPassShader.lcfx.inl"
		};
		MemoryStream stream(data, LN_ARRAY_SIZE_OF(data));
		m_builtinShaders[(int)BuiltinShader::LuminosityHighPassShader] = makeObject<Shader>(u"LuminosityHighPassShader", &stream);
	}
	// SeperableBlur
	{
		const unsigned char data[] =
		{
#include "../ImageEffect/Resource/SeperableBlur.lcfx.inl"
		};
		MemoryStream stream(data, LN_ARRAY_SIZE_OF(data));
		m_builtinShaders[(int)BuiltinShader::SeperableBlur] = makeObject<Shader>(u"SeperableBlur", &stream);
	}
	// BloomComposite
	{
		const unsigned char data[] =
		{
#include "../ImageEffect/Resource/BloomComposite.lcfx.inl"
		};
		MemoryStream stream(data, LN_ARRAY_SIZE_OF(data));
		m_builtinShaders[(int)BuiltinShader::BloomComposite] = makeObject<Shader>(u"BloomComposite", &stream);
	}
	// RadialBlur
	{
		const unsigned char data[] =
		{
#include "../ImageEffect/Resource/RadialBlur.lcfx.inl"
		};
		MemoryStream stream(data, LN_ARRAY_SIZE_OF(data));
		m_builtinShaders[(int)BuiltinShader::RadialBlur] = makeObject<Shader>(u"RadialBlur", &stream);
	}


	m_builtinShaders[(int)BuiltinShader::Sprite] = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx");
	m_builtinShaders[(int)BuiltinShader::ClusteredShadingDefault] = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/ClusteredShadingDefault.fx");
#if 0
    m_builtinShaders[(int)BuiltinShader::ClusteredShadingDefault] = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/ClusteredShadingDefault.fx");
    m_builtinShaders[(int)BuiltinShader::DepthPrepass] = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/DepthPrepass.fx");
	m_builtinShaders[(int)BuiltinShader::BlackShader] = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/BlackShader.fx");
	m_builtinShaders[(int)BuiltinShader::SkyLowAltitudeOptimized] = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Scene/Resource/SkyLowAltitudeOptimized.fx");
	m_builtinShaders[(int)BuiltinShader::LuminosityHighPassShader] = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/LuminosityHighPassShader.fx");
	m_builtinShaders[(int)BuiltinShader::SeperableBlur] = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/SeperableBlur.fx");
	m_builtinShaders[(int)BuiltinShader::BloomComposite] = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/BloomComposite.fx");
	m_builtinShaders[(int)BuiltinShader::RadialBlur] = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/RadialBlur.fx");

    m_builtinShaders[(int)BuiltinShader::ShadowCaster] = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/ShadowCaster.fx");
 //   m_builtinShaders[(int)BuiltinShader::ScreenBlurImageEffect] = Shader::create(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/ImageEffect/Resource/ScreenBlurImageEffect.fx");
    //m_builtinShaders[(int)BuiltinShader::ToneImageEffect] = Shader::create(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/ImageEffect/Resource/ToneImageEffect.fx");
#endif

    {
        m_builtinMaterials[(int)BuiltinMaterial::Default] = Material::create();
    }
    {
        auto material = Material::create();
        material->shadingModel = ShadingModel::Unlit;
        m_builtinMaterials[(int)BuiltinMaterial::Unlit] = material;
    }

    LN_LOG_DEBUG << "RenderingManager Initialization ended.";
}

void RenderingManager::dispose()
{
	for (int i = 0; i < m_builtinShaders.size(); i++) {
		m_builtinShaders[i] = nullptr;
	}
	m_stageDataPageManager = nullptr;
	m_shapesRenderFeature = nullptr;
	m_frameRectRenderFeature = nullptr;
    m_spriteTextRenderFeature = nullptr;
    m_meshGeneraterRenderFeature = nullptr;
	m_meshRenderFeature = nullptr;
	m_spriteRenderFeature2 = nullptr;
    m_blitRenderFeature = nullptr;
	//m_renderStageListBuilder = nullptr;
	m_standardVertexDeclaration = nullptr;
    m_renderFeatures.clear();
}

} // namespace detail
} // namespace ln

