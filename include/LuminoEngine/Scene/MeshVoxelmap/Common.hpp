
#pragma once
#include "../../Graphics/GeometryStructs.hpp"

namespace ln {
class Texture;
class Material;
class MeshPrimitive;
class RenderingContext;
class MeshVoxelset;
class MeshVoxelmapModel;
class MeshVoxelmapLayer;

enum class MeshTileFaceDirection
{
	XMinus,
	XPlus,
	YMinus,
	YPlus,
	ZMinus,
	ZPlus,
};

namespace detail {

enum SubtileCorner
{
	SubtileCorner_TopLeft = 0,
	SubtileCorner_TopRight = 1,
	SubtileCorner_BottomLeft = 2,
	SubtileCorner_BottomRight = 3,
};

struct MeshTile
{
	// 2D Tilemap と同様のルールの Id. 
	int tileId = 0;

	// FaceDirection. tileId が Autotile の場合のみ使用される.
	uint8_t faceTileId[6] = {};
};

struct MeshTileFaceAdjacency
{
	enum State
	{
		BURIED = 0, // 面方向に接続しているが、全面埋まっており、描画は必要ない
		//CONNECTED = 1,	// 面方向に接続しており、押し込みがあるため描画が必要
		OPEN = 2,	// 通常描画
	};


	State state[6]; // MeshTileFaceDirection
	//int tileIds[27];
};

} // namespace detail
} // namespace ln
