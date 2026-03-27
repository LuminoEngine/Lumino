#pragma once
#include <lumino_base/math/Math.hpp>
#include <lumino_base/RefObject.hpp>
#include <lumino_base/Result.hpp>
#include <lumino_core/graphics/rhi/Rhi.hpp>
#include <lumino_core/graphics/Vertex.hpp>
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
class Material : public RefObject {
public:
    ~Material() override = default;

    MaterialType type() const { return type_; }

    void setColor(const Color& color);
    void setTexture(rhi::Texture* texture);
    void setSpecular(const Color& color, f32 shininess);

    // Render state
    void setBlendEnabled(bool enabled);
    void setCullMode(rhi::CullMode mode);
    void setDepthTestEnabled(bool enabled);
    void setDepthWriteEnabled(bool enabled);

    // Access internals (used by ForwardRenderer)
    rhi::RenderPipeline* pipeline() const { return pipeline_.get(); }
    rhi::BindGroupLayout* materialBindGroupLayout() const { return materialBindGroupLayout_.get(); }
    rhi::BindGroup* materialBindGroup() const { return materialBindGroup_.get(); }

    /** Rebuild the BindGroup after parameter changes. Call before rendering. */
    Result<void> updateBindGroup(rhi::Device* device);

    /**
     * Rebuild the RenderPipeline. Must be called after render state changes
     * or initially during creation.
     */
    Result<void> buildPipeline(
        rhi::Device* device,
        rhi::PipelineLayout* pipelineLayout,
        rhi::TextureFormat colorFormat,
        rhi::TextureFormat depthFormat = rhi::TextureFormat::Depth32Float,
        rhi::PrimitiveTopology topology = rhi::PrimitiveTopology::TriangleList);

private:
    friend class MaterialFactory;

    MaterialType type_ = MaterialType::Unlit;

    // Shader modules
    Ref<rhi::ShaderModule> vertShader_;
    Ref<rhi::ShaderModule> fragShader_;
    std::string vertEntry_;
    std::string fragEntry_;

    // Pipeline
    Ref<rhi::RenderPipeline> pipeline_;

    // BindGroup for material (Set 1)
    Ref<rhi::BindGroupLayout> materialBindGroupLayout_;
    Ref<rhi::BindGroup> materialBindGroup_;

    // Parameters
    Color baseColor_ = Color::white();
    Color specularColor_ = Color::white();
    f32 shininess_ = 32.0f;

    // Uniform buffer for material params
    u64 materialParamBufferSize_ = 0;
    Ref<rhi::Buffer> paramBuffer_;

    // Textures
    Ref<rhi::Texture> baseTexture_;
    Ref<rhi::TextureView> baseTextureView_;
    Ref<rhi::Sampler> sampler_;

    // Render state
    rhi::CullMode cullMode_ = rhi::CullMode::Back;
    bool blendEnabled_ = false;
    bool depthTestEnabled_ = true;
    bool depthWriteEnabled_ = true;

    bool paramsDirty_ = true;
};

class GraphicsContext;

/** Factory for creating built-in materials from precompiled shaders. */
class MaterialFactory {
public:
    /** Create an Unlit material (texture * color, no lighting). */
    static Result<Ref<Material>> createUnlit(
        rhi::Device* device,
        rhi::PipelineLayout* pipelineLayout,
        rhi::TextureFormat colorFormat,
        rhi::TextureFormat depthFormat = rhi::TextureFormat::Depth32Float);

    /** Create an Unlit material from a GraphicsContext. */
    static Result<Ref<Material>> createUnlit(
        GraphicsContext* ctx,
        rhi::PipelineLayout* pipelineLayout);

    /** Create a BasicLit material (Blinn-Phong, 1 directional light). */
    static Result<Ref<Material>> createBasicLit(
        rhi::Device* device,
        rhi::PipelineLayout* pipelineLayout,
        rhi::TextureFormat colorFormat,
        rhi::TextureFormat depthFormat = rhi::TextureFormat::Depth32Float);

    /** Create a BasicLit material from a GraphicsContext. */
    static Result<Ref<Material>> createBasicLit(
        GraphicsContext* ctx,
        rhi::PipelineLayout* pipelineLayout);

private:
    static Result<Ref<Material>> createMaterialFromShaderData(
        rhi::Device* device,
        rhi::PipelineLayout* pipelineLayout,
        rhi::TextureFormat colorFormat,
        rhi::TextureFormat depthFormat,
        const unsigned char* shaderData, size_t shaderDataSize,
        MaterialType type);
};

} // namespace ln
