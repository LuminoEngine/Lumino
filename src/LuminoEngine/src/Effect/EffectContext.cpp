
#include "Internal.hpp"
#include <LuminoEngine/Effect/EffectContext.hpp>

namespace ln {

//==============================================================================
// EffectContext

EffectContext::EffectContext()
    : m_manager(nullptr)
{
}

EffectContext::~EffectContext()
{
}

void EffectContext::init()
{
    Object::init();
}

} // namespace ln
