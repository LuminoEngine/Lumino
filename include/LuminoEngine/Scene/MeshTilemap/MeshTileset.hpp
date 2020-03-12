
#pragma once
#include "Common.hpp"

namespace ln {

enum class PredefinedMeshAutoTilesetMesh
{
	Uniform,	// 全面均一
	Wall,	// 壁
};

class MeshAutoTileset
	: public Object
{
public:
	
LN_CONSTRUCT_ACCESS:
    MeshAutoTileset();
	void init();
};

class MeshTileset
	: public Object
{
    LN_OBJECT;
public:
	struct AutoTileInfo
	{
		uint8_t subtiles[4];	// [top-left][top-right][bottom-left][bottom-light]
	};

	static const int AutoTileOffset = 16384;
	static const int AutoTileSetStride = 1024;
	static const int AutoTileUnitStride = 48;
	static const int MaxAutoTilests = AutoTileOffset / AutoTileSetStride;
	static const int LocalAutoTileIdSlopeOffset = 96;
	static const AutoTileInfo AutoTileTable[48];

	static int autoTileKindId(int tileId) { return (tileId < AutoTileOffset) ? -1 : ((tileId - AutoTileOffset) / AutoTileSetStride); }
	static int localIdToGlobalId(int localTileId, int autotileKind) { return AutoTileSetStride * autotileKind + localTileId + AutoTileOffset; }

LN_CONSTRUCT_ACCESS:
    MeshTileset();
	void init();

private:
	void drawTile(RenderingContext* context, const detail::MeshTile& tile, const detail::MeshTileFaceAdjacency& adjacency) const;

	Ref<Material> m_material;
	Ref<Mesh> m_mesh;

	friend class MeshTilemapLayer;
};

namespace detail {

enum SubtileCorner
{
	SubtileCorner_TopLeft = 0,
	SubtileCorner_TopRight = 1,
	SubtileCorner_BottomLeft = 3,
	SubtileCorner_BottomRight = 3,
};

class MeshAutoTilesetUVMapper
{
public:
	enum class Format
	{
		//XP,
		//MV,
		MVWithWall,
	};

	MeshAutoTilesetUVMapper(const Size& textureSize, const Rect& sourceRect, Format format);

	// 
	Rect getUVRectFromLocalId(MeshTileFaceDirection direction, int autotileLocalId, SubtileCorner corner) const;

private:
	Format m_format;
	Vector2 m_globalUVOffset;
	Vector2 m_subtileUVSize;
};

} // namespace detail
} // namespace ln
