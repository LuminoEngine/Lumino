
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

LN_OBJECT_IMPLEMENT(TilemapLayer, Object) {}

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

void TilemapLayer::putAutoTileDirect(int x, int y, int autoTilesetId, int localAutoTileId)
{
	setTileId(x, y, Tileset::AutoTileOffset + Tileset::AutoTileSetStride * autoTilesetId + localAutoTileId);
}

bool TilemapLayer::putAutoTileSlope(int x, int y, int autoTilesetId)
{
	AutoTileNearbyInfo nearbyInfo;
	makeAutoTileNearbyInfo(x, y, autoTilesetId, &nearbyInfo);

	// 四隅いずれかとして配置できるかしらべる
	//PointI cornerCheckPoints[4][3] = {
	//	{ { x - 1, y - 1 }, { x - 1, y }, { x, y - 1 } },
	//	{ { x + 1, y - 1 }, { x + 1, y }, { x, y - 1 } },
	//	{ { x - 1, y + 1 }, { x - 1, y }, { x, y + 1 } },
	//	{ { x + 1, y + 1 }, { x + 1, y }, { x, y + 1 } },
	//};
	const int cornerCheckPoints[4][3] = {
		{ 0, 1, 3 }, { 1, 2, 5 }, { 3, 6, 7 }, { 5, 7, 8 }
	};
	Corner corner = Corner_Invalid;
	for (int i = 0; i < 4; i++) {
		const int* pts = cornerCheckPoints[i];
		if (nearbyInfo.equals[pts[0]] && nearbyInfo.equals[pts[1]] && nearbyInfo.equals[pts[2]]) {
			corner = static_cast<Corner>(i);
			break;
		}
	}
	if (corner == Corner_Invalid) {
		return false;
	}

	// Set Slope-tile.
	int slopeLocalAutoTileIds[4] = {
		Tileset::AutoTileOffset + Tileset::AutoTileSetStride * autoTilesetId + Tileset::LocalAutoTileIdSlopeOffset + 3,
		Tileset::AutoTileOffset + Tileset::AutoTileSetStride * autoTilesetId + Tileset::LocalAutoTileIdSlopeOffset + 2,
		Tileset::AutoTileOffset + Tileset::AutoTileSetStride * autoTilesetId + Tileset::LocalAutoTileIdSlopeOffset + 1,
		Tileset::AutoTileOffset + Tileset::AutoTileSetStride * autoTilesetId + Tileset::LocalAutoTileIdSlopeOffset + 0,
	};
	setTileId(x, y, slopeLocalAutoTileIds[corner]);

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

	return true;
}

