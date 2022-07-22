
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Asset/Assets.hpp>
#include <LuminoGraphics/RHI/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/RHI/ShaderDescriptor.hpp>
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>

namespace ln {

//==============================================================================
// Material
// https://docs.unrealengine.com/latest/JPN/Engine/Rendering/Materials/PhysicallyBased/index.html
// https://threejs.org/docs/#api/en/materials/MeshStandardMaterial
//
// フィールド名
// ----------
//
// - glTF > baseColor, Roughness, Metallic
// - filament > color, roughness, metallic
// - Three.js > color, roughness, metalness
// - Unity > Albedo, Metallic, Smoothness
// - UE4 > BaseColor, Roughness, Metallic, Specular

        

LN_OBJECT_IMPLEMENT(Material, Object) {}

static const Color Material_DefaultColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
static const float Material_DefaultRoughness = 0.5f;
static const float Material_DefaultMetallic = 0.0f; // 1.0 に近づくほど、BaseColor は全く反映されなくなる。思った色を出せない原因になりやすいので 0.0 にしておく。Unity もそんな感じ。
//static const float Material_DefaultSpecular = 0.5f;
static const Color Material_DefaultEmmisive = Color(0, 0, 0, 0);

Material* Material::defaultMaterial()
{
    return detail::RenderingManager::instance()->defaultMaterial();
}

Ref<Material> Material::create()
{
    return makeObject<Material>();
}

Ref<Material> Material::create(Texture* mainTexture)
{
    return makeObject<Material>(mainTexture);
}

Ref<Material> Material::create(Texture* mainTexture, ShadingModel shadingModel)
{
    return makeObject<Material>(mainTexture, shadingModel);
}

Material::Material()
    : m_needRefreshShaderBinding(false)
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
	Object::init();
}

void Material::init(Texture* mainTexture)
{
    init(mainTexture, ShadingModel::Default);
}

void Material::init(Texture* mainTexture, ShadingModel shadingModel)
{
    init();
    setMainTexture(mainTexture);
    m_shadingModel = shadingModel;
}

void Material::init(Texture* mainTexture, const detail::PhongMaterialData& phongMaterialData)
{
    init();
    setMainTexture(mainTexture);
    setColor(phongMaterialData.diffuse);
}

void Material::setMainTexture(Texture* value)
{
	m_mainTexture = value;
}

Texture* Material::mainTexture() const
{
	return m_mainTexture;
}

void Material::setNormalMap(Texture* value)
{
    m_normalMap = value;
}

Texture* Material::normalMap() const
{
    return m_normalMap;
}

void Material::setMetallicRoughnessTexture(Texture* value)
{
    m_metallicRoughnessTexture = value;
}

Texture* Material::metallicRoughnessTexture() const
{
    return m_metallicRoughnessTexture;
}

void Material::setOcclusionTexture(Texture* value)
{
    m_occlusionTexture = value;
}

