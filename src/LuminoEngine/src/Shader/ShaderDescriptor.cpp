
#include "Internal.hpp"
#include <LuminoEngine/Graphics/ConstantBuffer.hpp>
#include <LuminoEngine/Shader/ShaderDescriptor.hpp>

namespace ln {
	
//==============================================================================
// SceneRendererPass
	
ShaderDescriptor::ShaderDescriptor()
	: m_shader(nullptr)
{
}

bool ShaderDescriptor::init(Shader* shader)
{
	if (!Object::init()) return false;
	m_shader = shader;
	return true;
}

void ShaderDescriptor::setUniformBuffer(int index, const detail::ConstantBufferView& value)
{
	m_uniformBufferViews[index] = value;
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
	m_uniformBufferViews[index].setData(data, size);
}

void ShaderDescriptor::setInt(int memberIndex, int value)
{
	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignScalarsToBuffer((const byte_t*)&value, sizeof(int), 1, static_cast<byte_t*>(buffer.buffer->writableData()), member.desc.offset, 1, 0);
}

void ShaderDescriptor::setIntArray(int memberIndex, const int* value, int count)
{
	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignScalarsToBuffer((const byte_t*)value, sizeof(int), count, static_cast<byte_t*>(buffer.buffer->writableData()), member.desc.offset, member.desc.elements, member.desc.arrayStride);
}

void ShaderDescriptor::setFloat(int memberIndex, float value)
{
	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignScalarsToBuffer((const byte_t*)&value, sizeof(float), 1, static_cast<byte_t*>(buffer.buffer->writableData()), member.desc.offset, 1, 0);
}

void ShaderDescriptor::setFloatArray(int memberIndex, const float* value, int count)
{
	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignScalarsToBuffer((const byte_t*)value, sizeof(float), count, static_cast<byte_t*>(buffer.buffer->writableData()), member.desc.offset, member.desc.elements, member.desc.arrayStride);
}

void ShaderDescriptor::setVector(int memberIndex, const Vector4& value)
{
	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignVectorsToBuffer((const byte_t*)&value, 4, 1, static_cast<byte_t*>(buffer.buffer->writableData()), member.desc.offset, 1, 0, member.desc.columns);
}

void ShaderDescriptor::setVectorArray(int memberIndex, const Vector4* value, int count)
{
	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignVectorsToBuffer((const byte_t*)value, 4, count, static_cast<byte_t*>(buffer.buffer->writableData()), member.desc.offset, member.desc.elements, member.desc.arrayStride, member.desc.columns);
}

void ShaderDescriptor::setMatrix(int memberIndex, const Matrix& value)
{
#ifdef LN_SHADER_UBO_TRANSPORSE_MATRIX
	const bool transpose = true;
#else
	const bool transpose = false;
#endif

	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignMatricesToBuffer((const byte_t*)&value, 4, 4, 1, static_cast<byte_t*>(buffer.buffer->writableData()), member.desc.offset, 1, member.desc.matrixStride, 0, member.desc.rows, member.desc.columns, transpose);
}

void ShaderDescriptor::setMatrixArray(int memberIndex, const Matrix* value, int count)
{
#ifdef LN_SHADER_UBO_TRANSPORSE_MATRIX
	const bool transpose = true;
#else
	const bool transpose = false;
#endif
	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignMatricesToBuffer((const byte_t*)value, 4, 4, count, static_cast<byte_t*>(buffer.buffer->writableData()), member.desc.offset, member.desc.elements, member.desc.matrixStride, member.desc.arrayStride, member.desc.rows, member.desc.columns, transpose);
}

//void ShaderDescriptor::setTexture(int textureIndex, Texture* value)
//{
//	m_textures[textureIndex] = value;
//}
//
//void ShaderDescriptor::setSampler(int textureIndex, Texture* value)
//{
//	m_textures[textureIndex] = value;
//}
//
//void ShaderDescriptor::setSamplerState(int samplerIndex, SamplerState* value)
//{
//	m_samplers[samplerIndex] = value;
//}

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
