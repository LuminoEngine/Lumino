
#include "Internal.hpp"
#include <LuminoEngine/Engine/Object.hpp>

namespace ln {

//==============================================================================
// Object

Object::Object()
{
}

Object::~Object()
{
}

void Object::initialize()
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

