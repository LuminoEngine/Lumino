
#include "Internal.hpp"
#include "AssetManager.hpp"
#include <LuminoEngine/Asset/AssetObject.hpp>

namespace ln {

//=============================================================================
// AssetObject

LN_OBJECT_IMPLEMENT(AssetObject, Object);

AssetObject::AssetObject()
    : m_parent(nullptr)
    , m_children()
{
}

void AssetObject::init()
{
    Object::init();
}

} // namespace ln

