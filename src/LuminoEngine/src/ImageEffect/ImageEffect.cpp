
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/ImageEffect/ImageEffect.hpp>

namespace ln {

//==============================================================================
// ImageEffect

ImageEffect::ImageEffect()
{
}

ImageEffect::~ImageEffect()
{
}

void ImageEffect::init()
{
    Object::init();
}

Ref<ImageEffectInstance> ImageEffect::createInstance()
{
    auto i = onCreateInstance();
    i->m_owner = this;
    return i;
}

void ImageEffect::updateFrame(float elapsedSeconds)
{
    onUpdateFrame(elapsedSeconds);
}

void ImageEffect::onUpdateFrame(float elapsedSeconds)
{
}

//void ImageEffect::onPostRender()
//{
//}

} // namespace ln

