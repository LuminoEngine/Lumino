
#pragma once
#include "WorldObject.hpp"

namespace ln {
class RenderingContext;

class TransformControls
	: public WorldObject
{
public:

protected:
	virtual void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	TransformControls();
	virtual ~TransformControls() = default;
	void init();

private:
};

} // namespace ln
