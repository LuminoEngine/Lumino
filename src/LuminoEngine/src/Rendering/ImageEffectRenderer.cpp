
#include "Internal.hpp"
#include "ImageEffectRenderer.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ImageEffectRenderer

ImageEffectRenderer::ImageEffectRenderer()
    : m_manager(detail::EngineDomain::renderingManager())
{
}

void ImageEffectRenderer::addImageEffect(ImageEffect* effect)
{
    if (LN_REQUIRE(effect)) return;
    m_imageEffects.add(effect);
}

void ImageEffectRenderer::removeImageEffect(ImageEffect* effect)
{
    if (LN_REQUIRE(effect)) return;
    m_imageEffects.remove(effect);
}

void ImageEffectRenderer::render(RenderingContext* context)
{
    if (!m_imageEffects.isEmpty())
    {

    }
}

} // namespace detail
} // namespace ln

