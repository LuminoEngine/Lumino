
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

#if 0
        // NOTE: D3D12_ROOT_PARAMETER は複数作るべき？
        //   今は D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE しか使っていないが、他の TYPE を使いたい場合は分ける必要がある。
        //   また ShaderStage ごとにアクセス可否を決めてパフォーマンスを上げる時にも分ける必要がある。
        //   D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE しか使わず、D3D12_SHADER_VISIBILITY_ALL でもよいなら
        //   D3D12_ROOT_PARAMETER はひとつでも構わない。
        //   ShaderStage のアクセスを最適化するなら、それに引きずられて D3D12_DESCRIPTOR_RANGE も複数作る必要がある。
        //
        //   ちなみにTableすべてが1つのDescriptorしかバインドしないとしても64個のDescriptorをバインドできる。
        //   https://sites.google.com/site/monshonosuana/directxno-hanashi-1/directx-145

        // NOTE: D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE 以外の TYPE は使わない？
        //   それらは、Table を使うまでもない非常に小さいデータや、Table を使うとかえって冗長になってしまうケースで使用する。
        //   サンプルプログラムならともかく、複雑なシーンで DrawCall ごとに切り替えが多く発生するレジスタに対しては Table の方が良い。
        //   https://sites.google.com/site/monshonosuana/directxno-hanashi-1/directx-145

        //const size_t paramCount =
        //    createInfo.descriptorLayout->uniformBufferRegister.size() +
        //    createInfo.descriptorLayout->textureRegister.size() +
        //    createInfo.descriptorLayout->textureRegister.size();
        std::array<D3D12_DESCRIPTOR_RANGE, 32> ranges;
        std::array<D3D12_ROOT_PARAMETER, 32> params;
        //ranges.reserve(paramCount);
        //params.reserve(paramCount);
        int paramCount = 0;

        // 'b' register
        for (size_t i = 0; i < createInfo.descriptorLayout->uniformBufferRegister.size(); i++) {
            const DescriptorLayoutItem& item = createInfo.descriptorLayout->uniformBufferRegister[i];

            D3D12_DESCRIPTOR_RANGE range;
            range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
            range.NumDescriptors = 1;
            range.BaseShaderRegister = item.binding;   // 'b0' ... 'bn'
            range.RegisterSpace = 0;
            range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            ranges[paramCount] = range;

            D3D12_ROOT_PARAMETER param;
            param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            param.DescriptorTable.NumDescriptorRanges = 1;
            param.DescriptorTable.pDescriptorRanges = &ranges[paramCount];
            param.ShaderVisibility = getShaderVisibility(item);
            params[i] = param;

            paramCount++;
        }

        // 't' register
        for (size_t i = 0; i < createInfo.descriptorLayout->textureRegister.size(); i++) {
            const DescriptorLayoutItem& item = createInfo.descriptorLayout->textureRegister[i];

            D3D12_DESCRIPTOR_RANGE range;
            range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range.NumDescriptors = 1;
            range.BaseShaderRegister = item.binding;   // 't0' ... 'tn'
            range.RegisterSpace = 0;
            range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            ranges[paramCount] = range;

            D3D12_ROOT_PARAMETER param;
            param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            param.DescriptorTable.NumDescriptorRanges = 1;
            param.DescriptorTable.pDescriptorRanges = &ranges[paramCount];
            param.ShaderVisibility = getShaderVisibility(item);
            params[i] = param;

            paramCount++;
        }

        // 's' register
        for (size_t i = 0; i < createInfo.descriptorLayout->samplerRegister.size(); i++) {
            const DescriptorLayoutItem& item = createInfo.descriptorLayout->samplerRegister[i];

            D3D12_DESCRIPTOR_RANGE range;
            range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
            range.NumDescriptors = 1;
            range.BaseShaderRegister = item.binding;   // 's0' ... 'sn'
            range.RegisterSpace = 0;
            range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            ranges[paramCount] = range;

            D3D12_ROOT_PARAMETER param;
            param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            param.DescriptorTable.NumDescriptorRanges = 1;
            param.DescriptorTable.pDescriptorRanges = &ranges[paramCount];
            param.ShaderVisibility = getShaderVisibility(item);
            params[i] = param;

            paramCount++;
        }
