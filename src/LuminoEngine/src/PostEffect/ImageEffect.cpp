
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/PostEffect/ImageEffect.hpp>

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

void ImageEffect::onDispose(bool explicitDisposing)
{
    //for (auto& i : m_instances) {
    //    i->m_ownerDisposed = true;
    //}

    Object::onDispose(explicitDisposing);
}

Ref<ImageEffectInstance> ImageEffect::createInstance()
{
    auto i = onCreateInstance();
    i->m_owner = this;
    //m_instances.add(i);
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

