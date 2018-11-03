#pragma once

namespace ln {
class RenderingContext;
class ImageEffect;
namespace detail {

class ImageEffectRenderer
	: public RefObject
{
public:
    ImageEffectRenderer();
    void addImageEffect(ImageEffect* effect);
    void removeImageEffect(ImageEffect* effect);
    void render(RenderingContext* context);

private:
    List<Ref<ImageEffect>> m_imageEffects;
};

} // namespace detail
} // namespace ln

