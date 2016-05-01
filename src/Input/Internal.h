
#pragma once
#include "../Internal.h"

LN_NAMESPACE_BEGIN
namespace detail
{
class InputManager;

InputManager* GetInputManager(InputManager* priority);

} // namespace detail
LN_NAMESPACE_END
