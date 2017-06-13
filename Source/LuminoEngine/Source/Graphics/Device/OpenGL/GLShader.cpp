
/* GLSL : よくあるミス
 * http://kumar.hateblo.jp/entry/2013/08/28/200708
 */

/*
	HLSL pass render state
	https://msdn.microsoft.com/en-us/library/windows/desktop/bb173347(v=vs.85).aspx

*/
#include "../../Internal.h"
#include <Lumino/Graphics/GraphicsException.h>
#include "GLGraphicsDevice.h"
#include "GLShader.h"

#if defined(LN_DO_CHECK_THROW)
#define	LN_FAIL_CHECK_GLERROR()		for (GLenum lnglerr = glGetError(); lnglerr != GL_NO_ERROR && ln::detail::notifyException<OpenGLException>(__FILE__, __LINE__, lnglerr); lnglerr = GL_NO_ERROR)
#else
#error no implemented
#define LN_FAIL_CHECK_GLERROR()		for (GLenum lnglerr = glGetError(); lnglerr != GL_NO_ERROR; lnglerr = GL_NO_ERROR)//{ GLenum lnglerr = glGetError(); LN_THROW(lnglerr == GL_NO_ERROR , OpenGLException, lnglerr); }
#endif

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//// https://msdn.microsoft.com/ja-jp/library/bb173347%28v=vs.85%29.aspx#Sampler_States
//enum class SamplerStateId
//{
//	AddressU,
//	AddressV,
//	AddressW,
//	BorderColor,
//	MagFilter,
//	MaxAnisotropy,
//	MaxMipLevel,
//	MinFilter,
//	MipFilter,
//	MipMapLodBias,
//	SRGBTexture,
//};
//
//struct SamplerStatePair
//{
//	SamplerStateId	stateId;
//	uint32_t		value;
//
//	static bool Make(const String& name, const String& value, SamplerStatePair* out)
//	{
//		struct Item
//		{
//			const TCHAR*	name;
//			const TCHAR*	value;
//			SamplerStateId	stateId;
//			uint32_t		stateValue;
//		};
//		Item table[] =
//		{
//			{ _T("AddressU"), _T("WRAP"), SamplerStateId::AddressU, TextureWrapMode_Repeat },
//			{ _T("AddressU"), _T("CLAMP"), SamplerStateId::AddressU, TextureWrapMode_Clamp },
//			{ _T("AddressV"), _T("WRAP"), SamplerStateId::AddressU, TextureWrapMode_Repeat },
//			{ _T("AddressV"), _T("CLAMP"), SamplerStateId::AddressU, TextureWrapMode_Clamp },
//			//{ _T("AddressW"),,, },
//			//{ _T("BorderColor"),,, },
//			//{ _T("MagFilter"),,, },
//			//{ _T("MaxAnisotropy"),,, },
//			//{ _T("MaxMipLevel"),,, },
//			//{ _T("MinFilter"),,, },
//			//{ _T("MipFilter"),,, },
//			//{ _T("MipMapLodBias"),,, },
//			//{ _T("SRGBTexture"),,, },
//		};
//		for (int i = 0; i < LN_ARRAY_SIZE_OF(table); ++i)
//		{
//			if (name.Compare(table[i].name, -1, CaseSensitivity::CaseInsensitive) == 0 &&
//				value.Compare(table[i].value, -1, CaseSensitivity::CaseInsensitive) == 0)
//			{
//				out->stateId = table[i].stateId;
//				out->value = table[i].stateValue;
//				return true;
//			}
//		}
//		return false;	// 未対応のステート
//	}
//};

enum class RenderStateId
{
	AlphaBlendEnable,
	AlphaFunc,
	AlphaRef,
	AlphaTestEnable,
	BlendOp,
	ColorWriteEnable,
	DepthBias,
	DestBlend,
	DitherEnable,
	FillMode,
	LastPixel,
	ShadeMode,
	SlopeScaleDepthBias,
	SrcBlend,
	StencilEnable,
	StencilFail,
	StencilFunc,
	StencilMask,
	StencilPass,
	StencilRef,
	StencilWriteMask,
	StencilZFail,
	TextureFactor,
	//Wrap0 ～ Wrap15
	ZEnable,
	ZFunc,
	ZWriteEnable,
};


//struct RenderStatePair
//{
//	RenderStateId	stateId;
//	uint32_t		value;
//
//	static bool Make(const String& name, const String& value, SamplerStatePair* out)
//	{
//		struct Item
//		{
//			const TCHAR*	name;
//			const TCHAR*	value;
//			RenderStateId	stateId;
//			uint32_t		stateValue;
//		};
//		Item table[] =
//		{
//
//			//{ _T("AlphaBlendEnable"),,, },
//			//{ _T("AlphaFunc"),,, },
//			//{ _T("AlphaRef"),,, },
//			//{ _T("AlphaTestEnable"),,, },
//			//{ _T("BlendOp"),,, },
//			//{ _T("ColorWriteEnable"),,, },
//			//{ _T("DepthBias"),,, },
//			//{ _T("DestBlend"),,, },
//			//{ _T("DitherEnable"),,, },
//			//{ _T("FillMode"),,, },
//			//{ _T("LastPixel"),,, },
//			//{ _T("ShadeMode"),,, },
//			//{ _T("SlopeScaleDepthBias"),,, },
//			//{ _T("SrcBlend"),,, },
//			//{ _T("StencilEnable"),,, },
//			//{ _T("StencilFail"),,, },
//			//{ _T("StencilFunc"),,, },
//			//{ _T("StencilMask"),,, },
//			//{ _T("StencilPass"),,, },
//			//{ _T("StencilRef"),,, },
//			//{ _T("StencilWriteMask"),,, },
//			//{ _T("StencilZFail"),,, },
//			//{ _T("TextureFactor"),,, },
//			//Wrap0 ～ Wrap15
//			//{ _T("ZEnable"),,, },
//			//{ _T("ZFunc"),,, },
//			//{ _T("ZWriteEnable"),,, },
//		};
//		for (int i = 0; i < LN_ARRAY_SIZE_OF(table); ++i)
//		{
//			if (name.Compare(table[i].name, -1, CaseSensitivity::CaseInsensitive) == 0 &&
//				value.Compare(table[i].value, -1, CaseSensitivity::CaseInsensitive) == 0)
//			{
//				out->stateId = table[i].stateId;
//				out->value = table[i].stateValue;
//				return true;
//			}
//		}
//		return false;	// 未対応のステート
//	}
//};

//==============================================================================
// PlainGLSLBuilder
//==============================================================================

