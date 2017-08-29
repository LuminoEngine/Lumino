
#include "../Internal.h"
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/Text/GlyphIconFontManager.h"
//#include <Lumino/Graphics/Brush.h>
//#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/Rendering/RenderingContext.h>
//#include "SceneGraphManager.h"
//#include <Lumino/World.h>
//#include <Lumino/Scene/SceneGraph.h>
//#include <Lumino/Scene/TextBlock.h>
//#include "../Documents/DocumentElements.h"
#include <Lumino/World.h>
#include <Lumino/Scene/GlyphIcon.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// GlyphIcon2D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(GlyphIcon2D, VisualObject);

Ref<GlyphIcon2D> GlyphIcon2D::create()
{
	return newObject<GlyphIcon2D>();
}

Ref<GlyphIcon2D> GlyphIcon2D::create(const StringRef& glyphName)
{
	return newObject<GlyphIcon2D>(glyphName);
}

Ref<GlyphIcon2D> GlyphIcon2D::create(const StringRef& glyphName, int glyphSize)
{
	return newObject<GlyphIcon2D>(glyphName, glyphSize);
}

GlyphIcon2D::GlyphIcon2D()
	: m_component(nullptr)
{
}

GlyphIcon2D::~GlyphIcon2D()
{
}

bool GlyphIcon2D::initialize()
{
	LN_BASE_INITIALIZE(VisualObject);
	m_component = newObject<GlyphIconComponent>();
	m_component->setLayer(LayerMask::GetLayer(BuiltinLayers::Default2D));
	addComponent(m_component);
	detail::EngineDomain::getDefaultWorld2D()->addWorldObject(this, true);
	return true;
}

bool GlyphIcon2D::initialize(const StringRef& glyphName)
{
	if (!GlyphIcon2D::initialize()) return false;
	setGlyphName(glyphName);
	return true;
}

bool GlyphIcon2D::initialize(const StringRef& glyphName, int glyphSize)
{
	if (!GlyphIcon2D::initialize()) return false;
	setGlyphName(glyphName);
	setGlyphSize(glyphSize);
	return true;
}

void GlyphIcon2D::setGlyphName(const StringRef& glyphName)
{
	m_component->setGlyphName(glyphName);
}

void GlyphIcon2D::setGlyphSize(int glyphSize)
{
	m_component->setGlyphSize(glyphSize);
}

VisualComponent* GlyphIcon2D::getMainVisualComponent() const
{
	return m_component;
}

//==============================================================================
// GlyphIconComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(GlyphIconComponent, VisualComponent);

GlyphIconComponent::GlyphIconComponent()
{
}

GlyphIconComponent::~GlyphIconComponent()
{
}

bool GlyphIconComponent::initialize()
{
	LN_BASE_INITIALIZE(VisualComponent);

	// TODO: Node ‘¤‚Å§Œä‚·‚é‚×‚«H
	setBlendMode(BlendMode::Alpha);
	return true;
}

void GlyphIconComponent::setGlyphName(const StringRef& glyphName)
{
	auto& manager = detail::EngineDomain::getGraphicsManager()->getGlyphIconFontManager();
	m_codePoint = manager->getFontAwesomeCodePoint(glyphName);
}

void GlyphIconComponent::setGlyphSize(int glyphSize)
{
	auto& manager = detail::EngineDomain::getGraphicsManager()->getGlyphIconFontManager();
	m_font = manager->getGlyphIconFont(glyphSize);
}

void GlyphIconComponent::setAnchorPoint(const Vector2& ratio)
{
	m_anchor = ratio;
}

void GlyphIconComponent::onRender2(RenderingContext* context)
{
	VisualComponent::onRender2(context);


	//if (m_renderSize.width < 0)
	//{
	//	auto font = (m_font != nullptr) ? m_font : Font::getDefault();
	//	m_renderSize = font->measureRenderSize(m_text);
	//}

	context->setFont(m_font);

	//context->drawText_(m_text, Point(-m_renderSize.width * m_anchor.x, -m_renderSize.height * m_anchor.y));


	//context->drawChar(/*U'\uf2c5'*/0xf2c5, Rect(0, 0, 100, 100), StringFormatFlags::LeftAlignment);
	context->drawChar(m_codePoint, Rect(0, 0, 100, 100), StringFormatFlags::LeftAlignment);
}

LN_NAMESPACE_END
