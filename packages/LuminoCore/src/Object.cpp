#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/runtime/ObjectRegistry.hpp>
#include <LuminoCore/Object.hpp>

namespace ln {

Object::~Object() {
    CoreInstance::instance()->objectRegistry()->unregister(this);
}

} // namespace ln
