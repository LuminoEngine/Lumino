#include "Internal.hpp"
#include <LuminoEngine/Runtime/Runtime.hpp>
#include <LuminoEngine/Runtime/detail/RuntimeManager.hpp>

namespace ln {
 
LNHandle Runtime::wrapObject(Object* obj, bool fromCreate) {
    return detail::RuntimeManager::instance()->makeObjectWrap(obj, fromCreate);
}

Object* Runtime::getObject(LNHandle handle) {
    return detail::RuntimeManager::instance()->getObjectEntry(handle)->object;
}

} // namespace ln
