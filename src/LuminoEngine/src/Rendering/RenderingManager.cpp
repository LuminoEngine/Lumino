
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexDeclaration.hpp>
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include "../Engine/LinearAllocator.hpp"
#include "DrawElementListBuilder.hpp"
#include "BlitRenderFeature.hpp"
#include "SpriteRenderFeature.hpp"
#include "MeshRenderFeature.hpp"
#include "RenderTargetTextureCache.hpp"
#include "RenderingManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// RenderingManager
	
RenderingManager::RenderingManager()
	: m_graphicsManager(nullptr)
	, m_standardVertexDeclaration(nullptr)
	, m_spriteRenderFeature(nullptr)
	, m_meshRenderFeature(nullptr)
    , m_primitiveRenderFeature(nullptr)
	, m_stageDataPageManager(nullptr)
{
}

void RenderingManager::initialize(const Settings& settings)
{
    LN_LOG_DEBUG << "RenderingManager Initialization started.";

    m_graphicsManager = settings.graphicsManager;

    m_renderTargetTextureCacheManager = makeRef<RenderTargetTextureCacheManager>();
    m_depthBufferCacheManager = makeRef<DepthBufferCacheManager>();
    m_frameBufferCache = makeRef<detail::FrameBufferCache>(m_renderTargetTextureCacheManager, m_depthBufferCacheManager);

    static VertexElement elements[] =
    {
        { 0, VertexElementType::Float3, VertexElementUsage::Position, 0 },
        { 0, VertexElementType::Float3, VertexElementUsage::Normal, 0 },
        { 0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::Color, 0 },
    };
    m_standardVertexDeclaration = newObject<VertexDeclaration>(elements, 4);
    m_renderStageListBuilder = makeRef<DrawElementListBuilder>();

    m_blitRenderFeature = newObject<BlitRenderFeature>(this);
    m_spriteRenderFeature = newObject<SpriteRenderFeature>(this);
    m_meshRenderFeature = newObject<MeshRenderFeature>(this);
    m_primitiveRenderFeature = newObject<PrimitiveRenderFeature>(this);

    m_stageDataPageManager = makeRef<LinearAllocatorPageManager>();

#if 1
    // ClusteredShadingDefault.lcfx.h
    {
        static const unsigned char data[] =
        {
#include "Resource/ClusteredShadingDefault.lcfx.h"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        MemoryStream stream(data, size);
        m_builtinShaders[(int)BuiltinShader::ClusteredShadingDefault] = newObject<Shader>(u"ClusteredShadingDefault", &stream);
    }
    // DepthPrepass.lcfx.h
    {
        static const unsigned char data[] =
        {
#include "Resource/DepthPrepass.lcfx.h"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        MemoryStream stream(data, size);
        m_builtinShaders[(int)BuiltinShader::DepthPrepass] = newObject<Shader>(u"DepthPrepass", &stream);
    }
    // Sprite.lcfx.h
    {
        static const unsigned char data[] =
        {
#include "Resource/Sprite.lcfx.h"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        MemoryStream stream(data, size);
        m_builtinShaders[(int)BuiltinShader::Sprite] = newObject<Shader>(u"Sprite", &stream);
    }
#endif

    //#if 0
    //	m_builtinShaders[(int)BuiltinShader::ClusteredShadingDefault] = Shader::create(u"C:/Proj/GitHub/Lumino/src/LuminoEngine/src/Rendering/Resource/ClusteredShadingDefault.fx");
    //	m_builtinShaders[(int)BuiltinShader::Sprite] = Shader::create(u"C:/Proj/GitHub/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx");
    //	m_builtinShaders[(int)BuiltinShader::DepthPrepass] = Shader::create(u"C:/Proj/GitHub/Lumino/src/LuminoEngine/src/Rendering/Resource/DepthPrepass.fx");
    //	m_builtinShaders[(int)BuiltinShader::ShadowCaster] = Shader::create(u"C:/Proj/GitHub/Lumino/src/LuminoEngine/src/Rendering/Resource/ShadowCaster.fx");
    //#endif
#if 1
    m_builtinShaders[(int)BuiltinShader::ClusteredShadingDefault] = Shader::create(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/Rendering/Resource/ClusteredShadingDefault.fx");
    m_builtinShaders[(int)BuiltinShader::Sprite] = Shader::create(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx");
    //m_builtinShaders[(int)BuiltinShader::DepthPrepass] = Shader::create(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/Rendering/Resource/DepthPrepass.fx");
    //m_builtinShaders[(int)BuiltinShader::ShadowCaster] = Shader::create(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/Rendering/Resource/ShadowCaster.fx");
 //   m_builtinShaders[(int)BuiltinShader::ScreenBlurImageEffect] = Shader::create(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/ImageEffect/Resource/ScreenBlurImageEffect.fx");
    //m_builtinShaders[(int)BuiltinShader::ToneImageEffect] = Shader::create(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/ImageEffect/Resource/ToneImageEffect.fx");
#endif
//#if 0
//
//
//	m_builtinShaders[(int)BuiltinShader::ClusteredShadingDefault] = Shader::create(
//		u"/Users/lriki/Proj/Lumino/src/LuminoEngine/src/Rendering/Resource/ClusteredShadingDefault.fx");
//	
//	std::cout << "RenderingManager 12" << std::endl;
//	m_builtinShaders[(int)BuiltinShader::Sprite] = Shader::create(u"/Users/lriki/Proj/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx");
//	
//	std::cout << "RenderingManager 13" << std::endl;
//	m_builtinShaders[(int)BuiltinShader::DepthPrepass] = Shader::create(u"/Users/lriki/Proj/Lumino/src/LuminoEngine/src/Rendering/Resource/DepthPrepass.fx");
//	
//	//std::cout << "RenderingManager 14" << std::endl;
//	//m_builtinShaders[(int)BuiltinShader::ShadowCaster] = Shader::create(u"/Users/lriki/Proj/Lumino/src/LuminoEngine/src/Rendering/Resource/ShadowCaster.fx");
//#endif

    {
        m_builtinMaterials[(int)BuiltinMaterial::Default] = Material::create();
    }
    {
        auto material = Material::create();
        material->shadingModel = ShadingModel::UnLighting;
        m_builtinMaterials[(int)BuiltinMaterial::UnLighting] = material;
    }


    LN_LOG_DEBUG << "RenderingManager Initialization ended.";
}

void RenderingManager::dispose()
{
	for (int i = 0; i < m_builtinShaders.size(); i++) {
		m_builtinShaders[i] = nullptr;
	}
	m_stageDataPageManager = nullptr;
    m_primitiveRenderFeature = nullptr;
	m_meshRenderFeature = nullptr;
	m_spriteRenderFeature = nullptr;
    m_blitRenderFeature = nullptr;
	m_renderStageListBuilder = nullptr;
	m_standardVertexDeclaration = nullptr;
	m_depthBufferCacheManager = nullptr;
	m_renderTargetTextureCacheManager = nullptr;
}

} // namespace detail
} // namespace ln

