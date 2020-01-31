
#include <Lumino.fxh>

LN_VSOutput_Common VS_ClusteredForward_Geometry(LN_VSInput input)
{
	return LN_ProcessVertex_Common(input);
}

float4 PS_Main(LN_PSInput_Common input) : SV_TARGET
{
    //return float4(1, 0, 0, 1);
    //float4 color = ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.UV) * input.Color;
    float4 color = tex2D(ln_MaterialTexture, input.UV) * input.Color;
    
    clip(color.a - 0.0001);

    //color.xy *= input.UV;
    //color.a = 1.0;

    //color.r = 1;
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
