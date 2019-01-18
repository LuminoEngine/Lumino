

Texture2D ln_MaterialTexture : register(t0);
SamplerState g_samplerState1 : register(s0);

struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float2 TexUV : TEXCOORD0;
};
 
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 TexUV : TEXCOORD0;
};
 
VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = input.Pos;
    output.Color = input.Color;
    output.TexUV = input.TexUV;
    return output;
}

//cbuffer ConstBuff
//{
    float4 g_color;
//};

struct PS_INPUT
{
    float4 Color : COLOR;
    float2 TexUV : TEXCOORD0;
};

float4 psMain(PS_INPUT input) : SV_TARGET
{
    return ln_MaterialTexture.Sample(g_samplerState1, input.TexUV);
    //return input.Color * g_texture1.Sample(g_samplerState1, input.TexUV) + g_color;
}

technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain;
    }
}

