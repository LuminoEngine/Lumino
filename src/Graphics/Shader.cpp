
#pragma once

#include "../Internal.h"
#include "../../include/Lumino/Graphics/Shader.h"
#include "../../include/Lumino/Graphics/GraphicsManager.h"
#include "../../include/Lumino/Graphics/Renderer.h"
#include "../../include/Lumino/Graphics/Texture.h"
#include "RenderingCommand.h"
#include "GraphicsHelper.h"


#define LN_CALL_SHADER_COMMAND(func, command, ...) \
	if (m_owner->GetManager()->GetRenderingType() == RenderingType::Deferred) { \
		m_owner->GetManager()->GetRenderer()->m_primaryCommandList->AddCommand<command>(m_deviceObj, __VA_ARGS__); \
	} \
	else { \
		m_deviceObj->func(__VA_ARGS__); \
	}


namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// Shader
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Shader* Shader::Create(const char* code, int length)
{
	LN_THROW(Internal::Manager != NULL, ArgumentException);

	ShaderCompileResult result;
	RefPtr<Device::IShader> deviceObj(
		Internal::Manager->GetGraphicsDevice()->CreateShader(code, length, &result));

	LN_THROW(!deviceObj.IsNull(), CompilationException, result);
	return LN_NEW Shader(Internal::Manager, deviceObj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Shader* Shader::Create(GraphicsManager* manager, const void* textData, size_t byteCount)
{
	LN_THROW(manager != NULL, ArgumentException);
	ShaderCompileResult result;
	RefPtr<Device::IShader> deviceObj(
		manager->GetGraphicsDevice()->CreateShader(textData, byteCount, &result));
	LN_THROW(!deviceObj.IsNull(), CompilationException, result);
	return LN_NEW Shader(manager, deviceObj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Shader::TryCreate(GraphicsManager* manager, const void* textData, size_t byteCount, Shader** outShader, ShaderCompileResult* outResult)
{
	LN_THROW(manager != NULL, ArgumentException);
	LN_THROW(outShader != NULL, ArgumentException);

	*outShader = NULL;
	RefPtr<Device::IShader> deviceObj(
		manager->GetGraphicsDevice()->CreateShader(textData, byteCount, outResult));
	if (deviceObj.IsNull()) {
		return false;
	}

	*outShader = LN_NEW Shader(manager, deviceObj);
	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Shader::Shader(GraphicsManager* manager, Device::IShader* shader)
	: m_manager(manager)
	, m_deviceObj(shader)
{
	LN_SAFE_ADDREF(m_deviceObj);

	// 変数を展開
	for (int i = 0; i < shader->GetVariableCount(); ++i)
	{
		m_variables.Add(LN_NEW ShaderVariable(this, shader->GetVariable(i)));
	}

	// テクニックを展開
	for (int i = 0; i < shader->GetTechniqueCount(); ++i)
	{
		m_techniques.Add(LN_NEW ShaderTechnique(this, shader->GetTechnique(i)));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Shader::~Shader()
{
	LN_FOREACH(ShaderVariable* var, m_variables) {
		LN_SAFE_RELEASE(var);
	}
	LN_FOREACH(ShaderTechnique* tech, m_techniques) {
		LN_SAFE_RELEASE(tech);
	}

	LN_SAFE_RELEASE(m_deviceObj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderVariable* Shader::FindVariable(const TCHAR* name, CaseSensitivity cs) const
{
	LN_FOREACH(ShaderVariable* var, m_variables) {
		if (var->GetName().Compare(name, -1, cs) == 0) {
			return var;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Array<ShaderTechnique*>& Shader::GetTechniques() const
{
	return m_techniques;
}

//=============================================================================
// ShaderValue
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderValue::ShaderValue()
{
	m_type = ShaderVariableType_Unknown;
	memset(&m_value, 0, sizeof(m_value));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderValue::~ShaderValue()
{
	ReleaseValueBuffer();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderValue::ShaderValue(const ShaderValue& obj)
{
	memset(&m_value, 0, sizeof(m_value));
	Copy(obj);
}
ShaderValue& ShaderValue::operator = (const ShaderValue& obj)
{
	memset(&m_value, 0, sizeof(m_value));
	Copy(obj);
	return (*this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderValue::SetBool(bool value)
{
	m_type = ShaderVariableType_Bool;
	m_value.BoolVal = value;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderValue::SetInt(int value)
{
	m_type = ShaderVariableType_Int;
	m_value.Int = value;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderValue::SetFloat(float value)
{
	m_type = ShaderVariableType_Float;
	m_value.Float = value;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderValue::SetVector(const Vector4& vec)
{
	m_type = ShaderVariableType_Vector;
	AllocValueBuffer(sizeof(Vector4));
	*m_value.Vector = vec;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderValue::SetVectorArray(const Vector4* vectors, int count)
{
	m_type = ShaderVariableType_VectorArray;
	AllocValueBuffer(sizeof(Vector4) * count);
	if (vectors != NULL) {
		memcpy(m_value.VectorArray, vectors, sizeof(Vector4) * count);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderValue::SetMatrix(const Matrix& matrix)
{
	m_type = ShaderVariableType_Matrix;
	AllocValueBuffer(sizeof(Matrix));
	*m_value.Matrix = matrix;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderValue::SetMatrixArray(const Matrix* matrices, int count)
{
	m_type = ShaderVariableType_MatrixArray;
	AllocValueBuffer(sizeof(Matrix) * count);
	if (matrices != NULL) {
		memcpy(m_value.MatrixArray, matrices, sizeof(Matrix) * count);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderValue::SetTexture(Device::ITexture* texture)
{
	m_type = ShaderVariableType_Texture;
	LN_REFOBJ_SET(m_value.Texture, texture);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderValue::SetString(const char* str)
{
	m_type = ShaderVariableType_String;
	String s;
	s.AssignCStr(str);
	AllocValueBuffer(s.GetByteCount());
	memcpy(m_value.String, s.GetCStr(), s.GetByteCount());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int ShaderValue::GetArrayLength()
{
	if (m_type == ShaderVariableType_VectorArray) {
		//return m_value.ByteCount / sizeof(Vector4);
		return m_buffer.GetSize() / sizeof(Vector4);
	}
	if (m_type == ShaderVariableType_MatrixArray) {
		//return m_value.ByteCount / sizeof(Matrix);
		return m_buffer.GetSize() / sizeof(Matrix);
	}
	return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderValue::ReleaseValueBuffer()
{
	/*if (m_value.ByteCount > 0) {
		LN_SAFE_DELETE_ARRAY(m_value.Buffer);
	}
	else */
	if (m_type == ShaderVariableType_Texture) {
		LN_SAFE_RELEASE(m_value.Texture);
	}
	//m_value.ByteCount = 0;
	m_buffer.Release();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderValue::AllocValueBuffer(size_t byteCount)
{
	if (byteCount > m_buffer.GetSize()/*m_value.ByteCount*//* || m_buffer.GetRefCount() != 1*/)
	{
		//LN_SAFE_DELETE_ARRAY(m_value.Buffer);
		//m_value.Buffer = LN_NEW byte_t[byteCount];
		//m_value.ByteCount = byteCount;
		//m_buffer.Attach(LN_NEW ByteBuffer(byteCount), false);
		m_buffer.Resize(byteCount);
		m_value.Buffer = m_buffer.GetData();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderValue::Copy(const ShaderValue& value)
{
	m_type = value.m_type;
	m_buffer = value.m_buffer;	// 共有参照

	//if (m_buffer.IsNull()) {
	//	memcpy(&m_value, &value.m_value, sizeof(m_value));
	//}
	//else {
	//	m_value.Buffer = m_buffer->GetData();
	//}
	m_value.Buffer = m_buffer.GetData();

	if (m_type == ShaderVariableType_Texture) {
		m_value.Texture = value.m_value.Texture;
		LN_SAFE_ADDREF(m_value.Texture);
	}

}

//=============================================================================
// ShaderVariable
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderVariable::ShaderVariable(Shader* owner, Device::IShaderVariable* deviceObj)
	: m_owner(owner)
	, m_deviceObj(deviceObj)
	, m_textureValue(NULL)
{
	// 初期値として保持しておく
	m_value = deviceObj->GetValue();

	// アノテーションの展開
	for (int i = 0; i < m_deviceObj->GetAnnotationCount(); ++i) {
		m_annotations.Add(LN_NEW ShaderVariable(m_owner, m_deviceObj->GetAnnotation(i)));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderVariable::~ShaderVariable()
{
	LN_FOREACH(ShaderVariable* anno, m_annotations) {
		anno->Release();
	}
	LN_SAFE_RELEASE(m_textureValue);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderVariableType ShaderVariable::GetType() const
{
	return m_deviceObj->GetType();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const String& ShaderVariable::GetName() const
{
	return m_deviceObj->GetName();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const String& ShaderVariable::GetSemanticName() const
{
	return m_deviceObj->GetSemanticName();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int ShaderVariable::GetRows() const
{
	return m_deviceObj->GetMatrixRows();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int ShaderVariable::GetColumns() const
{
	return m_deviceObj->GetMatrixColumns();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int ShaderVariable::GetArrayElements() const
{
	return m_deviceObj->GetArrayElements();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariable::SetBool(bool value)
{
	m_value.SetBool(value);
	LN_CALL_SHADER_COMMAND(SetBool, SetShaderVariableCommand, value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ShaderVariable::GetBool() const
{
	return m_value.GetBool();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariable::SetInt(int value)
{
	m_value.SetInt(value);
	LN_CALL_SHADER_COMMAND(SetInt, SetShaderVariableCommand, value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int ShaderVariable::GetInt() const
{
	return m_value.GetInt();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariable::SetFloat(float value)
{
	m_value.SetFloat(value);
	LN_CALL_SHADER_COMMAND(SetFloat, SetShaderVariableCommand, value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float ShaderVariable::GetFloat() const
{
	return m_value.GetFloat();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariable::SetVector(const Vector4& value)
{
	m_value.SetVector(value);
	LN_CALL_SHADER_COMMAND(SetVectorArray, SetShaderVariableCommand, &value, 1);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Vector4& ShaderVariable::GetVector() const
{
	return m_value.GetVector();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariable::SetVectorArray(const Vector4* values, int count)
{
	m_value.SetVectorArray(values, count);
	LN_CALL_SHADER_COMMAND(SetVectorArray, SetShaderVariableCommand, values, count);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Vector4* ShaderVariable::GetVectorArray() const
{
	return m_value.GetVectorArray();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariable::SetMatrix(const Matrix& value)
{
	m_value.SetMatrix(value);
	LN_CALL_SHADER_COMMAND(SetMatrixArray, SetShaderVariableCommand, &value, 1);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Matrix& ShaderVariable::GetMatrix() const
{
	return m_value.GetMatrix();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariable::SetMatrixArray(const Matrix* values, int count)
{
	m_value.SetMatrixArray(values, count);
	LN_CALL_SHADER_COMMAND(SetMatrixArray, SetShaderVariableCommand, values, count);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Matrix* ShaderVariable::GetMatrixArray() const
{
	return m_value.GetMatrixArray();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariable::SetTexture(Texture* texture)
{
	Device::ITexture* t = (texture != NULL) ? Helper::GetDeviceObject(texture) : NULL;
	LN_REFOBJ_SET(m_textureValue, texture);
	LN_CALL_SHADER_COMMAND(SetTexture, SetShaderVariableCommand, t);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* ShaderVariable::GetTexture() const
{ 
	return m_textureValue;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariable::SetString(const char* str)
{
	m_value.SetString(str);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const TCHAR* ShaderVariable::GetString() const
{
	return m_value.GetString();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Array<ShaderVariable*>& ShaderVariable::GetAnnotations() const
{
	return m_annotations;
}

//=============================================================================
// ShaderTechnique
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderTechnique::ShaderTechnique(Shader* owner, Device::IShaderTechnique* deviceObj)
	: m_owner(owner)
	, m_deviceObj(deviceObj)
{
	// パスの展開
	for (int i = 0; i < m_deviceObj->GetPassCount(); ++i) {
		m_passes.Add(LN_NEW ShaderPass(m_owner, m_deviceObj->GetPass(i)));
	}

	// アノテーションの展開
	for (int i = 0; i < m_deviceObj->GetAnnotationCount(); ++i) {
		m_annotations.Add(LN_NEW ShaderVariable(m_owner, m_deviceObj->GetAnnotation(i)));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderTechnique::~ShaderTechnique()
{
	LN_FOREACH(ShaderPass* pass, m_passes) {
		LN_SAFE_RELEASE(pass);
	}
	LN_FOREACH(ShaderVariable* anno, m_annotations) {
		LN_SAFE_RELEASE(anno);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Array<ShaderPass*>& ShaderTechnique::GetPasses() const
{
	return m_passes;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Array<ShaderVariable*>& ShaderTechnique::GetAnnotations() const
{
	return m_annotations;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderVariable* ShaderTechnique::FindAnnotation(const TCHAR* name, CaseSensitivity cs) const
{
	LN_FOREACH(ShaderVariable* anno, m_annotations) {
		if (anno->GetName().Compare(name, -1, cs) == 0) {
			return anno;
		}
	}
	return NULL;
}

//=============================================================================
// ShaderPass
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderPass::ShaderPass(Shader* owner, Device::IShaderPass* deviceObj)
	: m_owner(owner)
	, m_deviceObj(deviceObj)
	, m_name(m_deviceObj->GetName())
{
	// アノテーションの展開
	for (int i = 0; i < m_deviceObj->GetAnnotationCount(); ++i) {
		m_annotations.Add(LN_NEW ShaderVariable(m_owner, m_deviceObj->GetAnnotation(i)));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderPass::~ShaderPass()
{
	LN_FOREACH(ShaderVariable* anno, m_annotations) {
		LN_SAFE_RELEASE(anno);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const String& ShaderPass::GetName() const
{
	return m_name;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderPass::Apply()
{
	LN_CALL_SHADER_COMMAND(Apply, ApplyShaderPassCommand);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Array<ShaderVariable*>& ShaderPass::GetAnnotations() const
{
	return m_annotations;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderVariable* ShaderPass::FindAnnotation(const TCHAR* name, CaseSensitivity cs) const
{
	LN_FOREACH(ShaderVariable* anno, m_annotations) {
		if (anno->GetName().Compare(name, -1, cs) == 0) {
			return anno;
		}
	}
	return NULL;
}

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
