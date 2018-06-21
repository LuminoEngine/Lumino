
#include "Internal.hpp"
#include <Lumino/Shader/Shader.hpp>
#include "../Graphics/GraphicsDeviceContext.hpp"

namespace ln {

//=============================================================================
// Shader

Ref<Shader> Shader::create(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath)
{
	return ln::newObject<Shader>(vertexShaderFilePath, pixelShaderFilePath, ShaderCodeType::RawGLSL);
}

Shader::Shader()
{

}

Shader::~Shader()
{

}

void Shader::initialize()
{
	GraphicsResource::initialize();
}

void Shader::initialize(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath, ShaderCodeType codeType)
{
	Shader::initialize();

	auto vsCode = FileSystem::readAllBytes(vertexShaderFilePath);
	auto psCode = FileSystem::readAllBytes(pixelShaderFilePath);

	ShaderCompilationDiag diag;
	auto rhiPass = deviceContext()->createShaderPass(vsCode.data(), vsCode.size(), psCode.data(), psCode.size(), &diag);

	auto tech = newObject<ShaderTechnique>();
	tech->setOwner(this);
	m_techniques.add(tech);

	auto pass = newObject<ShaderPass>(rhiPass);
	tech->addShaderPass(pass);
	pass->setupParameters();
}

ShaderParameter* Shader::getShaderParameter(const detail::ShaderUniformTypeDesc& desc, const String& name)
{
	for (auto& param : m_parameters)
	{
		if (detail::ShaderUniformTypeDesc::equals(param->desc(), desc) && param->name() == name)
		{
			return param;
		}
	}

	auto param = newObject<ShaderParameter>(desc, name);
	m_parameters.add(param);
	return param;
}

//=============================================================================
// ShaderParameter

ShaderParameter::ShaderParameter()
{
}

ShaderParameter::~ShaderParameter()
{
}

void ShaderParameter::initialize(const detail::ShaderUniformTypeDesc& desc, const String& name)
{
	m_desc = desc;
	m_name = name;
	m_value.reset(desc.type, desc.elements);
}

//=============================================================================
// ShaderTechnique

ShaderTechnique::ShaderTechnique()
{
}

ShaderTechnique::~ShaderTechnique()
{
}

void ShaderTechnique::initialize()
{
}

void ShaderTechnique::addShaderPass(ShaderPass* pass)
{
	m_passes.add(pass);
	pass->setOwner(this);
}

//=============================================================================
// ShaderPass

ShaderPass::ShaderPass()
	: m_rhiPass(nullptr)
{
}

ShaderPass::~ShaderPass()
{
}

void ShaderPass::initialize(detail::IShaderPass* rhiPass)
{
	if (LN_REQUIRE(rhiPass)) return;

	m_rhiPass = rhiPass;
}

void ShaderPass::setupParameters()
{
	m_parameters.clear();

	for (int i = 0; i < m_rhiPass->getUniformCount(); i++)
	{
		detail::IShaderUniform* uni = m_rhiPass->getUniform(i);
		ShaderParameter* param = m_owner->owner()->getShaderParameter(uni->desc(), String::fromStdString(uni->name()));
		m_parameters.add(param);
	}
}


//=============================================================================
// ShaderParameterValue
namespace detail {

ShaderParameterValue::ShaderParameterValue()
	: m_type(ShaderVariableType::Unknown)
	, m_buffer()
{
}

void ShaderParameterValue::reset(ShaderVariableType type, int elements)
{
	switch (type)
	{
	case ln::ShaderVariableType::Bool:
		setBool(false);
		break;
	case ln::ShaderVariableType::BoolArray:
		setBoolArray(nullptr, elements);
		break;
	case ln::ShaderVariableType::Int:
		setInt(0);
		break;
	case ln::ShaderVariableType::Float:
		setFloat(0);
		break;
	case ln::ShaderVariableType::FloatArray:
		setFloatArray(nullptr, elements);
		break;
	case ln::ShaderVariableType::Vector:
		setVector(Vector4());
		break;
	case ln::ShaderVariableType::VectorArray:
		setVectorArray(nullptr, elements);
		break;
	case ln::ShaderVariableType::Matrix:
		setMatrix(Matrix());
		break;
	case ln::ShaderVariableType::MatrixArray:
		setMatrixArray(nullptr, elements);
		break;
	case ln::ShaderVariableType::Texture:
		setTexture(nullptr);
		break;
	case ln::ShaderVariableType::Pointer:
		setPointer(nullptr);
		break;
	default:
		LN_UNREACHABLE();
		break;
	}
}

void ShaderParameterValue::setBool(bool value)
{
	m_type = ShaderVariableType::Bool;
	m_data.Bool = value;
	dirty();
}

void ShaderParameterValue::setInt(int value)
{
	m_type = ShaderVariableType::Int;
	m_data.Int = value;
	dirty();
}

void ShaderParameterValue::setBoolArray(const bool* values, int count)
{
	m_type = ShaderVariableType::BoolArray;
	allocValueBuffer(sizeof(bool) * count);
	if (values) {
		memcpy(m_buffer.data(), values, sizeof(bool) * count);
	}
	dirty();
}

void ShaderParameterValue::setFloat(float value)
{
	m_type = ShaderVariableType::Float;
	m_data.Float = value;
	dirty();
}

void ShaderParameterValue::setFloatArray(const float* values, int count)
{
	m_type = ShaderVariableType::FloatArray;
	allocValueBuffer(sizeof(float) * count);
	if (values) {
		memcpy(m_buffer.data(), values, sizeof(float) * count);
	}
	dirty();
}

void ShaderParameterValue::setVector(const Vector4& vec)
{
	m_type = ShaderVariableType::Vector;
	*reinterpret_cast<Vector4*>(m_data.Vector4) = vec;
	dirty();
}

void ShaderParameterValue::setVectorArray(const Vector4* vectors, int count)
{
	m_type = ShaderVariableType::VectorArray;
	allocValueBuffer(sizeof(Vector4) * count);
	if (vectors) {
		memcpy(m_buffer.data(), vectors, sizeof(Vector4) * count);
	}
	dirty();
}

void ShaderParameterValue::setMatrix(const Matrix& matrix)
{
	m_type = ShaderVariableType::Matrix;
	allocValueBuffer(sizeof(Matrix));
	*reinterpret_cast<Matrix*>(m_data.Matrix4x4) = matrix;
	dirty();
}

void ShaderParameterValue::setMatrixArray(const Matrix* matrices, int count)
{
	m_type = ShaderVariableType::MatrixArray;
	allocValueBuffer(sizeof(Matrix) * count);
	if (matrices) {
		memcpy(m_buffer.data(), matrices, sizeof(Matrix) * count);
		dirty();
	}
}

void ShaderParameterValue::setTexture(Texture* texture)
{
	if (m_type != ShaderVariableType::Texture)
	{
		m_type = ShaderVariableType::Texture;
		m_data.texture = texture;
		if (m_data.texture) {
			RefObjectHelper::retain(m_data.texture);
		}
	}
	else
	{
		if (m_data.texture) {
			RefObjectHelper::release(m_data.texture);
		}
		m_data.texture = texture;
		if (m_data.texture) {
			RefObjectHelper::retain(m_data.texture);
		}
	}

	dirty();
}

void ShaderParameterValue::setPointer(void* value)
{
	m_type = ShaderVariableType::Pointer;
	m_data.Pointer = value;
	dirty();
}

void ShaderParameterValue::dirty()
{
}

void ShaderParameterValue::allocValueBuffer(size_t size)
{
	if (size > m_buffer.size())
	{
		m_buffer.resize(size);
	}
}

bool ShaderParameterValue::isShortSizeType(ShaderVariableType type)
{
	return
		type == ShaderVariableType::Bool ||
		type == ShaderVariableType::Int ||
		type == ShaderVariableType::Float ||
		type == ShaderVariableType::Vector ||
		type == ShaderVariableType::Matrix;
}

} // namespace detail
} // namespace ln
