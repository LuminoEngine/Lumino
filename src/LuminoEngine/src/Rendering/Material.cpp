
#include "Internal.hpp"
#include <Lumino/Rendering/Material.hpp>

namespace ln {

//==============================================================================
// AbstractMaterial

AbstractMaterial::AbstractMaterial(detail::MaterialType type)
	: m_type(type)
{
}

AbstractMaterial::~AbstractMaterial()
{
}

void AbstractMaterial::initialize()
{
	Object::initialize();
}

void AbstractMaterial::setShader(Shader* shader)
{
	LN_NOTIMPLEMENTED();
}

Shader* AbstractMaterial::shader() const
{
	return m_shader;
}

void AbstractMaterial::setInt(const StringRef& name, int value)
{
	LN_NOTIMPLEMENTED();
}

void AbstractMaterial::setFloat(const StringRef& name, float value)
{
	LN_NOTIMPLEMENTED();
}

void AbstractMaterial::setVector(const StringRef& name, const Vector4& value)
{
	LN_NOTIMPLEMENTED();
}

void AbstractMaterial::setMatrix(const StringRef& name, const Matrix& value)
{
	LN_NOTIMPLEMENTED();
}

void AbstractMaterial::setTexture(const StringRef& name, Texture* value)
{
	LN_NOTIMPLEMENTED();
}

void AbstractMaterial::setColor(const StringRef& name, const Color& value)
{
	LN_NOTIMPLEMENTED();
}

} // namespace ln

