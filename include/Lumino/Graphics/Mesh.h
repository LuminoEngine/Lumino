
#pragma once
#include <Lumino/Graphics/Common.h>
#include "Vertex.h"

#include "Shader.h"	// TODO: for Material

LN_NAMESPACE_BEGIN
class VertexDeclaration;
class StaticMeshModel;
class Material3;
using StaticMeshModelPtr = RefPtr<StaticMeshModel>;
using MaterialPtr = RefPtr<Material3>;


/// メッシュの属性
struct MeshAttribute
{
	int		MaterialIndex;  ///< 対応するマテリアル番号
	int		StartIndex;     ///< 開始インデックス
	int		PrimitiveNum;   ///< 描画プリミティブ数 (三角形の数)
};
typedef Array<MeshAttribute>		MeshAttributeList;

/**
	@brief
*/
class Material3	// TODO: Scene の Material のベースクラスにしたい
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
	
protected:
	Material3();
	virtual ~Material3();

LN_INTERNAL_ACCESS:
	using ShaderValuePtr = std::shared_ptr<ShaderValue>;

	struct ValuePair
	{
		ShaderVariable*	variable;
		ShaderValuePtr	value;
	};

	const Array<ValuePair>& GetLinkedVariableList() { return m_linkedVariableList; }

public:	// TODO:

	void SetMaterialTexture(Texture* v) { FindShaderValue(_T("MaterialTexture"))->SetManagedTexture(v); }
	Texture* GetMaterialTexture() const { return GetTexture(_T("MaterialTexture"), nullptr); }

	void SetOpacity(float v) { SetFloatParameter(_T("Opacity"), v); }
	float GetOpacity() const { auto* v = FindShaderValueConst(_T("Opacity")); return (v) ? v->GetFloat() : 1.0f; }

	void SetColorScale(const Color& v) { SetColorParameter(_T("ColorScale"), v); }
	Color GetColorScale() const { auto* v = FindShaderValueConst(_T("ColorScale")); return (v) ? Color(v->GetVector()) : Color::White; }

	void SetBlendColor(const Color& v) { SetColorParameter(_T("BlendColor"), v); }
	Color GetBlendColor() const { auto* v = FindShaderValueConst(_T("BlendColor")); return (v) ? Color(v->GetVector()) : Color::Transparency; }

	void SetTone(const ToneF& v) { SetVectorParameter(_T("Tone"), v); }
	ToneF GetTone() const { auto* v = FindShaderValueConst(_T("Tone")); return (v) ? ToneF(v->GetVector()) : ToneF(); }

	Matrix GetUVTransform() const { auto* v = FindShaderValueConst(_T("UVTransform")); return (v) ? v->GetMatrix() : Matrix::Identity; }

	Color GetColor(const StringRef& name, const Color& defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? Color(v->GetVector()) : defaultValue; }
	float GetFloat(const StringRef& name, float defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? v->GetFloat() : defaultValue; }
	Texture* GetTexture(const StringRef& name, Texture* defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? v->GetManagedTexture() : defaultValue; }
	int GetInt(const StringRef& name, int defaultValue) const { auto* v = FindShaderValueConst(name); return (v) ? v->GetInt() : defaultValue; }

private:
	void LinkVariables();
	ShaderValue* FindShaderValue(const StringRef& name);
	ShaderValue* FindShaderValueConst(const StringRef& name) const;

	RefPtr<Shader>						m_shader;
	SortedArray<String, ShaderValuePtr>	m_valueList;
	Array<ValuePair>					m_linkedVariableList;

LN_INTERNAL_ACCESS:
	bool								m_modifiedForMaterialInstance;
};

/**
	@brief
*/
class MaterialList3
	: public tr::ReflectionObjectList<Material3*>	// SubMaterials (0 の場合もある)
{
LN_INTERNAL_ACCESS:
	MaterialList3();
	virtual ~MaterialList3();
};

/**
	@brief
*/
class StaticMeshModel
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	void SetMaterialCount(int count);


LN_INTERNAL_ACCESS:
	StaticMeshModel();
	virtual ~StaticMeshModel();
	void Initialize(GraphicsManager* manager);
	void CreateBox(const Vector3& size);

	int GetSubsetCount() const { return m_attributes.GetCount(); }
	void Draw(GraphicsContext* g);

	 
	GraphicsManager*			m_manager;
	RefPtr<VertexDeclaration>	m_vertexDeclaration;
	RefPtr<VertexBuffer>		m_vertexBuffer;
	RefPtr<IndexBuffer>			m_indexBuffer;

	RefPtr<MaterialList3>		m_materials;
	MeshAttributeList			m_attributes;
};

LN_NAMESPACE_END
