
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
// CommonMaterial
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(CommonMaterial, Object);
//LN_TR_PROPERTY_IMPLEMENT(CommonMaterial, BlendMode, blendMode, tr::PropertyMetadata(CommonMaterial::onRenderStateChanged));
//LN_TR_PROPERTY_IMPLEMENT(CommonMaterial, CullingMode, cullingMode, tr::PropertyMetadata(CommonMaterial::onRenderStateChanged));
//LN_TR_PROPERTY_IMPLEMENT(CommonMaterial, bool, depthTestEnabled, tr::PropertyMetadata(CommonMaterial::onRenderStateChanged));
//LN_TR_PROPERTY_IMPLEMENT(CommonMaterial, bool, depthWriteEnabled, tr::PropertyMetadata(CommonMaterial::onRenderStateChanged));

const String CommonMaterial::DiffuseParameter(_LT("Diffuse"));
const String CommonMaterial::AmbientParameter(_LT("Ambient"));
const String CommonMaterial::SpecularParameter(_LT("Specular"));
const String CommonMaterial::EmissiveParameter(_LT("Emissive"));
const String CommonMaterial::PowerParameter(_LT("Power"));
const String CommonMaterial::MaterialTextureParameter(_LT("MaterialTexture"));

const Color CommonMaterial::DefaultDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
const Color CommonMaterial::DefaultAmbient(0.0f, 0.0f, 0.0f, 0.0f);
const Color CommonMaterial::DefaultSpecular(0.5f, 0.5f, 0.5f, 0.5f);
const Color CommonMaterial::DefaultEmissive(0.0f, 0.0f, 0.0f, 0.0f);
const float CommonMaterial::DefaultPower = 50.0f;

//------------------------------------------------------------------------------
MaterialPtr CommonMaterial::create()
{
	auto ptr = MaterialPtr::makeRef();
	ptr->initialize();
	return ptr;
}

//------------------------------------------------------------------------------
CommonMaterial::CommonMaterial()
	: blendMode(nullptr)
	, cullingMode(nullptr)
	, depthTestEnabled(nullptr)
	, depthWriteEnabled(nullptr)
	, m_revisionCount(0)
{
	initializeProperties();
	reset();
}

//------------------------------------------------------------------------------
CommonMaterial::~CommonMaterial()
{
}

//------------------------------------------------------------------------------
void CommonMaterial::initialize()
{
	//m_combinedMaterial = Ref<detail::CombinedMaterial>::MakeRef();
}

//------------------------------------------------------------------------------
void CommonMaterial::reset()
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
void CommonMaterial::setBuiltinIntParameter(const StringRef& name, int value) { m_builtinValueMap[Hash::calcHash(name.getBegin(), name.getLength())].setInt(value); }
void CommonMaterial::setBuiltinFloatParameter(const StringRef& name, float value) { m_builtinValueMap[Hash::calcHash(name.getBegin(), name.getLength())].setFloat(value); }
void CommonMaterial::setBuiltinVectorParameter(const StringRef& name, const Vector4& value) { m_builtinValueMap[Hash::calcHash(name.getBegin(), name.getLength())].setVector(value); }
void CommonMaterial::setBuiltinMatrixParameter(const StringRef& name, const Matrix& value) { m_builtinValueMap[Hash::calcHash(name.getBegin(), name.getLength())].setMatrix(value); }
void CommonMaterial::setBuiltinTextureParameter(const StringRef& name, Texture* value) { m_builtinValueMap[Hash::calcHash(name.getBegin(), name.getLength())].setManagedTexture(value); }
void CommonMaterial::setBuiltinColorParameter(const StringRef& name, const Color& value) { setBuiltinVectorParameter(name, value); }
void CommonMaterial::setBuiltinColorParameter(const StringRef& name, float r, float g, float b, float a) { setBuiltinVectorParameter(name, Color(r, g, b, a)); }

