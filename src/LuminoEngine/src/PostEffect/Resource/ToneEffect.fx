
#include <Lumino.fxh>

float4 _Tone;
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
	output.UV			= input.UV;// + (float2(0.5, 0.5) / ln_Resolution.xy);
	output.Color			= input.Color;
	return output;
}
*/

LN_VSOutput_Common VS_ClusteredForward_Geometry(LN_VSInput input)
{
	LN_VSOutput_Common output = LN_ProcessVertex_Common(input);
	output.svPos = float4(input.Pos, 1.0f);
	return output;
}

float4 PS_Main(LN_PSInput_Common input) : SV_TARGET
{
	//return float4(input.UV, 0, 1);
	//return float4(1, 0, 0, 1);
	float4 color = tex2D(ln_MaterialTexture, input.UV);
	//float4 color = ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.UV);
	//loat4 t = float4(0.8, 0.5, 0.2, 0.5);
	//color.r = 1;
	//color.a = 1;
	//return color;
	return LN_CalculateToneColor(color, _Tone);
}

technique Forward_Geometry_UnLighting
{
    ShadingModel = Unlit;
    pass Pass1
    {
        VertexShader = VS_ClusteredForward_Geometry;
        PixelShader = PS_Main;
    }
}

