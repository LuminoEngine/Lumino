
#include "Internal.hpp"
#include "DX12DeviceContext.hpp"
#include "DX12ShaderPass.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// DX12ShaderPass

DX12ShaderPass::DX12ShaderPass()
{
}

static D3D12_SHADER_VISIBILITY getShaderVisibility(const DescriptorLayoutItem& item)
{
    if (item.stageFlags == ShaderStageFlags_Vertex)
        return D3D12_SHADER_VISIBILITY_VERTEX;
    else if (item.stageFlags == ShaderStageFlags_Pixel)
        return D3D12_SHADER_VISIBILITY_PIXEL;
    else
        return D3D12_SHADER_VISIBILITY_ALL;
}

static D3D12_SHADER_VISIBILITY getShaderVisibility(const DescriptorLayout* layout, DescriptorType type)
{
    bool vs = layout->isReferenceFromVertexStage(type);
    bool ps = layout->isReferenceFromPixelStage(type);
    if (vs && ps)
        return D3D12_SHADER_VISIBILITY_ALL;
    else if (vs)
        return D3D12_SHADER_VISIBILITY_VERTEX;
    else
        return D3D12_SHADER_VISIBILITY_PIXEL;
}


Result DX12ShaderPass::init(DX12Device* deviceContext, const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag)
{
    LN_DCHECK(deviceContext);
	if (!IShaderPass::init(createInfo)) {
		return false;
	}

    m_deviceContext = deviceContext;

    m_layoutInfo.vs_CBV_RootParamIndex = -1;
    m_layoutInfo.vs_SRV_RootParamIndex = -1;
    m_layoutInfo.vs_Sampler_RootParamIndex = -1;
    m_layoutInfo.ps_CBV_RootParamIndex = -1;
    m_layoutInfo.ps_SRV_RootParamIndex = -1;
    m_layoutInfo.ps_Sampler_RootParamIndex = -1;

    //m_layoutInfo.srvRootParamIndex = -1;
    //m_layoutInfo.srvCount = createInfo.descriptorLayout->textureRegister.size();
    //m_layoutInfo.samperRootParamIndex = -1;
    //m_layoutInfo.samplerCount = createInfo.descriptorLayout->samplerRegister.size();
    //m_layoutInfo.cvbSizes.resize(m_layoutInfo.cbvCount);
    //for (int i = 0; i < m_layoutInfo.cbvCount; i++) {
    //    m_layoutInfo.cvbSizes[i] = createInfo.descriptorLayout->uniformBufferRegister[i].size;
    //}

    // TODO: IBlob にする必要ないかも
    {
        if (createInfo.vsCodeLen > 0) {
            if (FAILED(D3DCompilerAPI::D3DCreateBlob(createInfo.vsCodeLen, &m_vsCode))) {
                LN_ERROR("D3DCreateBlob failed.");
                return false;
            }
            memcpy(m_vsCode->GetBufferPointer(), createInfo.vsCode, createInfo.vsCodeLen);
        }

        if (createInfo.psCodeLen > 0) {
            if (FAILED(D3DCompilerAPI::D3DCreateBlob(createInfo.psCodeLen, &m_psCode))) {
                LN_ERROR("D3DCreateBlob failed.");
                return false;
            }
            memcpy(m_psCode->GetBufferPointer(), createInfo.psCode, createInfo.psCodeLen);
        }
    }

    ID3D12Device* dxDevice = m_deviceContext->device();

    {
        // ShaderVisibility
        // ----------
        //
        // DirectX12 では ShaderVisibility を、実際にシェーダコードが要求しているものと完全に一致させなければならない。
        // 例えば次のようなコードを考えてみる。
        // 
        // ```
        // cbuffer Camera {
        //     ...
        // }
        // cbuffer Material {
        //     ...
        // }
        // void VSMain() {
        //    // Camera を使って座標変換
        // }
        // void PSMain() {
        //    // Material を使ってシェーディング
        // }
        // ```
        // 
        // Vulkan では最適化を考えず雑に "VS と PS 両方で使用可能な Descriptor を 2 つ作る" でも問題なかった。
        //
        // DirectX12 では、HLSL で register を省略した場合、
        // - VS としては、Camera が b0 (Material は未使用)
        // - PS としては、Material が b0 (Camera は未使用)
        // というような割り当てが行われる。(RenderDoc で確認できる)
        //
        // ここでもし D3D12_DESCRIPTOR_RANGE_TYPE_CBV, BaseShaderRegister=0, D3D12_SHADER_VISIBILITY_ALL な Range を作ってしまうと、
        // PS の b0 が Camera を参照してしまう。この矛盾はデバッグレイヤーで通知されない。
        // 


        /*
        Layout(BindingIndex)
        ----------

        register 指定の無い HLSL コードを入力した場合、コンパイラによって自動割り当てが行われるが、
        glslangValidator と fxc では異なる BindingIndex を出力する。

        例えば次のようなコードをコンパイルした場合、
        
        ```
        sampler2D _ColorSampler;
        sampler2D _NormalAndDepthSampler;
        sampler2D _ViewDepthSampler;
        sampler2D _MetalRoughSampler;
        ```

        spirv-dis の出力は次のようになる。

        ```
        OpDecorate %_MetalRoughSampler Location 3
        OpDecorate %_MetalRoughSampler DescriptorSet 1
        OpDecorate %_MetalRoughSampler Binding 0
        OpDecorate %_NormalAndDepthSampler Location 1
        OpDecorate %_NormalAndDepthSampler DescriptorSet 1
        OpDecorate %_NormalAndDepthSampler Binding 1
        OpDecorate %_ViewDepthSampler Location 2
        OpDecorate %_ViewDepthSampler DescriptorSet 1
        OpDecorate %_ViewDepthSampler Binding 2
        OpDecorate %_ColorSampler Location 0
        OpDecorate %_ColorSampler DescriptorSet 1
        OpDecorate %_ColorSampler Binding 3
        ```

        fxc -dumpbin の出力は次のようになる。

        ```
        // Name                                 Type  Format         Dim      HLSL Bind  Count
        // ------------------------------ ---------- ------- ----------- -------------- ------
        // _ColorSampler                     sampler      NA          NA             s0      1
        // _NormalAndDepthSampler            sampler      NA          NA             s1      1
        // _ViewDepthSampler                 sampler      NA          NA             s2      1
        // _MetalRoughSampler                sampler      NA          NA             s3      1
        // _ViewDepthSampler                 texture  float4          2d             t0      1
        // _MetalRoughSampler                texture  float4          2d             t1      1
        // _NormalAndDepthSampler            texture  float4          2d             t2      1
        // _ColorSampler                     texture  float4          2d             t3      1
        ```

        CombindSampler を使用した場合、 D3DCompiler の方は Texture-Sampler のペア間でも Bind が一致しないことがある。

        このため RootSignature を作るための Refrection に、glslang を利用することはできない。
        HLSL bitcode を D3DCompiler で作ったのであれば、Refrection も D3DCompiler を使わなければならない。
        */

        struct Descriptor
        {
            DescriptorType type;
            std::string name;
            int32_t bindingIndex;
        };
        struct Reflection
        {
            std::vector<Descriptor> descriptors;
        };
        static const int ShaderStageCount = ShaderStage2_Count;//2;
        Reflection reflections[ShaderStageCount];

        // まず D3DReflect を使って、D3DCompiler が必要としている Descriptor を取得する。
        // この時点で、glslang では最適化で削除された Descriptor が、D3DCompiler では存在していることもある。 
        {
            std::pair<const void*, size_t> shaderCodes[ShaderStageCount] = {
                { createInfo.vsCode, createInfo.vsCodeLen },
                { createInfo.psCode, createInfo.psCodeLen },
                { createInfo.csCode, createInfo.csCodeLen } };
            for (int iStage = 0; iStage < ShaderStageCount; iStage++) {
                if (shaderCodes[iStage].first) {
                    ComPtr<ID3D12ShaderReflection> shaderReflection;
                    if (FAILED(D3DCompilerAPI::D3DReflect(shaderCodes[iStage].first, shaderCodes[iStage].second, IID_PPV_ARGS(&shaderReflection)))) {
                        LN_ERROR("D3DReflect failed.");
                        return false;
                    }

                    D3D12_SHADER_DESC desc;
                    shaderReflection->GetDesc(&desc);

                    for (UINT i = 0; i < desc.ConstantBuffers; ++i) {
                        ID3D12ShaderReflectionConstantBuffer* cbuffer = shaderReflection->GetConstantBufferByIndex(i);
                        D3D12_SHADER_BUFFER_DESC desc;
                        cbuffer->GetDesc(&desc);
                        LN_LOG_VERBOSE << "ConstantBuffer '" << desc.Name << "':";

                        for (UINT iVariable = 0; iVariable < desc.Variables; iVariable++) {
                            D3D12_SHADER_VARIABLE_DESC varDesc;
                            D3D12_SHADER_TYPE_DESC typeDesc;
                            ID3D12ShaderReflectionVariable* varRefl = cbuffer->GetVariableByIndex(iVariable);
                            ID3D12ShaderReflectionType* varTypeRefl = varRefl->GetType();
                            varRefl->GetDesc(&varDesc);
                            varTypeRefl->GetDesc(&typeDesc);
                            if (varDesc.uFlags & D3D_SVF_USED) {
                                LN_LOG_VERBOSE << "  '" << varDesc.Name << "' used.";
                            }
                            else {
                                LN_LOG_VERBOSE << "  '" << varDesc.Name << "' unused.";
                            }
                        }
                    }

                    for (UINT i = 0; i < desc.BoundResources; ++i) {
                        D3D12_SHADER_INPUT_BIND_DESC desc2;
                        shaderReflection->GetResourceBindingDesc(i, &desc2);

                        DescriptorType type = DescriptorType_Count;
                        if (desc2.Type == D3D_SIT_CBUFFER)
                            type = DescriptorType_UniformBuffer;
                        else if (desc2.Type == D3D_SIT_TEXTURE)
                            type = DescriptorType_Texture;
                        else if (desc2.Type == D3D_SIT_SAMPLER)
                            type = DescriptorType_SamplerState;
                        else if (desc2.Type == D3D_SIT_STRUCTURED) // StructuredBuffer<> xxx : register(t#);
                            type = DescriptorType_Texture;
                        else if (desc2.Type == D3D_SIT_UAV_RWSTRUCTURED) // RWStructuredBuffer<> xxx : register(u#);
                            type = DescriptorType_UnorderdAccess;

                        if (type != DescriptorType_Count) {
                            std::string name = desc2.Name;
                            if (name == "$Globals") name = "$Global";
                            reflections[iStage].descriptors.push_back({ type, std::move(name), (int32_t)desc2.BindPoint });
                        }
                    }
                }
            }
        }

        // D3DCompiler では static キーワードが無いと $Global に変数が含まれてしまう。
        // そのまま進むと PipelineState 作成時に次のような検証エラーとなる。
        // > Root Signature doesn't match Pixel Shader: Shader CBV descriptor range (BaseShaderRegister=0, NumDescriptors=1, RegisterSpace=0) is not fully bound in root signature
        // glslang だと定数として扱われるため、問題の原因が変わりづらい。
        // ひとまずの対策として、glslang が検出したものと不一致があればエラーとして通知する。
        {
            const auto& requiredDescriptors = createInfo.descriptorLayout->uniformBufferRegister;
            for (const auto& reflection : reflections) {
                for (const auto& descriptor : reflection.descriptors) {
                    if (descriptor.type == DescriptorType_UniformBuffer) {
                        if (std::find_if(requiredDescriptors.begin(), requiredDescriptors.end(), [&](const DescriptorLayoutItem& item) { return item.name == descriptor.name; }) == requiredDescriptors.end()) {
                            LN_ERROR("Variables that appear to be constants are visible in '%s'. Use the 'static' keyword.", descriptor.name.c_str());
                            return false;
                        }
                    }
                }
            }
        }

        auto getBindingIndex = [&](ShaderStage2 stage, DescriptorType type, const std::string& name) -> int32_t {
            const auto& list = reflections[stage].descriptors;
            const auto itr = std::find_if(list.begin(), list.end(), [&](const Descriptor& d) {
                if (d.type != type) return false;
                return d.name == name;
            });
            if (itr != list.end()) {
                return itr->bindingIndex;
            }
            else {
                // 定義されてはいるが未使用の場合は D3DReflect では拾えない
                return -1;
            }
        };

        DX12ShaderPassLayoutInfo::Descriptors& vsDescriptors = m_layoutInfo.vsDescriptors;
        DX12ShaderPassLayoutInfo::Descriptors& psDescriptors = m_layoutInfo.psDescriptors;
        DX12ShaderPassLayoutInfo::Descriptors& csDescriptors = m_layoutInfo.csDescriptors;

        // VS, PS、CS ごとに集計
        {
            // 'b' register
            for (int32_t i = 0; i < createInfo.descriptorLayout->uniformBufferRegister.size(); i++) {
                const DescriptorLayoutItem& item = createInfo.descriptorLayout->uniformBufferRegister[i];
                if (item.stageFlags & ShaderStageFlags_Vertex) {
                    int32_t bindingIndex = getBindingIndex(ShaderStage2_Vertex, DescriptorType_UniformBuffer, item.name);
                    if (bindingIndex >= 0) vsDescriptors.bufferDescriptors.push_back({ i, bindingIndex, (int32_t)item.size });
                }
                if (item.stageFlags & ShaderStageFlags_Pixel) {
                    int32_t bindingIndex = getBindingIndex(ShaderStage2_Fragment, DescriptorType_UniformBuffer, item.name);
                    if (bindingIndex >= 0) psDescriptors.bufferDescriptors.push_back({ i, bindingIndex, (int32_t)item.size });
                }
                if (item.stageFlags & ShaderStageFlags_Compute) {
                    int32_t bindingIndex = getBindingIndex(ShaderStage2_Compute, DescriptorType_UniformBuffer, item.name);
                    if (bindingIndex >= 0) csDescriptors.bufferDescriptors.push_back({ i, bindingIndex, (int32_t)item.size });
                }
            }
            // 'u' register
            for (int32_t i = 0; i < createInfo.descriptorLayout->unorderdRegister.size(); i++) {
                const DescriptorLayoutItem& item = createInfo.descriptorLayout->unorderdRegister[i];
                if (item.stageFlags & ShaderStageFlags_Compute) {
                    int32_t bindingIndex = getBindingIndex(ShaderStage2_Compute, DescriptorType_UnorderdAccess, item.name);
                    if (bindingIndex >= 0) csDescriptors.unorderedDescriptors.push_back({ i,bindingIndex, 0 });
                }
            }
            // 't' register
            for (int32_t i = 0; i < createInfo.descriptorLayout->textureRegister.size(); i++) {
                const DescriptorLayoutItem& item = createInfo.descriptorLayout->textureRegister[i];
                if (item.stageFlags & ShaderStageFlags_Vertex) {
                    int32_t bindingIndex = getBindingIndex(ShaderStage2_Vertex, DescriptorType_Texture, item.name);
                    if (bindingIndex >= 0) vsDescriptors.textureDescriptors.push_back({ i,bindingIndex, 0 });
                }
                if (item.stageFlags & ShaderStageFlags_Pixel) {
                    int32_t bindingIndex = getBindingIndex(ShaderStage2_Fragment, DescriptorType_Texture, item.name);
                    if (bindingIndex >= 0) psDescriptors.textureDescriptors.push_back({ i, bindingIndex, 0 });
                }
                if (item.stageFlags & ShaderStageFlags_Compute) {
                    int32_t bindingIndex = getBindingIndex(ShaderStage2_Compute, DescriptorType_Texture, item.name);
                    if (bindingIndex >= 0) csDescriptors.textureDescriptors.push_back({ i, bindingIndex, 0 });
                }
            }
            // 's' register
            for (int32_t i = 0; i < createInfo.descriptorLayout->samplerRegister.size(); i++) {
                const DescriptorLayoutItem& item = createInfo.descriptorLayout->samplerRegister[i];
                if (item.stageFlags & ShaderStageFlags_Vertex) {
                    int32_t bindingIndex = getBindingIndex(ShaderStage2_Vertex, DescriptorType_SamplerState, item.name);
                    if (bindingIndex >= 0) vsDescriptors.samplerDescriptors.push_back({ i, bindingIndex, 0 });
                }
                if (item.stageFlags & ShaderStageFlags_Pixel) {
                    int32_t bindingIndex = getBindingIndex(ShaderStage2_Fragment, DescriptorType_SamplerState, item.name);
                    if (bindingIndex >= 0) psDescriptors.samplerDescriptors.push_back({ i, bindingIndex, 0 });
                }
            }
        }

        std::array<D3D12_DESCRIPTOR_RANGE, MaxDescriptors * 8> ranges = {};
        std::array<D3D12_ROOT_PARAMETER, 8> params = {};
        //int i = 0;
        int rangeCount = 0;
        int paramCount = 0;
        int rangeBegin;
        {
            // [VertexShader Stage] 'b0' ~ 'bn'
            if (vsDescriptors.bufferDescriptors.size() > 0) {
                rangeBegin = rangeCount;
                for (int i = 0; i < vsDescriptors.bufferDescriptors.size(); i++) {
                    ranges[rangeCount].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                    ranges[rangeCount].NumDescriptors = 1;
                    ranges[rangeCount].BaseShaderRegister = vsDescriptors.bufferDescriptors[i].registerIndex;
                    ranges[rangeCount].RegisterSpace = 0;
                    ranges[rangeCount].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    rangeCount++;
                }
                m_layoutInfo.vs_CBV_RootParamIndex = paramCount;
                params[paramCount].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[paramCount].DescriptorTable.NumDescriptorRanges = rangeCount - rangeBegin;
                params[paramCount].DescriptorTable.pDescriptorRanges = &ranges[rangeBegin];
                params[paramCount].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
                paramCount++;
            }
            // [VertexShader Stage] 't0' ~ 'tn'
            if (vsDescriptors.textureDescriptors.size() > 0) {
                rangeBegin = rangeCount;
                for (int i = 0; i < vsDescriptors.textureDescriptors.size(); i++) {
                    ranges[rangeCount].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                    ranges[rangeCount].NumDescriptors = 1;
                    ranges[rangeCount].BaseShaderRegister = vsDescriptors.textureDescriptors[i].registerIndex;
                    ranges[rangeCount].RegisterSpace = 0;
                    ranges[rangeCount].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    rangeCount++;
                }
                m_layoutInfo.vs_SRV_RootParamIndex = paramCount;
                params[paramCount].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[paramCount].DescriptorTable.NumDescriptorRanges = rangeCount - rangeBegin;
                params[paramCount].DescriptorTable.pDescriptorRanges = &ranges[rangeBegin];
                params[paramCount].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
                paramCount++;
            }
            // [VertexShader Stage] 's0' ~ 'sn'
            if (vsDescriptors.samplerDescriptors.size() > 0) {
                rangeBegin = rangeCount;
                for (int i = 0; i < vsDescriptors.samplerDescriptors.size(); i++) {
                    ranges[rangeCount].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
                    ranges[rangeCount].NumDescriptors = 1;
                    ranges[rangeCount].BaseShaderRegister = vsDescriptors.samplerDescriptors[i].registerIndex;
                    ranges[rangeCount].RegisterSpace = 0;
                    ranges[rangeCount].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    rangeCount++;
                }
                m_layoutInfo.vs_Sampler_RootParamIndex = paramCount;
                params[paramCount].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[paramCount].DescriptorTable.NumDescriptorRanges = rangeCount - rangeBegin;
                params[paramCount].DescriptorTable.pDescriptorRanges = &ranges[rangeBegin];
                params[paramCount].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
                paramCount++;
            }
            // [PixelShader Stage] 'b0' ~ 'bn'
            if (psDescriptors.bufferDescriptors.size() > 0) {
                rangeBegin = rangeCount;
                for (int i = 0; i < psDescriptors.bufferDescriptors.size(); i++) {
                    ranges[rangeCount].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                    ranges[rangeCount].NumDescriptors = 1;
                    ranges[rangeCount].BaseShaderRegister = psDescriptors.bufferDescriptors[i].registerIndex;
                    ranges[rangeCount].RegisterSpace = 0;
                    ranges[rangeCount].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    rangeCount++;
                }
                m_layoutInfo.ps_CBV_RootParamIndex = paramCount;
                params[paramCount].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[paramCount].DescriptorTable.NumDescriptorRanges = rangeCount - rangeBegin;
                params[paramCount].DescriptorTable.pDescriptorRanges = &ranges[rangeBegin];
                params[paramCount].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
                paramCount++;
            }
            // [PixelShader Stage] 't0' ~ 'tn'
            if (psDescriptors.textureDescriptors.size() > 0) {
                rangeBegin = rangeCount;
                for (int i = 0; i < psDescriptors.textureDescriptors.size(); i++) {
                    ranges[rangeCount].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                    ranges[rangeCount].NumDescriptors = 1;
                    ranges[rangeCount].BaseShaderRegister = psDescriptors.textureDescriptors[i].registerIndex;
                    ranges[rangeCount].RegisterSpace = 0;
                    ranges[rangeCount].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    rangeCount++;
                }
                m_layoutInfo.ps_SRV_RootParamIndex = paramCount;
                params[paramCount].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[paramCount].DescriptorTable.NumDescriptorRanges = rangeCount - rangeBegin;
                params[paramCount].DescriptorTable.pDescriptorRanges = &ranges[rangeBegin];
                params[paramCount].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
                paramCount++;
            }
            // [PixelShader Stage] 's0' ~ 'sn'
            if (psDescriptors.samplerDescriptors.size() > 0) {
                rangeBegin = rangeCount;
                for (int i = 0; i < psDescriptors.samplerDescriptors.size(); i++) {
                    ranges[rangeCount].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
                    ranges[rangeCount].NumDescriptors = 1;
                    ranges[rangeCount].BaseShaderRegister = psDescriptors.samplerDescriptors[i].registerIndex;
                    ranges[rangeCount].RegisterSpace = 0;
                    ranges[rangeCount].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    rangeCount++;
                }
                m_layoutInfo.ps_Sampler_RootParamIndex = paramCount;
                params[paramCount].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[paramCount].DescriptorTable.NumDescriptorRanges = rangeCount - rangeBegin;
                params[paramCount].DescriptorTable.pDescriptorRanges = &ranges[rangeBegin];
                params[paramCount].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
                paramCount++;
            }
            // [ComputeShader Stage] 'u#'
            if (csDescriptors.unorderedDescriptors.size() > 0) {
                rangeBegin = rangeCount;
                for (int i = 0; i < csDescriptors.unorderedDescriptors.size(); i++) {
                    ranges[rangeCount].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
                    ranges[rangeCount].NumDescriptors = 1;
                    ranges[rangeCount].BaseShaderRegister = csDescriptors.unorderedDescriptors[i].registerIndex;
                    ranges[rangeCount].RegisterSpace = 0;
                    ranges[rangeCount].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    rangeCount++;
                }
                m_layoutInfo.cs_CBV_RootParamIndex = paramCount;
                params[paramCount].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[paramCount].DescriptorTable.NumDescriptorRanges = rangeCount - rangeBegin;
                params[paramCount].DescriptorTable.pDescriptorRanges = &ranges[rangeBegin];
                params[paramCount].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
                paramCount++;
            }
            // [ComputeShader Stage] 't#'
            if (csDescriptors.textureDescriptors.size() > 0) {
                rangeBegin = rangeCount;
                for (int i = 0; i < csDescriptors.textureDescriptors.size(); i++) {
                    ranges[rangeCount].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                    ranges[rangeCount].NumDescriptors = 1;
                    ranges[rangeCount].BaseShaderRegister = csDescriptors.textureDescriptors[i].registerIndex;
                    ranges[rangeCount].RegisterSpace = 0;
                    ranges[rangeCount].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    rangeCount++;
                }
                m_layoutInfo.cs_SRV_RootParamIndex = paramCount;
                params[paramCount].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[paramCount].DescriptorTable.NumDescriptorRanges = rangeCount - rangeBegin;
                params[paramCount].DescriptorTable.pDescriptorRanges = &ranges[rangeBegin];
                params[paramCount].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
                paramCount++;
            }
        }

        {
            D3D12_ROOT_SIGNATURE_DESC desc = {};
            desc.NumParameters = paramCount;
            desc.pParameters = params.data();
            desc.NumStaticSamplers = 0;
            desc.pStaticSamplers = nullptr;
            desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

            ComPtr<ID3DBlob> signature;
            ComPtr<ID3DBlob> error;
            if (FAILED(D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error))) {
                const char* message = static_cast<const char*>(error->GetBufferPointer());
                LN_ERROR("D3D12SerializeRootSignature failed.");
                return false;
            }

            if (FAILED(dxDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)))) {
                LN_ERROR("CreateRootSignature failed.");
                return false;
            }
        }
    }

    return true;
}

void DX12ShaderPass::dispose()
{
    m_rootSignature.Reset();
    m_vsCode.Reset();
    m_psCode.Reset();
    IShaderPass::dispose();
}

D3D12_SHADER_BYTECODE DX12ShaderPass::dxVSByteCode() const
{
    if (m_vsCode)
        return D3D12_SHADER_BYTECODE{ m_vsCode->GetBufferPointer(), m_vsCode->GetBufferSize() };
    else
        return D3D12_SHADER_BYTECODE{ nullptr, 0 };
}

D3D12_SHADER_BYTECODE DX12ShaderPass::dxPSByteCode() const
{
    if (m_psCode)
        return D3D12_SHADER_BYTECODE{ m_psCode->GetBufferPointer(), m_psCode->GetBufferSize() };
    else
        return D3D12_SHADER_BYTECODE{ nullptr, 0 };
}

} // namespace detail
} // namespace ln