////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//ShaderCompileResultLevel GLSLUtils::Build(GLGraphicsDevice* device, const void* code, size_t codeByteCount, GLShader** outShader, StringA* outMessage)
//{
//	GLuint program;
//	ShaderCompileResultLevel resultLevel = MakeShaderProgram(code, codeByteCount, &program, outMessage);
//	if (resultLevel == ShaderCompileResultLevel_Error) {
//		return resultLevel;
//	}
//
//	RefPtr<GLShader> shader(LN_NEW GLShader(device, program), false);
//
//	// uniform 変数の数を調べて、その数分 ShaderVariable 作成
//	GLint params;
//	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &params); LN_CHECK_GLERROR();
//
//	List<GLShaderPassVariableInfo> passVarList;
//	for (int i = 0; i < params; ++i)
//	{
//		GLShaderPassVariableInfo passVar;
//
//		GLsizei name_len = 0;
//		GLsizei var_size = 0;   // 配列サイズっぽい
//		GLenum  var_type = 0;
//		GLchar  name[256] = { 0 };
//		glGetActiveUniform(program, i, 256, &name_len, &var_size, &var_type, name); LN_CHECK_GLERROR();
//
//		// 変数情報作成
//		ShaderVariableBase::ShaderVariableTypeDesc desc;
//		GLShaderVariable::ConvertVariableTypeGLToLN(var_type, var_size, &desc);
//
//		// 名前を String 化
//		String tname;
//		tname.AssignCStr(name);
//
//		// Location
//		passVar.Location = glGetUniformLocation(program, name); LN_CHECK_GLERROR();
//
//		// ShaderVariable を作る
//		passVar.Variable = shader->TryCreateShaderVariable(desc, tname, String(), passVar.Location);	// TODO:この loc はいらない
//	
//		passVarList.Add(passVar);
//	}
//
//	// テクニック作成 (1対の頂点シェーダとフラグメントシェーダだけなので1つで良い)
//	GLShaderTechnique* tech = shader->CreateShaderTechnique(_T("Main"));
//
//	/*
//		■ attribute 変数への Usage 自動割り当てについて
//
//		特定の名前の attribute 変数は、自動的に Usage と UsageIndex を割り当てる。
//		これは Unity と同じ動作。
//
//		HLSL はシェーダコード上で POSITION 等のセマンティクスを指定することで
//		C++ コードをいじらなくてもレイアウトを作ることができるが、
//		一方 GLSL は自動レイアウトとかしてくれないので基本的に自分で配置を行うことになる。
//		ただ、それだと HLSL と GLSL を隠蔽する意味が無くなる。
//		そこで Unity 等は変数名によって自動割り当てを行っている。
//
//		とりあえず昨今最もメジャーな Unity に合わせ、次の名前の変数に自動割り当てをする。
//		attribute vec4 gl_Vertex;
//		attribute vec4 gl_Color;
//		attribute vec3 gl_Normal;
//		attribute vec4 gl_MultiTexCoord0;
//		attribute vec4 gl_MultiTexCoord1;
//
//		GLSL Programming/Unity/Debugging of Shaders
//		http://en.wikibooks.org/wiki/GLSL_Programming/Unity/Debugging_of_Shaders
//
//		ただ、これだと PMX として最低限必要な TexCoord0～7 をカバーできないので
//		それぞれ 0～15 (DX9の最大数) で UsageIndex をつけられるようにする。
//	*/
//	struct AttrNameData
//	{
//		const char* Name;
//		int			Length;
//	};
//	static const AttrNameData attrNameTable[] =
//	{
//		{ "",					0 },	// VertexElementUsage_Unknown,
//		//{ "gl_Vertex",			9 },	// VertexElementUsage_Position,
//		{ "ln_Vertex",			9 },	// VertexElementUsage_Position,
//		{ "ln_Normal",			9 },	// VertexElementUsage_Normal,
//		{ "ln_Color",			8 },	// VertexElementUsage_Color,
//		{ "ln_MultiTexCoord",	16 },	// VertexElementUsage_TexCoord,
//		{ "ln_PointSize",		12 },	// VertexElementUsage_PointSize,
//		{ "ln_BlendIndices",	15 },	// VertexElementUsage_BlendIndices,
//		{ "ln_BlendWeight",		14 },	// VertexElementUsage_BlendWeight,
//	};
//	assert(LN_ARRAY_SIZE_OF(attrNameTable) == VertexElementUsage_Max);
//
//	int8_t usageAttrIndexTable[VertexElementUsage_Max][GLShaderPass::MaxUsageIndex];
//	memset(usageAttrIndexTable, -1, sizeof(usageAttrIndexTable));
//
//	// attribute 変数の数
//	GLint attrCount;
//	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &attrCount);	LN_CHECK_GLERROR();
//	for (int i = 0; i < attrCount; ++i)
//	{
//		GLsizei name_len = 0;
//		GLsizei var_size = 0;
//		GLenum  var_type = 0;
//		GLchar  name[256] = { 0 };
//		glGetActiveAttrib(program, i, 256, &name_len, &var_size, &var_type, name); LN_CHECK_GLERROR();
//
//		for (int iUsage = 1; iUsage < VertexElementUsage_Max; ++iUsage)	// 0 番はダミーなので 1番から
//		{
//			// 変数名の一致確認 (UsageIndex の前の部分)
//			if (strncmp(name, attrNameTable[iUsage].Name, attrNameTable[iUsage].Length) == 0)
//			{
//				// UsageIndex チェック (gl_MultiTexCoord0 とかの '0' の部分)
//				size_t idxLen = name_len - attrNameTable[iUsage].Length;
//				const char* idxStr = &name[attrNameTable[iUsage].Length];
//				int usageIndex = -1;
//				if (idxLen == 0) {
//					usageIndex = 0;		// 省略されているので 0
//				}
//				else if (idxLen == 1 && idxStr[0] == '0') {
//					usageIndex = 0;		// 1文字で '0' なので 0 (atoi でエラーを確認できるようにするため特別処理)
//				}
//				else if (idxLen <= 2) {
//					usageIndex = atoi(idxStr);
//				}
//
//				if (0 <= usageIndex && usageIndex < GLShaderPass::MaxUsageIndex) {
//					// TODO: error  不正 UsageIndex
//				}
//
//				// Location 取得
//				int loc = glGetAttribLocation(program, name); LN_CHECK_GLERROR();
//				usageAttrIndexTable[iUsage][usageIndex] = glGetAttribLocation(program, name); LN_CHECK_GLERROR();
//				break;
//			}
//		}
//	}
//
//	// パス作成 (1対の頂点シェーダとフラグメントシェーダだけなので1つで良い)
//	GLShaderPass* pass = tech->CreateShaderPass(_T("Main"), program, (int8_t*)usageAttrIndexTable, passVarList);
//
//	shader.SafeAddRef();
//	*outShader = shader;
//	return resultLevel;
//}

