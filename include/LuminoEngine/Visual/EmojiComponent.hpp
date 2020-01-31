
#pragma once
#include "VisualComponent.hpp"

namespace ln {

class EmojiComponent
	: public VisualComponent
{
public:

protected:
	void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	EmojiComponent();
	bool init();

};

} // namespace ln
