
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Visual/SpriteComponent.hpp>
#include "../Rendering/SpriteRenderFeature.hpp"

namespace ln {

//=============================================================================
// SpriteFrame

SpriteFrame::SpriteFrame()
	: m_sourceRect()
	, m_anchorPoint()
{
}

void SpriteFrame::init()
{
	Object::init();
	m_anchorPoint = Vector2(Math::NaN, Math::NaN);
}

//=============================================================================
// SpriteFrameSet

Ref<SpriteFrameSet> SpriteFrameSet::create(Texture* texture, int frameWidth, int frameHeight, const Vector2& anchorPoint)
{
	return newObject<SpriteFrameSet>(texture, frameWidth, frameHeight, anchorPoint);
}

SpriteFrameSet::SpriteFrameSet()
{
}

void SpriteFrameSet::init()
{
	Object::init();
}

void SpriteFrameSet::init(Texture* texture, int frameWidth, int frameHeight, const Vector2& anchorPoint)
{
	init();

	if (LN_REQUIRE(texture)) return;
	if (LN_REQUIRE(frameWidth > 0)) return;
	if (LN_REQUIRE(frameHeight > 0)) return;
	m_texture = texture;
	m_frames = makeList<Ref<SpriteFrame>>();

	int cols = m_texture->width() / frameWidth;
	int rows = m_texture->height() / frameHeight;

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			// TODO: モノによっては大量の小オブジェクトができるので、できればまとめて alloc したりキャッシュしたい
			auto frame = newObject<SpriteFrame>();
			frame->setSourceRect(Rect(x * frameWidth, y * frameHeight, frameWidth, frameHeight));
			frame->setAnchorPoint(anchorPoint);
            m_frames->add(frame);
		}
	}
}

Texture* SpriteFrameSet::texture() const
{
    return m_texture;
}

SpriteFrame* SpriteFrameSet::frame(int index) const
{
	if (0 <= index && index < m_frames->size()) {
		return m_frames[index];
	}
	else {
		return nullptr;
	}
}

//=============================================================================
// SpriteComponent
/*
 * 正面方向について
 * --------
 * Luminoは 奥方向を正面としている。なら Sprite も面方向と法線はデフォルトで Z+ を向いた方がよいのでは？
 * → Sprite は普通、常にカメラ方向を向くビルボードとして使われることを想定している。
 *   まぁ、3D 空間に看板オブジェクトみたいに板ポリ置くのに使えないことはないけれど、
 *   それってマップオブジェクトを表現するための Mesh としたほうがいいよね。
 */

SpriteComponent::SpriteComponent()
    : m_material(nullptr)
    , m_size()
    , m_anchorPoint(0.5, 0.5)
	, m_frameIndex(-1)
{
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::init()
{
    VisualComponent::init();
    m_sourceRect.set(0, 0, -1, -1);
    setSize(Size(1, 1));

    m_material = newObject<Material>();
    //m_material->setEmissive(Color(1,1,1,0.5));
    setBlendMode(BlendMode::Alpha);
}

void SpriteComponent::setTexture(Texture* texture)
{
    m_material->setMainTexture(texture);
}

Texture* SpriteComponent::texture() const
{
    return m_material->mainTexture();
}

void SpriteComponent::setSize(const Size& size)
{
    m_size = size;
}

void SpriteComponent::setSourceRect(const Rect& rect)
{
    m_sourceRect = rect;
}

void SpriteComponent::setFrameSet(SpriteFrameSet* value)
{
	m_frameSet = value;
    m_material->setMainTexture(m_frameSet->texture());
}

void SpriteComponent::setFrameIndex(int index)
{
	m_frameIndex = index;
}

void SpriteComponent::onRender(RenderingContext* context)
{
    Vector2 anchorPoint = m_anchorPoint;
	Rect sourceRect = m_sourceRect;
    Texture* tex = texture();

	if (sourceRect.isEmpty() && m_frameSet) {
		if (SpriteFrame* frame = m_frameSet->frame(m_frameIndex)) {
			sourceRect = frame->sourceRect();
            anchorPoint = frame->anchorPoint();
            tex = m_frameSet->texture();
		}
	}

    Size renderSize;
    Rect renderSourceRect;
    detail::SpriteRenderFeature::makeRenderSizeAndSourceRectHelper(
        tex, m_size, sourceRect, &renderSize, &renderSourceRect);

    context->drawSprite(
        Matrix(), renderSize, anchorPoint, renderSourceRect, Color::White,
        SpriteBaseDirection::ZMinus, BillboardType::None, m_material);
}

} // namespace ln

