
#include "Internal.hpp"
#include <LuminoEngine/Tilemap/TilemapLayer.hpp>
#include <LuminoEngine/Tilemap/TilemapModel.hpp>
#include <LuminoEngine/Tilemap/Tileset.hpp>

/*

---  -*-  --*  ---  ---  ---  ---  ---
-@-  -@-  -@-  *@-  -@*  -@-  -@-  -@-
---  ---  ---  ---  ---  *--  -*-  --*

*--  **-  *-*  *--  *--  *--  *--  *--
-@-  -@-  -@-  *@-  -@*  -@-  -@-  -@-
---  ---  ---  ---  ---  *--  -*-  --*


-*-  **-  ***  **-  **-  **-  **-  **-
-@-  -@-  -@-  *@-  -@*  -@-  -@-  -@-
---  ---  ---  ---  ---  *--  -*-  --*

---  *--  *-*  *--  *--  *--  *--  *--
-@-  -@-  -@-  *@-  -@*  -@-  -@-  -@-
---  ---  ---  ---  ---  *--  -*-  --*



***  ***  ***  ***  ***  ***
-@-  *@-  -@*  -@-  -@-  -@-
---  ---  ---  *--  -*-  --*

***  ***  ***  ***  ***
*@-  *@*  *@-  *@-  *@-
---  ---  *--  -*-  --*

***  ***  ***  ***
*@*  *@*  *@*  *@*
---  *--  -*-  --*

***  ***  ***
*@*  *@*  *@*
*--  **-  *-*

***  ***
*@*  *@*
**-  ***

*/

