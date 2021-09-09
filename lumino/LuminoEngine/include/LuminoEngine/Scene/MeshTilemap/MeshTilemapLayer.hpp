
#pragma once
#include "Common.hpp"

namespace ln {

class MeshTilemapLayer
    : public Object
{
    LN_OBJECT;
public:

LN_CONSTRUCT_ACCESS:
    MeshTilemapLayer();
    void init();

private:
    void draw(RenderingContext* context, MeshTileset* tileset);

    friend class MeshTilemapModel;
};

} // namespace ln
