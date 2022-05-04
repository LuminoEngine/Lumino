
#include "Internal.hpp"
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Rendering/FeatureRenderer/SpriteRenderer.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UISprite.hpp>
#include "../../Graphics/src/Rendering/RenderFeature/SpriteRenderFeature.hpp"

namespace ln {

//==============================================================================
// UISprite

Ref<UISprite> UISprite::create()
{
    return makeObject<UISprite>();
}

Ref<UISprite> UISprite::create(Texture* texture)
{
    return makeObject<UISprite>(texture);
}

Ref<UISprite> UISprite::load(StringView filePath)
{
	return makeObject<UISprite>(Texture2D::load(filePath));
}

UISprite::UISprite()
{
	m_objectManagementFlags.set(detail::ObjectManagementFlags::AutoAddToPrimaryElement);
}

void UISprite::init()
{
    UIElement::init();

	setAlignments(UIHAlignment::Center, UIVAlignment::Center);
    m_material = makeObject<Material>();
    m_size = Size(-1, -1);
    m_sourceRect = Rect(0, 0, -1, -1);
    setBlendMode(BlendMode::Alpha);
}

void UISprite::init(Texture* texture)
{
    init();
    setTexture(texture);
}

void UISprite::setTexture(Texture* texture)
{
    m_material->setMainTexture(texture);
}

void UISprite::setSourceRect(const Rect & rect)
{
    m_sourceRect = rect;
}

void UISprite::setSourceRect(float x, float y, float width, float height)
{
    m_sourceRect = Rect(x, y, width, height);
}

const Rect & UISprite::sourceRect() const
{
    return m_sourceRect;
}

void UISprite::setShader(Shader* shader)
{
    m_material->setShader(shader);
}

Size UISprite::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
	Texture* texture = m_material->mainTexture();

	Size size = m_sourceRect.getSize();
	if (size.width < 0.0f) {
		size.width = (texture) ? texture->width() : 0.0f;
	}
	if (size.height < 0.0f) {
		size.height = (texture) ? texture->height() : 0.0f;
	}

	return Size::max(
		UIElement::measureOverride(layoutContext, constraint),
		size);
}

void UISprite::onRender(UIRenderingContext* context)
{
    Size renderSize;
    Rect renderSourceRect;
    detail::SpriteRenderFeature2::makeRenderSizeAndSourceRectHelper(
        m_material->mainTexture(), m_size, m_sourceRect, &renderSize, &renderSourceRect);

    auto r = SpriteRenderer::get();
    r->begin(context, m_material);
    r->drawSprite(
        Matrix(), renderSize, Vector2(0, 0), renderSourceRect, Color::White,
        SpriteBaseDirection::Basic2D, BillboardType::None, SpriteFlipFlags::None);
    r->end();
}

} // namespace ln

