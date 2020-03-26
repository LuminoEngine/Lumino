#pragma once

namespace ln {
class RenderTargetTexture;
class RenderingContext;
class ImageEffect;
using ImageEffectInstanceRenderer = ImageEffect;
class Material;
namespace detail {

class ImageEffectRenderer
	: public RefObject
{
public:
    ImageEffectRenderer();
    void addImageEffect(ImageEffect* effect);
    void removeImageEffect(ImageEffect* effect);
    void updateFrame(float elapsedSeconds);
    void applyInSceneImageEffects(const List<ImageEffect*>& imageEffects);
    void render(RenderingContext* context, RenderTargetTexture* inout);

private:
    RenderingManager* m_manager;
    List<Ref<ImageEffect>> m_imageEffects;
    List<Ref<ImageEffectInstanceRenderer>> m_imageEffectRenderers;
	Ref<Material> m_copyMaterial;
};

} // namespace detail
} // namespace ln

