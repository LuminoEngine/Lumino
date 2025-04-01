//#include <Lumino.fxh>

/*



C:\VulkanSDK\1.4.309.0\Bin\slangc.exe E:\Proj\Lumino\packages\LuminoEngine\shader\CopyScreen.slang -target spirv -entry PS_Main -o CopyScreen.spv -reflection-json ref.json

C:\VulkanSDK\1.4.309.0\Bin\slangc.exe E:\Proj\Lumino\packages\LuminoEngine\shader\CopyScreen.slang -target dxbc -entry PS_Main -o CopyScreen.dxbc -reflection-json ref.dxbc.frag.json
C:\VulkanSDK\1.4.309.0\Bin\slangc.exe E:\Proj\Lumino\packages\LuminoEngine\shader\CopyScreen.slang -target dxbc -entry VS_Main -o CopyScreen.dxbc -reflection-json ref.dxbc.vert.json

C:\VulkanSDK\1.4.309.0\Bin\slangc.exe E:\Proj\Lumino\packages\LuminoEngine\shader\CopyScreen.slang -target spirv -entry VS_Main -o CopyScreen.spv -reflection-json ref.spirv.vert.json


*/

struct LN_VSInput
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL0;
    float2 UV : TEXCOORD0;
    float4 Color : COLOR0;
    float4 tangent: TANGENT0;
};

uniform int ln_Value1;

//[[vk::binding(1,2)]]
Sampler2D ln_BoneTexture;
Sampler2D ln_MaterialTexture;
//[[vk::binding(1,2)]]
//Texture2D ln_MaterialTexture;
//[[vk::binding(1,3)]]
//SamplerState ln_MaterialTextureSampler;
//sampler2D ln_MaterialTexture;
uniform int ln_Value2;

//==============================================================================
// Vertex shader

struct VS_Output
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};

[shader("vertex")]
VS_Output VS_Main(LN_VSInput input)
{
    VS_Output output;
    output.Pos = float4(input.Pos, 1.0);
    output.UV = input.UV;
    float4 p = ln_MaterialTexture.SampleLevel(input.UV, 0);
    output.Pos.xyz += p.xyz;
    return output;
}

//==============================================================================
// Pixel shader

struct PS_Input
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};

// エイリアス？として pixel を使うことはできるらしい
[shader("pixel")]
float4 PS_Main(PS_Input input) : SV_TARGET
{
    return ln_MaterialTexture.Sample(input.UV);
    //return ln_MaterialTexture.Sample(ln_MaterialTextureSampler, input.UV);
    //
    //return tex2D(ln_MaterialTexture, input.UV);
}

// //==============================================================================
// // Technique

// technique Default
// {
//     ShadingModel = Unlit;
//     pass Pass0
//     {
//         VertexShader = VS_Main;
//         PixelShader = PS_Main;
//     }
// }
