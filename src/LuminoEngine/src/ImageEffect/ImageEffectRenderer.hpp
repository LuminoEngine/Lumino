#pragma once

namespace ln {
class RenderTargetTexture;
class RenderingContext;
class ImageEffect;
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
    void render(RenderingContext* context, RenderTargetTexture* inout);

private:
    RenderingManager* m_manager;
    List<Ref<ImageEffect>> m_imageEffects;
    Ref<Material> m_blitMaterial;
};

} // namespace detail
} // namespace ln

