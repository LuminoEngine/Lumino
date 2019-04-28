
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Tilemap/Tileset.hpp>

namespace ln {

//==============================================================================
// Tileset

Ref<Tileset> Tileset::create()
{
    return newObject<Tileset>();
}

Tileset::Tileset()
    : m_tilePixelWidth(32)
    , m_tilePixelHeight(32)
{
}

Tileset::~Tileset()
{
}

void Tileset::init()
{
    Object::init();

    m_material = Material::create();
    m_material->setMainTexture(newObject<Texture2D>(u"D:/tmp/110220c_as019.png"));
}

void Tileset::setTilePixelSize(int width, int height)
{
    m_tilePixelWidth = width;
    m_tilePixelHeight = height;
    resetInfo();
}

void Tileset::setMaterial(Material* material)
{
    m_material = material;
    resetInfo();
}

void Tileset::drawTile(RenderingContext* context, int tileId, const Vector3& pos, const Size& tileSize)
{
    if (!m_material) return;

    Texture* texture = m_material->mainTexture();
    if (!texture) return;

    float sx = m_tileUVSize.width * (tileId % m_horizontalTileCount);
    float sy = m_tileUVSize.height * (tileId / m_horizontalTileCount);

    context->drawSprite(
        Matrix::makeTranslation(pos), tileSize, Vector2::Zero,
        Rect(sx, sy, m_tileUVSize), Color::White,
        SpriteBaseDirection::ZMinus, BillboardType::None, detail::SpriteFlipFlags::None,
        m_material);
}

void Tileset::resetInfo()
{
    if (!m_material) return;
    if (!m_material->mainTexture()) return;

    int m_horizontalTileCount = m_material->mainTexture()->width() / m_tilePixelWidth;

    m_tileUVSize.width = static_cast<float>(m_tilePixelWidth) / m_material->mainTexture()->width();
    m_tileUVSize.height = static_cast<float>(m_tilePixelHeight) / m_material->mainTexture()->height();
}

} // namespace ln

