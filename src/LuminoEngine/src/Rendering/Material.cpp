
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

void AbstractMaterial::init()
{
	Object::init();
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
    m_shader = shader;
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
	detail::ShaderParameterValue* param = getValue(name);
	param->setFloat(value);
}

void AbstractMaterial::setVector(const StringRef& name, const Vector4& value)
{
    detail::ShaderParameterValue* param = getValue(name);
    param->setVector(value);
}

void AbstractMaterial::setMatrix(const StringRef& name, const Matrix& value)
{
    detail::ShaderParameterValue* param = getValue(name);
    param->setMatrix(value);
}

void AbstractMaterial::setTexture(const StringRef& name, Texture* value)
{
	LN_NOTIMPLEMENTED();
}

void AbstractMaterial::setColor(const StringRef& name, const Color& value)
{
	detail::ShaderParameterValue* param = getValue(name);
	param->setVector(value);
}

void AbstractMaterial::setBlendMode(Optional<BlendMode> mode)
{
    blendMode = mode;
}

void AbstractMaterial::setCullingMode(Optional<CullMode> mode)
{
    cullingMode = mode;
}

void AbstractMaterial::setDepthTestEnabled(Optional<bool> enabled)
{
    depthTestEnabled = enabled;
}

void AbstractMaterial::setDepthWriteEnabled(Optional<bool> enabled)
{
    depthWriteEnabled = enabled;
}

detail::ShaderParameterValue* AbstractMaterial::getValue(const ln::StringRef& name)
{
	for (auto& pair : m_values) {
		if (pair.first == name) {
			return pair.second.get();
		}
	}

	auto v = std::make_shared<detail::ShaderParameterValue>();
	m_values.push_back({ String(name), v });
	return m_values.back().second.get();
}

void AbstractMaterial::updateShaderVariables(Shader* target)
{
    // Material から Shader へ検索をかける。
    // Shader はビルトインの変数がいくつか含まれているので、この方が高速に検索できる。

    for (auto& pair : m_values) {
        ShaderParameter* param = target->findParameter(pair.first);
        if (param) {
            switch (pair.second->type())
            {
            case ShaderVariableType::Unknown:
                LN_UNREACHABLE();
                break;
            case ShaderVariableType::Bool:
                LN_NOTIMPLEMENTED();
                break;
            case ShaderVariableType::BoolArray:
                LN_NOTIMPLEMENTED();
                break;
            case ShaderVariableType::Int:
                param->setInt(pair.second->getInt());
                break;
            case ShaderVariableType::Float:
                param->setFloat(pair.second->getFloat());
                break;
            case ShaderVariableType::FloatArray:
                param->setFloatArray(pair.second->getFloatArray(), pair.second->getArrayLength());
                break;
            case ShaderVariableType::Vector:
                param->setVector(pair.second->getVector());
                break;
            case ShaderVariableType::VectorArray:
                param->setVectorArray(pair.second->getVectorArray(), pair.second->getArrayLength());
                break;
            case ShaderVariableType::Matrix:
                param->setMatrix(pair.second->getMatrix());
                break;
            case ShaderVariableType::MatrixArray:
                param->setMatrixArray(pair.second->getMatrixArray(), pair.second->getArrayLength());
                break;
            case ShaderVariableType::Texture:
                param->setTexture(pair.second->getTexture());
                break;
            case ShaderVariableType::Pointer:
                LN_NOTIMPLEMENTED();
                break;
            default:
                break;
            }
        }
    }
}

//==============================================================================
// Material
// https://docs.unrealengine.com/latest/JPN/Engine/Rendering/Materials/PhysicallyBased/index.html
// https://threejs.org/docs/#api/en/materials/MeshStandardMaterial

//static const uint32_t Material_ColorPropertyNameId = CRCHash::compute(_LT("Color"));
//static const uint32_t Material_RoughnessPropertyNameId = CRCHash::compute(_LT("Roughness"));
//static const uint32_t Material_MetallicPropertyNameId = CRCHash::compute(_LT("Metallic"));
//static const uint32_t Material_SpecularPropertyNameId = CRCHash::compute(_LT("Specular"));

