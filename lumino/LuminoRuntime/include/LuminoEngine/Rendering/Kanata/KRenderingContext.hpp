#pragma once
#include "Common.hpp"

namespace ln {
namespace kanata {

class PartRenderingContext : public URefObject {
public:
    PartRenderingContext(detail::RenderingManager* manager);

    void setRenderPass(RenderPass* value);
    void setRenderPass(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);



private:
    URef<BatchCollector> m_collector;
};


class RenderingContext : public URefObject {
public:
    RenderingContext(detail::RenderingManager* manager);

    //PartRenderingContext* partContext(RenderPart part) const { return m_partContext[(int)part]; }

private:
    std::array<URef<PartRenderingContext>, (int)RenderPart::_Count> m_partContext;
};

} // namespace kanata
} // namespace ln

