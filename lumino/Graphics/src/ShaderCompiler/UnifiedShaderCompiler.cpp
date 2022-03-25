
#include "Internal.hpp"
#include <LuminoShaderCompiler/ShaderHelper.hpp>
#include "ShaderTranspiler.hpp"
#include "UnifiedShaderCompiler.hpp"

namespace ln {
namespace detail {

#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

//==============================================================================
// UnifiedShaderCompiler

UnifiedShaderCompiler::UnifiedShaderCompiler(ShaderManager* manager, DiagnosticsManager* diag)
	: m_manager(manager)
	, m_diag(diag)
{
    m_unifiedShader = makeRef<UnifiedShader>(m_diag);
}

bool UnifiedShaderCompiler::compile(
	char* inputCode, size_t inputCodeLength,
	const List<Path>& includeDirectories, const List<String>& definitions)
{
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

#if 1

    // Tech と Pass を作り、各 Pass のシェーダを Compile
    for (auto& tech : m_metadataTechniques)
    {
        UnifiedShader::TechniqueId techId;
        if (!m_unifiedShader->addTechnique(tech.name, tech.techniqueClass, &techId)) {
            return false;
        }

		List<String> actualDefinitions = definitions;
		{
			if (tech.techniqueClass.shadingModel == ShaderTechniqueClass_ShadingModel::Default)
				actualDefinitions.add(_TT("LN_SHADINGMODEL_DEFAULT=1"));
			if (tech.techniqueClass.shadingModel == ShaderTechniqueClass_ShadingModel::Unlit)
				actualDefinitions.add(_TT("LN_SHADINGMODEL_UNLIT=1"));
			if (tech.techniqueClass.meshProcess == ShaderTechniqueClass_MeshProcess::SkinnedMesh)
				actualDefinitions.add(_TT("LN_USE_SKINNING=1"));
			if (tech.techniqueClass.normalClass == ShaderTechniqueClass_Normal::NormalMap)
				actualDefinitions.add(_TT("LN_USE_NORMALMAP=1"));
			if (tech.techniqueClass.roughnessClass == ShaderTechniqueClass_Roughness::RoughnessMap)
				actualDefinitions.add(_TT("LN_USE_ROUGHNESS_MAP=1"));
			if (tech.techniqueClass.drawMode == ShaderTechniqueClass_DrawMode::Instancing)
				actualDefinitions.add(_TT("LN_USE_INSTANCING=1"));
		}


        for (auto& pass : tech.passes)
        {
            UnifiedShader::PassId passId;
            if (!m_unifiedShader->addPass(techId, pass.name, &passId)) {
                return false;
            }

            // Vertex shader
            {
				auto entryPointName = pass.vertexShader;
				if (entryPointName.empty()) entryPointName = "LN_VSMain";

                auto transpiler = std::make_shared<ShaderCodeTranspiler>(m_manager);
                transpiler->compileAndLinkFromHlsl(ShaderStage2_Vertex, inputCode, inputCodeLength, entryPointName, includeDirectories, &actualDefinitions, m_diag);
                if (m_diag->hasError()) {
                    return false;
                }
                m_unifiedShader->setAttributes(passId, transpiler->attributes());
				m_unifiedShader->addMergeDescriptorLayoutItem(passId, transpiler->descriptorLayout);
				transpiler->passId = passId;
                m_transpilerMap[makeKey2(tech.name, pass.name, ShaderStage2_Vertex, entryPointName)] = transpiler;

                // 空の CodeContainer を作っておく (実際のコードは最後に格納する)
                UnifiedShader::CodeContainerId containerId;
                if (!m_unifiedShader->addCodeContainer(ShaderStage2_Vertex, entryPointName, &containerId)) {
                    return false;
                }
                m_unifiedShader->setVertexShader(passId, containerId);
            }

            // Pixel shader
            {
				auto entryPointName = pass.pixelShader;
				if (entryPointName.empty()) entryPointName = "LN_PSMain";

                auto transpiler = std::make_shared<ShaderCodeTranspiler>(m_manager);
                transpiler->compileAndLinkFromHlsl(ShaderStage2_Fragment, inputCode, inputCodeLength, entryPointName, includeDirectories, &actualDefinitions, m_diag);
                if (m_diag->hasError()) {
                    return false;
                }
				m_unifiedShader->addMergeDescriptorLayoutItem(passId, transpiler->descriptorLayout);
				transpiler->passId = passId;
                m_transpilerMap[makeKey2(tech.name, pass.name, ShaderStage2_Fragment, entryPointName)] = transpiler;

                // 空の CodeContainer を作っておく (実際のコードは最後に格納する)
                UnifiedShader::CodeContainerId containerId;
                if (!m_unifiedShader->addCodeContainer(ShaderStage2_Fragment, entryPointName, &containerId)) {
                    return false;
                }
                m_unifiedShader->setPixelShader(passId, containerId);
            }

            // ShaderRenderState
            m_unifiedShader->setRenderState(passId, pass.renderState);
        }
    }


#else

	// まずは compile と link を行う
	for (auto& tech : m_metadataTechniques)
	{
		for (auto& pass : tech.passes)
		{
			// Vertex shader
			{
				auto transpiler = std::make_shared<ShaderCodeTranspiler>(m_manager);
				transpiler->compileAndLinkFromHlsl(ShaderStage2_Vertex, inputCode, inputCodeLength, pass.vertexShader, includeDirectories, &actualDefinitions, m_diag);
				if (m_diag->hasError()) {
					return false;
				}
				m_transpilerMap[makeKey(ShaderStage2_Vertex, pass.vertexShader)] = transpiler;
			}

			// Pixel shader
			{
				auto transpiler = std::make_shared<ShaderCodeTranspiler>(m_manager);
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
	const char* code, size_t len, const std::string& entryPoint,
	const List<Path>& includeDirectories, const List<String>& definitions)
{
	ShaderTechniqueClass techClass;
	UnifiedShader::TechniqueId techId;
	if (!m_unifiedShader->addTechnique("Compute", techClass, &techId)) {
		return false;
	}

	UnifiedShader::PassId passId;
	if (!m_unifiedShader->addPass(techId, "Compute", &passId)) {
		return false;
	}

	auto transpiler = std::make_shared<ShaderCodeTranspiler>(m_manager);
	transpiler->compileAndLinkFromHlsl(ShaderStage2_Compute, code, len, entryPoint, includeDirectories, &definitions, m_diag);
	if (m_diag->hasError()) {
		return false;
	}

	m_unifiedShader->addMergeDescriptorLayoutItem(passId, transpiler->descriptorLayout);

	if (!transpiler->mapIOAndGenerateSpirv(m_unifiedShader->descriptorLayout(passId), m_diag)) {
		return false;
	}

	UnifiedShader::CodeContainerId containerId;
	if (!m_unifiedShader->addCodeContainer(ShaderStage2_Compute, entryPoint, &containerId)) {
		return false;
	}

	UnifiedShaderTriple triple1 = { "spv", 110, "" };
	m_unifiedShader->setCode(containerId, triple1, transpiler->spirvCode());
	UnifiedShaderTriple triple2 = { "hlsl", 5, "" };
	m_unifiedShader->setCode(containerId, triple2, transpiler->generateHlslByteCode());

	m_unifiedShader->setComputeShader(passId, containerId);

	return true;
}

bool UnifiedShaderCompiler::compileSingleCodes(
	const char* vsData, size_t vsLen, const std::string& vsEntryPoint,
	const char* psData, size_t psLen, const std::string& psEntryPoint,
	const List<Path>& includeDirectories, const List<String>& definitions)
{
	HLSLTechnique tech;
	tech.name = "MainTech";

	HLSLPass pass;
	pass.name = "Pass1";
    pass.vertexShader = vsEntryPoint;
    pass.pixelShader = psEntryPoint;
	pass.renderState = makeRef<ShaderRenderState>();

#if 1

    UnifiedShader::TechniqueId techId;
    if (!m_unifiedShader->addTechnique(tech.name, tech.techniqueClass, &techId)) {
        return false;
    }

    UnifiedShader::PassId passId;
    if (!m_unifiedShader->addPass(techId, pass.name, &passId)) {
        return false;
    }

    // Vertex shader
    {
        auto transpiler = std::make_shared<ShaderCodeTranspiler>(m_manager);
        transpiler->compileAndLinkFromHlsl(ShaderStage2_Vertex, vsData, vsLen, pass.vertexShader, includeDirectories, &definitions, m_diag);
        if (m_diag->hasError()) {
            return false;
        }
        m_unifiedShader->setAttributes(passId, transpiler->attributes());
		m_unifiedShader->addMergeDescriptorLayoutItem(passId, transpiler->descriptorLayout);
		transpiler->passId = passId;
        m_transpilerMap[makeKey2(tech.name, pass.name, ShaderStage2_Vertex, pass.vertexShader)] = transpiler;

        // 空の CodeContainer を作っておく (実際のコードは最後に格納する)
        UnifiedShader::CodeContainerId containerId;
        if (!m_unifiedShader->addCodeContainer(ShaderStage2_Vertex, pass.vertexShader, &containerId)) {
            return false;
        }
        m_unifiedShader->setVertexShader(passId, containerId);
    }

    // Pixel shader
    {
        auto transpiler = std::make_shared<ShaderCodeTranspiler>(m_manager);
        transpiler->compileAndLinkFromHlsl(ShaderStage2_Fragment, psData, psLen, pass.pixelShader, includeDirectories, &definitions, m_diag);
        if (m_diag->hasError()) {
            return false;
        }
		m_unifiedShader->addMergeDescriptorLayoutItem(passId, transpiler->descriptorLayout);
		transpiler->passId = passId;
        m_transpilerMap[makeKey2(tech.name, pass.name, ShaderStage2_Fragment, pass.pixelShader)] = transpiler;

        // 空の CodeContainer を作っておく (実際のコードは最後に格納する)
        UnifiedShader::CodeContainerId containerId;
        if (!m_unifiedShader->addCodeContainer(ShaderStage2_Fragment, pass.pixelShader, &containerId)) {
            return false;
        }
        m_unifiedShader->setPixelShader(passId, containerId);
    }

    // ShaderRenderState
    m_unifiedShader->setRenderState(passId, pass.renderState);
#else
	// Vertex shader
	{
		auto transpiler = std::make_shared<ShaderCodeTranspiler>(m_manager);
		transpiler->compileAndLinkFromHlsl(ShaderStage2_Vertex, vsData, vsLen, vsEntryPoint, includeDirectories, &definitions, m_diag);
		if (m_diag->hasError()) {
			return false;
		}
		m_transpilerMap[makeKey(ShaderStage2_Vertex, vsEntryPoint)] = transpiler;

		pass.vertexShader = vsEntryPoint;
	}

	// Pixel shader
	{
		auto transpiler = std::make_shared<ShaderCodeTranspiler>(m_manager);
		transpiler->compileAndLinkFromHlsl(ShaderStage2_Fragment, psData, psLen, psEntryPoint, includeDirectories, &definitions, m_diag);
		if (m_diag->hasError()) {
			return false;
		}
		m_transpilerMap[makeKey(ShaderStage2_Fragment, psEntryPoint)] = transpiler;

		pass.pixelShader = psEntryPoint;
	}
#endif

    tech.passes.push_back(std::move(pass));
	m_metadataTechniques.push_back(std::move(tech));


	return true;
}

//bool createTechPassCodeContainer()
//{
//}

bool UnifiedShaderCompiler::link()
{













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

	for (auto& pair : m_transpilerMap) {
		auto& tp = pair.second;
		if (!tp->mapIOAndGenerateSpirv(m_unifiedShader->descriptorLayout(tp->passId), m_diag)) {
			return false;
		}

		//{
		//	UnifiedShaderTriple triple = { "spv", 110, "" };
		//	if (!m_unifiedShader->hasCode(tp->stage(), tp->entryPoint(), triple)) {
		//		m_unifiedShader->setCode(tp->stage(), tp->entryPoint(), triple, tp->spirvCode(), tp->refrection());
		//	}
		//}

		//{
		//	UnifiedShaderTriple triple = { "glsl", 400, "" };
		//	if (!m_unifiedShader->hasCode(tp->stage(), tp->entryPoint(), triple)) {
		//		m_unifiedShader->setCode(tp->stage(), tp->entryPoint(), triple, tp->generateGlsl(400, false), makeRef<UnifiedShaderRefrectionInfo>());
		//	}
		//}

		//{
		//	UnifiedShaderTriple triple = { "glsl", 300, "es" };
		//	if (!m_unifiedShader->hasCode(tp->stage(), tp->entryPoint(), triple)) {
		//		m_unifiedShader->setCode(tp->stage(), tp->entryPoint(), triple, tp->generateGlsl(300, true), makeRef<UnifiedShaderRefrectionInfo>());
		//	}
		//}
	}

    for (int iTech = 0; iTech < m_unifiedShader->techniqueCount(); iTech++)
    {
        UnifiedShader::TechniqueId techId = m_unifiedShader->techniqueId(iTech);
        
        for (int iPass = 0; iPass < m_unifiedShader->getPassCountInTechnique(techId); iPass++)
        {
            UnifiedShader::PassId passId = m_unifiedShader->getPassIdInTechnique(techId, iPass);

            // VertexShader
            {
                UnifiedShader::CodeContainerId containerId = m_unifiedShader->vertexShader(passId);

                auto& tp = m_transpilerMap[makeKey2(m_unifiedShader->techniqueName(techId), m_unifiedShader->passName(passId), ShaderStage2_Vertex, m_unifiedShader->entryPointName(containerId))];
                
                UnifiedShaderTriple triple1 = { "spv", 110, "" };
                m_unifiedShader->setCode(containerId, triple1, tp->spirvCode());
				UnifiedShaderTriple triple2 = { "hlsl", 5, "" };
				m_unifiedShader->setCode(containerId, triple2, tp->generateHlslByteCode());
				UnifiedShaderTriple triple3 = { "glsl", 400, "" };
				m_unifiedShader->setCode(containerId, triple3, tp->generateGlsl(400, false));
				UnifiedShaderTriple triple4 = { "glsl", 300, "es" };
				m_unifiedShader->setCode(containerId, triple4, tp->generateGlsl(300, true));
            }

            // PixelShader
            {
                UnifiedShader::CodeContainerId containerId = m_unifiedShader->pixelShader(passId);

                auto& tp = m_transpilerMap[makeKey2(m_unifiedShader->techniqueName(techId), m_unifiedShader->passName(passId), ShaderStage2_Fragment, m_unifiedShader->entryPointName(containerId))];

                UnifiedShaderTriple triple1 = { "spv", 110, "" };
                m_unifiedShader->setCode(containerId, triple1, tp->spirvCode());
				UnifiedShaderTriple triple2 = { "hlsl", 5, "" };
				m_unifiedShader->setCode(containerId, triple2, tp->generateHlslByteCode());
				UnifiedShaderTriple triple3 = { "glsl", 400, "" };
				m_unifiedShader->setCode(containerId, triple3, tp->generateGlsl(400, false));
				UnifiedShaderTriple triple4 = { "glsl", 300, "es" };
				m_unifiedShader->setCode(containerId, triple4, tp->generateGlsl(300, true));
            }
        }
    }


	//// Tech と Pass を作る
	//for (auto& tech : m_metadataTechniques)
	//{
	//	UnifiedShader::TechniqueId techId;
	//	if (!m_unifiedShader->addTechnique(tech.name, &techId)) {
	//		return false;
	//	}

	//	for (auto& pass : tech.passes)
	//	{
	//		UnifiedShader::CodeContainerId codeId;
	//		UnifiedShader::PassId passId;
	//		if (!m_unifiedShader->addPass(techId, pass.name, &passId)) {
	//			return false;
	//		}

	//		// VertexShader
	//		if (!m_unifiedShader->findCodeContainer(ShaderStage2_Vertex, pass.vertexShader, &codeId)) {
	//			return false;
	//		}
	//		m_unifiedShader->setVertexShader(passId, codeId);

	//		// PixelShader
	//		if (!m_unifiedShader->findCodeContainer(ShaderStage2_Fragment, pass.pixelShader, &codeId)) {
	//			return false;
	//		}
	//		m_unifiedShader->setPixelShader(passId, codeId);

	//		// ShaderRenderState
	//		m_unifiedShader->setRenderState(passId, pass.renderState);
	//	}
	//}

	return true;
}

std::string UnifiedShaderCompiler::makeKey(ShaderStage2 stage, const std::string& entryPoint)
{
	switch (stage)
	{
	case ShaderStage2_Vertex:
		return "1v:" + entryPoint;
	case ShaderStage2_Fragment:
		return "2p:" + entryPoint;
	default:
		LN_UNREACHABLE();
		return std::string();
	}
}

std::string UnifiedShaderCompiler::makeKey2(const std::string& techName, const std::string& passName, ShaderStage2 stage, const std::string& entryPoint)
{
    const char* stageName = nullptr;
    switch (stage)
    {
    case ShaderStage2_Vertex:
        stageName = "VS";
        break;
    case ShaderStage2_Fragment:
        stageName = "PS";
        break;
    default:
        LN_UNREACHABLE();
        return std::string();
    }

    return techName + passName + stageName + entryPoint;
}

#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

} // namespace detail
} // namespace ln
