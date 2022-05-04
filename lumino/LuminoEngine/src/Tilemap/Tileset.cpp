#include "Internal.hpp"
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoGraphics/Rendering/FeatureRenderer/SpriteRenderer.hpp>
#include <LuminoEngine/Tilemap/Tileset.hpp>
#include "../../Graphics/src/Rendering/RenderFeature/SpriteRenderFeature.hpp"

namespace ln {
namespace detail {

// LocalAutoTileId => SubTileIds
AutoTileInfo g_AutoTileTable[Tileset::AutoTileUnitStride] =
{
	// Block tiles.
	/*[0]*/  {1,1,1,1},{2,1,1,1},{3,3,1,1},{1,2,1,1}, {2,2,1,1},{4,1,4,1},{5,3,4,1},{4,2,4,1},
	/*[8]*/  {1,4,1,4},{2,4,1,4},{3,5,1,4},{4,4,4,4}, {5,5,4,4},{1,1,2,1},{2,1,2,1},{3,3,2,1},
	/*[16]*/ {1,2,2,1},{2,2,2,1},{4,3,4,1},{1,4,2,4}, {2,4,2,4},{3,5,2,4},{1,1,3,3},{2,1,3,3},
	/*[24]*/ {3,3,3,3},{1,2,3,3},{2,2,3,3},{4,1,5,3}, {5,3,5,3},{4,2,5,3},{1,4,3,5},{2,4,3,5},
	/*[32]*/ {3,5,3,5},{4,4,5,5},{5,5,5,5},{1,1,1,2}, {2,1,1,2},{3,3,1,2},{1,2,1,2},{2,2,1,2},
	/*[40]*/ {4,1,4,2},{5,3,4,2},{4,2,4,2},{1,1,2,2}, {2,1,2,2},{3,3,2,2},{1,2,2,2},{2,2,2,2},

	// Slope corner tiles.
	/*[48]*/ {6,1,1,1},{1,6,1,1},{4,6,4,1},{6,4,1,4}, {1,1,6,1},{3,3,6,1},{1,6,6,1},{1,1,2,6},
	/*[56]*/ {1,2,6,1},{1,4,6,4},{3,5,6,4},{6,1,3,3}, {1,6,3,3},{4,6,5,3},{6,4,3,5},{1,1,1,6},
	/*[64]*/ {6,1,1,6},{6,1,1,2},{2,1,1,6},{3,3,1,6}, {4,1,4,6},{5,3,4,6},{0,0,0,0},{0,0,0,0},

	// Half-Slope corner tiles.
	/*[72]*/ {7,8,1,1},{9,1,10,1},{8,7,1,1},{1,9,1,10}, {9,4,10,4},{9,4,10,4},{1,1,7,8},{10,1,9,1},
	/*[80]*/ {3,3,8,7},{8,7,2,1},{1,9,2,10},{1,2,7,8},  {10,2,9,1},{10,4,9,4},{7,8,3,3},{8,7,3,3},
	/*[88]*/ {1,1,8,7},{1,10,1,9},{7,8,1,2},{9,1,10,2}, {2,1,8,7},{2,10,1,9},{3,3,8,7},{4,10,4,9},
	///*[72]*/ {7,1,1,1},{8,1,1,1},{1,7,1,1},{1,8,1,1}, {4,8,4,1},{8,4,1,4},{1,1,7,1},{1,1,8,1},
	///*[80]*/ {3,3,7,1},{1,7,2,1},{1,8,2,1},{1,2,7,1}, {1,2,8,1},{1,4,8,4},{7,1,3,3},{1,7,3,3},
	///*[88]*/ {1,1,1,7},{1,1,1,8},{7,1,1,2},{8,1,1,2}, {2,1,1,7},{2,1,1,8},{3,3,1,7},{4,1,4,8},

	// Slope tiles.														15~18 not used. (reserved) ※Slope の内側にBlockTileの角があるパターン
	/*[96]*/ {11,11,11,11},{12,12,12,12},{13,13,13,13},{14,14,14,14}, {15,15,15,15},{16,16,16,16},{17,17,17,17},{18,18,18,18},

	// Half-Slope tiles.
	/*[104]*/{19,19,19,19},{20,20,20,20}, {21,21,21,21},{22,22,22,22}, {23,23,23,23},{24,24,24,24}, {25,25,25,25},{26,26,26,26},
	/*[112]*/{27,27,27,27},{28,28,28,28}, {29,29,29,29},{30,30,30,30}, {31,31,31,31},{32,32,32,32}, {33,33,33,33},{34,34,34,34},
	///*[104]*/{14,14,14,14},{15,15,15,15}, {16,16,16,16},{17,17,17,17}, {18,18,18,18},{19,19,19,19}, {20,20,20,20},{21,21,21,21},
	///*[112]*/{22,22,22,22},{23,23,23,23}, {24,24,24,24},{25,25,25,25}, {26,26,26,26},{27,27,27,27}, {28,28,28,28},{29,29,29,29},

	// Reserved.
	/*[120]*/{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}, {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
};

// SubTileId([0] is invalid) => PhysicalTileIndex
float g_AutoTileSourcePosTable_StandardFormat[4][35] =
{
	// [top-left]                    block [10]  <-|-> [11] slope                  <-|-> half-slope
	{ -1, 25, 2, 17, 24, 0, 44, 12, 14, 49, 53,  /*|*/ -1, 37, 46, 39, 0, 0, 0, 0, /*|*/ 4, -1, 20, 28, 6, -1, 22, 30, 48, -1, 50, 51, 52, -1, 54, 55, },

	// [top-right]
	{ -1, 26, 3, 18, 27, 1, 45, 15, 13, 50, 54,  /*|*/ 36, -1, 38, 47, 0, 0, 0, 0, /*|*/ 5, -1, 21, 29, 7, -1, 23, 31, 49, 48, 51, -1, 53, 52, 55, -1, },

	// [bottom-left]
	{ -1, 33, 10, 41, 32, 8, 46, 20, 22, 61,57,  /*|*/ 36, 45, -1, 47, 0, 0, 0, 0, /*|*/ 12, 4, 28, -1, 14, 6, 30, -1, 56, -1, 58, 59, 60, -1, 62, 63, },

	// [bottom-light]
	{ -1, 34, 11, 42, 35, 9, 47, 23, 21, 62, 58, /*|*/ 44, 37, 46, -1, 0, 0, 0, 0, /*|*/ 13, 5, 29, -1, 15, 7, 31, -1, 57, 56, 59, -1, 61, 60, 63, -1, },
};

} // namespace detail

//==============================================================================
// AutoTileset

AutoTileset::AutoTileset()
{
}

void AutoTileset::init()
{
	Object::init();
}

Rect AutoTileset::getSourceRectUV(int autotileId, int component) const
{
	if (LN_REQUIRE(autotileId < Tileset::AutoTileUnitStride)) return Rect();

	uint8_t subtile = detail::g_AutoTileTable[autotileId].subtiles[component];
	if (LN_REQUIRE(subtile > 0)) return Rect();

	float ti = detail::g_AutoTileSourcePosTable_StandardFormat[component][subtile];
	if (ti < 0.0f) return Rect::Empty;

	float tx = std::fmod(ti, 8.0f);
	float ty = std::floor(ti / 8.0f);

	// TODO: とりあえず 8x8px 固定
	float sz = 8.0f;
	return Rect(tx * sz, ty * sz, sz, sz);
}

//==============================================================================
// Tileset

LN_OBJECT_IMPLEMENT(Tileset, Object) {}

Ref<Tileset> Tileset::create()
{
    return makeObject<Tileset>();
}

Ref<Tileset> Tileset::create(Texture* texture, int tilePixelWidth, int tilePixelHeight)
{
	auto material = makeObject<Material>();
	material->setMainTexture(texture);
	auto tileset = makeObject<Tileset>();
	tileset->reset(material, tilePixelWidth, tilePixelHeight);
	return tileset;
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

uint8_t Tileset::tilePassageFlags(int tileId) const
{
	if (tileId < AutoTileOffset) {
		return m_tiles[tileId].passageFlags;
	}
	else {
		// TODO:
		return 0x00;
	}
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

void Tileset::addAutoTileset(AutoTileset* autoTileset)
{
	if (LN_REQUIRE(m_autoTilesets.size() < MaxAutoTilests)) return;
	m_autoTilesets.add(autoTileset);
}

void Tileset::drawTile(RenderingContext* context, int tileId, const Vector3& pos, const Size& tileSize)
{
    if (!m_material) return;

	int autoTilesetIndex = autoTileIndex(tileId);
	if (autoTilesetIndex >= 0) {
		int localTileId = autoTileLocalId(tileId);
		const auto& autotile = detail::g_AutoTileTable[localTileId];
		const auto& autotilest = m_autoTilesets[autoTilesetIndex];

		Size hsz(tileSize.width / 2, tileSize.height / 2);
		//float w1 = m_tilePixelWidth / 2;
		//float h1 = m_tilePixelHeight / 2;
		Vector3 points[] = {
			{pos.x, pos.y + hsz.height, 0},
			{pos.x + hsz.width, pos.y + hsz.height, 0},
			{pos.x, pos.y, 0},
			{pos.x + hsz.width, pos.y, 0},
		};
		for (int i = 0; i < 4; i++) {
			Rect sourceRect = autotilest->getSourceRectUV(localTileId, i);
			if (!sourceRect.isEmpty()) {
				Texture* texture = autotilest->material->mainTexture();

				Size renderSize;
				Rect renderSourceRect;
				detail::SpriteRenderFeature2::makeRenderSizeAndSourceRectHelper(
					texture, tileSize, sourceRect, &renderSize, &renderSourceRect);

				// TODO: 最適化の余地あり。ここで begin end しない。
				auto r = SpriteRenderer::get();
				r->begin(context, autotilest->material);
				r->drawSprite(
					Matrix::makeTranslation(points[i]), hsz, Vector2::Zero,
					renderSourceRect, Color::White,
					SpriteBaseDirection::ZMinus, BillboardType::None, SpriteFlipFlags::None);
				r->end();
			}
		}

	}
	else {
		if (tileId <= 0 || m_tiles.size() <= tileId) return;

		Texture* texture = m_material->mainTexture();
		if (!texture) return;

		//float sx = m_tileUVSize.width * (tileId % m_horizontalTileCount);
		//float sy = m_tileUVSize.height * (tileId / m_horizontalTileCount);

		const auto& tile = m_tiles[tileId];

		//Size worldSize(tile.sourceRect.width * m_tileScale.x, tile.sourceRect.height * m_tileScale.y);
		Size renderSize;
		Rect renderSourceRect;
		detail::SpriteRenderFeature2::makeRenderSizeAndSourceRectHelper(
			texture, tileSize, tile.sourceRect, &renderSize, &renderSourceRect);

#ifdef LN_COORD_RH
		const SpriteBaseDirection frontDir = SpriteBaseDirection::ZPlus;
#else
		const SpriteBaseDirection frontDir = SpriteBaseDirection::ZMinus;
#endif
        auto r = SpriteRenderer::get();
        r->begin(context, m_material);
		r->drawSprite(
			Matrix::makeTranslation(pos), tileSize, Vector2::Zero,
			renderSourceRect, Color::White,
			frontDir, BillboardType::None, SpriteFlipFlags::None);
        r->end();
	}
}

//void Tileset::serialize(Archive& ar)
//{
//    Object::serialize(ar);
//    ar & makeNVP(_TT("tilePixelWidth", m_tilePixelWidth);
//    ar & makeNVP(_TT("tilePixelHeight", m_tilePixelHeight);
//    ar & makeNVP(_TT("tileScale", m_tileScale);
//    ar & makeNVP(_TT("material", m_material);
//    ar & makeNVP(_TT("tiles", m_tiles);
//}
//
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
	LN_NOTIMPLEMENTED();
	//ar & makeNVP(_TT("passageFlags"), passageFlags);
 //   ar & makeNVP(_TT("sourceRect"), sourceRect);
}

} // namespace ln

