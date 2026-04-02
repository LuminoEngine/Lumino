#pragma once
#include "rhi/Rhi.hpp"
#include <array>
#include <string>

namespace ln {

enum class BuiltinShader {
    Unlit = 0,
    BasicLit = 1,
    StencilMask = 2,
};

struct BuiltinShaderData {
    Ref<rhi::ShaderModule> vertShader;
    Ref<rhi::ShaderModule> fragShader;
    std::string vertEntry;
    std::string fragEntry;
    Ref<rhi::BindGroupLayout> materialBindGroupLayout;
    u64 materialParamBufferSize = 0;
    bool initialized = false;
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

    /** Cached builtin shader data. */
    const BuiltinShaderData& builtinShader(BuiltinShader id) const { return m_builtinShaders[static_cast<int>(id)]; }

    /** Shared default white texture. */
    const Ref<rhi::Texture>& whiteTexture() const { return m_whiteTexture; }

private:
    GraphicsModule();
    VoidResult init(const Settings& settings);
    VoidResult initBuiltinShader(BuiltinShader id, const unsigned char* data, size_t size);

    Ref<rhi::Device> m_device;
    std::array<BuiltinShaderData, 3> m_builtinShaders;
    Ref<rhi::Texture> m_whiteTexture;
};

} // namespace ln
