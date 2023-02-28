#include "VulkanDeviceContext.hpp"
#include "VulkanBuffers.hpp"
#include "VulkanTextures.hpp"
#include "VulkanShaderPass.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// VulkanShaderPass

VulkanShaderPass::VulkanShaderPass()
    : m_vertShaderModule(VK_NULL_HANDLE)
    , m_fragShaderModule(VK_NULL_HANDLE)
    , m_compShaderModule(VK_NULL_HANDLE)
{
}

Result VulkanShaderPass::init(VulkanDevice* deviceContext, const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag)
{
    LN_DCHECK(deviceContext);
	if (!IShaderPass::init(createInfo)) {
		return err();
	}

    m_deviceContext = deviceContext;

    VkDevice device = m_deviceContext->vulkanDevice();

    // Create ShaderModule
    {
        // vert
        if (createInfo.vsCode) {
            m_vertEntryPointName = createInfo.vsEntryPointName;

            VkShaderModuleCreateInfo shaderCreateInfo = {};
            shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            shaderCreateInfo.codeSize = createInfo.vsCodeLen;
            shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(createInfo.vsCode);

            LN_VK_CHECK(vkCreateShaderModule(device, &shaderCreateInfo, m_deviceContext->vulkanAllocator(), &m_vertShaderModule));
        }

        // frag
        if (createInfo.psCode) {
            m_fragEntryPointName = createInfo.psEntryPointName;

            VkShaderModuleCreateInfo shaderCreateInfo = {};
            shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            shaderCreateInfo.codeSize = createInfo.psCodeLen;
            shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(createInfo.psCode);

            LN_VK_CHECK(vkCreateShaderModule(device, &shaderCreateInfo, m_deviceContext->vulkanAllocator(), &m_fragShaderModule));
        }

        // comp
        if (createInfo.csCode) {
            m_compEntryPointName = createInfo.csEntryPointName;

            VkShaderModuleCreateInfo shaderCreateInfo = {};
            shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            shaderCreateInfo.codeSize = createInfo.csCodeLen;
            shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(createInfo.csCode);

            LN_VK_CHECK(vkCreateShaderModule(device, &shaderCreateInfo, m_deviceContext->vulkanAllocator(), &m_compShaderModule));
        }
    }

    // DescriptorSetLayout
    {
        // https://docs.microsoft.com/ja-jp/windows/desktop/direct3dhlsl/dx-graphics-hlsl-variable-register

        // NOTE: なんで DescriptorSet を3つ作るの？
        // → https://qiita.com/lriki/items/934804030d56fd88dcc8#%E6%9C%AC%E9%A1%8C
        //   set=0 を UniformBuffer,
        //   set=1 を Texture,
        //   set=2 を Sampler として扱いたい。
        //   GLSL でいうところの layout(set=*) を変えるには、複数の DescriptorSet を作らなければならない。

        const auto stageFlag = [](const kokage::DescriptorLayout* d, kokage::DescriptorType t) {
            return
                ((d->isReferenceFromVertexStage(t)) ? VK_SHADER_STAGE_VERTEX_BIT : 0) |
                ((d->isReferenceFromPixelStage(t)) ? VK_SHADER_STAGE_FRAGMENT_BIT : 0) |
                ((d->isReferenceFromComputeStage(t)) ? VK_SHADER_STAGE_COMPUTE_BIT : 0);
        };

        // set=0, 'b' register in HLSL
        {
            std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
            layoutBindings.reserve(createInfo.descriptorLayout->bufferSlots().size());
            m_bufferDescriptorBufferInfo.reserve(createInfo.descriptorLayout->bufferSlots().size());
            for (auto& item : createInfo.descriptorLayout->bufferSlots()) {
                VkDescriptorSetLayoutBinding layoutBinding = {};
                layoutBinding.binding = item.binding;
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags = stageFlag(createInfo.descriptorLayout, kokage::DescriptorType_UniformBuffer);
                // NOTE: ↑この getShaderVisibility() は全ての CBV に対しての設定となるため、最適解ではない。
                // ただ個々の CBV まで対応となると非常に複雑になるためここまでにしておく。
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBindings.push_back(layoutBinding);

                DescriptorInfo2 info;
                info.bufferInfo.buffer = VK_NULL_HANDLE;   // set from submitDescriptorWriteInfo
                info.bufferInfo.offset = 0;
                info.bufferInfo.range = item.size;
                m_bufferDescriptorBufferInfo.push_back(info);
                
                VkWriteDescriptorSet set;
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.pNext = nullptr;
                set.dstSet = VK_NULL_HANDLE;   // set from submitDescriptorWriteInfo
                set.dstBinding = item.binding;
                set.dstArrayElement = 0;
                set.descriptorCount = 1;
                set.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                set.pImageInfo = nullptr;
                set.pBufferInfo = &m_bufferDescriptorBufferInfo.back().bufferInfo;
                set.pTexelBufferView = nullptr;
                m_descriptorWriteInfo.push_back(set);
            }

            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = layoutBindings.size();    // 0 で空のインスタンスだけ作ることは可能
            layoutInfo.pBindings = layoutBindings.data();
            LN_VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutInfo, m_deviceContext->vulkanAllocator(), &m_descriptorSetLayouts[kokage::DescriptorType_UniformBuffer]));
        }

        // set=1, 't' register in HLSL (Texture and CombinedSampler)
        {
            std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
            layoutBindings.reserve(createInfo.descriptorLayout->resourceSlots().size());
            m_textureDescripterImageInfo.reserve(createInfo.descriptorLayout->resourceSlots().size());
            for (auto& item : createInfo.descriptorLayout->resourceSlots()) {
                // FIXME: descriptorType を computeShader かどうかで固定しているが、 今は ComputeShader では Texture 型は許可しない (というか使えるのか未調査)
                VkDescriptorSetLayoutBinding layoutBinding = {};
                layoutBinding.binding = item.binding;
                layoutBinding.descriptorType = (m_compShaderModule) ? VK_DESCRIPTOR_TYPE_STORAGE_BUFFER : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags = stageFlag(createInfo.descriptorLayout, kokage::DescriptorType_Texture);
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBindings.push_back(layoutBinding);

                DescriptorInfo2 info;
                info.imageInfo.sampler = VK_NULL_HANDLE;
                info.imageInfo.imageView = VK_NULL_HANDLE;    // set from submitDescriptorWriteInfo
                info.imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                m_textureDescripterImageInfo.push_back(info);

                VkWriteDescriptorSet set;
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.pNext = nullptr;
                set.dstSet = VK_NULL_HANDLE;   // set from submitDescriptorWriteInfo
                set.dstBinding = item.binding;
                set.dstArrayElement = 0;
                set.descriptorCount = 1;
                set.descriptorType = (m_compShaderModule) ? VK_DESCRIPTOR_TYPE_STORAGE_BUFFER : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                set.pBufferInfo = (m_compShaderModule) ? &m_textureDescripterImageInfo.back().bufferInfo : nullptr;
                set.pImageInfo = (!m_compShaderModule) ? &m_textureDescripterImageInfo.back().imageInfo : nullptr;
                set.pTexelBufferView = nullptr;
                m_descriptorWriteInfo.push_back(set);
            }

            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = layoutBindings.size();    // 0 で空のインスタンスだけ作ることは可能
            layoutInfo.pBindings = layoutBindings.data();
            LN_VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutInfo, m_deviceContext->vulkanAllocator(), &m_descriptorSetLayouts[kokage::DescriptorType_Texture]));
        }

        // set=2, 's' register in HLSL (SamplerState)
        {
            std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
            layoutBindings.reserve(createInfo.descriptorLayout->samplerSlots().size());
            m_samplerDescripterImageInfo.reserve(createInfo.descriptorLayout->samplerSlots().size());
            for (auto& item : createInfo.descriptorLayout->samplerSlots()) {
                VkDescriptorSetLayoutBinding layoutBinding = {};
                layoutBinding.binding = item.binding;
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER; //VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//   // VK_DESCRIPTOR_TYPE_SAMPLER としても使える。ただし、ImageView をセットしておく必要がある。
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags = stageFlag(createInfo.descriptorLayout, kokage::DescriptorType_SamplerState);
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBindings.push_back(layoutBinding);

                DescriptorInfo2 info;
                info.imageInfo.sampler = VK_NULL_HANDLE;      // set from submitDescriptorWriteInfo
                info.imageInfo.imageView = VK_NULL_HANDLE;    // set from submitDescriptorWriteInfo
                info.imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                m_samplerDescripterImageInfo.push_back(info);

                VkWriteDescriptorSet set;
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.pNext = nullptr;
                set.dstSet = VK_NULL_HANDLE;   // set from submitDescriptorWriteInfo
                set.dstBinding = item.binding;
                set.dstArrayElement = 0;
                set.descriptorCount = 1;
                set.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;//VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                set.pImageInfo = &m_samplerDescripterImageInfo.back().imageInfo;
                set.pBufferInfo = nullptr;
                set.pTexelBufferView = nullptr;
                m_descriptorWriteInfo.push_back(set);
            }

            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = layoutBindings.size();    // 0 で空のインスタンスだけ作ることは可能
            layoutInfo.pBindings = layoutBindings.data();
            LN_VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutInfo, m_deviceContext->vulkanAllocator(), &m_descriptorSetLayouts[kokage::DescriptorType_SamplerState]));
        }

        // set=3, 'u' register in HLSL (UnorderdAccess)
        {
            std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
            layoutBindings.reserve(createInfo.descriptorLayout->unorderdSlots().size());
            m_storageDescriptorBufferInfo.reserve(createInfo.descriptorLayout->unorderdSlots().size());
            for (auto& item : createInfo.descriptorLayout->unorderdSlots()) {
                VkDescriptorSetLayoutBinding layoutBinding = {};
                layoutBinding.binding = item.binding;
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags = stageFlag(createInfo.descriptorLayout, kokage::DescriptorType_UnorderdAccess);
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBindings.push_back(layoutBinding);

                DescriptorInfo2 info;
                info.bufferInfo.buffer = VK_NULL_HANDLE;   // set from submitDescriptorWriteInfo
                info.bufferInfo.offset = 0;
                info.bufferInfo.range = item.size;
                m_storageDescriptorBufferInfo.push_back(info);

                VkWriteDescriptorSet set;
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.pNext = nullptr;
                set.dstSet = VK_NULL_HANDLE;   // set from submitDescriptorWriteInfo
                set.dstBinding = item.binding;
                set.dstArrayElement = 0;
                set.descriptorCount = 1;
                set.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                set.pImageInfo = nullptr;
                set.pBufferInfo = &m_storageDescriptorBufferInfo.back().bufferInfo;
                set.pTexelBufferView = nullptr;
                m_descriptorWriteInfo.push_back(set);
            }

            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = layoutBindings.size();    // 0 で空のインスタンスだけ作ることは可能
            layoutInfo.pBindings = layoutBindings.data();
            LN_VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutInfo, m_deviceContext->vulkanAllocator(), &m_descriptorSetLayouts[kokage::DescriptorType_UnorderdAccess]));
        }
    }

    // PipelineLayout
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = m_descriptorSetLayouts.size();
        pipelineLayoutInfo.pSetLayouts = m_descriptorSetLayouts.data();
        LN_VK_CHECK(vkCreatePipelineLayout(device, &pipelineLayoutInfo, m_deviceContext->vulkanAllocator(), &m_pipelineLayout));
    }

    //m_descriptorTable = makeRef<VulkanShaderDescriptorTable>();
    //if (!m_descriptorTable->init(m_deviceContext, this, createInfo.descriptorLayout)) {
    //    return false;
    //}

    return ok();
}