void TilemapLayer::makeAutoTileNearbyInfo(int x, int y, int autoTilesetId, AutoTileNearbyInfo* outInfo) const
{
	assert(outInfo);
	outInfo->points[0] = { x - 1, y - 1 };
	outInfo->points[1] = { x, y - 1 };
	outInfo->points[2] = { x + 1, y - 1 };
	outInfo->points[3] = { x - 1, y };
	outInfo->points[4] = { x, y };
	outInfo->points[5] = { x + 1, y };
	outInfo->points[6] = { x - 1, y + 1 };
	outInfo->points[7] = { x, y + 1 };
	outInfo->points[8] = { x + 1, y + 1 };

	// 周囲のタイルが同一種類かを調べる
	for (int i = 0; i < 9; i++) {
		int placedAutoTilesetId = -1;
		const auto& pt = outInfo->points[i];
		if (isOutOfRange(pt.x, pt.y)) {
			// Out range is same Id.
			outInfo->equals[i] = true;
			outInfo->tileIds[i] = -1;
			outInfo->localAutoTileIds[i] = -1;
		}
		else {
			int tileId = getTileId(pt.x, pt.y);
			placedAutoTilesetId = Tileset::autoTileIndex(tileId);
			outInfo->equals[i] = (placedAutoTilesetId == autoTilesetId);
			outInfo->tileIds[i] = tileId;
			outInfo->localAutoTileIds[i] = Tileset::autoTileLocalId(tileId);
		}
	}

	{
		memset(&(outInfo->halfSlopeTipWeights), 0, sizeof(outInfo->halfSlopeTipWeights));
		if (outInfo->localAutoTileIds[7] == 107) {
			outInfo->halfSlopeTipWeights[7][Corner_BL] = 1;
			outInfo->halfSlopeTipWeights[7][Corner_BR] = 2;
		}
		if (outInfo->localAutoTileIds[7] == 111) {
			outInfo->halfSlopeTipWeights[7][Corner_BL] = 2;
			outInfo->halfSlopeTipWeights[7][Corner_BR] = 1;
		}
		if (outInfo->localAutoTileIds[3] == 113) {
			outInfo->halfSlopeTipWeights[3][Corner_TL] = 1;
			outInfo->halfSlopeTipWeights[3][Corner_BL] = 2;
		}
		if (outInfo->localAutoTileIds[3] == 117) {
			outInfo->halfSlopeTipWeights[3][Corner_TL] = 2;
			outInfo->halfSlopeTipWeights[3][Corner_BL] = 1;
		}
		if (outInfo->localAutoTileIds[5] == 115) {
			outInfo->halfSlopeTipWeights[5][Corner_TR] = 1;
			outInfo->halfSlopeTipWeights[5][Corner_BR] = 2;
		}
		if (outInfo->localAutoTileIds[5] == 119) {
			outInfo->halfSlopeTipWeights[5][Corner_TR] = 2;
			outInfo->halfSlopeTipWeights[5][Corner_BR] = 1;
		}
		if (outInfo->localAutoTileIds[1] == 104) {
			outInfo->halfSlopeTipWeights[1][Corner_TL] = 1;
			outInfo->halfSlopeTipWeights[1][Corner_TR] = 2;
		}
		if (outInfo->localAutoTileIds[1] == 109) {
			outInfo->halfSlopeTipWeights[1][Corner_TL] = 2;
			outInfo->halfSlopeTipWeights[1][Corner_TR] = 1;
		}
	}
}