#endif
#if 0
        // NOTE: D3D12_ROOT_PARAMETER は複数作るべき？
        //   前提として、最低限 [CBV,SRV] [SAMPLER] の 2 つは同じ ID3D12DescriptorHeap で
        //   扱うことができないため、絶対に分けなければならない。
        //   それなら Vulkan の実装と同じように 3 つ分けてしまった方が似た実装にできるので混乱を避けられそう。
        //
        //   ちなみにTableすべてが1つのDescriptorしかバインドしないとしても64個のDescriptorをバインドできる。
        //   https://sites.google.com/site/monshonosuana/directxno-hanashi-1/directx-145

        // NOTE: D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE 以外の TYPE は使わない？
        //   それらは、Table を使うまでもない非常に小さいデータや、Table を使うとかえって冗長になってしまうケースで使用する。
        //   ただ、サンプルプログラムならともかく、複雑なシーンで DrawCall が頻発するようなケース相手にするため
        //   今はこれ以上実装を複雑にしたくない。

        // 1つの DescriptorTable にすべての Descriptor をバインドするパターン。
        // DescriptorHeap もひとつ。

        std::array<D3D12_DESCRIPTOR_RANGE, 3> ranges = {};
        std::array<D3D12_ROOT_PARAMETER, 3> params = {};
        int i = 0;

        // 'b' register
        if (m_layoutInfo.cbvCount > 0) {
            ranges[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
            ranges[i].NumDescriptors = m_layoutInfo.cbvCount;       // 'b0' ~ 'bn'
            ranges[i].BaseShaderRegister = 0;                       // 'b0' ~ 'bn'
            ranges[i].RegisterSpace = 0;
            ranges[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

            params[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            params[i].DescriptorTable.NumDescriptorRanges = 1;
            params[i].DescriptorTable.pDescriptorRanges = &ranges[i];
            params[i].ShaderVisibility = getShaderVisibility(createInfo.descriptorLayout, DescriptorType_UniformBuffer);
            // NOTE: ↑この getShaderVisibility() は全ての CBV に対しての設定となるため、最適解ではない。
            // ただ個々の CBV まで対応となると非常に複雑になるためここまでにしておく。

            m_layoutInfo.cbvRootParamIndex = i;
            i++;
        }

        // 't' register
        if (m_layoutInfo.srvCount > 0) {
            ranges[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            ranges[i].NumDescriptors = m_layoutInfo.srvCount;       // 't0' ~ 'tn'
            ranges[i].BaseShaderRegister = 0;                       // 't0' ~ 'tn'
            ranges[i].RegisterSpace = 0;
            ranges[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

            params[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            params[i].DescriptorTable.NumDescriptorRanges = 1;
            params[i].DescriptorTable.pDescriptorRanges = &ranges[i];
            params[i].ShaderVisibility = getShaderVisibility(createInfo.descriptorLayout, DescriptorType_Texture);

            m_layoutInfo.srvRootParamIndex = i;
            i++;
        }

        // 's' register
        if (m_layoutInfo.samplerCount > 0) {
            ranges[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
            ranges[i].NumDescriptors = m_layoutInfo.samplerCount;   // 's0' ~ 'sn'
            ranges[i].BaseShaderRegister = 0;                       // 's0' ~ 'sn'
            ranges[i].RegisterSpace = 0;
            ranges[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

            params[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            params[i].DescriptorTable.NumDescriptorRanges = 1;
            params[i].DescriptorTable.pDescriptorRanges = &ranges[i];
            params[i].ShaderVisibility = getShaderVisibility(createInfo.descriptorLayout, DescriptorType_SamplerState);

            m_layoutInfo.samperRootParamIndex = i;
            i++;
        }

        D3D12_ROOT_SIGNATURE_DESC desc = {};
        desc.NumParameters = i;
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
#endif
#if 0
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

        DX12ShaderPassLayoutInfo::Descriptors& vsDescriptors = m_layoutInfo.vsDescriptors;
        DX12ShaderPassLayoutInfo::Descriptors& psDescriptors = m_layoutInfo.psDescriptors;

        // VS, PS ごとに集計
        {
            // 'b' register
            for (size_t i = 0; i < createInfo.descriptorLayout->uniformBufferRegister.size(); i++) {
                const DescriptorLayoutItem& item = createInfo.descriptorLayout->uniformBufferRegister[i];
                if (item.stageFlags & ShaderStageFlags_Vertex) {
                    vsDescriptors.bufferDescriptors.push_back(i);
                    vsDescriptors.bufferSizes.push_back(item.size);
                }
                if (item.stageFlags & ShaderStageFlags_Pixel) {
                    psDescriptors.bufferDescriptors.push_back(i);
                    psDescriptors.bufferSizes.push_back(item.size);
                }
            }
            // 't' register
            for (size_t i = 0; i < createInfo.descriptorLayout->textureRegister.size(); i++) {
                const DescriptorLayoutItem& item = createInfo.descriptorLayout->textureRegister[i];
                if (item.stageFlags & ShaderStageFlags_Vertex) {
                    vsDescriptors.textureDescriptors.push_back(i);
                }
                if (item.stageFlags & ShaderStageFlags_Pixel) {
                    psDescriptors.textureDescriptors.push_back(i);
                }
            }
            // 's' register
            for (size_t i = 0; i < createInfo.descriptorLayout->samplerRegister.size(); i++) {
                const DescriptorLayoutItem& item = createInfo.descriptorLayout->samplerRegister[i];
                if (item.stageFlags & ShaderStageFlags_Vertex) {
                    vsDescriptors.samplerDescriptors.push_back(i);
                }
                if (item.stageFlags & ShaderStageFlags_Pixel) {
                    psDescriptors.samplerDescriptors.push_back(i);
                }
            }
        }

        std::array<D3D12_DESCRIPTOR_RANGE, 6> ranges = {};
        std::array<D3D12_ROOT_PARAMETER, 6> params = {};
        int i = 0;
        int paramCount = 0;
        {
            // [VertexShader Stage] 'b0' ~ 'bn'
            if (vsDescriptors.bufferDescriptors.size() > 0) {
                ranges[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                ranges[i].NumDescriptors = vsDescriptors.bufferDescriptors.size();
                ranges[i].BaseShaderRegister = 0;
                ranges[i].RegisterSpace = 0;
                ranges[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

                params[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[i].DescriptorTable.NumDescriptorRanges = 1;
                params[i].DescriptorTable.pDescriptorRanges = &ranges[i];
                params[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

                m_layoutInfo.vs_CBV_RootParamIndex = i;
                m_layoutInfo.vs_CBV_Count = vsDescriptors.bufferDescriptors.size();
                i++;
            }
            // [VertexShader Stage] 't0' ~ 'tn'
            if (vsDescriptors.textureDescriptors.size() > 0) {
                ranges[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                ranges[i].NumDescriptors = vsDescriptors.textureDescriptors.size();
                ranges[i].BaseShaderRegister = 0;
                ranges[i].RegisterSpace = 0;
                ranges[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

                params[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[i].DescriptorTable.NumDescriptorRanges = 1;
                params[i].DescriptorTable.pDescriptorRanges = &ranges[i];
                params[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

                m_layoutInfo.vs_SRV_RootParamIndex = i;
                m_layoutInfo.vs_SRV_Count = vsDescriptors.textureDescriptors.size();
                i++;
            }
            // [VertexShader Stage] 's0' ~ 'sn'
            if (vsDescriptors.samplerDescriptors.size() > 0) {
                ranges[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
                ranges[i].NumDescriptors = vsDescriptors.samplerDescriptors.size();
                ranges[i].BaseShaderRegister = 0;
                ranges[i].RegisterSpace = 0;
                ranges[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

                params[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[i].DescriptorTable.NumDescriptorRanges = 1;
                params[i].DescriptorTable.pDescriptorRanges = &ranges[i];
                params[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

                m_layoutInfo.vs_Sampler_RootParamIndex = i;
                m_layoutInfo.vs_Sampler_Count = vsDescriptors.samplerDescriptors.size();
                i++;
            }

            // [PixelShader Stage] 'b0' ~ 'bn'
            if (psDescriptors.bufferDescriptors.size() > 0) {
                ranges[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                ranges[i].NumDescriptors = psDescriptors.bufferDescriptors.size();
                ranges[i].BaseShaderRegister = 0;
                ranges[i].RegisterSpace = 0;
                ranges[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

                params[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[i].DescriptorTable.NumDescriptorRanges = 1;
                params[i].DescriptorTable.pDescriptorRanges = &ranges[i];
                params[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

                m_layoutInfo.ps_CBV_RootParamIndex = i;
                m_layoutInfo.ps_CBV_Count = psDescriptors.bufferDescriptors.size();
                i++;
            }
            // [PixelShader Stage] 't0' ~ 'tn'
            if (psDescriptors.textureDescriptors.size() > 0) {
                ranges[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                ranges[i].NumDescriptors = psDescriptors.textureDescriptors.size();
                ranges[i].BaseShaderRegister = 0;
                ranges[i].RegisterSpace = 0;
                ranges[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

                params[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[i].DescriptorTable.NumDescriptorRanges = 1;
                params[i].DescriptorTable.pDescriptorRanges = &ranges[i];
                params[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

                m_layoutInfo.ps_SRV_RootParamIndex = i;
                m_layoutInfo.ps_SRV_Count = psDescriptors.textureDescriptors.size();
                i++;
            }
            // [PixelShader Stage] 's0' ~ 'sn'
            if (psDescriptors.samplerDescriptors.size() > 0) {
                ranges[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
                ranges[i].NumDescriptors = psDescriptors.samplerDescriptors.size();
                ranges[i].BaseShaderRegister = 0;
                ranges[i].RegisterSpace = 0;
                ranges[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

                params[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[i].DescriptorTable.NumDescriptorRanges = 1;
                params[i].DescriptorTable.pDescriptorRanges = &ranges[i];
                params[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

                m_layoutInfo.ps_Sampler_RootParamIndex = i;
                m_layoutInfo.ps_Sampler_Count = psDescriptors.samplerDescriptors.size();
                i++;
            }
        }

        {
            D3D12_ROOT_SIGNATURE_DESC desc = {};
            desc.NumParameters = i;
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
#endif


#if 1
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
        Reflection reflections[ShaderStage2_Count];

        // まず D3DReflect を使って、D3DCompiler が必要としている Descriptor を取得する。
        // この時点で、glslang では最適化で削除された Descriptor が、D3DCompiler では存在していることもある。 
        {
            std::pair<const void*, size_t> shaderCodes[ShaderStage2_Count] = { { createInfo.vsCode, createInfo.vsCodeLen },  { createInfo.psCode, createInfo.psCodeLen } };
            for (int iStage = 0; iStage < ShaderStage2_Count; iStage++) {
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

                    if (type != DescriptorType_Count) {
                        std::string name = desc2.Name;
                        if (name == "$Globals") name = "$Global";
                        reflections[iStage].descriptors.push_back({ type, std::move(name), (int32_t)desc2.BindPoint });
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

    //const auto cbCount = desc.ConstantBuffers;
    //for (auto i = 0; i < cbCount; ++i)
    //{
    //    D3D12_SHADER_BUFFER_DESC shaderBufDesc{};
    //    auto cbuffer = shaderReflection->GetConstantBufferByIndex(i);
    //    cbuffer->GetDesc(&shaderBufDesc);

    //    std::cout << "  " << shaderBufDesc.Name << std::endl;
    //}

        DX12ShaderPassLayoutInfo::Descriptors& vsDescriptors = m_layoutInfo.vsDescriptors;
        DX12ShaderPassLayoutInfo::Descriptors& psDescriptors = m_layoutInfo.psDescriptors;

        // VS, PS ごとに集計
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

        std::array<D3D12_DESCRIPTOR_RANGE, MaxDescriptors * 6> ranges = {};
        std::array<D3D12_ROOT_PARAMETER, 6> params = {};
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
#endif
    }

    //{
    //    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    //    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    //    desc.NumDescriptors = 1;
    //    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    //    desc.NodeMask = 0;
    //    if (FAILED(m_deviceContext->device()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_descriptorHeap)))) {
    //        LN_ERROR("CreateDescriptorHeap failed.");
    //        return false;
    //    }
    //}

    return true;
}

void DX12ShaderPass::dispose()
{
    //m_descriptorHeap.Reset();
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
