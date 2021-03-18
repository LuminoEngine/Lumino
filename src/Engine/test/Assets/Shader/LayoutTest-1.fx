
float4x4 _WorldViewProjection;	// PASS0 unused
float4 _Color1;	// unused
float4 _Color2;

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
 
VS_OUTPUT vsMain1(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = input.Pos;
    return output;
}

VS_OUTPUT vsMain2(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(input.Pos, _WorldViewProjection);
    return output;
}

//------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Color : COLOR0;
};

float4 psMain1(PS_INPUT input) : SV_TARGET
{
    return _Color2 + tex2D(_Texture1, float2(0, 0)) + tex2D(_Texture2, float2(0, 0));
}

float4 psMain2(PS_INPUT input) : SV_TARGET
{
    return _Color2 + tex2D(_Texture2, float2(0, 0));
}

//------------------------------------------------------------------------------
technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain1;
        PixelShader = psMain1;
    }
    pass Pass1
    {
        VertexShader = vsMain2;
        PixelShader = psMain2;
    }
}

