#pragma once
#include "Common.hpp"

namespace ln {

class Runtime
{
public:
	static LnHandle makeObjectWrap(Object* obj);
	static LnResult processException(Exception* e);

private:
};

} // namespace ln
