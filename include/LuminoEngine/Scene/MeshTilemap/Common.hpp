
#pragma once
#include "../../Graphics/GeometryStructs.hpp"

namespace ln {
class Texture;
class Material;
class Mesh;
class RenderingContext;
class MeshTileset;
class MeshTilemapModel;
class MeshTilemapLayer;

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

struct MeshTile
{
	// 2D Tilemap と同様のルールの Id. 
	int tileId = 0;

	// FaceDirection. tileId が Autotile の場合のみ使用される.
	uint8_t id[6] = {};
};
} // namespace detail
} // namespace ln
