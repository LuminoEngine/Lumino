
#include <Lumino.fxh>

LN_VSOutput_Common VS_ClusteredForward_Geometry(LN_VSInput input)
{
	return LN_ProcessVertex_Common(input);
}

float4 PS_Main(LN_PSInput_Common input) : SV_TARGET
{
    float4 color = ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.UV) * input.Color;
	return LN_GetBuiltinEffectColor(color);
}

technique Forward_Geometry
{
    pass Pass1
    {
        VertexShader = VS_ClusteredForward_Geometry;
        PixelShader = PS_Main;
    }
}
