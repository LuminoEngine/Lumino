#pragma once

namespace ln {
class RenderTargetTexture;
class RenderingContext;
class ImageEffect;
using ImageEffectInstanceRenderer = ImageEffect;
class ImageEffectInstance;
namespace detail {

class ImageEffectRenderer
	: public RefObject
{
public:
    ImageEffectRenderer();
    void dispose();
    void addImageEffect(ImageEffect* effect);
    void removeImageEffect(ImageEffect* effect);
    void updateFrame(float elapsedSeconds);
    void applyInSceneImageEffects(const List<ImageEffect*>& imageEffects);
    void render(RenderingContext* context, RenderTargetTexture* inout);

private:
    struct Instance
    {
        Ref<ImageEffectInstance> instance;
        bool found = false;
    };

    void refreshImageEffectInstances();

    RenderingManager* m_manager;
    List<Ref<ImageEffect>> m_imageEffects;
    //List<Ref<ImageEffectInstanceRenderer>> m_imageEffectRenderers;

    List<Instance> m_imageEffectInstances;
    List<Instance> m_collectedImageEffectInstances;
	Ref<AbstractMaterial> m_copyMaterial;
};

} // namespace detail
} // namespace ln

