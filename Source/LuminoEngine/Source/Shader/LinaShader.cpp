
#include "../Internal.h"
#include <hlsl2glsl.h>
#include "LinaShader.h"

#include <Fluorite/AnalyzerContext.h>
#include <Fluorite/Diagnostics.h>
#include "../Source/Fluorite/Lexer/CppLexer.h"

LN_NAMESPACE_BEGIN

struct HLSLPass
{
	std::string vertexShader;
	std::string pixelShader;
};

struct HLSLTechnique
{
	std::vector<HLSLPass>	passes;
};

class HLSLMetadataParser
{
public:
	std::vector<HLSLTechnique>	techniques;

	bool parse(const fl::TokenList* tokens);

private:
	fl::Token* current() { return m_tokens->getAt(m_current); }
	bool next();
	bool nextTo(char ch) { return nextTo(&ch, 1); }
	bool nextTo(const char* word, int len);
	bool isSpaceToken(fl::Token* token) const;
	bool isEof() const;

	bool parseCompileUnit();
	bool parseTechnique(HLSLTechnique* tech);
	bool parsePass(HLSLPass* pass);
	bool parseRenderState(HLSLPass* pass);

	const fl::TokenList* m_tokens;
	int m_current;

};

bool HLSLMetadataParser::parse(const fl::TokenList* tokens)
{
	m_current = 0;
	m_tokens = tokens;

	parseCompileUnit();

	return true;
}

bool HLSLMetadataParser::next()
{
	do
	{
		m_current++;

	} while(isSpaceToken(m_tokens->getAt(m_current)));

	return !isEof();
}

bool HLSLMetadataParser::nextTo(const char* word, int len)
{
	do
	{
		m_current++;

	} while (
		isSpaceToken(m_tokens->getAt(m_current)) ||
		!m_tokens->getAt(m_current)->EqualString(word, len));

	return !isEof();
}

bool HLSLMetadataParser::isSpaceToken(fl::Token* token) const
{
	return
		token->GetTokenGroup() == fl::TokenGroup::SpaceSequence ||
		token->GetTokenGroup() == fl::TokenGroup::NewLine ||
		token->GetTokenGroup() == fl::TokenGroup::Comment;
}

bool HLSLMetadataParser::isEof() const
{
	return m_tokens->getAt(m_current)->GetTokenGroup() == fl::TokenGroup::Eof;
}

bool HLSLMetadataParser::parseCompileUnit()
{
	while (next())
	{
		if (current()->EqualString("technique", 9))
		{
			HLSLTechnique tech;
			if (!parseTechnique(&tech)) return false;
			techniques.push_back(std::move(tech));
		}
	}

	return true;
}

bool HLSLMetadataParser::parseTechnique(HLSLTechnique* tech)
{
	int begin = m_current;
	if (!nextTo('{')) return false;

	bool closed = false;
	while (next())
	{
		if (current()->EqualString("pass", 4))
		{
			HLSLPass pass;
			if (!parsePass(&pass)) return false;
			tech->passes.push_back(std::move(pass));
		}
		else if (current()->EqualChar('}'))
		{
			closed = true;
			break;
		}
	}
	if (!closed) return false;

	// hlsl2glsl は technique ブロックを理解できないのですべて無効化しておく
	for (int i = begin; i <= m_current; i++)
	{
		m_tokens->getAt(i)->setValid(false);
	}

	return true;
}

