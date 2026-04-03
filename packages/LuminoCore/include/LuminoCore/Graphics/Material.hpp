#pragma once
#include <LuminoBase/math/Math.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoBase/Result.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <LuminoCore/Graphics/Vertex.hpp>
#include <string>

namespace ln {

/** GPU-aligned view params (Set 0 — camera): must match shader ViewParams struct. */
struct ViewParamsUBO {
    f32 viewProj[16];
    f32 cameraPos[4];
};

/** GPU-aligned scene params (Set 1 — lighting): must match shader SceneParams struct. */
struct SceneParamsUBO {
    f32 lightDir[4];
    f32 lightColor[4];
    f32 ambientColor[4];
};

/** GPU-aligned material params for Unlit (Set 2). */
struct UnlitMaterialParamsUBO {
    f32 color[4];
};

/** GPU-aligned material params for BasicLit (Set 2). */
struct BasicLitMaterialParamsUBO {
    f32 color[4];
    f32 specular[4]; // xyz = specular color, w = shininess
};

/** GPU-aligned object params (Set 3): must match shader ObjectParams struct. */
struct ObjectParamsUBO {
    f32 world[16];
    f32 normalMatrix[16];
};

enum class MaterialType {
    Unlit,
    BasicLit,
};

/**
 * Material: shader + parameters + render state + textures.
 * Manages a RenderPipeline and per-material BindGroup (Set 2).
 */
class Material : public Object {
public:
    ~Material() override = default;

    MaterialType type() const { return m_type; }

    void setColor(const Color& color);
    void setTexture(rhi::Texture* texture);
    void setSpecular(const Color& color, f32 shininess);

    // Render state
    void setBlendEnabled(bool enabled);
    void setCullMode(rhi::CullMode mode);
    void setDepthTestEnabled(bool enabled);
    void setDepthWriteEnabled(bool enabled);

    // Shader / render state accessors (used by PipelineCache key construction)
    rhi::ShaderModule* vertexShader() const { return m_vertShader.get(); }
    rhi::ShaderModule* fragmentShader() const { return m_fragShader.get(); }
    const std::string& vertexEntry() const { return m_vertEntry; }
    const std::string& fragmentEntry() const { return m_fragEntry; }
    rhi::CullMode cullMode() const { return m_cullMode; }
    bool blendEnabled() const { return m_blendEnabled; }
    bool depthTestEnabled() const { return m_depthTestEnabled; }
    bool depthWriteEnabled() const { return m_depthWriteEnabled; }

    // BindGroup access (used by Renderer)
    rhi::BindGroupLayout* materialBindGroupLayout() const { return m_materialBindGroupLayout.get(); }

    /** Parameter version counter. Incremented whenever material parameters change. */
    uint64_t paramVersion() const { return m_paramVersion; }

    // Accessors for Renderer-side BindGroup construction
    rhi::Texture* baseTexture() const { return m_baseTexture.get(); }
    u64 materialParamBufferSize() const { return m_materialParamBufferSize; }
    const Color& baseColor() const { return m_baseColor; }
    const Color& specularColor() const { return m_specularColor; }
    f32 shininess() const { return m_shininess; }

    /** Write material UBO data into the given mapped pointer. */
    void writeMaterialUBO(void* dst) const;

private:
    Material();
    friend class MaterialFactory;

    MaterialType m_type;

    // Shader modules
    Ref<rhi::ShaderModule> m_vertShader;
    Ref<rhi::ShaderModule> m_fragShader;
    std::string m_vertEntry;
    std::string m_fragEntry;

    // BindGroup for material (Set 2)
    Ref<rhi::BindGroupLayout> m_materialBindGroupLayout;

    // Parameter version counter (incremented on any parameter change)
    uint64_t m_paramVersion;

    // Parameters
    Color m_baseColor;
    Color m_specularColor;
    f32 m_shininess;

    // Material param buffer size (from shader reflection)
    u64 m_materialParamBufferSize;

    // Textures
    Ref<rhi::Texture> m_baseTexture;

    // Render state
    rhi::CullMode m_cullMode;
    bool m_blendEnabled;
    bool m_depthTestEnabled;
    bool m_depthWriteEnabled;

    void markDirty() { ++m_paramVersion; }
};

class GraphicsContext;
class GraphicsModule;
enum class BuiltinShader;

/** Factory for creating built-in materials from precompiled shaders. */
class MaterialFactory {
public:
    /** Create an Unlit material (texture * color, no lighting). */
    static Result<Ref<Material>> createUnlit(GraphicsModule* module);

    /** Create an Unlit material from a GraphicsContext. */
    static Result<Ref<Material>> createUnlit(GraphicsContext* ctx);

    /** Create a BasicLit material (Blinn-Phong, 1 directional light). */
    static Result<Ref<Material>> createBasicLit(GraphicsModule* module);

    /** Create a BasicLit material from a GraphicsContext. */
    static Result<Ref<Material>> createBasicLit(GraphicsContext* ctx);

    /** Create a StencilMask material (alpha-tested stencil write, no color output). */
    static Result<Ref<Material>> createStencilMask(GraphicsModule* module);

    /** Create a StencilMask material from a GraphicsContext. */
    static Result<Ref<Material>> createStencilMask(GraphicsContext* ctx);

    /** Create a material from a compiled shader binary (.lcsh). */
    static Result<Ref<Material>> createFromCompiledShader(
        GraphicsModule* module, const void* data, size_t size);

    /** Create a material from a compiled shader binary (.lcsh) via GraphicsContext. */
    static Result<Ref<Material>> createFromCompiledShader(
        GraphicsContext* ctx, const void* data, size_t size);

private:
    static Result<Ref<Material>> createMaterialFromBuiltin(
        GraphicsModule* module, BuiltinShader shader, MaterialType type);
};

} // namespace ln
