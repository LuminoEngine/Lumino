
#include "../../Internal.h"
#include "ShaderVariableBase.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//==============================================================================
// ShaderVariableBase
//==============================================================================

//------------------------------------------------------------------------------
ShaderVariableBase::ShaderVariableBase()
	: m_modifid(false)
{
}

//------------------------------------------------------------------------------
ShaderVariableBase::~ShaderVariableBase()
{
}

//------------------------------------------------------------------------------
void ShaderVariableBase::initialize(ShaderVariableTypeDesc desc, const String& name, const String& semanticName)
{
	m_desc = desc;
	m_name = name;
	m_semanticName = semanticName;
	m_modifid = false;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::MakeInitialValue()
{
	switch (m_desc.Type)
	{
		case ShaderVariableType_Bool:
		{
			SetBool(false);
			break;
		}
		case ShaderVariableType_BoolArray:
		{
			SetBoolArray(nullptr, m_desc.Elements);
			break;
		}
		case ShaderVariableType_Int:
		{
			SetInt(0);
			break;
		}
		case ShaderVariableType_Float:
		{
			SetFloat(0.0f);
			break;
		}
		case ShaderVariableType_FloatArray:
		{
			SetFloatArray(nullptr, m_desc.Elements);
			break;
		}
		case ShaderVariableType_Vector:
		{
			SetVector(Vector4::Zero);
			break;
		}
		case ShaderVariableType_VectorArray:
		{
			SetVectorArray(nullptr, m_desc.Elements);
			break;
		}
		case ShaderVariableType_Matrix:
		{
			SetMatrix(Matrix::Identity);
			break;
		}
		case ShaderVariableType_MatrixArray:
		{
			SetMatrixArray(nullptr, m_desc.Elements);
			break;
		}
		case ShaderVariableType_DeviceTexture:
		{
			SetTexture(nullptr);
			break;
		}
		case ShaderVariableType_String:
		{
			SetString(String::GetEmpty());
			break;
		}
		default:
			LN_UNREACHABLE();
			break;
	}
}

//------------------------------------------------------------------------------
void ShaderVariableBase::SetBool(bool value)
{
	m_value.SetBool(value);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::SetBoolArray(const bool* values, int count)
{
	m_value.SetBoolArray(values, count);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::SetInt(int value)
{
	m_value.SetInt(value);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::SetFloat(float value)
{
	m_value.SetFloat(value);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::SetFloatArray(const float* values, int count)
{
	m_value.SetFloatArray(values, count);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::SetVector(const Vector4& vec)
{
	m_value.SetVector(vec);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::SetVectorArray(const Vector4* vectors, int count)
{
	m_value.SetVectorArray(vectors, count);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::SetMatrix(const Matrix& matrix)
{
	m_value.SetMatrix(matrix);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::SetMatrixArray(const Matrix* matrices, int count)
{
	m_value.SetMatrixArray(matrices, count);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::SetTexture(ITexture* texture)
{
	m_value.SetDeviceTexture(texture);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::SetString(const char* str)
{
	m_value.SetString(str);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::SetString(const String& str)
{
	m_value.SetString(str);
	m_modifid = true;
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
