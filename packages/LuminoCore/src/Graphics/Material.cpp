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
    // $Global スコープで名前付けされた CB メンバ内の "color" フィールドに書き込む。
    // 組み込みシェーダでは CB は u_params で、レイアウトは { float4 color; ... }。
    // color フィールドは CB 内のオフセット 0 にあり、CB 自体は "u_params" メンバの
    // オフセット + 0 (color フィールド) の位置にある。
    // $Global CB はベアな uniform と明示的な CB の両方をパックするため、u_params の
    // オフセットは $Global バッファ内で CB データが始まる位置を表す。
    // ただし ConstantBuffer<MaterialParams> の場合、CB は別のバインディングになる。
    // 現在のレイアウトでは、暗黙の $Global CB (binding 0) と明示的な u_params CB
    // (binding 1) がある。materialParamBufferSize は $Material ブロック内の
    // *最初の* ConstantBuffer から得られる。
    // 単純化のため、色をバッファの先頭 16 バイトとしてそのまま書き込む。
    // BasicLit の MaterialParams が float4 color から始まるので、これで動作する。
    if (m_paramBuffer.size() >= sizeof(float) * 4) {
        float rgba[4] = { color.r, color.g, color.b, color.a };
        std::memcpy(m_paramBuffer.data(), rgba, sizeof(rgba));
        markParamDirty();
    }
}

void Material::setSpecular(const Color& color, float shininess) {
    // オフセット 16 (float4 color の後) の "specular" フィールドに書き込む
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

// パス自身の名前をキーとして、Material に ShaderPass を登録する。
// 最初に登録されたパスが「既定」のパスになる。
void MaterialFactory::registerPass(Material* mat, Ref<ShaderPass> pass) {
    const std::string& name = pass->passName();
    if (!mat->m_defaultShaderPass) {
        mat->m_defaultShaderPass = pass;
    }
    // 同名パスの再登録は上書き (unordered_map だったときの挙動を維持する)。
    for (auto& existing : mat->m_shaderPasses) {
        if (existing->passName() == name) {
            existing = std::move(pass);
            return;
        }
    }
    mat->m_shaderPasses.push_back(std::move(pass));
}

// 既定パスのリフレクション情報から Material の $Material パラメータバッファを初期化する。
// 先頭の float4 があれば、既定値として白を書き込む。
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
    // BasicLit の既定のスペキュラを設定する
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
