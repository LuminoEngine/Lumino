
#pragma once
#include "Common.hpp"

namespace ln {

class WorldRenderView
	: public Object
{
protected:

LN_CONSTRUCT_ACCESS:
    WorldRenderView();
	virtual ~WorldRenderView();
	void initialize();

private:
};

} // namespace ln
