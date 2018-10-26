// lumino-cli fxc FxcTest1.fx FxcTest1.lnufx

struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
};
 
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};
 
VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = input.Pos;
    output.Color = input.Color;
    return output;
}

cbuffer ConstBuff\
{
    float4 g_color;
};

struct PS_INPUT
{
    float4 Color : COLOR;
};

float4 psMain(PS_INPUT input) : SV_TARGET
{
    return input.Color + g_color;
}

technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain;
    }
}

