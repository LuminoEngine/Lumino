#pragma once
#include "Common.hpp"

namespace ln {
class RenderingContext;

class EffectModel
    : public Object
{
public:

protected:
    virtual void onDraw(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    EffectModel();
    virtual ~EffectModel();
    void init();

private:
};

class SpriteFrameEffectModel
    : public EffectModel
{
public:
    

protected:
    virtual void onDraw(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    SpriteFrameEffectModel();
    virtual ~SpriteFrameEffectModel();
    void init();

private:
};

} // namespace ln
