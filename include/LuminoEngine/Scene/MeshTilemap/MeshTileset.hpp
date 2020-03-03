
#pragma once
#include "Common.hpp"

namespace ln {

class AutoMeshTileset
	: public Object
{
public:
	
LN_CONSTRUCT_ACCESS:
    AutoMeshTileset();
	void init();
};

class MeshTileset
	: public Object
{
    LN_OBJECT;
public:
	static const int AutoTileOffset = 16384;
	static const int AutoTileSetStride = 1024;
	static const int AutoTileUnitStride = 128;
	static const int MaxAutoTilests = AutoTileOffset / AutoTileSetStride;
	static const int LocalAutoTileIdSlopeOffset = 96;

LN_CONSTRUCT_ACCESS:
    MeshTileset();
	void init();

private:
};

} // namespace ln
