#pragma once
#include "Common.hpp"

namespace ln {
namespace kanata {

/**
 * Rendering entory point.
 */
class RenderView : public URefObject {
public:
    RenderView(detail::RenderingManager* manager);



    void build();

    void render();

protected:

private:
    URef<RenderingContext> m_renderingContext;
};

} // namespace kanata
} // namespace ln

