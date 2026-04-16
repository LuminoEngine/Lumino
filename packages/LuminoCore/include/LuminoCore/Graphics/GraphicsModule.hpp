#pragma once
#include "rhi/Rhi.hpp"
#include "ShaderPass.hpp"
#include <array>
#include <string>
#include <vector>

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

    /** Cached builtin ShaderPasses (all passes of a built-in shader). */
    const std::vector<Ref<ShaderPass>>& builtinShaderPasses(BuiltinShader id) const {
        return m_builtinShaders[static_cast<int>(id)];
    }

    /** Default (primary) ShaderPass of a builtin shader — first pass registered (usually "Forward"). */
    const Ref<ShaderPass>& builtinShader(BuiltinShader id) const {
        return m_builtinShaders[static_cast<int>(id)].front();
    }

    /** Shared default white texture. */
    const Ref<rhi::Texture>& whiteTexture() const { return m_whiteTexture; }

private:
    GraphicsModule();
    VoidResult init(const Settings& settings);
    VoidResult initBuiltinShader(BuiltinShader id, const unsigned char* data, size_t size);

    Ref<rhi::Device> m_device;
    // Each builtin shader holds a list of ShaderPasses (e.g. {Forward, GBuffer, ...}).
    std::array<std::vector<Ref<ShaderPass>>, 3> m_builtinShaders;
    Ref<rhi::Texture> m_whiteTexture;

};

} // namespace ln
