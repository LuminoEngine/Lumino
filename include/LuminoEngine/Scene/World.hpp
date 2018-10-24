
#pragma once
#include "Common.hpp"

namespace ln {

class World
	: public Object
{
protected:

LN_CONSTRUCT_ACCESS:
	World();
	virtual ~World();
	void initialize();

private:
};

} // namespace ln
