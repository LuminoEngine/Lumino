
#include "Internal.h"
#include <Lumino/Graphics/Material.h>

LN_NAMESPACE_BEGIN


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
	: m_shader(nullptr)
	, m_valueList()
	, m_blendMode(BlendMode::Normal)
	, m_culling(CullingMode::Back)
	, m_fill(FillMode_Solid)
	, m_alphaTest(true)
	, m_depthTestEnabled(true)
	, m_depthWriteEnabled(true)
	, m_shaderModified(false)
	, m_modifiedForMaterialInstance(false)
{
}

//------------------------------------------------------------------------------
Material::~Material()
{
}

//------------------------------------------------------------------------------
void Material::Initialize()
{
	m_combinedMaterial = RefPtr<detail::CombinedMaterial>::MakeRef();
}

//------------------------------------------------------------------------------
RefPtr<Material> Material::CopyShared() const
{
	auto m = RefPtr<Material>::MakeRef();
	m->Initialize();	// TODO: base
	m->m_shader = m_shader;
	m->m_valueList = m_valueList;
	m->m_linkedVariableList = m_linkedVariableList;

	m->m_blendMode = m_blendMode;
	m->m_culling = m_culling;
	m->m_fill = m_fill;
	m->m_alphaTest = m_alphaTest;
	m->m_depthTestEnabled = m_depthTestEnabled;
	m->m_depthWriteEnabled = m_depthWriteEnabled;

	// 遅延評価のための変更フラグはそのまま維持。コピー時は評価しない。
	// コピー先が本当に必要になったタイミングで評価されるようにする。
	m->m_shaderModified = m_shaderModified;

	m->m_modifiedForMaterialInstance = m_modifiedForMaterialInstance;

	return m;
}

//------------------------------------------------------------------------------
void Material::ResolveCombinedMaterial()
{
	m_combinedMaterial->Combine(nullptr, this, nullptr);	// TODO: parent and base
}

//------------------------------------------------------------------------------
detail::CombinedMaterial* Material::GetCombinedMaterial() const
{
	return m_combinedMaterial;
}

