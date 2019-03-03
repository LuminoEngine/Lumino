
float4x4 model;
float4x4 view;
float4x4 proj;

Texture2D g_texture1;
SamplerState g_samplerState1;

struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Color : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Color : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    //output.Pos = (model * view * proj) * float4(input.Pos, 1);
    //output.Pos = proj * view * model * float4(input.Pos, 1);
    //output.Pos = mul(float4(input.Pos, 1), (model * view * proj));
    //output.Pos = mul(float4(input.Pos, 1), model);
    //output.Pos = mul(float4(input.Pos, 1), (proj * view * model));
    output.Pos = mul(float4(input.Pos, 1), model);
    output.Pos = mul(output.Pos, view);
    output.Pos = mul(output.Pos, proj);
    output.Color = input.Color;
    output.TexCoord = input.TexCoord;
    return output;
}

struct PS_INPUT
{
    float3 Color : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

float4 psMain(PS_INPUT input) : SV_Target
{
    //return float4(input.TexCoord, 0, 1);
    return g_texture1.Sample(g_samplerState1, input.TexCoord);
}

technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain;
    }
}


