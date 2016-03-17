
#include "Internal.h"
#include <Lumino/Graphics/GraphicsException.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Texture.h>
#include "RendererImpl.h"
#include "RenderingCommand.h"
#include "GraphicsHelper.h"


#define LN_CALL_SHADER_COMMAND(func, command, ...) \
	if (m_owner->GetManager()->GetRenderingType() == RenderingType::Deferred) { \
		m_owner->GetManager()->GetRenderer()->m_primaryCommandList->AddCommand<command>(m_deviceObj, __VA_ARGS__); \
	} \
	else { \
		m_deviceObj->func(__VA_ARGS__); \
	}


LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// Shader
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RefPtr<Shader> Shader::Create(const StringRef& filePath)
{
	RefPtr<Shader> obj(LN_NEW Shader(), false);
	obj->Initialize(detail::GetGraphicsManager(nullptr), filePath);
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RefPtr<Shader> Shader::Create(const char* code, int length)
{
	RefPtr<Shader> obj(LN_NEW Shader(), false);
	obj->Initialize(detail::GetGraphicsManager(nullptr), code, length);
	return obj;
}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//Shader* Shader::Create(GraphicsManager* manager, const void* textData, size_t byteCount)
//{
//	LN_THROW(manager != NULL, ArgumentException);
//	ShaderCompileResult result;
//	RefPtr<Driver::IShader> deviceObj(
//		manager->GetGraphicsDevice()->CreateShader(textData, byteCount, &result), false);
//	LN_THROW(!deviceObj.IsNull(), CompilationException, result);
//	return LN_NEW Shader(manager, deviceObj, ByteBuffer(textData, byteCount));
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//bool Shader::TryCreate(GraphicsManager* manager, const void* textData, size_t byteCount, Shader** outShader, ShaderCompileResult* outResult)
//{
//	LN_THROW(manager != NULL, ArgumentException);
//	LN_THROW(outShader != NULL, ArgumentException);
//
//	*outShader = NULL;
//	RefPtr<Driver::IShader> deviceObj(
//		manager->GetGraphicsDevice()->CreateShader(textData, byteCount, outResult), false);
//	if (deviceObj.IsNull()) {
//		return false;
//	}
//
//	*outShader = LN_NEW Shader(manager, deviceObj, ByteBuffer(textData, byteCount));
//	return true;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Shader::Shader()
	: m_deviceObj(nullptr)
	, m_sourceCode()
	, m_viewportPixelSize(nullptr)
	, m_modifiedVariables(true)
{
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
void Shader::Initialize(GraphicsManager* manager, const StringRef& filePath)
{
	GraphicsResourceObject::Initialize(manager);
	
	RefPtr<Stream> stream(manager->GetFileManager()->CreateFileStream(filePath), false);
	ByteBuffer buf(stream->GetLength() + 1, false);
	stream->Read(buf.GetData(), buf.GetSize());
	buf[stream->GetLength()] = 0x00;

	// TODO: 最後には改行を入れておく。環境によっては改行がないとエラーになる。しかもエラーなのにエラー文字列が出ないこともある。

	ShaderCompileResult result;
	m_deviceObj = m_manager->GetGraphicsDevice()->CreateShader(buf.GetConstData(), buf.GetSize(), &result);
	LN_THROW(m_deviceObj != nullptr, CompilationException, result);

	PostInitialize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Shader::Initialize(GraphicsManager* manager, const void* code, int length)
{
	GraphicsResourceObject::Initialize(manager);

	ShaderCompileResult result;
	m_deviceObj = m_manager->GetGraphicsDevice()->CreateShader(code, length, &result);
	LN_THROW(m_deviceObj != nullptr, CompilationException, result);

	PostInitialize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Shader::PostInitialize()
{
	// 変数を展開
	for (int i = 0; i < m_deviceObj->GetVariableCount(); ++i)
	{
		ShaderVariable* v = LN_NEW ShaderVariable(this, m_deviceObj->GetVariable(i));
		m_variables.Add(v);

		if (v->GetSemanticName().Compare(_T("VIEWPORTPIXELSIZE"), 17, CaseSensitivity::CaseInsensitive) == 0)
		{
			// こいつが必要なのは DX9 の HLSL だけなので、セマンティクスだけ見ればOK
			m_viewportPixelSize = v;
		}
	}

	// テクニックを展開
	for (int i = 0; i < m_deviceObj->GetTechniqueCount(); ++i)
	{
		m_techniques.Add(LN_NEW ShaderTechnique(this, m_deviceObj->GetTechnique(i)));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderVariable* Shader::FindVariable(const TCHAR* name, CaseSensitivity cs) const
{
	for (ShaderVariable* var : m_variables) {
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderTechnique* Shader::FindTechnique(const TCHAR* name, CaseSensitivity cs) const
{
	for (auto* var : m_techniques) {
		if (var->GetName().Compare(name, -1, cs) == 0) {
			return var;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Shader::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL)
	{
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else
	{
		ShaderCompileResult result;
		m_deviceObj = m_manager->GetGraphicsDevice()->CreateShader(m_sourceCode.GetConstData(), m_sourceCode.GetSize(), &result);
		LN_THROW(result.Level != ShaderCompileResultLevel_Error, InvalidOperationException);	// 一度生成に成功しているので発生はしないはず

		// 変数再割り当て
		int varCount = m_deviceObj->GetVariableCount();
		for (int i = 0; i < varCount; ++i)
		{
			auto* varObj = m_deviceObj->GetVariable(i);
			auto* var = FindVariable(varObj->GetName());
			var->ChangeDevice(varObj);
		}

		// テクニック再割り当て
		int techCount = m_deviceObj->GetTechniqueCount();
		for (int i = 0; i < techCount; ++i)
		{
			auto* techObj = m_deviceObj->GetTechnique(i);
			auto* tech = FindTechnique(techObj->GetName());
			tech->ChangeDevice(techObj);
		}
	}
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
void ShaderValue::SetDeviceTexture(Driver::ITexture* texture)
{
	m_type = ShaderVariableType_DeviceTexture;
	LN_REFOBJ_SET(m_value.DeviceTexture, texture);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void ShaderValue::SetManagedTexture(Texture* texture)
//{
//	m_type = ShaderVariableType_ManagedTexture;
//	LN_REFOBJ_SET(m_value.ManagedTexture, texture);
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderValue::SetString(const char* str)
{
	m_type = ShaderVariableType_String;
	String s;
	s.AssignCStr(str);
	AllocValueBuffer(s.GetByteCount());
	memcpy(m_value.String, s.c_str(), s.GetByteCount());
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
	if (m_type == ShaderVariableType_DeviceTexture) {
		LN_SAFE_RELEASE(m_value.DeviceTexture);
	}
	//if (m_type == ShaderVariableType_ManagedTexture) {
	//	LN_SAFE_RELEASE(m_value.ManagedTexture);
	//}
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
	if (IsBufferCopyType(m_type)) {
		m_value.Buffer = m_buffer.GetData();
	}

	if (m_type == ShaderVariableType_DeviceTexture) {
		m_value.DeviceTexture = value.m_value.DeviceTexture;
		LN_SAFE_ADDREF(m_value.DeviceTexture);
	}
	//if (m_type == ShaderVariableType_ManagedTexture) {
	//	m_value.ManagedTexture = value.m_value.ManagedTexture;
	//	LN_SAFE_ADDREF(m_value.ManagedTexture);
	//}

}

//=============================================================================
// ShaderVariable
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderVariable::ShaderVariable(Shader* owner, Driver::IShaderVariable* deviceObj)
	: m_owner(owner)
	, m_deviceObj(deviceObj)
	, m_textureValue(NULL)
	, m_modified(true)
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
	if (m_value.GetType() != ShaderVariableType_Bool || value != m_value.GetBool())
	{
		SetModified();
		m_value.SetBool(value);
	}
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
	if (m_value.GetType() != ShaderVariableType_Int || value != m_value.GetInt())
	{
		SetModified();
		m_value.SetInt(value);
	}
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
	if (m_value.GetType() != ShaderVariableType_Float || value != m_value.GetFloat())
	{
		SetModified();
		m_value.SetFloat(value);
	}
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
	if (m_value.GetType() != ShaderVariableType_Vector || value != m_value.GetVector())
	{
		SetModified();
		m_value.SetVector(value);
	}
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
	// TODO: != チェックした方がパフォーマンス良い？
	SetModified();
	m_value.SetVectorArray(values, count);
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
	if (m_value.GetType() != ShaderVariableType_Matrix || value != m_value.GetMatrix())
	{
		SetModified();
		m_value.SetMatrix(value);
	}
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
	// TODO: != チェックした方がパフォーマンス良い？
	SetModified();
	m_value.SetMatrixArray(values, count);
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
	bool modified = false;
	if (m_value.GetType() == ShaderVariableType_DeviceTexture)
	{
		if (texture == nullptr)
		{
			if (m_value.GetDeviceTexture() != nullptr)
			{
				modified = true;
			}
		}
		else if (texture->GetDeviceObject() != m_value.GetDeviceTexture())
		{
			modified = true;
		}
	}
	else
	{
		modified = true;
	}

	if (modified)
	{
		SetModified();
		Driver::ITexture* t = (texture != nullptr) ? texture->GetDeviceObject() : nullptr;
		m_value.SetDeviceTexture(t);
		LN_REFOBJ_SET(m_textureValue, texture);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* ShaderVariable::GetTexture() const
{
	//return m_value.GetDeviceTexture();
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderVariable* ShaderVariable::FindAnnotation(const TCHAR* name, CaseSensitivity cs) const
{
	for (ShaderVariable* anno : m_annotations) {
		if (anno->GetName().Compare(name, -1, cs) == 0) {
			return anno;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariable::ChangeDevice(Driver::IShaderVariable* obj)
{
	if (obj == NULL) {
	}
	else
	{
		m_deviceObj = obj;	// 今は特に参照カウントを操作してはいないのでこれだけ

		// アノテーション再割り当て
		int annoCount = m_deviceObj->GetAnnotationCount();
		for (int i = 0; i < annoCount; ++i)
		{
			auto* annoObj = m_deviceObj->GetAnnotation(i);
			auto* anno = FindAnnotation(annoObj->GetName());
			anno->ChangeDevice(annoObj);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariable::SetModified()
{
	m_owner->SetModifiedVariables(true);
	m_modified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariable::TryCommitChanges()
{
	if (m_modified)
	{
		switch (m_value.GetType())
		{
			case ShaderVariableType_Bool:
				LN_CALL_SHADER_COMMAND(SetBool, SetShaderVariableCommand, m_value.GetBool());
				break;
			case ShaderVariableType_Int:
				LN_CALL_SHADER_COMMAND(SetInt, SetShaderVariableCommand, m_value.GetInt());
				break;
			case ShaderVariableType_Float:
				LN_CALL_SHADER_COMMAND(SetFloat, SetShaderVariableCommand, m_value.GetFloat());
				break;
			case ShaderVariableType_Vector:
				LN_CALL_SHADER_COMMAND(SetVector, SetShaderVariableCommand, m_value.GetVector());
				break;
			case ShaderVariableType_VectorArray:
				LN_CALL_SHADER_COMMAND(SetVectorArray, SetShaderVariableCommand, m_value.GetVectorArray(), m_value.GetArrayLength());
				break;
			case ShaderVariableType_Matrix:
				LN_CALL_SHADER_COMMAND(SetMatrix, SetShaderVariableCommand, m_value.GetMatrix());
				break;
			case ShaderVariableType_MatrixArray:
				LN_CALL_SHADER_COMMAND(SetMatrixArray, SetShaderVariableCommand, m_value.GetMatrixArray(), m_value.GetArrayLength());
				break;
			case ShaderVariableType_DeviceTexture:
				LN_CALL_SHADER_COMMAND(SetTexture, SetShaderVariableCommand, m_value.GetDeviceTexture());
				break;
		}
		m_modified = false;
	}
}

//=============================================================================
// ShaderTechnique
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderTechnique::ShaderTechnique(Shader* owner, Driver::IShaderTechnique* deviceObj)
	: m_owner(owner)
	, m_deviceObj(deviceObj)
{
	m_name = m_deviceObj->GetName();

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
const String& ShaderTechnique::GetName() const
{
	return m_name;
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
ShaderPass* ShaderTechnique::GetPass(const TCHAR* name) const
{
	auto itr = std::find_if(m_passes.begin(), m_passes.end(), [name](ShaderPass* pass) { return pass->GetName() == name; });
	LN_THROW(itr != m_passes.end(), KeyNotFoundException);
	return *itr;
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
	for (ShaderVariable* anno : m_annotations) {
		if (anno->GetName().Compare(name, -1, cs) == 0) {
			return anno;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderTechnique::ChangeDevice(Driver::IShaderTechnique* obj)
{
	if (obj == NULL) {
	}
	else
	{
		m_deviceObj = obj;	// 今は特に参照カウントを操作してはいないのでこれだけ

		// パス再割り当て
		int passCount = m_deviceObj->GetPassCount();
		for (int i = 0; i < passCount; ++i)
		{
			auto* passObj = m_deviceObj->GetPass(i);
			auto* pass = GetPass(passObj->GetName());
			pass->ChangeDevice(passObj);
		}

		// アノテーション再割り当て
		int annoCount = m_deviceObj->GetAnnotationCount();
		for (int i = 0; i < annoCount; ++i)
		{
			auto* annoObj = m_deviceObj->GetAnnotation(i);
			auto* anno = FindAnnotation(annoObj->GetName());
			anno->ChangeDevice(annoObj);
		}
	}
}

//=============================================================================
// ShaderPass
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderPass::ShaderPass(Shader* owner, Driver::IShaderPass* deviceObj)
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
	for (ShaderVariable* anno : m_annotations) {
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
	//if (m_owner->m_viewportPixelSize != nullptr)
	//{
	//	Texture* tex = m_owner->GetManager()->GetRenderer()->GetRenderTarget(0);
	//	const Size& size = tex->GetRealSize();
	//	float w = size.Width;
	//	float h = size.Height;
	//	const Vector4& vec = m_owner->m_viewportPixelSize->GetVector();
	//	if (vec.X != w || vec.Y != h) {
	//		m_owner->m_viewportPixelSize->SetVector(Vector4(w, h, 0, 0));
	//	}
	//}

	for (ShaderVariable* v : m_owner->GetVariables())
	{
		v->TryCommitChanges();
	}

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
	for (ShaderVariable* anno : m_annotations) {
		if (anno->GetName().Compare(name, -1, cs) == 0) {
			return anno;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderPass::ChangeDevice(Driver::IShaderPass* obj)
{
	if (obj == NULL) {
	}
	else
	{
		m_deviceObj = obj;	// 今は特に参照カウントを操作してはいないのでこれだけ

		// アノテーション再割り当て
		int annoCount = m_deviceObj->GetAnnotationCount();
		for (int i = 0; i < annoCount; ++i)
		{
			auto* annoObj = m_deviceObj->GetAnnotation(i);
			auto* anno = FindAnnotation(annoObj->GetName());
			anno->ChangeDevice(annoObj);
		}
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