//------------------------------------------------------------------------------
ShaderCompileResultLevel GLSLUtils::MakeShaderProgram(const char* vsCode, size_t vsCodeLen, const char* fsCode, size_t fsCodeLen, GLuint* outProgram, StringA* outMessage)
{
	*outProgram = NULL;
	outMessage->clear();

	// 頂点シェーダコード
	const char* vs_codes[] =
	{
		vsCode,
	};
	GLint vs_sizes[] =
	{
		(GLint)vsCodeLen,
	};

	// フラグメントシェーダコード
	const char* fs_codes[] =
	{
		fsCode,
	};
	GLint fs_sizes[] =
	{
		(GLint)fsCodeLen,
	};

	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
	GLuint program = 0;
	ShaderDiag diag;
	try
	{
		// シェーダオブジェクトの作成
		//vertexShader = glCreateShader(GL_VERTEX_SHADER);
		//LN_CHECK_GLERROR();
		//fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		//LN_CHECK_GLERROR();

		// シェーダオブジェクトのコンパイル
		vertexShader = CompileShader2(GL_VERTEX_SHADER, 1, vs_codes, vs_sizes, &diag);
		fragmentShader = CompileShader2(GL_FRAGMENT_SHADER, 1, fs_codes, fs_sizes, &diag);
		if (vertexShader == 0 || fragmentShader == 0)
		{
			// コンパイルエラー
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			return ShaderCompileResultLevel_Error;
		}
		(*outMessage) = diag.message;

		// プログラムオブジェクトの作成
		program = glCreateProgram();
		LN_CHECK_GLERROR();

		// シェーダオブジェクトをシェーダプログラムへの登録
		glAttachShader(program, vertexShader);
		LN_CHECK_GLERROR();
		glAttachShader(program, fragmentShader);
		LN_CHECK_GLERROR();

		// シェーダプログラムのリンク
		glLinkProgram(program);
		LN_CHECK_GLERROR();
		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		LN_CHECK_GLERROR();

		// ログがあるかチェック
		int logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		LN_CHECK_GLERROR();
		if (logSize > 1)
		{
			char* buf = LN_NEW char[logSize];
			int length;
			glGetProgramInfoLog(program, logSize, &length, buf);

			(*outMessage) += "Link info:\n";
			(*outMessage) += buf;
			LN_SAFE_DELETE_ARRAY(buf);

			LN_CHECK_GLERROR();
		}
		if (linked == GL_FALSE) {
			// リンクエラー
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			glDeleteProgram(program);
			return ShaderCompileResultLevel_Error;
		}

		// シェーダオブジェクトの削除 (プログラムオブジェクトだけあれば良い)
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		*outProgram = program;
		if (outMessage->isEmpty()) {
			return ShaderCompileResultLevel_Success;
		}
		else {
			return ShaderCompileResultLevel_Warning;
		}
	}
	catch (...)
	{
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(program);
		throw;
	}

	// ここに来ることは無いはず
	LN_THROW(0, InvalidOperationException);
}

//------------------------------------------------------------------------------
void GLSLUtils::AnalyzeLNBasicShaderCode(const char* code, size_t codeLen, GLuint type, const char* entryName, CodeRange* outCode)
{
	if (LN_CHECK_ARG(code != nullptr)) return;
	if (LN_CHECK_ARG(entryName != nullptr)) return;
	if (LN_CHECK_ARG(outCode != nullptr)) return;
	outCode->code = nullptr;
	outCode->length = 0;

	// make keyword
	StringA ifdef;
	StringA endKey;
	if (type == GL_VERTEX_SHADER)
	{
		ifdef = StringA::format("#ifdef LN_GLSL_VERTEX_{0}", entryName);
		endKey = StringA::format("#endif LN_GLSL_VERTEX_{0}", entryName);
	}
	else if (type == GL_FRAGMENT_SHADER)
	{
		ifdef = StringA::format("#ifdef LN_GLSL_FRAGMENT_{0}", entryName);
		endKey = StringA::format("#endif LN_GLSL_FRAGMENT_{0}", entryName);
	}

	// find begin - end
	int codeBegin = StringTraits::indexOf(code, codeLen, ifdef.c_str(), -1);
	if (LN_CHECK_FORMAT(codeBegin >= 0)) return;
	//int codeEnd = StringTraits::IndexOf(code, codeLen, "LN_GLSL_END", 11, codeBegin);
	int codeEnd = StringTraits::indexOf(code, codeLen, endKey.c_str(), endKey.getLength(), codeBegin);
	if (LN_CHECK_FORMAT(codeEnd >= 0)) return;

	// make range
	const char* begin = code + codeBegin + ifdef.getLength();
	const char* end = code + codeEnd;

	// output
	outCode->code = begin;
	outCode->length = end - begin;
}

//------------------------------------------------------------------------------
GLuint GLSLUtils::CompileShader2(GLuint type, int codeCount, const char** codes, const GLint* sizes, ShaderDiag* diag)
{
	GLuint shader = glCreateShader(type);
	LN_FAIL_CHECK_GLERROR() return 0;

	glShaderSource(shader, codeCount, (const GLchar **)codes, sizes);
	LN_FAIL_CHECK_GLERROR() { glDeleteShader(shader); return 0; }

	glCompileShader(shader);
	LN_FAIL_CHECK_GLERROR() { glDeleteShader(shader); return 0; }

	// result
	GLint compiled;
	GLint logSize;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);	// excluding the null terminator

	if (logSize > 1)
	{
		ByteBuffer buf(logSize);
		int length;
		glGetShaderInfoLog(shader, logSize, &length, (GLchar*)buf.getData());
		diag->message += "Compile info:\n";
		diag->message += (const char*)buf.getConstData();
		diag->level = ShaderCompileResultLevel_Warning;
	}

	if (compiled == GL_FALSE)
	{
		diag->level = ShaderCompileResultLevel_Error;
		glDeleteShader(shader);
		shader = 0;
	}

	return shader;
}

//------------------------------------------------------------------------------
GLuint GLSLUtils::CompileShader3(GLuint type, const char* code, GLint codeLen, const char* entryName, ShaderDiag* diag)
{
	// make keyword
	StringA ifdef;
	if (type == GL_VERTEX_SHADER)
	{
		ifdef = StringA::format("#define LN_GLSL_VERTEX_{0}\n", entryName);
	}
	else if (type == GL_FRAGMENT_SHADER)
	{
		ifdef = StringA::format("#define LN_GLSL_FRAGMENT_{0}\n", entryName);
	}

	const char* codes[] =
	{
		"#version 120\n\n#define LN_GLSL\n",
		ifdef.c_str(),
		code,
	};
	GLint codeLens[] =
	{
		strlen(codes[0]),
		ifdef.getLength(),
		codeLen,
	};

	return CompileShader2(type, 3, codes, codeLens, diag);
}

//==============================================================================
// GLShader
//==============================================================================

//------------------------------------------------------------------------------
GLShader::GLShader()
	: m_device(nullptr)
{
}

//------------------------------------------------------------------------------
GLShader::~GLShader()
{
	for (auto& pair : m_glVertexShaderEntryMap)
	{
		glDeleteShader(pair.second);
	}
	for (auto& pair : m_glPixelShaderEntryMap)
	{
		glDeleteShader(pair.second);
	}

	for (GLShaderVariable* v : m_variables) {
		LN_SAFE_RELEASE(v);
	}
	for (GLShaderTechnique* t : m_techniques) {
		LN_SAFE_RELEASE(t);
	}
}