void VulkanShaderPass::dispose()
{
    if (m_deviceContext) {
        VkDevice device = m_deviceContext->vulkanDevice();

        //if (m_descriptorTable) {
        //    m_descriptorTable->dispose();
        //    m_descriptorTable = nullptr;
        //}

        if (m_pipelineLayout) {
            vkDestroyPipelineLayout(device, m_pipelineLayout, m_deviceContext->vulkanAllocator());
            m_pipelineLayout = VK_NULL_HANDLE;
        }

        for (auto& layout : m_descriptorSetLayouts) {
            if (layout) {
                vkDestroyDescriptorSetLayout(device, layout, m_deviceContext->vulkanAllocator());
                layout = VK_NULL_HANDLE;
            }
        }

        if (m_vertShaderModule) {
            vkDestroyShaderModule(device, m_vertShaderModule, m_deviceContext->vulkanAllocator());
            m_vertShaderModule = VK_NULL_HANDLE;
        }

        if (m_fragShaderModule) {
            vkDestroyShaderModule(device, m_fragShaderModule, m_deviceContext->vulkanAllocator());
            m_fragShaderModule = VK_NULL_HANDLE;
        }

        //m_deviceContext->pipelineCache()->invalidateFromShaderPass(this);
        m_deviceContext = nullptr;
    }

    IShaderPass::dispose();
}

