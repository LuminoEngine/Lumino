
#pragma once 
#include "../ShaderVariableBase.h"
#include "GLCommon.h"
#include "GLRenderer.h"

LN_NAMESPACE_BEGIN
struct LuminoShaderIRTechnique;
namespace Driver
{
class GLShader;
class GLShaderVariable;
class GLShaderAnnotation;
class GLShaderTechnique;
class GLShaderPass;

struct ShaderDiag
{
	ShaderCompileResultLevel	level;
	std::string					message;
};

/// Pass が利用するシェーダ変数と、その GLSL Location
struct GLShaderPassVariableInfo
{
	GLShaderVariable*	Variable;
	int					Location;
	int					TextureStageIndex;
};

class GLSLUtils
{
public:
	struct CodeRange
	{
		const char*	code;
		GLint		length;
	};

	static ShaderCompileResultLevel makeShaderProgram(const char* vsCode, size_t vsCodeLen, const char* fsCode, size_t fsCodeLen, GLuint* outProgram, std::string* outMessage);

	static void analyzeLNBasicShaderCode(const char* code, size_t codeLen, GLuint type, const char* entryName, CodeRange* outCode);

	// return: shader object
	static GLuint compileShader2(GLuint type, int codeCount, const char** codes, const GLint* sizes, ShaderDiag* diag);

	// return: shader object
	static GLuint compileShader3(GLuint type, const char* code, GLint codeLen, const char* entryName, ShaderDiag* diag);

