
float4x4 _WorldViewProjection;  // unused
float4 _Color1;
float4 _Color2;

//------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
};
 
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};
 
VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = input.Pos;
    output.Color = _Color1;
    return output;
}

//------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};

float4 psMain(PS_INPUT input) : SV_TARGET
{
    return input.Color + _Color2;
}

//------------------------------------------------------------------------------
technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain;
    }
}

