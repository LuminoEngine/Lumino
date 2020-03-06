
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapLayer.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapModel.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTileset.hpp>

namespace ln {


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
	tile(x, y, z).id[(int)dir] = id;
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

	AutoTileNearbyInfo nearbyInfo;
	makeAutoTileNearbyInfo(x, y, z, dir, &nearbyInfo);

	int autotileKindId = MeshTileset::autoTileKindId(tileInfo.tileId);

	//int localTileId = MeshTileset::autoTileLocalId(tileId);

	bool equals[9];
	for (int i = 0; i < 9; i++) {
		// -1(範囲外) は同一種類とする
		equals[i] = (nearbyInfo.tileIds[i] < 0) || (MeshTileset::autoTileKindId(nearbyInfo.tileIds[i]) == autotileKindId);
	}

	// 四隅の種類を確定する
	int subtiles[4] = { 0, 0, 0, 0 };
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
	if (LN_REQUIRE(newLocalTileId >= 0)) return;

	tileInfo.id[static_cast<int>(dir)] = newLocalTileId;
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
		if (isValidIndex(cx, cy, cz)) {
			outInfo->tileIds[i] = tile(cx, cy, cz).tileId;
		}
		else {
			outInfo->tileIds[i] = -1;
		}
	}

}

void MeshTilemapLayer::draw(RenderingContext* context, const MeshTileset* tileset)
{
	// TODO: View culling

	for (int y = 0; y < m_sizeY; y++) {
		for (int z = 0; z < m_sizeZ; z++) {
			for (int x = 0; x < m_sizeX; x++) {
				const detail::MeshTile& t = tile(x, y, z);

				context->setBaseTransfrom(Matrix::makeTranslation(x, y, z));
				tileset->drawTile(context, t);
			}
		}
	}


	//   context->setMaterial(m_material);
	   //for (int i = 0; i < 48; i++) {
	   //	context->setBaseTransfrom(Matrix::makeTranslation(i, 0, 0));
	   //	context->drawMesh(m_mesh, i);
	   //}
}

} // namespace ln

