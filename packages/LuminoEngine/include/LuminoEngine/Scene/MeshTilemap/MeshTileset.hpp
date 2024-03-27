
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
    Result<> init();

private:
};

class MeshTileset
	: public Object
{
    LN_OBJECT;
public:

LN_CONSTRUCT_ACCESS:
    MeshTileset();
    Result<> init();

private:
};

} // namespace ln