//------------------------------------------------------------------------------
Ref<CommonMaterial> CommonMaterial::copyShared() const
{
	auto m = Ref<CommonMaterial>::makeRef();
	m->initialize();	// TODO: base
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
//void CommonMaterial::ResolveCombinedMaterial()
//{
//	m_combinedMaterial->Combine(nullptr, this, nullptr);	// TODO: parent and base
//}
//
////------------------------------------------------------------------------------
//detail::CombinedMaterial* CommonMaterial::getCombinedMaterial() const
//{
//	return m_combinedMaterial;
//}

//------------------------------------------------------------------------------
void CommonMaterial::setShader(Shader* shader)
{
	if (m_shader != shader)
	{
		m_shader = shader;
		m_revisionCount++;
	}
}

//------------------------------------------------------------------------------
Shader* CommonMaterial::getShader() const
{
	return m_shader;
}

//------------------------------------------------------------------------------
void CommonMaterial::setIntParameter(const StringRef& name, int value)
{
	uint32_t hashKey = Hash::calcHash(name.getBegin(), name.getLength());
	findAndCreateUserShaderValue(hashKey)->setInt(value);
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void CommonMaterial::setFloatParameter(const StringRef& name, float value)
{
	uint32_t hashKey = Hash::calcHash(name.getBegin(), name.getLength());
	findAndCreateUserShaderValue(hashKey)->setFloat(value);
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void CommonMaterial::setVectorParameter(const StringRef& name, const Vector4& value)
{
	uint32_t hashKey = Hash::calcHash(name.getBegin(), name.getLength());
	findAndCreateUserShaderValue(hashKey)->setVector(value);
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void CommonMaterial::setMatrixParameter(const StringRef& name, const Matrix& value)
{
	uint32_t hashKey = Hash::calcHash(name.getBegin(), name.getLength());
	findAndCreateUserShaderValue(hashKey)->setMatrix(value);
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void CommonMaterial::setTextureParameter(const StringRef& name, Texture* value)
{
	uint32_t hashKey = Hash::calcHash(name.getBegin(), name.getLength());
	findAndCreateUserShaderValue(hashKey)->setManagedTexture(value);
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void CommonMaterial::setColorParameter(const StringRef& name, const Color& value)
{
	setVectorParameter(name, value);
}

//------------------------------------------------------------------------------
void CommonMaterial::setColorParameter(const StringRef& name, float r, float g, float b, float a)
{
	setColorParameter(name, Color(r, g, b, a));
}

//------------------------------------------------------------------------------
void CommonMaterial::setBlendMode(Nullable<BlendMode> mode)
{
	blendMode = mode;
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void CommonMaterial::setCullingMode(Nullable<CullingMode> mode)
{
	cullingMode = mode;
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void CommonMaterial::setDepthTestEnabled(Nullable<bool> enabled)
{
	depthTestEnabled = enabled;
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void CommonMaterial::setDepthWriteEnabled(Nullable<bool> enabled)
{
	depthWriteEnabled = enabled;
	m_revisionCount++;
}

////------------------------------------------------------------------------------
//void CommonMaterial::setBlendMode(BlendMode mode)
//{
//	m_builtin.blendMode = mode;
//	m_revisionCount++;
//}
//
////------------------------------------------------------------------------------
//void CommonMaterial::setCullingMode(CullingMode mode)
//{
//	m_builtin.cullingMode = mode;
//	m_revisionCount++;
//}
//
////------------------------------------------------------------------------------
//void CommonMaterial::setDepthTestEnabled(bool enabled)
//{
//	m_builtin.depthTestEnabled = enabled;
//	m_revisionCount++;
//}
//
////------------------------------------------------------------------------------
//void CommonMaterial::setDepthWriteEnabled(bool enabled)
//{
//	m_builtin.depthWriteEnabled = enabled;
//	m_revisionCount++;
//}

////------------------------------------------------------------------------------
//void CommonMaterial::SetFillMode(FillMode mode)
//{
//}
//
////------------------------------------------------------------------------------
//void CommonMaterial::SetAlphaTestEnabled(bool enabled)
//{
//}

//------------------------------------------------------------------------------
ShaderValue* CommonMaterial::findAndCreateUserShaderValue(uint32_t hashKey)
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
const ShaderValue* CommonMaterial::findUserShaderValueConst(uint32_t hashKey) const
{
	auto itr = m_userValueMap.find(hashKey);
	if (itr != m_userValueMap.end())
		return &(itr->second);
	return nullptr;
}

//------------------------------------------------------------------------------
uint32_t CommonMaterial::getHashCode()
{
	if (m_revisionCount != 0)
	{
		m_hashCode = 0;
		m_revisionCount = 0;

		// TODO: map じゃなくて List で思う。20個も普通値いれないだろうし
		for (auto& pair : m_userValueMap)
			m_hashCode += pair.second.getHashCode();

		for (auto& pair : m_builtinValueMap)
			m_hashCode += pair.second.getHashCode();

		//m_hashCode += Hash::CalcHash(reinterpret_cast<const char*>(&m_builtin), sizeof(m_builtin));

		
		m_hashCode += reinterpret_cast<intptr_t>(m_shader.get());
		m_hashCode += blendMode.getHashCode();
		m_hashCode += cullingMode.getHashCode();
		m_hashCode += depthTestEnabled.getHashCode();
		m_hashCode += depthWriteEnabled.getHashCode();
	}

	return m_hashCode;
}

//------------------------------------------------------------------------------
void CommonMaterial::onRenderStateChanged(Object* obj)
{
	static_cast<CommonMaterial*>(obj)->m_revisionCount++;
}

//------------------------------------------------------------------------------
static const Color DefaultColorScale(1, 1, 1, 1);
static const Color DefaultBlendColor(0, 0, 0, 0);
static const ToneF DefaultTone(0, 0, 0, 0);
static const uint32_t MaterialTextureHash = Hash::calcHash(_LT("MaterialTexture"));
static const uint32_t OpacityHash = Hash::calcHash(_LT("Opacity"));
static const uint32_t ColorScaleHash = Hash::calcHash(_LT("ColorScale"));
static const uint32_t BlendColorHash = Hash::calcHash(_LT("BlendColor"));
static const uint32_t ToneHash = Hash::calcHash(_LT("Tone"));
static const uint32_t DiffuseHash = Hash::calcHash(_LT("Diffuse"));
static const uint32_t AmbientHash = Hash::calcHash(_LT("Ambient"));
static const uint32_t SpecularHash = Hash::calcHash(_LT("Specular"));
static const uint32_t EmissiveHash = Hash::calcHash(_LT("Emissive"));
static const uint32_t PowerHash = Hash::calcHash(_LT("Power"));

void CommonMaterial::setMaterialTexture(Texture* v)
{
	m_builtinValueMap[MaterialTextureHash].setManagedTexture(v);
	m_revisionCount++;
}
Texture* CommonMaterial::getMaterialTexture(Texture* defaultValue) const { auto itr = m_builtinValueMap.find(MaterialTextureHash); return (itr != m_builtinValueMap.end()) ? itr->second.getManagedTexture() : defaultValue; }

//void CommonMaterial::setOpacity(float v)
//{
//	m_builtinValueMap[OpacityHash].SetFloat(v);
//	m_revisionCount++;
//}
//float CommonMaterial::getOpacity() const { auto itr = m_builtinValueMap.find(OpacityHash); return (itr != m_builtinValueMap.end()) ? itr->second.GetFloat() : 1.0f; }
//
//void CommonMaterial::setColorScale(const Color& v)
//{
//	m_builtinValueMap[ColorScaleHash].setVector(v);
//	m_revisionCount++;
//}
//Color CommonMaterial::getColorScale() const { auto itr = m_builtinValueMap.find(ColorScaleHash); return (itr != m_builtinValueMap.end()) ? Color(itr->second.getVector()) : DefaultColorScale; }
//
//void CommonMaterial::setBlendColor(const Color& v)
//{
//	m_builtinValueMap[BlendColorHash].setVector(v);
//	m_revisionCount++;
//}
//Color CommonMaterial::getBlendColor() const { auto itr = m_builtinValueMap.find(BlendColorHash); return (itr != m_builtinValueMap.end()) ? Color(itr->second.getVector()) : DefaultBlendColor; }
//
//void CommonMaterial::setTone(const ToneF& v)
//{
//	m_builtinValueMap[ToneHash].setVector(v);
//	m_revisionCount++;
//}
//ToneF CommonMaterial::getTone() const { auto itr = m_builtinValueMap.find(ToneHash); return (itr != m_builtinValueMap.end()) ? ToneF(itr->second.getVector()) : DefaultTone; }
//


//==============================================================================
// Material
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Material, CommonMaterial);

static const String Material_ColorPropertyName(_LT("Color"));
static const String Material_RoughnessPropertyName(_LT("Roughness"));
static const String Material_MetallicPropertyName(_LT("Metallic"));
static const String Material_SpecularPropertyName(_LT("Specular"));

static const uint32_t Material_ColorPropertyNameId = Hash::calcHash(_LT("Color"));
static const uint32_t Material_RoughnessPropertyNameId = Hash::calcHash(_LT("Roughness"));
static const uint32_t Material_MetallicPropertyNameId = Hash::calcHash(_LT("Metallic"));
static const uint32_t Material_SpecularPropertyNameId = Hash::calcHash(_LT("Specular"));

static const Color Material_DefaultColor = Color(0.5f, 0.5f, 0.5f, 1.0f);
static const float Material_DefaultRoughness = 0.5f;
static const float Material_DefaultMetallic = 0.5f;
static const float Material_DefaultSpecular = 0.5f;

Ref<Material> Material::create()
{
	return newObject<Material>();
}

Material::Material()
{
}

Material::~Material()
{
}

void Material::initialize()
{
	CommonMaterial::initialize();
	setColor(Material_DefaultColor);
	setRoughness(Material_DefaultRoughness);
	setMetallic(Material_DefaultMetallic);
	setSpecular(Material_DefaultSpecular);
}

void Material::setColor(const Color& value)
{
	setBuiltinColorParameter(Material_ColorPropertyName, value);
}

void Material::setRoughness(float value)
{
	setBuiltinFloatParameter(Material_RoughnessPropertyName, value);
}

void Material::setMetallic(float value)
{
	setBuiltinFloatParameter(Material_MetallicPropertyName, value);
}

void Material::setSpecular(float value)
{
	setBuiltinFloatParameter(Material_SpecularPropertyName, value);
}

//==============================================================================
// DiffuseMaterial
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(DiffuseMaterial, CommonMaterial);

//------------------------------------------------------------------------------
DiffuseMaterialPtr DiffuseMaterial::create()
{
	auto ptr = DiffuseMaterialPtr::makeRef();
	ptr->initialize();
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
void DiffuseMaterial::initialize()
{
	CommonMaterial::initialize();
}

//------------------------------------------------------------------------------
void DiffuseMaterial::setDiffuse(const Color& value)
{
	setBuiltinColorParameter(CommonMaterial::DiffuseParameter, value);
}

//------------------------------------------------------------------------------
void DiffuseMaterial::setAmbient(const Color& value)
{
	setBuiltinColorParameter(CommonMaterial::AmbientParameter, value);
}

//------------------------------------------------------------------------------
void DiffuseMaterial::setSpecular(const Color& value)
{
	setBuiltinColorParameter(CommonMaterial::SpecularParameter, value);
}

//------------------------------------------------------------------------------
void DiffuseMaterial::setEmissive(const Color& value)
{
	setBuiltinColorParameter(CommonMaterial::EmissiveParameter, value);
}

//------------------------------------------------------------------------------
void DiffuseMaterial::setSpecularPower(float value)
{
	setBuiltinFloatParameter(CommonMaterial::PowerParameter, value);
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
	, m_diffuse(CommonMaterial::DefaultDiffuse)
	, m_ambient(CommonMaterial::DefaultAmbient)
	, m_specular(CommonMaterial::DefaultSpecular)
	, m_emissive(CommonMaterial::DefaultEmissive)
	, m_power(CommonMaterial::DefaultPower)
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
void CombinedMaterial::combine(/*CommonMaterial* owner, */CommonMaterial* ownerBase, const BuiltinEffectData& builtinEffectData)
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
	//if (owner != nullptr)		hashCode |= owner->getHashCode();
	if (ownerBase != nullptr)	hashCode |= ownerBase->getHashCode();

	if (m_lastSourceHashCode != hashCode ||
		m_lastBuiltinEffectHashCode != builtinEffectData.getHashCode())
	{
		CommonMaterial* source1 = ownerBase;//(ownerBase != nullptr) ? ownerBase : owner;
		CommonMaterial* source2 = nullptr;//(ownerBase != nullptr) ? owner : nullptr;

		// source1
		m_shader = source1->m_shader;
		m_colorScale = builtinEffectData.getColorScale();
		m_colorScale.a *= builtinEffectData.getOpacity();
		m_blendColor = builtinEffectData.getBlendColor();
		m_tone = builtinEffectData.getTone();
		copyUserValueTable(source1);
		// TODO
		m_blendMode = source1->getBlendMode();
		m_cullingMode = source1->getCullingMode();
		m_depthTestEnabled = source1->isDepthTestEnabled();
		m_depthWriteEnabled = source1->isDepthWriteEnabled();
		m_shadingModel = source1->shadingModel;

		// source2 (base があるなら owner を後からマージ)
		if (source2 != nullptr)
		{
			if (m_shader == nullptr) m_shader = source2->getShader();
			//m_colorScale.multiplyClamp(source2->getColorScale());
			//m_colorScale.a *= source2->getOpacity();
			//m_blendColor.addClamp(source2->getBlendColor());
			//m_tone.addClamp(source2->getTone());
			mergeUserValueTable(source2);
		}

		// parent
		//if (parent != nullptr)
		//{
		//	if (m_shader == nullptr) m_shader = parent->getShader();
		//	m_colorScale.multiplyClamp(parent->getColorScale());
		//	m_colorScale.a *= parent->getOpacity();
		//	m_blendColor.addClamp(parent->getBlendColor());
		//	m_tone.addClamp(parent->getTone());
		//}

		// CommonMaterial params (from base only. not parent inherit)
		// TODO: 文字列検索とかしまくっている。いろいろ最適化の余地ある
		if (source2 != nullptr)
		{
			LN_NOTIMPLEMENTED();
			m_diffuse = source1->getBuiltinColor(DiffuseHash, source2->getBuiltinColor(DiffuseHash, CommonMaterial::DefaultDiffuse));
			m_ambient = source1->getBuiltinColor(AmbientHash, source2->getBuiltinColor(AmbientHash, CommonMaterial::DefaultAmbient));
			m_specular = source1->getBuiltinColor(SpecularHash, source2->getBuiltinColor(SpecularHash, CommonMaterial::DefaultSpecular));
			m_emissive = source1->getBuiltinColor(EmissiveHash, source2->getBuiltinColor(EmissiveHash, CommonMaterial::DefaultEmissive));
			m_power = source1->getBuiltinFloat(PowerHash, source2->getBuiltinFloat(PowerHash, CommonMaterial::DefaultPower));
			m_mainTexture = source1->getMaterialTexture(source2->getMaterialTexture(nullptr));
		}
		else
		{
			m_diffuse = source1->getBuiltinColor(DiffuseHash, CommonMaterial::DefaultDiffuse);
			m_ambient = source1->getBuiltinColor(AmbientHash, CommonMaterial::DefaultAmbient);
			m_specular = source1->getBuiltinColor(SpecularHash, CommonMaterial::DefaultSpecular);
			m_emissive = source1->getBuiltinColor(EmissiveHash, CommonMaterial::DefaultEmissive);
			m_power = source1->getBuiltinFloat(PowerHash, CommonMaterial::DefaultPower);
			m_mainTexture = source1->getMaterialTexture(nullptr);

			m_m2_color = source1->getBuiltinColor(Material_ColorPropertyNameId, Material_DefaultColor);
			m_m2_roughness = source1->getBuiltinFloat(Material_RoughnessPropertyNameId, Material_DefaultRoughness);
			m_m2_metallic = source1->getBuiltinFloat(Material_MetallicPropertyNameId, Material_DefaultMetallic);
			m_m2_specular = source1->getBuiltinFloat(Material_SpecularPropertyNameId, Material_DefaultSpecular);
		}

		if (m_shader == nullptr)
		{
			m_shader = builtinEffectData.getShader();
		}

		m_lastSourceHashCode = hashCode;
		m_lastBuiltinEffectHashCode = builtinEffectData.getHashCode();
	}
}

//------------------------------------------------------------------------------
void CombinedMaterial::applyUserShaderValeues(Shader* targetShader)
{
	for (auto& pair : m_userValueTable)
	{
		targetShader->setShaderValue(pair.nameHash, pair.value);
	}
}

//------------------------------------------------------------------------------
void CombinedMaterial::copyUserValueTable(CommonMaterial* source)
{
	assert(source != nullptr);
	m_userValueTable.clear();
	for (auto& pair : source->getUserValueMap())
	{
		m_userValueTable.add(ValuePair{ pair.first, pair.second });
	}
}

//------------------------------------------------------------------------------
void CombinedMaterial::mergeUserValueTable(CommonMaterial* source)
{
	assert(source != nullptr);
	for (auto& pair : source->getUserValueMap())
	{
		uint32_t key = pair.first;
		ValuePair* p = m_userValueTable.find([key](const ValuePair& p) { return p.nameHash == key; });
		if (p != nullptr)
		{
			p->value = pair.second;
		}
		else
		{
			m_userValueTable.add(ValuePair{ pair.first, pair.second });
		}
	}
}

} // namespace detail
LN_NAMESPACE_END