#pragma once
#include "Common.hpp"

namespace ln {

class DebugPrint : public URefObject {
public:
    DebugPrint(BatchRenderer* batchRenderer);
    ~DebugPrint() override;
    MaybeResult init();
    void dispose();
    void print(CommandList* commandList, const std::string_view& text);

private:
    BatchRenderer* m_batchRenderer;
    Ref<Texture2D> m_fontTexture;
    Ref<Material> m_material;
};

} // namespace ln

