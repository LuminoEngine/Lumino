#include <LuminoGraphics/Rendering/Kanata/KRenderingContext.hpp>
#include <LuminoGraphics/Rendering/Kanata/KRenderView.hpp>
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>

namespace ln {
namespace kanata {

RenderView::RenderView(detail::RenderingManager* manager)
    : m_renderingContext(makeURef<RenderingContext>(manager)) {
}

} // namespace kanata
} // namespace ln

