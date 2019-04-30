
#pragma once
#include "../Visual/VisualComponent.hpp"

namespace ln {
class Material;
class TilemapModel;

class TilemapComponent
	: public VisualComponent
{
public:
    void setTilemapModel(TilemapModel* tilemapModel);

protected:
    void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    TilemapComponent();
	virtual ~TilemapComponent();
	void init();

private:
    Ref<TilemapModel> m_tilemap;
};

} // namespace ln
