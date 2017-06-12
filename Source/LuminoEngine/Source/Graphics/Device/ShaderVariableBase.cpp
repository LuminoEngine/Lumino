
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
			setBool(false);
			break;
		}
		case ShaderVariableType_BoolArray:
		{
			SetBoolArray(nullptr, m_desc.Elements);
			break;
		}
		case ShaderVariableType_Int:
		{
			setInt(0);
			break;
		}
		case ShaderVariableType_Float:
		{
			setFloat(0.0f);
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
			setString(String::getEmpty());
			break;
		}
		default:
			LN_UNREACHABLE();
			break;
	}
}

//------------------------------------------------------------------------------
void ShaderVariableBase::setBool(bool value)
{
	m_value.setBool(value);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::SetBoolArray(const bool* values, int count)
{
	m_value.SetBoolArray(values, count);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::setInt(int value)
{
	m_value.setInt(value);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::setFloat(float value)
{
	m_value.setFloat(value);
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
void ShaderVariableBase::setString(const char* str)
{
	m_value.setString(str);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::setString(const String& str)
{
	m_value.setString(str);
	m_modifid = true;
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
