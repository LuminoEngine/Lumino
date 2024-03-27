#include <LuminoEngine/Rendering/Kanata/KRenderingContext.hpp>
#include <LuminoEngine/Rendering/Kanata/KRenderView.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>

namespace ln {
namespace kanata {

RenderView::RenderView(detail::RenderingManager* manager)
    : m_renderingContext(makeURef<RenderingContext>(manager)) {
}

} // namespace kanata
} // namespace ln

