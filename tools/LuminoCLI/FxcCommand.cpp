
#include "../../src/LuminoEngine/src/Shader/ShaderManager.hpp"
#include "../../src/LuminoEngine/src/Shader/ShaderTranspiler.hpp"
#include "../../src/LuminoEngine/src/Shader/HLSLMetadataParser.hpp"
#include "../../src/LuminoEngine/src/Shader/UnifiedShader.hpp"
#include "../../src/LuminoEngine/src/Shader/UnifiedShaderCompiler.hpp"
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

	m_diag = ln::makeObject<ln::DiagnosticsManager>();

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


	ln::detail::UnifiedShaderCompiler compiler(m_manager, m_diag);
	if (!compiler.compile(inputCode, inputCodeLength, includeDirectories, definitions)) {
		return false;
	}
	if (!compiler.link()) {
		return false;
	}


	if (!compiler.unifiedShader()->save(outputFilePath)) {
		return false;
	}

	// dump intermediate codes.
    if (saveCodes) {
		compiler.unifiedShader()->saveCodes(outputFilePath);
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
