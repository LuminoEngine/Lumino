
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

void ImageEffect::initialize()
{
    Object::initialize();
}

void ImageEffect::updateFrame(float elapsedSeconds)
{
    onUpdateFrame(elapsedSeconds);
}

void ImageEffect::onUpdateFrame(float elapsedSeconds)
{
}

} // namespace ln

