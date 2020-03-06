
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
	uint8_t id[6] = {};  // FaceDirection. Non Autotile の場合は [0] だけ使用する
};
} // namespace detail
} // namespace ln
