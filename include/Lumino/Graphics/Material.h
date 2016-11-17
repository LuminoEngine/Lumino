
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
namespace detail { class CombinedMaterial; }

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
	Shader* GetShader() const { return m_shader; }
	
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
	void SetFillMode(FillMode mode);
	void SetAlphaTestEnabled(bool enabled);

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

LN_INTERNAL_ACCESS:
	using ShaderValuePtr = std::shared_ptr<ShaderValue>;

	struct ValuePair
	{
		ShaderVariable*	variable;
		ShaderValuePtr	value;
	};

	const List<ValuePair>& GetLinkedVariableList() { return m_linkedVariableList; }

	RefPtr<Material> CopyShared() const;

	void ResolveCombinedMaterial();
	detail::CombinedMaterial* GetCombinedMaterial() const;

public:	// TODO:

	void SetMaterialTexture(Texture* v);
	Texture* GetMaterialTexture() const;

	void SetOpacity(float v);
	float GetOpacity() const;

	void SetColorScale(const Color& v);
	Color GetColorScale() const;

	void SetBlendColor(const Color& v);
	Color GetBlendColor() const;

	void SetTone(const ToneF& v);
	ToneF GetTone() const;

	Matrix GetUVTransform() const { auto* v = FindShaderValueConst(_T("UVTransform")); return (v) ? v->GetMatrix() : Matrix::Identity; }

	Color GetColor(const StringRef& name, const Color& defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? Color(v->GetVector()) : defaultValue; }
	float GetFloat(const StringRef& name, float defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? v->GetFloat() : defaultValue; }
	Texture* GetTexture(const StringRef& name, Texture* defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? v->GetManagedTexture() : defaultValue; }
	int GetInt(const StringRef& name, int defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? v->GetInt() : defaultValue; }


	static const Color DefaultDiffuse;	// (1.0f, 1.0f, 1.0f, 1.0f)
	static const Color DefaultAmbient;	// (0.0f, 0.0f, 0.0f, 0.0f)
	static const Color DefaultSpecular;	// (0.5f, 0.5f, 0.5f, 0.5f)
	static const Color DefaultEmissive;	// (0.0f, 0.0f, 0.0f, 0.0f)
	static const float DefaultPower;	// (50.0f)

private:
	void LinkVariables();
	ShaderValue* FindShaderValue(const StringRef& name);
	ShaderValue* FindShaderValueConst(const StringRef& name) const;

	std::unordered_map<uint32_t, ShaderValue>	m_userValueMap;
	std::unordered_map<uint32_t, ShaderValue>	m_builtinValueMap;


	RefPtr<detail::CombinedMaterial>	m_combinedMaterial;
	RefPtr<Shader>						m_shader;
	SortedArray<String, ShaderValuePtr>	m_valueList;
	List<ValuePair>					m_linkedVariableList;
	
	BlendMode							m_blendMode;
	CullingMode							m_culling;
	FillMode							m_fill;
	bool								m_alphaTest;
	bool								m_depthTestEnabled;
	bool								m_depthWriteEnabled;

	bool								m_shaderModified;

	friend class RenderingContext;

LN_INTERNAL_ACCESS:
	bool								m_modifiedForMaterialInstance;

	void ApplyToShaderVariables();

	const std::unordered_map<uint32_t, ShaderValue>& GetUserValueMap() const { return m_userValueMap; }
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

	struct ValuePair
	{
		uint32_t	nameHash;
		ShaderValue	value;
	};

	List<ValuePair>	m_userValueTable;

	Shader*			m_shader;
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

private:
	void CopyUserValueTable(Material* source);
	void MergeUserValueTable(Material* source);
};

} // namespace detail

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END