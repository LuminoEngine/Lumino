
#include "Internal.hpp"
#include <LuminoEngine/Shader/ShaderDescriptor.hpp>

namespace ln {
	
//==============================================================================
// SceneRendererPass
	
ShaderDescriptor::ShaderDescriptor()
	: m_shader(nullptr)
{
}

void ShaderDescriptor::setUniformBuffer(int index, const detail::UniformBufferView& value)
{
	m_uniforms[index] = value;
}

void ShaderDescriptor::setTexture(int index, Texture* value)
{
	m_textures[index] = value;
}

void ShaderDescriptor::setSamplerState(int index, SamplerState* value)
{
	m_samplers[index] = value;
}

void ShaderDescriptor::setUniformBufferData(int index, const void* data, size_t size)
{
	m_uniforms[index].setData(data, size);
}

bool ShaderDescriptor::init(Shader* shader)
{
	if (!Object::init()) return false;
	m_shader = shader;
	return true;
}

#if 0
//==============================================================================
// SceneRendererPass
	
ShaderDescriptor::ShaderDescriptor()
	: m_pool(nullptr)
{
}

bool ShaderDescriptor::init(detail::ShaderDescriptorPool* pool, void* data)
{
	//if (!Object::init()) return false;
	m_pool = pool;
	m_data = data;
	return true;
}
	
//==============================================================================
// ShaderDescriptorPool

namespace detail {

ShaderDescriptorPool::ShaderDescriptorPool()
	: m_shader(nullptr)
	, m_descriptorDataSize(0)
	, m_pageDataList()
	, m_descriptors()
	, m_used(0)
{
}

bool ShaderDescriptorPool::init(Shader* shader)
{
	m_shader = shader;

	const auto& layout = m_shader->descriptorLayout();
	m_descriptorDataSize = 0;
	for (const auto& i : layout->m_buffers) {
		m_descriptorDataSize += i.size;
	}
	for (const auto& i : layout->m_textures) {
		m_descriptorDataSize += sizeof(Texture*);		// pointer size
	}
	for (const auto& i : layout->m_samplers) {
		m_descriptorDataSize += sizeof(SamplerState*);	// pointer size
	}

	grow();

	return true;
}

void ShaderDescriptorPool::reset()
{
	m_used = 0;
}

ShaderDescriptor* ShaderDescriptorPool::allocate()
{
	if (m_used >= m_descriptors.size()) {
		grow();
	}

	ShaderDescriptor* d = m_descriptors[m_used];
	m_used++;
	return d;
}

void ShaderDescriptorPool::grow()
{
	size_t oldSize = (m_descriptors.empty()) ? InitialCount : m_descriptors.size();
	size_t newSize = oldSize * 2;

	size_t pageSize = oldSize * m_descriptorDataSize;
	auto pageData = makeRef<ByteBuffer>(pageSize);
	m_pageDataList.push_back(pageData);
	uint8_t* dataHead = static_cast<uint8_t*>(pageData->data());

	m_descriptors.resize(newSize);
	for (size_t i = oldSize; i < newSize; i++) {
		auto descriptor = makeObject<ShaderDescriptor>(this, dataHead);
		m_descriptors[i] = descriptor;
		dataHead += m_descriptorDataSize;
	}
}

} // namespace detail
#endif
} // namespace ln
