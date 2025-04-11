#include "Internal.hpp"
#include <LuminoEngine/Asset/Assets.hpp>
#include <LuminoEngine/Graphics/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Graphics/GPU/ShaderDescriptor.hpp>
#include <LuminoEngine/Graphics/GPU/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>

namespace ln {

//==============================================================================
// Material
// 
// NOTE: パラメータは Category(Register) ごとに分けるか？ Variant な１つの配列にするか？
//   A. 分ける場合はこんな感じ
//       m_constantBuffers[];
//       m_textures[];
//       m_samplers[];
//		 m_storages[];
//   B. Variant な場合はこんな感じ
//	     m_parameters[];
//   内部的にはどちらでも大変さは変わらないかもしれない。
//   A は UnifiedShader2 ベースの方式とは異なるためマッピングが必要。
//   B は Lumino 初期のちょっと複雑な ShaderParameter の実装を踏襲することになる。
// 
//   0.11 時点の Material は A と B のハイブリッド。
//   A は ShaderSemanticManager が担当し、 B は Material が担当している。
//   ただし ShaderSemanticManager は $Global な ConstantBuffer のメンバ には対応していない。
//   また Material は Shader が変更された時でも前回値を保持したかったため Shader の Layout とは独立管理であり
//   毎フレーム、設定先のスロットを名前検索していた。
//   0.12 では、 set only にしてこの辺りを整理してもよい。 (Bに固執しなくてよい)
// 
//   最初期は B 方式だったけど、 Vulkan や DX12 をサポートし始めたあたりで、
//   複雑さをなんとか緩和しようと b, t, s, u... といった Register を意識した A 方式を導入した。
//   Slang はバックエンドによって異なる BindingLayout を生成するので、
//   Variant ではなく型情報を固定的にしたコードの見やすさや、バックエンドに引きずられない仕様を決めてのデバッグのやり易さを重視し、
//   差は RHI レイヤーで対応することにしてみる。
//   
//   なお Material に値をセットする API は次のようにする予定。
//   ```
//   // 名前指定
//   material->setIntByName("value1", 123);
//   // ID 指定
//   int id = material->findParameter("value1");
//   material->setIntById(id, 123);
//   ```
//   検索できるものは次の通り。
//   - ConstantBuffer
//   - Texture
//   - SamplerState
//   - StorageBuffer
//   - $Global な ConstantBuffer のメンバ
//
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
    return makeObject_deprecated<Material>();
}

Ref<Material> Material::create(Texture* mainTexture)
{
    return makeObject_deprecated<Material>(mainTexture);
}

Material::Material()
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

    

   //URef<kokage::ShaderCompiler> compiler = std::move(result);

    //auto result2 = compiler->build("E:/Proj/Lumino/packages/LuminoEngine/shader/CopyScreen.slang");
    //
    // 
    setShader(
            detail::RenderingManager::instance()->builtinShader(detail::BuiltinShader::Sprite));
}

void Material::init(Texture* mainTexture) {
    init();
    setMainTexture(mainTexture);
    m_shadingModel = ShadingModel::Default;
}

void Material::setMainTexture(Texture* value)
{
	m_mainTexture = value;
}

Texture* Material::mainTexture() const
{
	return m_mainTexture;
}

Texture* Material::normalMap() const
{
    return nullptr;
}

Texture* Material::metallicRoughnessTexture() const {
    return nullptr;
}

Texture* Material::occlusionTexture() const {
    return nullptr;
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
    
    m_unifiedShader2 = shader->m_unifiedShader2;
    kokage::UnifiedShader2* unifiedShader = shader->m_unifiedShader2;
    m_globalConstantBufferMember.clear();
    if (unifiedShader) {
        for (const auto& member : unifiedShader->globalConstantBufferMembers()) {
            GlobalConstantBufferMember m;
            m.value = Vector4(0, 0, 0, 0);
            m_globalConstantBufferMember.push_back(m);
        }
    }
}

Shader* Material::shader() const
{
	return m_shader;
}

int Material::findParameterIndex(const std::string_view& name) const {
    if (m_unifiedShader2) {
        const auto& memberInfos = m_unifiedShader2->globalConstantBufferMembers();
        auto itr = std::find_if(memberInfos.begin(), memberInfos.end(), [&](const auto& e) { return e->name == name; });
        if (itr != memberInfos.end()) {
            return static_cast<int>(itr - memberInfos.begin());
        }
    }
    else {
        //detail::ShaderParameterValue* param = getValue(name);
        //param->setFloat(value);
    }
    return -1;

}

void Material::setInt(const StringView& name, int value)
{
    LN_NOTIMPLEMENTED();
}

void Material::setFloat(int parameterIndex, float value) {
    if (m_unifiedShader2) {
        m_globalConstantBufferMember[parameterIndex].value = Vector4(value, 0, 0, 0);
    }
    else {
        //detail::ShaderParameterValue* param = getValue(name);
        //param->setFloat(value);
    }
}

void Material::setFloatArray(const StringView& name, const float* values, int length) {
    LN_NOTIMPLEMENTED();
}

void Material::setVector(const StringView& name, const Vector4& value) {
    LN_NOTIMPLEMENTED();
}

void Material::setVectorArray(const StringView& name, const Vector4* values, int length) {
    LN_NOTIMPLEMENTED();
}

void Material::setMatrix(const StringView& name, const Matrix& value) {
    LN_NOTIMPLEMENTED();
}

void Material::setTexture(const StringView& name, Texture* value) {
    LN_NOTIMPLEMENTED();
}

void Material::setColor(const StringView& name, const Color& value) {
    LN_NOTIMPLEMENTED();
}

void Material::setBufferData(const StringView& uniformBufferName, const void* data, int size) {
    LN_NOTIMPLEMENTED();
    #if 0
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
    #endif
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

void Material::updateShaderVariables2(GraphicsCommandList* commandList, ShaderDescriptor* descriptor) {
    if (!m_globalConstantBufferMember.empty()) {
        const auto& members = m_unifiedShader2->globalConstantBufferMembers();
        for (int i = 0; i < m_globalConstantBufferMember.size(); i++) {
            const auto& memberInfo = members[i];
            const auto& memberData = m_globalConstantBufferMember[i];
            const auto& bufferView = descriptor->uniformBuffer(0); // TODO: 念のため $Global 見たほうがいいかも
            switch (memberInfo->kind) {
                case kokage::ShaderGlobalMemberKind::ShaderGlobalMemberKind_Scalar:
                    bufferView.setData(&memberData.value.x, sizeof(float));
                    break;
                //case kokage::ShaderGlobalMemberKind::ShaderGlobalMemberKind_Scalar:
                //    bufferView.setData(&memberData.value.x, sizeof(float));
                //    break;
                default:
                    LN_NOTIMPLEMENTED();
                    break;
            }
        }
    }
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
//	return makeObject_deprecated<PhongMaterial>();
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


} // namespace ln

