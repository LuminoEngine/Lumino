
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/ImageEffect/BloomImageEffect.hpp>

namespace ln {

//==============================================================================
// BloomImageEffect

static const int MIPS = 5;
static const Vector2 BlurDirectionX(1.0, 0.0);
static const Vector2 BlurDirectionY(0.0, 1.0);

Ref<BloomImageEffect> BloomImageEffect::create()
{
    return makeObject<BloomImageEffect>();
}

BloomImageEffect::BloomImageEffect()
	: m_viewWidth(0)
	, m_viewHeight(0)
{
}

void BloomImageEffect::init()
{
    ImageEffect::init();

    auto shader = makeObject<Shader>(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/LuminosityHighPassShader.fx");
	m_materialHighPassFilter = makeObject<Material>();
	m_materialHighPassFilter->setShader(shader);

	auto separableBlurShader = makeObject<Shader>(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/SeperableBlur.fx");
	for (int i = 0; i < MIPS; i++) {
		auto material = makeObject<Material>();
		material->setShader(shader);
		m_separableBlurMaterials.add(material);
	}

	auto bloomImageEffectShader = makeObject<Shader>(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/BloomImageEffect.fx");
	m_compositeMaterial = makeObject<Material>();
	m_compositeMaterial->setShader(bloomImageEffectShader);
}

void BloomImageEffect::onUpdateFrame(float elapsedSeconds)
{
}

void BloomImageEffect::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
	if (m_viewWidth != source->width() || m_viewHeight != source->height()) {
		resetResources(source->width(), source->height());
	}


	// Extract Bright Areas
	{
		m_materialHighPassFilter->setVector(u"defaultColor", Vector4(0.0f, 0.0f, 0.0f, 0.0f));
		m_materialHighPassFilter->setFloat(u"defaultOpacity", 0.0f);
		m_materialHighPassFilter->setFloat(u"luminosityThreshold", 0.8f);
		m_materialHighPassFilter->setFloat(u"smoothWidth", 0.01);

		m_materialHighPassFilter->setMainTexture(source);
		////m_material->setMainTexture(Texture2D::whiteTexture());
	 //   m_material->setVector(u"_Tone", m_toneValue.value());
		context->blit(m_materialHighPassFilter, destination);
	}



}

void BloomImageEffect::resetResources(int resx, int resy)
{

	for (int i = 0; i < MIPS; i++) {

	}

	m_viewWidth = resx;
	m_viewHeight = resy;
}

} // namespace ln

