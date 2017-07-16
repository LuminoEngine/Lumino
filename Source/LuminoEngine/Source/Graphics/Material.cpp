
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
//LN_TR_PROPERTY_IMPLEMENT(Material, BlendMode, blendMode, tr::PropertyMetadata(Material::onRenderStateChanged));
//LN_TR_PROPERTY_IMPLEMENT(Material, CullingMode, cullingMode, tr::PropertyMetadata(Material::onRenderStateChanged));
//LN_TR_PROPERTY_IMPLEMENT(Material, bool, depthTestEnabled, tr::PropertyMetadata(Material::onRenderStateChanged));
//LN_TR_PROPERTY_IMPLEMENT(Material, bool, depthWriteEnabled, tr::PropertyMetadata(Material::onRenderStateChanged));

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
MaterialPtr Material::create()
{
	auto ptr = MaterialPtr::makeRef();
	ptr->initialize();
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
	initializeProperties();
	reset();
}

//------------------------------------------------------------------------------
Material::~Material()
{
}

//------------------------------------------------------------------------------
void Material::initialize()
{
	//m_combinedMaterial = Ref<detail::CombinedMaterial>::MakeRef();
}

//------------------------------------------------------------------------------
void Material::reset()
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
void Material::setBuiltinIntParameter(const StringRef& name, int value) { m_builtinValueMap[Hash::calcHash(name.getBegin(), name.getLength())].setInt(value); }
void Material::setBuiltinFloatParameter(const StringRef& name, float value) { m_builtinValueMap[Hash::calcHash(name.getBegin(), name.getLength())].setFloat(value); }
void Material::setBuiltinVectorParameter(const StringRef& name, const Vector4& value) { m_builtinValueMap[Hash::calcHash(name.getBegin(), name.getLength())].setVector(value); }
void Material::setBuiltinMatrixParameter(const StringRef& name, const Matrix& value) { m_builtinValueMap[Hash::calcHash(name.getBegin(), name.getLength())].setMatrix(value); }
void Material::setBuiltinTextureParameter(const StringRef& name, Texture* value) { m_builtinValueMap[Hash::calcHash(name.getBegin(), name.getLength())].setManagedTexture(value); }
void Material::setBuiltinColorParameter(const StringRef& name, const Color& value) { setBuiltinVectorParameter(name, value); }
void Material::setBuiltinColorParameter(const StringRef& name, float r, float g, float b, float a) { setBuiltinVectorParameter(name, Color(r, g, b, a)); }

