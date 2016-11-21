
#pragma once
#include <unordered_map>
#include "Color.h"
#include "Texture.h"
#include "Shader.h"
#include "RenderState.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
class Material;
using MaterialPtr = RefPtr<Material>;
namespace detail {

class CombinedMaterial;
struct BuiltinParameters
{
	RefPtr<Shader>	shader;
	BlendMode		blendMode;
	CullingMode		cullingMode;
	//FillMode		fill;
	bool			alphaTest;
	bool			depthTestEnabled;
	bool			depthWriteEnabled;
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
	static MaterialPtr Create();

public:
	void SetShader(Shader* shader);
	Shader* GetShader() const { return m_builtin.shader; }
	
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

	void SetBlendMode(BlendMode mode);
	void SetCullingMode(CullingMode mode);

	/** @} */

	//--------------------------------------------------------------------------
	/** @name DepthStencilState */
	/** @{ */

	void SetDepthTestEnabled(bool enabled);
	void SetDepthWriteEnabled(bool enabled);

	/** @} */

LN_INTERNAL_ACCESS:
	Material();
	virtual ~Material();
	void Initialize();
	void Reset();

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

	void SetMaterialTexture(Texture* v);
	Texture* GetMaterialTexture(Texture* defaultValue) const;

	void SetOpacity(float v);
	float GetOpacity() const;

	void SetColorScale(const Color& v);
	Color GetColorScale() const;

	void SetBlendColor(const Color& v);
	Color GetBlendColor() const;

	void SetTone(const ToneF& v);
	ToneF GetTone() const;

	Matrix GetUVTransform() const { /*auto* v = FindShaderValueConst(_T("UVTransform")); return (v) ? v->GetMatrix() : */ return Matrix::Identity; }

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

	

	std::unordered_map<uint32_t, ShaderValue>	m_userValueMap;
	std::unordered_map<uint32_t, ShaderValue>	m_builtinValueMap;



	friend class RenderingContext;

LN_INTERNAL_ACCESS:
	detail::BuiltinParameters	m_builtin;
	int									m_revisionCount;
	uint32_t							m_hashCode;


	const std::unordered_map<uint32_t, ShaderValue>& GetUserValueMap() const { return m_userValueMap; }
	ShaderValue* FindAndCreateUserShaderValue(uint32_t hashKey);
	const ShaderValue* FindUserShaderValueConst(uint32_t hashKey) const;
	const Color& GetColor(uint32_t hashKey, const Color& defaultValue) const { auto* v = FindUserShaderValueConst(hashKey); return (v) ? static_cast<const Color&>(v->GetVector()) : defaultValue; }
	float GetFloat(uint32_t hashKey, float defaultValue) const { auto* v = FindUserShaderValueConst(hashKey); return (v) ? v->GetFloat() : defaultValue; }
	Texture* GetTexture(uint32_t hashKey, Texture* defaultValue) const { auto* v = FindUserShaderValueConst(hashKey); return (v) ? v->GetManagedTexture() : defaultValue; }
	uint32_t GetHashCode();
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


namespace detail {

class CombinedMaterial
	: public RefObject
{
public:
	CombinedMaterial();
	virtual ~CombinedMaterial();

	//Shader*			m_shader;
	BuiltinParameters	m_builtinParameters;
	Color			m_colorScale;	// 乗算結合済み (opacity 込み)
	Color			m_blendColor;	// 加算結合済み
	ToneF			m_tone;			// 加算結合済み

	Color			m_diffuse;
	Color			m_ambient;
	Color			m_specular;
	Color			m_emissive;
	float			m_power;
	Texture*		m_mainTexture;

	//CullingMode		m_culling;

	void Combine(Material* parent, Material* owner, Material* ownerBase);

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
};

} // namespace detail

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END