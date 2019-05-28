
#include "Internal.hpp"
#include "AssetManager.hpp"
#include <LuminoEngine/Asset/AssetObject.hpp>

namespace ln {

//=============================================================================
// AssetObject

LN_OBJECT_IMPLEMENT(AssetObject, Object);

AssetObject::AssetObject()
{
}

void AssetObject::init()
{
    Object::init();
}

} // namespace ln

