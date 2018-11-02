
#include <Lumino.fxh>

/*
struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD0;
};
*/
/*
struct VS_OUTPUT
{
    //float4 svPos : SV_POSITION;
    //float4 Color : COLOR;
    //float2 UV : TEXCOORD0;

    
	float4	svPos		: SV_POSITION;
	float4	Color		: COLOR0;
	float2	UV			: TEXCOORD0;
	float4	Normal		: NORMAL0;
    //float3	Normal		: NORMAL0;
};
 
VS_OUTPUT vsMain(LN_VSInput input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
	output.svPos			= float4(input.Pos, 1.0f);//mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
	output.Normal		= float4(0, 1, 0, 0);//input.Normal;//mul(float4(input.Normal, 1.0f), ln_WorldViewIT).xyz;
	output.UV			= input.UV;// + (float2(0.5, 0.5) / ln_ViewportPixelSize);
	output.Color			= input.Color;
	return output;
}
*/

LN_VSOutput_Common VS_ClusteredForward_Geometry(LN_VSInput input)
{
	return LN_ProcessVertex_Common(input);
}

float4 PS_Main(LN_PSInput_Common input) : SV_TARGET
{
    return ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.UV) * input.Color;
}

technique Forward_Geometry
{
    pass Pass1
    {
        VertexShader = VS_ClusteredForward_Geometry;
        PixelShader = PS_Main;
    }
}

