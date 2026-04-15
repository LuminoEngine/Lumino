#pragma once
#include "rhi/Rhi.hpp"
#include "ShaderPass.hpp"
#include <array>
#include <string>

namespace ln {

enum class BuiltinShader {
    Unlit = 0,
    BasicLit = 1,
    StencilMask = 2,
};

class GraphicsModule : public RefObject {
public:
    struct Settings {
        rhi::Backend preferredBackend = rhi::Backend::Vulkan;
        bool enableValidation = false;
    };
    static Result<Ref<GraphicsModule>> create(const Settings& settings);

    ~GraphicsModule() override;
    void dispose();

    /** The RHI device owned by this instance. */
    rhi::Device* device() const { return m_device.get(); }

    /** Cached builtin ShaderPass. */
    const Ref<ShaderPass>& builtinShader(BuiltinShader id) const { return m_builtinShaders[static_cast<int>(id)]; }

    /** Shared default white texture. */
    const Ref<rhi::Texture>& whiteTexture() const { return m_whiteTexture; }

private:
    GraphicsModule();
    VoidResult init(const Settings& settings);
    VoidResult initBuiltinShader(BuiltinShader id, const unsigned char* data, size_t size);

    Ref<rhi::Device> m_device;
    std::array<Ref<ShaderPass>, 3> m_builtinShaders;
    Ref<rhi::Texture> m_whiteTexture;

};

} // namespace ln
