#pragma once
#include "Common.hpp"
#include <LuminoEngine/Base/Builder.hpp>
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoGraphics/RHI/Shader.hpp>
#include <LuminoGraphics/RHI/ShaderInterfaceFramework.hpp>
#include <LuminoGraphics/RHI/ShaderParameterValue.hpp>

namespace ln {

/**
 *
 * ### metallic-roughness テクスチャ
 * Metallic 値は B チャネル、Roughness 値は G チャネルからサンプリングされます。
 * https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#pbrmetallicroughnessmetallicroughnesstexture
 * 
 * 
 */
// TODO: フレーム開始～描画に使われた後にパラメータを変更できないようにする。わかりにくい不具合のもとになる。
LN_CLASS()
class Material
	: public Object
{
	LN_OBJECT;
	LN_BUILDER;
public:
	static Material* defaultMaterial();

	static Ref<Material> create();
	static Ref<Material> create(Texture* mainTexture);
	static Ref<Material> create(Texture* mainTexture, ShadingModel shadingModel);
	//LN_OBJECT;
//public:
//	static const String DiffuseParameter;
//	static const String AmbientParameter;
//	static const String SpecularParameter;
//	static const String EmissiveParameter;
//	static const String PowerParameter;
//	static const String MaterialTextureParameter;

public:
	/** mainTexture */
	LN_METHOD(Property)
	void setMainTexture(Texture* value);

	/** mainTexture */
	LN_METHOD(Property)
	Texture* mainTexture() const;


	void setNormalMap(Texture* value);
	Texture* normalMap() const;

	/** metallic-roughness テクスチャを設定します。 */
	void setMetallicRoughnessTexture(Texture* value);

	/** metallic-roughness テクスチャを取得します。 */
	Texture* metallicRoughnessTexture() const;

	/** occlusion テクスチャを設定します。 */
	void setOcclusionTexture(Texture* value);

	/** occlusion テクスチャを取得します。 */
	Texture* occlusionTexture() const;

	//metallicRoughnessTexture

	/** setColor */
	LN_METHOD(Property)
	void setColor(const Color& value);

	/** setRoughness */
	LN_METHOD(Property)
	void setRoughness(float value);

	/** setMetallic */
	LN_METHOD(Property)
	void setMetallic(float value);

	/** setEmissive */
	LN_METHOD(Property)
	void setEmissive(const Color& value);


	
	/** setShadingModel */
	LN_METHOD(Property)
	void setShadingModel(ShadingModel value) { m_shadingModel = value; }

	/** shadingModel */
	LN_METHOD(Property)
	ShadingModel shadingModel() const { return m_shadingModel; }
	
    /** shader */
	LN_METHOD(Property)
	void setShader(Shader* shader);

	/** shader */
	LN_METHOD(Property)
	Shader* shader() const;



	/** 整数値を設定します。 */
	void setInt(const StringView& name, int value);

	/** 整数値の配列を設定します。 */
	void setIntArray(const StringView& name, const int* value, int count);

	/** 浮動小数点値を設定します。 */
	void setFloat(const StringView& name, float value);

	/** 浮動小数点値の配列を設定します。 */
	void setFloatArray(const StringView& name, const float* values, int length);
	void setVector(const StringView& name, const Vector4& value);
	void setVectorArray(const StringView& name, const Vector4* values, int length);
	void setMatrix(const StringView& name, const Matrix& value);
	void setTexture(const StringView& name, Texture* value);
	void setColor(const StringView& name, const Color& value);

	void setBufferData(const StringView& uniformBufferName, const void* data, int size);

	//--------------------------------------------------------------------------
	/** @name RenderState */
	/** @{ */

private:
	Optional_deprecated<BlendMode>		blendMode;
	Optional_deprecated<CullMode>	cullingMode;
	Optional_deprecated<bool>			depthTestEnabled;
	Optional_deprecated<bool>			depthWriteEnabled;

public:
	String m_name;
	ShadingModel			m_shadingModel = ShadingModel::Default;

	void setBlendMode(Optional_deprecated<BlendMode> mode);
	Optional_deprecated<BlendMode> getBlendMode() const { return blendMode; }

	void setCullingMode(Optional_deprecated<CullMode> mode);
	Optional_deprecated<CullMode> getCullingMode() const { return cullingMode; }

	void setDepthTestEnabled(Optional_deprecated<bool> enabled);
	Optional_deprecated<bool> isDepthTestEnabled() const { return depthTestEnabled; }

	void setDepthWriteEnabled(Optional_deprecated<bool> enabled);
	Optional_deprecated<bool> isDepthWriteEnabled() const { return depthWriteEnabled; }


LN_CONSTRUCT_ACCESS:
	Material();
	virtual ~Material();

	/** init */
	LN_METHOD()
	void init();

	void init(Texture* mainTexture);
	void init(Texture* mainTexture, ShadingModel shadingModel);
	void init(Texture* mainTexture, const detail::PhongMaterialData& phongMaterialData);

//LN_INTERNAL_ACCESS:
//	void reset();
//
//	void setBuiltinIntParameter(const StringView& name, int value);
//	void setBuiltinFloatParameter(const StringView& name, float value);
//	void setBuiltinVectorParameter(const StringView& name, const Vector4& value);
//	void setBuiltinMatrixParameter(const StringView& name, const Matrix& value);
//	void setBuiltinTextureParameter(const StringView& name, Texture* value);
//	void setBuiltinColorParameter(const StringView& name, const Color& value);
//	void setBuiltinColorParameter(const StringView& name, float r, float g, float b, float a);

LN_PROTECTED_INTERNAL_ACCESS:

LN_INTERNAL_ACCESS:
	//using ShaderValuePtr = std::shared_ptr<ShaderValue>;

	//struct ValuePair
	//{
	//	ShaderVariable*	variable;
	//	ShaderValuePtr	value;
	//};

	//const List<ValuePair>& GetLinkedVariableList() { return m_linkedVariableList; }

	//Ref<CommonMaterial> copyShared() const;

	//void ResolveCombinedMaterial();
	//detail::CombinedMaterial* getCombinedMaterial() const;

public:	// TODO:

	// TODO: 他の Builtin パラーメータを追い出したのでこれだけになってしまった。普通のメンバ変数でいいのでは？
	//void setMaterialTexture(Texture* v);
	//Texture* getMaterialTexture(Texture* defaultValue) const;

	//void setOpacity(float v);
	//float getOpacity() const;

	//void setColorScale(const Color& v);
	//Color getColorScale() const;

	//void setBlendColor(const Color& v);
	//Color getBlendColor() const;

	//void setTone(const ColorTone& v);
	//ColorTone getTone() const;

	//Matrix GetUVTransform() const { /*auto* v = FindShaderValueConst(_T("UVTransform")); return (v) ? v->getMatrix() : */ return Matrix::Identity; }

	//Color getColor(const StringView& name, const Color& defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? Color(v->getVector()) : defaultValue; }
	//float GetFloat(const StringView& name, float defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? v->GetFloat() : defaultValue; }
	//Texture* getTexture(const StringView& name, Texture* defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? v->getManagedTexture() : defaultValue; }
	//int GetInt(const StringView& name, int defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? v->GetInt() : defaultValue; }


	//static const Color DefaultDiffuse;	// (1.0f, 1.0f, 1.0f, 1.0f)
	//static const Color DefaultAmbient;	// (0.0f, 0.0f, 0.0f, 0.0f)
	//static const Color DefaultSpecular;	// (0.5f, 0.5f, 0.5f, 0.5f)
	//static const Color DefaultEmissive;	// (0.0f, 0.0f, 0.0f, 0.0f)
	//static const float DefaultPower;	// (50.0f)

    // TODO: internal
    void updateShaderVariables(GraphicsCommandList* commandList, detail::ShaderSecondaryDescriptor* descriptor);
    void updateShaderVariables2(GraphicsCommandList* commandList, ShaderDescriptor* descriptor);

protected:  // TODO:
	struct UniformBufferEntiry
	{
		String name;
		Ref<ByteBuffer> data;
		int descriptorIndex = -1;
	};

	//void LinkVariables();
	//ShaderValue* FindShaderValue(const StringView& name);
	//ShaderValue* FindShaderValueConst(const StringView& name) const;

	//static void onRenderStateChanged(Object* obj);

	detail::ShaderParameterValue* getValue(const ln::StringView& name);

	Ref<Shader> m_shader;
	Ref<Texture> m_mainTexture;
	Ref<Texture> m_normalMap;
	Ref<Texture> m_metallicRoughnessTexture;
	Ref<Texture> m_occlusionTexture;
	std::vector<std::pair<String, std::shared_ptr<detail::ShaderParameterValue>>> m_values;
	std::vector<UniformBufferEntiry> m_uniformBufferData;

protected:
	void serialize_deprecated(Serializer2_deprecated& ar) override;

LN_INTERNAL_ACCESS:

	bool equalStatus(const Material* other) const
	{
		if (LN_REQUIRE(other)) return false;
		if (this == other) return true;
		return
			m_shader == other->m_shader &&
			blendMode == other->blendMode &&
			cullingMode == other->cullingMode &&
			depthTestEnabled == other->depthTestEnabled &&
			depthWriteEnabled == other->depthWriteEnabled;
	}

	//int									m_revisionCount;
	//uint32_t							m_hashCode;


	//const std::unordered_map<uint32_t, ShaderValue>& getUserValueMap() const { return m_userValueMap; }
	//ShaderValue* findAndCreateUserShaderValue(uint32_t hashKey);
	//const ShaderValue* findUserShaderValueConst(uint32_t hashKey) const;
	////const Color& getColor(uint32_t hashKey, const Color& defaultValue) const { auto* v = findUserShaderValueConst(hashKey); return (v) ? static_cast<const Color&>(v->getVector()) : defaultValue; }
	////float GetFloat(uint32_t hashKey, float defaultValue) const { auto* v = findUserShaderValueConst(hashKey); return (v) ? v->GetFloat() : defaultValue; }
	////Texture* getTexture(uint32_t hashKey, Texture* defaultValue) const { auto* v = findUserShaderValueConst(hashKey); return (v) ? v->getManagedTexture() : defaultValue; }

	//const Color& getBuiltinColor(uint32_t hashKey, const Color& defaultValue) const { auto itr = m_builtinValueMap.find(hashKey); return (itr != m_builtinValueMap.end()) ? static_cast<const Color&>(itr->second.getVector()) : defaultValue; }
	//float getBuiltinFloat(uint32_t hashKey, float defaultValue) const { auto itr = m_builtinValueMap.find(hashKey); return (itr != m_builtinValueMap.end()) ? itr->second.getFloat() : defaultValue; }
	//Texture* getBuiltinTexture(uint32_t hashKey, Texture* defaultValue) const { auto itr = m_builtinValueMap.find(hashKey); return (itr != m_builtinValueMap.end()) ? itr->second.getManagedTexture() : defaultValue; }

	//void applyUserShaderValeues(Shader* targetShader);

	//uint32_t getHashCode();

	const detail::PbrMaterialData& getPbrMaterialData() const { return m_data; }
private:

	detail::PbrMaterialData m_data;
	bool m_needRefreshShaderBinding;

	friend class detail::SceneRenderer;
};


//==============================================================================
// Material::Builder

struct Material::BuilderDetails : public AbstractBuilderDetails
{
	LN_BUILDER_DETAILS(Material);

	Color color;
	float roughness;
	float metallic;

	BuilderDetails();
	void apply(Material* p) const;
};

template<class T, class B, class D>
struct Material::BuilderCore : public AbstractBuilder<T, B, D>
{
	LN_BUILDER_CORE(AbstractBuilder);

	B& color(const Color& value) { d()->color = value; return self(); }

	B& color(float r, float g, float b, float a = 1.0f) { d()->color = Color(r, g, b, a); return self(); }

	B& roughness(float value) { d()->roughness = value; return self(); }

	B& metallic(float value) { d()->metallic = value; return self(); }
};

LN_BUILDER_IMPLEMENT(Material);

} // namespace ln

