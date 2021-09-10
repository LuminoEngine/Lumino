#pragma once
#include "Common.hpp"
#include <LuminoEngine/Asset/AssetObject.hpp>

namespace ln {
class RenderingContext;

/**
 * EffectResource
 */
LN_CLASS()
class EffectResource
    : public AssetObject
{
public:

//protected:
//    virtual void onDraw(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    EffectResource();
    virtual ~EffectResource();
    bool init();

private:
};


// EffectResource に対するインスタンス
class EffectEmitter
    : public Object
{
public:
    void setTransform(const Matrix& value) { m_transform = value; }
    const Matrix& transform() const { return m_transform; }

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
    Matrix m_transform;
    float m_localTime;
};


} // namespace ln
