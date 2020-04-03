
#pragma once
#include "Common.hpp"

namespace ln {
class InstancedMeshList;

enum class PredefinedMeshAutoTilesetMesh
{
	Uniform,	// 全面均一
	Wall,	// 壁
};

class MeshAutoTileset
	: public Object
{
public:
	bool beveled[6];

	void setMaterial(Material* value);
	void setAnimationFrames(int value) { m_animationFrameCount = value; }

	// 正方形メッシュ, 上下面に Floor, 側面に Wall
	void buildQubeFloorAndWall();

	// 上面のみに平面 Floor
	void buildFloor();

	// 壁に少し角度をつける
	void buildFloorAndSlopeWall();


	void resetBatch();
	void setInstancedMeshList(int d, int autotileId, bool dent, InstancedMeshList* value);
	InstancedMeshList* instancedMeshList(int d, int autotileId, bool dent) const;
	void drawVoxel(const detail::MeshTile& tile, const detail::MeshTileFaceAdjacency& adjacency, const Matrix& transform, int animationFrame) const;
	void flushBatch(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    MeshAutoTileset();
	void init();

private:
	Ref<Material> m_material;
	Vector2 m_frameUVOffset;
	int m_animationFrameCount = 0;
	Ref<Mesh> m_mesh;
	std::array<Ref<InstancedMeshList>, 6 * 48> m_meshList;
	std::array<Ref<InstancedMeshList>, 6 * 48> m_dentMeshList;
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
	static const AutoTileInfo PiledAutoTileTable[48];

	static const int PiledAutoBlockOffset = 100;

	static int autoTileKindId(int tileId) { return (tileId < AutoTileOffset) ? -1 : ((tileId - AutoTileOffset) / AutoTileSetStride); }
	static int localIdToGlobalId(int localTileId, int autotileKind) { return AutoTileSetStride * autotileKind + localTileId + AutoTileOffset; }

	const Ref<MeshAutoTileset>& autotileSet(int akid) const { return m_autotileSet[akid]; }

LN_CONSTRUCT_ACCESS:
    MeshTileset();
	void init();

private:
	void beginBatch();
	void drawTile(RenderingContext* context, const detail::MeshTile& tile, const detail::MeshTileFaceAdjacency& adjacency, const Matrix& transform, int animationFrame) const;
	void drawBatch(RenderingContext* context);

	Ref<Material> m_material;
	std::array<Ref<MeshAutoTileset>, 16> m_autotileSet;


	friend class MeshTilemapLayer;
};

namespace detail {

enum SubtileCorner
{
	SubtileCorner_TopLeft = 0,
	SubtileCorner_TopRight = 1,
	SubtileCorner_BottomLeft = 2,
	SubtileCorner_BottomRight = 3,
};

class MeshAutoTilesetUVMapper
{
public:
	enum class Format
	{
		XP,
		//MV,
		MVWithWall,
		MVFloor,
	};

	MeshAutoTilesetUVMapper(const Size& textureSize, const Rect& sourceRect, Format format);

	// 
	Rect getUVRectFromLocalId(MeshTileFaceDirection direction, int autotileLocalId, SubtileCorner corner, bool beveled = false) const;

private:
	Format m_format;
	Vector2 m_globalUVOffset;
	Vector2 m_subtileUVSize;
};

} // namespace detail
} // namespace ln
