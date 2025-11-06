#include "Internal.hpp"
#include <LuminoEngine/Mesh/Mesh.hpp>

namespace ln {

Ref<Mesh> Mesh::create() {
    Ref<Mesh> ref(LN_NEW Mesh(), false);
    return ref;
}

} // namespace ln
