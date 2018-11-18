
#pragma once
#include "Common.hpp"
#include "../Rendering/RenderingContext.hpp"
#include "../Scene/Component.hpp"

namespace ln {

class VisualComponent
	: public Component
{
    LN_OBJECT;
protected:

LN_CONSTRUCT_ACCESS:
    VisualComponent();
	virtual ~VisualComponent();
	void initialize();

private:
    virtual void render(RenderingContext* context) override;
};

} // namespace ln
