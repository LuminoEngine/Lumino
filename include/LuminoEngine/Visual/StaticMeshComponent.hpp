
#pragma once
#include "VisualComponent.hpp"

namespace ln {

class StaticMeshComponent
	: public VisualComponent
{
public:

protected:
    void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    StaticMeshComponent();
	virtual ~StaticMeshComponent();
	void initialize();

private:
};

} // namespace ln
