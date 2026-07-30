#include <LuminoCore/Graphics/Material.hpp>
#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/Graphics/GraphicsModule.hpp>
#include <LuminoCore/Graphics/ShaderPass.hpp>
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/UnifiedShaderSerializer2.hpp>
#include <cstring>

#ifdef LUMINO_USE_SLANG
#include <LuminoShader/ShaderCompiler2.hpp>
#endif // LUMINO_USE_SLANG

namespace ln {

// ------ Material ------------------------------------------------------------------------------------------------------------------------

Material::Material()
    : Object()
    , m_defaultShaderPass(nullptr)
    , m_paramVersion(1)
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
    markDirty();
}

void Material::setFloat(const std::string& name, float value) {
    int offset = findMemberOffset(name);
    if (offset < 0 || static_cast<size_t>(offset) + sizeof(float) > m_paramBuffer.size()) return;
    std::memcpy(m_paramBuffer.data() + offset, &value, sizeof(float));
    markDirty();
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
        markDirty();
    }
}

void Material::setSpecular(const Color& color, float shininess) {
    // Write to "specular" field at offset 16 (after float4 color)
    if (m_paramBuffer.size() >= sizeof(float) * 8) {
        float spec[4] = { color.r, color.g, color.b, shininess };
        std::memcpy(m_paramBuffer.data() + sizeof(float) * 4, spec, sizeof(spec));
        markDirty();
    }
}

void Material::setTexture(rhi::Texture* texture) {
    if (texture) {
        texture->addRef();
        m_baseTexture = Ref<rhi::Texture>::adopt(texture);
    } else {
        m_baseTexture = CoreInstance::instance()->graphicsModule()->whiteTexture();
    }
    markDirty();
}

void Material::writeMaterialUBO(void* dst) const {
    if (!m_paramBuffer.empty()) {
        std::memcpy(dst, m_paramBuffer.data(), m_paramBuffer.size());
    }
}

void Material::setNamedTexture(const std::string& name, rhi::Texture* texture) {
    if (texture) {
        texture->addRef();
        m_namedTextures[name] = Ref<rhi::Texture>::adopt(texture);
    } else {
        m_namedTextures[name] = CoreInstance::instance()->graphicsModule()->whiteTexture();
    }
    markDirty();
}

void Material::setSamplerState(const SamplerState& state) {
    if (m_samplerState == state) return;
    m_samplerState = state;
    // サンプラーが変わると BindGroup を作り直す必要があるため paramVersion を進める。
    markDirty();
}

void Material::setNamedSamplerState(const std::string& name, const SamplerState& state) {
    auto it = m_namedSamplerStates.find(name);
    if (it != m_namedSamplerStates.end() && it->second == state) return;
    m_namedSamplerStates[name] = state;
    markDirty();
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

Result<Ref<Material>> MaterialFactory::createFromCompiledShader(
    GraphicsModule* module, const void* data, size_t size) {
    // Deserialize once so we can discover how many passes the shader contains.
    auto loadResult = shader::UnifiedShaderSerializer2::loadFromData(data, size);
    if (!loadResult) return LN_FORWARD_ERROR(loadResult);
    auto unifiedShader = std::move(*loadResult);

    auto& globalPasses = unifiedShader->globalShaderPasses();
    if (globalPasses.empty()) {
        return LN_MAKE_ERROR("No shader passes found");
    }

    auto mat = Ref<Material>::adopt(new Material());
    for (size_t i = 0; i < globalPasses.size(); ++i) {
        auto passResult = ShaderPass::createFromCompiledShader(
            data, size, module->device(), i);
        if (!passResult) return LN_FORWARD_ERROR(passResult);
        registerPass(mat.get(), std::move(*passResult));
    }
    mat->m_baseTexture = module->whiteTexture();

    initParamBufferFromDefaultPass(mat.get());
    return mat;
}

Result<Ref<Material>> MaterialFactory::createFromCompiledShader(
    GraphicsContext* ctx, const void* data, size_t size) {
    return createFromCompiledShader(ctx->module(), data, size);
}

Result<Ref<Material>> MaterialFactory::createFromShaderSourceFile(
    GraphicsContext* ctx,
    std::string_view shaderFilePath,
    std::string_view searchPath) {
#ifdef LUMINO_USE_SLANG
    using namespace ln::shader;
    namespace fs = std::filesystem;

    auto compilerResult = ShaderCompiler2::create();
    if (!compilerResult) return LN_FORWARD_ERROR(compilerResult);
    auto& compiler = *compilerResult;

    if (!searchPath.empty()) {
        compiler->addSearchPath(fs::path(searchPath));
    }

    auto buildResult = compiler->build(fs::path(shaderFilePath));
    if (!buildResult) {
        // TODO: ログとかにちゃんと出す
        std::cerr << "Error: " << buildResult.error().message << std::endl;
        return LN_FORWARD_ERROR(buildResult);
    }

    auto* unifiedShader = compiler->shader();
    auto& globalPasses = unifiedShader->globalShaderPasses();
    if (globalPasses.empty()) {
        return LN_MAKE_ERROR("No shader passes found");
    }

    auto mat = Ref<Material>::adopt(new Material());
    for (size_t i = 0; i < globalPasses.size(); ++i) {
        auto passResult = ShaderPass::createFromUnifiedShader(
            unifiedShader, ctx->module()->device(), i);
        if (!passResult) return LN_FORWARD_ERROR(passResult);
        registerPass(mat.get(), std::move(*passResult));
    }
    mat->m_baseTexture = ctx->module()->whiteTexture();

    initParamBufferFromDefaultPass(mat.get());
    return mat;
#else
    (void)ctx; (void)shaderFilePath; (void)searchPath;
    return LN_MAKE_ERROR("LNMaterial_CreateFromShaderSourceFile requires LUMINO_USE_SLANG");
#endif // LUMINO_USE_SLANG
}

} // namespace ln
