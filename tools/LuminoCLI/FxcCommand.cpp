
#include "../../src/LuminoEngine/src/Shader/ShaderManager.hpp"
#include "../../src/LuminoEngine/src/Shader/ShaderAnalyzer.hpp"
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

void FxcCommand::execute(const ln::Path& inputFile)
{
    ln::detail::ShaderManager::Settings settings;
    auto shaderManager = ln::makeRef<ln::detail::ShaderManager>();
    shaderManager->initialize(settings);

    ln::List<ln::Path> includeDirectories;

    auto inputCodeBuffer = ln::FileSystem::readAllBytes(inputFile);
    char* inputCode = (char*)inputCodeBuffer.data();
    size_t inputCodeLength = inputCodeBuffer.size();

    auto diag = ln::newObject<ln::DiagnosticsManager>();

    ln::detail::HLSLMetadataParser metadataParser;
    metadataParser.parse(inputCode, inputCodeLength, diag);
    if (diag->hasError()) {
        return;
    }

    // glslang は hlsl の technique ブロックを理解できないので、空白で潰しておく
    for (auto& tech : metadataParser.techniques) {
        memset(inputCode + tech.blockBegin, ' ', tech.blockEnd - tech.blockBegin);
    }


    for (auto& tech : metadataParser.techniques) {
        for (auto& pass : tech.passes) {

            // Vertex shader
            {
                auto itr = m_vertexShaderCodeMap.find(pass.vertexShader);
                if (itr == m_vertexShaderCodeMap.end())
                {
                    ln::detail::ShaderCodeTranspiler transpiler;
                    transpiler.parseAndGenerateSpirv(ln::detail::ShaderCodeStage::Vertex, inputCode, inputCodeLength, pass.vertexShader, includeDirectories, diag);
					if (diag->hasError()) {
						return;
					}

                    ShaderCode code;
                    code.glslCode = transpiler.generateGlsl();
                    m_vertexShaderCodeMap.insert({ pass.vertexShader, std::move(code) });
                }
            }

            // Pixel shader
            {
                auto itr = m_pixelShaderCodeMap.find(pass.pixelShader);
                if (itr == m_pixelShaderCodeMap.end())
                {
                    ln::detail::ShaderCodeTranspiler transpiler;
                    transpiler.parseAndGenerateSpirv(ln::detail::ShaderCodeStage::Fragment, inputCode, inputCodeLength, pass.pixelShader, includeDirectories, diag);
					if (diag->hasError()) {
						return;
					}

                    ShaderCode code;
                    code.glslCode = transpiler.generateGlsl();
                    m_pixelShaderCodeMap.insert({ pass.pixelShader, std::move(code) });
                }
            }
        }
    }



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



    shaderManager->dispose();
}
