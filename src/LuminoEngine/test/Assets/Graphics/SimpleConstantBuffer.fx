
float4 g_color;

cbuffer ConstBuff2
{
    float4 g_color5;
    float4 g_color6;
};

cbuffer ConstBuff : register(b0)
{
    float4 g_color2;
};

float4 g_color3;
float4 g_color4;



struct VS_INPUT
{
    float3 Pos : POSITION;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
};

VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = float4(input.Pos, 1);
    return output;
}

float4 psMain() : SV_Target
{
    return g_color + g_color2 + g_color3 +  + g_color4 +  + g_color5 +  + g_color6;
}

technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain;
    }
}

