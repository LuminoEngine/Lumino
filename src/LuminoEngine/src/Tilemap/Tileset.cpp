
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Tilemap/Tileset.hpp>
#include "../Rendering/SpriteRenderFeature.hpp"

namespace ln {

//==============================================================================
// Tileset

LN_OBJECT_IMPLEMENT(Tileset, Object)
{
    context->registerType<Tileset>({});
}

Ref<Tileset> Tileset::create()
{
    return makeObject<Tileset>();
}

Tileset::Tileset()
    : m_tilePixelWidth(32)
    , m_tilePixelHeight(32)
    //: m_tileScale(1.0f)
{
}

Tileset::~Tileset()
{
}

void Tileset::init()
{
    Object::init();

    //m_material = Material::create();
    //m_material->setMainTexture(makeObject<Texture2D>(u"D:/tmp/110220c_as019.png"));
}

void Tileset::reset(Material* material, int tilePixelWidth, int tilePixelHeight)
{
    setMaterial(material);
    setTilePixelSize(tilePixelWidth, tilePixelHeight);
    int hc = material->mainTexture()->width() / tilePixelWidth;
    int vc = material->mainTexture()->height() / tilePixelHeight;
    resize(hc * vc);
    for (int v = 0; v < vc; v++) {
        for (int h = 0; h < hc; h++) {
            setTileImageRect(v * hc + h, h * tilePixelWidth, v * tilePixelHeight, tilePixelWidth, tilePixelHeight);
        }
    }
}

void Tileset::resize(int tileCount)
{
    m_tiles.resize(tileCount);
}

void Tileset::setTilePixelSize(int width, int height)
{
    m_tilePixelWidth = width;
    m_tilePixelHeight = height;
    //resetInfo();
    m_tileScale.x = 1.0f / width;
    m_tileScale.y = 1.0f / height;
}

void Tileset::setTileImageRect(int tileId, int x, int y, int width, int height)
{
    m_tiles[tileId].sourceRect.set(x, y, width, height);
}

Material* Tileset::material() const
{
    return m_material;
}

void Tileset::setMaterial(Material* material)
{
    m_material = material;
    resetInfo();
}

void Tileset::drawTile(RenderingContext* context, int tileId, const Vector3& pos, const Size& tileSize)
{
    if (!m_material) return;
    if (tileId <= 0 || m_tiles.size() <= tileId) return;

    Texture* texture = m_material->mainTexture();
    if (!texture) return;

    //float sx = m_tileUVSize.width * (tileId % m_horizontalTileCount);
    //float sy = m_tileUVSize.height * (tileId / m_horizontalTileCount);
    
    const auto& tile = m_tiles[tileId];

    //Size worldSize(tile.sourceRect.width * m_tileScale.x, tile.sourceRect.height * m_tileScale.y);
    Size renderSize;
    Rect renderSourceRect;
    detail::SpriteRenderFeature::makeRenderSizeAndSourceRectHelper(
        texture, tileSize, tile.sourceRect, &renderSize, &renderSourceRect);

    context->drawSprite(
        Matrix::makeTranslation(pos), tileSize, Vector2::Zero,
        renderSourceRect, Color::White,
        SpriteBaseDirection::ZMinus, BillboardType::None, detail::SpriteFlipFlags::None,
        m_material);
}

void Tileset::serialize(Archive& ar)
{
    Object::serialize(ar);
    ar & makeNVP(u"tilePixelWidth", m_tilePixelWidth);
    ar & makeNVP(u"tilePixelHeight", m_tilePixelHeight);
    ar & makeNVP(u"tileScale", m_tileScale);
    ar & makeNVP(u"material", m_material);
    ar & makeNVP(u"tiles", m_tiles);
}

void Tileset::resetInfo()
{
    //if (!m_material) return;
    //if (!m_material->mainTexture()) return;

    //int m_horizontalTileCount = m_material->mainTexture()->width() / m_tilePixelWidth;

    //m_tileUVSize.width = static_cast<float>(m_tilePixelWidth) / m_material->mainTexture()->width();
    //m_tileUVSize.height = static_cast<float>(m_tilePixelHeight) / m_material->mainTexture()->height();
}

void Tileset::Tile::serialize(Archive& ar)
{
    ar & makeNVP(u"sourceRect", sourceRect);
}

} // namespace ln

