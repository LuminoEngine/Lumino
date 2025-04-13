#pragma once
#include "Common.hpp"
#include "Kanata/KUnlitRenderPass.hpp"

namespace ln {

class DebugPrint : public URefObject {
public:
    DebugPrint(detail::RenderingManager* manager, BatchRenderer* batchRenderer);
    ~DebugPrint() override;
    MaybeResult init();
    void dispose();
    void print(const std::string_view& text);
    MaybeResult render(SurfaceContext* surfaceContext, CommandList* commandList);

private:
    detail::RenderingManager* m_manager;
    BatchRenderer* m_batchRenderer;
    Ref<Texture2D> m_fontTexture;
    Ref<Material> m_material;
    std::array<char, 512> m_textBuffer;
    int32_t m_textBufferUsed;
    Ref<kanata::UnlitRenderPass> m_renderPass;
    Ref<RenderViewPoint> m_viewPoint;
};

} // namespace ln

