
#pragma once
#include "Common.hpp"
#include "MeshTileset.hpp"

namespace ln {

class MeshTilemapModel
	: public Object
{
    LN_OBJECT;
public:
	void setTileset(MeshTileset* tileset);
	void addLayer(MeshTilemapLayer* layer);

	void draw(RenderingContext* context, const Matrix& transform);

LN_CONSTRUCT_ACCESS:
	MeshTilemapModel();
	Result<> init();

private:
	Ref<MeshTileset> m_tileset;
	List<Ref<MeshTilemapLayer>> m_layers;
};

} // namespace ln
