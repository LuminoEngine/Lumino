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

    /** Shared BindGroupLayoutDesc for view/camera data. */
    const rhi::BindGroupLayoutDesc& viewLayoutDesc() const { return m_viewLayoutDesc; }

    /** Shared BindGroupLayoutDesc for scene/lighting data. */
    const rhi::BindGroupLayoutDesc& sceneLayoutDesc() const { return m_sceneLayoutDesc; }

    /** Shared BindGroupLayoutDesc for per-object data. */
    const rhi::BindGroupLayoutDesc& objectLayoutDesc() const { return m_objectLayoutDesc; }

    /** Per-object UBO size from shader reflection. */
    u64 objectUBOSize() const { return m_objectUBOSize; }

    /** Descriptor set index for view data (from shader reflection). */
    int16_t viewSetIndex() const { return m_viewSetIndex; }

    /** Descriptor set index for scene data (from shader reflection). */
    int16_t sceneSetIndex() const { return m_sceneSetIndex; }

    /** Descriptor set index for object data (from shader reflection). */
    int16_t objectSetIndex() const { return m_objectSetIndex; }

private:
    GraphicsModule();
    VoidResult init(const Settings& settings);
    VoidResult extractSharedLayoutDescs(const unsigned char* data, size_t size);
    VoidResult initBuiltinShader(BuiltinShader id, const unsigned char* data, size_t size);

    Ref<rhi::Device> m_device;
    std::array<Ref<ShaderPass>, 3> m_builtinShaders;
    Ref<rhi::Texture> m_whiteTexture;

    // Shared BindGroupLayoutDescs (value types, no GPU objects)
    rhi::BindGroupLayoutDesc m_viewLayoutDesc;
    rhi::BindGroupLayoutDesc m_sceneLayoutDesc;
    rhi::BindGroupLayoutDesc m_objectLayoutDesc;
    u64 m_objectUBOSize = 0;

    // Descriptor set indices from shader reflection
    int16_t m_viewSetIndex = -1;
    int16_t m_sceneSetIndex = -1;
    int16_t m_objectSetIndex = -1;
};

} // namespace ln
