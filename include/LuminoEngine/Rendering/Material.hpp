#pragma once
#include "Common.hpp"
#include "../Graphics/ColorStructs.hpp"
#include "../Graphics/RenderState.hpp"
#include "../Shader/Shader.hpp"
#include "../Shader/ShaderInterfaceFramework.hpp"

namespace ln {
namespace detail {

enum class MaterialType : uint8_t
{
	PBR,
	Phong,
};

} // namespace detail

/**
	@brief
*/
class AbstractMaterial
	: public Object
{
	//LN_OBJECT;
//public:
//	static const String DiffuseParameter;
//	static const String AmbientParameter;
//	static const String SpecularParameter;
//	static const String EmissiveParameter;
//	static const String PowerParameter;
//	static const String MaterialTextureParameter;

public:
	void setMainTexture(Texture* value);
	Texture* mainTexture() const;

	void setShader(Shader* shader);
	Shader* shader() const;

	void setInt(const StringRef& name, int value);
	void setFloat(const StringRef& name, float value);
	void setVector(const StringRef& name, const Vector4& value);
	void setMatrix(const StringRef& name, const Matrix& value);
	void setTexture(const StringRef& name, Texture* value);
	void setColor(const StringRef& name, const Color& value);

	//--------------------------------------------------------------------------
	/** @name RenderState */
	/** @{ */

private:
	Optional<BlendMode>		blendMode;
	Optional<CullMode>	cullingMode;
	Optional<bool>			depthTestEnabled;
	Optional<bool>			depthWriteEnabled;

public:
	ShadingModel			shadingModel = ShadingModel::Default;

	void setBlendMode(Optional<BlendMode> mode);
	Optional<BlendMode> getBlendMode() const { return blendMode; }

	void setCullingMode(Optional<CullMode> mode);
	Optional<CullMode> getCullingMode() const { return cullingMode; }

	void setDepthTestEnabled(Optional<bool> enabled);
	Optional<bool> isDepthTestEnabled() const { return depthTestEnabled; }

	void setDepthWriteEnabled(Optional<bool> enabled);
	Optional<bool> isDepthWriteEnabled() const { return depthWriteEnabled; }


protected:
	AbstractMaterial(detail::MaterialType type);
	virtual ~AbstractMaterial();
	void init();

public: // TODO: internal
	virtual void translateToPBRMaterialData(detail::PbrMaterialData* outData) = 0;
	virtual void translateToPhongMaterialData(detail::PhongMaterialData* outData) = 0;

//LN_INTERNAL_ACCESS:
//	void reset();
//
//	void setBuiltinIntParameter(const StringRef& name, int value);
//	void setBuiltinFloatParameter(const StringRef& name, float value);
//	void setBuiltinVectorParameter(const StringRef& name, const Vector4& value);
//	void setBuiltinMatrixParameter(const StringRef& name, const Matrix& value);
//	void setBuiltinTextureParameter(const StringRef& name, Texture* value);
//	void setBuiltinColorParameter(const StringRef& name, const Color& value);
//	void setBuiltinColorParameter(const StringRef& name, float r, float g, float b, float a);

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

	//void setTone(const ToneF& v);
	//ToneF getTone() const;

	//Matrix GetUVTransform() const { /*auto* v = FindShaderValueConst(_T("UVTransform")); return (v) ? v->getMatrix() : */ return Matrix::Identity; }

	//Color getColor(const StringRef& name, const Color& defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? Color(v->getVector()) : defaultValue; }
	//float GetFloat(const StringRef& name, float defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? v->GetFloat() : defaultValue; }
	//Texture* getTexture(const StringRef& name, Texture* defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? v->getManagedTexture() : defaultValue; }
	//int GetInt(const StringRef& name, int defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? v->GetInt() : defaultValue; }


	//static const Color DefaultDiffuse;	// (1.0f, 1.0f, 1.0f, 1.0f)
	//static const Color DefaultAmbient;	// (0.0f, 0.0f, 0.0f, 0.0f)
	//static const Color DefaultSpecular;	// (0.5f, 0.5f, 0.5f, 0.5f)
	//static const Color DefaultEmissive;	// (0.0f, 0.0f, 0.0f, 0.0f)
	//static const float DefaultPower;	// (50.0f)

    // TODO: internal
    void updateShaderVariables(Shader* target);

private:
	//void LinkVariables();
	//ShaderValue* FindShaderValue(const StringRef& name);
	//ShaderValue* FindShaderValueConst(const StringRef& name) const;

	//static void onRenderStateChanged(Object* obj);

	detail::ShaderParameterValue* getValue(const ln::StringRef& name);

	detail::MaterialType m_type;
	Ref<Shader> m_shader;
	Ref<Texture> m_mainTexture;
	std::vector<std::pair<String, std::shared_ptr<detail::ShaderParameterValue>>> m_values;

protected:


LN_INTERNAL_ACCESS:

	bool equalStatus(const AbstractMaterial* other) const
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
};

/**
 * 標準的な物理ベースレンダリングのマテリアルです。
 */
class Material
	: public AbstractMaterial
{
	//LN_OBJECT;
public:
	static Ref<Material> create();
    static Ref<Material> create(Texture* mainTexture);
    static Ref<Material> create(Texture* mainTexture, ShadingModel shadingModel);

public:
	void setColor(const Color& value);
	void setRoughness(float value);
	void setMetallic(float value);
	//void setSpecular(float value);  // TODO: 不要？http://envgameartist.blogspot.com/2014/12/pbr.html
    void setEmissive(const Color& value);

protected:
	virtual void translateToPBRMaterialData(detail::PbrMaterialData* outData) override;
	virtual void translateToPhongMaterialData(detail::PhongMaterialData* outData) override;

LN_CONSTRUCT_ACCESS:
	Material();
	virtual ~Material();
	void init();
    void init(Texture* mainTexture);
    void init(Texture* mainTexture, ShadingModel shadingModel);
    void init(Texture* mainTexture, const detail::PhongMaterialData& phongMaterialData);

private:
	detail::PbrMaterialData m_data;
};


/** 非物理ベースレンダリングで使用される Phong シェーディング用のマテリアルです。 */
class PhongMaterial
	: public AbstractMaterial
{
	//LN_OBJECT;
public:
	static const String DiffuseParameterName;
	static const String AmbientParameterName;
	static const String EmissiveParameterName;
	static const String SpecularParameterName;
	static const String SpecularPowerParameterName;

	static const Color DefaultDiffuse;
	static const Color DefaultAmbient;
	static const Color DefaultSpecular;
	static const Color DefaultEmissive;
	static const float DefaultPower;

	static Ref<PhongMaterial> create();

	void setDiffuse(const Color& value);
	void setAmbient(const Color& value);
	void setEmissive(const Color& value);
	void setSpecular(const Color& value);
	void setSpecularPower(float value);

protected:
	virtual void translateToPBRMaterialData(detail::PbrMaterialData* outData) override;
	virtual void translateToPhongMaterialData(detail::PhongMaterialData* outData) override;

LN_CONSTRUCT_ACCESS:
	PhongMaterial();
	virtual ~PhongMaterial();
	void init();

private:
	detail::PhongMaterialData m_data;
};

} // namespace ln

