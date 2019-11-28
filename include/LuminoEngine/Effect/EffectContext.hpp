
#pragma once
#include "../Graphics/GeometryStructs.hpp"
#include "Common.hpp"
#include "EffectModel.hpp"

namespace ln {
class RenderingContext;
class SpriteFrameSet;   // TODO:
class Material;   // TODO:

class Effect
{
public:
    // mainWorld の指定位置にエフェクトを発生させる。
    // 戻り値はコントロールしたい場合に保持すればよい。自動解放される。
    static EffectEmitter* emit(EffectResource* effect, const Matrix& transform);

    // for Effekseer
    static EffectEmitter* emit(const Path& filePath, const Matrix& transform);
};

class EffectContext
    : public Object
{
public:

protected:

public: // TODO: internal
    EffectEmitter* createEmitter(EffectResource* model);
    void addEmitter(EffectEmitter* emitter);
    void update(float elapsedSeconds);
    void render(RenderingContext* renderingContext);

LN_CONSTRUCT_ACCESS:
    EffectContext();
    virtual ~EffectContext();
    void init();

private:
    detail::EffectManager* m_manager;
    List<Ref<EffectEmitter>> m_emitters;
};







class SpriteFrameEffectResource
    : public EffectResource
{
public:

public: // TODO: internal

LN_CONSTRUCT_ACCESS:
    SpriteFrameEffectResource();
    virtual ~SpriteFrameEffectResource();
    void init(const Size& spriteSize, SpriteFrameSet* spriteFrameSet, int startNumber, int lastNumber, float frameTime);

public: // TODO: private
    Size m_spriteSize;
    Ref<SpriteFrameSet> m_spriteFrameSet;
    int m_startNumber;
    int m_lastNumber;
    float m_frameTime;
    Ref<Material> m_material;
};

class SpriteFrameEffectEmitter
    : public EffectEmitter
{
protected:
    virtual bool onUpdate(float localTime, float elapsedSeconds) override;
    virtual void onRender(RenderingContext* renderingContext) override;

LN_CONSTRUCT_ACCESS:
    SpriteFrameEffectEmitter();
    virtual ~SpriteFrameEffectEmitter();
    void init(SpriteFrameEffectResource* data);

private:
    Ref<SpriteFrameEffectResource> m_data;
    int m_frameNumber;
    //bool m_finished;
};

} // namespace ln