//------------------------------------------------------------------------------
void Material::SetShader(Shader* shader)
{
	m_shader = shader;
	m_shaderModified = true;
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material::SetIntParameter(const StringRef& name, int value)
{
	FindShaderValue(name)->SetInt(value);
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material::SetFloatParameter(const StringRef& name, float value)
{
	FindShaderValue(name)->SetFloat(value);
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material::SetVectorParameter(const StringRef& name, const Vector4& value)
{
	FindShaderValue(name)->SetVector(value);
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material::SetMatrixParameter(const StringRef& name, const Matrix& value)
{
	FindShaderValue(name)->SetMatrix(value);
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material::SetTextureParameter(const StringRef& name, Texture* value)
{
	FindShaderValue(name)->SetManagedTexture(value);
	m_modifiedForMaterialInstance = true;
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
void Material::SetBlendMode(BlendMode mode)
{
}

//------------------------------------------------------------------------------
void Material::SetCullingMode(CullingMode mode)
{
}

//------------------------------------------------------------------------------
void Material::SetFillMode(FillMode mode)
{
}

//------------------------------------------------------------------------------
void Material::SetAlphaTestEnabled(bool enabled)
{
}

//------------------------------------------------------------------------------
void Material::LinkVariables()
{
	m_linkedVariableList.Clear();

	if (m_shader != nullptr)
	{
		for (ShaderVariable* v : m_shader->GetVariables())
		{
			if (v->GetType() == ShaderVariableType_Unknown ||
				v->GetType() == ShaderVariableType_String)
			{
				// Unknown と String 型は無視。String 型は読み取り専用で、Material としては持っておく必要ない。
			}
			else
			{
				// このマテリアルの値として、シェーダ変数値を保持する変数を作る
				ShaderValuePtr valuePtr;
				if (!m_valueList.TryGetValue(v->GetName(), &valuePtr))
				{
					valuePtr = std::make_shared<ShaderValue>(v->GetShaderValue());	// 初期値
					m_valueList.Add(v->GetName(), valuePtr);
				}

				// 変数と値のペア
				ValuePair pair = { v, valuePtr };
				m_linkedVariableList.Add(pair);
			}
		}
	}

	// TODO: シェーダが変更されたことで不要となるものを m_valueList から除外したほうがいいかも
}

//------------------------------------------------------------------------------
ShaderValue* Material::FindShaderValue(const StringRef& name)
{
	ShaderValuePtr v;
	if (!m_valueList.TryGetValue(name, &v))
	{
		v = std::make_shared<ShaderValue>();
		m_valueList.Add(name, v);
	}
	return v.get();
}

//------------------------------------------------------------------------------
ShaderValue* Material::FindShaderValueConst(const StringRef& name) const
{
	ShaderValuePtr v;
	if (!m_valueList.TryGetValue(name, &v))
	{
		return nullptr;
	}
	return v.get();
}

//------------------------------------------------------------------------------
void Material::ApplyToShaderVariables()
{
	if (m_shaderModified)
	{
		LinkVariables();
		m_shaderModified = false;
	}

	for (auto& pair : m_linkedVariableList)
	{
		pair.variable->SetShaderValue(*pair.value);
	}
}

//------------------------------------------------------------------------------
static const Color DefaultColorScale(1, 1, 1, 1);
static const Color DefaultBlendColor(0, 0, 0, 0);
static const ToneF DefaultTone(0, 0, 0, 0);
static const uint32_t MaterialTextureHash = Hash::CalcHash("MaterialTexture");
static const uint32_t OpacityHash = Hash::CalcHash("Opacity");
static const uint32_t ColorScaleHash = Hash::CalcHash("ColorScale");
static const uint32_t BlendColorHash = Hash::CalcHash("BlendColor");
static const uint32_t ToneHash = Hash::CalcHash("Tone");

void Material::SetMaterialTexture(Texture* v) { m_builtinValueMap[MaterialTextureHash].SetManagedTexture(v); }
Texture* Material::GetMaterialTexture() const { auto itr = m_builtinValueMap.find(MaterialTextureHash); return (itr != m_builtinValueMap.end()) ? itr->second.GetManagedTexture() : nullptr; }

void Material::SetOpacity(float v) { m_builtinValueMap[OpacityHash].SetFloat(v); }
float Material::GetOpacity() const { auto itr = m_builtinValueMap.find(OpacityHash); return (itr != m_builtinValueMap.end()) ? itr->second.GetFloat() : 1.0f; }

void Material::SetColorScale(const Color& v) { m_builtinValueMap[ColorScaleHash].SetVector(v); }
Color Material::GetColorScale() const { auto itr = m_builtinValueMap.find(ColorScaleHash); return (itr != m_builtinValueMap.end()) ? Color(itr->second.GetVector()) : DefaultColorScale; }

void Material::SetBlendColor(const Color& v) { m_builtinValueMap[BlendColorHash].SetVector(v); }
Color Material::GetBlendColor() const { auto itr = m_builtinValueMap.find(BlendColorHash); return (itr != m_builtinValueMap.end()) ? Color(itr->second.GetVector()) : DefaultBlendColor; }

void Material::SetTone(const ToneF& v) { m_builtinValueMap[ToneHash].SetVector(v); }
ToneF Material::GetTone() const { auto itr = m_builtinValueMap.find(ToneHash); return (itr != m_builtinValueMap.end()) ? ToneF(itr->second.GetVector()) : DefaultTone; }


namespace detail {

//==============================================================================
// MaterialList
//==============================================================================

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
{
}

//------------------------------------------------------------------------------
CombinedMaterial::~CombinedMaterial()
{
}

//------------------------------------------------------------------------------
void CombinedMaterial::Combine(Material* parent, Material* owner, Material* ownerBase)
{
	bool modified = false;
	if (owner == nullptr || owner != owner || owner->m_modifiedForMaterialInstance)
	{
		modified = true;
	}
	if (parent != nullptr && parent->m_modifiedForMaterialInstance)
	{
		modified = true;
	}

	if (modified)
	{
		Material* source1 = (ownerBase != nullptr) ? ownerBase : owner;
		Material* source2 = (ownerBase != nullptr) ? owner : nullptr;

		// source1
		m_shader = source1->GetShader();
		m_colorScale = source1->GetColorScale();
		m_colorScale.a *= source1->GetOpacity();
		m_blendColor = source1->GetBlendColor();
		m_tone = source1->GetTone();

		// source2
		if (source2 != nullptr)
		{
			if (m_shader == nullptr) m_shader = source2->GetShader();
			m_colorScale.MultiplyClamp(source2->GetColorScale());
			m_colorScale.a *= source2->GetOpacity();
			m_blendColor.AddClamp(source2->GetBlendColor());
			m_tone.AddClamp(source2->GetTone());
		}

		// parent
		if (parent != nullptr)
		{
			if (m_shader == nullptr) m_shader = parent->GetShader();
			m_colorScale.MultiplyClamp(parent->GetColorScale());
			m_colorScale.a *= parent->GetOpacity();
			m_blendColor.AddClamp(parent->GetBlendColor());
			m_tone.AddClamp(parent->GetTone());
		}

		// Material params (from base only. not parent inherit)
		// TODO: 文字列検索とかしまくっている。いろいろ最適化の余地ある
		if (source2 != nullptr)
		{
			m_diffuse = source1->GetColor(_T("Diffuse"), source2->GetColor(_T("Diffuse"), Material::DefaultDiffuse));
			m_ambient = source1->GetColor(_T("Ambient"), source2->GetColor(_T("Ambient"), Material::DefaultAmbient));
			m_specular = source1->GetColor(_T("Specular"), source2->GetColor(_T("Specular"), Material::DefaultSpecular));
			m_emissive = source1->GetColor(_T("Emissive"), source2->GetColor(_T("Emissive"), Material::DefaultEmissive));
			m_power = source1->GetFloat(_T("Power"), source2->GetFloat(_T("Power"), Material::DefaultPower));
			m_mainTexture = source1->GetTexture(Material::MaterialTextureParameter, source2->GetTexture(Material::MaterialTextureParameter, nullptr));
		}
		else
		{
			m_diffuse = source1->GetColor(_T("Diffuse"), Material::DefaultDiffuse);
			m_ambient = source1->GetColor(_T("Ambient"), Material::DefaultAmbient);
			m_specular = source1->GetColor(_T("Specular"), Material::DefaultSpecular);
			m_emissive = source1->GetColor(_T("Emissive"), Material::DefaultEmissive);
			m_power = source1->GetFloat(_T("Power"), Material::DefaultPower);
			m_mainTexture = source1->GetTexture(Material::MaterialTextureParameter, nullptr);
		}

		owner->m_modifiedForMaterialInstance = false;
	}
}

} // namespace detail
LN_NAMESPACE_END