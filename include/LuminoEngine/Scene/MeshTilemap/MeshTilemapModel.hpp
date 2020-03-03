
#pragma once
#include "Common.hpp"
#include "MeshTileset.hpp"

namespace ln {

class MeshTilemapModel
	: public Object
{
    LN_OBJECT;
public:

	void draw(RenderingContext* context, const Matrix& transform);

LN_CONSTRUCT_ACCESS:
    MeshTilemapModel();
	void init();

private:
};

} // namespace ln
