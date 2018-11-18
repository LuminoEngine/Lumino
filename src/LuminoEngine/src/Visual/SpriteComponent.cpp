
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Visual/SpriteComponent.hpp>

namespace ln {

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
{
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::initialize()
{
    VisualComponent::initialize();
    m_srcRect.set(0, 0, -1, -1);
    setSize(Size(1, 1));

    m_material = newObject<Material>();
    //m_material->setEmissive(Color(1,1,1,0.5));
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
    m_srcRect = rect;
}

void SpriteComponent::onRender(RenderingContext* context)
{
    // 転送元矩形が負値ならテクスチャ全体を転送する
    Texture* tex = texture();
    const SizeI& texSize = (tex != nullptr) ? tex->size() : SizeI::Zero;
    Rect renderSourceRect = m_srcRect;
    if (renderSourceRect.width < 0 && renderSourceRect.height < 0)
    {
        renderSourceRect.width = texSize.width;
        renderSourceRect.height = texSize.height;
    }
    Size renderSize = m_size;
    if (renderSize.width < 0 && renderSize.height < 0)
    {
        renderSize.width = renderSourceRect.width;
        renderSize.height = renderSourceRect.height;
    }

    renderSourceRect.x /= texSize.width;
    renderSourceRect.width /= texSize.width;
    renderSourceRect.y /= texSize.height;
    renderSourceRect.height /= texSize.height;


    //context->setBlendMode(BlendMode::Alpha);
    //context->setOpacity(0.5);

    context->drawSprite(
        Matrix(), renderSize, Vector2(0, 0), renderSourceRect, Color::White,
        SpriteBaseDirection::ZMinus, BillboardType::None, m_material);
}

} // namespace ln