//------------------------------------------------------------------------------
void GLShader::initialize(GLGraphicsDevice* device, const void* code_, size_t codeByteCount)
{
	m_device = device;

	const char* code = (const char*)code_;
	if (memcmp(code, "#ifdef LN_FXC_METADATA", 22) == 0)
	{
		int index = StringTraits::indexOf(code, codeByteCount, "#endif", 6);
		LN_THROW(index >= 0, InvalidFormatException);
		StringReader reader(String::fromNativeCharString(code + 22, index - 22));
		tr::JsonReader2 json(&reader);
		json.readAsStartObject();

		// vertexShaders
		if (json.readAsPropertyName() == _T("vertexShaders"))
		{
			json.readAsStartArray();
			while (json.read() && json.getTokenType() != tr::JsonToken::EndArray)
			{
				String entry = json.getValue();
				m_glVertexShaderEntryMap[entry] = CompileShader(code, codeByteCount, entry.toStringA().c_str(), GL_VERTEX_SHADER);
			}
		}
		// pixelShaders
		if (json.readAsPropertyName() == _T("pixelShaders"))
		{
			json.readAsStartArray();
			while (json.read() && json.getTokenType() != tr::JsonToken::EndArray)
			{
				String entry = json.getValue();
				m_glPixelShaderEntryMap[entry] = CompileShader(code, codeByteCount, entry.toStringA().c_str(), GL_FRAGMENT_SHADER);
			}
		}
		// techniques
		if (json.readAsPropertyName() == _T("techniques"))
		{
			json.readAsStartArray();
			while (json.read() && json.getTokenType() != tr::JsonToken::EndArray)
			{
				m_techniques.add(GLShaderTechnique::deserialize(this, &json));
			}
		}
		// parameters
		if (json.readAsPropertyName() == _T("parameters"))
		{
			json.readAsStartArray();
			while (json.read() && json.getTokenType() != tr::JsonToken::EndArray)
			{
				bool overwrited = false;	//TODO: いらないかも
				GLShaderVariable* v = GLShaderVariable::deserialize(this, &json, &overwrited);
				//if (!overwrited)
				//{
				//	m_variables.Add(v);
				//}
			}
		}

		json.readAsEndObject();
	}
	else
	{
		GLuint vertShader = CompileShader(code, codeByteCount, "Main", GL_VERTEX_SHADER);
		GLuint fragShader = CompileShader(code, codeByteCount, "Main", GL_FRAGMENT_SHADER);
		m_glVertexShaderEntryMap[_T("Main")] = vertShader;	// delete のため
		m_glPixelShaderEntryMap[_T("Main")] = fragShader;	// delete のため
		auto* tech = LN_NEW GLShaderTechnique();
		tech->initialize(this, _T("Main"));
		m_techniques.add(tech);
		auto* pass = LN_NEW GLShaderPass();
		pass->initialize(this, _T("Main"), vertShader, fragShader);
		tech->AddPass(pass);
	}
}

//------------------------------------------------------------------------------
//void GLShader::create()
//{
//}

//------------------------------------------------------------------------------
GLShaderVariable* GLShader::FindShaderVariable(const String& name)
{
	for (GLShaderVariable* v : m_variables)
	{
		if (v->getName() == name)
		{
			//if (v->getSemanticName() == semanticName) {
			//	return v;
			//}
			//else {
			//	// 同名変数なのにセマンティックが違う。なんかおかしい
			//	LN_THROW(0, InvalidOperationException);
			//}
			return v;
		}
	}
	return nullptr;
}

//------------------------------------------------------------------------------
GLShaderVariable* GLShader::CreateShaderVariable(ShaderVariableBase::ShaderVariableTypeDesc desc, const String& name, const String& semanticName)
{
	GLShaderVariable* v = LN_NEW GLShaderVariable();
	v->initialize(this, desc, name, semanticName, 0);
	m_variables.add(v);
	return v;
}

////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//GLShaderTechnique* GLShader::CreateShaderTechnique(const String& name)
//{
//	GLShaderTechnique* tech = LN_NEW GLShaderTechnique(this, name);
//	m_techniques.Add(tech);
//	return tech;
//}
//
//------------------------------------------------------------------------------
int GLShader::GetVariableCount() const
{
	return m_variables.getCount();
}

//------------------------------------------------------------------------------
IShaderVariable* GLShader::GetVariable(int index) const
{
	return m_variables[index];
}

//------------------------------------------------------------------------------
int GLShader::GetTechniqueCount() const
{
	return m_techniques.getCount();
}

//------------------------------------------------------------------------------
IShaderTechnique* GLShader::GetTechnique(int index) const
{
	return m_techniques[index];
}

//------------------------------------------------------------------------------
void GLShader::onLostDevice()
{
	LN_THROW(0, NotImplementedException);
}

//------------------------------------------------------------------------------
void GLShader::onResetDevice()
{
	LN_THROW(0, NotImplementedException);
}

//------------------------------------------------------------------------------
GLuint GLShader::CompileShader(const char* code, size_t codeLen, const char* entryName, GLuint type)
{
	//GLSLUtils::CodeRange codeRange;
	//GLSLUtils::AnalyzeLNBasicShaderCode(code, codeLen, type, entryName, &codeRange);

	//const char* codes[] =
	//{
	//	codeRange.code,
	//	"#define LN_GLSL\n",
	//};
	//GLint codeLens[] =
	//{
	//	codeRange.length,
	//	strlen(codes[1]),
	//};

	//GLuint shader = GLSLUtils::CompileShader2(type, 2, codes, codeLens, &m_diag);
	GLuint shader = GLSLUtils::CompileShader3(type, code, codeLen, entryName, &m_diag);
	if (shader == 0)	// TODO: エラーメッセージとか
	{
		glDeleteShader(shader);
		LN_THROW(0, InvalidFormatException);
	}
	return shader;
}

//------------------------------------------------------------------------------
GLuint GLShader::GetVertexShader(const String& name)
{
	auto itr = m_glVertexShaderEntryMap.find(name);
	LN_THROW(itr != m_glVertexShaderEntryMap.end(), KeyNotFoundException);
	return itr->second;
}

//------------------------------------------------------------------------------
GLuint GLShader::GetFlagmentShader(const String& name)
{
	auto itr = m_glPixelShaderEntryMap.find(name);
	LN_THROW(itr != m_glPixelShaderEntryMap.end(), KeyNotFoundException);
	return itr->second;
}

//==============================================================================
// GLShaderVariable
//==============================================================================

//------------------------------------------------------------------------------
GLShaderVariable* GLShaderVariable::deserialize(GLShader* ownerShader, tr::JsonReader2* json, bool* outOverwrited)
{
	String name, semantic/*, samplerName*/;
	bool shared;
	if (json->readAsPropertyName() == _T("name")) name = json->readAsString();

	GLShaderVariable* var = ownerShader->FindShaderVariable(name);
	if (var == nullptr)
	{
		// TODO: ダミーのままでいいものか・・・
		ShaderVariableTypeDesc dummy{};
		var = ownerShader->CreateShaderVariable(dummy, name, String());
		*outOverwrited = false;
	}
	else
	{
		*outOverwrited = true;
	}
	//RefPtr<GLShaderVariable> var = ownerShader->TryCreateShaderVariable(dummy, name, String(), 0);	// TODO:この loc はいらない

	while (json->read())
	{
		if (json->getTokenType() == tr::JsonToken::EndObject) break;
		if (json->getTokenType() == tr::JsonToken::PropertyName)
		{
			const String& prop = json->getValue();
			if (prop == _T("semantic")) var->SetSemanticName(json->readAsString());
			//else if (prop == _T("samplerName")) samplerName = json->ReadAsString();
			else if (prop == _T("shared")) shared = json->readAsBool();
			else if (prop == _T("samplerStatus"))
			{
				json->readAsStartObject();
				while (json->read() && json->getTokenType() != tr::JsonToken::EndObject)
				{
					String stateName = json->getValue();
					String stateValue = json->readAsString();
					//if (json->ReadAsPropertyName() == _T("stateName")) stateName = json->ReadAsString();
					//if (json->ReadAsPropertyName() == _T("value")) stateValue = json->ReadAsString();

					// TODO
					//SamplerStatePair state;
					//if (SamplerStatePair::Make(stateName, stateValue, &state))
					//{
					//	var->m_samplerStatus.Add(state);
					//}
				}
			}
			else if (prop == _T("annotations"))
			{
				json->readAsStartArray();
				while (json->read() && json->getTokenType() != tr::JsonToken::EndArray)
				{
					var->m_annotations.add(GLShaderAnnotation::deserialize(json));
				}
			}
		}
	}

	//var->initialize(ownerShader, , name, semantic, 0);
	//json->ReadAsEndObject();
	return var;
}

