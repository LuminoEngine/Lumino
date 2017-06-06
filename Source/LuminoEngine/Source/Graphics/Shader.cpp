
#include "Internal.h"
#include <unordered_map>
#include <Lumino/IO/FileManager.h>
#include <Lumino/Graphics/GraphicsException.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/GraphicsContext.h>
#include "GraphicsManager.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Material.h>
#include <Lumino/Rendering/Rendering.h>	// TODO: for LightInfo
#include "RendererImpl.h"
#include "RenderingCommand.h"
#include "ShaderAnalyzer.h"

#define LN_CALL_SHADER_COMMAND(func, command, ...) \
	if (m_owner->GetManager()->GetRenderingType() == GraphicsRenderingType::Threaded) { \
		m_owner->GetManager()->GetRenderer()->m_primaryCommandList->AddCommand<command>(m_deviceObj, __VA_ARGS__); \
	} \
	else { \
		m_deviceObj->func(__VA_ARGS__); \
	}

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

namespace detail {

//==============================================================================
// ShaderSemanticsManager
//		参考:
//		Unity Builtin Shader variables
//		https://docs.unity3d.com/Manual/SL-UnityShaderVariables.html
//==============================================================================

static std::unordered_map<String, BuiltinSemantics> g_builtinNameMap_CameraUnit =
{
	{ _T("ln_View"), BuiltinSemantics::View },
	{ _T("ln_Projection"), BuiltinSemantics::Projection },
	{ _T("ln_ViewportPixelSize"), BuiltinSemantics::ViewportPixelSize },
};

static std::unordered_map<String, BuiltinSemantics> g_builtinNameMap_ElementUnit =
{
	{ _T("ln_WorldViewProjection"), BuiltinSemantics::WorldViewProjection },
	{ _T("ln_World"), BuiltinSemantics::World },
	{ _T("ln_LightEnables"), BuiltinSemantics::LightEnables },
	{ _T("ln_LightWVPMatrices"), BuiltinSemantics::LightWVPMatrices },
	{ _T("ln_LightDirections"), BuiltinSemantics::LightDirections },
	{ _T("ln_LightPositions"), BuiltinSemantics::LightPositions },
	{ _T("ln_LightZFars"), BuiltinSemantics::LightZFars },
	{ _T("ln_LightDiffuses"), BuiltinSemantics::LightDiffuses },
	{ _T("ln_LightAmbients"), BuiltinSemantics::LightAmbients },
	{ _T("ln_LightSpeculars"), BuiltinSemantics::LightSpeculars },
};

static std::unordered_map<String, BuiltinSemantics> g_builtinNameMap_SubsetUnit =
{
	{ _T("ln_MaterialTexture"), BuiltinSemantics::MaterialTexture },
	{ _T("ln_MaterialDiffuse"), BuiltinSemantics::MaterialDiffuse },
	{ _T("ln_MaterialAmbient"), BuiltinSemantics::MaterialAmbient },
	{ _T("ln_MaterialEmmisive"), BuiltinSemantics::MaterialEmmisive },
	{ _T("ln_MaterialSpecular"), BuiltinSemantics::MaterialSpecular },
	{ _T("ln_MaterialSpecularPower"), BuiltinSemantics::MaterialSpecularPower },
	{ _T("ln_ColorScale"), BuiltinSemantics::ColorScale },
	{ _T("ln_BlendColor"), BuiltinSemantics::BlendColor },
	{ _T("ln_ToneColor"), BuiltinSemantics::ToneColor },
};

//------------------------------------------------------------------------------
ShaderSemanticsManager::ShaderSemanticsManager()
	: m_manager(nullptr)
	, m_sceneVariables()
	, m_cameraVariables()
	, m_elementVariables()
	, m_subsetVariables()
	, m_lastCameraInfoId(0)
	, m_tempBufferWriter(&m_tempBuffer)
{
}

//------------------------------------------------------------------------------
void ShaderSemanticsManager::Initialize(GraphicsManager* manager)
{
	if (LN_CHECK_ARG(manager != nullptr)) return;
	m_manager = manager;
}

//------------------------------------------------------------------------------
void ShaderSemanticsManager::TryPushVariable(ShaderVariable* var)
{
	const String& name = var->GetName();

	// Camera unit
	{
		auto itr = g_builtinNameMap_CameraUnit.find(name);
		if (itr != g_builtinNameMap_CameraUnit.end())
		{
			m_cameraVariables.Add({ var, itr->second });
			return;
		}
	}

	// Element unit
	{
		auto itr = g_builtinNameMap_ElementUnit.find(name);
		if (itr != g_builtinNameMap_ElementUnit.end())
		{
			m_elementVariables.Add({ var, itr->second });
			return;
		}
	}

	// Subset unit
	{
		auto itr = g_builtinNameMap_SubsetUnit.find(name);
		if (itr != g_builtinNameMap_SubsetUnit.end())
		{
			m_subsetVariables.Add({ var, itr->second });
			return;
		}
	}
}

//------------------------------------------------------------------------------
void ShaderSemanticsManager::UpdateSceneVariables(const SceneInfo& info)
{

}

//------------------------------------------------------------------------------
void ShaderSemanticsManager::UpdateCameraVariables(const CameraInfo& info)
{
	if (m_lastCameraInfoId != info.dataSourceId)
	{
		m_lastCameraInfoId = info.dataSourceId;

		for (const VariableKindPair& varInfo : m_cameraVariables)
		{
			switch (varInfo.kind)
			{
				case BuiltinSemantics::View:
					varInfo.variable->SetMatrix(info.viewMatrix);
					break;
				case BuiltinSemantics::Projection:
					varInfo.variable->SetMatrix(info.projMatrix);
					break;
				case BuiltinSemantics::ViewportPixelSize:
					varInfo.variable->SetVector(Vector4(info.viewPixelSize.width, info.viewPixelSize.height, 0, 0));
					break;
				default:
					break;
			}
		}
	}
}

//------------------------------------------------------------------------------
void ShaderSemanticsManager::UpdateElementVariables(const ElementInfo& info)
{
	DynamicLightInfo** lights = info.affectedLights;

	for (const VariableKindPair& varInfo : m_elementVariables)
	{
		switch (varInfo.kind)
		{
			case BuiltinSemantics::WorldViewProjection:
				varInfo.variable->SetMatrix(info.WorldViewProjectionMatrix);
				break;
			case BuiltinSemantics::World:
				varInfo.variable->SetMatrix(info.WorldMatrix);
				break;


			// TODO: 以下、ライト列挙時に確定したい。何回もこんな計算するのはちょっと・・
			case BuiltinSemantics::LightEnables:
				m_tempBufferWriter.Seek(0, SeekOrigin_Begin);
				for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
				{
					// TODO: WriteBool() がほしい
					m_tempBufferWriter.WriteUInt8((lights == nullptr) ? false : lights[i] != nullptr);
				}
				varInfo.variable->SetBoolArray((const bool*)m_tempBuffer.GetBuffer(), DynamicLightInfo::MaxLights);
				break;
			case BuiltinSemantics::LightWVPMatrices:
				LN_NOTIMPLEMENTED();
				//if (lights != nullptr)
				//{
				//	m_tempBufferWriter.Seek(0, SeekOrigin_Begin);
				//	for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
				//	{
				//		Matrix m = (lights[i] != nullptr) ? (lights[i]->transform * (*info.viewProjMatrix)) : Matrix::Identity;
				//		m_tempBufferWriter.Write(&m, sizeof(Matrix));
				//	}
				//	varInfo.variable->SetMatrixArray((const Matrix*)m_tempBuffer.GetBuffer(), DynamicLightInfo::MaxLights);
				//}
				break;
			case BuiltinSemantics::LightDirections:
				if (lights != nullptr)
				{
					m_tempBufferWriter.Seek(0, SeekOrigin_Begin);
					for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
					{
						// TODO: Vector4::Zero がほしい
						Vector4 v = (lights[i] != nullptr) ? Vector4(lights[i]->m_direction, 0) : Vector4(0, 0, 0, 0);
						m_tempBufferWriter.Write(&v, sizeof(Vector4));
					}
					varInfo.variable->SetVectorArray((const Vector4*)m_tempBuffer.GetBuffer(), DynamicLightInfo::MaxLights);
				}
				break;
			case BuiltinSemantics::LightPositions:
				if (lights != nullptr)
				{
					for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
					{
						// TODO: Vector4::Zero がほしい
						Vector4 v = (lights[i] != nullptr) ? Vector4(lights[i]->transform.GetPosition(), 0) : Vector4(0, 0, 0, 0);
						m_tempBufferWriter.Write(&v, sizeof(Vector4));
					}
					varInfo.variable->SetVectorArray((const Vector4*)m_tempBuffer.GetBuffer(), DynamicLightInfo::MaxLights);
				}
				break;
			case BuiltinSemantics::LightZFars:
				if (lights != nullptr)
				{
					m_tempBufferWriter.Seek(0, SeekOrigin_Begin);
					for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
					{
						m_tempBufferWriter.WriteFloat((lights[i] != nullptr) ? lights[i]->m_shadowZFar : 0.0f);
					}
					varInfo.variable->SetFloatArray((const float*)m_tempBuffer.GetBuffer(), DynamicLightInfo::MaxLights);
				}
				break;
			case BuiltinSemantics::LightDiffuses:
				if (lights != nullptr)
				{
					m_tempBufferWriter.Seek(0, SeekOrigin_Begin);
					for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
					{
						auto& v = (lights[i] != nullptr) ? lights[i]->m_diffuse : Color::Black;
						m_tempBufferWriter.Write(&v, sizeof(Color));
					}
					varInfo.variable->SetVectorArray((const Vector4*)m_tempBuffer.GetBuffer(), DynamicLightInfo::MaxLights);
				}
				break;
			case BuiltinSemantics::LightAmbients:
				if (lights != nullptr)
				{
					m_tempBufferWriter.Seek(0, SeekOrigin_Begin);
					for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
					{
						auto& v = (lights[i] != nullptr) ? lights[i]->m_ambient : Color::Transparency;		// TODO: デフォルト値は？
						m_tempBufferWriter.Write(&v, sizeof(Color));
					}
					varInfo.variable->SetVectorArray((const Vector4*)m_tempBuffer.GetBuffer(), DynamicLightInfo::MaxLights);
				}
				break;
			case BuiltinSemantics::LightSpeculars:
				if (lights != nullptr)
				{
					m_tempBufferWriter.Seek(0, SeekOrigin_Begin);
					for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
					{
						auto& v = (lights[i] != nullptr) ? lights[i]->m_specular : Color::Black;		// TODO: デフォルト値は？
						m_tempBufferWriter.Write(&v, sizeof(Color));
					}
					varInfo.variable->SetVectorArray((const Vector4*)m_tempBuffer.GetBuffer(), DynamicLightInfo::MaxLights);
				}
				break;

			default:
				break;
		}
	}
}

//------------------------------------------------------------------------------
void ShaderSemanticsManager::UpdateSubsetVariables(const SubsetInfo& info)
{
	detail::CombinedMaterial* cm = info.combinedMaterial;

	// TODO: このあたり、もし最適化で変数が消えているなら set しなくて良いようにしたい。

	for (const VariableKindPair& varInfo : m_subsetVariables)
	{
		switch (varInfo.kind)
		{
			case BuiltinSemantics::MaterialTexture:
				varInfo.variable->SetTexture((info.materialTexture != nullptr) ? info.materialTexture : m_manager->GetDummyWhiteTexture());
				break;
			case BuiltinSemantics::MaterialDiffuse:
				if (cm != nullptr)
					varInfo.variable->SetVector(cm->m_diffuse);
				break;
			case BuiltinSemantics::MaterialAmbient:
				if (cm != nullptr)
					varInfo.variable->SetVector(cm->m_ambient);
				break;
			case BuiltinSemantics::MaterialEmmisive:
				if (cm != nullptr)
					varInfo.variable->SetVector(cm->m_emissive);
				break;
			case BuiltinSemantics::MaterialSpecular:
				if (cm != nullptr)
					varInfo.variable->SetVector(cm->m_specular);
				break;
			case BuiltinSemantics::MaterialSpecularPower:
				if (cm != nullptr)
					varInfo.variable->SetFloat(cm->m_power);
				break;
			case BuiltinSemantics::ColorScale:
				if (cm != nullptr)
					varInfo.variable->SetVector(cm->m_colorScale);
				break;
			case BuiltinSemantics::BlendColor:
				if (cm != nullptr)
					varInfo.variable->SetVector(cm->m_blendColor);
				break;
			case BuiltinSemantics::ToneColor:
				if (cm != nullptr)
					varInfo.variable->SetVector(cm->m_tone);
				break;
			default:
				break;
		}
	}
}

//------------------------------------------------------------------------------
//void ShaderSemanticsManager::SetMaterialTexture(Texture* texture)
//{
//	if (m_subsetVariables[SubsetSemantics::MaterialTexture] != nullptr)
//	{
//		m_subsetVariables[SubsetSemantics::MaterialTexture]->SetTexture((texture != nullptr) ? texture : m_manager->GetDummyWhiteTexture());
//	}
//}

} // namespace detail


