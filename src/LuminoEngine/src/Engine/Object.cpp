
#include "Internal.hpp"
#include <LuminoEngine/Engine/Object.hpp>
#include <LuminoEngine/Engine/Property.hpp>

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
    TypeInfo::initializeObjectProperties(this);
}

void Object::finalize()
{
	dispose();
}

void Object::dispose()
{
}

TypeInfo* Object::_lnref_getThisTypeInfo() const
{
    return nullptr;
}

} // namespace ln

