
#include <Lumino.fxh>

float4x4 _TextureMatrix;

struct VSInput
{
    float3 Pos  : POSITION0;
    float2 UV   : TEXCOORD0;
};

struct VSOutput
{
    float4 Pos  : SV_POSITION;
    float4 UV   : TEXCOORD0;
};

struct PSInput
{
    float4 UV   : TEXCOORD0;
};

inline float4 ComputeNonStereoScreenPos(float4 pos) {
    float4 o = pos * 0.5f;
    o.xy = float2(o.x, o.y*-1.0) + o.w;
    o.zw = pos.zw;
    return o;
}

VSOutput VSMain(VSInput input)
{
    VSOutput output;
    output.Pos = mul(float4(input.Pos, 1.0), ln_WorldViewProjection);
    output.UV = mul(float4(input.Pos, 1.0), _TextureMatrix);
    output.UV = ComputeNonStereoScreenPos(output.UV);
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET0
{
    float4 result = tex2Dproj(ln_MaterialTexture, input.UV);
    result.rgb *= 0.8;
    return result;
}

technique Forward_Geometry
{
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}
