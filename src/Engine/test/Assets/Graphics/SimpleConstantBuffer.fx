
float4 _Color;

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
    return _Color;
}

technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain;
    }
}

