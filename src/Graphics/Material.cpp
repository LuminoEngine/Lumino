
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
	, m_shaderModified(false)
	, m_modifiedForMaterialInstance(false)
{
}

//------------------------------------------------------------------------------
Material::~Material()
{
}

//------------------------------------------------------------------------------
RefPtr<Material> Material::Copy() const
{
	auto m = RefPtr<Material>::MakeRef();
	m->m_shader = m_shader;
	m->m_valueList = m_valueList;
	m->m_linkedVariableList = m_linkedVariableList;

	m->m_blendMode = m_blendMode;
	m->m_culling = m_culling;
	m->m_fill = m_fill;
	m->m_alphaTest = m_alphaTest;
	m->m_depthTestEnabled = m_depthTestEnabled;
	m->m_depthWriteEnabled = m_depthWriteEnabled;

	// 遅延評価のための変更フラグはそのまま維持。コピー時は評価しない。
	// コピー先が本当に必要になったタイミングで評価されるようにする。
	m->m_shaderModified = m_shaderModified;

	m->m_modifiedForMaterialInstance = m_modifiedForMaterialInstance;

	return m;
}

//------------------------------------------------------------------------------
void Material::SetShader(Shader* shader)
{
	m_shader = shader;
	m_shaderModified = true;
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
	m_linkedVariableList.Clear();

	if (m_shader != nullptr)
	{
		for (ShaderVariable* v : m_shader->GetVariables())
		{
			if (v->GetType() == ShaderVariableType_Unknown ||
				v->GetType() == ShaderVariableType_String)
			{
				// Unknown と String 型は無視。String 型は読み取り専用で、Material としては持っておく必要ない。
			}
			else
			{
				// このマテリアルの値として、シェーダ変数値を保持する変数を作る
				ShaderValuePtr valuePtr;
				if (!m_valueList.TryGetValue(v->GetName(), &valuePtr))
				{
					valuePtr = std::make_shared<ShaderValue>(v->GetShaderValue());	// 初期値
					m_valueList.Add(v->GetName(), valuePtr);
				}

				// 変数と値のペア
				ValuePair pair = { v, valuePtr };
				m_linkedVariableList.Add(pair);
			}
		}
	}

	// TODO: シェーダが変更されたことで不要となるものを m_valueList から除外したほうがいいかも
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

//------------------------------------------------------------------------------
void Material::ApplyToShaderVariables()
{
	if (m_shaderModified)
	{
		LinkVariables();
		m_shaderModified = false;
	}

	for (auto& pair : m_linkedVariableList)
	{
		pair.variable->SetShaderValue(*pair.value);
	}
}

LN_NAMESPACE_END
