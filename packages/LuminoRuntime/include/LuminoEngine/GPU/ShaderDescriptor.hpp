#pragma once
#include <LuminoEngine/Common.hpp>
#include "Shader.hpp"

namespace ln {

class ShaderDescriptor : public Object {
public:
    void setUniformBuffer(int slotIndex, const detail::ConstantBufferView& value);
    void setTexture(int slotIndex, IGraphicsResource* value);
    void setSamplerState(int slotIndex, SamplerState* value);
    void setStorageData(int slotIndex, IGraphicsResource* value);

    void setUniformBufferData(int index, const void* data, size_t size);

    const detail::ConstantBufferView& uniformBuffer(int slotIndex) const { return m_uniformBufferViews[slotIndex]; }
    IGraphicsResource* texture(int slotIndex) const { return m_textures[slotIndex]; }
    SamplerState* samplerState(int slotIndex) const { return m_samplers[slotIndex]; }
    IGraphicsResource* storage(int slotIndex) const { return m_storages[slotIndex]; }

    const ShaderPass* shaderPass() const { return m_shaderPass; }

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

private:
    ShaderDescriptor();
    virtual ~ShaderDescriptor();
    void reset(ShaderPass* shaderPass);
    void submit(GraphicsCommandList* commandList);

    const ShaderDescriptorLayout* descriptorLayout() const { return m_shaderPass->shaderPassDescriptorLayout(); }

    ShaderPass* m_shaderPass;
    const detail::ShaderDescriptorPool* m_descriptorPool;
    std::array<detail::ConstantBufferView, detail::MaxDescriptorSlots> m_uniformBufferViews;
    std::array<IGraphicsResource*, detail::MaxDescriptorSlots> m_textures;
    std::array<SamplerState*, detail::MaxDescriptorSlots> m_samplers;
    std::array<IGraphicsResource*, detail::MaxDescriptorSlots> m_storages;

    friend class detail::ShaderDescriptorPool;
    friend class GraphicsCommandList;
};

namespace detail {

class ShaderDescriptorPool : public URefObject {
public:
    ShaderDescriptorPool();
    void dispose();
    void reset();
    ShaderDescriptor* allocate(ShaderPass* shaderPass);

private:
    std::vector<Ref<ShaderDescriptor>> m_instances;
    size_t m_instancesUsed;
};

class ShaderSecondaryDescriptor
    : public Object {
public:
    void reset(GraphicsCommandList* commandList);
    // ShaderTechniqueSemanticsManager が必要としている要素に限定して realloc する。SceneRenderer で使用する。
    void reallocFromSemanticsManager(GraphicsCommandList* commandList, const detail::ShaderTechniqueSemanticsManager* semanticsManager);

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
