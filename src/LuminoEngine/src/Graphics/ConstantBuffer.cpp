
#include "Internal.hpp"
#include <LuminoEngine/Graphics/ConstantBuffer.hpp>
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"

namespace ln {

//==============================================================================
// ConstantBuffer
	
ConstantBuffer::ConstantBuffer()
	: m_rhiObject()
	, m_mappedData(nullptr)
{
}

bool ConstantBuffer::init(size_t size)
{
	if (!Object::init()) return false;
	
	m_rhiObject = detail::EngineDomain::graphicsManager()->deviceContext()->createUniformBuffer(size);
	if (!m_rhiObject) {
		return false;
	}

	return true;
}

void* ConstantBuffer::writableData()
{
	if (!m_mappedData) {
		m_mappedData = m_rhiObject->map();
	}
	return m_mappedData;
}

void ConstantBuffer::unmap()
{
	if (m_mappedData) {
		m_rhiObject->unmap();
		m_mappedData = nullptr;
	}
}

} // namespace ln
