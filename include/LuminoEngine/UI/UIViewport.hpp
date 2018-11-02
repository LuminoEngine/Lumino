#pragma once
#include "UIContainerElement.hpp"

namespace ln {

class UIViewport
	: public UIContainerElement
{
public:
    UIViewport();
	void initialize();

protected:
	virtual void onRender(UIRenderingContext* context);

private:
};

} // namespace ln

