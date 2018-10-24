
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
#include "Internal.hpp"
#include <sstream>
#include <glslang/Public/ShaderLang.h>
#include <glslang/Include/ResourceLimits.h>
#include <SPIRV/GlslangToSpv.h>
#include <spirv_cross/spirv_glsl.hpp>
#include "../Grammar/CppLexer.hpp"
#include "ShaderAnalyzer.hpp"

namespace ln {
namespace detail {

// from glslang: StanAalone/ResourceLimits.cpp
const TBuiltInResource DefaultTBuiltInResource = {
	/* .MaxLights = */ 32,
	/* .MaxClipPlanes = */ 6,
	/* .MaxTextureUnits = */ 32,
	/* .MaxTextureCoords = */ 32,
	/* .MaxVertexAttribs = */ 64,
	/* .MaxVertexUniformComponents = */ 4096,
	/* .MaxVaryingFloats = */ 64,
	/* .MaxVertexTextureImageUnits = */ 32,
	/* .MaxCombinedTextureImageUnits = */ 80,
	/* .MaxTextureImageUnits = */ 32,
	/* .MaxFragmentUniformComponents = */ 4096,
	/* .MaxDrawBuffers = */ 32,
	/* .MaxVertexUniformVectors = */ 128,
	/* .MaxVaryingVectors = */ 8,
	/* .MaxFragmentUniformVectors = */ 16,
	/* .MaxVertexOutputVectors = */ 16,
	/* .MaxFragmentInputVectors = */ 15,
	/* .MinProgramTexelOffset = */ -8,
	/* .MaxProgramTexelOffset = */ 7,
	/* .MaxClipDistances = */ 8,
	/* .MaxComputeWorkGroupCountX = */ 65535,
	/* .MaxComputeWorkGroupCountY = */ 65535,
	/* .MaxComputeWorkGroupCountZ = */ 65535,
	/* .MaxComputeWorkGroupSizeX = */ 1024,
	/* .MaxComputeWorkGroupSizeY = */ 1024,
	/* .MaxComputeWorkGroupSizeZ = */ 64,
	/* .MaxComputeUniformComponents = */ 1024,
	/* .MaxComputeTextureImageUnits = */ 16,
	/* .MaxComputeImageUniforms = */ 8,
	/* .MaxComputeAtomicCounters = */ 8,
	/* .MaxComputeAtomicCounterBuffers = */ 1,
	/* .MaxVaryingComponents = */ 60,
	/* .MaxVertexOutputComponents = */ 64,
	/* .MaxGeometryInputComponents = */ 64,
	/* .MaxGeometryOutputComponents = */ 128,
	/* .MaxFragmentInputComponents = */ 128,
	/* .MaxImageUnits = */ 8,
	/* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
	/* .MaxCombinedShaderOutputResources = */ 8,
	/* .MaxImageSamples = */ 0,
	/* .MaxVertexImageUniforms = */ 0,
	/* .MaxTessControlImageUniforms = */ 0,
	/* .MaxTessEvaluationImageUniforms = */ 0,
	/* .MaxGeometryImageUniforms = */ 0,
	/* .MaxFragmentImageUniforms = */ 8,
	/* .MaxCombinedImageUniforms = */ 8,
	/* .MaxGeometryTextureImageUnits = */ 16,
	/* .MaxGeometryOutputVertices = */ 256,
	/* .MaxGeometryTotalOutputComponents = */ 1024,
	/* .MaxGeometryUniformComponents = */ 1024,
	/* .MaxGeometryVaryingComponents = */ 64,
	/* .MaxTessControlInputComponents = */ 128,
	/* .MaxTessControlOutputComponents = */ 128,
	/* .MaxTessControlTextureImageUnits = */ 16,
	/* .MaxTessControlUniformComponents = */ 1024,
	/* .MaxTessControlTotalOutputComponents = */ 4096,
	/* .MaxTessEvaluationInputComponents = */ 128,
	/* .MaxTessEvaluationOutputComponents = */ 128,
	/* .MaxTessEvaluationTextureImageUnits = */ 16,
	/* .MaxTessEvaluationUniformComponents = */ 1024,
	/* .MaxTessPatchComponents = */ 120,
	/* .MaxPatchVertices = */ 32,
	/* .MaxTessGenLevel = */ 64,
	/* .MaxViewports = */ 16,
	/* .MaxVertexAtomicCounters = */ 0,
	/* .MaxTessControlAtomicCounters = */ 0,
	/* .MaxTessEvaluationAtomicCounters = */ 0,
	/* .MaxGeometryAtomicCounters = */ 0,
	/* .MaxFragmentAtomicCounters = */ 8,
	/* .MaxCombinedAtomicCounters = */ 8,
	/* .MaxAtomicCounterBindings = */ 1,
	/* .MaxVertexAtomicCounterBuffers = */ 0,
	/* .MaxTessControlAtomicCounterBuffers = */ 0,
	/* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
	/* .MaxGeometryAtomicCounterBuffers = */ 0,
	/* .MaxFragmentAtomicCounterBuffers = */ 1,
	/* .MaxCombinedAtomicCounterBuffers = */ 1,
	/* .MaxAtomicCounterBufferSize = */ 16384,
	/* .MaxTransformFeedbackBuffers = */ 4,
	/* .MaxTransformFeedbackInterleavedComponents = */ 64,
	/* .MaxCullDistances = */ 8,
	/* .MaxCombinedClipAndCullDistances = */ 8,
	/* .MaxSamples = */ 4,
	/* .limits = */{
		/* .nonInductiveForLoops = */ 1,
		/* .whileLoops = */ 1,
		/* .doWhileLoops = */ 1,
		/* .generalUniformIndexing = */ 1,
		/* .generalAttributeMatrixVectorIndexing = */ 1,
		/* .generalVaryingIndexing = */ 1,
		/* .generalSamplerIndexing = */ 1,
		/* .generalVariableIndexing = */ 1,
		/* .generalConstantMatrixVectorIndexing = */ 1,
	}
};

//=============================================================================
// HLSLPass

void HLSLPass::save(BinaryWriter* w, int version)
{
    HLSLTechnique::writeString(w, name);
    HLSLTechnique::writeString(w, vertexShader);
    HLSLTechnique::writeString(w, pixelShader);
    HLSLTechnique::writeString(w, surfaceShader);
    HLSLTechnique::writeString(w, shadingModel);
    HLSLTechnique::writeString(w, ligitingModel);
}

void HLSLPass::load(BinaryReader* r, int version)
{
    name = HLSLTechnique::readString(r);
    vertexShader = HLSLTechnique::readString(r);
    pixelShader = HLSLTechnique::readString(r);
    surfaceShader = HLSLTechnique::readString(r);
    shadingModel = HLSLTechnique::readString(r);
    ligitingModel = HLSLTechnique::readString(r);
}

//=============================================================================
// HLSLTechnique

void HLSLTechnique::save(BinaryWriter* w, int version)
{
    LN_CHECK(passes.size() < 255);
    writeString(w, name);

    // passes
    w->writeUInt8(passes.size());
    for (auto& pass : passes) {
        pass.save(w, version);
    }
}

void HLSLTechnique::load(BinaryReader* r, int version)
{
    name = readString(r);

    // passes
    int count = r->readUInt8();
    for (int i = 0; i < count; i++) {
        HLSLPass pass;
        pass.load(r, version);
        passes.push_back(std::move(pass));
    }
}

void HLSLTechnique::writeString(BinaryWriter* w, const std::string& str)
{
    LN_CHECK(str.length() < 255);
    w->writeUInt8(str.length());
    w->write(str.data(), str.length());
}

std::string HLSLTechnique::readString(BinaryReader* r)
{
    char buf[255] = { 0 };
    uint8_t len = r->readUInt8();
    if (len == 0) {
        return std::string();
    }
    else {
        r->read(buf, len);
        return std::string(buf, len);
    }
}

//void save(JsonWriter* w, int version)
//{
//    w->writeStartObject();
//    w->writeString(name);
//    w->writeEndObject();
//}
//
//void load(JsonReader* r, int version)
//{
//}


//=============================================================================
// LocalIncluder

class LocalIncluder
	: public glslang::TShader::Includer
{
public:
	const List<Path>* includeDirs;

	virtual IncludeResult* includeSystem(const char* headerName, const char* sourceName, size_t inclusionDepth)
	{
		// TODO: とりあえず共通にしてみる
		return includeLocal(headerName, sourceName, inclusionDepth);
	}

	virtual IncludeResult* includeLocal(const char* headerName, const char* sourceName, size_t inclusionDepth)
	{
		for (auto& dir : (*includeDirs))
		{
			auto path = Path(dir, String::fromCString(headerName));
			if (FileSystem::existsFile(path))
			{
				ByteBuffer* buf = new ByteBuffer(FileSystem::readAllBytes(path));
				return new IncludeResult(path.str().toStdString(), (const char*)buf->data(), buf->size(), buf);
			}
		}

		return nullptr;
	}

	virtual void releaseInclude(IncludeResult* result)
	{
		if (result)
		{
			delete reinterpret_cast<ByteBuffer*>(result->userData);
			delete result;
		}
	}
};

//=============================================================================
// ShaderCodeTranspiler

void ShaderCodeTranspiler::initializeGlobals()
{
	glslang::InitializeProcess();
}

void ShaderCodeTranspiler::finalizeGlobals()
{
	glslang::FinalizeProcess();
}

ShaderCodeTranspiler::ShaderCodeTranspiler()
	: m_stage(ShaderCodeStage::Vertex)
{
}

bool ShaderCodeTranspiler::parseAndGenerateSpirv(
	ShaderCodeStage stage, const char* code, size_t length, const std::string& entryPoint,
	const List<Path>& includeDir, DiagnosticsManager* diag)
{
	m_stage = stage;

	LocalIncluder includer;
	includer.includeDirs = &includeDir;

	// -d オプション
	//const int defaultVersion = Options & EOptionDefaultDesktop ? 110 : 100;
	const int defaultVersion = 100;

	glslang::EShSource sourceType = glslang::EShSourceHlsl;
	const int ClientInputSemanticsVersion = 410;//320;
	glslang::EshTargetClientVersion OpenGLClientVersion = glslang::EShTargetOpenGL_450;
	bool forwardCompatible = true;
	EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

	EShLanguage lang;
	switch (stage)
	{
	case ShaderCodeStage::Vertex:
		lang = EShLanguage::EShLangVertex;
		break;
	case ShaderCodeStage::Fragment:
		lang = EShLanguage::EShLangFragment;
		break;
	default:
		LN_NOTIMPLEMENTED();
		break;
	}

	// この２つは開放順が重要。TProgram の ヘッダや Standalone.cpp CompileAndLinkShaderUnits の一番下に書いてある。
	glslang::TShader shader(lang);
	glslang::TProgram program;

	// parse
	{
		const char*shaderCode[1] = { code };
		const int shaderLenght[1] = { static_cast<int>(length) };
		const char* shaderName[1] = { "shadercode" };
		shader.setStringsWithLengthsAndNames(shaderCode, shaderLenght, shaderName, 1);
		shader.setEntryPoint(entryPoint.c_str());

		shader.setEnvInput(sourceType, lang, glslang::EShClientOpenGL, ClientInputSemanticsVersion);
		shader.setEnvClient(glslang::EShClientOpenGL, OpenGLClientVersion);
		/* Vulkan Rule
		shader->setEnvInput((Options & EOptionReadHlsl) ? glslang::EShSourceHlsl : glslang::EShSourceGlsl,
		compUnit.stage, glslang::EShClientVulkan, ClientInputSemanticsVersion);
		shader->setEnvClient(glslang::EShClientVulkan, VulkanClientVersion);
		*/

		/* TODO: parse でメモリリークしてるぽい。EShLangFragment の時に発生する。
			Dumping objects ->
			{12053} normal block at 0x06EB1410, 8 bytes long.
			 Data: <k       > 6B 0F 00 00 FF FF FF FF 
		*/
		if (!shader.parse(&DefaultTBuiltInResource, defaultVersion, forwardCompatible, messages, includer)) {
			if (!StringHelper::isNullOrEmpty(shader.getInfoLog())) diag->reportError(shader.getInfoLog());
			if (!StringHelper::isNullOrEmpty(shader.getInfoDebugLog())) diag->reportError(shader.getInfoDebugLog());
			return false;
		}
		else if (shader.getInfoLog()) {
			if (!StringHelper::isNullOrEmpty(shader.getInfoLog())) diag->reportWarning(shader.getInfoLog());
			if (!StringHelper::isNullOrEmpty(shader.getInfoDebugLog())) diag->reportWarning(shader.getInfoDebugLog());
		}
	}

	// link
	{
		program.addShader(&shader);

		if (!program.link(messages)) {
			if (!StringHelper::isNullOrEmpty(shader.getInfoLog())) diag->reportError(shader.getInfoLog());
			if (!StringHelper::isNullOrEmpty(shader.getInfoDebugLog())) diag->reportError(shader.getInfoDebugLog());
			return false;
		}
		else if (shader.getInfoLog()) {
			if (!StringHelper::isNullOrEmpty(shader.getInfoLog())) diag->reportWarning(shader.getInfoLog());
			if (!StringHelper::isNullOrEmpty(shader.getInfoDebugLog())) diag->reportWarning(shader.getInfoDebugLog());
		}

	}


	program.buildReflection();
	program.dumpReflection();
#if 0

	for (int i = 0; i < program.getNumLiveAttributes(); i++)
	{
		auto* type = program.getAttributeTType(i);
		printf("");
	}
	// 変数名はどこで作られる？
#endif

	glslang::GlslangToSpv(*program.getIntermediate(lang), m_spirvCode);

	return true;
}

std::string ShaderCodeTranspiler::generateGlsl()
{
	spirv_cross::CompilerGLSL glsl(m_spirvCode);

#if 1
	spirv_cross::ShaderResources resources = glsl.get_shader_resources();

	// Get all sampled images in the shader.
	for (auto &resource : resources.sampled_images)
	{
		unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
		unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
		printf("Image %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);

		// Modify the decoration to prepare it for GLSL.
		glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);
		
		

		// Some arbitrary remapping if we want.
		glsl.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);
	}


#endif

