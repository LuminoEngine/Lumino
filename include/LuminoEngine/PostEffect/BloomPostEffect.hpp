#pragma once
#include "PostEffect.hpp"

namespace ln {
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

class BloomPostEffectInstance
    : public PostEffectInstance
{
protected:
    bool onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    BloomPostEffectInstance();
    bool init(BloomPostEffect* owner);

private:
    void resetResources(int resx, int resy);

    BloomPostEffect* m_owner;

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

} // namespace detail
} // namespace ln

