
#pragma once
#include "Common.hpp"

namespace ln {

class Tileset
	: public Object
{

LN_CONSTRUCT_ACCESS:
    Tileset();
	virtual ~Tileset();
	void initialize();

private:
};

} // namespace ln
