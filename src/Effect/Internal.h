
#pragma once
#include "../Internal.h"

LN_NAMESPACE_BEGIN
namespace detail
{
class EffectManager;

EffectManager* GetEffectManager(EffectManager* priority);

} // namespace detail
LN_NAMESPACE_END
