#pragma once

namespace ln {
class RenderTargetTexture;
class RenderingContext;
class PostEffect;
using PostEffectInstanceRenderer = PostEffect;
class PostEffectInstance;
namespace detail {

class PostEffectRenderer
	: public RefObject
{
public:
    PostEffectRenderer();
    void dispose();
    void addPostEffect(PostEffect* effect);
    void removePostEffect(PostEffect* effect);
    void updateFrame(float elapsedSeconds);
    void applyInScenePostEffects(const List<PostEffect*>& imageEffects);
    void render(RenderingContext* context, RenderTargetTexture* input, RenderTargetTexture* output);

private:
    struct Instance
    {
        Ref<PostEffectInstance> instance;
        bool found = false;
    };

    void refreshPostEffectInstances();

    RenderingManager* m_manager;
    List<Ref<PostEffect>> m_imageEffects;
    //List<Ref<PostEffectInstanceRenderer>> m_imageEffectRenderers;

    List<Instance> m_imageEffectInstances;
    List<Instance> m_collectedPostEffectInstances;
	Ref<Material> m_copyMaterial;
};

} // namespace detail
} // namespace ln

