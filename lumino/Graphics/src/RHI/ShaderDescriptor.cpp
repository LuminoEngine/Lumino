
#include "Internal.hpp"
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoGraphics/RHI/SamplerState.hpp>
#include <LuminoGraphics/RHI/ConstantBuffer.hpp>
#include <LuminoGraphics/RHI/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/RHI/ShaderDescriptor.hpp>

namespace ln {
namespace detail {

//==============================================================================
// SceneRendererPass
	
ShaderSecondaryDescriptor::ShaderSecondaryDescriptor()
	: m_shader(nullptr)
{
}

bool ShaderSecondaryDescriptor::init(Shader* shader)
{
	if (!Object::init()) return false;
	m_shader = shader;
	return true;
}

void ShaderSecondaryDescriptor::reset(GraphicsCommandList* commandList)
{
	const auto& defaulValues = m_shader->descriptor();
	const auto& layout = descriptorLayout();

	for (int i = 0; i < layout->m_buffers.size(); i++) {
		const auto& info = layout->m_buffers[i];
		m_uniformBufferViews[i] = commandList->allocateUniformBuffer(info.size);
	}

	for (int i = 0; i < layout->m_textures.size(); i++) {
		m_textures[i] = defaulValues->m_textures[i];
	}
	for (int i = 0; i < layout->m_samplers.size(); i++) {
		m_samplers[i] = defaulValues->m_samplers[i];
	}
}

void ShaderSecondaryDescriptor::reallocFromSemanticsManager(GraphicsCommandList* commandList, const detail::ShaderTechniqueSemanticsManager* semanticsManager)
{
	const auto& layout = descriptorLayout();
	const auto& defaulValues = m_shader->descriptor();

	for (int i = 0; i < layout->m_buffers.size(); i++) {
		const auto& info = layout->m_buffers[i];
		m_uniformBufferViews[i].clear();
	}


	// TODO: ひとまず全部確保。LNRenderViewBuffer や LNClusteredShadingParameters は Element 単位ではなく共有可能なので、もう少し最適化したい
	int index = semanticsManager->getBuiltinShaderUniformBufferIndex(BuiltinShaderUniformBuffers_LNRenderViewBuffer);
	if (index >= 0) setUniformBuffer(index, commandList->allocateUniformBuffer(layout->m_buffers[index].size));
	index = semanticsManager->getBuiltinShaderUniformBufferIndex(BuiltinShaderUniformBuffers_LNRenderElementBuffer);
	if (index >= 0) setUniformBuffer(index, commandList->allocateUniformBuffer(layout->m_buffers[index].size));
	index = semanticsManager->getBuiltinShaderUniformBufferIndex(BuiltinShaderUniformBuffers_LNEffectColorBuffer);
	if (index >= 0) setUniformBuffer(index, commandList->allocateUniformBuffer(layout->m_buffers[index].size));
	index = semanticsManager->getBuiltinShaderUniformBufferIndex(BuiltinShaderUniformBuffers_LNPBRMaterialParameter);
	if (index >= 0) setUniformBuffer(index, commandList->allocateUniformBuffer(layout->m_buffers[index].size));
	index = semanticsManager->getBuiltinShaderUniformBufferIndex(BuiltinShaderUniformBuffers_LNClusteredShadingParameters);
	if (index >= 0) setUniformBuffer(index, commandList->allocateUniformBuffer(layout->m_buffers[index].size));
	index = semanticsManager->getBuiltinShaderUniformBufferIndex(BuiltinShaderUniformBuffers_LNShadowParameters);
	if (index >= 0) setUniformBuffer(index, commandList->allocateUniformBuffer(layout->m_buffers[index].size));
	index = semanticsManager->getBuiltinShaderUniformBufferIndex(BuiltinShaderUniformBuffers_Global);
	if (index >= 0) {
		// "$Global" はデフォルト値からコピーしておく
		setUniformBuffer(index, commandList->allocateUniformBuffer(layout->m_buffers[index].size));
		const auto& data = shader()->descriptor()->m_buffers[index];
		setUniformBufferData(0, data.data(), data.size());
	}


	for (int i = 0; i < layout->m_buffers.size(); i++) {
		if (!m_uniformBufferViews[i].buffer) {
			const auto& info = layout->m_buffers[i];
			m_uniformBufferViews[i] = commandList->allocateUniformBuffer(info.size);
		}
	}


	for (int i = 0; i < layout->m_textures.size(); i++) {
		m_textures[i] = defaulValues->m_textures[i];
	}
	for (int i = 0; i < layout->m_samplers.size(); i++) {
		m_samplers[i] = defaulValues->m_samplers[i];
	}
}

void ShaderSecondaryDescriptor::setUniformBuffer(int index, const detail::ConstantBufferView& value)
{
	m_uniformBufferViews[index] = value;
}

void ShaderSecondaryDescriptor::setTexture(int index, IGraphicsResource* value)
{
	m_textures[index] = value;
}

void ShaderSecondaryDescriptor::setSamplerState(int index, SamplerState* value)
{
	m_samplers[index] = value;
}

void ShaderSecondaryDescriptor::setStorageData(int index, IGraphicsResource* value)
{
	m_storages[index] = value;
}

void ShaderSecondaryDescriptor::setUniformBufferData(int index, const void* data, size_t size)
{
	m_uniformBufferViews[index].setData(data, size);
}

void ShaderSecondaryDescriptor::setInt(int memberIndex, int value)
{
	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignScalarsToBuffer((const byte_t*)&value, sizeof(int), 1, static_cast<byte_t*>(buffer.writableData()), member.desc.offset, 1, 0);
}

void ShaderSecondaryDescriptor::setIntArray(int memberIndex, const int* value, int count)
{
	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignScalarsToBuffer((const byte_t*)value, sizeof(int), count, static_cast<byte_t*>(buffer.writableData()), member.desc.offset, member.desc.elements, member.desc.arrayStride);
}

void ShaderSecondaryDescriptor::setFloat(int memberIndex, float value)
{
	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignScalarsToBuffer((const byte_t*)&value, sizeof(float), 1, static_cast<byte_t*>(buffer.writableData()), member.desc.offset, 1, 0);
}

void ShaderSecondaryDescriptor::setFloatArray(int memberIndex, const float* value, int count)
{
	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignScalarsToBuffer((const byte_t*)value, sizeof(float), count, static_cast<byte_t*>(buffer.writableData()), member.desc.offset, member.desc.elements, member.desc.arrayStride);
}

void ShaderSecondaryDescriptor::setVector(int memberIndex, const Vector4& value)
{
	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignVectorsToBuffer((const byte_t*)&value, 4, 1, static_cast<byte_t*>(buffer.writableData()), member.desc.offset, 1, 0, member.desc.columns);
}

void ShaderSecondaryDescriptor::setVectorArray(int memberIndex, const Vector4* value, int count)
{
	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignVectorsToBuffer((const byte_t*)value, 4, count, static_cast<byte_t*>(buffer.writableData()), member.desc.offset, member.desc.elements, member.desc.arrayStride, member.desc.columns);
}

void ShaderSecondaryDescriptor::setMatrix(int memberIndex, const Matrix& value)
{
#ifdef LN_SHADER_UBO_TRANSPORSE_MATRIX
	const bool transpose = true;
#else
	const bool transpose = false;
#endif

	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignMatricesToBuffer((const byte_t*)&value, 4, 4, 1, static_cast<byte_t*>(buffer.writableData()), member.desc.offset, 1, member.desc.matrixStride, 0, member.desc.rows, member.desc.columns, transpose);
}

void ShaderSecondaryDescriptor::setMatrixArray(int memberIndex, const Matrix* value, int count)
{
#ifdef LN_SHADER_UBO_TRANSPORSE_MATRIX
	const bool transpose = true;
#else
	const bool transpose = false;
#endif
	const auto& member = descriptorLayout()->m_members[memberIndex];
	auto& buffer = m_uniformBufferViews[member.uniformBufferRegisterIndex];
	detail::ShaderHelper::alignMatricesToBuffer((const byte_t*)value, 4, 4, count, static_cast<byte_t*>(buffer.writableData()), member.desc.offset, member.desc.elements, member.desc.matrixStride, member.desc.arrayStride, member.desc.rows, member.desc.columns, transpose);
}

//void ShaderSecondaryDescriptor::setTexture(int textureIndex, Texture* value)
//{
//	m_textures[textureIndex] = value;
//}
//
//void ShaderSecondaryDescriptor::setSampler(int textureIndex, Texture* value)
//{
//	m_textures[textureIndex] = value;
//}
//
//void ShaderSecondaryDescriptor::setSamplerState(int samplerIndex, SamplerState* value)
//{
//	m_samplers[samplerIndex] = value;
//}


} // namespace detail

#if 0
//==============================================================================
// SceneRendererPass
	
ShaderSecondaryDescriptor::ShaderSecondaryDescriptor()
	: m_pool(nullptr)
{
}

bool ShaderSecondaryDescriptor::init(detail::ShaderDescriptorPool* pool, void* data)
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

ShaderSecondaryDescriptor* ShaderDescriptorPool::allocate()
{
	if (m_used >= m_descriptors.size()) {
		grow();
	}

	ShaderSecondaryDescriptor* d = m_descriptors[m_used];
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
		auto descriptor = makeObject<ShaderSecondaryDescriptor>(this, dataHead);
		m_descriptors[i] = descriptor;
		dataHead += m_descriptorDataSize;
	}
}

} // namespace detail
#endif
} // namespace ln
