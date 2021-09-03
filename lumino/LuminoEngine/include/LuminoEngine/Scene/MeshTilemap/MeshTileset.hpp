
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
	bool init();

private:
};

class MeshTileset
	: public Object
{
    LN_OBJECT;
public:

LN_CONSTRUCT_ACCESS:
    MeshTileset();
	bool init();

private:
};

} // namespace ln
