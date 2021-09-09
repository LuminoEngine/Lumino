
#include <Lumino.fxh>

float _Factor;
float4 _ColorScale;
sampler2D _OverrayTexture;

struct VS_Input
{
    float3    Pos        : POSITION;
    float2    UV        : TEXCOORD0;
};

struct VS_Output
{
    float4    Pos        : SV_POSITION;
    float2    UV        : TEXCOORD0;
};

struct PS_Input
{
    float2    UV        : TEXCOORD0;
};

VS_Output VS_Main(VS_Input input)
{
    VS_Output o;
    o.Pos = float4(input.Pos, 1.0);
    o.UV  = input.UV;
    return o;
}

float4 PS_Main(PS_Input input) : COLOR0
{
    float3 c = lerp(
        tex2D(ln_MaterialTexture, input.UV).rgb,
        tex2D(_OverrayTexture, input.UV).rgb * _ColorScale.rgb,
        saturate(_Factor));
    return float4(c, 1.0);
}

technique Forward_Geometry_UnLighting
{
    ShadingModel = Unlit;
    pass P0
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}

