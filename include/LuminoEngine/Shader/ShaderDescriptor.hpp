#pragma once
#include "../Graphics/Common.hpp"
#include "Shader.hpp"

namespace ln {

	
class ShaderDescriptor
	: public Object
{
public:
	// 各 index は ShaderDescriptorLayout のフィールドの index と一致する。
	// BindingIndex ではない点に注意。
	void setUniformBuffer(int index, const detail::UniformBufferView& value);
	void setTexture(int index, Texture* value);
	void setSamplerState(int index, SamplerState* value);

	const detail::UniformBufferView& uniformBuffer(int index) const { return m_uniforms[index]; }
	Texture* texture(int index) const { return m_textures[index]; }
	SamplerState* samplerState(int index) const { return m_samplers[index]; }

	void setUniformBufferData(int index, const void* data, size_t size);
	
LN_INTERNAL_NEW_OBJECT;
    ShaderDescriptor();
    bool init(Shader* shader);

private:
	static const int MaxElements = 8;

	Shader* m_shader;
	std::array<detail::UniformBufferView, MaxElements> m_uniforms = {};
	std::array<Texture*, MaxElements> m_textures = {};
	std::array<SamplerState*, MaxElements> m_samplers = {};
};

#if 0
class ShaderDescriptor
	: public RefObject	// 大量に作られるオブジェクトなので、今は外部公開は考えない。TODO: struct でもいいかも。
{
public:
	
LN_INTERNAL_NEW_OBJECT;
    ShaderDescriptor();
    bool init(detail::ShaderDescriptorPool* pool, void* data);

private:
	detail::ShaderDescriptorPool* m_pool;
	void* m_data;
};


namespace detail {

// ShaderDescriptor は Shader クラスに対応してデータを保持する。
// Vulkan など RHI では ShaderPass が対応する。
class ShaderDescriptorPool
{
public:
	ShaderDescriptorPool();
	bool init(Shader* shader);
	size_t descriptorDataSize() const { return m_descriptorDataSize; }
	void reset();
	ShaderDescriptor* allocate();

private:
	void grow();

	static const size_t InitialCount = 32;

	Shader* m_shader;
	size_t m_descriptorDataSize;
	std::vector<Ref<ByteBuffer>> m_pageDataList;
	std::vector<Ref<ShaderDescriptor>> m_descriptors;
	size_t m_used;
};

} // namespace detail
#endif
} // namespace ln
