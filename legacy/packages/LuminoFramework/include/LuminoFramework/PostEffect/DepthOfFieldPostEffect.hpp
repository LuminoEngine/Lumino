#pragma once
#include "PostEffect.hpp"

namespace ln {
namespace detail { class DepthOfFieldPostEffectInstance; }

class DepthOfFieldPostEffect
    : public PostEffect
{
public:

protected:
    virtual Ref<PostEffectInstance> onCreateInstance() override;

LN_CONSTRUCT_ACCESS:
    DepthOfFieldPostEffect();
    void init();

private:
    friend class detail::DepthOfFieldPostEffectInstance;
};

namespace detail {


// Filmic でも使いたいのでクラス化したもの
class DepthOfFieldPostEffectCore
{
public:
    static const int MaxMips = 4;

    DepthOfFieldPostEffectCore();
    bool init(Material* compositeMaterial);
    void prepare(RenderView* renderView, CommandList* context, RenderTargetTexture* source);

private:
    void resetResources(int resx, int resy);

    int m_viewWidth;
    int m_viewHeight;

    // 旧 DOF は 1 枚の大きなテクスチャにすべてのダウンサンプリング結果を書き込んでいたが、
    // そうするとぼかしのためのサンプリング時に境界付近で別のレベルの結果を読み取ってしまい、
    // 境界に変なアーティファクトが現れることがあった。
    // そのため、個別のテクスチャを使うことにした。
    List<Ref<RenderTargetTexture>> m_mipTargets;
    Ref<Material> m_copyMaterial;
    Ref<Material> m_compositeMaterial;
    Ref<SamplerState> m_samplerState;
};

class DepthOfFieldPostEffectInstance
    : public PostEffectInstance
{
protected:
    bool onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    DepthOfFieldPostEffectInstance();
    bool init(DepthOfFieldPostEffect* owner);

private:
    DepthOfFieldPostEffect* m_owner;
    Ref<Material> m_copyMaterial;
    Ref<Material> m_dofMaterial;
    Ref<SamplerState> m_samplerState;
};

} // namespace detail
} // namespace ln

