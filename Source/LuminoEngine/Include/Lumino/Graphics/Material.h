
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

	void Reset()
	{
		SetShader(nullptr);
		SetOpacity(1.0f);
		SetColorScale(Color(1, 1, 1, 1));
		SetBlendColor(Color(0, 0, 0, 0));
		SetTone(ToneF(0, 0, 0, 0));
	}

	void SetShader(Shader* value)
	{
		if (m_shader != value)
		{
			m_shader = value;
			m_hashDirty = true;
		}
	}

	Shader* GetShader() const
	{
		return m_shader;
	}

	void SetOpacity(float value)
	{
		if (m_opacity != value)
		{
			m_opacity = value;
			m_hashDirty = true;
		}
	}

	float GetOpacity() const
	{
		return m_opacity;
	}

	void SetColorScale(const Color& value)
	{
		if (m_colorScale != value)
		{
			m_colorScale = value;
			m_hashDirty = true;
		}
	}

	const Color& GetColorScale() const
	{
		return m_colorScale;
	}

	void SetBlendColor(const Color& value)
	{
		if (m_blendColor != value)
		{
			m_blendColor = value;
			m_hashDirty = true;
		}
	}

	const Color& GetBlendColor() const
	{
		return m_blendColor;
	}

	void SetTone(const ToneF& value)
	{
		if (m_tone != value)
		{
			m_tone = value;
			m_hashDirty = true;
		}
	}

	const ToneF& GetTone() const
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
		outData->m_colorScale.MultiplyClamp(parent.m_colorScale);
		outData->m_blendColor.AddClamp(parent.m_blendColor);
		outData->m_tone.AddClamp(parent.m_tone);
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
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
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
	void SetShader(Shader* shader);
	Shader* GetShader() const;
	
	void SetIntParameter(const StringRef& name, int value);
	void SetFloatParameter(const StringRef& name, float value);
	void SetVectorParameter(const StringRef& name, const Vector4& value);
	void SetMatrixParameter(const StringRef& name, const Matrix& value);
	void SetTextureParameter(const StringRef& name, Texture* value);
	void SetColorParameter(const StringRef& name, const Color& value);
	void SetColorParameter(const StringRef& name, float r, float g, float b, float a);

	//--------------------------------------------------------------------------
	/** @name RenderState */
	/** @{ */

private:
	Nullable<BlendMode>		blendMode;
	Nullable<CullingMode>	cullingMode;
	Nullable<bool>			depthTestEnabled;
	Nullable<bool>			depthWriteEnabled;

public:

	void SetBlendMode(Nullable<BlendMode> mode);
	Nullable<BlendMode> GetBlendMode() const { return blendMode; }

	void SetCullingMode(Nullable<CullingMode> mode);
	Nullable<CullingMode> GetCullingMode() const { return cullingMode; }

	void SetDepthTestEnabled(Nullable<bool> enabled);
	Nullable<bool> IsDepthTestEnabled() const { return depthTestEnabled; }

	void SetDepthWriteEnabled(Nullable<bool> enabled);
	Nullable<bool> IsDepthWriteEnabled() const { return depthWriteEnabled; }


LN_INTERNAL_ACCESS:
	Material();
	virtual ~Material();
	void initialize();
	void Reset();

	void SetBuiltinIntParameter(const StringRef& name, int value);
	void SetBuiltinFloatParameter(const StringRef& name, float value);
	void SetBuiltinVectorParameter(const StringRef& name, const Vector4& value);
	void SetBuiltinMatrixParameter(const StringRef& name, const Matrix& value);
	void SetBuiltinTextureParameter(const StringRef& name, Texture* value);
	void SetBuiltinColorParameter(const StringRef& name, const Color& value);
	void SetBuiltinColorParameter(const StringRef& name, float r, float g, float b, float a);

