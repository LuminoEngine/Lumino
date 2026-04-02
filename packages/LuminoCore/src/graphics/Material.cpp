#include <LuminoCore/graphics/Material.hpp>
#include <LuminoCore/graphics/GraphicsContext.hpp>
#include <LuminoCore/graphics/GraphicsModule.hpp>
#include <cstring>

namespace ln {

// ------ Material ------------------------------------------------------------------------------------------------------------------------

Material::Material()
    : Object()
    , m_type(MaterialType::Unlit)
    , m_vertShader(nullptr)
    , m_fragShader(nullptr)
    , m_vertEntry()
    , m_fragEntry()
    , m_materialBindGroupLayout(nullptr)
    , m_paramVersion(1)
    , m_baseColor(Color::white())
    , m_specularColor(Color::white())
    , m_shininess(32.0f)
    , m_materialParamBufferSize(0)
    , m_baseTexture(nullptr)
    , m_cullMode(rhi::CullMode::Back)
    , m_blendEnabled(true)
    , m_depthTestEnabled(true)
    , m_depthWriteEnabled(true) {
}

void Material::setColor(const Color& color) {
    m_baseColor = color;
    markDirty();
}

void Material::setTexture(rhi::Texture* texture) {
    if (texture) {
        texture->addRef();
        m_baseTexture = Ref<rhi::Texture>::adopt(texture);
    } else {
        m_baseTexture.reset();
    }
    markDirty();
}

void Material::setSpecular(const Color& color, f32 shininess) {
    m_specularColor = color;
    m_shininess = shininess;
    markDirty();
}

void Material::writeMaterialUBO(void* dst) const {
    if (m_type == MaterialType::Unlit) {
        UnlitMaterialParamsUBO ubo;
        ubo.color[0] = m_baseColor.r; ubo.color[1] = m_baseColor.g;
        ubo.color[2] = m_baseColor.b; ubo.color[3] = m_baseColor.a;
        std::memcpy(dst, &ubo, sizeof(ubo));
    } else {
        BasicLitMaterialParamsUBO ubo;
        ubo.color[0] = m_baseColor.r; ubo.color[1] = m_baseColor.g;
        ubo.color[2] = m_baseColor.b; ubo.color[3] = m_baseColor.a;
        ubo.specular[0] = m_specularColor.r; ubo.specular[1] = m_specularColor.g;
        ubo.specular[2] = m_specularColor.b; ubo.specular[3] = m_shininess;
        std::memcpy(dst, &ubo, sizeof(ubo));
    }
}

void Material::setBlendEnabled(bool enabled) { m_blendEnabled = enabled; }
void Material::setCullMode(rhi::CullMode mode) { m_cullMode = mode; }
void Material::setDepthTestEnabled(bool enabled) { m_depthTestEnabled = enabled; }
void Material::setDepthWriteEnabled(bool enabled) { m_depthWriteEnabled = enabled; }

// ------ MaterialFactory -----------------------------------------------------------------------------------------------------------------

Result<Ref<Material>> MaterialFactory::createMaterialFromBuiltin(
    GraphicsModule* module, BuiltinShader shader, MaterialType type) {
    const auto& data = module->builtinShader(shader);
    if (!data.initialized) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "Builtin shader not initialized"});
    }

    auto mat = Ref<Material>::adopt(new Material());
    mat->m_type = type;
    mat->m_vertShader = data.vertShader;
    mat->m_fragShader = data.fragShader;
    mat->m_vertEntry = data.vertEntry;
    mat->m_fragEntry = data.fragEntry;
    mat->m_materialBindGroupLayout = data.materialBindGroupLayout;
    mat->m_materialParamBufferSize = data.materialParamBufferSize;
    mat->m_baseTexture = module->whiteTexture();
    return mat;
}

Result<Ref<Material>> MaterialFactory::createUnlit(GraphicsModule* module) {
    return createMaterialFromBuiltin(module, BuiltinShader::Unlit, MaterialType::Unlit);
}

Result<Ref<Material>> MaterialFactory::createBasicLit(GraphicsModule* module) {
    return createMaterialFromBuiltin(module, BuiltinShader::BasicLit, MaterialType::BasicLit);
}

Result<Ref<Material>> MaterialFactory::createUnlit(GraphicsContext* ctx) {
    return createUnlit(ctx->module());
}

Result<Ref<Material>> MaterialFactory::createBasicLit(GraphicsContext* ctx) {
    return createBasicLit(ctx->module());
}

Result<Ref<Material>> MaterialFactory::createStencilMask(GraphicsModule* module) {
    return createMaterialFromBuiltin(module, BuiltinShader::StencilMask, MaterialType::Unlit);
}

Result<Ref<Material>> MaterialFactory::createStencilMask(GraphicsContext* ctx) {
    return createStencilMask(ctx->module());
}

} // namespace ln
