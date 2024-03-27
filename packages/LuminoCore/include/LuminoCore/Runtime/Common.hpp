#pragma once
#include "../Base/RefObject.hpp"
#include "../Base/String.hpp"
#include "../Base/Result.hpp"
#include "../Base/Logger.hpp"

namespace ln {
class TypeInfo;
class Variant;
class Archive;

enum class TypeInfoClass {
    Primitive,
    /*Struct,*/
    Object,
};

} // namespace ln
