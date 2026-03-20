
sampler2D _Texture1;
sampler2D _Texture2;

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
    output.Pos = input.Pos;
    return output;
}

//------------------------------------------------------------------------------
float4 psMain1() : SV_TARGET
{
    return tex2D(_Texture1, float2(0, 0));
}

//------------------------------------------------------------------------------
float4 psMain2() : SV_TARGET
{
    return tex2D(_Texture2, float2(0, 0));
}

//------------------------------------------------------------------------------
float4 psMain3() : SV_TARGET
{
    return tex2D(_Texture1, float2(0, 0)) + tex2D(_Texture2, float2(0, 0));
}

//------------------------------------------------------------------------------
technique Tech1
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain1;
    }
}

technique Tech2
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain2;
    }
}

technique Tech3
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain3;
    }
}

