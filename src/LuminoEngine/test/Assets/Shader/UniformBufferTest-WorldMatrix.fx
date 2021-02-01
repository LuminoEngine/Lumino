
cbuffer Element1
{
    float4x4 _World;
};

cbuffer Element2
{
    float4 _Color;
};

//------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
};
 
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color: COLOR0;
};
 
VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(input.Pos, _World);
    output.Color = _Color;
    //output.Pos = mul(_World, input.Pos);
    return output;
}

//------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color: COLOR0;
};

float4 psMain(PS_INPUT input) : SV_TARGET
{
    return input.Color;
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

