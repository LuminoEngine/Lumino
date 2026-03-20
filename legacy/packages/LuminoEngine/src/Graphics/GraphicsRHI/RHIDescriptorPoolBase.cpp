#include <LuminoEngine/Graphics/GraphicsRHI/GraphicsDeviceContext.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/RHIProfiler.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/RHIDescriptorPoolBase.hpp>

namespace ln {
namespace detail {

IDescriptorPool::~IDescriptorPool() {
    if (IGraphicsDevice* d = device()) {
        d->profiler()->removeDescriptorPool(this);
    }
}

} // namespace detail
} // namespace ln
