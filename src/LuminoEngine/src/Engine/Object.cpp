
#include "Internal.hpp"
#include <Lumino/Engine/Object.hpp>

namespace ln {

//==============================================================================
// Object

Object::Object()
{
}

Object::~Object()
{
}

void Object::finalize()
{
	dispose();
}

void Object::dispose()
{
}

} // namespace ln

