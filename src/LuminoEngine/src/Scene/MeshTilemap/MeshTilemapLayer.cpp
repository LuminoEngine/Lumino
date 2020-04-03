
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapLayer.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapModel.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTileset.hpp>

namespace ln {


const MeshTilemapLayer::Vec3I MeshTilemapLayer::TopOffsets[6] = {
	{ // XMinus
		-1, 0, 0,
	},
	{ // XPlus
		+1, 0, 0,
	},
	{ // YMinus
		0, -1, 0,
	},
	{ // YPlus
		0, +1, 0,
	},
	{ // ZMinus
		0, 0, -1,
	},
	{ // ZPlus
		0, 0, +1,
	},
};

// [軸平面上のSubtileIndex][面方向にひとつ進んだ位置のSubtileIndex] -> Mapに保存する LocalAutoBlockId
static const int AutBlockPairMap[6][6] = {
	{ 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 2, 4, 3, 5 },
	{ 0, 0, 0, 4, 3, 5 },
	{ 0, 0, 0, 0, 3, 3 },
	{ 0, 0, 0, 4, 0, 4 },
	{ 0, 0, 0, 0, 0, 0 },
};

//==============================================================================
// MeshTilemapLayer

LN_OBJECT_IMPLEMENT(MeshTilemapLayer, Object) {}

MeshTilemapLayer::MeshTilemapLayer()
	: m_tiles()
	, m_sizeX(0)
	, m_sizeY(0)
	, m_sizeZ(0)
{
}

void MeshTilemapLayer::init()
{
	Object::init();
}

void MeshTilemapLayer::resize(int sizeX, int sizeY, int sizeZ)
{
	m_sizeX = sizeX;
	m_sizeY = sizeY;
	m_sizeZ = sizeZ;
	m_tiles.resize(m_sizeX * m_sizeY * m_sizeZ);
}

void MeshTilemapLayer::setTileId(int x, int y, int z, int id)
{
	if (LN_REQUIRE(isValidIndex(x, y, z))) return;
	tile(x, y, z).tileId = id;
}

void MeshTilemapLayer::putAutoTile(int x, int y, int z, int autotileKindId)
{
	// 仮で設定して、
	setTileId(x, y, z, MeshTileset::AutoTileOffset + MeshTileset::AutoTileSetStride * autotileKindId);

	// 周辺を含めて更新する
	Vec3I points[27] = {
		{ x - 1, y - 1, z - 1 }, { x, y - 1, z - 1 }, { x + 1, y - 1, z - 1 },
		{ x - 1, y, z - 1 }, { x, y , z - 1}, { x + 1, y, z - 1 },
		{ x - 1, y + 1, z - 1 }, { x, y + 1, z - 1 }, { x + 1, y + 1, z - 1 },

		{ x - 1, y - 1, z }, { x, y - 1, z }, { x + 1, y - 1, z },
		{ x - 1, y, z }, { x, y, z }, { x + 1, y, z },
		{ x - 1, y + 1, z }, { x, y + 1, z }, { x + 1, y + 1, z },

		{ x - 1, y - 1, z + 1 }, { x, y - 1, z + 1 }, { x + 1, y - 1, z + 1 },
		{ x - 1, y, z + 1 }, { x, y, z + 1 }, { x + 1, y, z + 1 },
		{ x - 1, y + 1, z + 1 }, { x, y + 1, z + 1 }, { x + 1, y + 1, z + 1 },
	};
	for (int i = 0; i < LN_ARRAY_SIZE_OF(points); i++) {
		if (isValidIndex(points[i].x, points[i].y, points[i].z)) {
			refreshAutoTile(points[i].x, points[i].y, points[i].z);
		}
	}
}

void MeshTilemapLayer::setAutoTileIdDirect(int x, int y, int z, MeshTileFaceDirection dir, int id)
{
	if (LN_REQUIRE(isValidIndex(x, y, z))) return;
	tile(x, y, z).faceTileId[(int)dir] = id;
}

void MeshTilemapLayer::refreshAutoTile(int x, int y, int z)
{
	auto& tileInfo = tile(x, y, z);
	if (MeshTileset::autoTileKindId(tileInfo.tileId) >= 0) {
		for (int i = 0; i < 6; i++) {
			refreshAutoTileFace(x, y, z, static_cast<MeshTileFaceDirection>(i));
		}
	}
}

void MeshTilemapLayer::refreshAutoTileFace(int x, int y, int z, MeshTileFaceDirection dir)
{
	auto& tileInfo = tile(x, y, z);
	int autotileKindId = MeshTileset::autoTileKindId(tileInfo.tileId);



	auto& topOffset = MeshTilemapLayer::TopOffsets[(int)dir];

	int gbid = getGBIDSafe(x + topOffset.x, y + topOffset.y, z + topOffset.z);
	if (MeshTileset::autoTileKindId(gbid) == autotileKindId) {
		// 面方向に同一種類のブロックが存在している
		AutoTileNearbyInfo nearbyInfo1, nearbyInfo2;
		makeAutoTileNearbyInfo(x, y, z, dir, &nearbyInfo1);
		makeAutoTileNearbyInfo(x + topOffset.x, y + topOffset.y, z + topOffset.z, dir, &nearbyInfo2);

			
		tileInfo.faceTileId[static_cast<int>(dir)] = getAutoBlockLocalIdFromNearbyInfo2(nearbyInfo1, nearbyInfo2, autotileKindId) + MeshTileset::PiledAutoBlockOffset;
	}
	else {
		AutoTileNearbyInfo nearbyInfo;
		makeAutoTileNearbyInfo(x, y, z, dir, &nearbyInfo);
		tileInfo.faceTileId[static_cast<int>(dir)] = getAutoBlockLocalIdFromNearbyInfo(nearbyInfo, autotileKindId);
	}
}

void MeshTilemapLayer::makeAutoTileNearbyInfo(int x, int y, int z, MeshTileFaceDirection dir, AutoTileNearbyInfo* outInfo) const
{
	assert(outInfo);

	const Vec3I offsets[6][9] = {
		{ // XMinus
			{  0, +1, +1 }, {  0, +1, 0 }, {  0, +1, -1 },
			{  0,  0, +1 }, {  0,  0, 0 }, {  0,  0, -1 },
			{  0, -1, +1 }, {  0, -1, 0 }, {  0, -1, -1 },
		},
		{ // XPlus
			{  0, +1, -1 }, {  0, +1, 0 }, {  0, +1, +1 },
			{  0,  0, -1 }, {  0,  0, 0 }, {  0,  0, +1 },
			{  0, -1, -1 }, {  0, -1, 0 }, {  0, -1, +1 },
		},
		{ // YMinus
			{ +1,  0, +1 }, {  0,  0, +1 }, { -1,  0, +1 },
			{ +1,  0,  0 }, {  0,  0,  0 }, { -1,  0,  0 },
			{ +1,  0, -1 }, {  0,  0, -1 }, { -1,  0, -1 },
		},
		{ // YPlus
			{ -1,  0, +1 }, {  0,  0, +1 }, { +1,  0, +1 },
			{ -1,  0,  0 }, {  0,  0,  0 }, { +1,  0,  0 },
			{ -1,  0, -1 }, {  0,  0, -1 }, { +1,  0, -1 },
		},
		{ // ZMinus
			{  -1, +1, 0 }, {  0, +1, 0 }, {  +1, +1,  0 },
			{  -1,  0, 0 }, {  0,  0, 0 }, {  +1,  0,  0 },
			{  -1, -1, 0 }, {  0, -1, 0 }, {  +1, -1,  0 },
		},
		{ // ZPlus
			{  +1, +1, 0 }, {  0, +1, 0 }, {  -1, +1,  0 },
			{  +1,  0, 0 }, {  0,  0, 0 }, {  -1,  0,  0 },
			{  +1, -1, 0 }, {  0, -1, 0 }, {  -1, -1,  0 },
		},
	};

	//PointI offsets2D[9] = {
	//	{ -1, -1 }, { 0, -1 }, { +1, -1 },
	//	{ -1,  0 }, { 0,  0 }, { +1,  0 },
	//	{ -1, +1,}, { 0, +1 }, { +1, +1 },
	//};

	const auto& ofs = offsets[static_cast<int>(dir)];
	for (int i = 0; i < 9; i++) {
		//outInfo->points[i].x = x + ofs[i].x;
		//outInfo->points[i].y = y + ofs[i].y;
		//outInfo->points[i].z = z + ofs[i].z;

		int cx = x + ofs[i].x;
		int cy = y + ofs[i].y;
		int cz = z + ofs[i].z;

		outInfo->tileIds[i] = 0;
		if (isValidIndex(cx, cy, cz)) {
			//outInfo->tileIds[i] = tile(cx, cy, cz).tileId;
			int id1 = tile(cx, cy, cz).tileId;

			if (0) {	// 面が覆われている場合、別種類タイルにするかどうか（TODO: NG: 押し込まれたオートタイルが不自然になる。完全 box のばあいはいいが。）
				if (id1 >= 1) {
					// 面方向にひとつ進んだ Block も調べる (面が覆われているか)
					int cx2 = cx + TopOffsets[static_cast<int>(dir)].x;
					int cy2 = cy + TopOffsets[static_cast<int>(dir)].y;
					int cz2 = cz + TopOffsets[static_cast<int>(dir)].z;
					if (isValidIndex(cx2, cy2, cz2)) {
						int id2 = tile(cx2, cy2, cz2).tileId;
						//outInfo->piledTileIds[i] = id1;

						if (id2 >= 1) {
							id1 = 0;
						}
					}
				}
			}

			outInfo->tileIds[i] = id1;
		}

	}

	//{
	//	int cx = x + topOffsets[static_cast<int>(dir)].x;
	//	int cy = y + topOffsets[static_cast<int>(dir)].y;
	//	int cz = z + topOffsets[static_cast<int>(dir)].z;
	//	if (isValidIndex(cx, cy, cz)) {
	//		outInfo->topTileId = tile(cx, cy, cz).tileId;
	//	}
	//	else {
	//		outInfo->topTileId = -1;
	//	}
	//}
	
}

void MeshTilemapLayer::getSubtileIdsFromNearbyInfo(const AutoTileNearbyInfo& nearbyInfo, int autoBlockKindId, int subtiles[4]) const
{
	//int localTileId = MeshTileset::autoTileLocalId(tileId);

	bool equals[9];
	for (int i = 0; i < 9; i++) {
		// -1(マップ範囲外) は同一種類とする
		equals[i] = (nearbyInfo.tileIds[i] < 0) || (MeshTileset::autoTileKindId(nearbyInfo.tileIds[i]) == autoBlockKindId);
	}

	// 四隅の種類を確定する
	//int subtiles[4] = { 0, 0, 0, 0 };
	const int checkIdxs[4][3] = { { 0, 3, 1 }, { 2, 5, 1 }, { 6, 3, 7 }, { 8, 5, 7 }, };	// [対角,横,縦]
	for (int i = 0; i < 4; i++) {
		const int* checkIdx = checkIdxs[i];
		if (!equals[checkIdx[1]] || !equals[checkIdx[2]]) {
			if (!equals[checkIdx[1]] && !equals[checkIdx[2]])
				subtiles[i] = 5;
			else if (equals[checkIdx[2]])
				subtiles[i] = 4;
			else
				subtiles[i] = 3;
		}
		else if (!equals[checkIdx[0]])
			subtiles[i] = 2;
		else
			subtiles[i] = 1;
	}
}

// 0~47
int MeshTilemapLayer::getAutoBlockLocalIdFromNearbyInfo(const AutoTileNearbyInfo& nearbyInfo, int autoBlockKindId) const
{
	int subtiles[4] = { 0, 0, 0, 0 };
	getSubtileIdsFromNearbyInfo(nearbyInfo, autoBlockKindId, subtiles);

	// subtiles が一致するものを線形で検索
	int newLocalTileId = -1;
	for (int i = 0; i < MeshTileset::AutoTileUnitStride; i++) {
		const auto& info = MeshTileset::AutoTileTable[i];
		if (info.subtiles[0] == subtiles[0] &&
			info.subtiles[1] == subtiles[1] &&
			info.subtiles[2] == subtiles[2] &&
			info.subtiles[3] == subtiles[3]) {
			newLocalTileId = i;
		}
	}
	if (LN_REQUIRE(newLocalTileId >= 0)) return -1;

	return newLocalTileId;
}

int MeshTilemapLayer::getAutoBlockLocalIdFromNearbyInfo2(const AutoTileNearbyInfo& nearbyInfo1, const AutoTileNearbyInfo& nearbyInfo2, int autoBlockKindId) const
{
	int subtiles1[4] = { 0, 0, 0, 0 };
	int subtiles2[4] = { 0, 0, 0, 0 };
	getSubtileIdsFromNearbyInfo(nearbyInfo1, autoBlockKindId, subtiles1);
	getSubtileIdsFromNearbyInfo(nearbyInfo2, autoBlockKindId, subtiles2);

	int subtiles[4];
	for (int i = 0; i < 4; i++) {
		subtiles[i] = AutBlockPairMap[subtiles1[i]][subtiles2[i]];
	}

	if (subtiles[0] == 0 &&
		subtiles[1] == 0 &&
		subtiles[2] == 0 &&
		subtiles[3] == 0) {
		return 0;
	}

	// subtiles が一致するものを線形で検索
	int newLocalTileId = -1;
	for (int i = 0; i < MeshTileset::AutoTileUnitStride; i++) {
		const auto& info = MeshTileset::AutoTileTable[i];
		if (info.subtiles[0] == subtiles[0] &&
			info.subtiles[1] == subtiles[1] &&
			info.subtiles[2] == subtiles[2] &&
			info.subtiles[3] == subtiles[3]) {
			newLocalTileId = i;
		}
	}
	if (LN_REQUIRE(newLocalTileId >= 0)) return -1;

	return newLocalTileId;
}

static int g_frames = 0;

void MeshTilemapLayer::draw(RenderingContext* context, MeshTileset* tileset)
{
	g_frames++;

	// TODO: View culling

	tileset->beginBatch();


	for (int y = 0; y < m_sizeY; y++) {
		for (int z = 0; z < m_sizeZ; z++) {
			for (int x = 0; x < m_sizeX; x++) {

				const detail::MeshTile& t = tile(x, y, z);

				if (t.tileId > 0) {

					detail::MeshTileFaceAdjacency adjacency;
					for (int iFace = 0; iFace < 6; iFace++) {
						adjacency.state[iFace] = detail::MeshTileFaceAdjacency::OPEN;

						int autoKID = MeshTileset::autoTileKindId(t.tileId);
						auto& autotileset = tileset->autotileSet(autoKID);

						//if (!autotileset->beveled[iFace]) {
						int cx = x + TopOffsets[iFace].x;
						int cy = y + TopOffsets[iFace].y;
						int cz = z + TopOffsets[iFace].z;
						if (isValidIndex(cx, cy, cz)) {
							if (autoKID == MeshTileset::autoTileKindId(tile(cx, cy, cz).tileId)) {
								adjacency.state[iFace] = detail::MeshTileFaceAdjacency::BURIED;
							}
						}
						//}
						//else {

						//}
					}





					//context->setBaseTransfrom(Matrix::makeTranslation(x, y, z));
					tileset->drawTile(context, t, adjacency, Matrix::makeTranslation(x, y, z), g_frames / 20);
					//return;	// TODO: test
				}
			}
		}
	}

	tileset->drawBatch(context);

	//   context->setMaterial(m_material);
	   //for (int i = 0; i < 48; i++) {
	   //	context->setBaseTransfrom(Matrix::makeTranslation(i, 0, 0));
	   //	context->drawMesh(m_mesh, i);
	   //}
}

} // namespace ln

