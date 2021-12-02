
#include "Internal.hpp"
#include <LuminoBitmap/Bitmap.hpp>
#include <LuminoGraphics/VertexLayout.hpp>
#include <LuminoGraphics/Shader.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include "../Engine/LinearAllocator.hpp"
#include "../../../ShaderCompiler/src/UnifiedShaderCompiler.hpp"
#include "DrawElementListBuilder.hpp"
#include "RenderFeature/BlitRenderFeature.hpp"
#include "RenderFeature/SpriteRenderFeature.hpp"
#include "RenderFeature/MeshRenderFeature.hpp"
#include "RenderFeature/PathRenderFeature.hpp"
#include "RenderFeature/ExtensionRenderFeature.hpp"
#include "RenderingProfiler.hpp"
#include "RenderingManager.hpp"

namespace ln {

const Vertex Vertex::Default;
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
    LN_LOG_DEBUG("RenderingManager Initialization started.");
    EngineDomain::registerType<Material>();

    m_graphicsManager = settings.graphicsManager;
    m_fontManager = settings.fontManager;

    static VertexElement elements[] =
    {
        { 0, VertexElementType::Float4, VertexElementUsage::Position, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::Normal, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::TexCoord, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::Color, 0 },
		{ 0, VertexElementType::Float4, VertexElementUsage::Tangent, 0 },
    };
    m_standardVertexDeclaration = makeObject<VertexLayout>(elements, LN_ARRAY_SIZE_OF(elements));
    m_standardVertexDeclarationRHI = detail::GraphicsResourceInternal::resolveRHIObject<detail::IVertexDeclaration>(nullptr, m_standardVertexDeclaration, nullptr);
    //m_renderStageListBuilder = makeRef<DrawElementListBuilder>();

	{
		static const unsigned char data[] = {
#include "Resource/Random.png.inl"
		};
		MemoryStream stream(data, LN_ARRAY_SIZE_OF(data));
		auto bmp = makeObject<Bitmap2D>();
		bmp->load(&stream);
		m_randomTexture = makeObject<Texture2D>(bmp, TextureFormat::RGBA8);
	}

