
#include "Internal.hpp"
#include <LuminoGraphics/RHI/ConstantBuffer.hpp>
#include "GraphicsManager.hpp"
#include "GraphicsProfiler.hpp"
#include "Backend/GraphicsDeviceContext.hpp"

namespace ln {

//==============================================================================
// ConstantBuffer
	
ConstantBuffer::ConstantBuffer()
	: m_manager(nullptr)
	, m_profiling(false)
	, m_rhiObject()
	, m_mappedData(nullptr)
{
	detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);
	detail::GraphicsResourceInternal::manager(this)->profiler()->addConstantBuffer(this);
}

ConstantBuffer::~ConstantBuffer()
{
	detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);
	detail::GraphicsResourceInternal::manager(this)->profiler()->removeConstantBuffer(this);
}

bool ConstantBuffer::init(size_t size)
{
	if (!Object::init()) return false;
	
	m_rhiObject = detail::GraphicsManager::instance()->deviceContext()->createUniformBuffer(size);
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

void ConstantBuffer::onManagerFinalizing()
{

}

void ConstantBuffer::onChangeDevice(detail::IGraphicsDevice* device)
{

}

} // namespace ln
