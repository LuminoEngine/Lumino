
cbuffer Element
{
    float4x4 _World;
};

//------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
};
 
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
};
 
VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(input.Pos, _World);
    //output.Pos = mul(_World, input.Pos);
    return output;
}

//------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
};

float4 psMain(PS_INPUT input) : SV_TARGET
{
    return float4(1.0, 0.0, 0.0, 1.0);
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

