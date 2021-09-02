
#include <Lumino.fxh>

struct VSInput
{
    float3    Pos            : POSITION0;
};

struct VSOutput
{
    float4    svPos        : SV_POSITION;
    float4    Pos            : TEXCOORD0;
};

struct PSInput
{
    float4    svPos        : SV_POSITION;
    float4    Pos            : TEXCOORD0;
};

VSOutput VS_ShadowCaster(VSInput input)
{
    VSOutput output;
    float4 pos = mul(float4(input.Pos, 1.0), ln_World);
    pos = mul(pos, ln_ViewProjection_Light0);
    output.Pos = pos;
    output.svPos = pos;
    return output;
}

float4 PS_ShadowCaster(PSInput input) : SV_TARGET0
{
    //return float4(1, 1, 1, 1);
    return float4(input.Pos.z / input.Pos.w, 0, 0, 1);
}

technique Main
{
    pass ShadowCaster
    {
        VertexShader = VS_ShadowCaster;
        PixelShader  = PS_ShadowCaster;
    }
}
