#include <LuminoGraphics/RHI/VertexLayout.hpp>
#include "RenderingManager2.hpp"

namespace ln {
namespace detail {

//==============================================================================
// RenderingManager2

RenderingManager2::RenderingManager2()
    : m_standardVertexLayout(nullptr) {
}

Result RenderingManager2::init(const Settings& settings) {
    LN_LOG_DEBUG("RenderingManager2 Initialization started.");

    static VertexElement elements[] = {
        { 0, VertexElementType::Float4, VertexElementUsage::Position, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::Normal, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::TexCoord, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::Color, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::Tangent, 0 },
    };
    m_standardVertexLayout = makeObject<VertexLayout>(elements, LN_ARRAY_SIZE_OF(elements));

    LN_LOG_DEBUG("RenderingManager2 Initialization finished.");
    return ok();
}

void RenderingManager2::dispose() {
    LN_LOG_DEBUG("RenderingManager2 dispose started.");

    if (m_standardVertexLayout) {
        m_standardVertexLayout->dispose();
        m_standardVertexLayout = nullptr;
    }

    LN_LOG_DEBUG("RenderingManager2 dispose finished.");
}

} // namespace detail
} // namespace ln