Texture* Material::occlusionTexture() const
{
    return m_occlusionTexture;
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

void Material::setEmissive(const Color& value)
{
    m_data.emissive = value;
}

void Material::setShader(Shader* shader)
{
    m_shader = shader;
    m_needRefreshShaderBinding = true;
}

Shader* Material::shader() const
{
	return m_shader;
}

void Material::setInt(const StringView& name, int value)
{
	detail::ShaderParameterValue* param = getValue(name);
	param->setInt(value);
}

void Material::setFloat(const StringView& name, float value)
{
	detail::ShaderParameterValue* param = getValue(name);
	param->setFloat(value);
}

void Material::setFloatArray(const StringView& name, const float* values, int length)
{
	detail::ShaderParameterValue* param = getValue(name);
	param->setFloatArray(values, length);
}

void Material::setVector(const StringView& name, const Vector4& value)
{
    detail::ShaderParameterValue* param = getValue(name);
    param->setVector(value);
}

void Material::setVectorArray(const StringView& name, const Vector4* values, int length)
{
	detail::ShaderParameterValue* param = getValue(name);
	param->setVectorArray(values, length);
}

void Material::setMatrix(const StringView& name, const Matrix& value)
{
    detail::ShaderParameterValue* param = getValue(name);
    param->setMatrix(value);
}

void Material::setTexture(const StringView& name, Texture* value)
{
	detail::ShaderParameterValue* param = getValue(name);
	param->setTexture(value);
}

void Material::setColor(const StringView& name, const Color& value)
{
	detail::ShaderParameterValue* param = getValue(name);
	param->setVector(value.toVector4());
}

void Material::setBufferData(const StringView& uniformBufferName, const void* data, int size)
{
    ByteBuffer* buffer;

    const auto itr = std::find_if(
        m_uniformBufferData.begin(), m_uniformBufferData.end(), [&](const UniformBufferEntiry& e) {
            return e.name == uniformBufferName;
        });
    if (itr != m_uniformBufferData.end()) {
        buffer = itr->data;
    }
    else {
        auto newBuf = makeRef<ByteBuffer>();
        m_uniformBufferData.push_back({ uniformBufferName, newBuf, -1 });
        buffer = newBuf;
        m_needRefreshShaderBinding = true;
    }

    buffer->assign(data, size);
}

void Material::setBlendMode(Optional_deprecated<BlendMode> mode)
{
    blendMode = mode;
}

void Material::setCullingMode(Optional_deprecated<CullMode> mode)
{
    cullingMode = mode;
}

void Material::setDepthTestEnabled(Optional_deprecated<bool> enabled)
{
    depthTestEnabled = enabled;
}

void Material::setDepthWriteEnabled(Optional_deprecated<bool> enabled)
{
    depthWriteEnabled = enabled;
}

detail::ShaderParameterValue* Material::getValue(const ln::StringView& name)
{
	for (auto& pair : m_values) {
		if (pair.first == name) {
			return pair.second.get();
		}
	}

	auto v = std::make_shared<detail::ShaderParameterValue>();
	m_values.push_back({ String(name), v });
    m_needRefreshShaderBinding = true;
	return m_values.back().second.get();
}

void Material::updateShaderVariables(GraphicsCommandList* commandList, detail::ShaderSecondaryDescriptor* descriptor)
{
    Shader* target = descriptor->shader();
    const ShaderDescriptorLayout* layout = target->descriptorLayout();

    if (m_needRefreshShaderBinding) {
        for (UniformBufferEntiry& e : m_uniformBufferData) {
            e.descriptorIndex = layout->findUniformBufferRegisterIndex(e.name);
        }
        m_needRefreshShaderBinding = false;
    }

    // Material から Shader へ検索をかける。
    // Shader はビルトインの変数がいくつか含まれているので、この方が高速に検索できる。

    for (const auto& pair : m_values) {
        auto* param = target->findParameter(pair.first);
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
                param->setInt(pair.second->getInt(), descriptor);
                break;
            case ShaderVariableType::Float:
                param->setFloat(pair.second->getFloat(), descriptor);
                break;
            case ShaderVariableType::FloatArray:
                param->setFloatArray(pair.second->getFloatArray(), pair.second->getArrayLength(), descriptor);
                break;
            case ShaderVariableType::Vector:
                param->setVector(pair.second->getVector(), descriptor);
                break;
            case ShaderVariableType::VectorArray:
                param->setVectorArray(pair.second->getVectorArray(), pair.second->getArrayLength(), descriptor);
                break;
            case ShaderVariableType::Matrix:
                param->setMatrix(pair.second->getMatrix(), descriptor);
                break;
            case ShaderVariableType::MatrixArray:
                param->setMatrixArray(pair.second->getMatrixArray(), pair.second->getArrayLength(), descriptor);
                break;
            case ShaderVariableType::Texture:
                //param->setTexture(pair.second->getTexture(), descriptor);
                descriptor->setTexture(param->m_dataIndex, pair.second->getTexture());
                break;
            case ShaderVariableType::Pointer:
                LN_NOTIMPLEMENTED();
                break;
            default:
                break;
            }
        }
    }

    for (const UniformBufferEntiry& e : m_uniformBufferData) {
        if (!descriptor->uniformBuffer(e.descriptorIndex).buffer) {
            descriptor->setUniformBuffer(e.descriptorIndex, commandList->allocateUniformBuffer(layout->m_buffers[e.descriptorIndex].size));
        }
        descriptor->setUniformBufferData(e.descriptorIndex, e.data->data(), e.data->size());
    }
}

