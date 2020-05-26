
cbuffer ConstBuff0 : register(b1)
{
	float4x4 _WorldViewProjection;
};

cbuffer ConstBuff1 : register(b0)
{
	float4 _Color1;
}

sampler2D _Texture1 : register(s0);
sampler2D _Texture2 : register(s1);

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
    return _Color1 + tex2D(_Texture1, float2(0, 0)) + tex2D(_Texture2, float2(0, 0));
}

//------------------------------------------------------------------------------
technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain1;
        PixelShader = psMain1;
    }
}

