
#pragma once
#include "Common.hpp"
#include "MeshVoxelset.hpp"

namespace ln {

class MeshVoxelmapModel
	: public Object
{
    LN_OBJECT;
public:
	void setTileset(MeshVoxelset* tileset);
	void addLayer(MeshVoxelmapLayer* layer);

	void draw(RenderingContext* context, const Matrix& transform);

LN_CONSTRUCT_ACCESS:
    MeshVoxelmapModel();
	void init();

private:
	Ref<MeshVoxelset> m_tileset;
	List<Ref<MeshVoxelmapLayer>> m_layers;
};

} // namespace ln