void Material::updateShaderVariables2(GraphicsCommandList* commandList, ShaderDescriptor* descriptor) {
    const ShaderPass* target = descriptor->shaderPass();
    const ShaderDescriptorLayout* layout = target->shaderPassDescriptorLayout();

    if (m_needRefreshShaderBinding) {
        for (UniformBufferEntiry& e : m_uniformBufferData) {
            e.descriptorIndex = layout->findUniformBufferRegisterIndex(e.name);
        }
        m_needRefreshShaderBinding = false;
    }

    // Material から Shader へ検索をかける。
    // Shader はビルトインの変数がいくつか含まれているので、この方が高速に検索できる。

    for (const auto& pair : m_values) {
        if (pair.second->type() == ShaderVariableType::Texture) {
            int memberIndex = layout->findTextureRegisterIndex(pair.first);
            if (memberIndex >= 0) {
                descriptor->setTexture(memberIndex, pair.second->getTexture());
            }
        }
        else {
            int memberIndex = layout->findUniformMemberIndex(pair.first);
            if (memberIndex >= 0) {
                const auto& memberInfo = layout->m_members[memberIndex];

                switch (pair.second->type()) {
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
                        descriptor->setInt(memberIndex, pair.second->getInt());
                        break;
                    case ShaderVariableType::Float:
                        descriptor->setFloat(memberIndex, pair.second->getFloat());
                        break;
                    case ShaderVariableType::FloatArray:
                        descriptor->setFloatArray(memberIndex, pair.second->getFloatArray(), pair.second->getArrayLength());
                        break;
                    case ShaderVariableType::Vector:
                        descriptor->setVector(memberIndex, pair.second->getVector());
                        break;
                    case ShaderVariableType::VectorArray:
                        descriptor->setVectorArray(memberIndex, pair.second->getVectorArray(), pair.second->getArrayLength());
                        break;
                    case ShaderVariableType::Matrix:
                        descriptor->setMatrix(memberIndex, pair.second->getMatrix());
                        break;
                    case ShaderVariableType::MatrixArray:
                        descriptor->setMatrixArray(memberIndex, pair.second->getMatrixArray(), pair.second->getArrayLength());
                        break;
                    case ShaderVariableType::Texture:
                        //param->setTexture(pair.second->getTexture(), descriptor);
                        //descriptor->setTexture(param->m_dataIndex, pair.second->getTexture());
                        break;
                    case ShaderVariableType::Pointer:
                        LN_NOTIMPLEMENTED();
                        break;
                    default:
                        break;
                }
            }
        }



        //auto* param = target->findParameter(pair.first);
        //if (param) {
        //}
    }

    for (const UniformBufferEntiry& e : m_uniformBufferData) {
        if (!descriptor->uniformBuffer(e.descriptorIndex).buffer) {
            descriptor->setUniformBuffer(e.descriptorIndex, commandList->allocateUniformBuffer(layout->m_buffers[e.descriptorIndex].size));
        }
        descriptor->setUniformBufferData(e.descriptorIndex, e.data->data(), e.data->size());
    }
}

