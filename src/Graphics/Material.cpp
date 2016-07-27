
#include "Internal.h"
#include <Lumino/Graphics/Material.h>

LN_NAMESPACE_BEGIN


//==============================================================================
// MaterialList
//==============================================================================

//------------------------------------------------------------------------------
MaterialList::MaterialList()
{
}

//------------------------------------------------------------------------------
MaterialList::~MaterialList()
{
}

//==============================================================================
// Material
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Material, Object);

const String Material::DiffuseParameter(_T("Diffuse"));
const String Material::AmbientParameter(_T("Ambient"));
const String Material::SpecularParameter(_T("Specular"));
const String Material::EmissiveParameter(_T("Emissive"));
const String Material::PowerParameter(_T("Power"));
const String Material::MaterialTextureParameter(_T("MaterialTexture"));

//------------------------------------------------------------------------------
MaterialPtr Material::Create()
{
	auto ptr = MaterialPtr::MakeRef();
	return ptr;
}

//------------------------------------------------------------------------------
Material::Material()
	: m_shader(nullptr)
	, m_valueList()
	, m_blendMode(BlendMode::Normal)
	, m_culling(CullingMode::Back)
	, m_fill(FillMode_Solid)
	, m_alphaTest(true)
	, m_depthTestEnabled(true)
	, m_depthWriteEnabled(true)
	, m_modifiedForMaterialInstance(false)
{
}

//------------------------------------------------------------------------------
Material::~Material()
{
}

//------------------------------------------------------------------------------
void Material::SetShader(Shader* shader)
{
	m_shader = shader;
	LinkVariables();
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material::SetIntParameter(const StringRef& name, int value)
{
	FindShaderValue(name)->SetInt(value);
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material::SetFloatParameter(const StringRef& name, float value)
{
	FindShaderValue(name)->SetFloat(value);
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material::SetVectorParameter(const StringRef& name, const Vector4& value)
{
	FindShaderValue(name)->SetVector(value);
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material::SetMatrixParameter(const StringRef& name, const Matrix& value)
{
	FindShaderValue(name)->SetMatrix(value);
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material::SetTextureParameter(const StringRef& name, Texture* value)
{
	FindShaderValue(name)->SetManagedTexture(value);
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material::SetColorParameter(const StringRef& name, const Color& value)
{
	SetVectorParameter(name, value);
}

//------------------------------------------------------------------------------
void Material::SetColorParameter(const StringRef& name, float r, float g, float b, float a)
{
	SetColorParameter(name, Color(r, g, b, a));
}

//------------------------------------------------------------------------------
void Material::SetBlendMode(BlendMode mode)
{
}

//------------------------------------------------------------------------------
void Material::SetCullingMode(CullingMode mode)
{
}

//------------------------------------------------------------------------------
void Material::SetFillMode(FillMode mode)
{
}

//------------------------------------------------------------------------------
void Material::SetAlphaTestEnabled(bool enabled)
{
}

//------------------------------------------------------------------------------
void Material::LinkVariables()
{
#if 0
	m_valueList.Clear();
	m_linkedVariableList.Clear();

	if (m_shader != nullptr)
	{
		for (ShaderVariable* v : m_shader->GetVariables())
		{
			if (v->GetType() != ShaderVariableType_Unknown &&
				v->GetType() != ShaderVariableType_String)
			{
				// Unknown と String 型は無視。String 型は読み取り専用で、Material としては持っておく必要ない。
			}
			else
			{
				// 名前と値の対応表
				auto var = std::make_shared<ShaderValue>(v->GetShaderValue());	// 初期値
				m_valueList.Add(v->GetName(), var);

				// 変数と値のペア
				ValuePair pair = { v, var };
				m_linkedVariableList.Add(pair);
			}
		}
	}
#endif
}

//------------------------------------------------------------------------------
ShaderValue* Material::FindShaderValue(const StringRef& name)
{
	ShaderValuePtr v;
	if (!m_valueList.TryGetValue(name, &v))
	{
		v = std::make_shared<ShaderValue>();
		m_valueList.Add(name, v);
	}
	return v.get();
}

//------------------------------------------------------------------------------
ShaderValue* Material::FindShaderValueConst(const StringRef& name) const
{
	ShaderValuePtr v;
	if (!m_valueList.TryGetValue(name, &v))
	{
		return nullptr;
	}
	return v.get();
}

LN_NAMESPACE_END
