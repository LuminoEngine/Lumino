
#include "../../Internal.h"
#include "ShaderVariableBase.h"

LN_NAMESPACE_BEGIN
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
void ShaderVariableBase::makeInitialValue()
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
			setBoolArray(nullptr, m_desc.Elements);
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
			setFloatArray(nullptr, m_desc.Elements);
			break;
		}
		case ShaderVariableType_Vector:
		{
			setVector(Vector4::Zero);
			break;
		}
		case ShaderVariableType_VectorArray:
		{
			setVectorArray(nullptr, m_desc.Elements);
			break;
		}
		case ShaderVariableType_Matrix:
		{
			setMatrix(Matrix::Identity);
			break;
		}
		case ShaderVariableType_MatrixArray:
		{
			setMatrixArray(nullptr, m_desc.Elements);
			break;
		}
		case ShaderVariableType_DeviceTexture:
		{
			setTexture(nullptr);
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
void ShaderVariableBase::setBoolArray(const bool* values, int count)
{
	m_value.setBoolArray(values, count);
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
void ShaderVariableBase::setFloatArray(const float* values, int count)
{
	m_value.setFloatArray(values, count);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::setVector(const Vector4& vec)
{
	m_value.setVector(vec);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::setVectorArray(const Vector4* vectors, int count)
{
	m_value.setVectorArray(vectors, count);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::setMatrix(const Matrix& matrix)
{
	m_value.setMatrix(matrix);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::setMatrixArray(const Matrix* matrices, int count)
{
	m_value.setMatrixArray(matrices, count);
	m_modifid = true;
}

//------------------------------------------------------------------------------
void ShaderVariableBase::setTexture(ITexture* texture)
{
	m_value.setDeviceTexture(texture);
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
LN_NAMESPACE_END