static const Color Material_DefaultColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
static const float Material_DefaultRoughness = 0.5f;
static const float Material_DefaultMetallic = 0.5f;
//static const float Material_DefaultSpecular = 0.5f;
static const Color Material_DefaultEmmisive = Color(0, 0, 0, 0);

Ref<Material> Material::create()
{
	return newObject<Material>();
}

Ref<Material> Material::create(Texture* mainTexture)
{
    return newObject<Material>(mainTexture);
}

Ref<Material> Material::create(Texture* mainTexture, ShadingModel shadingModel)
{
    return newObject<Material>(mainTexture, shadingModel);
}

Material::Material()
	: AbstractMaterial(detail::MaterialType::PBR)
{
	m_data.color = Material_DefaultColor;
	m_data.roughness = Material_DefaultRoughness;
	m_data.metallic = Material_DefaultMetallic;
	//m_data.specular = Material_DefaultSpecular;
    m_data.emissive = Material_DefaultEmmisive;
}

Material::~Material()
{
}

void Material::init()
{
	AbstractMaterial::init();
}

void Material::init(Texture* mainTexture)
{
    init(mainTexture, ShadingModel::Default);
}

void Material::init(Texture* mainTexture, ShadingModel shadingModel)
{
    init();
    setMainTexture(mainTexture);
    this->shadingModel = shadingModel;
}

void Material::init(Texture* mainTexture, const detail::PhongMaterialData& phongMaterialData)
{
    init();
    setMainTexture(mainTexture);
    setColor(phongMaterialData.diffuse);
    //setSpecular(phongMaterialData.power);
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
//
//void Material::setSpecular(float value)
//{
//	m_data.specular = value;
//}

void Material::setEmissive(const Color& value)
{
    m_data.emissive = value;
}

void Material::translateToPBRMaterialData(detail::PbrMaterialData* outData)
{
	*outData = m_data;
}

void Material::translateToPhongMaterialData(detail::PhongMaterialData* outData)
{
	LN_NOTIMPLEMENTED();
}


//==============================================================================
// PhongMaterial

//LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PhongMaterial, PhongMaterial);

const String PhongMaterial::DiffuseParameterName(u"_Diffuse");
const String PhongMaterial::AmbientParameterName(u"_Ambient");
const String PhongMaterial::EmissiveParameterName(u"_Emissive");
const String PhongMaterial::SpecularParameterName(u"_Specular");
const String PhongMaterial::SpecularPowerParameterName(u"_Power");

const Color PhongMaterial::DefaultDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
const Color PhongMaterial::DefaultAmbient(0.0f, 0.0f, 0.0f, 0.0f);
const Color PhongMaterial::DefaultSpecular(0.5f, 0.5f, 0.5f, 0.5f);
const Color PhongMaterial::DefaultEmissive(0.0f, 0.0f, 0.0f, 0.0f);
const float PhongMaterial::DefaultPower = 50.0f;

Ref<PhongMaterial> PhongMaterial::create()
{
	return newObject<PhongMaterial>();
}

PhongMaterial::PhongMaterial()
	: AbstractMaterial(detail::MaterialType::Phong)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::init()
{
	AbstractMaterial::init();
}

void PhongMaterial::setDiffuse(const Color& value)
{
	m_data.diffuse = value;
	setColor(DiffuseParameterName, value);
}

void PhongMaterial::setAmbient(const Color& value)
{
	m_data.ambient = value;
	setColor(AmbientParameterName, value);
}

void PhongMaterial::setEmissive(const Color& value)
{
	m_data.emissive = value;
	setColor(EmissiveParameterName, value);
}

void PhongMaterial::setSpecular(const Color& value)
{
	m_data.specular = value;
	setColor(SpecularParameterName, value);
}

void PhongMaterial::setSpecularPower(float value)
{
	m_data.power = value;
	setFloat(SpecularPowerParameterName, value);
}

void PhongMaterial::translateToPBRMaterialData(detail::PbrMaterialData* outData)
{
	outData->color = m_data.diffuse;
	outData->roughness = Material_DefaultRoughness;
	outData->metallic = Material_DefaultMetallic;
	//outData->specular = Material_DefaultSpecular;
}

void PhongMaterial::translateToPhongMaterialData(detail::PhongMaterialData* outData)
{
	*outData = m_data;
}

} // namespace ln

