#pragma once
#include "PostEffect.hpp"

namespace ln {
class SamplerState;
class ShaderParameter2;
namespace detail { class SSRPostEffectInstance; }

class SSRPostEffect
    : public PostEffect
{
public:

protected:
    virtual Ref<PostEffectInstance> onCreateInstance() override;

LN_CONSTRUCT_ACCESS:
    SSRPostEffect();
    void init();

private:
    friend class detail::SSRPostEffectInstance;
};

namespace detail {


// Filmic でも使いたいのでクラス化したもの
class SSRPostEffectCore
{
public:
    SSRPostEffectCore();
    bool init(Material* compositeMaterial);
    bool prepare(RenderView* renderView, CommandList* context, RenderTargetTexture* source);
    void render(CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination);

    Ref<RenderTargetTexture> ssrResultTexture() const { return m_blurTarget2; }

private:
    void resetResources(int resx, int resy);

    Ref<Material> m_ssrMaterial;
    Ref<Material> m_ssrBlurMaterial1;
    Ref<Material> m_ssrBlurMaterial2;
    Ref<Material> m_ssrCompositeMaterial;

    int m_viewWidth;
    int m_viewHeight;
    Ref<RenderTargetTexture> m_ssrTarget;
    Ref<RenderTargetTexture> m_blurTarget1;
    Ref<RenderTargetTexture> m_blurTarget2;
    Ref<RenderTargetTexture> m_compositeTarget;
    Ref<SamplerState> m_samplerState;

    // Find optimization
    ShaderParameter2* m_ssrMaterial_ColorSampler;
    ShaderParameter2* m_ssrMaterial_NormalAndDepthSampler;
    ShaderParameter2* m_ssrMaterial_ViewDepthSampler;
    ShaderParameter2* m_ssrMaterial_MetalRoughSampler;
    ShaderParameter2* m_paramColorSampler;
    ShaderParameter2* m_paramSSRSampler;
};

class SSRPostEffectInstance
    : public PostEffectInstance
{
protected:
    bool onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    SSRPostEffectInstance();
    bool init(SSRPostEffect* owner);

private:
    SSRPostEffect* m_owner;
    SSRPostEffectCore m_effect;
};

} // namespace detail
} // namespace ln

