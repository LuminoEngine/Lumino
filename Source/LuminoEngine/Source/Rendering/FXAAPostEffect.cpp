
#include "../Internal.h"
//#include <Lumino/Graphics/Texture.h>
//#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/Material.h>
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Rendering/FXAAPostEffect.h>
#include <Lumino/Scene/Camera.h>
#include "../Animation/AnimationManager.h"
#include "../Graphics/GraphicsManager.h"
//#include "../Rendering/ClusteredShadingSceneRenderer.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// FXAAPostEffect
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(FXAAPostEffect, PostEffect);

FXAAPostEffect::FXAAPostEffect()
{
	initializeProperties();
}

FXAAPostEffect::~FXAAPostEffect()
{
}

void FXAAPostEffect::initialize()
{
	PostEffect::initialize();

	m_material = Object::makeRef<CommonMaterial>();
	m_material->setShader(Object::makeRef<Shader>(detail::GraphicsManager::getInstance(), _T("D:/Proj/LN/HC1/External/Lumino/Source/LuminoEngine/Source/Rendering/Resource/FXAAPostEffect.fx"), ShaderCodeType::RawHLSL));
}

void FXAAPostEffect::onRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
	context->blit(source, destination, m_material);
}

LN_NAMESPACE_END
