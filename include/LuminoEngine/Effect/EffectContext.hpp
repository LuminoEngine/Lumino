
#pragma once
#include "Common.hpp"

namespace ln {

class EffectContext
    : public Object
{
public:

protected:

LN_CONSTRUCT_ACCESS:
    EffectContext();
    virtual ~EffectContext();
    void init();

private:
    detail::EffectManager* m_manager;
};

} // namespace ln
