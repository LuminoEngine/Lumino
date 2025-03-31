//#include <Lumino.fxh>


struct LN_VSInput
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL0;
    float2 UV : TEXCOORD0;
    float4 Color : COLOR0;
    float4 tangent: TANGENT0;
};
Sampler2D ln_MaterialTexture;
//Texture2D ln_MaterialTexture;
//SamplerState ln_MaterialTextureSampler;
//sampler2D ln_MaterialTexture;

//==============================================================================
// Vertex shader

struct VS_Output
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};

VS_Output VS_Main(LN_VSInput input)
{
    VS_Output output;
    output.Pos = float4(input.Pos, 1.0);
    output.UV = input.UV;
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
