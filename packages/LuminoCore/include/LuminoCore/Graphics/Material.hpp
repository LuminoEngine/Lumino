#pragma once
#include <LuminoBase/math/Math.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoBase/Result.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <LuminoCore/Graphics/Vertex.hpp>
#include <LuminoCore/Graphics/ShaderPass.hpp>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

namespace ln {

/** @see LNBlendMode */
enum class BlendMode {
    Normal = 0,
    Alpha = 1,
    Add = 2,
    Subtract = 3,
    Multiply = 4,
};

/** GPU-aligned view params (Set N - camera): must match shader ViewParams struct. */
struct ViewParamsUBO {
    float viewProj[16];
    float cameraPos[4];
    float view[16];         // view matrix (world -> view)
    float proj[16];         // projection matrix (view -> clip)
    float invViewProj[16]; // inverse of viewProj (clip -> world)
    float invProj[16];     // inverse of proj (clip -> view)
    float screenSize[4];   // (width, height, 1/width, 1/height)
};

/** GPU-aligned scene params (Set N - lighting): must match shader SceneParams struct. */
struct SceneParamsUBO {
    float lightDir[4];
    float lightColor[4];
    float ambientColor[4];
};

/** GPU-aligned object params (Set N): must match shader ObjectParams struct. */
struct ObjectParamsUBO {
    float world[16];
    float normalMatrix[16];
};

/**
 * Material: shader + parameters + render state + textures.
 * Manages a RenderPipeline and per-material BindGroup.
 *
 * Parameter storage is driven by shader reflection:
 * - $Global CB members are stored in a byte buffer (m_paramBuffer)
 * - Texture/Sampler slots are stored in m_baseTexture (extensible later)
 */
class Material : public Object {
public:
    ~Material() override = default;

    /** Set a named float4 parameter in the material's $Global constant buffer. */
    void setFloat4(const std::string& name, const float* values);

    /** Set a named float parameter in the material's $Global constant buffer. */
    void setFloat(const std::string& name, float value);

    /** Convenience: set the "color" field of the CB named "u_params". */
    void setColor(const Color& color);

    /** Convenience: set color + specular for BasicLit-style shaders. */
    void setSpecular(const Color& color, float shininess);

    void setTexture(rhi::Texture* texture);

    /** Set a texture by shader binding name (e.g., "u_sceneColor"). */
    void setNamedTexture(const std::string& name, rhi::Texture* texture);

    // Render state
    void setBlendMode(BlendMode mode);
    void setCullMode(rhi::CullMode mode);
    void setDepthTestEnabled(bool enabled);
    void setDepthWriteEnabled(bool enabled);

    // ShaderPass accessor
    /** Default (primary) ShaderPass — the first pass registered, typically "Forward".
     *  Used for material parameter layout and as a fallback accessor. */
    ShaderPass* shaderPass() const { return m_defaultShaderPass.get(); }

    /** Look up a ShaderPass by name (e.g. "Forward", "GBuffer"). Returns nullptr if absent. */
    ShaderPass* findPass(const std::string& name) const {
        auto it = m_shaderPasses.find(name);
        return (it != m_shaderPasses.end()) ? it->second.get() : nullptr;
    }

    /** Whether this material has a ShaderPass with the given name. */
    bool hasPass(const std::string& name) const {
        return m_shaderPasses.find(name) != m_shaderPasses.end();
    }

    /** All ShaderPasses registered on this material, keyed by pass name. */
    const std::unordered_map<std::string, Ref<ShaderPass>>& shaderPasses() const { return m_shaderPasses; }

    // Shader / render state accessors (used by PipelineCache key construction)
    rhi::ShaderModule* vertexShader() const { return m_defaultShaderPass->vertexShader(); }
    rhi::ShaderModule* fragmentShader() const { return m_defaultShaderPass->fragmentShader(); }
    const std::string& vertexEntry() const { return m_defaultShaderPass->vertexEntry(); }
    const std::string& fragmentEntry() const { return m_defaultShaderPass->fragmentEntry(); }
    rhi::CullMode cullMode() const { return m_cullMode; }
    BlendMode blendMode() const { return m_blendMode; }
    bool depthTestEnabled() const { return m_depthTestEnabled; }
    bool depthWriteEnabled() const { return m_depthWriteEnabled; }

    /** Parameter version counter. Incremented whenever material parameters change. */
    uint64_t paramVersion() const { return m_paramVersion; }

    // Accessors for Renderer-side BindGroup construction
    rhi::Texture* baseTexture() const { return m_baseTexture.get(); }
    uint64_t materialParamBufferSize() const { return m_defaultShaderPass->materialParamBufferSize(); }
    const Color& baseColor() const { return m_baseColor; }

    /** Named textures map (for reflection-driven bind group construction). */
    const std::unordered_map<std::string, Ref<rhi::Texture>>& namedTextures() const { return m_namedTextures; }

    /** Write material UBO data into the given mapped pointer. */
    void writeMaterialUBO(void* dst) const;

private:
    Material();
    friend class MaterialFactory;

    // ShaderPasses keyed by pass name (e.g. "Forward", "GBuffer").
    // Same material can be rendered in different render passes by looking up by name.
    // 前提: 同一マテリアル内の全パスは $Material の layout (params/テクスチャ slot) を共有する。
    std::unordered_map<std::string, Ref<ShaderPass>> m_shaderPasses;

    // Default (primary) pass — typically the first registered, used for material param layout.
    // Held as a separate Ref to keep hot-path accessors (shaderPass()/materialParamBufferSize)
    // from doing map lookups.
    Ref<ShaderPass> m_defaultShaderPass;

    // Parameter version counter (incremented on any parameter change)
    uint64_t m_paramVersion;

    // Generic material parameter buffer (matches $Global CB layout from reflection)
    std::vector<uint8_t> m_paramBuffer;

    // Cached base color (for convenience accessors)
    Color m_baseColor;

    // Textures
    Ref<rhi::Texture> m_baseTexture;

    // Named texture slots (keyed by shader binding name, e.g., "u_sceneColor")
    std::unordered_map<std::string, Ref<rhi::Texture>> m_namedTextures;

    // Render state
    rhi::CullMode m_cullMode;
    BlendMode m_blendMode;
    bool m_depthTestEnabled;
    bool m_depthWriteEnabled;

    void markDirty() { ++m_paramVersion; }

    /** Find offset of a named member in $Global CB. Returns -1 if not found. */
    int findMemberOffset(const std::string& name) const;
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

    /**
     * .slang ソースファイルを実行時コンパイルしてマテリアルを作成します。
     * LUMINO_USE_SLANG が有効なデスクトップビルドでのみ動作します。
     * @param ctx            GraphicsContext のハンドル
     * @param shaderFilePath コンパイルする .slang ファイルのパス
     * @param searchPath     lumino.slang を含むディレクトリのパス (空文字列で無効)
     */
    static Result<Ref<Material>> createFromShaderSourceFile(
        GraphicsContext* ctx,
        std::string_view shaderFilePath,
        std::string_view searchPath);

private:
    static Result<Ref<Material>> createMaterialFromBuiltin(
        GraphicsModule* module, BuiltinShader shader);

    // Shared helpers (friend access to Material internals via MaterialFactory).
    static void registerPass(Material* mat, Ref<ShaderPass> pass);
    static void initParamBufferFromDefaultPass(Material* mat);
};

} // namespace ln
