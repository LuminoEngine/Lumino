#pragma once
#include <LuminoGraphics/Common.hpp>
#include "Shader.hpp"

namespace ln {
namespace detail {

	
class ShaderSecondaryDescriptor
	: public Object
{
public:
	void reset(detail::GraphicsCommandList* commandList);
	// ShaderTechniqueSemanticsManager が必要としている要素に限定して realloc する。SceneRenderer で使用する。
	void reallocFromSemanticsManager(detail::GraphicsCommandList* commandList, const detail::ShaderTechniqueSemanticsManager* semanticsManager);


	// 各 index は ShaderDescriptorLayout のフィールドの index と一致する。
	// BindingIndex ではない点に注意。
	void setUniformBuffer(int index, const detail::ConstantBufferView& value);
	void setTexture(int index, IGraphicsResource* value);
	void setSamplerState(int index, SamplerState* value);
	void setStorageData(int index, IGraphicsResource* value);

	const detail::ConstantBufferView& uniformBuffer(int index) const { return m_uniformBufferViews[index]; }
	IGraphicsResource* texture(int index) const { return m_textures[index]; }
	SamplerState* samplerState(int index) const { return m_samplers[index]; }
	IGraphicsResource* storage(int index) const { return m_storages[index]; }

	void setUniformBufferData(int index, const void* data, size_t size);

	Shader* shader() const { return m_shader; }
	const ShaderDescriptorLayout* descriptorLayout() const { return m_shader->descriptorLayout(); }


	/** 整数値を設定します。 */
	void setInt(int memberIndex, int value);

	/** 整数値の配列を設定します。 */
	void setIntArray(int memberIndex, const int* value, int count);

	/** 浮動小数点値を設定します。 */
	void setFloat(int memberIndex, float value);

	/** 浮動小数点値の配列を設定します。 */
	void setFloatArray(int memberIndex, const float* value, int count);

	/** ベクトルを設定します。 */
	void setVector(int memberIndex, const Vector4& value);

	/** ベクトルの配列を設定します。 */
	void setVectorArray(int memberIndex, const Vector4* value, int count);

	/** 行列を設定します。 */
	void setMatrix(int memberIndex, const Matrix& value);

	/** 行列の配列を設定します。 */
	void setMatrixArray(int memberIndex, const Matrix* value, int count);

	///** テクスチャを設定します。 */
	//void setTexture(int textureIndex, Texture* value);

	///** テクスチャを設定します。 */
	//void setSampler(int textureIndex, Texture* value);

	///** SamplerState を設定します。 */
	//void setSamplerState(int samplerIndex, SamplerState* value);

LN_INTERNAL_NEW_OBJECT;
    ShaderSecondaryDescriptor();
    bool init(Shader* shader);

private:
	static const int MaxElements = 16;

	Shader* m_shader;
	std::array<detail::ConstantBufferView, MaxElements> m_uniformBufferViews = {};
	std::array<IGraphicsResource*, MaxElements> m_textures = {};
	std::array<SamplerState*, MaxElements> m_samplers = {};
	std::array<IGraphicsResource*, MaxElements> m_storages = {};
};
} // namespace detail

#if 0
class ShaderSecondaryDescriptor
	: public RefObject	// 大量に作られるオブジェクトなので、今は外部公開は考えない。TODO: struct でもいいかも。
{
public:
	
LN_INTERNAL_NEW_OBJECT;
    ShaderSecondaryDescriptor();
    bool init(detail::ShaderDescriptorPool* pool, void* data);

private:
	detail::ShaderDescriptorPool* m_pool;
	void* m_data;
};


namespace detail {

// ShaderSecondaryDescriptor は Shader クラスに対応してデータを保持する。
// Vulkan など RHI では ShaderPass が対応する。
class ShaderDescriptorPool
{
public:
	ShaderDescriptorPool();
	bool init(Shader* shader);
	size_t descriptorDataSize() const { return m_descriptorDataSize; }
	void reset();
	ShaderSecondaryDescriptor* allocate();

private:
	void grow();

	static const size_t InitialCount = 32;

	Shader* m_shader;
	size_t m_descriptorDataSize;
	std::vector<Ref<ByteBuffer>> m_pageDataList;
	std::vector<Ref<ShaderSecondaryDescriptor>> m_descriptors;
	size_t m_used;
};

} // namespace detail
#endif
} // namespace ln
