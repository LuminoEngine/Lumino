
/* GLSL : よくあるミス
 * http://kumar.hateblo.jp/entry/2013/08/28/200708
 */
#include "../../../Internal.h"
#include <Lumino/Graphics/GraphicsException.h>
#include "GLGraphicsDevice.h"
#include "GLShader.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
//=============================================================================
// PlainGLSLBuilder
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderCompileResultLevel PlainGLSLBuilder::Build(GLGraphicsDevice* device, const void* code, size_t codeByteCount, GLShader** outShader, StringA* outMessage)
{
	GLuint program;
	ShaderCompileResultLevel resultLevel = MakeShaderProgram(code, codeByteCount, &program, outMessage);
	if (resultLevel == ShaderCompileResultLevel_Error) {
		return resultLevel;
	}

	RefPtr<GLShader> shader(LN_NEW GLShader(device, program), false);

	// uniform 変数の数を調べて、その数分 ShaderVariable 作成
	GLint params;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &params); LN_CHECK_GLERROR();

	Array<GLShaderPassVariableInfo> passVarList;
	for (int i = 0; i < params; ++i)
	{
		GLShaderPassVariableInfo passVar;

		GLsizei name_len = 0;
		GLsizei var_size = 0;   // 配列サイズっぽい
		GLenum  var_type = 0;
		GLchar  name[256] = { 0 };
		glGetActiveUniform(program, i, 256, &name_len, &var_size, &var_type, name); LN_CHECK_GLERROR();

		// 変数情報作成
		ShaderVariableBase::ShaderVariableTypeDesc desc;
		GLShaderVariable::ConvertVariableTypeGLToLN(var_type, var_size, &desc);

		// 名前を String 化
		String tname;
		tname.AssignCStr(name);

		// Location
		passVar.Location = glGetUniformLocation(program, name); LN_CHECK_GLERROR();

		// ShaderVariable を作る
		passVar.Variable = shader->TryCreateShaderVariable(desc, tname, String(), passVar.Location);	// TODO:この loc はいらない
	
		passVarList.Add(passVar);
	}

	// テクニック作成 (1対の頂点シェーダとフラグメントシェーダだけなので1つで良い)
	GLShaderTechnique* tech = shader->CreateShaderTechnique(_T("Main"));

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
		const char* Name;
		int			Length;
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

	int8_t usageAttrIndexTable[VertexElementUsage_Max][GLShaderPass::MaxUsageIndex];
	memset(usageAttrIndexTable, -1, sizeof(usageAttrIndexTable));

	// attribute 変数の数
	GLint attrCount;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &attrCount);	LN_CHECK_GLERROR();
	for (int i = 0; i < attrCount; ++i)
	{
		GLsizei name_len = 0;
		GLsizei var_size = 0;
		GLenum  var_type = 0;
		GLchar  name[256] = { 0 };
		glGetActiveAttrib(program, i, 256, &name_len, &var_size, &var_type, name); LN_CHECK_GLERROR();

		for (int iUsage = 1; iUsage < VertexElementUsage_Max; ++iUsage)	// 0 番はダミーなので 1番から
		{
			// 変数名の一致確認 (UsageIndex の前の部分)
			if (strncmp(name, attrNameTable[iUsage].Name, attrNameTable[iUsage].Length) == 0)
			{
				// UsageIndex チェック (gl_MultiTexCoord0 とかの '0' の部分)
				size_t idxLen = name_len - attrNameTable[iUsage].Length;
				const char* idxStr = &name[attrNameTable[iUsage].Length];
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
				int loc = glGetAttribLocation(program, name); LN_CHECK_GLERROR();
				usageAttrIndexTable[iUsage][usageIndex] = glGetAttribLocation(program, name); LN_CHECK_GLERROR();
				break;
			}
		}
	}

	// パス作成 (1対の頂点シェーダとフラグメントシェーダだけなので1つで良い)
	GLShaderPass* pass = tech->CreateShaderPass(_T("Main"), program, (int8_t*)usageAttrIndexTable, passVarList);

	shader.SafeAddRef();
	*outShader = shader;
	return resultLevel;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderCompileResultLevel PlainGLSLBuilder::MakeShaderProgram(const void* code, size_t codeByteCount, GLuint* outProgram, StringA* outMessage)
{
	*outProgram = NULL;
	outMessage->SetEmpty();

	// 頂点シェーダコード
	const char* vs_codes[] =
	{
		"#define LN_GLSL_VERTEX 1\n",
		(const char*)code,
	};
	GLint vs_sizes[] =
	{
		(GLint)strlen(vs_codes[0]),
		(GLint)codeByteCount,
	};

	// フラグメントシェーダコード
	const char* fs_codes[] =
	{
		"#define LN_GLSL_FRAGMENT 1\n",
		(const char*)code,
	};
	GLint fs_sizes[] =
	{
		(GLint)strlen(fs_codes[0]),
		(GLint)codeByteCount,
	};

	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
	GLuint program = 0;
	try
	{
		// シェーダオブジェクトの作成
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		LN_CHECK_GLERROR();
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		LN_CHECK_GLERROR();

		// シェーダオブジェクトのコンパイル
		bool vsResult = CompileShader(vertexShader, 2, vs_codes, vs_sizes, outMessage);
		bool fsResult = CompileShader(fragmentShader, 2, fs_codes, fs_sizes, outMessage);
		if (!vsResult || !fsResult)
		{
			// コンパイルエラー
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			return ShaderCompileResultLevel_Error;
		}

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
		if (outMessage->IsEmpty()) {
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool PlainGLSLBuilder::CompileShader(GLuint shader, int codeCount, const char** codes, const GLint* sizes, StringA* log)
{
	// シェーダオブジェクトにソースプログラムをセット
	glShaderSource(shader, codeCount, (const GLchar **)codes, sizes);
	//LN_CHECK_GLERROR();

	// シェーダのコンパイル
	glCompileShader(shader);
	//LN_CHECK_GLERROR();

	// 結果
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	//LN_CHECK_GLERROR();

	// ログがあるかチェック (ログの長さは、最後のNULL文字も含む)
	int logSize;
	int length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
	//LN_CHECK_GLERROR();
	if (logSize > 1)
	{
		char* buf = LN_NEW char[logSize];
		glGetShaderInfoLog(shader, logSize, &length, buf);
		(*log) += "Compile info:\n";
		(*log) += buf;
		LN_SAFE_DELETE_ARRAY(buf);
		LN_CHECK_GLERROR();
	}

	return (compiled != GL_FALSE);
}

//=============================================================================
// GLShader
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShader::GLShader(GLGraphicsDevice* device, GLuint program)
	: m_device(device)
	, m_glProgram(program)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShader::~GLShader()
{
	LN_FOREACH(GLShaderVariable* v, m_variables) {
		LN_SAFE_RELEASE(v);
	}
	LN_FOREACH(GLShaderTechnique* t, m_techniques) {
		LN_SAFE_RELEASE(t);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLShader::Initialize(GLGraphicsDevice* device, const void* code_, size_t codeByteCount)
{
	const char* code = (const char*)code_;
	if (memcmp(code, "#define LN_FXC_METADATA", 23) == 0)
	{
		int index = StringTraits::IndexOf(code, codeByteCount, "#endif", 6);
		LN_THROW(index >= 0, InvalidFormatException);
		StringReader reader(String::FromNativeCharString(code + 23, index - 23));
		JsonReader2 json(&reader);
		json.ReadAsStartObject();

		// vertexShaders
		if (json.ReadAsPropertyName() == "vertexShaders")
		{
			json.ReadAsStartArray();
			while (json.Read() && json.GetTokenType() != JsonToken::EndArray)
			{
				StringA entry = json.ReadAsString();
				m_glVertexShaderEntryMap[entry] = CompileShader(code, codeByteCount, entry.c_str(), GL_VERTEX_SHADER);
			}
		}
		// pixelShaders
		if (json.ReadAsPropertyName() == "pixelShaders")
		{
			json.ReadAsStartArray();
			while (json.Read() && json.GetTokenType() != JsonToken::EndArray)
			{
				StringA entry = json.ReadAsString();
				m_glPixelShaderEntryMap[entry] = CompileShader(code, codeByteCount, entry.c_str(), GL_FRAGMENT_SHADER);
			}
		}
		// techniques
		if (json.ReadAsPropertyName() == "techniques")
		{
			json.ReadAsStartArray();
			while (json.Read() && json.GetTokenType() != JsonToken::EndArray)
			{
				m_techniques.Add(GLShaderTechnique::Deserialize(&json));
			}
		}
		// techniques
		if (json.ReadAsPropertyName() == "parameters")
		{
			json.ReadAsStartArray();
			while (json.Read() && json.GetTokenType() != JsonToken::EndArray)
			{
				m_variables.Add(GLShaderVariable::Deserialize(&json));
			}
		}

		json.ReadAsEndObject();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void GLShader::Create()
//{
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShaderVariable* GLShader::TryCreateShaderVariable(ShaderVariableBase::ShaderVariableTypeDesc desc, const String& name, const String& semanticName, GLint location)
{
	// 同名・同セマンティックの変数があればそれを返す
	for (GLShaderVariable* v : m_variables) 
	{
		if (v->GetName() == name)
		{
			if (v->GetSemanticName() == semanticName) {
				return v;
			}
			else {
				// 同名変数なのにセマンティックが違う。なんかおかしい
				LN_THROW(0, InvalidOperationException);
			}
		}
	}

	// 新しく作る
	GLShaderVariable* v = LN_NEW GLShaderVariable(this, desc, name, semanticName, location);
	m_variables.Add(v);
	return v;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShaderTechnique* GLShader::CreateShaderTechnique(const String& name)
{
	GLShaderTechnique* tech = LN_NEW GLShaderTechnique(this, name);
	m_techniques.Add(tech);
	return tech;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int GLShader::GetVariableCount() const
{
	return m_variables.GetCount();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IShaderVariable* GLShader::GetVariable(int index) const
{
	return m_variables[index];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int GLShader::GetTechniqueCount() const
{
	return m_techniques.GetCount();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IShaderTechnique* GLShader::GetTechnique(int index) const
{
	return m_techniques[index];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLShader::OnLostDevice()
{
	LN_THROW(0, NotImplementedException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLShader::OnResetDevice()
{
	LN_THROW(0, NotImplementedException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLuint GLShader::CompileShader(const char* code, size_t codeLen, const char* entryName, GLuint type)
{
	StringA define;
	if (type == GL_VERTEX_SHADER)
		define = StringA::Format("#define LN_GLSL_VERT_{0} 1\n", entryName);
	else if (type == GL_FRAGMENT_SHADER)
		define = StringA::Format("#define LN_GLSL_FLAG_{0} 1\n", entryName);

	const char* codes[] =
	{
		define.c_str(),
		(const char*)code,
	};
	GLint sizes[] =
	{
		(GLint)define.GetLength(),
		(GLint)codeLen,
	};

	GLuint shader = glCreateShader(type);
	if (!PlainGLSLBuilder::CompileShader(shader, 2, codes, sizes, &m_lastMessage))
	{
		glDeleteShader(shader);
		LN_THROW(0, InvalidFormatException);
	}
	return shader;
}

//=============================================================================
// GLShaderVariable
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShaderVariable* GLShaderVariable::Deserialize(JsonReader2* json)
{
	auto var = RefPtr<GLShaderVariable>::MakeRef();
	String name, semantic, samplerName;	// TODO: この辺、動的なメモリ確保を少なくしたいが・・・
	bool shared;
	if (json->ReadAsPropertyName() == _T("name")) name = json->ReadAsString();

	while (true)
	{
		const String& prop = json->ReadAsPropertyName();
		if (prop == _T("semantic")) semantic = json->ReadAsString();
		else if (prop == _T("samplerName")) samplerName = json->ReadAsString();
		else if (prop == _T("shared")) shared = json->ReadAsBool();
		else if (prop == _T("samplerStatus"))
		{

		}
		else if (prop == _T("annotations"))
		{
			json->ReadAsStartArray();
			while (json->Read() && json->GetTokenType() != JsonToken::EndArray)
			{
				var->m_annotations.Add(GLShaderAnnotation::Deserialize(json));
			}
		}
	}

	json->ReadAsEndObject();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShaderVariable::GLShaderVariable(GLShader* owner, ShaderVariableTypeDesc desc, const String& name, const String& semanticName, GLint location)
	: m_ownerShader(owner)
	, m_glUniformLocation(location)
{
	Initialize(desc, name, semanticName);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShaderVariable::~GLShaderVariable()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLShaderVariable::Apply(int location, int textureStageIndex)
{
	if (!m_value.IsValid()) {
		return;		// 一度も set されていない変数や、ライブラリが認識できない型である。その場合は何も処理をせず、デフォルト値のままにする。
	}

	const Vector4* vec;

	switch (m_desc.Type)
	{
	case ShaderVariableType_Bool:
		glUniform1i(location, m_value.GetBool());
		break;
	case ShaderVariableType_Int:
		glUniform1i(location, m_value.GetInt());
		break;
	case ShaderVariableType_Float:
		glUniform1f(location, m_value.GetFloat());
		break;
	case ShaderVariableType_Vector:
		vec = &m_value.GetVector();
		if (m_desc.Columns == 2) {
			glUniform2f(location, vec->x, vec->y);
		}
		else if (m_desc.Columns == 3) {
			glUniform3f(location, vec->x, vec->y, vec->z);
		}
		else if (m_desc.Columns == 4) {
			glUniform4f(location, vec->x, vec->y, vec->z, vec->w);
		}
		break;
	case ShaderVariableType_VectorArray:
		glUniform4fv(
			location, m_desc.Elements,
			(const GLfloat*)m_value.GetVectorArray());
		break;
	case ShaderVariableType_Matrix:
		glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat*)&m_value.GetMatrix());
		break;
	case ShaderVariableType_MatrixArray:
		glUniformMatrix4fv(
			location, m_desc.Elements, GL_TRUE,		// TODO: 転置でいいか確認
			(const GLfloat*)m_value.GetMatrixArray());
		break;
	case ShaderVariableType_DeviceTexture:
		// textureStageIndex のテクスチャステージにバインド
		glActiveTexture(GL_TEXTURE0 + textureStageIndex);
		glBindTexture(GL_TEXTURE_2D, static_cast<GLTextureBase*>(m_value.GetDeviceTexture())->GetGLTexture());

		//LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mSamplerState->MinFilter);
		//LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mSamplerState->MagFilter);
		//LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mSamplerState->AddressU);
		//LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mSamplerState->AddressV);

		// テクスチャステージ番号をセット
		glUniform1i(location, textureStageIndex);
		break;
	case ShaderVariableType_String:
		// GLSL に String 型は無い。この値は本ライブラリで使用しているただのメタデータ。
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int GLShaderVariable::GetAnnotationCount()
{
	LN_THROW(0, NotImplementedException);
	return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IShaderVariable* GLShaderVariable::GetAnnotation(int index)
{
	LN_THROW(0, NotImplementedException);
	return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLShaderVariable::ConvertVariableTypeGLToLN(GLenum gl_type, GLsizei gl_var_size, ShaderVariableBase::ShaderVariableTypeDesc* desc)
{
	desc->Elements = gl_var_size;
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
}

//=============================================================================
// GLShaderAnnotation
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShaderAnnotation* GLShaderAnnotation::Deserialize(JsonReader2* json)
{
	auto anno = RefPtr<GLShaderAnnotation>::MakeRef();
	String type, name, value;
	if (json->ReadAsPropertyName() == _T("type")) type = json->ReadAsString();
	else if (json->ReadAsPropertyName() == _T("name")) name = json->ReadAsString();
	else if (json->ReadAsPropertyName() == _T("value")) value = json->ReadAsString();
	anno->Initialize(type, name, value);
	return anno.DetachMove();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShaderAnnotation::GLShaderAnnotation()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShaderAnnotation::~GLShaderAnnotation()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLShaderAnnotation::Initialize(const String& type, const String& name, const String& value)
{
	ShaderVariableTypeDesc desc;
	desc.Type = ShaderVariableType_Unknown;
	desc.Columns = 0;
	desc.Rows = 0;			// 行列はサポートしていない
	desc.Elements = 0;		// 配列はサポートしていない
	desc.Shared = false;	// annotation に shared はつかない

	if (type.IndexOf(_T("bool")) == 0)
	{
		desc.Type = ShaderVariableType_Bool;
		ShaderVariableBase::Initialize(desc, name, String::GetEmpty());
		ShaderVariableBase::SetBool(value[0] == 't');	// "true"?
	}
	else if (type.IndexOf(_T("int")) == 0)
	{
		desc.Type = ShaderVariableType_Int;
		ShaderVariableBase::Initialize(desc, name, String::GetEmpty());
		ShaderVariableBase::SetInt(value.ToInt32());
	}
	else if (type.IndexOf(_T("string")) == 0)
	{
		desc.Type = ShaderVariableType_Int;
		ShaderVariableBase::Initialize(desc, name, String::GetEmpty());
		ShaderVariableBase::SetString(value.c_str());
	}
	else if (type.IndexOf(_T("float")) == 0)
	{
		if (type.GetLength() == 5)
		{
			desc.Type = ShaderVariableType_Float;
			ShaderVariableBase::Initialize(desc, name, String::GetEmpty());
			ShaderVariableBase::SetFloat(StringTraits::ToDouble(value.c_str(), value.GetLength()));

		}
		else if (type.GetLength() == 6)
		{
			// vector
			desc.Columns = (type[6] - '0');
			desc.Type = ShaderVariableType_Vector;
			ShaderVariableBase::Initialize(desc, name, String::GetEmpty());
			ShaderVariableBase::SetFloat(StringTraits::ToDouble(value.c_str(), value.GetLength()));

		}
		else
		{
			// 行列はサポートしていない
			LN_THROW(0, InvalidFormatException);
		}
	}
}

//=============================================================================
// GLShaderTechnique
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShaderTechnique* GLShaderTechnique::Deserialize(JsonReader2* json)
{
	auto tech = RefPtr<GLShaderTechnique>::MakeRef();

	if (json->ReadAsPropertyName() == _T("name"))
	{
		tech->m_name = json->ReadAsString();
	}
	if (json->ReadAsPropertyName() == _T("passes"))
	{
		json->ReadAsStartArray();
		while (json->Read() && json->GetTokenType() != JsonToken::EndArray)
		{
			tech->m_passes.Add(GLShaderPass::Deserialize(&json));
		}
	}
	if (json->ReadAsPropertyName() == _T("annotations"))
	{
		json->ReadAsStartArray();
		while (json->Read() && json->GetTokenType() != JsonToken::EndArray)
		{
			tech->m_annotations.Add(GLShaderAnnotation::Deserialize(&json));
		}
	}
	json->ReadAsEndObject();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShaderTechnique::GLShaderTechnique(GLShader* owner, const String& name)
	: m_ownerShader(owner)
	, m_name(name)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShaderTechnique::~GLShaderTechnique()
{
	LN_FOREACH(GLShaderPass* pass, m_passes) {
		LN_SAFE_RELEASE(pass);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShaderPass* GLShaderTechnique::CreateShaderPass(const String& name, GLuint program, int8_t* attrIndexTable, const Array<GLShaderPassVariableInfo>& passVarList)
{
	GLShaderPass* pass = LN_NEW GLShaderPass(m_ownerShader, name, program, attrIndexTable, passVarList);
	m_passes.Add(pass);
	return pass;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IShaderPass* GLShaderTechnique::GetPass(int index)
{
	return m_passes[index];
}

//=============================================================================
// GLShaderPass
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShaderPass* GLShaderPass::Deserialize(JsonReader2* json)
{
	auto pass = RefPtr<GLShaderPass>::MakeRef();
	String vsName, psName;

	if (json->ReadAsPropertyName() == _T("name"))
	{
		pass->m_name = json->ReadAsString();
	}
	if (json->ReadAsPropertyName() == _T("vertexShader"))
	{
		vsName = json->ReadAsString();
	}
	if (json->ReadAsPropertyName() == _T("pixelShader"))
	{
		psName = json->ReadAsString();
	}
	pass->LinkShader(vsName, psName);


	if (json->ReadAsPropertyName() == _T("passes"))
	{
		json->ReadAsStartArray();
		while (json->Read() && json->GetTokenType() != JsonToken::EndArray)
		{
			// TODO:
		}
	}
	if (json->ReadAsPropertyName() == _T("annotations"))
	{
		json->ReadAsStartArray();
		while (json->Read() && json->GetTokenType() != JsonToken::EndArray)
		{
			pass->m_annotations.Add(GLShaderAnnotation::Deserialize(&json));
		}
	}
	json->ReadAsEndObject();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShaderPass::GLShaderPass(GLShader* owner, const String& name, GLuint program, int8_t* attrIndexTable, const Array<GLShaderPassVariableInfo>& passVarList)
	: m_ownerShader(owner)
	, m_program(program)
	, m_name(name)
	, m_passVarList(passVarList)
	, m_textureVarCount(0)
{
	memcpy_s(m_usageAttrIndexTable, sizeof(m_usageAttrIndexTable), attrIndexTable, sizeof(m_usageAttrIndexTable));

	// テクスチャ型の変数にステージ番号を振っていく。
	LN_FOREACH(GLShaderPassVariableInfo& v, m_passVarList)
	{
		if (v.Variable->GetType() == ShaderVariableType_DeviceTexture) {
			v.TextureStageIndex = m_textureVarCount;
			m_textureVarCount++;
		}
		else {
			v.TextureStageIndex = -1;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLShaderPass::~GLShaderPass()
{
	if (m_program != 0)
	{
		glDeleteProgram(m_program);
		m_program = 0;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int GLShaderPass::GetUsageAttributeIndex(VertexElementUsage usage, int index)
{
	return m_usageAttrIndexTable[usage][index];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLShaderPass::Apply()
{
	glUseProgram(m_program); LN_CHECK_GLERROR();

	LN_FOREACH(GLShaderPassVariableInfo& v, m_passVarList)
	{
		v.Variable->Apply(v.Location, v.TextureStageIndex);
	}

	static_cast<GLRenderer*>(m_ownerShader->GetGraphicsDevice()->GetRenderer())->SetCurrentShaderPass(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void GLShaderPass::End()
//{
//	glUseProgram(NULL);
//	static_cast<GLRenderer*>(m_ownerShader->GetGraphicsDevice()->GetRenderer())->SetCurrentShaderPass(NULL);
//}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
