
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

struct PS_INPUT
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


float4 psMain(PS_INPUT input) : SV_TARGET
{
#ifdef BLUE
    return float4(0, 0, 1, 1);
#elif GREEN==1
    return float4(0, 1, 0, 1);
#else
    return float4(1, 0, 0, 1);
#endif
}

technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain;
    }
}