//==============================================================================
// Shader
//==============================================================================

//------------------------------------------------------------------------------
ShaderPtr Shader::GetBuiltinShader(BuiltinShader shader)
{
	return detail::GraphicsManager::GetInstance()->GetBuiltinShader(shader);
}

//------------------------------------------------------------------------------
RefPtr<Shader> Shader::Create(const StringRef& filePath, bool useTRSS)
{
	RefPtr<Shader> obj(LN_NEW Shader(), false);
	obj->Initialize(detail::GraphicsManager::GetInstance(), filePath, useTRSS);
	return obj;
}

//------------------------------------------------------------------------------
RefPtr<Shader> Shader::Create(const char* code, int length)
{
	RefPtr<Shader> obj(LN_NEW Shader(), false);
	obj->Initialize(detail::GraphicsManager::GetInstance(), code, length);
	return obj;
}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
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
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
Shader::Shader()
	: m_deviceObj(nullptr)
	, m_sourceCode()
	, m_modifiedVariables(true)
{
}

//------------------------------------------------------------------------------
Shader::~Shader()
{
	for (ShaderVariable* var : m_variables) {
		LN_SAFE_RELEASE(var);
	}
	for (ShaderTechnique* tech : m_techniques) {
		LN_SAFE_RELEASE(tech);
	}

	LN_SAFE_RELEASE(m_deviceObj);
}

