
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

void ImageEffect::updateFrame(float elapsedSeconds)
{
    onUpdateFrame(elapsedSeconds);
}

void ImageEffect::onUpdateFrame(float elapsedSeconds)
{
}

} // namespace ln

