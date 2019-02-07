
#include "../../src/LuminoEngine/src/Shader/ShaderManager.hpp"
#include "../../src/LuminoEngine/src/Shader/ShaderTranspiler.hpp"
#include "../../src/LuminoEngine/src/Shader/HLSLMetadataParser.hpp"
#include "../../src/LuminoEngine/src/Shader/UnifiedShader.hpp"
#include "Project.hpp"
#include "FxcCommand.hpp"

/*
    ## "Tech" Chunc
    Json Data.


    ## "HLSL" Chunc
    バイトコードでいいかな。直近では必要ないので後で考える。

    ## "GLSL"
    パラメータとして名前を持つ。
    もし OpenGL として動作していなければ読み飛ばしてよい。


*/


int FxcCommand::execute(const ln::Path& inputFile)
{
	ln::detail::ShaderManager::Settings settings;
    m_manager = ln::makeRef<ln::detail::ShaderManager>();
    m_manager->init(settings);

	m_diag = ln::newObject<ln::DiagnosticsManager>();

	bool result = generate(inputFile);

	m_diag->dumpToLog();

    m_manager->dispose();

	return ((!result) || m_diag->hasError()) ? 1 : 0;
}

bool FxcCommand::generate(const ln::Path& inputFile)
{
	ln::Path inputFilePath = inputFile.canonicalize();
	ln::Path outputFilePath = outputFile;
	if (outputFilePath.isEmpty()) {
		outputFilePath = inputFilePath.replaceExtension(ln::detail::UnifiedShader::FileExt);
	}

    ln::List<ln::Path> includeDirectories;
	ln::List<ln::String> definitions;

    auto inputCodeBuffer = ln::FileSystem::readAllBytes(inputFilePath);
    char* inputCode = (char*)inputCodeBuffer.data();
    size_t inputCodeLength = inputCodeBuffer.size();

	auto unifiedShader = ln::makeRef<ln::detail::UnifiedShader>(m_diag);

    ln::detail::HLSLMetadataParser metadataParser;
    metadataParser.parse(inputCode, inputCodeLength, m_diag);
    if (m_diag->hasError()) {
        return false;
    }

    // glslang は hlsl の technique ブロックを理解できないので、空白で潰しておく
    for (auto& tech : metadataParser.techniques) {
        memset(inputCode + tech.blockBegin, ' ', tech.blockEnd - tech.blockBegin);
    }

    // VertexShader 名, inputLayout
   //std::unordered_map<std::string, std::vector<ln::detail::VertexInputAttribute>> vertexInputAttributeMap;
	std::unordered_map<std::string, std::shared_ptr<ln::detail::ShaderCodeTranspiler>> transpilerMap;

	// まずは Code を作る
    for (auto& tech : metadataParser.techniques)
	{
        for (auto& pass : tech.passes)
		{
            // Vertex shader
            {
				auto transpiler = std::make_shared<ln::detail::ShaderCodeTranspiler>(m_manager);
				transpiler->parseAndGenerateSpirv(ln::detail::ShaderCodeStage::Vertex, inputCode, inputCodeLength, pass.vertexShader, includeDirectories, &definitions, m_diag);
				if (m_diag->hasError()) {
					return false;
				}
				transpilerMap[pass.vertexShader] = transpiler;

				{
					ln::detail::UnifiedShaderTriple triple = { "spv", 110, "" };
					if (!unifiedShader->hasCode(pass.vertexShader, triple)) {
						unifiedShader->setCode(pass.vertexShader, triple, transpiler->spirvCode(), &transpiler->uniformBuffers());
					}
				}

				{
					ln::detail::UnifiedShaderTriple triple = { "glsl", 400, "" };
					if (!unifiedShader->hasCode(pass.vertexShader, triple)) {
						unifiedShader->setCode(pass.vertexShader, triple, transpiler->generateGlsl(400, false), nullptr);
					}
				}

				{
					ln::detail::UnifiedShaderTriple triple = { "glsl", 300, "es" };
					if (!unifiedShader->hasCode(pass.vertexShader, triple)) {
						unifiedShader->setCode(pass.vertexShader, triple, transpiler->generateGlsl(300, true), nullptr);
					}
				}
            }

            // Pixel shader
            {
				auto transpiler = std::make_shared<ln::detail::ShaderCodeTranspiler>(m_manager);
				transpiler->parseAndGenerateSpirv(ln::detail::ShaderCodeStage::Fragment, inputCode, inputCodeLength, pass.pixelShader, includeDirectories, &definitions, m_diag);
				if (m_diag->hasError()) {
					return false;
				}
				transpilerMap[pass.pixelShader] = transpiler;

				{
					ln::detail::UnifiedShaderTriple triple = { "spv", 110, "" };
					if (!unifiedShader->hasCode(pass.pixelShader, triple)) {
						unifiedShader->setCode(pass.pixelShader, triple, transpiler->spirvCode(), &transpiler->uniformBuffers());
					}
				}

				{
					ln::detail::UnifiedShaderTriple triple = { "glsl", 400, "" };
					if (!unifiedShader->hasCode(pass.pixelShader, triple)) {
						unifiedShader->setCode(pass.pixelShader, triple, transpiler->generateGlsl(400, false), nullptr);
					}
				}

				{
					ln::detail::UnifiedShaderTriple triple = { "glsl", 300, "es" };
					if (!unifiedShader->hasCode(pass.pixelShader, triple)) {
						unifiedShader->setCode(pass.pixelShader, triple, transpiler->generateGlsl(300, true), nullptr);
					}
				}
            }
        }
    }

	// Tech と Pass を作る
	for (auto& tech : metadataParser.techniques)
	{
		ln::detail::UnifiedShader::TechniqueId techId;
		if (!unifiedShader->addTechnique(tech.name, &techId)) {
			return false;
		}

		for (auto& pass : tech.passes)
		{
			ln::detail::UnifiedShader::CodeContainerId codeId;
			ln::detail::UnifiedShader::PassId passId;
			if (!unifiedShader->addPass(techId, pass.name, &passId)) {
				return false;
			}

			// VertexShader
			if (!unifiedShader->findCodeContainer(pass.vertexShader, &codeId)) {
				return false;
			}
			unifiedShader->setVertexShader(passId, codeId);

			// PixelShader
			if (!unifiedShader->findCodeContainer(pass.pixelShader, &codeId)) {
				return false;
			}
			unifiedShader->setPixelShader(passId, codeId);

			// ShaderRenderState
			unifiedShader->setRenderState(passId, pass.renderState);

            // InputLayout
            unifiedShader->setAttributeSemantics(passId, transpilerMap[pass.vertexShader]->attributes());

			// UniformBuffers
            auto refrection = ln::makeRef<ln::detail::UnifiedShaderRefrectionInfo>();
			ln::detail::ShaderUniformBufferInfo::mergeBuffers(
				transpilerMap[pass.vertexShader]->uniformBuffers(),
				transpilerMap[pass.pixelShader]->uniformBuffers(),
				&refrection->buffers);

			unifiedShader->setRefrection(passId, refrection);
		}
	}

	if (!unifiedShader->save(outputFilePath)) {
		return false;
	}

    CLI::info(u"");
	CLI::info(u"Compilation succeeded; see " + outputFilePath);

	return true;

#if 0
    //{
    //    ln::StringWriter buffer;
    //    ln::JsonWriter writer(&buffer);
    //    writer.w
    //}


    const int fileVersion = 1;

    {
        auto file = ln::FileStream::create(ln::Path(ln::Path(ln::Environment::executablePath()).parent(), u"out.lnfx"), ln::FileOpenMode::Write | ln::FileOpenMode::Truncate);
        auto writer = ln::makeRef<ln::BinaryWriter>(file);

        writer->write("lnfx", 4);   // magic number
        writer->writeInt32(fileVersion);      // file version

        // techniques
        {
            LN_CHECK(metadataParser.techniques.size() < 255);

            writer->writeUInt8(metadataParser.techniques.size());
            for (auto& tech : metadataParser.techniques) {
                tech.save(writer, fileVersion);
            }
        }

        // input HLSL
        {
            writer->write("src-hlsl", 8);
            writer->writeUInt32(inputCodeLength);
            writer->write(inputCode, inputCodeLength);
        }

        // GLSL
        {
            for (auto& info : m_vertexShaderCodeMap)
            {
                // chunk header
                writer->write("gl.vert ", 8);

                // name
                ln::detail::HLSLTechnique::writeString(writer, info.first);

                // code
                writer->writeUInt32(info.second.glslCode.length());
                writer->write(info.second.glslCode.data(), info.second.glslCode.length());
            }

            for (auto& info : m_pixelShaderCodeMap)
            {
                // chunk header
                writer->write("gl.frag ", 8);

                // name
                ln::detail::HLSLTechnique::writeString(writer, info.first);

                // code
                writer->writeUInt32(info.second.glslCode.length());
                writer->write(info.second.glslCode.data(), info.second.glslCode.length());
            }
        }
    }
#endif


}
