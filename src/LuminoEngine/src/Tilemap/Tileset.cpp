
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

void Tileset::initialize()
{
    Object::initialize();

    m_material = Material::create();
    m_material->setMainTexture(newObject<Texture2D>(u"D:/tmp/110220c_as019.png"));
}

void Tileset::setTilePixelSize(int width, int height)
{
    m_tilePixelWidth = width;
    m_tilePixelHeight = height;
}

void Tileset::setMaterial(Material* material)
{
    m_material = material;
}

void Tileset::drawTile(RenderingContext* context, int tileId, const Vector3& pos, const Size& tileSize)
{
    if (tileId == 1)    // TODO:
    {
        context->drawSprite(
            Matrix::makeTranslation(pos), tileSize, Vector2::Zero,
            Rect(0, 0, 1, 1), Color::White,
            SpriteBaseDirection::ZMinus, BillboardType::None,
            m_material);
    }
}

} // namespace ln