//------------------------------------------------------------------------------
GLShaderVariable::GLShaderVariable()
	: m_ownerShader(nullptr)
	, m_glUniformLocation(0)
{
}

//------------------------------------------------------------------------------
GLShaderVariable::~GLShaderVariable()
{
	for (GLShaderAnnotation* anno : m_annotations)
	{
		anno->release();
	}
}

//------------------------------------------------------------------------------
void GLShaderVariable::initialize(GLShader* owner, ShaderVariableTypeDesc desc, const String& name, const String& semanticName, GLint location)
{
	m_ownerShader = owner;
	m_glUniformLocation = location;
	ShaderVariableBase::initialize(desc, name, semanticName);
	MakeInitialValue();
}

//------------------------------------------------------------------------------
void GLShaderVariable::apply(int location, int textureStageIndex)
{
	if (!m_value.isValid()) {
		return;		// 一度も set されていない変数や、ライブラリが認識できない型である。その場合は何も処理をせず、デフォルト値のままにする。
	}

	MemoryStream* tempBuffer = m_ownerShader->getGraphicsDevice()->GetUniformTempBuffer();
	BinaryWriter* tempWriter = m_ownerShader->getGraphicsDevice()->GetUniformTempBufferWriter();
	tempWriter->seek(0, SeekOrigin_Begin);

	const Vector4* vec;

	switch (m_desc.Type)
	{
	case ShaderVariableType_Bool:
		glUniform1i(location, m_value.getBool());
		LN_FAIL_CHECK_GLERROR() return;
		break;
	case ShaderVariableType_BoolArray:
	{
		const bool* begin = m_value.getBoolArray();
		const bool* end = begin + m_desc.Elements;

		std::for_each(begin, end, [&tempWriter](bool v) { GLint i = (v) ? 1 : 0; tempWriter->write(&i, sizeof(GLint)); });
		glUniform1iv(location, m_desc.Elements, (const GLint*)tempBuffer->getBuffer());
		LN_FAIL_CHECK_GLERROR() return;
		break;
	}
	case ShaderVariableType_Int:
		glUniform1i(location, m_value.getInt());
		LN_FAIL_CHECK_GLERROR() return;
		break;
	case ShaderVariableType_Float:
		glUniform1f(location, m_value.getFloat());
		LN_FAIL_CHECK_GLERROR() return;
		break;
	case ShaderVariableType_Vector:
		vec = &m_value.getVector();
		if (m_desc.Columns == 2) {
			glUniform2f(location, vec->x, vec->y);
		}
		else if (m_desc.Columns == 3) {
			glUniform3f(location, vec->x, vec->y, vec->z);
		}
		else if (m_desc.Columns == 4) {
			glUniform4f(location, vec->x, vec->y, vec->z, vec->w);
		}
		LN_FAIL_CHECK_GLERROR() return;
		break;
	case ShaderVariableType_VectorArray:
	{
		const Vector4* begin = m_value.getVectorArray();
		const Vector4* end = begin + m_desc.Elements;

		if (m_desc.Columns == 2)
		{
			std::for_each(begin, end, [&tempWriter](const Vector4& v) { tempWriter->write(&v, sizeof(float) * 2); });
			glUniform2fv(location, m_desc.Elements, (const GLfloat*)tempBuffer->getBuffer());
			LN_FAIL_CHECK_GLERROR() return;
		}
		else if (m_desc.Columns == 3)
		{
			std::for_each(begin, end, [&tempWriter](const Vector4& v) { tempWriter->write(&v, sizeof(float) * 3); });
			glUniform3fv(location, m_desc.Elements, (const GLfloat*)tempBuffer->getBuffer());
			LN_FAIL_CHECK_GLERROR() return;
		}
		else if (m_desc.Columns == 4)
		{
			glUniform4fv(location, m_desc.Elements, (const GLfloat*)m_value.getVectorArray());
			LN_FAIL_CHECK_GLERROR() return;
		}
		else
		{
			LN_UNREACHABLE();
		}
		break;
	}
	case ShaderVariableType_Matrix:
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat*)&m_value.getMatrix());
		LN_FAIL_CHECK_GLERROR();
		break;
	}
	case ShaderVariableType_MatrixArray:
		glUniformMatrix4fv(location, m_desc.Elements, GL_FALSE, (const GLfloat*)m_value.getMatrixArray());
		break;
	case ShaderVariableType_DeviceTexture:
		// textureStageIndex のテクスチャステージにバインド
		glActiveTexture(GL_TEXTURE0 + textureStageIndex);
		LN_FAIL_CHECK_GLERROR() return;

		if (m_value.getDeviceTexture() != nullptr)
			glBindTexture(GL_TEXTURE_2D, static_cast<GLTextureBase*>(m_value.getDeviceTexture())->GetGLTexture());
		else
			glBindTexture(GL_TEXTURE_2D, 0);
		LN_FAIL_CHECK_GLERROR();
		

		//LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mSamplerState->MinFilter);
		//LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mSamplerState->MagFilter);
		//LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mSamplerState->AddressU);
		//LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mSamplerState->AddressV);

		// テクスチャステージ番号をセット
		glUniform1i(location, textureStageIndex);
		LN_FAIL_CHECK_GLERROR() return;
		break;
	case ShaderVariableType_String:
		// GLSL に String 型は無い。この値は本ライブラリで使用しているただのメタデータ。
		break;
	default:
		LN_UNREACHABLE();
		break;
	}
}

//------------------------------------------------------------------------------
int GLShaderVariable::GetAnnotationCount()
{
	return m_annotations.getCount();
}

//------------------------------------------------------------------------------
IShaderVariable* GLShaderVariable::GetAnnotation(int index)
{
	return m_annotations[index];
}