//------------------------------------------------------------------------------
void Shader::Initialize(detail::GraphicsManager* manager, const StringRef& filePath, bool useTRSS)
{
	RefPtr<Stream> stream(manager->GetFileManager()->CreateFileStream(filePath), false);
	ByteBuffer buf((size_t)stream->GetLength() + 1, false);
	stream->Read(buf.GetData(), buf.GetSize());
	buf[(size_t)stream->GetLength()] = 0x00;

	Initialize(manager, buf.GetConstData(), buf.GetSize(), useTRSS);

	//GraphicsResourceObject::Initialize(manager);
	//

	//

	//ShaderCompileResult result;
	//m_deviceObj = m_manager->GetGraphicsDevice()->CreateShader(buf.GetConstData(), buf.GetSize(), &result);
	//LN_THROW(m_deviceObj != nullptr, CompilationException, result);

	//PostInitialize();
}

//------------------------------------------------------------------------------
void Shader::Initialize(detail::GraphicsManager* manager, const void* code, int length, bool useTRSS)
{
	GraphicsResourceObject::Initialize(manager);

	StringBuilderA newCode;
	if (useTRSS)
	{
		detail::ShaderAnalyzer analyzer;
		analyzer.AnalyzeLNFX((const char*)code, length);
		auto cc = analyzer.MakeHLSLCode();

		newCode.Append(cc.data(), cc.size());

		//FileSystem::WriteAllBytes(_T("code.c"), cc.data(), cc.size());
	}
	else
	{
		// ヘッダコード先頭に追加する
		newCode.Append(manager->GetCommonShaderHeader().c_str());
		newCode.Append("#line 5");
		newCode.Append(StringA::GetNewLine().c_str());
		newCode.Append((const char*)code, length);
		newCode.Append("\n");	// 最後には改行を入れておく。環境によっては改行がないとエラーになる。しかもエラーなのにエラー文字列が出ないこともある。
	}

	ShaderCompileResult result;
	m_deviceObj = m_manager->GetGraphicsDevice()->CreateShader(newCode.c_str(), newCode.GetLength(), &result);
	LN_THROW(m_deviceObj != nullptr, CompilationException, result);

	// ライブラリ外部からの DeviceContext 再設定に備えてコードを保存する
	m_sourceCode.Alloc(newCode.c_str(), newCode.GetLength());

	PostInitialize();
}

