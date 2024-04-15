#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoEngine/Graphics/RenderState.hpp>
#include "Common.hpp"

namespace ln {
namespace kanata {

struct PipelineState final {
    BlendStateDesc blendState;
    RasterizerStateDesc rasterizerState;
    DepthStencilStateDesc depthStencilState;
    VertexLayout* vertexLayout;
    PrimitiveTopology primitiveTopology;
};

} // namespace kanata
} // namespace ln

