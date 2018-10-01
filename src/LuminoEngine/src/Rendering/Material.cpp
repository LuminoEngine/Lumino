
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>

namespace ln {

//==============================================================================
// AbstractMaterial

AbstractMaterial::AbstractMaterial(detail::MaterialType type)
	: m_type(type)
{
}

AbstractMaterial::~AbstractMaterial()
{
}

void AbstractMaterial::initialize()
{
	Object::initialize();
}

void AbstractMaterial::setMainTexture(Texture* value)
{
	m_mainTexture = value;
}

Texture* AbstractMaterial::mainTexture() const
{
	return m_mainTexture;
}

void AbstractMaterial::setShader(Shader* shader)
{
	LN_NOTIMPLEMENTED();
}

Shader* AbstractMaterial::shader() const
{
	return m_shader;
}

void AbstractMaterial::setInt(const StringRef& name, int value)
{
	LN_NOTIMPLEMENTED();
}

void AbstractMaterial::setFloat(const StringRef& name, float value)
{
	LN_NOTIMPLEMENTED();
}

void AbstractMaterial::setVector(const StringRef& name, const Vector4& value)
{
	LN_NOTIMPLEMENTED();
}

void AbstractMaterial::setMatrix(const StringRef& name, const Matrix& value)
{
	LN_NOTIMPLEMENTED();
}

void AbstractMaterial::setTexture(const StringRef& name, Texture* value)
{
	LN_NOTIMPLEMENTED();
}

void AbstractMaterial::setColor(const StringRef& name, const Color& value)
{
	LN_NOTIMPLEMENTED();
}



//==============================================================================
// Material
// https://docs.unrealengine.com/latest/JPN/Engine/Rendering/Materials/PhysicallyBased/index.html
// https://threejs.org/docs/#api/en/materials/MeshStandardMaterial

static const uint32_t Material_ColorPropertyNameId = CRCHash::compute(_LT("Color"));
static const uint32_t Material_RoughnessPropertyNameId = CRCHash::compute(_LT("Roughness"));
static const uint32_t Material_MetallicPropertyNameId = CRCHash::compute(_LT("Metallic"));
static const uint32_t Material_SpecularPropertyNameId = CRCHash::compute(_LT("Specular"));

static const Color Material_DefaultColor = Color(0.5f, 0.5f, 0.5f, 1.0f);
static const float Material_DefaultRoughness = 0.5f;
static const float Material_DefaultMetallic = 0.5f;
static const float Material_DefaultSpecular = 0.5f;

Ref<Material> Material::create()
{
	return newObject<Material>();
}

Material::Material()
	: AbstractMaterial(detail::MaterialType::PBR)
{
	m_data.color = Material_DefaultColor;
	m_data.roughness = Material_DefaultRoughness;
	m_data.metallic = Material_DefaultMetallic;
	m_data.specular = Material_DefaultSpecular;
}

Material::~Material()
{
}

void Material::initialize()
{
	AbstractMaterial::initialize();
}

void Material::setColor(const Color& value)
{
	m_data.color = value;
}

void Material::setRoughness(float value)
{
	m_data.roughness = value;
}

void Material::setMetallic(float value)
{
	m_data.metallic = value;
}

void Material::setSpecular(float value)
{
	m_data.specular = value;
}

void Material::translateToPBRMaterialData(detail::PbrMaterialData* outData)
{
	*outData = m_data;
}

void Material::translateToPhongMaterialData(detail::PhongMaterialData* outData)
{
	LN_NOTIMPLEMENTED();
}

} // namespace ln