//------------------------------------------------------------------------------
void Shader::PostInitialize()
{
	m_semanticsManager.Initialize(m_manager);

	// 変数を展開
	for (int i = 0; i < m_deviceObj->GetVariableCount(); ++i)
	{
		ShaderVariable* v = LN_NEW ShaderVariable(this, m_deviceObj->GetVariable(i));
		m_variables.Add(v);
		m_semanticsManager.TryPushVariable(v);
	}

	// テクニックを展開
	for (int i = 0; i < m_deviceObj->GetTechniqueCount(); ++i)
	{
		m_techniques.Add(LN_NEW ShaderTechnique(this, m_deviceObj->GetTechnique(i)));
	}
}

//------------------------------------------------------------------------------
void Shader::TryCommitChanges()
{
	auto* serializer = GetManager()->GetShaderVariableCommitSerializeHelper();
	serializer->BeginSerialize();
	for (ShaderVariable* v : GetVariables())
	{
		v->OnCommitChanges();
		serializer->WriteValue(v->GetDeviceObject(), v->m_value);
	}

	RenderBulkData varsData(serializer->GetSerializeData(), serializer->GetSerializeDataLength());

	auto* cmdList = GetManager()->GetPrimaryRenderingCommandList();
	byte_t* data = (byte_t*)varsData.Alloc(cmdList);

	detail::GraphicsManager* manager = GetManager();
	Shader* _this = this;
	LN_ENQUEUE_RENDER_COMMAND_3(
		CommitChanges, GetManager(),
		detail::GraphicsManager*, manager,
		RenderBulkData, varsData,
		RefPtr<Shader>, _this,
		{
			auto* serializer = manager->GetShaderVariableCommitSerializeHelper();
			serializer->Deserialize(varsData.GetData(), varsData.GetSize());
		});
}

//------------------------------------------------------------------------------
ShaderVariable* Shader::FindVariable(const TCHAR* name, CaseSensitivity cs) const
{
	for (ShaderVariable* var : m_variables) {
		if (var->GetName().Compare(name, -1, cs) == 0) {
			return var;
		}
	}
	return NULL;
}

//------------------------------------------------------------------------------
const List<ShaderTechnique*>& Shader::GetTechniques() const
{
	return m_techniques;
}