LN_INTERNAL_ACCESS:
	//using ShaderValuePtr = std::shared_ptr<ShaderValue>;

	//struct ValuePair
	//{
	//	ShaderVariable*	variable;
	//	ShaderValuePtr	value;
	//};

	//const List<ValuePair>& GetLinkedVariableList() { return m_linkedVariableList; }

	RefPtr<Material> CopyShared() const;

	//void ResolveCombinedMaterial();
	//detail::CombinedMaterial* GetCombinedMaterial() const;

public:	// TODO:

	// TODO: 他の Builtin パラーメータを追い出したのでこれだけになってしまった。普通のメンバ変数でいいのでは？
	void SetMaterialTexture(Texture* v);
	Texture* GetMaterialTexture(Texture* defaultValue) const;

	//void SetOpacity(float v);
	//float GetOpacity() const;

	//void SetColorScale(const Color& v);
	//Color GetColorScale() const;

	//void SetBlendColor(const Color& v);
	//Color GetBlendColor() const;

	//void SetTone(const ToneF& v);
	//ToneF GetTone() const;

	//Matrix GetUVTransform() const { /*auto* v = FindShaderValueConst(_T("UVTransform")); return (v) ? v->GetMatrix() : */ return Matrix::Identity; }

	//Color GetColor(const StringRef& name, const Color& defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? Color(v->GetVector()) : defaultValue; }
	//float GetFloat(const StringRef& name, float defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? v->GetFloat() : defaultValue; }
	//Texture* GetTexture(const StringRef& name, Texture* defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? v->GetManagedTexture() : defaultValue; }
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

	static void OnRenderStateChanged(Object* obj);
	

	std::unordered_map<uint32_t, ShaderValue>	m_userValueMap;
	std::unordered_map<uint32_t, ShaderValue>	m_builtinValueMap;


LN_INTERNAL_ACCESS:
	RefPtr<Shader>						m_shader;
	int									m_revisionCount;
	uint32_t							m_hashCode;


	const std::unordered_map<uint32_t, ShaderValue>& GetUserValueMap() const { return m_userValueMap; }
	ShaderValue* FindAndCreateUserShaderValue(uint32_t hashKey);
	const ShaderValue* FindUserShaderValueConst(uint32_t hashKey) const;
	//const Color& GetColor(uint32_t hashKey, const Color& defaultValue) const { auto* v = FindUserShaderValueConst(hashKey); return (v) ? static_cast<const Color&>(v->GetVector()) : defaultValue; }
	//float GetFloat(uint32_t hashKey, float defaultValue) const { auto* v = FindUserShaderValueConst(hashKey); return (v) ? v->GetFloat() : defaultValue; }
	//Texture* GetTexture(uint32_t hashKey, Texture* defaultValue) const { auto* v = FindUserShaderValueConst(hashKey); return (v) ? v->GetManagedTexture() : defaultValue; }
	
	const Color& GetBuiltinColor(uint32_t hashKey, const Color& defaultValue) const { auto itr = m_builtinValueMap.find(hashKey); return (itr != m_builtinValueMap.end()) ? static_cast<const Color&>(itr->second.GetVector()) : defaultValue; }
	float GetBuiltinFloat(uint32_t hashKey, float defaultValue) const { auto itr = m_builtinValueMap.find(hashKey); return (itr != m_builtinValueMap.end()) ? itr->second.getFloat() : defaultValue; }
	Texture* GetBuiltinTexture(uint32_t hashKey, Texture* defaultValue) const { auto itr = m_builtinValueMap.find(hashKey); return (itr != m_builtinValueMap.end()) ? itr->second.GetManagedTexture() : defaultValue; }

	
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
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static DiffuseMaterialPtr create();

public:
	void SetDiffuse(const Color& value);
	void SetAmbient(const Color& value);
	void SetSpecular(const Color& value);
	void SetEmissive(const Color& value);
	void SetSpecularPower(float value);

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

	void combine(Material* owner, Material* ownerBase, const BuiltinEffectData& builtinEffectData);
	void ApplyUserShaderValeues(Shader* targetShader);

	uint32_t GetSourceHashCode() const { return m_lastSourceHashCode; }

private:
	void CopyUserValueTable(Material* source);
	void MergeUserValueTable(Material* source);

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