namespace ln {

////==============================================================================
//// AbstractTilemapLayer
//
//LN_OBJECT_IMPLEMENT(AbstractTilemapLayer, Object)
//{
//    context->registerType<AbstractTilemapLayer>({});
//}
//
//AbstractTilemapLayer::AbstractTilemapLayer()
//    : m_tileSize(1, 1)
//    , m_orientation(TilemapOrientation::UpFlow)
//{
//}
//
//AbstractTilemapLayer::~AbstractTilemapLayer()
//{
//}
//
//void AbstractTilemapLayer::init()
//{
//    Object::init();
//}
//
////void AbstractTilemapLayer::resize(int width, int height)
////{
////    m_size.set(width, height);
////    m_data.resize(width * height);
////}
////
////void AbstractTilemapLayer::setTileId(int x, int y, int id)
////{
////    m_data[y * m_size.width + x] = id;
////}
////
////int AbstractTilemapLayer::getTileId(int x, int y) const
////{
////    // TODO: round
////    // clamp
////    if (x < 0 || m_size.width <= x) return 0;
////    if (y < 0 || m_size.height <= y) return 0;
////
////    return m_data[y * m_size.width + x];
////}
//
//void AbstractTilemapLayer::setTileSize(const Size& size)
//{
//    m_tileSize = size;
//}
//
//// bounds: Y+ を上方向とした、ローカル空間上の描画範囲
//void AbstractTilemapLayer::render(TilemapModel* model, RenderingContext* context, const Matrix& transform, const detail::TilemapBounds& bounds)
//{
//    
//    //int l, t, r, b;         // 2D array としてどの範囲を描画するか
//    float wL, wT, wR, wB;   // world としてどの範囲を描画するか
//    //Vector3 offset;
//    int l, t, r, b;
//    {
//        wL = l = static_cast<int>(bounds.l / m_tileSize.width);
//        wT = t = static_cast<int>(bounds.t / m_tileSize.height);
//        wR = r = static_cast<int>(bounds.r / m_tileSize.width);
//        wB = b = static_cast<int>(bounds.b / m_tileSize.height);
//        //wL *= m_tileSize.width;
//        //wT *= m_tileSize.height;
//        //wR *= m_tileSize.width;
//        //wB *= m_tileSize.height;
//        //t = -t;
//        //b = -b;
//
//        //if (m_orientation == TilemapOrientation::UpFlow)
//        //{
//        //    offset.y = m_tileSize.height * m_size.height;
//        //    // Y=0 から↑へ向かって配置する。(0, 0) = [0, m_size.height-1]
//        //    //t = m_size.height - t;
//        //    //b = m_size.height - b;
//        //}
//        //else if (m_orientation == TilemapOrientation::DownFlow)
//        //{
//        //    // Y=0 から↓へ向かって配置する。(0, 0) = [0, 0]
//        //    //t = -t;
//        //    //b = -b;
//        //}
//        //else
//        //{
//        //    LN_UNREACHABLE();
//        //}
//
//        r += 1;
//        b -= 1;
//    }
//
//    int width = getWidth();
//    int height = getHeight();
//
//    //float tw = m_tileSize.width;
//    //float th = m_tileSize.height;
//    for (int y = t; y >= b; y--)
//    {
//        // clamp
//        //if (y < 0 || m_size.height <= y) continue;
//
//        for (int x = l; x < r; x++)
//        {
//            // clamp
//            //if (x < 0 || m_size.width <= x) continue;
//            int tileId = 0;
//
//            if (m_orientation == TilemapOrientation::UpFlow)
//            {
//                tileId = getTileId(x, height - y - 1);
//            }
//            else if (m_orientation == TilemapOrientation::DownFlow) {
//                tileId = getTileId(x, -y);
//            }
//            else {
//                LN_UNREACHABLE();
//            }
//
//
//
//            if (tileId > 0)
//            {
//                Vector3 pos(x * m_tileSize.width, y * m_tileSize.height, 0);
//                Tileset* tileset;
//                int localId;
//                if (model->fetchTileset(tileId, &tileset, &localId)) {
//                    tileset->drawTile(context, localId, pos, m_tileSize);
//                }
//            }
//        }
//    }
//
//
//    //for (int y = t; y < renderRange.bottom; ++y)
//    //{
//    //    Vector3 pos = offset + (stepY * y) + (stepX * renderRange.left);
//    //    for (int x = renderRange.left; x < renderRange.right; ++x)
//    //    {
//    //    }
//    //}
//
//}
//

//==============================================================================
// TilemapLayer

LN_OBJECT_IMPLEMENT(TilemapLayer, Object)
{
    context->registerType<TilemapLayer>({});
}

Ref<TilemapLayer> TilemapLayer::create()
{
    return makeObject<TilemapLayer>();
}

TilemapLayer::TilemapLayer()
    : m_tileSize(1, 1)
    , m_orientation(TilemapOrientation::UpFlow)
{
}

TilemapLayer::~TilemapLayer()
{
}

void TilemapLayer::init()
{
	Object::init();
}

void TilemapLayer::setTileSize(const Size& size)
{
    m_tileSize = size;
}

void TilemapLayer::resize(int width, int height)
{
	m_width = width;
	m_height = height;
    m_data.resize(width * height);
}

void TilemapLayer::setTileId(int x, int y, int id)
{
    m_data[y * m_width + x] = id;
}

int TilemapLayer::tileId(int x, int y)
{
	return m_data[y * m_width + x];
}

int TilemapLayer::getTileId(int x, int y) const
{
    // TODO: round
    // clamp
    if (x < 0 || m_width <= x) return 0;
    if (y < 0 || m_height <= y) return 0;

    return m_data[y * m_width + x];
}

void TilemapLayer::putAutoTile(int x, int y, int autoTilesetId)
{
	// 仮で設定して、
	setTileId(x, y, Tileset::AutoTileOffset + Tileset::AutoTileSetStride * autoTilesetId);

	// 周辺を含めて更新する
	PointI points[9] = {
		{ x - 1, y - 1 }, { x, y - 1 }, { x + 1, y - 1 },
		{ x - 1, y }, { x, y }, { x + 1, y  },
		{ x - 1, y + 1 }, { x, y + 1 }, { x + 1, y + 1 },
	};
	for (int i = 0; i < 9; i++) {
		if (isValidRange(points[i].x, points[i].y)) {
			refreshAutoTile(points[i].x, points[i].y);
		}
	}
}

void TilemapLayer::refreshAutoTile(int x, int y)
{
	int autoTilesetId = Tileset::autoTileIndex(getTileId(x, y));
	if (autoTilesetId < 0) return;

	PointI points[9] = {
		{ x - 1, y - 1 }, { x, y - 1 }, { x + 1, y - 1 },
		{ x - 1, y }, { x, y }, { x + 1, y  },
		{ x - 1, y + 1 }, { x, y + 1 }, { x + 1, y + 1 },
	};

	// 周囲のタイルが同一種類かを調べる
	bool ids[9];
	for (int i = 0; i < 9; i++) {
		int placedAutoTilesetId = -1;
		if (isOutOfRange(points[i].x, points[i].y)) {
			// Out range is same Id.
			ids[i] = true;
		}
		else {
			placedAutoTilesetId = Tileset::autoTileIndex(getTileId(points[i].x, points[i].y));
			ids[i] = (placedAutoTilesetId == autoTilesetId);
		}
	}

	// 四隅の種類を確定する
	int subtiles[4] = { 0, 0, 0, 0 };
	const int checkIdxs[4][3] = { { 0, 3, 1 }, { 2, 5, 1 }, { 6, 3, 7 }, { 8, 5, 7 }, };	// [対角,横,縦]
	for (int i = 0; i < 4; i++) {
		const int* checkIdx = checkIdxs[i];
		if (!ids[checkIdx[1]] || !ids[checkIdx[2]]) {
			if (!ids[checkIdx[1]] && !ids[checkIdx[2]])
				subtiles[i] = 5;
			else if (ids[checkIdx[2]])
				subtiles[i] = 4;
			else
				subtiles[i] = 3;
		}
		else if (!ids[checkIdx[0]])
			subtiles[i] = 2;
		else
			subtiles[i] = 1;
	}

	// subtiles が一致するものを線形で検索
	int localTileId = -1;
	for (int i = 0; i < Tileset::AutoTileUnitStride; i++) {
		const auto& info = detail::g_AutoTileTable[i];
		if (info.subtiles[0] == subtiles[0] &&
			info.subtiles[1] == subtiles[1] &&
			info.subtiles[2] == subtiles[2] &&
			info.subtiles[3] == subtiles[3]) {
			localTileId = i;
		}
	}
	if (LN_REQUIRE(localTileId >= 0)) return;

	setTileId(x, y, Tileset::AutoTileOffset + (Tileset::AutoTileSetStride * autoTilesetId) + localTileId);
}

// bounds: Y+ を上方向とした、ローカル空間上の描画範囲
void TilemapLayer::render(TilemapModel* model, RenderingContext* context, const Matrix& transform, const detail::TilemapBounds& bounds)
{

	//int l, t, r, b;         // 2D array としてどの範囲を描画するか
	float wL, wT, wR, wB;   // world としてどの範囲を描画するか
	//Vector3 offset;
	int l, t, r, b;
	{
		wL = l = static_cast<int>(bounds.l / m_tileSize.width);
		wT = t = static_cast<int>(bounds.t / m_tileSize.height);
		wR = r = static_cast<int>(bounds.r / m_tileSize.width);
		wB = b = static_cast<int>(bounds.b / m_tileSize.height);
		//wL *= m_tileSize.width;
		//wT *= m_tileSize.height;
		//wR *= m_tileSize.width;
		//wB *= m_tileSize.height;
		//t = -t;
		//b = -b;

		//if (m_orientation == TilemapOrientation::UpFlow)
		//{
		//    offset.y = m_tileSize.height * m_size.height;
		//    // Y=0 から↑へ向かって配置する。(0, 0) = [0, m_size.height-1]
		//    //t = m_size.height - t;
		//    //b = m_size.height - b;
		//}
		//else if (m_orientation == TilemapOrientation::DownFlow)
		//{
		//    // Y=0 から↓へ向かって配置する。(0, 0) = [0, 0]
		//    //t = -t;
		//    //b = -b;
		//}
		//else
		//{
		//    LN_UNREACHABLE();
		//}

		r += 1;
		b -= 1;
	}

	int width = getWidth();
	int height = getHeight();

	//float tw = m_tileSize.width;
	//float th = m_tileSize.height;
	for (int y = t; y >= b; y--)
	{
		// clamp
		//if (y < 0 || m_size.height <= y) continue;

		for (int x = l; x < r; x++)
		{
			// clamp
			//if (x < 0 || m_size.width <= x) continue;
			int tileId = 0;

			if (m_orientation == TilemapOrientation::UpFlow)
			{
				tileId = getTileId(x, height - y - 1);
			}
			else if (m_orientation == TilemapOrientation::DownFlow) {
				tileId = getTileId(x, -y);
			}
			else {
				LN_UNREACHABLE();
			}



			if (tileId > 0)
			{
				Vector3 pos(x * m_tileSize.width, y * m_tileSize.height, 0);
				Tileset* tileset;
				int localId;
				if (model->fetchTileset(tileId, &tileset, &localId)) {
					tileset->drawTile(context, localId, pos, m_tileSize);
				}
			}
		}
	}


	//for (int y = t; y < renderRange.bottom; ++y)
	//{
	//    Vector3 pos = offset + (stepY * y) + (stepX * renderRange.left);
	//    for (int x = renderRange.left; x < renderRange.right; ++x)
	//    {
	//    }
	//}

}

void TilemapLayer::serialize(Archive& ar)
{
	Object::serialize(ar);
	ar & makeNVP(u"width", m_width);
	ar & makeNVP(u"height", m_height);
	ar & makeNVP(u"data", m_data);
}

} // namespace ln

