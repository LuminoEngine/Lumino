
struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Color : COLOR0;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};

VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = float4(input.Pos, 1);
    output.Color = input.Color;
    return output;
}


struct PS_INPUT
{
    float4 Color : COLOR0;
};

float4 psMain(PS_INPUT input) : SV_Target
{
    return input.Color;
}


technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain;
    }
}

