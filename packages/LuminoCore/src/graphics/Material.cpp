#include <LuminoCore/graphics/Material.hpp>
#include <LuminoCore/graphics/GraphicsContext.hpp>
#include <LuminoCore/graphics/GraphicsModule.hpp>
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/UnifiedShaderSerializer2.hpp>
#include "ShaderUtils.hpp"
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

Result<Ref<Material>> MaterialFactory::createFromCompiledShader(
    GraphicsModule* module, const void* data, size_t size) {
    using namespace detail;

    // Deserialize the unified shader from the binary blob.
    auto loadResult = shader::UnifiedShaderSerializer2::loadFromData(data, size);
    if (!loadResult) return tl::make_unexpected(loadResult.error());
    auto unifiedShader = std::move(*loadResult);

    // Find the first pass for SPIR-V target.
    auto& globalPasses = unifiedShader->globalShaderPasses();
    if (globalPasses.empty()) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "No shader passes found"});
    }

    auto* globalPass = globalPasses[0].get();
    auto targetPassId = globalPass->getTargetShaderPassId(shader::ShaderTarget_SPIRV);
    auto* targetPass = unifiedShader->targetShaderPass(targetPassId);
    if (!targetPass) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "No SPIRV target pass"});
    }

    auto* vertEP = unifiedShader->targetEntryPoint(targetPass->vertEntryPointId);
    auto* fragEP = unifiedShader->targetEntryPoint(targetPass->fragEntryPointId);
    if (!vertEP || !fragEP) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "Missing entry points"});
    }

    auto* vertBlob = unifiedShader->blob(vertEP->codeBlobId);
    auto* fragBlob = unifiedShader->blob(fragEP->codeBlobId);

    // Create shader modules
    auto* device = module->device();

    rhi::ShaderModuleDesc vsDesc;
    vsDesc.spirvCode = reinterpret_cast<const u32*>(vertBlob->data.data());
    vsDesc.spirvSizeBytes = vertBlob->data.size();
    auto vsResult = device->createShaderModule(vsDesc);
    if (!vsResult) return tl::make_unexpected(vsResult.error());

    rhi::ShaderModuleDesc fsDesc;
    fsDesc.spirvCode = reinterpret_cast<const u32*>(fragBlob->data.data());
    fsDesc.spirvSizeBytes = fragBlob->data.size();
    auto fsResult = device->createShaderModule(fsDesc);
    if (!fsResult) return tl::make_unexpected(fsResult.error());

    // Build material BindGroupLayout from "materialData" ParameterBlock reflection
    auto* materialBlock = findParameterBlock(unifiedShader.get(), "materialData");
    if (!materialBlock) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "No 'materialData' ParameterBlock found"});
    }

    auto bglDesc = buildBindGroupLayoutFromReflection(*materialBlock, targetPass->bindingLayout);
    auto bglResult = device->createBindGroupLayout(bglDesc);
    if (!bglResult) return tl::make_unexpected(bglResult.error());

    // Get material CB size from reflection
    int16_t cbSize = findConstantBufferSize(*materialBlock);
    if (cbSize <= 0) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "No constant buffer found in 'materialData'"});
    }

    // Create the Material
    auto mat = Ref<Material>::adopt(new Material());
    mat->m_type = MaterialType::Unlit;
    mat->m_vertShader = std::move(*vsResult);
    mat->m_fragShader = std::move(*fsResult);
    mat->m_vertEntry = vertEP->name;
    mat->m_fragEntry = fragEP->name;
    mat->m_materialBindGroupLayout = std::move(*bglResult);
    mat->m_materialParamBufferSize = static_cast<u64>(cbSize);
    mat->m_baseTexture = module->whiteTexture();
    return mat;
}

Result<Ref<Material>> MaterialFactory::createFromCompiledShader(
    GraphicsContext* ctx, const void* data, size_t size) {
    return createFromCompiledShader(ctx->module(), data, size);
}

} // namespace ln
