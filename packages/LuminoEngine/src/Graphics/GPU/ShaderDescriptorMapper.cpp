#include "Internal.hpp"
#include <LuminoEngine/Graphics/GPU/ShaderDescriptorMapper.hpp>

namespace ln {

RHIBindingToDescriptorMapper::RHIBindingToDescriptorMapper() {
}

RHIBindingToDescriptorMapper::~RHIBindingToDescriptorMapper() {
}

void RHIBindingToDescriptorMapper::setup(kokage::TargetShaderPass* shaderPass) {
    std::vector<kokage::TargetBindingInfo>& bindingInfos = shaderPass->bindingLayout.bindings;

    for (kokage::TargetBindingInfo& binding : bindingInfos) {




        //MapperEntry entry;
        //switch (binding.category) {
        //    case kokage::BindingResourceCategory_ConstantBuffer:
        //        entry.category = kokage::RegisterCategory_ConstantBuffer;
        //        break;
        //    case kokage::BindingResourceCategory_Texture:
        //        entry.category = kokage::RegisterCategory_TextureOrCombinedSampler;
        //        break;
        //    case kokage::BindingResourceCategory_SamplerState:
        //        entry.category = kokage::RegisterCategory_SamplerState;
        //        break;
        //    case kokage::BindingResourceCategory_UnorderdAccess:
        //        entry.category = kokage::RegisterCategory_UnorderdAccess;
        //        break;
        //    default:
        //        LN_UNREACHABLE();
        //        break;
        //}
        //entry.index = registerCounts[binding.category];
        //m_entries.push_back(entry);
        //registerCounts[binding.category]++;
    }
}

const RHIBindingToDescriptorMapper::MapperEntry& RHIBindingToDescriptorMapper::getEntry(
    int index) const {
    return m_entries[index];
}

} // namespace ln
