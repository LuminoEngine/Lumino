#include <LuminoCore/Graphics/Material.hpp>
#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/Graphics/GraphicsModule.hpp>
#include <LuminoCore/Graphics/Shader.hpp>
#include <LuminoCore/Graphics/ShaderPass.hpp>
#include <cstring>

namespace ln {

// ------ Material ------------------------------------------------------------------------------------------------------------------------

Material::Material()
    : Object()
    , m_defaultShaderPass(nullptr)
    , m_paramVersion(1)
    , m_bindingVersion(1)
    , m_baseColor(Color::white())
    , m_baseTexture(nullptr)
    , m_cullMode(rhi::CullMode::Back)
    , m_blendMode(BlendMode::Normal)
    , m_depthTestEnabled(true)
    , m_depthWriteEnabled(true) {
}

int Material::findMemberOffset(const std::string& name) const {
    if (!m_defaultShaderPass) return -1;
    for (const auto& member : m_defaultShaderPass->materialMembers()) {
        if (member.name == name) return member.offset;
    }
    return -1;
}

void Material::setFloat4(const std::string& name, const float* values) {
    int offset = findMemberOffset(name);
    if (offset < 0 || static_cast<size_t>(offset) + sizeof(float) * 4 > m_paramBuffer.size()) return;
    std::memcpy(m_paramBuffer.data() + offset, values, sizeof(float) * 4);
    markParamDirty();
}

void Material::setFloat(const std::string& name, float value) {
    int offset = findMemberOffset(name);
    if (offset < 0 || static_cast<size_t>(offset) + sizeof(float) > m_paramBuffer.size()) return;
    std::memcpy(m_paramBuffer.data() + offset, &value, sizeof(float));
    markParamDirty();
}

void Material::setColor(const Color& color) {
    m_baseColor = color;
    // Write to "color" field within any CB member named in $Global scope.
    // For builtin shaders, the CB is u_params with layout { float4 color; ... }
    // The color field is at offset 0 within the CB, and the CB itself is at the
    // offset of the "u_params" member + 0 (color field).
    // Since $Global CB packs bare uniforms AND explicit CBs, u_params offset
    // represents where the CB data starts in the $Global buffer.
    // However, with ConstantBuffer<MaterialParams>, the CB is a separate binding.
    // In our current layout, there is an implicit $Global CB (binding 0) and an
    // explicit u_params CB (binding 1). The materialParamBufferSize comes from
    // the *first* ConstantBuffer in the $Material block.
    // For simplicity: write the entire color as the first 16 bytes of the buffer.
    // This works because BasicLit's MaterialParams starts with float4 color.
    if (m_paramBuffer.size() >= sizeof(float) * 4) {
        float rgba[4] = { color.r, color.g, color.b, color.a };
        std::memcpy(m_paramBuffer.data(), rgba, sizeof(rgba));
        markParamDirty();
    }
}

void Material::setSpecular(const Color& color, float shininess) {
    // Write to "specular" field at offset 16 (after float4 color)
    if (m_paramBuffer.size() >= sizeof(float) * 8) {
        float spec[4] = { color.r, color.g, color.b, shininess };
        std::memcpy(m_paramBuffer.data() + sizeof(float) * 4, spec, sizeof(spec));
        markParamDirty();
    }
}

void Material::setTexture(rhi::Texture* texture) {
    // 同じテクスチャの再設定でバージョンを進めない。Renderer 側は bindingVersion が
    // 動いたら BindGroup を捨てるだけなので、ここで弾かないと無駄な再生成になる。
    rhi::Texture* newTexture =
        texture ? texture : CoreInstance::instance()->graphicsModule()->whiteTexture().get();
    if (m_baseTexture.get() == newTexture) return;
    newTexture->addRef();
    m_baseTexture = Ref<rhi::Texture>::adopt(newTexture);
    markBindingDirty();
}

void Material::writeMaterialUBO(void* dst) const {
    if (!m_paramBuffer.empty()) {
        std::memcpy(dst, m_paramBuffer.data(), m_paramBuffer.size());
    }
}

void Material::setNamedTexture(const std::string& name, rhi::Texture* texture) {
    rhi::Texture* newTexture =
        texture ? texture : CoreInstance::instance()->graphicsModule()->whiteTexture().get();
    auto it = m_namedTextures.find(name);
    if (it != m_namedTextures.end() && it->second.get() == newTexture) return;
    newTexture->addRef();
    m_namedTextures[name] = Ref<rhi::Texture>::adopt(newTexture);
    markBindingDirty();
}

void Material::setSamplerState(const SamplerState& state) {
    if (m_samplerState == state) return;
    m_samplerState = state;
    markBindingDirty();
}

void Material::setNamedSamplerState(const std::string& name, const SamplerState& state) {
    auto it = m_namedSamplerStates.find(name);
    if (it != m_namedSamplerStates.end() && it->second == state) return;
    m_namedSamplerStates[name] = state;
    markBindingDirty();
}

void Material::addDestroyCallback(DestroyCallback callback) {
    m_destroyCallbacks.push_back(std::move(callback));
}

void Material::finalize() {
    // デストラクタではなく finalize で通知する。ここではまだメンバ (m_shaderPasses 等)
    // が生きているため、コールバック側が Material の情報を読んでも安全。
    for (auto& cb : m_destroyCallbacks) {
        cb(this);
    }
    m_destroyCallbacks.clear();
    Object::finalize();
}

void Material::setBlendMode(BlendMode mode) { m_blendMode = mode; }
void Material::setCullMode(rhi::CullMode mode) { m_cullMode = mode; }
void Material::setDepthTestEnabled(bool enabled) { m_depthTestEnabled = enabled; }
void Material::setDepthWriteEnabled(bool enabled) { m_depthWriteEnabled = enabled; }

// ------ MaterialFactory -----------------------------------------------------------------------------------------------------------------

// Register a ShaderPass on a Material, using the pass's own name as key.
// The first pass registered becomes the "default" pass.
void MaterialFactory::registerPass(Material* mat, Ref<ShaderPass> pass) {
    const std::string& name = pass->passName();
    if (!mat->m_defaultShaderPass) {
        mat->m_defaultShaderPass = pass;
    }
    mat->m_shaderPasses[name] = std::move(pass);
}

// Initialize Material's $Material param buffer from the default pass's reflection.
// Writes a default white color into the first float4 if present.
void MaterialFactory::initParamBufferFromDefaultPass(Material* mat) {
    auto bufSize = mat->m_defaultShaderPass->materialParamBufferSize();
    if (bufSize > 0) {
        mat->m_paramBuffer.resize(static_cast<size_t>(bufSize), 0);
        if (bufSize >= sizeof(float) * 4) {
            float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            std::memcpy(mat->m_paramBuffer.data(), white, sizeof(white));
        }
    }
}

Result<Ref<Material>> MaterialFactory::createMaterialFromBuiltin(
    GraphicsModule* module, BuiltinShader shader) {
    const auto& passes = module->builtinShaderPasses(shader);
    if (passes.empty()) {
        return LN_MAKE_ERROR("Builtin shader not initialized");
    }

    auto mat = Ref<Material>::adopt(new Material());
    for (const auto& p : passes) {
        registerPass(mat.get(), p);
    }
    mat->m_baseTexture = module->whiteTexture();

    initParamBufferFromDefaultPass(mat.get());
    return mat;
}

Result<Ref<Material>> MaterialFactory::createUnlit(GraphicsModule* module) {
    return createMaterialFromBuiltin(module, BuiltinShader::Unlit);
}

Result<Ref<Material>> MaterialFactory::createBasicLit(GraphicsModule* module) {
    auto result = createMaterialFromBuiltin(module, BuiltinShader::BasicLit);
    if (!result) return result;
    // Set default specular for BasicLit
    auto& mat = *result;
    mat->setSpecular(Color::white(), 32.0f);
    return result;
}

Result<Ref<Material>> MaterialFactory::createUnlit(GraphicsContext* ctx) {
    return createUnlit(ctx->module());
}

Result<Ref<Material>> MaterialFactory::createBasicLit(GraphicsContext* ctx) {
    return createBasicLit(ctx->module());
}

Result<Ref<Material>> MaterialFactory::createStencilMask(GraphicsModule* module) {
    return createMaterialFromBuiltin(module, BuiltinShader::StencilMask);
}

Result<Ref<Material>> MaterialFactory::createStencilMask(GraphicsContext* ctx) {
    return createStencilMask(ctx->module());
}

Result<Ref<Material>> MaterialFactory::createFromShader(
    GraphicsModule* module, Shader* shader) {
    if (!shader) return LN_MAKE_ERROR("Shader is null");

    const auto& passes = shader->passes();
    if (passes.empty()) {
        return LN_MAKE_ERROR("No shader passes found");
    }

    // ビルトインシェーダと同じく、既存の ShaderPass を共有する
    // (シェーダモジュール / パイプラインレイアウトは新規生成しない)。
    auto mat = Ref<Material>::adopt(new Material());
    for (const auto& p : passes) {
        registerPass(mat.get(), p);
    }
    mat->m_baseTexture = module->whiteTexture();

    initParamBufferFromDefaultPass(mat.get());
    return mat;
}

Result<Ref<Material>> MaterialFactory::createFromShader(
    GraphicsContext* ctx, Shader* shader) {
    return createFromShader(ctx->module(), shader);
}

Result<Ref<Material>> MaterialFactory::createFromCompiledShader(
    GraphicsModule* module, const void* data, size_t size) {
    // このマテリアル専用の Shader を作り、そこから作成する。Shader は Material が
    // 各パスを Ref で保持するためここで解放してよい。
    auto shaderResult = Shader::createFromCompiledShader(module, data, size);
    if (!shaderResult) return LN_FORWARD_ERROR(shaderResult);
    return createFromShader(module, shaderResult->get());
}

Result<Ref<Material>> MaterialFactory::createFromCompiledShader(
    GraphicsContext* ctx, const void* data, size_t size) {
    return createFromCompiledShader(ctx->module(), data, size);
}

Result<Ref<Material>> MaterialFactory::createFromShaderSourceFile(
    GraphicsContext* ctx,
    std::string_view shaderFilePath,
    std::string_view searchPath) {
    auto shaderResult = Shader::createFromShaderSourceFile(ctx, shaderFilePath, searchPath);
    if (!shaderResult) return LN_FORWARD_ERROR(shaderResult);
    return createFromShader(ctx->module(), shaderResult->get());
}

} // namespace ln
