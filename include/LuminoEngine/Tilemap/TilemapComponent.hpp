
#pragma once
#include "../Visual/VisualComponent.hpp"

namespace ln {
class Material;

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
    Ref<Material> m_material;
};

} // namespace ln
