
#include "Internal.hpp"
#include <LuminoGraphics/ShaderCompiler/ShaderHelper.hpp>
#include "ShaderTranspiler.hpp"
#include "MultiCompileCombination.hpp"
#include "UnifiedShaderCompiler.hpp"

namespace ln {
namespace kokage {

#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

//==============================================================================
// UnifiedShaderCompiler

UnifiedShaderCompiler::UnifiedShaderCompiler(detail::ShaderManager* manager, DiagnosticsManager* diag)
    : m_manager(manager)
    , m_diag(diag) {
    m_unifiedShader = makeRef<UnifiedShader>(m_diag);
}

bool UnifiedShaderCompiler::compile(
    char* inputCode,
    size_t inputCodeLength,
    const List<Path>& includeDirectories,
    const List<String>& definitions) {
    if (ShaderModuleParser::checkHasModuleBlock(inputCode, inputCodeLength)) {
        ShaderModuleParser parser;
        if (!parser.parse(inputCode, inputCodeLength, m_diag)) {
            return false;
        }
        m_metadataTechniques = std::move(parser.techniques);

        // @module ~ @end を空白で潰しておく
        memset((inputCode + parser.moduleBegin), ' ', parser.moduleEnd - parser.moduleBegin);
    }
    else {
        HLSLMetadataParser parser;
        parser.parse(inputCode, inputCodeLength, m_diag);
        if (m_diag->hasError()) {
            return false;
        }
        m_metadataTechniques = std::move(parser.techniques);
    }

    // glslang は hlsl の technique ブロックを理解できないので、空白で潰しておく
    for (auto& tech : m_metadataTechniques) {
        memset((inputCode + tech.blockBegin), ' ', tech.blockEnd - tech.blockBegin);
    }

#if 1   // TODO: variant test
    for (auto& metaTech : m_metadataTechniques) {
        MultiCompileCombination multiCompileCombination;
        multiCompileCombination.expand(metaTech.multiCompiles);

        for (const auto& values : multiCompileCombination.variantSets()) {
            UnifiedShaderVariantSet variantSet;
            variantSet.values = values;

            UnifiedShaderTechnique* tech = m_unifiedShader->addVariantTechnique("V", variantSet);

            List<String> actualDefinitions = definitions;
            for (const auto& def : variantSet.values) {
                actualDefinitions.add(String::fromUtf8(def) + U"=1");
            }

            for (auto& metaPass : metaTech.passes) {
                createVSPSPass(tech, metaPass, inputCode, inputCodeLength, includeDirectories, actualDefinitions);
            }
        }
    }
#endif
#if 1

    // Tech と Pass を作り、各 Pass のシェーダを Compile
    for (auto& metaTech : m_metadataTechniques) {
        auto* tech = m_unifiedShader->addTechnique(metaTech.name, metaTech.techniqueClass);

        List<String> actualDefinitions = definitions;
        {
            if (metaTech.techniqueClass.shadingModel == ShaderTechniqueClass_ShadingModel::Default)
                actualDefinitions.add(_TT("LN_SHADINGMODEL_DEFAULT=1"));
            if (metaTech.techniqueClass.shadingModel == ShaderTechniqueClass_ShadingModel::Unlit)
                actualDefinitions.add(_TT("LN_SHADINGMODEL_UNLIT=1"));
            if (metaTech.techniqueClass.meshProcess == ShaderTechniqueClass_MeshProcess::SkinnedMesh)
                actualDefinitions.add(_TT("LN_USE_SKINNING=1"));
            if (metaTech.techniqueClass.normalClass == ShaderTechniqueClass_Normal::NormalMap)
                actualDefinitions.add(_TT("LN_USE_NORMALMAP=1"));
            if (metaTech.techniqueClass.roughnessClass == ShaderTechniqueClass_Roughness::RoughnessMap)
                actualDefinitions.add(_TT("LN_USE_ROUGHNESS_MAP=1"));
            if (metaTech.techniqueClass.drawMode == ShaderTechniqueClass_DrawMode::Instancing)
                actualDefinitions.add(_TT("LN_USE_INSTANCING=1"));
        }

        for (auto& metaPass : metaTech.passes) {
            createVSPSPass(tech, metaPass, inputCode, inputCodeLength, includeDirectories, actualDefinitions);
        }
    }

#else

    // まずは compile と link を行う
    for (auto& tech : m_metadataTechniques) {
        for (auto& pass : tech.passes) {
            // Vertex shader
            {
                auto transpiler = makeURef<ShaderCodeTranspiler>(m_manager);
                transpiler->compileAndLinkFromHlsl(ShaderStage2_Vertex, inputCode, inputCodeLength, pass.vertexShader, includeDirectories, &actualDefinitions, m_diag);
                if (m_diag->hasError()) {
                    return false;
                }
                m_transpilerMap[makeKey(ShaderStage2_Vertex, pass.vertexShader)] = transpiler;
            }

            // Pixel shader
            {
                auto transpiler = makeURef<ShaderCodeTranspiler>(m_manager);
                transpiler->compileAndLinkFromHlsl(ShaderStage2_Fragment, inputCode, inputCodeLength, pass.pixelShader, includeDirectories, &actualDefinitions, m_diag);
                if (m_diag->hasError()) {
                    return false;
                }
                m_transpilerMap[makeKey(ShaderStage2_Fragment, pass.pixelShader)] = transpiler;
            }
        }
    }
#endif

    return true;
}

bool UnifiedShaderCompiler::compileCompute(
    const char* code,
    size_t len,
    const std::string& entryPoint,
    const List<Path>& includeDirectories,
    const List<String>& definitions) {
    ShaderTechniqueClass techClass;
    auto* tech = m_unifiedShader->addTechnique("Compute", techClass);
    auto* pass = m_unifiedShader->addPass(tech->id(), "Compute");

    auto transpiler = makeURef<ShaderCodeTranspiler>(m_manager);
    transpiler->compileAndLinkFromHlsl(ShaderStage2_Compute, code, len, entryPoint, includeDirectories, &definitions, m_diag);
    if (m_diag->hasError()) {
        return false;
    }

    m_unifiedShader->addMergeDescriptorLayoutItem(pass, transpiler->descriptorLayout);

    if (!transpiler->mapIOAndGenerateSpirv(pass->descriptorLayout, m_diag)) {
        return false;
    }

    auto* container = m_unifiedShader->addCodeContainer(ShaderStage2_Compute, entryPoint);

    UnifiedShaderTriple triple1 = { "spv", 110, "" };
    container->setCode(triple1, transpiler->spirvCode());
    UnifiedShaderTriple triple2 = { "hlsl", 5, "" };
    container->setCode(triple2, transpiler->generateHlslByteCode());

    pass->computeShader = container->id();

    return true;
}

bool UnifiedShaderCompiler::compileSingleCodes(
    const char* vsData,
    size_t vsLen,
    const std::string& vsEntryPoint,
    const char* psData,
    size_t psLen,
    const std::string& psEntryPoint,
    const List<Path>& includeDirectories,
    const List<String>& definitions) {
    HLSLTechnique metaTech;
    metaTech.name = "MainTech";

    HLSLPass metaPass;
    metaPass.name = "Pass1";
    metaPass.vertexShader = vsEntryPoint;
    metaPass.pixelShader = psEntryPoint;
    metaPass.renderState = makeRef<ShaderRenderState>();

#if 1
    auto* tech = m_unifiedShader->addTechnique(metaTech.name, metaTech.techniqueClass);
    auto* pass = m_unifiedShader->addPass(tech->id(), metaPass.name);

    // Vertex shader
    {
        auto transpiler = makeURef<ShaderCodeTranspiler>(m_manager);
        transpiler->compileAndLinkFromHlsl(ShaderStage2_Vertex, vsData, vsLen, metaPass.vertexShader, includeDirectories, &definitions, m_diag);
        if (m_diag->hasError()) {
            return false;
        }
        pass->attributes = transpiler->attributes();
        m_unifiedShader->addMergeDescriptorLayoutItem(pass, transpiler->descriptorLayout);
        transpiler->passId = pass->id;

        // 空の CodeContainer を作っておく (実際のコードは最後に格納する)
        auto* container = m_unifiedShader->addCodeContainer(ShaderStage2_Vertex, metaPass.vertexShader);
        pass->vertexShader = container->id();

        pass->vsTranspiler = transpiler;
        m_transpilers.push_back(std::move(transpiler));
    }

    // Pixel shader
    {
        auto transpiler = makeURef<ShaderCodeTranspiler>(m_manager);
        transpiler->compileAndLinkFromHlsl(ShaderStage2_Fragment, psData, psLen, metaPass.pixelShader, includeDirectories, &definitions, m_diag);
        if (m_diag->hasError()) {
            return false;
        }
        m_unifiedShader->addMergeDescriptorLayoutItem(pass, transpiler->descriptorLayout);
        transpiler->passId = pass->id;

        // 空の CodeContainer を作っておく (実際のコードは最後に格納する)
        auto* container = m_unifiedShader->addCodeContainer(ShaderStage2_Fragment, metaPass.pixelShader);
        pass->pixelShader = container->id();

        pass->psTranspiler = transpiler;
        m_transpilers.push_back(std::move(transpiler));
    }

    // ShaderRenderState
    pass->renderState = metaPass.renderState;
#else
    // Vertex shader
    {
        auto transpiler = makeURef<ShaderCodeTranspiler>(m_manager);
        transpiler->compileAndLinkFromHlsl(ShaderStage2_Vertex, vsData, vsLen, vsEntryPoint, includeDirectories, &definitions, m_diag);
        if (m_diag->hasError()) {
            return false;
        }
        m_transpilerMap[makeKey(ShaderStage2_Vertex, vsEntryPoint)] = transpiler;

        pass.vertexShader = vsEntryPoint;
    }

    // Pixel shader
    {
        auto transpiler = makeURef<ShaderCodeTranspiler>(m_manager);
        transpiler->compileAndLinkFromHlsl(ShaderStage2_Fragment, psData, psLen, psEntryPoint, includeDirectories, &definitions, m_diag);
        if (m_diag->hasError()) {
            return false;
        }
        m_transpilerMap[makeKey(ShaderStage2_Fragment, psEntryPoint)] = transpiler;

        pass.pixelShader = psEntryPoint;
    }
#endif

    metaTech.passes.push_back(std::move(metaPass));
    m_metadataTechniques.push_back(std::move(metaTech));

    return true;
}

UnifiedShaderPass* UnifiedShaderCompiler::createVSPSPass(
    UnifiedShaderTechnique* parentTech,
    const HLSLPass& metaPass,
    const char* inputCode,
    size_t inputCodeLength,
    const List<Path>& includeDirectories,
    const List<String>& definitions) {
    auto* pass = m_unifiedShader->addPass(parentTech->id(), metaPass.name);

    // Vertex shader
    {
        auto entryPointName = metaPass.vertexShader;
        if (entryPointName.empty()) entryPointName = "LN_VSMain";

        auto transpiler = makeURef<ShaderCodeTranspiler>(m_manager);
        transpiler->compileAndLinkFromHlsl(ShaderStage2_Vertex, inputCode, inputCodeLength, entryPointName, includeDirectories, &definitions, m_diag);
        if (m_diag->hasError()) {
            return false;
        }
        pass->attributes = transpiler->attributes();
        m_unifiedShader->addMergeDescriptorLayoutItem(pass, transpiler->descriptorLayout);
        transpiler->passId = pass->id;

        // 空の CodeContainer を作っておく (実際のコードは最後に格納する)
        auto container = m_unifiedShader->addCodeContainer(ShaderStage2_Vertex, entryPointName);
        pass->vertexShader = container->id();

        pass->vsTranspiler = transpiler;
        m_transpilers.push_back(std::move(transpiler));
    }

    // Pixel shader
    {
        auto entryPointName = metaPass.pixelShader;
        if (entryPointName.empty()) entryPointName = "LN_PSMain";

        auto transpiler = makeURef<ShaderCodeTranspiler>(m_manager);
        transpiler->compileAndLinkFromHlsl(ShaderStage2_Fragment, inputCode, inputCodeLength, entryPointName, includeDirectories, &definitions, m_diag);
        if (m_diag->hasError()) {
            return false;
        }
        m_unifiedShader->addMergeDescriptorLayoutItem(pass, transpiler->descriptorLayout);
        transpiler->passId = pass->id;

        // 空の CodeContainer を作っておく (実際のコードは最後に格納する)
        auto container = m_unifiedShader->addCodeContainer(ShaderStage2_Fragment, entryPointName);
        pass->pixelShader = container->id();

        pass->psTranspiler = transpiler;
        m_transpilers.push_back(std::move(transpiler));
    }

    // ShaderRenderState
    pass->renderState = metaPass.renderState;
}

bool UnifiedShaderCompiler::link() {

    //// vertex shader の最大 binding 数を求める
    //size_t maxVertexShaderBindingCounts[DescriptorType_Count] = {};
    //for (auto& pair : m_transpilerMap) {
    //	if (pair.second->stage() == ShaderStage2_Vertex) {
    //		maxVertexShaderBindingCounts[DescriptorType_UniformBuffer] = std::max(maxVertexShaderBindingCounts[DescriptorType_UniformBuffer], pair.second->descriptorLayout.uniformBufferRegister.size());
    //		maxVertexShaderBindingCounts[DescriptorType_Texture] = std::max(maxVertexShaderBindingCounts[DescriptorType_Texture], pair.second->descriptorLayout.textureRegister.size());
    //		maxVertexShaderBindingCounts[DescriptorType_SamplerState] = std::max(maxVertexShaderBindingCounts[DescriptorType_SamplerState], pair.second->descriptorLayout.samplerRegister.size());
    //	}
    //}
    //// 求めた maxVertexShaderBindingCount を PixelShader の binding の開始値としてマッピングする
    //for (auto& pair : m_transpilerMap) {
    //	if (pair.second->stage() == ShaderStage2_Vertex) {
    //		for (size_t i = 0; i < pair.second->descriptorLayout.uniformBufferRegister.size(); i++) {
    //			pair.second->descriptorLayout.uniformBufferRegister[i].binding = i;
    //			m_unifiedShader->addMergeDescriptorLayoutItem(DescriptorType_UniformBuffer, pair.second->descriptorLayout.uniformBufferRegister[i]);
    //		}
    //		for (size_t i = 0; i < pair.second->descriptorLayout.textureRegister.size(); i++) {
    //			pair.second->descriptorLayout.textureRegister[i].binding = i;
    //			m_unifiedShader->addMergeDescriptorLayoutItem(DescriptorType_Texture, pair.second->descriptorLayout.textureRegister[i]);
    //		}
    //		for (size_t i = 0; i < pair.second->descriptorLayout.samplerRegister.size(); i++) {
    //			pair.second->descriptorLayout.samplerRegister[i].binding = i;
    //			m_unifiedShader->addMergeDescriptorLayoutItem(DescriptorType_SamplerState, pair.second->descriptorLayout.samplerRegister[i]);
    //		}
    //	}
    //	if (pair.second->stage() == ShaderStage2_Fragment) {
    //		for (size_t i = 0; i < pair.second->descriptorLayout.uniformBufferRegister.size(); i++) {
    //			pair.second->descriptorLayout.uniformBufferRegister[i].binding = maxVertexShaderBindingCounts[DescriptorType_UniformBuffer] + i;
    //			m_unifiedShader->addMergeDescriptorLayoutItem(DescriptorType_UniformBuffer, pair.second->descriptorLayout.uniformBufferRegister[i]);
    //		}
    //		for (size_t i = 0; i < pair.second->descriptorLayout.textureRegister.size(); i++) {
    //			pair.second->descriptorLayout.textureRegister[i].binding = maxVertexShaderBindingCounts[DescriptorType_Texture] + i;
    //			m_unifiedShader->addMergeDescriptorLayoutItem(DescriptorType_Texture, pair.second->descriptorLayout.textureRegister[i]);
    //		}
    //		for (size_t i = 0; i < pair.second->descriptorLayout.samplerRegister.size(); i++) {
    //			pair.second->descriptorLayout.samplerRegister[i].binding = maxVertexShaderBindingCounts[DescriptorType_SamplerState] + i;
    //			m_unifiedShader->addMergeDescriptorLayoutItem(DescriptorType_SamplerState, pair.second->descriptorLayout.samplerRegister[i]);
    //		}
    //	}
    //}

    for (auto& transpiler : m_transpilers) {
        if (!transpiler->mapIOAndGenerateSpirv(m_unifiedShader->pass(transpiler->passId)->descriptorLayout, m_diag)) {
            return false;
        }
    }

    for (const auto& tech : m_unifiedShader->techniques()) {
        for (int iPass = 0; iPass < tech->passes.length(); iPass++) {
            UnifiedShader::PassId passId = tech->passes[iPass];
            const auto* pass = m_unifiedShader->pass(passId);

            // VertexShader
            {
                CodeContainerId containerId = pass->vertexShader;
                auto* container = m_unifiedShader->codeContainer(containerId);

                auto* tp = pass->vsTranspiler;
                UnifiedShaderTriple triple1 = { "spv", 110, "" };
                container->setCode(triple1, tp->spirvCode());
                UnifiedShaderTriple triple2 = { "hlsl", 5, "" };
                container->setCode(triple2, tp->generateHlslByteCode());
                UnifiedShaderTriple triple3 = { "glsl", 400, "" };
                container->setCode(triple3, tp->generateGlsl(400, false));
                UnifiedShaderTriple triple4 = { "glsl", 300, "es" };
                container->setCode(triple4, tp->generateGlsl(300, true));
            }

            // PixelShader
            {
                CodeContainerId containerId = pass->pixelShader;
                auto* container = m_unifiedShader->codeContainer(containerId);

                auto* tp = pass->psTranspiler;
                UnifiedShaderTriple triple1 = { "spv", 110, "" };
                container->setCode(triple1, tp->spirvCode());
                UnifiedShaderTriple triple2 = { "hlsl", 5, "" };
                container->setCode(triple2, tp->generateHlslByteCode());
                UnifiedShaderTriple triple3 = { "glsl", 400, "" };
                container->setCode(triple3, tp->generateGlsl(400, false));
                UnifiedShaderTriple triple4 = { "glsl", 300, "es" };
                container->setCode(triple4, tp->generateGlsl(300, true));
            }
        }
    }

    return true;
}

#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

} // namespace kokage
} // namespace ln
