
#pragma once
#include "Common.hpp"

namespace ln {
class RenderingContext;

// EffectModel に対するインスタンス
class EffectEmitter
    : public Object
{
public:

public: // TODO: internal
    virtual void update(float elapsedSeconds);
    virtual void render(RenderingContext* renderingContext);

LN_INTERNAL_NEW_OBJECT:
    EffectEmitter();
    virtual ~EffectEmitter();
    void init(EffectModel* model);

private:
    EffectModel* m_model;
};

class Effect
{
public:
    // mainWorld の指定位置にエフェクトを発生させる。
    // 戻り値はコントロールしたい場合に保持すればよい。自動解放される。
    static EffectEmitter* emit(EffectModel* effect, const Vector3& position);
};

class EffectContext
    : public Object
{
public:

protected:

public: // TODO: internal
    EffectEmitter* createEmitter(EffectModel* model);
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

} // namespace ln
