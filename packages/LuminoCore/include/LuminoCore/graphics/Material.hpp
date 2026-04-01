#pragma once
#include <LuminoBase/math/Math.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoBase/Result.hpp>
#include <LuminoCore/graphics/rhi/Rhi.hpp>
#include <LuminoCore/graphics/Vertex.hpp>
#include <string>

namespace ln {

/** GPU-aligned view params (Set 0): must match shader ViewParams struct. */
struct ViewParamsUBO {
    f32 viewProj[16];
    f32 cameraPos[4];
    f32 lightDir[4];
    f32 lightColor[4];
    f32 ambientColor[4];
};

/** GPU-aligned material params for Unlit (Set 1). */
struct UnlitMaterialParamsUBO {
    f32 color[4];
};

/** GPU-aligned material params for BasicLit (Set 1). */
struct BasicLitMaterialParamsUBO {
    f32 color[4];
    f32 specular[4]; // xyz = specular color, w = shininess
};

/** GPU-aligned object params (Set 2): must match shader ObjectParams struct. */
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
 * Manages a RenderPipeline and per-material BindGroup (Set 1).
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

    // BindGroup access (used by ForwardRenderer)
    rhi::BindGroupLayout* materialBindGroupLayout() const { return m_materialBindGroupLayout.get(); }
    rhi::BindGroup* materialBindGroup() const { return m_materialBindGroup.get(); }

    /** Rebuild the BindGroup after parameter changes. Call before rendering. */
    Result<void> updateBindGroup(rhi::Device* device);

private:
    friend class MaterialFactory;

    MaterialType m_type = MaterialType::Unlit;

    // Shader modules
    Ref<rhi::ShaderModule> m_vertShader;
    Ref<rhi::ShaderModule> m_fragShader;
    std::string m_vertEntry;
    std::string m_fragEntry;

    // BindGroup for material (Set 1)
    Ref<rhi::BindGroupLayout> m_materialBindGroupLayout;
    Ref<rhi::BindGroup> m_materialBindGroup;

    // Parameters
    Color m_baseColor = Color::white();
    Color m_specularColor = Color::white();
    f32 m_shininess = 32.0f;

    // Uniform buffer for material params
    u64 m_materialParamBufferSize = 0;
    Ref<rhi::Buffer> m_paramBuffer;

    // Textures
    Ref<rhi::Texture> m_baseTexture;
    Ref<rhi::TextureView> m_baseTextureView;
    Ref<rhi::Sampler> m_sampler;

    // Render state
    rhi::CullMode m_cullMode = rhi::CullMode::Back;
    bool m_blendEnabled = false;
    bool m_depthTestEnabled = true;
    bool m_depthWriteEnabled = true;

    bool m_paramsDirty = true;
};

class GraphicsContext;

/** Factory for creating built-in materials from precompiled shaders. */
class MaterialFactory {
public:
    /** Create an Unlit material (texture * color, no lighting). */
    static Result<Ref<Material>> createUnlit(rhi::Device* device);

    /** Create an Unlit material from a GraphicsContext. */
    static Result<Ref<Material>> createUnlit(GraphicsContext* ctx);

    /** Create a BasicLit material (Blinn-Phong, 1 directional light). */
    static Result<Ref<Material>> createBasicLit(rhi::Device* device);

    /** Create a BasicLit material from a GraphicsContext. */
    static Result<Ref<Material>> createBasicLit(GraphicsContext* ctx);

private:
    static Result<Ref<Material>> createMaterialFromShaderData(
        rhi::Device* device,
        const unsigned char* shaderData, size_t shaderDataSize,
        MaterialType type);
};

} // namespace ln
