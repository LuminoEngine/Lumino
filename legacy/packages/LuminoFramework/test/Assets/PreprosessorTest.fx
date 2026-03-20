
#include "PreprosessorTest.fxh"

VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = input.Pos;
    output.Color = input.Color;
    return output;
}

cbuffer ConstBuff2 : register(b0)
{
    float4 g_color;
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

