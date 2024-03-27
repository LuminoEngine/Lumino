
cbuffer ConstBuff// : register(b0)
{
    float4 g_color;
};

struct UserData
{
    float4 Color2 : TEXCOORD1;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
};
 
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    UserData User;
};
 
VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = input.Pos;
    output.Color = input.Color;
    output.User.Color2 = g_color;
    return output;
}

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    UserData User;
};

float4 psMain(PS_INPUT input) : SV_TARGET
{
    return input.Color + input.User.Color2;
}

technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain;
    }
}

