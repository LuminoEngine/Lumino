
#pragma once
#include <unordered_map>
#include "Color.h"
#include "Texture.h"
#include "Shader.h"
#include "RenderState.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
class Material;
class DiffuseMaterial;
using MaterialPtr = RefPtr<Material>;
using DiffuseMaterialPtr = RefPtr<DiffuseMaterial>;

namespace detail {

class CombinedMaterial;


struct BuiltinEffectData
{
public:
	static const BuiltinEffectData DefaultData;

	BuiltinEffectData()
	{
		m_shader = nullptr;
		m_opacity = 1.0f;
		m_colorScale = Color(1, 1, 1, 1);
		m_blendColor = Color(0, 0, 0, 0);
		m_tone = ToneF(0, 0, 0, 0);
		m_hashCode = 0;
		m_hashDirty = true;
	}

	void reset()
	{
		setShader(nullptr);
		setOpacity(1.0f);
		setColorScale(Color(1, 1, 1, 1));
		setBlendColor(Color(0, 0, 0, 0));
		setTone(ToneF(0, 0, 0, 0));
	}

	void setShader(Shader* value)
	{
		if (m_shader != value)
		{
			m_shader = value;
			m_hashDirty = true;
		}
	}

	Shader* getShader() const
	{
		return m_shader;
	}

	void setOpacity(float value)
	{
		if (m_opacity != value)
		{
			m_opacity = value;
			m_hashDirty = true;
		}
	}

	float getOpacity() const
	{
		return m_opacity;
	}

	void setColorScale(const Color& value)
	{
		if (m_colorScale != value)
		{
			m_colorScale = value;
			m_hashDirty = true;
		}
	}

	const Color& getColorScale() const
	{
		return m_colorScale;
	}

	void setBlendColor(const Color& value)
	{
		if (m_blendColor != value)
		{
			m_blendColor = value;
			m_hashDirty = true;
		}
	}

	const Color& getBlendColor() const
	{
		return m_blendColor;
	}

	void setTone(const ToneF& value)
	{
		if (m_tone != value)
		{
			m_tone = value;
			m_hashDirty = true;
		}
	}

	const ToneF& getTone() const
	{
		return m_tone;
	}

	bool equals(const BuiltinEffectData& rhs) const
	{
		return	m_shader == rhs.m_shader &&
			m_colorScale == rhs.m_colorScale &&
			m_blendColor == rhs.m_blendColor &&
			m_tone == rhs.m_tone;
	}

	uint32_t getHashCode() const
	{
		uint32_t hash = 0;
		hash += reinterpret_cast<intptr_t>(m_shader.get());
		hash += *reinterpret_cast<const uint32_t*>(&m_opacity);
		hash += Hash::calcHash(reinterpret_cast<const char*>(&m_colorScale), sizeof(m_colorScale));	// TODO: template
		hash += Hash::calcHash(reinterpret_cast<const char*>(&m_blendColor), sizeof(m_blendColor));
		hash += Hash::calcHash(reinterpret_cast<const char*>(&m_tone), sizeof(m_tone));
		return hash;
	}

	static void combine(const BuiltinEffectData& parent, const BuiltinEffectData& local, BuiltinEffectData* outData)
	{
		*outData = local;
		outData->m_opacity *= parent.m_opacity;
		outData->m_colorScale.multiplyClamp(parent.m_colorScale);
		outData->m_blendColor.addClamp(parent.m_blendColor);
		outData->m_tone.addClamp(parent.m_tone);
		outData->m_hashDirty = true;
	}

private:
	RefPtr<Shader>	m_shader;		// default shader (on VisualComponent, マテリアルの shader が null のときに使われる)
	float			m_opacity;
	Color			m_colorScale;
	Color			m_blendColor;
	ToneF			m_tone;
	mutable size_t	m_hashCode;
	mutable bool	m_hashDirty;
};

} // namespace detail

