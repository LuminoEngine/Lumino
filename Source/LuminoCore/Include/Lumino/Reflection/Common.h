
#pragma once
#include "../Base/Common.h"

LN_NAMESPACE_BEGIN
namespace tr
{
class TypeInfo;
class ReflectionObject;
class ReflectionEventInfo;
class ReflectionEventBase;
class ReflectionEventArgs;
class PropertyMetadata;
class PropertyChangedEventArgs;
struct PropertyInstanceData;

enum class PropertySetSource
{
	Default = 0,
	ByInherit = 1,
	ByLocal = 2,
	ByAnimation = 3,
};

} // namespace tr
LN_NAMESPACE_END