//------------------------------------------------------------------------------
void GLShaderVariable::ConvertVariableTypeGLToLN(const char* name, GLenum gl_type, GLsizei gl_var_size, ShaderVariableBase::ShaderVariableTypeDesc* desc)
{
	desc->Elements = 0;
	desc->Shared = false;

#define SET_LNDESC( c_, t_, row_, col_ ) { /*desc->Class = (c_); */desc->Type = (t_); desc->Rows = (row_); desc->Columns = (col_); }

	switch (gl_type)
	{
	case GL_FLOAT:      SET_LNDESC(LN_SVC_SCALAR, ShaderVariableType_Float, 1, 1); break;
	case GL_FLOAT_VEC2: SET_LNDESC(LN_SVC_VECTOR, ShaderVariableType_Vector, 1, 2); break;
	case GL_FLOAT_VEC3: SET_LNDESC(LN_SVC_VECTOR, ShaderVariableType_Vector, 1, 3); break;
	case GL_FLOAT_VEC4: SET_LNDESC(LN_SVC_VECTOR, ShaderVariableType_Vector, 1, 4); break;

	case GL_INT:        SET_LNDESC(LN_SVC_SCALAR, ShaderVariableType_Int, 1, 1); break;
	//case GL_INT_VEC2:   SET_LNDESC(LN_SVC_VECTOR, ShaderVariableType_IntVector, 1, 2); break;
	//case GL_INT_VEC3:   SET_LNDESC(LN_SVC_VECTOR, ShaderVariableType_IntVector, 1, 3); break;
	//case GL_INT_VEC4:   SET_LNDESC(LN_SVC_VECTOR, ShaderVariableType_IntVector, 1, 4); break;

	case GL_BOOL:        SET_LNDESC(LN_SVC_SCALAR, ShaderVariableType_Bool, 1, 1); break;
	//case GL_BOOL_VEC2:   SET_LNDESC(LN_SVC_VECTOR, ShaderVariableType_BoolVector, 1, 2); break;
	//case GL_BOOL_VEC3:   SET_LNDESC(LN_SVC_VECTOR, ShaderVariableType_BoolVector, 1, 3); break;
	//case GL_BOOL_VEC4:   SET_LNDESC(LN_SVC_VECTOR, ShaderVariableType_BoolVector, 1, 4); break;

	case GL_FLOAT_MAT2:     SET_LNDESC(LN_SVC_MATRIX, ShaderVariableType_Matrix, 2, 2); break;
	case GL_FLOAT_MAT3:     SET_LNDESC(LN_SVC_MATRIX, ShaderVariableType_Matrix, 3, 3); break;
	case GL_FLOAT_MAT4:     SET_LNDESC(LN_SVC_MATRIX, ShaderVariableType_Matrix, 4, 4); break;

	case GL_SAMPLER_2D:         SET_LNDESC(LN_SVC_SAMPLER, ShaderVariableType_DeviceTexture, 1, 1); break;
	case GL_SAMPLER_CUBE:       SET_LNDESC(LN_SVC_SAMPLER, ShaderVariableType_DeviceTexture, 1, 1); break;

//#if !defined(LNOTE_GLES)
	case GL_FLOAT_MAT2x3:   SET_LNDESC(LN_SVC_MATRIX, ShaderVariableType_Matrix, 2, 3); break;
	case GL_FLOAT_MAT2x4:   SET_LNDESC(LN_SVC_MATRIX, ShaderVariableType_Matrix, 2, 4); break;
	case GL_FLOAT_MAT3x2:   SET_LNDESC(LN_SVC_MATRIX, ShaderVariableType_Matrix, 3, 2); break;
	case GL_FLOAT_MAT3x4:   SET_LNDESC(LN_SVC_MATRIX, ShaderVariableType_Matrix, 3, 4); break;
	case GL_FLOAT_MAT4x2:   SET_LNDESC(LN_SVC_MATRIX, ShaderVariableType_Matrix, 4, 2); break;
	case GL_FLOAT_MAT4x3:   SET_LNDESC(LN_SVC_MATRIX, ShaderVariableType_Matrix, 4, 3); break;

	//case GL_FLOAT:      SET_LNDESC( LN_SVC_SCALAR, LN_SVT_FLOAT, 1, 1 ); break;
	//case GL_FLOAT_VEC2: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 2 ); break;
	//case GL_FLOAT_VEC3: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 3 ); break;
	//case GL_FLOAT_VEC4: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 4 ); break;

	case GL_SAMPLER_1D:         SET_LNDESC(LN_SVC_SAMPLER, ShaderVariableType_DeviceTexture, 1, 1); break;
	case GL_SAMPLER_3D:         SET_LNDESC(LN_SVC_SAMPLER, ShaderVariableType_DeviceTexture, 1, 1); break;

	case GL_SAMPLER_1D_SHADOW:  SET_LNDESC(LN_SVC_SAMPLER, ShaderVariableType_Unknown, 1, 1); break;
	case GL_SAMPLER_2D_SHADOW:  SET_LNDESC(LN_SVC_SAMPLER, ShaderVariableType_Unknown, 1, 1); break;
//#endif
	default: SET_LNDESC(LN_SVC_SAMPLER, ShaderVariableType_Unknown, 0, 0); break;
	}

	// check array type (e.g. "list[0]")
	if (StringTraits::indexOf(name, -1, "[", 1) >= 0)
	{
		desc->Elements = gl_var_size;

		switch (desc->Type)
		{
		case ShaderVariableType_Bool:
			desc->Type = ShaderVariableType_BoolArray;
			break;
		case ShaderVariableType_Float:
			desc->Type = ShaderVariableType_FloatArray;
			break;
		case ShaderVariableType_Vector:
			desc->Type = ShaderVariableType_VectorArray;
			break;
		case ShaderVariableType_Matrix:
			desc->Type = ShaderVariableType_MatrixArray;
			break;
		default:
			LN_UNREACHABLE();
			break;
		}
	}
}

//==============================================================================
// GLShaderAnnotation
//==============================================================================

//------------------------------------------------------------------------------
GLShaderAnnotation* GLShaderAnnotation::deserialize(tr::JsonReader2* json)
{
	auto anno = RefPtr<GLShaderAnnotation>::makeRef();
	String type, name, value;
	if (json->readAsPropertyName() == _T("type")) type = json->readAsString();
	if (json->readAsPropertyName() == _T("name")) name = json->readAsString();
	if (json->readAsPropertyName() == _T("value")) value = json->readAsString();
	json->readAsEndObject();
	anno->initialize(type, name, value);
	return anno.detachMove();
}

//------------------------------------------------------------------------------
GLShaderAnnotation::GLShaderAnnotation()
{
}

//------------------------------------------------------------------------------
GLShaderAnnotation::~GLShaderAnnotation()
{
}

