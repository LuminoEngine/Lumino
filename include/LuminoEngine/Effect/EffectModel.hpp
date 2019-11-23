#pragma once
#include "Common.hpp"

namespace ln {
class RenderingContext;

class EffectResource
    : public Object
{
public:

//protected:
//    virtual void onDraw(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    EffectResource();
    virtual ~EffectResource();
    void init();

private:
};


// EffectResource に対するインスタンス
class EffectEmitter
    : public Object
{
public:
    void setPosition(const Vector3& value) { m_position = value; }
    const Vector3& position() const { return m_position; }

public: // TODO: internal
    bool update(float elapsedSeconds);
    void render(RenderingContext* renderingContext);

protected:
    virtual bool onUpdate(float localTime, float elapsedSeconds);
    virtual void onRender(RenderingContext* renderingContext);


public: // TODO:
    EffectEmitter();
    virtual ~EffectEmitter();
    void init(EffectResource* model);

private:
    EffectResource* m_model;
    Vector3 m_position;
    float m_localTime;
};


} // namespace ln
