

#include <Lumino.fxh>

//==============================================================================
// Uniforms

float3 _Color;
float _Opacity;
float _LuminosityThreshold;
float _SmoothWidth;

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
    float4 texel = tex2D(ln_MaterialTexture, input.UV);
    float3 luma = float3(0.299, 0.587, 0.114);
    float v = saturate(dot(texel.xyz, luma));
    float4 outputColor = float4(_Color.rgb, _Opacity);
    float alpha = smoothstep(_LuminosityThreshold, _LuminosityThreshold + _SmoothWidth, v);
    return lerp(outputColor, texel, alpha);
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