void TilemapLayer::refreshAutoTile(int x, int y)
{
	int tileId = getTileId(x, y);
	int autoTilesetId = Tileset::autoTileIndex(tileId);
	if (autoTilesetId < 0) return;

	AutoTileNearbyInfo nearbyInfo;
	makeAutoTileNearbyInfo(x, y, autoTilesetId, &nearbyInfo);

	int localTileId = Tileset::autoTileLocalId(tileId);
	if (Tileset::isBlockAutoTile(localTileId)) {
		const bool* ids = nearbyInfo.equals;

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
			else if (Tileset::isSlopeAutoTile(nearbyInfo.localAutoTileIds[checkIdx[1]]) && Tileset::isSlopeAutoTile(nearbyInfo.localAutoTileIds[checkIdx[2]]))
				subtiles[i] = 6;
			else if (Tileset::isTipHalfSlopeAutoTile(nearbyInfo.localAutoTileIds[checkIdx[1]])) {
				if (nearbyInfo.halfSlopeTipWeights[checkIdx[1]][i] == 2)
					subtiles[i] = 9;
				else if (nearbyInfo.halfSlopeTipWeights[checkIdx[1]][i] == 1)
					subtiles[i] = 10;
				else {
					LN_UNREACHABLE();
				}
			}
			else if (Tileset::isTipHalfSlopeAutoTile(nearbyInfo.localAutoTileIds[checkIdx[2]])) {
				if (nearbyInfo.halfSlopeTipWeights[checkIdx[2]][i] == 2)
					subtiles[i] = 8;
				else if (nearbyInfo.halfSlopeTipWeights[checkIdx[2]][i] == 1)
					subtiles[i] = 7;
				else {
					LN_UNREACHABLE();
				}
			}
			else if (!ids[checkIdx[0]])
				subtiles[i] = 2;
			else
				subtiles[i] = 1;
		}

		// subtiles が一致するものを線形で検索
		int newLocalTileId = -1;
		for (int i = 0; i < Tileset::AutoTileUnitStride; i++) {
			const auto& info = detail::g_AutoTileTable[i];
			if (info.subtiles[0] == subtiles[0] &&
				info.subtiles[1] == subtiles[1] &&
				info.subtiles[2] == subtiles[2] &&
				info.subtiles[3] == subtiles[3]) {
				newLocalTileId = i;
			}
		}
		if (LN_REQUIRE(newLocalTileId >= 0)) return;

		setTileId(x, y, Tileset::AutoTileOffset + (Tileset::AutoTileSetStride * autoTilesetId) + newLocalTileId);
	}
	else if (Tileset::isHalfSlopeAutoTile(localTileId)) {
		// Half-slope は自動配置しない
	}
	else if (Tileset::isSlopeAutoTile(localTileId)) {
		// Half-slope は、Tip になりえる側(x, y)から、Root を作っていく。

		// Block への接続辺が１つである Slope のみ処理対象としたい。
		{
			int count = 0;
			count += (Tileset::isBlockAutoTile(nearbyInfo.localAutoTileIds[1])) ? 1 : 0;
			count += (Tileset::isBlockAutoTile(nearbyInfo.localAutoTileIds[3])) ? 1 : 0;
			count += (Tileset::isBlockAutoTile(nearbyInfo.localAutoTileIds[5])) ? 1 : 0;
			count += (Tileset::isBlockAutoTile(nearbyInfo.localAutoTileIds[7])) ? 1 : 0;
			if (count >= 2) return;
		}

		int checkEdges[4] = { 3, 5, 1, 7 };
		Edge edge = Edge_Invalid;
		for (int i = 0; i < 4; i++) {
			if (nearbyInfo.equals[checkEdges[i]] && Tileset::isBlockAutoTile(nearbyInfo.localAutoTileIds[checkEdges[i]])) {
				edge = static_cast<Edge>(i);
				break;
			}
		}
		if (edge != Edge_Invalid) {
			const struct Side {
				int pt;	// tip を基準にどちらを見たいか
				int tipTile;
				int rootTile;
			} sides[4][2] = {
				// Left
				{
					{ 7, 115, 114 },	// dir: top
					{ 1, 119, 118 },	// dir: bottom
				},
				// Right
				{
					{ 7, 113, 112 },	// dir: top
					{ 1, 117, 116 },	// dir: bottom
				},
				// Top
				{
					{ 3, 107, 106 },	// dir: left
					{ 5, 111, 110 },	// dir: right
				},

				// Bottom
				{
					{ 3, 109, 108 },	// dir: left
					{ 5, 105, 104 },	// dir: right
				},
			};

			for (int i = 0; i < 2; i++) {
				const auto& side = sides[edge][i];

				if (nearbyInfo.equals[side.pt]) {
					int localAutoTileId = Tileset::autoTileLocalId(nearbyInfo.tileIds[side.pt]);
					if (Tileset::isSlopeAutoTile(localAutoTileId)) {
						putAutoTileDirect(x, y, autoTilesetId, side.tipTile);

						const auto& rootPt = nearbyInfo.points[side.pt];
						if (isValidRange(rootPt.x, rootPt.y)) {
							putAutoTileDirect(rootPt.x, rootPt.y, autoTilesetId, side.rootTile);
						}
					}
				}
			}
		}
	}
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
//
//void TilemapLayer::serialize(Archive& ar)
//{
//	Object::serialize(ar);
//	ar & makeNVP(u"width", m_width);
//	ar & makeNVP(u"height", m_height);
//	ar & makeNVP(u"data", m_data);
//}
//
} // namespace ln

