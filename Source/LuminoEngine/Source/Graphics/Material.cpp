
#include "Internal.h"
#include <Lumino/Graphics/Material.h>

LN_NAMESPACE_BEGIN


//==============================================================================
// BuiltinEffectData
//==============================================================================
namespace detail {

const BuiltinEffectData BuiltinEffectData::DefaultData;

} // namespace detail

//==============================================================================
// MaterialList
//==============================================================================

//------------------------------------------------------------------------------
MaterialList::MaterialList()
{
}

//------------------------------------------------------------------------------
MaterialList::~MaterialList()
{
}

//==============================================================================
// Material
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Material, Object);
//LN_TR_PROPERTY_IMPLEMENT(Material, BlendMode, blendMode, tr::PropertyMetadata(Material::OnRenderStateChanged));
//LN_TR_PROPERTY_IMPLEMENT(Material, CullingMode, cullingMode, tr::PropertyMetadata(Material::OnRenderStateChanged));
//LN_TR_PROPERTY_IMPLEMENT(Material, bool, depthTestEnabled, tr::PropertyMetadata(Material::OnRenderStateChanged));
//LN_TR_PROPERTY_IMPLEMENT(Material, bool, depthWriteEnabled, tr::PropertyMetadata(Material::OnRenderStateChanged));

const String Material::DiffuseParameter(_T("Diffuse"));
const String Material::AmbientParameter(_T("Ambient"));
const String Material::SpecularParameter(_T("Specular"));
const String Material::EmissiveParameter(_T("Emissive"));
const String Material::PowerParameter(_T("Power"));
const String Material::MaterialTextureParameter(_T("MaterialTexture"));

const Color Material::DefaultDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
const Color Material::DefaultAmbient(0.0f, 0.0f, 0.0f, 0.0f);
const Color Material::DefaultSpecular(0.5f, 0.5f, 0.5f, 0.5f);
const Color Material::DefaultEmissive(0.0f, 0.0f, 0.0f, 0.0f);
const float Material::DefaultPower = 50.0f;

//------------------------------------------------------------------------------
MaterialPtr Material::Create()
{
	auto ptr = MaterialPtr::MakeRef();
	ptr->Initialize();
	return ptr;
}

//------------------------------------------------------------------------------
Material::Material()
	: blendMode(nullptr)
	, cullingMode(nullptr)
	, depthTestEnabled(nullptr)
	, depthWriteEnabled(nullptr)
	, m_revisionCount(0)
{
	InitializeProperties();
	Reset();
}

//------------------------------------------------------------------------------
Material::~Material()
{
}

//------------------------------------------------------------------------------
void Material::Initialize()
{
	//m_combinedMaterial = RefPtr<detail::CombinedMaterial>::MakeRef();
}

