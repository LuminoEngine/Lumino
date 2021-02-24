
#include <Lumino.fxh>

#define BLEND_LERP 0
#define BLEND_ADD 1

//==============================================================================
// Uniforms

sampler2D _LensflareOcclusionMap;
static const float radialBlurScale = 0.35;
static const float radialBlurStrength = 0.75;
static const float2 radialOrigin = float2(0.0, 0.0);   // screen space. center:(0,0) top-left:(-1,1)

//==============================================================================
// Vertex shader

struct VS_Input
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD0;
};

struct VS_Output
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};

VS_Output VS_Main(LN_VSInput input)
{
    VS_Output output;
    output.Pos = float4(input.Pos, 1.0);
    output.UV = input.UV;
    return output;
}

//==============================================================================
// Pixel shader

struct PS_Input
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};

float4 PS_Main(PS_Input input) : SV_TARGET
{
    // Sampled texture (_LensflareOcclusionMap) size.
    // FIXME: Originally, it should be the size of _LensflareOcclusionMap.
    //        Currently used only in Posteffect, and the size of in and out match, so leave it as is.
    float2 texDim = ln_Resolution.xy;
    float2 radialSize = float2(1.0 / texDim.x, 1.0 / texDim.y); // 1px size
    
    float2 UV = input.UV;

#if 1   // UV to Viewport
    float2 radialOffset =  ((float2(0.0, -1.0) * radialOrigin + 1.0) / 2.0);
#else
    float2 radialOffset = radialOrigin;
#endif

    float4 color = float4(0.0, 0.0, 0.0, 0.0);
    UV += radialSize * 0.5 - radialOffset;
 
    #define samples 32

    for (int i = 0; i < samples; i++) 
    {
        float scale = 1.0 - radialBlurScale * (float(i) / float(samples-1));
        color += tex2D(_LensflareOcclusionMap, UV * scale + radialOffset);
    }
 
    float4 result = (color / samples) * radialBlurStrength;

#if BLEND_LERP
    return lerp(tex2D(ln_MaterialTexture, input.UV), result, result.r);
#endif
#if BLEND_ADD
    return tex2D(ln_MaterialTexture, input.UV) + result;
#endif
}

//==============================================================================
// Technique

technique Forward_Geometry_UnLighting
{
    ShadingModel = Unlit;
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}

