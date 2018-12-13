
#pragma once
#include "../Visual/VisualComponent.hpp"

namespace ln {
class Material;
class TilemapModel;

class TilemapComponent
	: public VisualComponent
{
public:

protected:
    void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    TilemapComponent();
	virtual ~TilemapComponent();
	void initialize();

private:
    Ref<TilemapModel> m_tilemap;
};

} // namespace ln
