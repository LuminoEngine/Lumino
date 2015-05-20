
#pragma once 

#include "../DeviceInterface.h"
#include "../../../../include/Lumino/Graphics/GraphicsDevice.h"
#include "../ShaderVariableBase.h"
#include "GLRenderer.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{
class GLShader;
class GLShaderVariable;
class GLShaderTechnique;
class GLShaderPass;

/// Pass が利用するシェーダ変数と、その GLSL Location
struct GLShaderPassVariableInfo
{
	GLShaderVariable*	Variable;
	int					Location;
	int					TextureStageIndex;
};

/// GLSL コードをコンパイルし、GLShader を作成するクラス
class PlainGLSLBuilder
{
public:
	static ShaderCompileResultLevel Build(GLGraphicsDevice* device, const void* code, size_t codeByteCount, GLShader** outShader, StringA* outMessage);

	static ShaderCompileResultLevel MakeShaderProgram(const void* code, size_t codeByteCount, GLuint* outProgram, StringA* outMessage);
	static bool CompileShader(GLuint shader, int codeCount, const char** codes, const GLint* sizes, StringA* log);
};

/// OpenGL 用の IShader の実装
class GLShader
	: public IShader
{
public:
	GLShader(GLGraphicsDevice* device, GLuint program);
	virtual ~GLShader();

public:
	//void Create();

	GLGraphicsDevice* GetGraphicsDevice() { return m_device; }
	GLuint GetGLProgram() { return m_glProgram; }
	GLShaderVariable* TryCreateShaderVariable(ShaderVariableBase::ShaderVariableTypeDesc desc, const String& name, const String& semanticName, GLint location);
	GLShaderTechnique* CreateShaderTechnique(const String& name);

public:
	virtual int GetVariableCount() const;
	virtual IShaderVariable* GetVariable(int index) const;
	virtual int GetTechniqueCount() const;
	virtual IShaderTechnique* GetTechnique(int index) const;
	virtual void OnLostDevice();
	virtual void OnResetDevice();

private:
	GLGraphicsDevice*				m_device;
	GLuint							m_glProgram;
	ArrayList<GLShaderVariable*>	m_variables;
	ArrayList<GLShaderTechnique*>	m_techniques;
};

/// OpenGL 用の IShaderVariable の実装
class GLShaderVariable
	: public ShaderVariableBase
{
public:
	GLShaderVariable(GLShader* owner, ShaderVariableTypeDesc desc, const String& name, const String& semanticName, GLint location);
	virtual ~GLShaderVariable();

public:
	/// 指定された GLSL Location に値を設定する
	void Apply(int location, int textureStageIndex);

public:
	virtual int GetAnnotationCount();
	virtual IShaderVariable* GetAnnotation(int index);

public:
	static void ConvertVariableTypeGLToLN(GLenum gl_type, GLsizei gl_var_size, ShaderVariableBase::ShaderVariableTypeDesc* desc);

private:
	GLShader*	m_ownerShader;
	GLint		m_glUniformLocation;
};

/// OpenGL 用の GLShaderTechnique の実装
class GLShaderTechnique
	: public IShaderTechnique
{
public:
	GLShaderTechnique(GLShader* owner, const String& name);
	virtual ~GLShaderTechnique();

public:
	GLShaderPass* CreateShaderPass(const String& name, GLuint program, int8_t* attrIndexTable, const ArrayList<GLShaderPassVariableInfo>& passVarList);

public:
	virtual const TCHAR* GetName() const { return m_name.GetCStr(); }
	virtual int GetPassCount() const { return m_passes.GetCount(); }
	virtual IShaderPass* GetPass(int index);
	virtual int GetAnnotationCount() { return m_annotations.GetCount(); }
	virtual IShaderVariable* GetAnnotation(int index) { return m_annotations[index]; }

private:
	GLShader*						m_ownerShader;
	String							m_name;
	ArrayList<GLShaderPass*>		m_passes;
	ArrayList<GLShaderVariable*>	m_annotations;
};

/// OpenGL 用の IShaderPass の実装
class GLShaderPass
	: public IShaderPass
{
public:
	static const int MaxUsageIndex = 16;		///< UsageIndex の最大 (DX9 にあわせて最大の 16)

public:
	GLShaderPass(GLShader* owner, const String& name, GLuint program, int8_t* attrIndexTable, const ArrayList<GLShaderPassVariableInfo>& passVarList);
	virtual ~GLShaderPass();

public:
	/// glVertexAttribPointer() に指定する attribute インデックスを取得する (-1 の場合は存在しないことを示す)
	int GetUsageAttributeIndex(VertexElementUsage usage, int index);

public:
	virtual const TCHAR* GetName() const { return m_name.GetCStr(); }
	virtual int GetAnnotationCount() { return m_annotations.GetCount(); }
	virtual IShaderVariable* GetAnnotation(int index) { return m_annotations[index]; }
	virtual void Apply();
	//virtual void End();

private:
	GLShader*						m_ownerShader;
	GLuint							m_program;
	String							m_name;
	ArrayList<GLShaderVariable*>	m_annotations;
	int8_t							m_usageAttrIndexTable[VertexElementUsage_Max][MaxUsageIndex];
	ArrayList<GLShaderPassVariableInfo>	m_passVarList;		///< この Pass が本当に使う変数のリスト。最適化されていれば消えるものもある。
	int									m_textureVarCount;
};

} // namespace Device
} // namespace Graphics
} // namespace Lumino
