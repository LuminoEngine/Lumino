#pragma once
#include "Common.hpp"

namespace ln {
class RenderingContext;

class EffectResource
    : public Object
{
public:

protected:
    virtual void onDraw(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    EffectResource();
    virtual ~EffectResource();
    void init();

private:
};


} // namespace ln
