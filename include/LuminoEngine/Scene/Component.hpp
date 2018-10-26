
#pragma once
#include "Common.hpp"

namespace ln {

class Component
	: public Object
{
protected:

LN_CONSTRUCT_ACCESS:
	Component();
	virtual ~Component();
	void initialize();

private:
};

} // namespace ln
