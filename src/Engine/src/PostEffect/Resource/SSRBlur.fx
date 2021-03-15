
#include <Lumino.fxh>

//==============================================================================
// Uniforms


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
    float2 texelSize = (1.0 / ln_Resolution.xy);
    float4 result = float4(0.0, 0.0, 0.0, 0.0);

    for (int i=-2; i<=2; i++) {
        for (int j=-2; j<=2; j++) {
            float2 offset = ((float2(float(i),float(j)))*texelSize);
            result += tex2D(ln_MaterialTexture, input.UV+offset);
        }
    }

    return result / (5.0*5.0);
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