//------------------------------------------------------------------------------
Ref<Material> Material::copyShared() const
{
	auto m = Ref<Material>::makeRef();
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
//void Material::ResolveCombinedMaterial()
//{
//	m_combinedMaterial->Combine(nullptr, this, nullptr);	// TODO: parent and base
//}
//
////------------------------------------------------------------------------------
//detail::CombinedMaterial* Material::getCombinedMaterial() const
//{
//	return m_combinedMaterial;
//}

//------------------------------------------------------------------------------
void Material::setShader(Shader* shader)
{
	if (m_shader != shader)
	{
		m_shader = shader;
		m_revisionCount++;
	}
}

//------------------------------------------------------------------------------
Shader* Material::getShader() const
{
	return m_shader;
}

//------------------------------------------------------------------------------
void Material::setIntParameter(const StringRef& name, int value)
{
	uint32_t hashKey = Hash::calcHash(name.getBegin(), name.getLength());
	findAndCreateUserShaderValue(hashKey)->setInt(value);
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::setFloatParameter(const StringRef& name, float value)
{
	uint32_t hashKey = Hash::calcHash(name.getBegin(), name.getLength());
	findAndCreateUserShaderValue(hashKey)->setFloat(value);
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::setVectorParameter(const StringRef& name, const Vector4& value)
{
	uint32_t hashKey = Hash::calcHash(name.getBegin(), name.getLength());
	findAndCreateUserShaderValue(hashKey)->setVector(value);
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::setMatrixParameter(const StringRef& name, const Matrix& value)
{
	uint32_t hashKey = Hash::calcHash(name.getBegin(), name.getLength());
	findAndCreateUserShaderValue(hashKey)->setMatrix(value);
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::setTextureParameter(const StringRef& name, Texture* value)
{
	uint32_t hashKey = Hash::calcHash(name.getBegin(), name.getLength());
	findAndCreateUserShaderValue(hashKey)->setManagedTexture(value);
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::setColorParameter(const StringRef& name, const Color& value)
{
	setVectorParameter(name, value);
}

//------------------------------------------------------------------------------
void Material::setColorParameter(const StringRef& name, float r, float g, float b, float a)
{
	setColorParameter(name, Color(r, g, b, a));
}

//------------------------------------------------------------------------------
void Material::setBlendMode(Nullable<BlendMode> mode)
{
	blendMode = mode;
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::setCullingMode(Nullable<CullingMode> mode)
{
	cullingMode = mode;
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::setDepthTestEnabled(Nullable<bool> enabled)
{
	depthTestEnabled = enabled;
	m_revisionCount++;
}

//------------------------------------------------------------------------------
void Material::setDepthWriteEnabled(Nullable<bool> enabled)
{
	depthWriteEnabled = enabled;
	m_revisionCount++;
}

////------------------------------------------------------------------------------
//void Material::setBlendMode(BlendMode mode)
//{
//	m_builtin.blendMode = mode;
//	m_revisionCount++;
//}
//
////------------------------------------------------------------------------------
//void Material::setCullingMode(CullingMode mode)
//{
//	m_builtin.cullingMode = mode;
//	m_revisionCount++;
//}
//
////------------------------------------------------------------------------------
//void Material::setDepthTestEnabled(bool enabled)
//{
//	m_builtin.depthTestEnabled = enabled;
//	m_revisionCount++;
//}
//
////------------------------------------------------------------------------------
//void Material::setDepthWriteEnabled(bool enabled)
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
ShaderValue* Material::findAndCreateUserShaderValue(uint32_t hashKey)
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
const ShaderValue* Material::findUserShaderValueConst(uint32_t hashKey) const
{
	auto itr = m_userValueMap.find(hashKey);
	if (itr != m_userValueMap.end())
		return &(itr->second);
	return nullptr;
}

//------------------------------------------------------------------------------
uint32_t Material::getHashCode()
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
void Material::onRenderStateChanged(Object* obj)
{
	static_cast<Material*>(obj)->m_revisionCount++;
}

//------------------------------------------------------------------------------
static const Color DefaultColorScale(1, 1, 1, 1);
static const Color DefaultBlendColor(0, 0, 0, 0);
static const ToneF DefaultTone(0, 0, 0, 0);
static const uint32_t MaterialTextureHash = Hash::calcHash(_T("MaterialTexture"));
static const uint32_t OpacityHash = Hash::calcHash(_T("Opacity"));
static const uint32_t ColorScaleHash = Hash::calcHash(_T("ColorScale"));
static const uint32_t BlendColorHash = Hash::calcHash(_T("BlendColor"));
static const uint32_t ToneHash = Hash::calcHash(_T("Tone"));
static const uint32_t DiffuseHash = Hash::calcHash(_T("Diffuse"));
static const uint32_t AmbientHash = Hash::calcHash(_T("Ambient"));
static const uint32_t SpecularHash = Hash::calcHash(_T("Specular"));
static const uint32_t EmissiveHash = Hash::calcHash(_T("Emissive"));
static const uint32_t PowerHash = Hash::calcHash(_T("Power"));

void Material::setMaterialTexture(Texture* v)
{
	m_builtinValueMap[MaterialTextureHash].setManagedTexture(v);
	m_revisionCount++;
}
Texture* Material::getMaterialTexture(Texture* defaultValue) const { auto itr = m_builtinValueMap.find(MaterialTextureHash); return (itr != m_builtinValueMap.end()) ? itr->second.getManagedTexture() : defaultValue; }

//void Material::setOpacity(float v)
//{
//	m_builtinValueMap[OpacityHash].SetFloat(v);
//	m_revisionCount++;
//}
//float Material::getOpacity() const { auto itr = m_builtinValueMap.find(OpacityHash); return (itr != m_builtinValueMap.end()) ? itr->second.GetFloat() : 1.0f; }
//
//void Material::setColorScale(const Color& v)
//{
//	m_builtinValueMap[ColorScaleHash].setVector(v);
//	m_revisionCount++;
//}
//Color Material::getColorScale() const { auto itr = m_builtinValueMap.find(ColorScaleHash); return (itr != m_builtinValueMap.end()) ? Color(itr->second.getVector()) : DefaultColorScale; }
//
//void Material::setBlendColor(const Color& v)
//{
//	m_builtinValueMap[BlendColorHash].setVector(v);
//	m_revisionCount++;
//}
//Color Material::getBlendColor() const { auto itr = m_builtinValueMap.find(BlendColorHash); return (itr != m_builtinValueMap.end()) ? Color(itr->second.getVector()) : DefaultBlendColor; }
//
//void Material::setTone(const ToneF& v)
//{
//	m_builtinValueMap[ToneHash].setVector(v);
//	m_revisionCount++;
//}
//ToneF Material::getTone() const { auto itr = m_builtinValueMap.find(ToneHash); return (itr != m_builtinValueMap.end()) ? ToneF(itr->second.getVector()) : DefaultTone; }
//

//==============================================================================
// DiffuseMaterial
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(DiffuseMaterial, Material);

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
	Material::initialize();
}

//------------------------------------------------------------------------------
void DiffuseMaterial::setDiffuse(const Color& value)
{
	setBuiltinColorParameter(Material::DiffuseParameter, value);
}

//------------------------------------------------------------------------------
void DiffuseMaterial::setAmbient(const Color& value)
{
	setBuiltinColorParameter(Material::AmbientParameter, value);
}

//------------------------------------------------------------------------------
void DiffuseMaterial::setSpecular(const Color& value)
{
	setBuiltinColorParameter(Material::SpecularParameter, value);
}

//------------------------------------------------------------------------------
void DiffuseMaterial::setEmissive(const Color& value)
{
	setBuiltinColorParameter(Material::EmissiveParameter, value);
}

//------------------------------------------------------------------------------
void DiffuseMaterial::setSpecularPower(float value)
{
	setBuiltinFloatParameter(Material::PowerParameter, value);
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
void CombinedMaterial::combine(/*Material* owner, */Material* ownerBase, const BuiltinEffectData& builtinEffectData)
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
		Material* source1 = ownerBase;//(ownerBase != nullptr) ? ownerBase : owner;
		Material* source2 = nullptr;//(ownerBase != nullptr) ? owner : nullptr;

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

		// Material params (from base only. not parent inherit)
		// TODO: 文字列検索とかしまくっている。いろいろ最適化の余地ある
		if (source2 != nullptr)
		{
			m_diffuse = source1->getBuiltinColor(DiffuseHash, source2->getBuiltinColor(DiffuseHash, Material::DefaultDiffuse));
			m_ambient = source1->getBuiltinColor(AmbientHash, source2->getBuiltinColor(AmbientHash, Material::DefaultAmbient));
			m_specular = source1->getBuiltinColor(SpecularHash, source2->getBuiltinColor(SpecularHash, Material::DefaultSpecular));
			m_emissive = source1->getBuiltinColor(EmissiveHash, source2->getBuiltinColor(EmissiveHash, Material::DefaultEmissive));
			m_power = source1->getBuiltinFloat(PowerHash, source2->getBuiltinFloat(PowerHash, Material::DefaultPower));
			m_mainTexture = source1->getMaterialTexture(source2->getMaterialTexture(nullptr));
		}
		else
		{
			m_diffuse = source1->getBuiltinColor(DiffuseHash, Material::DefaultDiffuse);
			m_ambient = source1->getBuiltinColor(AmbientHash, Material::DefaultAmbient);
			m_specular = source1->getBuiltinColor(SpecularHash, Material::DefaultSpecular);
			m_emissive = source1->getBuiltinColor(EmissiveHash, Material::DefaultEmissive);
			m_power = source1->getBuiltinFloat(PowerHash, Material::DefaultPower);
			m_mainTexture = source1->getMaterialTexture(nullptr);
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
void CombinedMaterial::copyUserValueTable(Material* source)
{
	assert(source != nullptr);
	m_userValueTable.clear();
	for (auto& pair : source->getUserValueMap())
	{
		m_userValueTable.add(ValuePair{ pair.first, pair.second });
	}
}

//------------------------------------------------------------------------------
void CombinedMaterial::mergeUserValueTable(Material* source)
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