//------------------------------------------------------------------------------
ShaderTechnique* Shader::FindTechnique(const TCHAR* name, CaseSensitivity cs) const
{
	for (auto* var : m_techniques) {
		if (var->GetName().Compare(name, -1, cs) == 0) {
			return var;
		}
	}
	return NULL;
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
void Shader::SetShaderValue(uint32_t variableNameHash, const ShaderValue& value)
{
	for (ShaderVariable* v : m_variables)
	{
		if (v->GetNameHash() == variableNameHash)
		{
			v->SetShaderValue(value);
			return;
		}
	}
}


//==============================================================================
// ShaderValue
//==============================================================================

//------------------------------------------------------------------------------
ShaderValue::ShaderValue()
	: m_hashCode(0)
	, m_hashDirty(true)
{
	m_type = ShaderVariableType_Unknown;
	memset(&m_value, 0, sizeof(m_value));
}

//------------------------------------------------------------------------------
ShaderValue::~ShaderValue()
{
	ReleaseValueBuffer();
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
void ShaderValue::SetBool(bool value)
{
	m_type = ShaderVariableType_Bool;
	m_value.BoolVal = value;
	m_hashDirty = true;
}

//------------------------------------------------------------------------------
void ShaderValue::SetInt(int value)
{
	m_type = ShaderVariableType_Int;
	m_value.Int = value;
	m_hashDirty = true;
}

//------------------------------------------------------------------------------
void ShaderValue::SetBoolArray(const bool* values, int count)
{
	m_type = ShaderVariableType_BoolArray;
	AllocValueBuffer(sizeof(bool) * count);
	if (values != NULL) {
		memcpy(m_value.BoolArray, values, sizeof(bool) * count);
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
void ShaderValue::SetFloat(float value)
{
	m_type = ShaderVariableType_Float;
	m_value.Float = value;
	m_hashDirty = true;
}

//------------------------------------------------------------------------------
void ShaderValue::SetFloatArray(const float* values, int count)
{
	m_type = ShaderVariableType_FloatArray;
	AllocValueBuffer(sizeof(float) * count);
	if (values != NULL) {
		memcpy(m_value.FloatArray, values, sizeof(float) * count);
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
void ShaderValue::SetVector(const Vector4& vec)
{
	m_type = ShaderVariableType_Vector;
	AllocValueBuffer(sizeof(Vector4));
	*m_value.Vector = vec;
	m_hashDirty = true;
}

//------------------------------------------------------------------------------
void ShaderValue::SetVectorArray(const Vector4* vectors, int count)
{
	m_type = ShaderVariableType_VectorArray;
	AllocValueBuffer(sizeof(Vector4) * count);
	if (vectors != NULL) {
		memcpy(m_value.VectorArray, vectors, sizeof(Vector4) * count);
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
void ShaderValue::SetMatrix(const Matrix& matrix)
{
	m_type = ShaderVariableType_Matrix;
	AllocValueBuffer(sizeof(Matrix));
	*m_value.Matrix = matrix;
	m_hashDirty = true;
}


//------------------------------------------------------------------------------
void ShaderValue::SetMatrixArray(const Matrix* matrices, int count)
{
	m_type = ShaderVariableType_MatrixArray;
	AllocValueBuffer(sizeof(Matrix) * count);
	if (matrices != NULL) {
		memcpy(m_value.MatrixArray, matrices, sizeof(Matrix) * count);
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
void ShaderValue::SetDeviceTexture(Driver::ITexture* texture)
{
	m_type = ShaderVariableType_DeviceTexture;
	LN_REFOBJ_SET(m_value.DeviceTexture, texture);
	m_hashDirty = true;
}

//------------------------------------------------------------------------------
void ShaderValue::SetManagedTexture(Texture* texture)
{
	m_type = ShaderVariableType_ManagedTexture;
	LN_REFOBJ_SET(m_value.ManagedTexture, texture);
	m_hashDirty = true;
}

//------------------------------------------------------------------------------
void ShaderValue::SetString(const char* str)
{
	m_type = ShaderVariableType_String;
	String s;
	s.AssignCStr(str);
	size_t size = s.GetByteCount() + sizeof(TCHAR);
	AllocValueBuffer(size);
	memcpy(m_value.String, s.c_str(), size);
	m_hashDirty = true;
}

//------------------------------------------------------------------------------
void ShaderValue::SetString(const String& s)
{
	m_type = ShaderVariableType_String;
	size_t size = s.GetByteCount() + sizeof(TCHAR);
	AllocValueBuffer(size);
	memcpy(m_value.String, s.c_str(), size);
	m_hashDirty = true;
}

//------------------------------------------------------------------------------
bool ShaderValue::Equals(const ShaderValue& value) const
{
	if (m_type != value.m_type) return false;

	switch (m_type)
	{
		case ShaderVariableType_Bool:
			return m_value.BoolVal == value.m_value.BoolVal;
		case ShaderVariableType_BoolArray:
			return m_buffer.Equals(value.m_buffer);
		case ShaderVariableType_Int:
			return m_value.Int == value.m_value.Int;
		case ShaderVariableType_Float:
			return m_value.Float == value.m_value.Float;
		case ShaderVariableType_FloatArray:
			return m_buffer.Equals(value.m_buffer);
		case ShaderVariableType_Vector:
			return m_value.Vector == value.m_value.Vector;
		case ShaderVariableType_VectorArray:
			return m_buffer.Equals(value.m_buffer);
		case ShaderVariableType_Matrix:
			return m_value.Matrix == value.m_value.Matrix;
		case ShaderVariableType_MatrixArray:
			return m_buffer.Equals(value.m_buffer);
		case ShaderVariableType_DeviceTexture:
			return m_value.DeviceTexture == value.m_value.DeviceTexture;
		case ShaderVariableType_ManagedTexture:
			return m_value.ManagedTexture == value.m_value.ManagedTexture;
		case ShaderVariableType_String:
			return StringTraits::Compare(m_value.String, m_value.String, -1) == 0;
		default:
			assert(0);
			break;
	}
	return false;
}

//------------------------------------------------------------------------------
uint32_t ShaderValue::GetHashCode()
{
	if (m_hashDirty)
	{
		m_hashCode = Hash::CalcHash(reinterpret_cast<const char*>(&m_value), sizeof(m_value));
		m_hashCode += (int)m_type;

		if (IsBufferCopyType(m_type))
		{
			m_hashCode += Hash::CalcHash((const char*)m_buffer.GetConstData(), m_buffer.GetSize());
		}

		m_hashDirty = false;
	}

	return m_hashCode;
}

//------------------------------------------------------------------------------
int ShaderValue::GetArrayLength() const
{
	if (m_type == ShaderVariableType_BoolArray) {
		return m_buffer.GetSize() / sizeof(bool);
	}
	if (m_type == ShaderVariableType_FloatArray) {
		return m_buffer.GetSize() / sizeof(float);
	}
	if (m_type == ShaderVariableType_VectorArray) {
		return m_buffer.GetSize() / sizeof(Vector4);
	}
	if (m_type == ShaderVariableType_MatrixArray) {
		return m_buffer.GetSize() / sizeof(Matrix);
	}
	return 0;
}

//------------------------------------------------------------------------------
void ShaderValue::ReleaseValueBuffer()
{
	/*if (m_value.ByteCount > 0) {
		LN_SAFE_DELETE_ARRAY(m_value.Buffer);
	}
	else */
	if (m_type == ShaderVariableType_DeviceTexture) {
		LN_SAFE_RELEASE(m_value.DeviceTexture);
	}
	if (m_type == ShaderVariableType_ManagedTexture) {
		LN_SAFE_RELEASE(m_value.ManagedTexture);
	}
	//m_value.ByteCount = 0;
	m_buffer.Release();
}

//------------------------------------------------------------------------------
void ShaderValue::AllocValueBuffer(size_t byteCount)
{
	if (byteCount > m_buffer.GetSize()/*m_value.ByteCount*//* || m_buffer.GetReferenceCount() != 1*/)
	{
		//LN_SAFE_DELETE_ARRAY(m_value.Buffer);
		//m_value.Buffer = LN_NEW byte_t[byteCount];
		//m_value.ByteCount = byteCount;
		//m_buffer.Attach(LN_NEW ByteBuffer(byteCount), false);
		m_buffer.Resize(byteCount);
		m_value.Buffer = m_buffer.GetData();
	}
}

//------------------------------------------------------------------------------
void ShaderValue::Copy(const ShaderValue& value)
{
	m_type = value.m_type;
	m_buffer = value.m_buffer;	// 共有参照
	m_hashCode = value.m_hashCode;
	m_hashDirty = value.m_hashDirty;

	//if (m_buffer.IsNull()) {
	//	memcpy(&m_value, &value.m_value, sizeof(m_value));
	//}
	//else {
	//	m_value.Buffer = m_buffer->GetData();
	//}
	if (IsBufferCopyType(m_type))
	{
		m_value.Buffer = m_buffer.GetData();
	}
	else
	{
		m_value = value.m_value;
	}

	if (m_type == ShaderVariableType_DeviceTexture) {
		m_value.DeviceTexture = value.m_value.DeviceTexture;
		LN_SAFE_ADDREF(m_value.DeviceTexture);
	}
	if (m_type == ShaderVariableType_ManagedTexture) {
		m_value.ManagedTexture = value.m_value.ManagedTexture;
		LN_SAFE_ADDREF(m_value.ManagedTexture);
	}

}

//==============================================================================
// ShaderVariable
//==============================================================================

//------------------------------------------------------------------------------
ShaderVariable::ShaderVariable(Shader* owner, Driver::IShaderVariable* deviceObj)
	: m_owner(owner)
	, m_deviceObj(deviceObj)
	, m_textureValue(NULL)
	, m_modified(true)
{
	m_name = deviceObj->GetName();
	m_nameHash = Hash::CalcHash(m_name.c_str(), m_name.GetLength());

	// 初期値として保持しておく
	m_value = deviceObj->GetValue();

	// アノテーションの展開
	for (int i = 0; i < m_deviceObj->GetAnnotationCount(); ++i) {
		m_annotations.Add(LN_NEW ShaderVariable(m_owner, m_deviceObj->GetAnnotation(i)));
	}
}

//------------------------------------------------------------------------------
ShaderVariable::~ShaderVariable()
{
	for (ShaderVariable* anno : m_annotations) {
		anno->Release();
	}
	LN_SAFE_RELEASE(m_textureValue);
}

//------------------------------------------------------------------------------
ShaderVariableType ShaderVariable::GetType() const
{
	return m_deviceObj->GetType();
}

//------------------------------------------------------------------------------
const String& ShaderVariable::GetName() const
{
	return m_name;
}

//------------------------------------------------------------------------------
const String& ShaderVariable::GetSemanticName() const
{
	return m_deviceObj->GetSemanticName();
}

//------------------------------------------------------------------------------
int ShaderVariable::GetRows() const
{
	return m_deviceObj->GetMatrixRows();
}

//------------------------------------------------------------------------------
int ShaderVariable::GetColumns() const
{
	return m_deviceObj->GetMatrixColumns();
}

//------------------------------------------------------------------------------
int ShaderVariable::GetArrayElements() const
{
	return m_deviceObj->GetArrayElements();
}

//------------------------------------------------------------------------------
void ShaderVariable::SetBool(bool value)
{
	if (m_value.GetType() != ShaderVariableType_Bool || value != m_value.GetBool())
	{
		SetModified();
		m_value.SetBool(value);
	}
}

//------------------------------------------------------------------------------
bool ShaderVariable::GetBool() const
{
	return m_value.GetBool();
}

//------------------------------------------------------------------------------
void ShaderVariable::SetInt(int value)
{
	if (m_value.GetType() != ShaderVariableType_Int || value != m_value.GetInt())
	{
		SetModified();
		m_value.SetInt(value);
	}
}

//------------------------------------------------------------------------------
void ShaderVariable::SetBoolArray(const bool* values, int count)
{
	// TODO: != チェックした方がパフォーマンス良い？
	SetModified();
	m_value.SetBoolArray(values, count);
}

//------------------------------------------------------------------------------
int ShaderVariable::GetInt() const
{
	return m_value.GetInt();
}

//------------------------------------------------------------------------------
void ShaderVariable::SetFloat(float value)
{
	if (m_value.GetType() != ShaderVariableType_Float || value != m_value.GetFloat())
	{
		SetModified();
		m_value.SetFloat(value);
	}
}

//------------------------------------------------------------------------------
float ShaderVariable::GetFloat() const
{
	return m_value.GetFloat();
}

//------------------------------------------------------------------------------
void ShaderVariable::SetFloatArray(const float* values, int count)
{
	// TODO: != チェックした方がパフォーマンス良い？
	SetModified();
	m_value.SetFloatArray(values, count);
}

//------------------------------------------------------------------------------
void ShaderVariable::SetVector(const Vector4& value)
{
	if (m_value.GetType() != ShaderVariableType_Vector || value != m_value.GetVector())
	{
		SetModified();
		m_value.SetVector(value);
	}
}

//------------------------------------------------------------------------------
const Vector4& ShaderVariable::GetVector() const
{
	return m_value.GetVector();
}

//------------------------------------------------------------------------------
void ShaderVariable::SetVectorArray(const Vector4* values, int count)
{
	// TODO: != チェックした方がパフォーマンス良い？
	SetModified();
	m_value.SetVectorArray(values, count);
}

//------------------------------------------------------------------------------
const Vector4* ShaderVariable::GetVectorArray() const
{
	return m_value.GetVectorArray();
}

//------------------------------------------------------------------------------
void ShaderVariable::SetMatrix(const Matrix& value)
{
	if (m_value.GetType() != ShaderVariableType_Matrix || value != m_value.GetMatrix())
	{
		SetModified();
		m_value.SetMatrix(value);
	}
}

//------------------------------------------------------------------------------
const Matrix& ShaderVariable::GetMatrix() const
{
	return m_value.GetMatrix();
}

//------------------------------------------------------------------------------
void ShaderVariable::SetMatrixArray(const Matrix* values, int count)
{
	// TODO: != チェックした方がパフォーマンス良い？
	SetModified();
	m_value.SetMatrixArray(values, count);
}

//------------------------------------------------------------------------------
const Matrix* ShaderVariable::GetMatrixArray() const
{
	return m_value.GetMatrixArray();
}

//------------------------------------------------------------------------------
void ShaderVariable::SetTexture(Texture* texture)
{
	//if (m_value.GetType() != ShaderVariableType_ManagedTexture || texture != m_value.GetManagedTexture())
	//{
	//	SetModified();
	//	m_value.SetManagedTexture();
	//}
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
		else if (texture->ResolveDeviceObject() != m_value.GetDeviceTexture())
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
		Driver::ITexture* t = (texture != nullptr) ? texture->ResolveDeviceObject() : nullptr;
		m_value.SetDeviceTexture(t);
		LN_REFOBJ_SET(m_textureValue, texture);
	}
}

//------------------------------------------------------------------------------
Texture* ShaderVariable::GetTexture() const
{
	//return m_value.GetDeviceTexture();
	return m_textureValue;
}

//------------------------------------------------------------------------------
void ShaderVariable::SetString(const char* str)
{
	m_value.SetString(str);
}

//------------------------------------------------------------------------------
const TCHAR* ShaderVariable::GetString() const
{
	return m_value.GetString();
}

//------------------------------------------------------------------------------
void ShaderVariable::SetShaderValue(const ShaderValue& value)
{
	if (value.GetType() == ShaderVariableType_ManagedTexture)
	{
		// テクスチャ型の場合はちょっと特殊
		SetTexture(value.GetManagedTexture());
	}
	else
	{
		if (!m_value.Equals(value))
		{
			SetModified();
			m_value = value;
		}
	}
}

//------------------------------------------------------------------------------
const List<ShaderVariable*>& ShaderVariable::GetAnnotations() const
{
	return m_annotations;
}

//------------------------------------------------------------------------------
ShaderVariable* ShaderVariable::FindAnnotation(const TCHAR* name, CaseSensitivity cs) const
{
	for (ShaderVariable* anno : m_annotations) {
		if (anno->GetName().Compare(name, -1, cs) == 0) {
			return anno;
		}
	}
	return nullptr;
}

//------------------------------------------------------------------------------
void ShaderVariable::ChangeDevice(Driver::IShaderVariable* obj)
{
	if (obj == nullptr) {
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

//------------------------------------------------------------------------------
void ShaderVariable::SetModified()
{
	detail::ContextInterface* activeContext = m_owner->GetManager()->GetActiveContext();
	if (activeContext != nullptr) activeContext->OnShaderVariableModified(this);
	m_owner->SetModifiedVariables(true);
	m_modified = true;
}

//------------------------------------------------------------------------------
void ShaderVariable::OnCommitChanges()
{
	if (m_modified)
	{
		m_modified = false;
	}

	if (m_textureValue != nullptr)
	{
		m_textureValue->ResolveDeviceObject();
	}
}


//==============================================================================
// ShaderTechnique
//==============================================================================

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
ShaderTechnique::~ShaderTechnique()
{
	for (ShaderPass* pass : m_passes) {
		LN_SAFE_RELEASE(pass);
	}
	for (ShaderVariable* anno : m_annotations) {
		LN_SAFE_RELEASE(anno);
	}
}

//------------------------------------------------------------------------------
const String& ShaderTechnique::GetName() const
{
	return m_name;
}

//------------------------------------------------------------------------------
const List<ShaderPass*>& ShaderTechnique::GetPasses() const
{
	return m_passes;
}

//------------------------------------------------------------------------------
ShaderPass* ShaderTechnique::GetPass(const TCHAR* name) const
{
	auto itr = std::find_if(m_passes.begin(), m_passes.end(), [name](ShaderPass* pass) { return pass->GetName() == name; });
	LN_THROW(itr != m_passes.end(), KeyNotFoundException);
	return *itr;
}

//------------------------------------------------------------------------------
const List<ShaderVariable*>& ShaderTechnique::GetAnnotations() const
{
	return m_annotations;
}

//------------------------------------------------------------------------------
ShaderVariable* ShaderTechnique::FindAnnotation(const TCHAR* name, CaseSensitivity cs) const
{
	for (ShaderVariable* anno : m_annotations) {
		if (anno->GetName().Compare(name, -1, cs) == 0) {
			return anno;
		}
	}
	return NULL;
}

//------------------------------------------------------------------------------
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

//==============================================================================
// ShaderPass
//==============================================================================

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
ShaderPass::~ShaderPass()
{
	for (ShaderVariable* anno : m_annotations) {
		LN_SAFE_RELEASE(anno);
	}
}

//------------------------------------------------------------------------------
const String& ShaderPass::GetName() const
{
	return m_name;
}

//------------------------------------------------------------------------------
void ShaderPass::Apply()
{
	m_owner->TryCommitChanges();

	//LN_CALL_SHADER_COMMAND(Apply, ApplyShaderPassCommand);
    if (m_owner->GetManager()->GetRenderingType() == GraphicsRenderingType::Threaded) {
        m_owner->GetManager()->GetRenderer()->m_primaryCommandList->AddCommand<ApplyShaderPassCommand>(m_deviceObj);
    }
    else {
		m_owner->GetManager()->GetGraphicsDevice()->GetRenderer()->SetShaderPass(m_deviceObj);
    }
}

//------------------------------------------------------------------------------
const List<ShaderVariable*>& ShaderPass::GetAnnotations() const
{
	return m_annotations;
}

//------------------------------------------------------------------------------
ShaderVariable* ShaderPass::FindAnnotation(const TCHAR* name, CaseSensitivity cs) const
{
	for (ShaderVariable* anno : m_annotations) {
		if (anno->GetName().Compare(name, -1, cs) == 0) {
			return anno;
		}
	}
	return NULL;
}

//------------------------------------------------------------------------------
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
