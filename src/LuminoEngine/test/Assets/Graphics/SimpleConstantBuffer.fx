
cbuffer ConstBuffV1
{
    float4 g_pos1;
    float4 g_pos2;
};

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

float4 g_pos10;

Texture2D g_texture1 : register(t0);
SamplerState g_samplerState1 : register(s0);

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
    output.Pos = float4(input.Pos, 1) + g_pos2 + g_color5 + g_color6 + g_pos10;
    return output;
}

float4 psMain() : SV_Target
{
    return g_color + g_color2 + g_color3 +  + g_color4 +  + g_color5 +  + g_color6 + g_texture1.Sample(g_samplerState1, float2(0, 0));
}

technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain;
    }
}

