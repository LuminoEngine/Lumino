
#pragma once
#include "../Graphics/GeometryStructs.hpp"
#include "VisualComponent.hpp"

namespace ln {

class SphereComponent
	: public VisualComponent
{
public:

protected:
    void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    SphereComponent();
	virtual ~SphereComponent() = default;
	void initialize();

private:
};

} // namespace ln
