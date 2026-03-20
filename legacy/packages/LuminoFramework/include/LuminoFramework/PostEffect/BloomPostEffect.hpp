#pragma once
#include "PostEffect.hpp"

namespace ln {
class SamplerState;
namespace detail { class BloomPostEffectInstance; }

class BloomPostEffect
    : public PostEffect
{
public:
    static Ref<BloomPostEffect> create();

    /** ブルームに影響する輝度の閾値を設定します。値が小さいほど、シーン全体がブルームに影響するようになります。(default: 0.9) */
    void setThreshold(float value);

    /** ブルームの色の強さを設定します。 (default: 1.0) */
    void setStrength(float value);

    /** ブルームの広がり（ぼかし）の強さを設定します。 (default: 1.0) */
    void setSize(float value);

protected:
    virtual void onUpdateFrame(float elapsedSeconds) override;
    virtual Ref<PostEffectInstance> onCreateInstance() override;
    //virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    BloomPostEffect();
    void init();

private:
    float m_luminosityThreshold;
    float m_bloomStrength;
    float m_bloomRadius;

    friend class detail::BloomPostEffectInstance;
};


namespace detail {

// cbuffer BloomCompositeParams
struct BloomCompositeParams
{
    /* [0] */   Vector4 _BloomTintColorsAndFactors[8];
    /* [128] */ float _BloomStrength;
    /* [132] */ float _BloomRadius;
};  /* size:136 */

// Filmic でも使いたいのでクラス化したもの
class BloomPostEffectCore
{
public:
    BloomPostEffectCore();
    bool init(Material* compositeMaterial);

    void setLuminosityThreshold(float value) { m_luminosityThreshold = value; }
    void setBloomStrength(float value) { m_bloomStrength = value; }
    void setBloomRadius(float value) { m_bloomRadius = value; }

    void prepare(CommandList* context, RenderTargetTexture* source);
    void render(CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination);

private:
    void resetResources(int resx, int resy);

    float m_luminosityThreshold;
    float m_bloomStrength;
    float m_bloomRadius;

    BloomCompositeParams m_bloomCompositeParams;

    Ref<RenderTargetTexture> m_renderTargetBright;
    List<Ref<RenderTargetTexture>> m_renderTargetsHorizontal;
    List<Ref<RenderTargetTexture>> m_renderTargetsVertical;
    Ref<SamplerState> m_samplerState;
    Ref<Material> m_compositeMaterial;
    int m_viewWidth;
    int m_viewHeight;

    List<Ref<Material>> m_separableBlurMaterialsH;
    List<Ref<Material>> m_separableBlurMaterialsV;
    Ref<Material> m_materialHighPassFilter;
};

class BloomPostEffectInstance
    : public PostEffectInstance
{
protected:
    bool onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    BloomPostEffectInstance();
    bool init(BloomPostEffect* owner);

private:
    BloomPostEffect* m_owner;
    BloomPostEffectCore m_effect;
};

} // namespace detail
} // namespace ln

