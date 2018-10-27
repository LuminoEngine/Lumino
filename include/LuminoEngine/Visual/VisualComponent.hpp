
#pragma once
#include "Common.hpp"
#include "../Rendering/RenderingContext.hpp"
#include "../Scene/Component.hpp"

namespace ln {

class VisualComponent
	: public Component
{
protected:

LN_CONSTRUCT_ACCESS:
    VisualComponent();
	virtual ~VisualComponent();
	void initialize();

private:
};

} // namespace ln
