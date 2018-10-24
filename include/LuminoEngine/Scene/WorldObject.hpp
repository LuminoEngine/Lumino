
#pragma once
#include "Common.hpp"

namespace ln {

class WorldObject
	: public Object
{
protected:

LN_CONSTRUCT_ACCESS:
	WorldObject();
	virtual ~WorldObject();
	void initialize();

private:
};

} // namespace ln
