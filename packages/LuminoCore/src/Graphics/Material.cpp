#include <LuminoCore/Graphics/Material.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/Graphics/GraphicsModule.hpp>
#include <LuminoCore/Graphics/ShaderPass.hpp>
#include <cstring>

namespace ln {

// ------ Material ------------------------------------------------------------------------------------------------------------------------

Material::Material()
    : Object()
    , m_shaderPass(nullptr)
    , m_paramVersion(1)
    , m_baseColor(Color::white())
    , m_baseTexture(nullptr)
    , m_cullMode(rhi::CullMode::Back)
    , m_blendEnabled(true)
    , m_depthTestEnabled(true)
    , m_depthWriteEnabled(true) {
}

int Material::findMemberOffset(const std::string& name) const {
    if (!m_shaderPass) return -1;
    for (const auto& member : m_shaderPass->materialMembers()) {
        if (member.name == name) return member.offset;
    }
    return -1;
}

void Material::setFloat4(const std::string& name, const f32* values) {
    int offset = findMemberOffset(name);
    if (offset < 0 || static_cast<size_t>(offset) + sizeof(f32) * 4 > m_paramBuffer.size()) return;
    std::memcpy(m_paramBuffer.data() + offset, values, sizeof(f32) * 4);
    markDirty();
}

void Material::setFloat(const std::string& name, f32 value) {
    int offset = findMemberOffset(name);
    if (offset < 0 || static_cast<size_t>(offset) + sizeof(f32) > m_paramBuffer.size()) return;
    std::memcpy(m_paramBuffer.data() + offset, &value, sizeof(f32));
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
    if (m_paramBuffer.size() >= sizeof(f32) * 4) {
        f32 rgba[4] = { color.r, color.g, color.b, color.a };
        std::memcpy(m_paramBuffer.data(), rgba, sizeof(rgba));
        markDirty();
    }
}

void Material::setSpecular(const Color& color, f32 shininess) {
    // Write to "specular" field at offset 16 (after float4 color)
    if (m_paramBuffer.size() >= sizeof(f32) * 8) {
        f32 spec[4] = { color.r, color.g, color.b, shininess };
        std::memcpy(m_paramBuffer.data() + sizeof(f32) * 4, spec, sizeof(spec));
        markDirty();
    }
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

void Material::writeMaterialUBO(void* dst) const {
    if (!m_paramBuffer.empty()) {
        std::memcpy(dst, m_paramBuffer.data(), m_paramBuffer.size());
    }
}

void Material::setBlendEnabled(bool enabled) { m_blendEnabled = enabled; }
void Material::setCullMode(rhi::CullMode mode) { m_cullMode = mode; }
void Material::setDepthTestEnabled(bool enabled) { m_depthTestEnabled = enabled; }
void Material::setDepthWriteEnabled(bool enabled) { m_depthWriteEnabled = enabled; }

// ------ MaterialFactory -----------------------------------------------------------------------------------------------------------------

Result<Ref<Material>> MaterialFactory::createMaterialFromBuiltin(
    GraphicsModule* module, BuiltinShader shader) {
    const auto& shaderPass = module->builtinShader(shader);
    if (!shaderPass) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "Builtin shader not initialized"});
    }

    auto mat = Ref<Material>::adopt(new Material());
    mat->m_shaderPass = shaderPass;
    mat->m_baseTexture = module->whiteTexture();

    // Initialize param buffer to the size of the material CB, filled with zeros
    auto bufSize = shaderPass->materialParamBufferSize();
    if (bufSize > 0) {
        mat->m_paramBuffer.resize(static_cast<size_t>(bufSize), 0);
        // Set default color to white (first float4)
        if (bufSize >= sizeof(f32) * 4) {
            f32 white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            std::memcpy(mat->m_paramBuffer.data(), white, sizeof(white));
        }
    }

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
    auto shaderPassResult = ShaderPass::createFromCompiledShader(
        data, size,
        module->viewLayoutDesc(), module->sceneLayoutDesc(), module->objectLayoutDesc(),
        module->viewSetIndex(), module->sceneSetIndex(), module->objectSetIndex(),
        module->device());
    if (!shaderPassResult) return tl::make_unexpected(shaderPassResult.error());

    // Create the Material
    auto mat = Ref<Material>::adopt(new Material());
    mat->m_shaderPass = std::move(*shaderPassResult);
    mat->m_baseTexture = module->whiteTexture();

    // Initialize param buffer
    auto bufSize = mat->m_shaderPass->materialParamBufferSize();
    if (bufSize > 0) {
        mat->m_paramBuffer.resize(static_cast<size_t>(bufSize), 0);
        if (bufSize >= sizeof(f32) * 4) {
            f32 white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            std::memcpy(mat->m_paramBuffer.data(), white, sizeof(white));
        }
    }

    return mat;
}

Result<Ref<Material>> MaterialFactory::createFromCompiledShader(
    GraphicsContext* ctx, const void* data, size_t size) {
    return createFromCompiledShader(ctx->module(), data, size);
}

} // namespace ln