/**
	@brief
*/
class Material
	: public Object
{
	LN_OBJECT();
public:
	static const String DiffuseParameter;
	static const String AmbientParameter;
	static const String SpecularParameter;
	static const String EmissiveParameter;
	static const String PowerParameter;
	static const String MaterialTextureParameter;

public:
	static MaterialPtr create();

public:
	void setShader(Shader* shader);
	Shader* getShader() const;
	
	void setIntParameter(const StringRef& name, int value);
	void setFloatParameter(const StringRef& name, float value);
	void setVectorParameter(const StringRef& name, const Vector4& value);
	void setMatrixParameter(const StringRef& name, const Matrix& value);
	void setTextureParameter(const StringRef& name, Texture* value);
	void setColorParameter(const StringRef& name, const Color& value);
	void setColorParameter(const StringRef& name, float r, float g, float b, float a);

	//--------------------------------------------------------------------------
	/** @name RenderState */
	/** @{ */

private:
	Nullable<BlendMode>		blendMode;
	Nullable<CullingMode>	cullingMode;
	Nullable<bool>			depthTestEnabled;
	Nullable<bool>			depthWriteEnabled;

public:

	void setBlendMode(Nullable<BlendMode> mode);
	Nullable<BlendMode> getBlendMode() const { return blendMode; }

	void setCullingMode(Nullable<CullingMode> mode);
	Nullable<CullingMode> getCullingMode() const { return cullingMode; }

	void setDepthTestEnabled(Nullable<bool> enabled);
	Nullable<bool> isDepthTestEnabled() const { return depthTestEnabled; }

	void setDepthWriteEnabled(Nullable<bool> enabled);
	Nullable<bool> isDepthWriteEnabled() const { return depthWriteEnabled; }


LN_INTERNAL_ACCESS:
	Material();
	virtual ~Material();
	void initialize();
	void reset();

	void setBuiltinIntParameter(const StringRef& name, int value);
	void setBuiltinFloatParameter(const StringRef& name, float value);
	void setBuiltinVectorParameter(const StringRef& name, const Vector4& value);
	void setBuiltinMatrixParameter(const StringRef& name, const Matrix& value);
	void setBuiltinTextureParameter(const StringRef& name, Texture* value);
	void setBuiltinColorParameter(const StringRef& name, const Color& value);
	void setBuiltinColorParameter(const StringRef& name, float r, float g, float b, float a);

LN_INTERNAL_ACCESS:
	//using ShaderValuePtr = std::shared_ptr<ShaderValue>;

	//struct ValuePair
	//{
	//	ShaderVariable*	variable;
	//	ShaderValuePtr	value;
	//};

	//const List<ValuePair>& GetLinkedVariableList() { return m_linkedVariableList; }

	RefPtr<Material> copyShared() const;

	//void ResolveCombinedMaterial();
	//detail::CombinedMaterial* getCombinedMaterial() const;

public:	// TODO:

	// TODO: 他の Builtin パラーメータを追い出したのでこれだけになってしまった。普通のメンバ変数でいいのでは？
	void setMaterialTexture(Texture* v);
	Texture* getMaterialTexture(Texture* defaultValue) const;

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


	static const Color DefaultDiffuse;	// (1.0f, 1.0f, 1.0f, 1.0f)
	static const Color DefaultAmbient;	// (0.0f, 0.0f, 0.0f, 0.0f)
	static const Color DefaultSpecular;	// (0.5f, 0.5f, 0.5f, 0.5f)
	static const Color DefaultEmissive;	// (0.0f, 0.0f, 0.0f, 0.0f)
	static const float DefaultPower;	// (50.0f)

private:
	//void LinkVariables();
	//ShaderValue* FindShaderValue(const StringRef& name);
	//ShaderValue* FindShaderValueConst(const StringRef& name) const;

	static void onRenderStateChanged(Object* obj);
	

	std::unordered_map<uint32_t, ShaderValue>	m_userValueMap;
	std::unordered_map<uint32_t, ShaderValue>	m_builtinValueMap;


LN_INTERNAL_ACCESS:
	RefPtr<Shader>						m_shader;
	int									m_revisionCount;
	uint32_t							m_hashCode;


	const std::unordered_map<uint32_t, ShaderValue>& getUserValueMap() const { return m_userValueMap; }
	ShaderValue* findAndCreateUserShaderValue(uint32_t hashKey);
	const ShaderValue* findUserShaderValueConst(uint32_t hashKey) const;
	//const Color& getColor(uint32_t hashKey, const Color& defaultValue) const { auto* v = findUserShaderValueConst(hashKey); return (v) ? static_cast<const Color&>(v->getVector()) : defaultValue; }
	//float GetFloat(uint32_t hashKey, float defaultValue) const { auto* v = findUserShaderValueConst(hashKey); return (v) ? v->GetFloat() : defaultValue; }
	//Texture* getTexture(uint32_t hashKey, Texture* defaultValue) const { auto* v = findUserShaderValueConst(hashKey); return (v) ? v->getManagedTexture() : defaultValue; }
	
	const Color& getBuiltinColor(uint32_t hashKey, const Color& defaultValue) const { auto itr = m_builtinValueMap.find(hashKey); return (itr != m_builtinValueMap.end()) ? static_cast<const Color&>(itr->second.getVector()) : defaultValue; }
	float getBuiltinFloat(uint32_t hashKey, float defaultValue) const { auto itr = m_builtinValueMap.find(hashKey); return (itr != m_builtinValueMap.end()) ? itr->second.getFloat() : defaultValue; }
	Texture* getBuiltinTexture(uint32_t hashKey, Texture* defaultValue) const { auto itr = m_builtinValueMap.find(hashKey); return (itr != m_builtinValueMap.end()) ? itr->second.getManagedTexture() : defaultValue; }

	
	uint32_t getHashCode();
};

