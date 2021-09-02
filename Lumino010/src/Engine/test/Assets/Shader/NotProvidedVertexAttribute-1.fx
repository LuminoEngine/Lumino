
//------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 BlendWeight : BLENDWEIGHT;
};
 
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
};
 
VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = input.Pos;
    return output;
}

//------------------------------------------------------------------------------
float4 psMain() : SV_TARGET
{
    return float4(0, 0, 1, 1);
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