bool HLSLMetadataParser::parsePass(HLSLPass* pass)
{
	if (!nextTo('{')) return false;

	bool closed = false;
	while (next())
	{
		if (current()->GetTokenGroup() == fl::TokenGroup::Identifier)
		{
			if (!parseRenderState(pass)) return false;
		}
		else if (current()->EqualChar('}'))
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
	fl::Token* name = current();
	if (!nextTo('=')) return false;
	if (!next()) return false;
	fl::Token* value = current();
	
	if (name->EqualString("VertexShader", 12))
	{
		next();	// skip "compile"
		next();	// skip "vs_x_x"
		pass->vertexShader = current()->getString();
	}
	else if (name->EqualString("PixelShader", 11))
	{
		next();	// skip "compile"
		next();	// skip "ps_x_x"
		pass->pixelShader = current()->getString();
	}

	if (!nextTo(';')) return false;

	return true;
}

LinaShaderIRGenerater::LinaShaderIRGenerater()
{

}

void LinaShaderIRGenerater::initialize()
{
	int r = Hlsl2Glsl_Initialize();
	LN_ENSURE(r != 0, "failed Hlsl2Glsl_Initialize.");
}

void LinaShaderIRGenerater::finalize()
{
	Hlsl2Glsl_Shutdown();
}

static bool Hlsl2Glsl(const std::string& input, const std::string& entryPoint, EShLanguage codeType, std::string* output, std::string* log)
{
	ShHandle shParser = nullptr;

	try
	{
		static EAttribSemantic kAttribSemantic[] =
		{
			EAttrSemPosition,
			EAttrSemPosition1,
			EAttrSemPosition2,
			EAttrSemPosition3,
			EAttrSemVPos,
			EAttrSemVFace,
			EAttrSemNormal,
			EAttrSemNormal1,
			EAttrSemNormal2,
			EAttrSemNormal3,
			EAttrSemColor0,
			EAttrSemColor1,
			EAttrSemColor2,
			EAttrSemColor3,
			EAttrSemTex0,
			EAttrSemTex1,
			EAttrSemTex2,
			EAttrSemTex3,
			EAttrSemTex4,
			EAttrSemTex5,
			EAttrSemTex6,
			EAttrSemTex7,
			EAttrSemTex8,
			EAttrSemTex9,
			EAttrSemTangent,
			EAttrSemTangent1,
			EAttrSemTangent2,
			EAttrSemTangent3,
			EAttrSemBinormal,
			EAttrSemBinormal1,
			EAttrSemBinormal2,
			EAttrSemBinormal3,
			EAttrSemBlendWeight,
			EAttrSemBlendWeight1,
			EAttrSemBlendWeight2,
			EAttrSemBlendWeight3,
			EAttrSemBlendIndices,
			EAttrSemBlendIndices1,
			EAttrSemBlendIndices2,
			EAttrSemBlendIndices3,
			EAttrSemPSize,
			EAttrSemPSize1,
			EAttrSemPSize2,
			EAttrSemPSize3,
			EAttrSemDepth,
			//EAttrSemUnknown,
			//EAttrSemVertexID,
			//EAttrSemInstanceID,
			//EAttrSemPrimitiveID,
			//EAttrSemCoverage,
		};
		static const char* kAttribString[] =
		{	// TODO: このへん、使える attr かどうかチェックしたい
			"ln_Vertex", //EAttrSemPosition,
			"ln_Vertex1", //EAttrSemPosition1,
			"ln_Vertex2", //EAttrSemPosition2,
			"ln_Vertex3", //EAttrSemPosition3,
			"ln_VPos",	//EAttrSemVPos,
			"ln_VFace", //EAttrSemVFace,
			"ln_Normal",	//EAttrSemNormal,
			"ln_Normal1",	//EAttrSemNormal1,
			"ln_Normal2",	//EAttrSemNormal2,
			"ln_Normal3",	//EAttrSemNormal3,
			"ln_Color0",	//EAttrSemColor0,
			"ln_Color1",	//EAttrSemColor1,
			"ln_Color2",	//EAttrSemColor2,
			"ln_Color3",	//EAttrSemColor3,
			"ln_MultiTexCoord0",	//EAttrSemTex0,
			"ln_MultiTexCoord1",	//EAttrSemTex1,
			"ln_MultiTexCoord2",	//EAttrSemTex2,
			"ln_MultiTexCoord3",	//EAttrSemTex3,
			"ln_MultiTexCoord4",	//EAttrSemTex4,
			"ln_MultiTexCoord5",	//EAttrSemTex5,
			"ln_MultiTexCoord6",	//EAttrSemTex6,
			"ln_MultiTexCoord7",	//EAttrSemTex7,
			"ln_MultiTexCoord8",	//EAttrSemTex8,
			"ln_MultiTexCoord9",	//EAttrSemTex9,
			"ln_Tangent",	//EAttrSemTangent,
			"ln_Tangent1",	//EAttrSemTangent1,
			"ln_Tangent2",	//EAttrSemTangent2,
			"ln_Tangent3",	//EAttrSemTangent3,
			"ln_Binormal",	//EAttrSemBinormal,
			"ln_Binormal1",	//EAttrSemBinormal1,
			"ln_Binormal2",	//EAttrSemBinormal2,
			"ln_Binormal3",	//EAttrSemBinormal3,
			"ln_BlendWeight",	//EAttrSemBlendWeight,
			"ln_BlendWeight1",	//EAttrSemBlendWeight1,
			"ln_BlendWeight2",	//EAttrSemBlendWeight2,
			"ln_BlendWeight3",	//EAttrSemBlendWeight3,
			"ln_BlendIndices",	//EAttrSemBlendIndices,
			"ln_BlendIndices1",	//EAttrSemBlendIndices1,
			"ln_BlendIndices2",	//EAttrSemBlendIndices2,
			"ln_BlendIndices3",	//EAttrSemBlendIndices3,
			"ln_PSize",	//EAttrSemPSize,
			"ln_PSize1",	//EAttrSemPSize1,
			"ln_PSize2",	//EAttrSemPSize2,
			"ln_PSize3",	//EAttrSemPSize3,
			"ln_Depth",	//EAttrSemDepth,
						//"ln_Unknown",	//EAttrSemUnknown,
						//"ln_VertexId",	//EAttrSemVertexID,
						//"ln_InstanceId",	//EAttrSemInstanceID,
						//"ln_PrimitiveId",	//EAttrSemPrimitiveID,
						//"ln_Coverage",	//EAttrSemCoverage,
		};


		shParser = Hlsl2Glsl_ConstructCompiler(codeType);
		if (shParser == 0)
		{
			(*log) += "failed ConstructCompiler.";
			return false;
		}

		int setUserAttrOk = Hlsl2Glsl_SetUserAttributeNames(shParser, kAttribSemantic, kAttribString, LN_ARRAY_SIZE_OF(kAttribSemantic));
		if (setUserAttrOk == 0)
		{
			(*log) += "failed SetUserAttributeNames.";
			return false;
		}

		unsigned options = 0;
		//if (false)
		//	options |= ETranslateOpIntermediate;
		ETargetVersion version = ETargetGLSL_120;//ETargetGLSL_ES_300;//
		int parseOk = Hlsl2Glsl_Parse(shParser, input.c_str(), version, nullptr, options);
		(*log) += Hlsl2Glsl_GetInfoLog(shParser);
		if (parseOk == 0)
		{
			(*log) += "failed Parse.";
			return false;
		}

		int translateOk = Hlsl2Glsl_Translate(shParser, entryPoint.c_str(), version, options);
		if (translateOk == 0)
		{
			(*log) += "failed Translate.";
			return false;
		}

		*output = Hlsl2Glsl_GetShader(shParser);

		Hlsl2Glsl_DestructCompiler(shParser);
	}
	catch (...)
	{
		if (shParser != 0)
		{
			Hlsl2Glsl_DestructCompiler(shParser);
		}
		throw;
	}

	return true;
}

void LinaShaderIRGenerater::loadRawHLSL(const std::string& code)
{
	//fl::InputFile f(std::string{}, code.c_str(), code.length());
	//fl::CppLexer lex;
	////fl::TokenList tokenList;
	////fl::DiagnosticsItemSet diag();
	//lex.Tokenize(&f);

	fl::AnalyzerContext ctx;
	auto file = ctx.RegisterInputMemoryCode(std::string{}, code.c_str(), code.length());
	ctx.LexFile(file);
	auto tokens = file->GetTokenList();

	HLSLMetadataParser parser;
	parser.parse(tokens);

	auto minHLSL = tokens->toStringValidCode();
	FileSystem::writeAllBytes(_T("test.fx"), minHLSL.c_str(), minHLSL.length());

	std::string glslCode, log;
	Hlsl2Glsl(minHLSL, parser.techniques[0].passes[0].vertexShader, EShLangVertex, &glslCode, &log);
	FileSystem::writeAllBytes(_T("test.vert"), glslCode.c_str(), glslCode.length());
	Hlsl2Glsl(minHLSL, parser.techniques[0].passes[0].pixelShader, EShLangFragment, &glslCode, &log);
	FileSystem::writeAllBytes(_T("test.frag"), glslCode.c_str(), glslCode.length());

	printf("");
}

LinaShader::LinaShader()
{

}

LN_NAMESPACE_END
