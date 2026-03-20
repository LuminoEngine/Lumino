#include <LuminoEngine/Graphics/GraphicsRHI/RHIProfiler.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/RHISamplerStateBase.hpp>

namespace ln {
namespace detail {

ISamplerState::ISamplerState() {
    LN_LOG_VERBOSE("ISamplerState [0x{:x}] constructed.", (intptr_t)this);
}

ISamplerState::~ISamplerState() {
    if (IGraphicsDevice* d = device()) {
        d->profiler()->removeSamplerState(this);
    }
}

} // namespace detail
} // namespace ln
