
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Tilemap/TilemapComponent.hpp>

namespace ln {

//==============================================================================
// TilemapComponent

TilemapComponent::TilemapComponent()
{
}

TilemapComponent::~TilemapComponent()
{
}

void TilemapComponent::initialize()
{
    VisualComponent::initialize();

    m_material = Material::create();
    m_material->setMainTexture(newObject<Texture2D>(u"D:/tmp/110220c_as019.png"));
}

void TilemapComponent::onRender(RenderingContext* context)
{
    context->drawSprite(Matrix::Identity, Size(5, 5), Vector2::Zero, Rect(0, 0, 1, 1), Color::White, SpriteBaseDirection::ZMinus, BillboardType::None, m_material);
}

} // namespace ln

