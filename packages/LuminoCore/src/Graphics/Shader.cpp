#include <LuminoCore/Graphics/Shader.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/Graphics/GraphicsModule.hpp>
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/UnifiedShaderSerializer2.hpp>
#include "ShaderUtils.hpp"

#ifdef LUMINO_USE_SLANG
#include <LuminoShader/ShaderCompiler2.hpp>
#include <filesystem>
#include <iostream>
#endif // LUMINO_USE_SLANG

namespace ln {

ShaderPass* Shader::findPass(const std::string& name) const {
    for (const auto& pass : m_passes) {
        if (pass->passName() == name) return pass.get();
    }
    return nullptr;
}

//------------------------------------------------------------------------------
Result<Ref<Shader>> Shader::buildFromUnifiedShader(
    shader::UnifiedShader2* unifiedShader, rhi::Device* device,
    const std::string& shaderName) {

    auto& globalPasses = unifiedShader->globalShaderPasses();
    if (globalPasses.empty()) {
        const std::string& name = shaderName.empty() ? unifiedShader->sourceName() : shaderName;
        return LN_MAKE_ERROR(
            "No shader passes found (shader: %s)",
            name.empty() ? "(unnamed shader)" : name.c_str());
    }

    auto shader = Ref<Shader>::adopt(new Shader());
    shader->m_passes.reserve(globalPasses.size());
    for (size_t i = 0; i < globalPasses.size(); ++i) {
        auto passResult = ShaderPass::buildFromUnifiedShader(unifiedShader, device, i, shaderName);
        if (!passResult) return LN_FORWARD_ERROR(passResult);
        shader->m_passes.push_back(std::move(*passResult));
    }
    return shader;
}

//------------------------------------------------------------------------------
Result<Ref<Shader>> Shader::createFromCompiledShader(
    GraphicsModule* module, const void* data, size_t size,
    const std::string& shaderName) {
    // デシリアライズはパスの数に関係なく 1 回だけ行う。
    auto loadResult = shader::UnifiedShaderSerializer2::loadFromData(
        data, size, detail::backendToShaderTarget(module->device()->backend()));
    if (!loadResult) return LN_FORWARD_ERROR(loadResult);
    auto unifiedShader = std::move(*loadResult);

    return buildFromUnifiedShader(unifiedShader.get(), module->device(), shaderName);
}

Result<Ref<Shader>> Shader::createFromCompiledShader(
    GraphicsContext* ctx, const void* data, size_t size,
    const std::string& shaderName) {
    return createFromCompiledShader(ctx->module(), data, size, shaderName);
}

//------------------------------------------------------------------------------
Result<Ref<Shader>> Shader::createFromShaderSourceFile(
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

    return buildFromUnifiedShader(compiler->shader(), ctx->module()->device());
#else
    (void)ctx; (void)shaderFilePath; (void)searchPath;
    return LN_MAKE_ERROR("Shader::createFromShaderSourceFile requires LUMINO_USE_SLANG");
#endif // LUMINO_USE_SLANG
}

} // namespace ln