	// CopyScreen
	{
		static const unsigned char data[] =
		{
#include "Resource/CopyScreen.lcfx.inl"
		};
		static const size_t size = LN_ARRAY_SIZE_OF(data);
		MemoryStream stream(data, size);
		m_builtinShaders[(int)BuiltinShader::CopyScreen] = makeObject<Shader>(_TT("CopyScreen"), &stream);
	}
	// Sprite
	{
		static const unsigned char data[] =
		{
#include "Resource/Sprite.lcfx.inl"
		};
		static const size_t size = LN_ARRAY_SIZE_OF(data);
		MemoryStream stream(data, size);
		m_builtinShaders[(int)BuiltinShader::Sprite] = makeObject<Shader>(_TT("Sprite"), &stream);
	}
    // ClusteredShadingDefault
    {
        static const unsigned char data[] =
        {
#include "Resource/ClusteredShadingDefault.lcfx.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        MemoryStream stream(data, size);
        m_builtinShaders[(int)BuiltinShader::ClusteredShadingDefault] = makeObject<Shader>(_TT("ClusteredShadingDefault"), &stream);
    }
    // ForwardGBufferPrepass
    {
        static const unsigned char data[] =
        {
#include "Resource/ForwardGBufferPrepass.lcfx.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        MemoryStream stream(data, size);
        m_builtinShaders[(int)BuiltinShader::ForwardGBufferPrepass] = makeObject<Shader>(_TT("ForwardGBufferPrepass"), &stream);
    }
	// ShadowCaster
	{
		static const unsigned char data[] =
		{
#include "Resource/ShadowCaster.lcfx.inl"
		};
		static const size_t size = LN_ARRAY_SIZE_OF(data);
		MemoryStream stream(data, size);
		m_builtinShaders[(int)BuiltinShader::ShadowCaster] = makeObject<Shader>(_TT("ShadowCaster"), &stream);
	}
	// BlackShader
	{
		static const unsigned char data[] =
		{
#include "Resource/BlackShader.lcfx.inl"
		};
		MemoryStream stream(data, LN_ARRAY_SIZE_OF(data));
		m_builtinShaders[(int)BuiltinShader::BlackShader] = makeObject<Shader>(_TT("BlackShader"), &stream);
	}
	// InfinitePlaneGrid
	{
		static const unsigned char data[] = {
#include "Resource/InfinitePlaneGrid.lcfx.inl"
		};
		createBuiltinShader(BuiltinShader::InfinitePlaneGrid, _TT("InfinitePlaneGrid"), data, LN_ARRAY_SIZE_OF(data));
	}
	// SkyLowAltitudeOptimized
	{
		static const unsigned char data[] =
		{
#include "../Scene/Resource/SkyLowAltitudeOptimized.lcfx.inl"
		};
		MemoryStream stream(data, LN_ARRAY_SIZE_OF(data));
		m_builtinShaders[(int)BuiltinShader::SkyLowAltitudeOptimized] = makeObject<Shader>(_TT("SkyLowAltitudeOptimized"), &stream);
	}
	// SkyDome
	{
		static const unsigned char data[] = {
#include "../Scene/Resource/SkyDome.lcfx.inl"
		};
		createBuiltinShader(BuiltinShader::SkyDome, _TT("SkyDome"), data, LN_ARRAY_SIZE_OF(data));
	}
	

	// LuminosityHighPassShader
	{
		const unsigned char data[] =
		{
#include "../PostEffect/Resource/LuminosityHighPassShader.lcfx.inl"
		};
		MemoryStream stream(data, LN_ARRAY_SIZE_OF(data));
		m_builtinShaders[(int)BuiltinShader::LuminosityHighPassShader] = makeObject<Shader>(_TT("LuminosityHighPassShader"), &stream);
	}
	// SeperableBlur
	{
		const unsigned char data[] =
		{
#include "../PostEffect/Resource/SeperableBlur.lcfx.inl"
		};
		MemoryStream stream(data, LN_ARRAY_SIZE_OF(data));
		m_builtinShaders[(int)BuiltinShader::SeperableBlur] = makeObject<Shader>(_TT("SeperableBlur"), &stream);
	}
	// BloomComposite
	{
		const unsigned char data[] =
		{
#include "../PostEffect/Resource/BloomComposite.lcfx.inl"
		};
		MemoryStream stream(data, LN_ARRAY_SIZE_OF(data));
		m_builtinShaders[(int)BuiltinShader::BloomComposite] = makeObject<Shader>(_TT("BloomComposite"), &stream);
	}

	// SSRRayTracing
	{
		const unsigned char data[] = {
#include "../PostEffect/Resource/SSRRayTracing.lcfx.inl"
		};
		createBuiltinShader(BuiltinShader::SSRRayTracing, _TT("SSRRayTracing"), data, LN_ARRAY_SIZE_OF(data));
	}
	// SSRBlur
	{
		const unsigned char data[] = {
#include "../PostEffect/Resource/SSRBlur.lcfx.inl"
		};
		createBuiltinShader(BuiltinShader::SSRBlur, _TT("SSRBlur"), data, LN_ARRAY_SIZE_OF(data));
	}
	// SSRComposite
	{
		const unsigned char data[] = {
#include "../PostEffect/Resource/SSRComposite.lcfx.inl"
		};
		createBuiltinShader(BuiltinShader::SSRComposite, _TT("SSRComposite"), data, LN_ARRAY_SIZE_OF(data));
	}


	// RadialBlur
	{
		const unsigned char data[] = {
#include "../PostEffect/Resource/RadialBlur.lcfx.inl"
		};
		createBuiltinShader(BuiltinShader::RadialBlur, _TT("RadialBlur"), data, LN_ARRAY_SIZE_OF(data));
	}

	// SSAOOcclusionMap
	{
		const unsigned char data[] = {
#include "../PostEffect/Resource/SSAOOcclusionMap.lcfx.inl"
		};
		createBuiltinShader(BuiltinShader::SSAOOcclusionMap, _TT("SSAOOcclusionMap"), data, LN_ARRAY_SIZE_OF(data));
	}
	// FilmicPostEffect
	{
		const unsigned char data[] = {
#include "../PostEffect/Resource/FilmicPostEffect.lcfx.inl"
		};
		createBuiltinShader(BuiltinShader::FilmicPostEffect, _TT("FilmicPostEffect"), data, LN_ARRAY_SIZE_OF(data));
	}
	// Copy
	{
		const unsigned char data[] = {
#include "../PostEffect/Resource/Copy.lcfx.inl"
		};
		createBuiltinShader(BuiltinShader::Copy, _TT("Copy"), data, LN_ARRAY_SIZE_OF(data));
	}
	// TransitionEffectWithoutMask
	{
		const unsigned char data[] = {
#include "../PostEffect/Resource/TransitionEffectWithoutMask.lcfx.inl"
		};
		createBuiltinShader(BuiltinShader::TransitionEffectWithoutMask, _TT("TransitionEffectWithoutMask"), data, LN_ARRAY_SIZE_OF(data));
	}
	// TransitionEffectWithMask
	{
		const unsigned char data[] = {
#include "../PostEffect/Resource/TransitionEffectWithMask.lcfx.inl"
		};
		createBuiltinShader(BuiltinShader::TransitionEffectWithMask, _TT("TransitionEffectWithMask"), data, LN_ARRAY_SIZE_OF(data));
	}
	// nanovg
	{
		const unsigned char data[] = {
#include "../Rendering/Resource/nanovg.lcfx.inl"
		};
		createBuiltinShader(BuiltinShader::NanoVG, _TT("nanovg"), data, LN_ARRAY_SIZE_OF(data));
	}
	// MToon
	{
		const unsigned char data[] = {
#include "../Rendering/Resource/MToon.lcfx.inl"
		};
		createBuiltinShader(BuiltinShader::MToon, _TT("MToon"), data, LN_ARRAY_SIZE_OF(data));
	}

#define ROOT_PATH _TT("C:/Proj/LN/Lumino/src/Engine/"
	//m_builtinShaders[(int)BuiltinShader::Sprite] = Shader::create(ROOT_PATH _TT("src/Rendering/Resource/Sprite.fx");
#if 0	// テスト用
	m_builtinShaders[(int)BuiltinShader::ClusteredShadingDefault] = Shader::create(ROOT_PATH _TT("src/Rendering/Resource/ClusteredShadingDefault.fx");
	m_builtinShaders[(int)BuiltinShader::SSRRayTracing] = Shader::create(ROOT_PATH _TT("src/PostEffect/Resource/SSRRayTracing.fx");
	m_builtinShaders[(int)BuiltinShader::MToon] = Shader::create(ROOT_PATH _TT("src/Rendering/Resource/MToon.hlsl");
	m_builtinShaders[(int)BuiltinShader::SSAOOcclusionMap] = Shader::create(ROOT_PATH _TT("src/PostEffect/Resource/SSAOOcclusionMap.fx");
	m_builtinShaders[(int)BuiltinShader::RadialBlur] = Shader::create(ROOT_PATH _TT("src/PostEffect/Resource/RadialBlur.fx");
	m_builtinShaders[(int)BuiltinShader::FilmicPostEffect] = Shader::create(ROOT_PATH _TT("src/PostEffect/Resource/FilmicPostEffect.fx");
	m_builtinShaders[(int)BuiltinShader::ForwardGBufferPrepass] = Shader::create(ROOT_PATH _TT("src/Rendering/Resource/ForwardGBufferPrepass.fx");
	m_builtinShaders[(int)BuiltinShader::CopyScreen] = Shader::create(ROOT_PATH _TT("src/Rendering/Resource/CopyScreen.fx");
	m_builtinShaders[(int)BuiltinShader::LuminosityHighPassShader] = Shader::create(ROOT_PATH _TT("src/PostEffect/Resource/LuminosityHighPassShader.fx");;
	m_builtinShaders[(int)BuiltinShader::SeperableBlur] = Shader::create(ROOT_PATH _TT("src/PostEffect/Resource/SeperableBlur.fx");
	m_builtinShaders[(int)BuiltinShader::BloomComposite] = Shader::create(ROOT_PATH _TT("src/PostEffect/Resource/BloomComposite.fx");
	m_builtinShaders[(int)BuiltinShader::SSRComposite] = Shader::create(ROOT_PATH _TT("src/PostEffect/Resource/SSRComposite.fx");
	m_builtinShaders[(int)BuiltinShader::SSRBlur] = Shader::create(ROOT_PATH _TT("src/PostEffect/Resource/SSRBlur.fx");
	m_builtinShaders[(int)BuiltinShader::NanoVG] = Shader::create(ROOT_PATH _TT("src/Rendering/Resource/nanovg.fx");


	m_builtinShaders[(int)BuiltinShader::BlackShader] = Shader::create(ROOT_PATH _TT("src/Rendering/Resource/BlackShader.fx");

	m_builtinShaders[(int)BuiltinShader::ShadowCaster] = Shader::create(ROOT_PATH _TT("src/Rendering/Resource/ShadowCaster.fx");
#endif

	{
#if 0
		const auto code = FileSystem::readAllBytes(_TT("C:/Proj/LN/Lumino/src/Engine/src/Rendering/Resource/BlendShape.compute");
		Ref<DiagnosticsManager> localDiag = makeObject<DiagnosticsManager>();
		UnifiedShaderCompiler compiler(EngineDomain::shaderManager(), localDiag);
		compiler.compileCompute((const char*)code.data(), code.size(), "Main", {}, {});
		UnifiedShader* ush = compiler.unifiedShader();
		auto shader = makeObject<Shader>(ush, localDiag);
		m_builtinShaders[(int)BuiltinShader::BlendShape] = shader;
		//shader->descriptorLayout()->
		blendShapeShader.shader = shader;
		blendShapeShader.shaderPass = shader->techniques()[0]->passes()[0];

		const auto& layout = shader->descriptorLayout();
		blendShapeShader.dstVerticesGID = layout->findStorageRegisterIndex(_TT("dstVertices");
		blendShapeShader.srcVerticesGID = layout->findTextureRegisterIndex(_TT("srcVertices");
		blendShapeShader.target0GID = layout->findTextureRegisterIndex(_TT("target0");
		blendShapeShader.target1GID = layout->findTextureRegisterIndex(_TT("target1");
		blendShapeShader.target2GID = layout->findTextureRegisterIndex(_TT("target2");
		blendShapeShader.target3GID = layout->findTextureRegisterIndex(_TT("target3");
		blendShapeShader.blendInfoGID = layout->findUniformBufferRegisterIndex(_TT("BlendInfo");
#endif
	}


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

	m_pathRenderFeature = makeObject<PathRenderFeature>(this);
	m_renderFeatures.add(m_pathRenderFeature);

	m_extensionRenderFeature = makeObject<ExtensionRenderFeature>(this);
	m_renderFeatures.add(m_extensionRenderFeature);

	m_profiler = std::make_unique<RenderingProfiler>();

	m_stageDataPageManager = makeRef<LinearAllocatorPageManager>();

	m_defaultMaterial = makeObject<Material>();


    {
        m_builtinMaterials[(int)BuiltinMaterial::Default] = Material::create();
    }
    {
        auto material = Material::create();
        material->setShadingModel(ShadingModel::Unlit);
        m_builtinMaterials[(int)BuiltinMaterial::Unlit] = material;
    }

    LN_LOG_DEBUG("RenderingManager Initialization finished.");
}

void RenderingManager::dispose()
{
	LN_LOG_DEBUG("RenderingManager dispose started.");

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
	m_standardVertexDeclarationRHI = nullptr;
	m_standardVertexDeclaration = nullptr;
    m_renderFeatures.clear();

	LN_LOG_DEBUG("RenderingManager dispose finished.");
}

void RenderingManager::createBuiltinShader(BuiltinShader index, const Char* name, const void* data, int dataLen)
{
	MemoryStream stream(data, dataLen);
	m_builtinShaders[static_cast<int>(index)] = makeObject<Shader>(name, &stream);
}

} // namespace detail
} // namespace ln