//------------------------------------------------------------------------------
void GLShaderAnnotation::initialize(const String& type, const String& name, const String& value)
{
	ShaderVariableTypeDesc desc;
	desc.Type = ShaderVariableType_Unknown;
	desc.Columns = 0;
	desc.Rows = 0;			// 行列はサポートしていない
	desc.Elements = 0;		// 配列はサポートしていない
	desc.Shared = false;	// annotation に shared はつかない

	if (type.indexOf(_T("bool")) == 0)
	{
		desc.Type = ShaderVariableType_Bool;
		ShaderVariableBase::initialize(desc, name, String::getEmpty());
		ShaderVariableBase::setBool(value[0] == 't');	// "true"?
	}
	else if (type.indexOf(_T("int")) == 0)
	{
		desc.Type = ShaderVariableType_Int;
		ShaderVariableBase::initialize(desc, name, String::getEmpty());
		ShaderVariableBase::setInt(value.toInt32());
	}
	else if (type.indexOf(_T("string")) == 0)
	{
		desc.Type = ShaderVariableType_Int;
		ShaderVariableBase::initialize(desc, name, String::getEmpty());
		ShaderVariableBase::setString(value.c_str());
	}
	else if (type.indexOf(_T("float")) == 0)
	{
		if (type.getLength() == 5)
		{
			desc.Type = ShaderVariableType_Float;
			ShaderVariableBase::initialize(desc, name, String::getEmpty());
			ShaderVariableBase::setFloat((float)StringTraits::toDouble(value.c_str(), value.getLength()));
		}
		else if (type.getLength() == 6)
		{
			// vector
			desc.Columns = (type[6] - '0');

			StringArray tokens = value.split(_T(","));
			Vector4 v;
			v.x = (float)((tokens.getCount() >= 1) ? StringTraits::toDouble(tokens[0].c_str(), tokens[0].getLength()) : 0);
			v.y = (float)((tokens.getCount() >= 2) ? StringTraits::toDouble(tokens[1].c_str(), tokens[1].getLength()) : 0);
			v.z = (float)((tokens.getCount() >= 3) ? StringTraits::toDouble(tokens[2].c_str(), tokens[2].getLength()) : 0);
			v.w = (float)((tokens.getCount() >= 4) ? StringTraits::toDouble(tokens[3].c_str(), tokens[3].getLength()) : 0);

			desc.Type = ShaderVariableType_Vector;
			ShaderVariableBase::initialize(desc, name, String::getEmpty());
			ShaderVariableBase::setVector(v);
		}
		else
		{
			// 行列はサポートしていない
			LN_THROW(0, InvalidFormatException);
		}
	}
}

//==============================================================================
// GLShaderTechnique
//==============================================================================

//------------------------------------------------------------------------------
GLShaderTechnique* GLShaderTechnique::deserialize(GLShader* ownerShader, tr::JsonReader2* json)
{
	String name;
	if (json->readAsPropertyName() == _T("name")) name = json->readAsString();
	auto tech = RefPtr<GLShaderTechnique>::makeRef();
	tech->initialize(ownerShader, name);

	if (json->readAsPropertyName() == _T("passes"))
	{
		json->readAsStartArray();
		while (json->read() && json->getTokenType() != tr::JsonToken::EndArray)
		{
			tech->m_passes.add(GLShaderPass::deserialize(ownerShader, json));
		}
	}

	while (json->read())
	{
		if (json->getTokenType() == tr::JsonToken::EndObject) break;
		if (json->getTokenType() == tr::JsonToken::PropertyName)
		{
			if (json->readAsPropertyName() == _T("annotations"))
			{
				json->readAsStartArray();
				while (json->read() && json->getTokenType() != tr::JsonToken::EndArray)
				{
					tech->m_annotations.add(GLShaderAnnotation::deserialize(json));
				}
			}
		}
	}

	return tech.detachMove();
}

//------------------------------------------------------------------------------
GLShaderTechnique::GLShaderTechnique()
	: m_ownerShader(nullptr)
{
}

//------------------------------------------------------------------------------
GLShaderTechnique::~GLShaderTechnique()
{
	for (GLShaderPass* pass : m_passes) {
		LN_SAFE_RELEASE(pass);
	}
	for (GLShaderAnnotation* anno : m_annotations)
	{
		anno->release();
	}
}

//------------------------------------------------------------------------------
void GLShaderTechnique::initialize(GLShader* ownerShader, const String& name)
{
	m_ownerShader = ownerShader;
	m_name = name;
}

//------------------------------------------------------------------------------
IShaderPass* GLShaderTechnique::getPass(int index)
{
	return m_passes[index];
}

//==============================================================================
// GLShaderPass
//==============================================================================

//------------------------------------------------------------------------------
GLShaderPass* GLShaderPass::deserialize(GLShader* ownerShader, tr::JsonReader2* json)
{
	auto pass = RefPtr<GLShaderPass>::makeRef();
	String name, vsName, psName;

	if (json->readAsPropertyName() == _T("name")) name = json->readAsString();
	if (json->readAsPropertyName() == _T("vertexShader")) vsName = json->readAsString();
	if (json->readAsPropertyName() == _T("pixelShader")) psName = json->readAsString();

	pass->initialize(ownerShader, name, vsName, psName);

	while (json->read())
	{
		if (json->getTokenType() == tr::JsonToken::EndObject) break;
		if (json->getTokenType() == tr::JsonToken::PropertyName)
		{
			if (json->getValue() == _T("status"))
			{
				json->readAsStartArray();
				while (json->read() && json->getTokenType() != tr::JsonToken::EndArray)
				{
					// TODO:
				}
			}
			else if (json->readAsPropertyName() == _T("annotations"))
			{
				json->readAsStartArray();
				while (json->read() && json->getTokenType() != tr::JsonToken::EndArray)
				{
					pass->m_annotations.add(GLShaderAnnotation::deserialize(json));
				}
			}
		}
	}

	return pass.detachMove();
}

//------------------------------------------------------------------------------
GLShaderPass::GLShaderPass()
	: m_ownerShader(nullptr)
{
}

//------------------------------------------------------------------------------
GLShaderPass::~GLShaderPass()
{
	for (GLShaderAnnotation* anno : m_annotations)
	{
		anno->release();
	}
	if (m_program != 0)
	{
		glDeleteProgram(m_program);
		m_program = 0;
	}
}

//------------------------------------------------------------------------------
void GLShaderPass::initialize(GLShader* ownerShader, const String& name, GLuint vertShader, GLuint fragShader)
{
	m_ownerShader = ownerShader;
	m_name = name;
	if (LinkShader(vertShader, fragShader, m_ownerShader->GetDiag()))
	{
		build();
	}
}

//------------------------------------------------------------------------------
void GLShaderPass::initialize(GLShader* ownerShader, const String& name, const String& vertShaderName, const String& fragShaderName)
{
	GLuint vertShader = ownerShader->GetVertexShader(vertShaderName);
	GLuint fragShader = ownerShader->GetFlagmentShader(fragShaderName);
	initialize(ownerShader, name, vertShader, fragShader);
}

//------------------------------------------------------------------------------
int GLShaderPass::GetUsageAttributeIndex(VertexElementUsage usage, int index)
{
	return m_usageAttrIndexTable[usage][index];
}

//------------------------------------------------------------------------------
void GLShaderPass::apply()
{
	glUseProgram(m_program); LN_CHECK_GLERROR();

	for (GLShaderPassVariableInfo& v : m_passVarList)
	{
		v.Variable->apply(v.Location, v.TextureStageIndex);
	}

	static_cast<GLRenderer*>(m_ownerShader->getGraphicsDevice()->getRenderer())->SetCurrentShaderPass(this);
}

//------------------------------------------------------------------------------
//void GLShaderPass::end()
//{
//	glUseProgram(NULL);
//	static_cast<GLRenderer*>(m_ownerShader->getGraphicsDevice()->getRenderer())->SetCurrentShaderPass(NULL);
//}

