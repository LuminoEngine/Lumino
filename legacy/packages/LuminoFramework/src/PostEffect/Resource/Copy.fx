
#include <Lumino.fxh>

//==============================================================================
// Vertex shader

struct VSInput
{
    float3 Pos : POSITION0;
    float2 UV  : TEXCOORD0;
};

struct VSOutput
{
    float4 Pos : SV_POSITION;
    float2 UV  : TEXCOORD0;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;
    output.Pos = float4(input.Pos, 1.0);
    output.UV = input.UV;
    return output;
}

//==============================================================================
// Pixel shader

struct PSInput
{
    float2 UV : TEXCOORD0;
};

float4 PSMain(PSInput input) : SV_TARGET0
{
	return tex2D(ln_MaterialTexture, input.UV);
}

//==============================================================================
// Technique

technique Forward_Geometry_UnLighting
{
    ShadingModel = Unlit;
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}

