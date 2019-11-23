#pragma once
#include "Common.hpp"

namespace ln {
class Exception;
class Object;

class Runtime
{
public:
	static LnHandle makeObjectWrap(Object* obj, bool fromCreate);
	static Object* getObject(LnHandle handle);
	static void setManagedObjectId(LnHandle handle, int64_t id);
	static int64_t getManagedObjectId(LnHandle handle);
	static LnResult processException(Exception* e);

private:
};

} // namespace ln