//------------------------------------------------------------------------------
void Material::Reset()
{
	m_shader = nullptr;
	// TODO: とりあえず
	blendMode = nullptr;
	cullingMode = nullptr;
	////m_builtin.fill = FillMode_Solid;
	//m_builtin.alphaTest = true;
	depthTestEnabled = nullptr;
	depthWriteEnabled = nullptr;
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::SetBuiltinIntParameter(const StringRef& name, int value) { m_builtinValueMap[Hash::CalcHash(name.GetBegin(), name.GetLength())].SetInt(value); }
void Material::SetBuiltinFloatParameter(const StringRef& name, float value) { m_builtinValueMap[Hash::CalcHash(name.GetBegin(), name.GetLength())].SetFloat(value); }
void Material::SetBuiltinVectorParameter(const StringRef& name, const Vector4& value) { m_builtinValueMap[Hash::CalcHash(name.GetBegin(), name.GetLength())].SetVector(value); }
void Material::SetBuiltinMatrixParameter(const StringRef& name, const Matrix& value) { m_builtinValueMap[Hash::CalcHash(name.GetBegin(), name.GetLength())].SetMatrix(value); }
void Material::SetBuiltinTextureParameter(const StringRef& name, Texture* value) { m_builtinValueMap[Hash::CalcHash(name.GetBegin(), name.GetLength())].SetManagedTexture(value); }
void Material::SetBuiltinColorParameter(const StringRef& name, const Color& value) { SetBuiltinVectorParameter(name, value); }
void Material::SetBuiltinColorParameter(const StringRef& name, float r, float g, float b, float a) { SetBuiltinVectorParameter(name, Color(r, g, b, a)); }

//------------------------------------------------------------------------------
RefPtr<Material> Material::CopyShared() const
{
	auto m = RefPtr<Material>::MakeRef();
	m->Initialize();	// TODO: base
	m->m_shader = m_shader;
	//m->m_shader = m_shader;
	////m->m_valueList = m_valueList;
	////m->m_linkedVariableList = m_linkedVariableList;

	//m->m_blendMode = m_blendMode;
	//m->m_culling = m_culling;
	//m->m_fill = m_fill;
	//m->m_alphaTest = m_alphaTest;
	//m->m_depthTestEnabled = m_depthTestEnabled;
	//m->m_depthWriteEnabled = m_depthWriteEnabled;

	// 遅延評価のための変更フラグはそのまま維持。コピー時は評価しない。
	// コピー先が本当に必要になったタイミングで評価されるようにする。
	//m->m_shaderModified = m_shaderModified;

	//m->m_modifiedForMaterialInstance = m_modifiedForMaterialInstance;

	m->m_revisionCount = m_revisionCount;
	return m;
}

////------------------------------------------------------------------------------
//void Material::ResolveCombinedMaterial()
//{
//	m_combinedMaterial->Combine(nullptr, this, nullptr);	// TODO: parent and base
//}
//
////------------------------------------------------------------------------------
//detail::CombinedMaterial* Material::GetCombinedMaterial() const
//{
//	return m_combinedMaterial;
//}

//------------------------------------------------------------------------------
void Material::SetShader(Shader* shader)
{
	if (m_shader != shader)
	{
		m_shader = shader;
		m_revisionCount++;
	}
}

//------------------------------------------------------------------------------
Shader* Material::GetShader() const
{
	return m_shader;
}

//------------------------------------------------------------------------------
void Material::SetIntParameter(const StringRef& name, int value)
{
	uint32_t hashKey = Hash::CalcHash(name.GetBegin(), name.GetLength());
	FindAndCreateUserShaderValue(hashKey)->SetInt(value);
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::SetFloatParameter(const StringRef& name, float value)
{
	uint32_t hashKey = Hash::CalcHash(name.GetBegin(), name.GetLength());
	FindAndCreateUserShaderValue(hashKey)->SetFloat(value);
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::SetVectorParameter(const StringRef& name, const Vector4& value)
{
	uint32_t hashKey = Hash::CalcHash(name.GetBegin(), name.GetLength());
	FindAndCreateUserShaderValue(hashKey)->SetVector(value);
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::SetMatrixParameter(const StringRef& name, const Matrix& value)
{
	uint32_t hashKey = Hash::CalcHash(name.GetBegin(), name.GetLength());
	FindAndCreateUserShaderValue(hashKey)->SetMatrix(value);
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::SetTextureParameter(const StringRef& name, Texture* value)
{
	uint32_t hashKey = Hash::CalcHash(name.GetBegin(), name.GetLength());
	FindAndCreateUserShaderValue(hashKey)->SetManagedTexture(value);
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::SetColorParameter(const StringRef& name, const Color& value)
{
	SetVectorParameter(name, value);
}

//------------------------------------------------------------------------------
void Material::SetColorParameter(const StringRef& name, float r, float g, float b, float a)
{
	SetColorParameter(name, Color(r, g, b, a));
}

//------------------------------------------------------------------------------
void Material::SetBlendMode(Nullable<BlendMode> mode)
{
	blendMode = mode;
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::SetCullingMode(Nullable<CullingMode> mode)
{
	cullingMode = mode;
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::SetDepthTestEnabled(Nullable<bool> enabled)
{
	depthTestEnabled = enabled;
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::SetDepthWriteEnabled(Nullable<bool> enabled)
{
	depthWriteEnabled = enabled;
	m_revisionCount++;
}

////------------------------------------------------------------------------------
//void Material::SetBlendMode(BlendMode mode)
//{
//	m_builtin.blendMode = mode;
//	m_revisionCount++;
//}
//
////------------------------------------------------------------------------------
//void Material::SetCullingMode(CullingMode mode)
//{
//	m_builtin.cullingMode = mode;
//	m_revisionCount++;
//}
//
////------------------------------------------------------------------------------
//void Material::SetDepthTestEnabled(bool enabled)
//{
//	m_builtin.depthTestEnabled = enabled;
//	m_revisionCount++;
//}
//
////------------------------------------------------------------------------------
//void Material::SetDepthWriteEnabled(bool enabled)
//{
//	m_builtin.depthWriteEnabled = enabled;
//	m_revisionCount++;
//}

////------------------------------------------------------------------------------
//void Material::SetFillMode(FillMode mode)
//{
//}
//
////------------------------------------------------------------------------------
//void Material::SetAlphaTestEnabled(bool enabled)
//{
//}

//------------------------------------------------------------------------------
ShaderValue* Material::FindAndCreateUserShaderValue(uint32_t hashKey)
{
	auto itr = m_userValueMap.find(hashKey);
	if (itr != m_userValueMap.end())
	{
		return &(itr->second);
	}

	ShaderValue& value = m_userValueMap[hashKey];
	return &value;
}

//------------------------------------------------------------------------------
const ShaderValue* Material::FindUserShaderValueConst(uint32_t hashKey) const
{
	auto itr = m_userValueMap.find(hashKey);
	if (itr != m_userValueMap.end())
		return &(itr->second);
	return nullptr;
}

//------------------------------------------------------------------------------
uint32_t Material::GetHashCode()
{
	if (m_revisionCount != 0)
	{
		m_hashCode = 0;
		m_revisionCount = 0;

		// TODO: map じゃなくて List で思う。20個も普通値いれないだろうし
		for (auto& pair : m_userValueMap)
			m_hashCode += pair.second.GetHashCode();

		for (auto& pair : m_builtinValueMap)
			m_hashCode += pair.second.GetHashCode();

		//m_hashCode += Hash::CalcHash(reinterpret_cast<const char*>(&m_builtin), sizeof(m_builtin));

		
		m_hashCode += reinterpret_cast<intptr_t>(m_shader.Get());
		m_hashCode += blendMode.GetHashCode();
		m_hashCode += cullingMode.GetHashCode();
		m_hashCode += depthTestEnabled.GetHashCode();
		m_hashCode += depthWriteEnabled.GetHashCode();
	}

	return m_hashCode;
}

//------------------------------------------------------------------------------
void Material::OnRenderStateChanged(Object* obj)
{
	static_cast<Material*>(obj)->m_revisionCount++;
}

//------------------------------------------------------------------------------
static const Color DefaultColorScale(1, 1, 1, 1);
static const Color DefaultBlendColor(0, 0, 0, 0);
static const ToneF DefaultTone(0, 0, 0, 0);
static const uint32_t MaterialTextureHash = Hash::CalcHash(_T("MaterialTexture"));
static const uint32_t OpacityHash = Hash::CalcHash(_T("Opacity"));
static const uint32_t ColorScaleHash = Hash::CalcHash(_T("ColorScale"));
static const uint32_t BlendColorHash = Hash::CalcHash(_T("BlendColor"));
static const uint32_t ToneHash = Hash::CalcHash(_T("Tone"));
static const uint32_t DiffuseHash = Hash::CalcHash(_T("Diffuse"));
static const uint32_t AmbientHash = Hash::CalcHash(_T("Ambient"));
static const uint32_t SpecularHash = Hash::CalcHash(_T("Specular"));
static const uint32_t EmissiveHash = Hash::CalcHash(_T("Emissive"));
static const uint32_t PowerHash = Hash::CalcHash(_T("Power"));

void Material::SetMaterialTexture(Texture* v)
{
	m_builtinValueMap[MaterialTextureHash].SetManagedTexture(v);
	m_revisionCount++;
}
Texture* Material::GetMaterialTexture(Texture* defaultValue) const { auto itr = m_builtinValueMap.find(MaterialTextureHash); return (itr != m_builtinValueMap.end()) ? itr->second.GetManagedTexture() : defaultValue; }

//void Material::SetOpacity(float v)
//{
//	m_builtinValueMap[OpacityHash].SetFloat(v);
//	m_revisionCount++;
//}
//float Material::GetOpacity() const { auto itr = m_builtinValueMap.find(OpacityHash); return (itr != m_builtinValueMap.end()) ? itr->second.GetFloat() : 1.0f; }
//
//void Material::SetColorScale(const Color& v)
//{
//	m_builtinValueMap[ColorScaleHash].SetVector(v);
//	m_revisionCount++;
//}
//Color Material::GetColorScale() const { auto itr = m_builtinValueMap.find(ColorScaleHash); return (itr != m_builtinValueMap.end()) ? Color(itr->second.GetVector()) : DefaultColorScale; }
//
//void Material::SetBlendColor(const Color& v)
//{
//	m_builtinValueMap[BlendColorHash].SetVector(v);
//	m_revisionCount++;
//}
//Color Material::GetBlendColor() const { auto itr = m_builtinValueMap.find(BlendColorHash); return (itr != m_builtinValueMap.end()) ? Color(itr->second.GetVector()) : DefaultBlendColor; }
//
//void Material::SetTone(const ToneF& v)
//{
//	m_builtinValueMap[ToneHash].SetVector(v);
//	m_revisionCount++;
//}
//ToneF Material::GetTone() const { auto itr = m_builtinValueMap.find(ToneHash); return (itr != m_builtinValueMap.end()) ? ToneF(itr->second.GetVector()) : DefaultTone; }
//

//==============================================================================
// DiffuseMaterial
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(DiffuseMaterial, Material);

//------------------------------------------------------------------------------
DiffuseMaterialPtr DiffuseMaterial::Create()
{
	auto ptr = DiffuseMaterialPtr::MakeRef();
	ptr->Initialize();
	return ptr;
}

//------------------------------------------------------------------------------
DiffuseMaterial::DiffuseMaterial()
{
}

//------------------------------------------------------------------------------
DiffuseMaterial::~DiffuseMaterial()
{
}

//------------------------------------------------------------------------------
void DiffuseMaterial::Initialize()
{
	Material::Initialize();
}

//------------------------------------------------------------------------------
void DiffuseMaterial::SetDiffuse(const Color& value)
{
	SetBuiltinColorParameter(Material::DiffuseParameter, value);
}

//------------------------------------------------------------------------------
void DiffuseMaterial::SetAmbient(const Color& value)
{
	SetBuiltinColorParameter(Material::AmbientParameter, value);
}

//------------------------------------------------------------------------------
void DiffuseMaterial::SetSpecular(const Color& value)
{
	SetBuiltinColorParameter(Material::SpecularParameter, value);
}

//------------------------------------------------------------------------------
void DiffuseMaterial::SetEmissive(const Color& value)
{
	SetBuiltinColorParameter(Material::EmissiveParameter, value);
}

//------------------------------------------------------------------------------
void DiffuseMaterial::SetSpecularPower(float value)
{
	SetBuiltinFloatParameter(Material::PowerParameter, value);
}


namespace detail {

//==============================================================================
// CombinedMaterial
//==============================================================================
/*
	ユーザー指定のシェーダ変数値の保持は普通の動的配列にしている。
	検索でのアクセスがメインだが、20個以下なら unorderd_map や SortedList よりも速い。
	実際にユーザー指定のシェーダ変数が 20 を超えることも稀だろう。
*/

//------------------------------------------------------------------------------
CombinedMaterial::CombinedMaterial()
	: m_shader(nullptr)
	, m_colorScale(Color::White)
	, m_blendColor(Color(0, 0, 0, 1))
	, m_tone()
	, m_diffuse(Material::DefaultDiffuse)
	, m_ambient(Material::DefaultAmbient)
	, m_specular(Material::DefaultSpecular)
	, m_emissive(Material::DefaultEmissive)
	, m_power(Material::DefaultPower)
	, m_mainTexture(nullptr)
	//, m_culling(CullingMode::Back)

	//, m_lastSourceMaterial(nullptr)
	, m_lastSourceHashCode(0)
{
}

//------------------------------------------------------------------------------
CombinedMaterial::~CombinedMaterial()
{
}

//------------------------------------------------------------------------------
void CombinedMaterial::Combine(Material* owner, Material* ownerBase, const BuiltinEffectData& builtinEffectData)
{
	//bool modified = false;
	//if (owner == nullptr || owner != owner || owner->m_modifiedForMaterialInstance)
	//{
	//	modified = true;
	//}
	//if (parent != nullptr && parent->m_modifiedForMaterialInstance)
	//{
	//	modified = true;
	//}

	uint32_t hashCode = 0;
	if (owner != nullptr)		hashCode |= owner->GetHashCode();
	if (ownerBase != nullptr)	hashCode |= ownerBase->GetHashCode();

	if (m_lastSourceHashCode != hashCode ||
		m_lastBuiltinEffectHashCode != builtinEffectData.GetHashCode())
	{
		Material* source1 = (ownerBase != nullptr) ? ownerBase : owner;
		Material* source2 = (ownerBase != nullptr) ? owner : nullptr;

		// source1
		m_shader = source1->m_shader;
		m_colorScale = builtinEffectData.GetColorScale();
		m_colorScale.a *= builtinEffectData.GetOpacity();
		m_blendColor = builtinEffectData.GetBlendColor();
		m_tone = builtinEffectData.GetTone();
		CopyUserValueTable(source1);
		// TODO
		m_blendMode = source1->GetBlendMode();
		m_cullingMode = source1->GetCullingMode();
		m_depthTestEnabled = source1->IsDepthTestEnabled();
		m_depthWriteEnabled = source1->IsDepthWriteEnabled();

		// source2 (base があるなら owner を後からマージ)
		if (source2 != nullptr)
		{
			if (m_shader == nullptr) m_shader = source2->GetShader();
			//m_colorScale.MultiplyClamp(source2->GetColorScale());
			//m_colorScale.a *= source2->GetOpacity();
			//m_blendColor.AddClamp(source2->GetBlendColor());
			//m_tone.AddClamp(source2->GetTone());
			MergeUserValueTable(source2);
		}

		// parent
		//if (parent != nullptr)
		//{
		//	if (m_shader == nullptr) m_shader = parent->GetShader();
		//	m_colorScale.MultiplyClamp(parent->GetColorScale());
		//	m_colorScale.a *= parent->GetOpacity();
		//	m_blendColor.AddClamp(parent->GetBlendColor());
		//	m_tone.AddClamp(parent->GetTone());
		//}

		// Material params (from base only. not parent inherit)
		// TODO: 文字列検索とかしまくっている。いろいろ最適化の余地ある
		if (source2 != nullptr)
		{
			m_diffuse = source1->GetBuiltinColor(DiffuseHash, source2->GetBuiltinColor(DiffuseHash, Material::DefaultDiffuse));
			m_ambient = source1->GetBuiltinColor(AmbientHash, source2->GetBuiltinColor(AmbientHash, Material::DefaultAmbient));
			m_specular = source1->GetBuiltinColor(SpecularHash, source2->GetBuiltinColor(SpecularHash, Material::DefaultSpecular));
			m_emissive = source1->GetBuiltinColor(EmissiveHash, source2->GetBuiltinColor(EmissiveHash, Material::DefaultEmissive));
			m_power = source1->GetBuiltinFloat(PowerHash, source2->GetBuiltinFloat(PowerHash, Material::DefaultPower));
			m_mainTexture = source1->GetMaterialTexture(source2->GetMaterialTexture(nullptr));
		}
		else
		{
			m_diffuse = source1->GetBuiltinColor(DiffuseHash, Material::DefaultDiffuse);
			m_ambient = source1->GetBuiltinColor(AmbientHash, Material::DefaultAmbient);
			m_specular = source1->GetBuiltinColor(SpecularHash, Material::DefaultSpecular);
			m_emissive = source1->GetBuiltinColor(EmissiveHash, Material::DefaultEmissive);
			m_power = source1->GetBuiltinFloat(PowerHash, Material::DefaultPower);
			m_mainTexture = source1->GetMaterialTexture(nullptr);
		}

		if (m_shader == nullptr)
		{
			m_shader = builtinEffectData.GetShader();
		}

		m_lastSourceHashCode = hashCode;
		m_lastBuiltinEffectHashCode = builtinEffectData.GetHashCode();
	}
}

//------------------------------------------------------------------------------
void CombinedMaterial::ApplyUserShaderValeues(Shader* targetShader)
{
	for (auto& pair : m_userValueTable)
	{
		targetShader->SetShaderValue(pair.nameHash, pair.value);
	}
}

//------------------------------------------------------------------------------
void CombinedMaterial::CopyUserValueTable(Material* source)
{
	assert(source != nullptr);
	m_userValueTable.Clear();
	for (auto& pair : source->GetUserValueMap())
	{
		m_userValueTable.Add(ValuePair{ pair.first, pair.second });
	}
}

//------------------------------------------------------------------------------
void CombinedMaterial::MergeUserValueTable(Material* source)
{
	assert(source != nullptr);
	for (auto& pair : source->GetUserValueMap())
	{
		uint32_t key = pair.first;
		ValuePair* p = m_userValueTable.Find([key](const ValuePair& p) { return p.nameHash == key; });
		if (p != nullptr)
		{
			p->value = pair.second;
		}
		else
		{
			m_userValueTable.Add(ValuePair{ pair.first, pair.second });
		}
	}
}

} // namespace detail
LN_NAMESPACE_END