const std::vector<VkWriteDescriptorSet>& VulkanShaderPass::submitDescriptorWriteInfo(
    VulkanCommandBuffer* commandBuffer,
    const std::array<VkDescriptorSet, kokage::DescriptorType_Count>& descriptorSets,
    const ShaderDescriptorTableUpdateInfo& data)
{
    uint32_t writeIndex = 0;
    //m_descriptorTable->setData(&data);

    //const auto& uniforms = m_descriptorTable->uniforms();
    for (int i = 0; i < m_bufferDescriptorBufferInfo.size(); i++) {
        const auto& uniformBuffer = data.uniforms[i];

#if 1
        //VulkanSingleFrameBufferInfo bufferInfo = commandBuffer->uniformBufferSingleFrameAllocator()->allocate(uniformBuffer.data.size());
        //bufferInfo.buffer->setData(bufferInfo.offset, uniformBuffer.data.data(), uniformBuffer.data.size());
        VulkanSingleFrameBufferInfo bufferInfo;
        bufferInfo.buffer = static_cast<VulkanUniformBuffer*>(uniformBuffer.object)->buffer();
        bufferInfo.offset = uniformBuffer.offset;

        DescriptorInfo2& info = m_bufferDescriptorBufferInfo[i];
        info.bufferInfo.buffer = bufferInfo.buffer->nativeBuffer();
        info.bufferInfo.offset = bufferInfo.offset;
#else
        // UniformBuffer の内容を CopyCommand に乗せる。
        // Inside RenderPass では vkCmdCopyBuffer が禁止されているので、DeviceLocal に置いたメモリを使うのではなく、
        // 毎回新しい HostVisible な Buffer を作ってそれを使う。
        // 
        // ちなみに VertexBuffer などへのデータ転送の時は VertexBuffer へ CopyBuffer しているが、
        // ここでは 1 コマンドバッファ内でのみ有効な VulkanBuffer を作って、それを直接 Descripter にセットしている。
        // なぜこうしているのかというと、
        // - VertexBuffer の動的な書き換えでは、特に Sprite のバッチ描画などで 巨大な Buffer の一部を書き換えることが多いため、毎回動的に確保するとメモリ消費がひどいことになる。
        // - 対して UniformBuffer は、数 100 byte 程度の小さいバッファを毎フレーム大量に確保することになる。
        //   リアルタイムグラフィックスでは、ずっと const な UBO はほとんど存在しない。小さな動的 Buffer の一部を頻繁に書き換えるよりも丸ごと転送してしまった方が簡単だし速い。
        //   また Vulkan の仕様として Descripter の Write は CommandBuffer に乗るものではないので、基本的にドローコールの数だけ Descripter が必要となる。
        VulkanBuffer* buffer = commandBuffer->allocateBuffer(uniformBuffer.data.size(), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        buffer->setData(0, uniformBuffer.data.data(), uniformBuffer.data.size());

        VkDescriptorBufferInfo& info = m_bufferDescriptorBufferInfo[i];
        info.buffer = buffer->nativeBuffer();
#endif

        VkWriteDescriptorSet& writeInfo = m_descriptorWriteInfo[i];
        writeInfo.dstSet = descriptorSets[kokage::DescriptorType_UniformBuffer];

        writeIndex++;
    }

    //const auto& textures = m_descriptorTable->textures();
    for (int i = 0; i < m_textureDescripterImageInfo.size(); i++) {
        DescriptorInfo2& info = m_textureDescripterImageInfo[i];
        VkWriteDescriptorSet& writeInfo = m_descriptorWriteInfo[writeIndex];
        if (writeInfo.pImageInfo) {
            VulkanTexture* texture = static_cast<VulkanTexture*>(data.resources[i].object);
            VulkanSamplerState* samplerState = static_cast<VulkanSamplerState*>(data.resources[i].stamplerState);
            info.imageInfo.imageView = (texture) ? texture->image()->vulkanImageView() : 0;
            info.imageInfo.sampler = (samplerState) ? samplerState->vulkanSampler() : 0;
            info.imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            writeInfo.pImageInfo = &info.imageInfo;
            writeInfo.dstSet = descriptorSets[kokage::DescriptorType_Texture];
        }
        else {
            if (data.resources[i].object) {
                VulkanVertexBuffer* buffer = dynamic_cast<VulkanVertexBuffer*>(data.resources[i].object);
                assert(buffer);
                info.bufferInfo.buffer = buffer->vulkanBuffer();
                info.bufferInfo.range = buffer->buffer()->size();
            }
            else {
                // Vulkan は null 禁止
                LN_UNREACHABLE();
            }
            info.bufferInfo.offset = 0;
            writeInfo.pBufferInfo = &info.bufferInfo;
            writeInfo.dstSet = descriptorSets[kokage::DescriptorType_Texture];
        }
        writeIndex++;
    }

    //const auto& samplers = m_descriptorTable->samplers();
    for (int i = 0; i < m_samplerDescripterImageInfo.size(); i++) {
        DescriptorInfo2& info = m_samplerDescripterImageInfo[i];
        VkWriteDescriptorSet& writeInfo = m_descriptorWriteInfo[writeIndex];
        VulkanSamplerState* samplerState = static_cast<VulkanSamplerState*>(data.samplers[i].stamplerState);
        info.imageInfo.sampler = (samplerState) ? samplerState->vulkanSampler() : 0;
        //writeInfo.pImageInfo = &info.imageInfo;
        writeInfo.dstSet = descriptorSets[kokage::DescriptorType_SamplerState];
        writeIndex++;

        //const auto& info = m_textureDescripterImageInfo[i];
        //VkWriteDescriptorSet& writeInfo = m_descriptorWriteInfo[info.descriptorWriteInfoIndex];
        //writeInfo.pImageInfo = &info.imageInfo;
        //
    }

    for (int i = 0; i < m_storageDescriptorBufferInfo.size(); i++) {
        DescriptorInfo2& info = m_storageDescriptorBufferInfo[i];
        VkWriteDescriptorSet& writeInfo = m_descriptorWriteInfo[writeIndex];

        VulkanVertexBuffer* buffer = static_cast<VulkanVertexBuffer*>(data.storages[i].object);
        info.bufferInfo.buffer = buffer->vulkanBuffer();
        info.bufferInfo.offset = 0;
        info.bufferInfo.range = buffer->buffer()->size();
        writeInfo.pBufferInfo = &info.bufferInfo;
        writeInfo.dstSet = descriptorSets[kokage::DescriptorType_UnorderdAccess];
        
        writeIndex++;
    }

    return m_descriptorWriteInfo;
}

//==============================================================================
// VulkanShaderDescriptorTable

//VulkanShaderDescriptorTable::VulkanShaderDescriptorTable()
//{
//}
//
//bool VulkanShaderDescriptorTable::init(VulkanDevice* deviceContext, const VulkanShaderPass* ownerPass, const DescriptorLayout* descriptorLayout)
//{
//    uint32_t writeIndex = 0;
//
//    // UniformBuffers
//    for (const auto& item : descriptorLayout->uniformBufferRegister) {
//        UniformBufferInfo info;
//        info.descriptorWriteInfoIndex = writeIndex;
//        info.bindingIndex = item.binding;
//
//        // Verify
//        const VkWriteDescriptorSet& writeInfo = ownerPass->witeInfo(info.descriptorWriteInfoIndex);
//        if (LN_ENSURE(writeInfo.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)) return false;
//        if (LN_ENSURE(writeInfo.dstBinding == info.bindingIndex)) return false;
//
//        m_uniforms.push_back(info);
//        writeIndex++;
//    }
//
//    // Textures
//    for (const auto& item : descriptorLayout->textureRegister) {
//        ImageBufferInfo info;
//        info.descriptorWriteInfoIndex = writeIndex;
//        info.bindingIndex = item.binding;
//        info.imageInfo.sampler = VK_NULL_HANDLE;
//        info.imageInfo.imageView = VK_NULL_HANDLE;
//        info.imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//
//        // Verify
//        const VkWriteDescriptorSet& writeInfo = ownerPass->witeInfo(info.descriptorWriteInfoIndex);
//        if (LN_ENSURE(writeInfo.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)) return false;
//        if (LN_ENSURE(writeInfo.dstBinding == info.bindingIndex)) return false;
//
//        m_textures.push_back(info);
//        writeIndex++;
//    }
//
//    // Samplers
//    for (const auto& item : descriptorLayout->samplerRegister) {
//        ImageBufferInfo info;
//        info.descriptorWriteInfoIndex = writeIndex;
//        info.bindingIndex = item.binding;
//        info.imageInfo.sampler = VK_NULL_HANDLE;
//        info.imageInfo.imageView = VK_NULL_HANDLE;
//        info.imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//
//        // Verify
//        const VkWriteDescriptorSet& writeInfo = ownerPass->witeInfo(info.descriptorWriteInfoIndex);
//        if (LN_ENSURE(writeInfo.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER)) return false;
//        if (LN_ENSURE(writeInfo.dstBinding == info.bindingIndex)) return false;
//
//        m_samplers.push_back(info);
//        writeIndex++;
//    }
//
//    return true;
//}
//
//void VulkanShaderDescriptorTable::dispose()
//{
//    m_uniforms.clear();
//    m_textures.clear();
//    m_samplers.clear();
//}
//
//// ここではメンバに保持するだけ。次の SubmitState で、実際に Vulkan の DescripterSet を作り、CommandList に乗せて Transfar する。
//void VulkanShaderDescriptorTable::setData(const ShaderDescriptorTableUpdateInfo* data)
//{
//    if (LN_REQUIRE(data)) return;
//    //if (LN_REQUIRE(data->uniforms.size() == m_uniforms.size())) return;
//    //if (LN_REQUIRE(data->textures.size() == m_textures.size())) return;
//    //if (LN_REQUIRE(data->samplers.size() == m_samplers.size())) return;
//
//    for (int i = 0; i < m_uniforms.size(); i++) {
//        m_uniforms[i].bufferView = data->uniforms[i];
//    }
//
//    for (int i = 0; i < m_textures.size(); i++) {
//        auto& info = m_textures[i];
//        info.texture = static_cast<VulkanTexture*>(data->textures[i].texture);
//        info.samplerState = static_cast<VulkanSamplerState*>(data->textures[i].stamplerState);
//        info.imageInfo.imageView = (info.texture) ? info.texture->image()->vulkanImageView() : 0;
//        info.imageInfo.sampler = (info.samplerState) ? info.samplerState->vulkanSampler() : 0;
//    }
//
//    for (int i = 0; i < m_samplers.size(); i++) {
//        auto& info = m_samplers[i];
//        info.texture = static_cast<VulkanTexture*>(data->samplers[i].texture);
//        info.samplerState = static_cast<VulkanSamplerState*>(data->samplers[i].stamplerState);
//        info.imageInfo.imageView = (info.texture) ? info.texture->image()->vulkanImageView() : 0;
//        info.imageInfo.sampler = (info.samplerState) ? info.samplerState->vulkanSampler() : 0;
//    }
//}


} // namespace detail
} // namespace ln