	// return: Program object
	static GLuint linkShader(GLuint vertShader, GLuint fragShader, ShaderDiag* diag);
};

/// OpenGL 用の IShader の実装
class GLShader
	: public IShader
{
public:
	GLShader();
	virtual ~GLShader();
	void initialize(GLGraphicsDevice* device, const void* code, size_t codeByteCount);
	void initialize(GLGraphicsDevice* device, const std::vector<LuminoShaderIRTechnique>& techniques);

	GLGraphicsDevice* getGraphicsDevice() { return m_device; }
	GLShaderVariable* findShaderVariable(const String& name);
	GLShaderVariable* createShaderVariable(ShaderVariableBase::ShaderVariableTypeDesc desc, const String& name, const String& semanticName);

public:
	virtual int getVariableCount() const;
	virtual IShaderVariable* getVariable(int index) const;
	virtual int getTechniqueCount() const;
	virtual IShaderTechnique* getTechnique(int index) const;
	virtual void onLostDevice();
	virtual void onResetDevice();

	ShaderDiag* getDiag() { return &m_diag; }
	GLuint getVertexShader(const String& name);
	GLuint getFlagmentShader(const String& name);

private:
	GLuint compileShader(const char* code, size_t codeLen, const char* entryName, GLuint type, bool addPreproHeader);

	GLGraphicsDevice*				m_device;

	ShaderDiag					m_diag;

	List<GLShaderVariable*>	m_variables;
	List<GLShaderTechnique*>	m_techniques;
	std::map<String, GLuint>	m_glVertexShaderEntryMap;
	std::map<String, GLuint>	m_glPixelShaderEntryMap;
};

/// OpenGL 用の IShaderVariable の実装
class GLShaderVariable
	: public ShaderVariableBase
{
public:
	static GLShaderVariable* deserialize(GLShader* ownerShader, tr::JsonReader2* json, bool* outOverwrited);

	GLShaderVariable();
	virtual ~GLShaderVariable();
	void initialize(GLShader* owner, ShaderVariableTypeDesc desc, const String& name, const String& semanticName, GLint location);

public:
	/// 指定された GLSL Location に値を設定する
	void apply(int location, int textureStageIndex);

public:
	virtual int getAnnotationCount();
	virtual IShaderVariable* getAnnotation(int index);

public:
	static void convertVariableTypeGLToLN(const char* name, GLenum gl_type, GLsizei gl_var_size, ShaderVariableBase::ShaderVariableTypeDesc* desc);

private:
	GLShader*					m_ownerShader;
	GLint						m_glUniformLocation;
	//List<SamplerStatePair>		m_samplerStatus;
	List<GLShaderAnnotation*>	m_annotations;
};

// アノテーション
class GLShaderAnnotation
	: public ShaderVariableBase
{
public:
	static GLShaderAnnotation* deserialize(tr::JsonReader2* json);

	GLShaderAnnotation();
	virtual ~GLShaderAnnotation();
	void initialize(const String& type, const String& name, const String& value);

	virtual void setBool(bool value) { LN_UNREACHABLE(); }
	virtual void setInt(int value) { LN_UNREACHABLE(); }
	virtual void setFloat(float value) { LN_UNREACHABLE(); }
	virtual void setVector(const Vector4& vec) { LN_UNREACHABLE(); }
	virtual void setVectorArray(const Vector4* vectors, int count) { LN_UNREACHABLE(); }
	virtual void setMatrix(const Matrix& matrix) { LN_UNREACHABLE(); }
	virtual void setMatrixArray(const Matrix* matrices, int count) { LN_UNREACHABLE(); }
	virtual void setTexture(ITexture* texture) { LN_UNREACHABLE(); }
	virtual int getAnnotationCount() { return 0; }
	virtual IShaderVariable* getAnnotation(int index) { return NULL; }
};

// Technique
class GLShaderTechnique
	: public IShaderTechnique
{
public:
	static GLShaderTechnique* deserialize(GLShader* ownerShader, tr::JsonReader2* json);

	GLShaderTechnique();
	virtual ~GLShaderTechnique();
	void initialize(GLShader* ownerShader, const String& name);
	void addPass(GLShaderPass* pass) { m_passes.add(pass); }

public:
	virtual const Char* getName() const { return m_name.c_str(); }
	virtual int getPassCount() const { return m_passes.getCount(); }
	virtual IShaderPass* getPass(int index);
	virtual int getAnnotationCount() { return m_annotations.getCount(); }
	virtual IShaderVariable* getAnnotation(int index) { return m_annotations[index]; }

private:
	GLShader*					m_ownerShader;
	String						m_name;
	List<GLShaderPass*>		m_passes;
	List<GLShaderAnnotation*>	m_annotations;
};

/// OpenGL 用の IShaderPass の実装
class GLShaderPass
	: public IShaderPass
{
public:
	static const int MaxUsageIndex = 16;		///< UsageIndex の最大 (DX9 にあわせて最大の 16)

public:
	static GLShaderPass* deserialize(GLShader* ownerShader, tr::JsonReader2* json);

	GLShaderPass();
	virtual ~GLShaderPass();
	void initialize(GLShader* ownerShader, const String& name, GLuint vertShader, GLuint fragShader);
	void initialize(GLShader* ownerShader, const String& name, const String& vertShaderName, const String& fragShaderName);

public:
	/// glVertexAttribPointer() に指定する attribute インデックスを取得する (-1 の場合は存在しないことを示す)
	int getUsageAttributeIndex(VertexElementUsage usage, int index);

public:
	virtual IShader* getShader() const { return m_ownerShader; }
	virtual const Char* getName() const { return m_name.c_str(); }
	virtual int getAnnotationCount() { return m_annotations.getCount(); }
	virtual IShaderVariable* getAnnotation(int index) { return m_annotations[index]; }
	virtual void apply();
	//virtual void end();

private:
	bool linkShader(GLuint vertShader, GLuint fragShader, ShaderDiag* diag);
	void build();
	
	GLShader*						m_ownerShader;
	GLuint							m_program;
	String							m_name;
	List<GLShaderAnnotation*>		m_annotations;
	int8_t							m_usageAttrIndexTable[VertexElementUsage_Max][MaxUsageIndex];
	List<GLShaderPassVariableInfo>	m_passVarList;		// この Pass が本当に使う変数のリスト。最適化されていれば消えるものもある。
	//int								m_textureVarCount;
};

} // namespace Driver
LN_NAMESPACE_END