//------------------------------------------------------------------------------
bool GLShaderPass::LinkShader(GLuint vertShader, GLuint fragShader, ShaderDiag* diag)
{
	// 最初に失敗扱いにしておく
	diag->level = ShaderCompileResultLevel_Error;
	
	// プログラムオブジェクトの作成
	m_program = glCreateProgram();
	LN_CHECK_GLERROR();

	// シェーダオブジェクトをシェーダプログラムへの登録
	glAttachShader(m_program, vertShader);
	LN_CHECK_GLERROR();
	glAttachShader(m_program, fragShader);
	LN_CHECK_GLERROR();

	// シェーダプログラムのリンク
	glLinkProgram(m_program);
	LN_CHECK_GLERROR();
	GLint linked;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
	LN_CHECK_GLERROR();

	// ログがあるかチェック
	int logSize;
	glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logSize);
	LN_CHECK_GLERROR();
	if (logSize > 1)
	{
		char* buf = LN_NEW char[logSize];
		int length;
		glGetProgramInfoLog(m_program, logSize, &length, buf);
		diag->message += "Link info:\n";
		diag->message += buf;
		LN_SAFE_DELETE_ARRAY(buf);
		LN_CHECK_GLERROR();

		diag->level = ShaderCompileResultLevel_Warning;
	}
	else
	{
		diag->level = ShaderCompileResultLevel_Success;
	}
	if (linked == GL_FALSE)
	{
		// リンクエラー
		glDeleteProgram(m_program);
		m_program = 0;
		diag->level = ShaderCompileResultLevel_Error;
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------
void GLShaderPass::build()
{
	int textureVarCount = 0;

	//---------------------------------------------------------
	// この Pass にが使っているシェーダ変数の ShaderVariable 作成
	GLint params;
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &params); LN_CHECK_GLERROR();
	for (int i = 0; i < params; ++i)
	{
		GLShaderPassVariableInfo passVar;

		GLsizei name_len = 0;
		GLsizei var_size = 0;   // 配列サイズっぽい
		GLenum  var_type = 0;
		GLchar  name[256] = { 0 };
		glGetActiveUniform(m_program, i, 256, &name_len, &var_size, &var_type, name); LN_CHECK_GLERROR();

		// 変数情報作成
		ShaderVariableBase::ShaderVariableTypeDesc desc;
		GLShaderVariable::ConvertVariableTypeGLToLN(name, var_type, var_size, &desc);

		// 名前を String 化
		String tname;
		tname.assignCStr(name);
		tname = tname.replace(_T("[0]"), _T(""));

		// Location
		passVar.Location = glGetUniformLocation(m_program, name); LN_CHECK_GLERROR();

		// ShaderVariable を作る
		passVar.Variable = m_ownerShader->FindShaderVariable(tname);
		if (passVar.Variable == nullptr)
		{
			passVar.Variable = m_ownerShader->CreateShaderVariable(desc, tname, String());
		}

		// テクスチャ型の変数にステージ番号を振っていく。
		if (passVar.Variable->getType() == ShaderVariableType_DeviceTexture)
		{
			passVar.TextureStageIndex = textureVarCount;
			textureVarCount++;
		}
		else
		{
			passVar.TextureStageIndex = -1;
		}

		// 格納
		m_passVarList.add(passVar);
	}
	
	//---------------------------------------------------------
	// attribute 割り当て
	/*
		■ attribute 変数への Usage 自動割り当てについて

		特定の名前の attribute 変数は、自動的に Usage と UsageIndex を割り当てる。
		これは Unity と同じ動作。

		HLSL はシェーダコード上で POSITION 等のセマンティクスを指定することで
		C++ コードをいじらなくてもレイアウトを作ることができるが、
		一方 GLSL は自動レイアウトとかしてくれないので基本的に自分で配置を行うことになる。
		ただ、それだと HLSL と GLSL を隠蔽する意味が無くなる。
		そこで Unity 等は変数名によって自動割り当てを行っている。

		とりあえず昨今最もメジャーな Unity に合わせ、次の名前の変数に自動割り当てをする。
		attribute vec4 gl_Vertex;
		attribute vec4 gl_Color;
		attribute vec3 gl_Normal;
		attribute vec4 gl_MultiTexCoord0;
		attribute vec4 gl_MultiTexCoord1;

		GLSL Programming/Unity/Debugging of Shaders
		http://en.wikibooks.org/wiki/GLSL_Programming/Unity/Debugging_of_Shaders

		ただ、これだと PMX として最低限必要な TexCoord0～7 をカバーできないので
		それぞれ 0～15 (DX9の最大数) で UsageIndex をつけられるようにする。
	*/
	struct AttrNameData
	{
		const char* name;
		int			length;
	};
	static const AttrNameData attrNameTable[] =
	{
		{ "",					0 },	// VertexElementUsage_Unknown,
										//{ "gl_Vertex",			9 },	// VertexElementUsage_Position,
		{ "ln_Vertex",			9 },	// VertexElementUsage_Position,
		{ "ln_Normal",			9 },	// VertexElementUsage_Normal,
		{ "ln_Color",			8 },	// VertexElementUsage_Color,
		{ "ln_MultiTexCoord",	16 },	// VertexElementUsage_TexCoord,
		{ "ln_PointSize",		12 },	// VertexElementUsage_PointSize,
		{ "ln_BlendIndices",	15 },	// VertexElementUsage_BlendIndices,
		{ "ln_BlendWeight",		14 },	// VertexElementUsage_BlendWeight,
	};
	assert(LN_ARRAY_SIZE_OF(attrNameTable) == VertexElementUsage_Max);

	memset(m_usageAttrIndexTable, -1, sizeof(m_usageAttrIndexTable));

	// attribute 変数の数
	GLint attrCount;
	glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &attrCount);	LN_CHECK_GLERROR();
	for (int i = 0; i < attrCount; ++i)
	{
		GLsizei name_len = 0;
		GLsizei var_size = 0;
		GLenum  var_type = 0;
		GLchar  name[256] = { 0 };
		glGetActiveAttrib(m_program, i, 256, &name_len, &var_size, &var_type, name); LN_CHECK_GLERROR();

		for (int iUsage = 1; iUsage < VertexElementUsage_Max; ++iUsage)	// 0 番はダミーなので 1番から
		{
			// 変数名の一致確認 (UsageIndex の前の部分)
			if (strncmp(name, attrNameTable[iUsage].name, attrNameTable[iUsage].length) == 0)
			{
				// UsageIndex チェック (gl_MultiTexCoord0 とかの '0' の部分)
				size_t idxLen = name_len - attrNameTable[iUsage].length;
				const char* idxStr = &name[attrNameTable[iUsage].length];
				int usageIndex = -1;
				if (idxLen == 0) {
					usageIndex = 0;		// 省略されているので 0
				}
				else if (idxLen == 1 && idxStr[0] == '0') {
					usageIndex = 0;		// 1文字で '0' なので 0 (atoi でエラーを確認できるようにするため特別処理)
				}
				else if (idxLen <= 2) {
					usageIndex = atoi(idxStr);
				}

				if (0 <= usageIndex && usageIndex < GLShaderPass::MaxUsageIndex) {
					// TODO: error  不正 UsageIndex
				}

				// Location 取得
				int loc = glGetAttribLocation(m_program, name); LN_CHECK_GLERROR();
				m_usageAttrIndexTable[iUsage][usageIndex] = loc; LN_CHECK_GLERROR();

				break;
			}
		}
	}
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