void Material::serialize_deprecated(Serializer2_deprecated& ar)
{
    Object::serialize_deprecated(ar);
    LN_NOTIMPLEMENTED();

    //// TODO: ↓Assets辺りに関数化
    //if (ar.isSaving()) {
    //    Path path = Assets::getAssetPath(m_mainTexture);
    //    if (path.isEmpty()) {
    //        // assetPath が空であればインスタンスを serialize する
    //        ar & makeNVP(u"mainTexture", m_mainTexture);
    //    }
    //    else {
    //        // assetPath を持っているときは assetPath を serialize する
    //        ar & makeNVP(u"mainTexture", path);
    //    }
    //}
    //else {
    //    if (ar.readName(u"mainTexture")) {
    //        if (ar.readingValueIsObject()) {
    //            Path path = ar.readString();
    //            LN_NOTIMPLEMENTED();
    //        }
    //        else {
    //            Path path = ar.readString();
    //            m_mainTexture = Texture2D::load(path);
    //        }
    //    }
    //    else {
    //        m_mainTexture = nullptr;
    //    }
    //}

}

////==============================================================================
//// PhongMaterial
//
////LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PhongMaterial, PhongMaterial);
//
//const String PhongMaterial::DiffuseParameterName(u"_Diffuse");
//const String PhongMaterial::AmbientParameterName(u"_Ambient");
//const String PhongMaterial::EmissiveParameterName(u"_Emissive");
//const String PhongMaterial::SpecularParameterName(u"_Specular");
//const String PhongMaterial::SpecularPowerParameterName(u"_Power");
//
//const Color PhongMaterial::DefaultDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
//const Color PhongMaterial::DefaultAmbient(0.0f, 0.0f, 0.0f, 0.0f);
//const Color PhongMaterial::DefaultSpecular(0.5f, 0.5f, 0.5f, 0.5f);
//const Color PhongMaterial::DefaultEmissive(0.0f, 0.0f, 0.0f, 0.0f);
//const float PhongMaterial::DefaultPower = 50.0f;
//
//Ref<PhongMaterial> PhongMaterial::create()
//{
//	return makeObject<PhongMaterial>();
//}
//
//PhongMaterial::PhongMaterial()
//	: Material(detail::MaterialType::Phong)
//{
//}
//
//PhongMaterial::~PhongMaterial()
//{
//}
//
//void PhongMaterial::init()
//{
//	Material::init();
//}
//
//void PhongMaterial::setDiffuse(const Color& value)
//{
//	m_data.diffuse = value;
//	setColor(DiffuseParameterName, value);
//}
//
//void PhongMaterial::setAmbient(const Color& value)
//{
//	m_data.ambient = value;
//	setColor(AmbientParameterName, value);
//}
//
//void PhongMaterial::setEmissive(const Color& value)
//{
//	m_data.emissive = value;
//	setColor(EmissiveParameterName, value);
//}
//
//void PhongMaterial::setSpecular(const Color& value)
//{
//	m_data.specular = value;
//	setColor(SpecularParameterName, value);
//}
//
//void PhongMaterial::setSpecularPower(float value)
//{
//	m_data.power = value;
//	setFloat(SpecularPowerParameterName, value);
//}
//
//void PhongMaterial::translateToPBRMaterialData(detail::PbrMaterialData* outData)
//{
//	outData->color = m_data.diffuse;
//	outData->roughness = Material_DefaultRoughness;
//	outData->metallic = Material_DefaultMetallic;
//	//outData->specular = Material_DefaultSpecular;
//}

//==============================================================================
// Material::BuilderDetails

Material::BuilderDetails::BuilderDetails()
    : color(Material_DefaultColor)
    , roughness(Material_DefaultRoughness)
    , metallic(Material_DefaultMetallic)
{
}

void Material::BuilderDetails::apply(Material* p) const
{
    p->setColor(color);
    p->setRoughness(roughness);
    p->setMetallic(metallic);
}

} // namespace ln