	// Set some options.
	spirv_cross::CompilerGLSL::Options options;
	options.version = 410;
	options.es = false;
	//options.version = 300;
	//options.es = true;
	glsl.set_common_options(options);



	// From main.cpp
	// Builds a mapping for all combinations of images and samplers.
	glsl.build_combined_image_samplers();

	// Give the remapped combined samplers new names.
	// Here you can also set up decorations if you want (binding = #N).
	for (auto &remap : glsl.get_combined_image_samplers())
	{
		glsl.set_name(remap.combined_id, "lnCIS_lnT_" + glsl.get_name(remap.image_id) + "_lnS_" + glsl.get_name(remap.sampler_id));
		//glsl.set_name(remap.combined_id, "test" /*join("SPIRV_Cross_Combined", glsl.get_name(remap.image_id), glsl.get_name(remap.sampler_id))*/);
	}





	// uniform textureXX
	for (auto &resource : resources.separate_images)
	{
		unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
		unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
		printf("Image %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);
	}

	// uniform textureXX
	for (auto &resource : resources.separate_samplers)
	{
		unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
		unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
		printf("Image %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);
		
	}


	{
		// TODO: 名前一致対応したい
		/*
			今のままだと、頂点シェーダの出力とピクセルシェーダの入力構造体のレイアウトが一致していなければ、
			glUseProgram が INVALID を返すみたいな原因のわかりにくい問題となる。
		
			glslang がセマンティクスまで理解していればいいが、そうでなければ構造体メンバの名前一致で
			指定できるようにしたい。

		*/


		if (m_stage == ShaderCodeStage::Vertex)
		{
			for (size_t i = 0; i < resources.stage_outputs.size(); i++)
			{
				// _entryPointOutput_Color

				//auto s = "ln_varying_" + resources.stage_outputs[i].name.substr(18);

				//

				std::stringstream s;
				s << "ln_varying_" << i;
				glsl.set_name(resources.stage_outputs[i].id, s.str());
				glsl.set_name(resources.stage_outputs[i].id, s.str());
			}



			//for (auto &resource : resources.stage_inputs)
			//{
			//	auto s = glsl.get_decoration_string(resource.id, spv::DecorationHlslSemanticGOOGLE);
			//	std::cout << s << std::endl;
			//}

			//
			
		}
		else if (m_stage == ShaderCodeStage::Fragment)
		{
			for (size_t i = 0; i < resources.stage_inputs.size(); i++)
			{
				//auto s = "ln_varying_" + resources.stage_inputs[i].name.substr(6);
				std::stringstream s;
				s << "ln_varying_" << i;
				glsl.set_name(resources.stage_inputs[i].id, s.str());
			}

			for (size_t i = 0; i < resources.stage_outputs.size(); i++)
			{
				//glsl.set_decoration(resources.stage_outputs[i].id, spv::DecorationLocation, 6);
				//glsl.unset_decoration(resources.stage_outputs[i].id, spv::DecorationLocation);
			}
		}

		////auto resources = compiler.get_shader_resources();
		//// For fragment shaders
		//for (auto &v : resources.stage_inputs)
		//	if (glsl.get_decoration(v.id, spv::DecorationLocation) == 0)
		//	{
		//		//glsl.set_name(v.id, "FFFF");
		//		//printf("%s\n", v.);
		//		//auto s = glsl.get_name(v.id);
		//		//glsl.set_name(v.base_type_id, "VertexFragmentLinkage");

		//		auto& a = glsl.get_type(v.type_id);
		//		std::cout  << std::endl;
		//		

		//	}
		//// For vertex shaders
		//for (auto &v : resources.stage_outputs)
		//	if (glsl.get_decoration(v.id, spv::DecorationLocation) == 0)
		//	{
		//		//auto s = glsl.get_name(v.id);
		//		//glsl.set_name(v.base_type_id, "VertexFragmentLinkage");

		//	}
	}

	// デフォルトでは binding の値が格納されており、 GLSL に出力すると１
	//   layout(binding = 0, std140) uniform ConstBuff
	// というようなコードが出力される。
	// しかし、macOS (Mojave) では binding には対応しておらず、
	//   unknown identifer 'binding' in layout
	// というエラーになってしまう。
	// そこで、binding の decoration は削除してしまう。
	// こうしておくと、glsl.compile() で binding 指定を含まない GLSL を生成することができる。
	for (size_t i = 0; i < resources.uniform_buffers.size(); i++)
	{
		// Modify the decoration to prepare it for GLSL.
		glsl.unset_decoration(resources.uniform_buffers[i].id, spv::DecorationBinding);
		
		
		
		// Some arbitrary remapping if we want.
		//glsl.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);
	}

	return glsl.compile();
}


//=============================================================================
// HLSLMetadataParser

bool HLSLMetadataParser::parse(const char* code, size_t length, DiagnosticsManager* diag)
{
	m_code = code;
	m_codeLength = length;

	TextDiagnostics textdiag(diag);
	CppLexer lexer;
	lexer.setDiag(&textdiag);
	m_tokens = lexer.makeTokenList(code, length);

	m_current = 0;
	m_isLuminoShader = false;

	parseCompileUnit();

	return true;
}

//Token& HLSLMetadataParser::current()
//{
//	return m_tokens->at(m_current);
//}

const Token& HLSLMetadataParser::current() const
{
	return m_tokens->at(m_current);
}

bool HLSLMetadataParser::next()
{
	do
	{
		m_current++;

	} while (!isEof() && isSpaceToken(current()));

	return !isEof();
}

bool HLSLMetadataParser::nextTo(const char* word, int len)
{
	do
	{
		m_current++;

	} while (
		isSpaceToken(current()) ||
		!equalString(current(), word, len));

	return !isEof();
}

bool HLSLMetadataParser::isSpaceToken(const Token& token) const
{
	return
		token.group() == TokenGroup::SpaceSequence ||
		token.group() == TokenGroup::NewLine ||
		token.group() == TokenGroup::Comment;
}

bool HLSLMetadataParser::isEof() const
{
	return m_current >= m_tokens->size();
	//return current().group() == TokenGroup::Eof;
}

bool HLSLMetadataParser::equalString(const Token& token, const char* str, size_t len)
{
	if (token.length() != len) return false;
	const char* ts = CppLexer::getTokenString(token, m_code, m_codeLength);
	return strncmp(ts, str, len) == 0;
}

std::string HLSLMetadataParser::getString(const Token& token)
{
	const char* ts = CppLexer::getTokenString(token, m_code, m_codeLength);
	return std::string(ts, token.length());
}

bool HLSLMetadataParser::parseCompileUnit()
{
	do
	{
		if (current().group() != TokenGroup::Unknown)
		{
			if (equalString(current(), "technique", 9))
			{
				HLSLTechnique tech;
				if (!parseTechnique(&tech)) return false;
				techniques.push_back(std::move(tech));
			}
		}

	} while (next());

	return true;
}

bool HLSLMetadataParser::parseTechnique(HLSLTechnique* tech)
{
	int begin = m_current;

	// 名前
	if (!next()) return false;
	if (current().group() == TokenGroup::Identifier)
	{
		tech->name = getString(current());
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return false;
	}

	if (!nextTo('{')) return false;

	bool closed = false;
	while (next())
	{
		if (equalString(current(), "pass", 4))
		{
			HLSLPass pass;
			if (!parsePass(&pass)) return false;
			tech->passes.push_back(std::move(pass));
		}
		else if (equalString(current(), "}", 1))
		{
			closed = true;
			break;
		}
	}
	if (!closed) return false;

	tech->blockBegin = m_tokens->at(begin).location();
	tech->blockEnd = current().location() + 1;

	return true;
}

bool HLSLMetadataParser::parsePass(HLSLPass* pass)
{
	// 名前
	if (!next()) return false;
	if (current().group() == TokenGroup::Identifier)
	{
		pass->name = getString(current());
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return false;
	}

	if (!nextTo('{')) return false;

	bool closed = false;
	while (next())
	{
		if (current().group() == TokenGroup::Identifier)
		{
			if (!parseRenderState(pass)) return false;
		}
		else if (equalString(current(), "}", 1))
		{
			closed = true;
			break;
		}
	}
	if (!closed) return false;

	return true;
}

bool HLSLMetadataParser::parseRenderState(HLSLPass* pass)
{
	// TODO:  VertexShader　 = compile vs_3_0 VS_WriteLinearDepth(); とか間違えて書いてしまうことが多いので、警告したい。

	const Token& name = current();
	if (!nextTo('=')) return false;
	if (!next()) return false;

	if (equalString(name, "VertexShader", 12))
	{
		//next();	// skip "compile"
		//next();	// skip "vs_x_x"
		pass->vertexShader = getString(current());
	}
	else if (equalString(name, "PixelShader", 11))
	{
		//next();	// skip "compile"
		//next();	// skip "ps_x_x"
		pass->pixelShader = getString(current());
	}
	else if (equalString(name, "ShadingModel", 12))
	{
		pass->shadingModel = getString(current());
		m_isLuminoShader = true;
	}
	else if (equalString(name, "LigitingModel", 13))
	{
		pass->ligitingModel = getString(current());
		m_isLuminoShader = true;
	}
	else if (equalString(name, "SurfaceShader", 13))
	{
		pass->surfaceShader = getString(current());
		m_isLuminoShader = true;
	}

	if (!nextTo(';')) return false;

	return true;
}

} // namespace detail
} // namespace ln

#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