/**
	@brief
*/
class MaterialList
	: public tr::ReflectionObjectList<Material*>	// SubMaterials (0 の場合もある)
{
LN_INTERNAL_ACCESS:
	MaterialList();
	virtual ~MaterialList();
};



/**
	@brief		非物理ベースレンダリングで使用されるレガシーなマテリアルです。
*/
class DiffuseMaterial
	: public Material
{
	LN_OBJECT();
public:
	static DiffuseMaterialPtr create();

public:
	void setDiffuse(const Color& value);
	void setAmbient(const Color& value);
	void setSpecular(const Color& value);
	void setEmissive(const Color& value);
	void setSpecularPower(float value);

LN_CONSTRUCT_ACCESS:
	DiffuseMaterial();
	virtual ~DiffuseMaterial();
	void initialize();
};


namespace detail {

class CombinedMaterial
	: public RefObject
{
public:
	CombinedMaterial();
	virtual ~CombinedMaterial();

	RefPtr<Shader>	m_shader;
	Color			m_colorScale;	// 乗算結合済み (opacity 込み)
	Color			m_blendColor;	// 加算結合済み
	ToneF			m_tone;			// 加算結合済み

	Color			m_diffuse;
	Color			m_ambient;
	Color			m_specular;
	Color			m_emissive;
	float			m_power;
	Texture*		m_mainTexture;

	Nullable<BlendMode>		m_blendMode;
	Nullable<CullingMode>	m_cullingMode;
	Nullable<bool>			m_depthTestEnabled;
	Nullable<bool>			m_depthWriteEnabled;

	void combine(/*Material* owner, */Material* ownerBase, const BuiltinEffectData& builtinEffectData);
	void applyUserShaderValeues(Shader* targetShader);

	uint32_t getSourceHashCode() const { return m_lastSourceHashCode; }

private:
	void copyUserValueTable(Material* source);
	void mergeUserValueTable(Material* source);

	struct ValuePair
	{
		uint32_t	nameHash;
		ShaderValue	value;
	};

	List<ValuePair>	m_userValueTable;

	// change monitoring
	//Material*	m_lastSourceMaterial;
	uint32_t	m_lastSourceHashCode;
	uint32_t	m_lastBuiltinEffectHashCode;
};

} // namespace detail

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END