
#include <Lumino.fxh>

//==============================================================================

sampler2D _occlusionMap;

//==============================================================================
// Vertex shader

struct VSInput
{
    float3 pos : POSITION0;
    float2 uv  : TEXCOORD0;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;
    output.pos = float4(input.pos, 1.0);
    output.uv = input.uv;
    return output;
}

//==============================================================================
// Pixel shader

struct PSInput
{
    float2 uv : TEXCOORD0;
};

float4 LN_5x5AverageBlur(sampler2D s, float2 resolution, float2 uv)
{
    float2 texelSize = (1.0 / resolution);
    float4 result = float4(0.0);

    for (int i=-2; i<=2; i++) {
        for (int j=-2; j<=2; j++) {
            float2 offset = ((float2(float(i),float(j)))*texelSize);
            result += tex2D(s, uv + offset);
        }
    }

    return result / (5.0*5.0);
}



float4 blur9(float2 uv, float2 resolution, float2 direction) {
  float4 color = float4(0, 0, 0, 0);
  float2 off1 = float2(1.3846153846, 1.3846153846) * direction;
  float2 off2 = float2(3.2307692308, 3.2307692308) * direction;
  color += tex2D(_occlusionMap, uv) * 0.2270270270;
  color += tex2D(_occlusionMap, uv + (off1 / resolution)) * 0.3162162162;
  color += tex2D(_occlusionMap, uv - (off1 / resolution)) * 0.3162162162;
  color += tex2D(_occlusionMap, uv + (off2 / resolution)) * 0.0702702703;
  color += tex2D(_occlusionMap, uv - (off2 / resolution)) * 0.0702702703;
  return color;
}

float4 PSMain(PSInput input) : SV_TARGET0
{
	float4 c = clamp(tex2D(ln_MaterialTexture, input.uv), float4(0,0,0,0), float4(1,1,1,1)) + 0.4;
	//float4 c1 = blur9(_occlusionMap, input.uv, ln_Resolution.xy, float2(0.75, 0.0));
	float4 c2 = blur9(input.uv, ln_Resolution.xy, float2(0.0, 0.95));
    //float4 c2 = LN_5x5AverageBlur(_occlusionMap, ln_Resolution.xy, input.uv);
	return c * c2* c2;
	//return c1 * c2;
	//return tex2D(_occlusionMap, input.uv);
}

//==============================================================================
// Technique

technique Forward_Geometry_UnLighting
{
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}

