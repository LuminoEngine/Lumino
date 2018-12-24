

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
	output.UV			= input.UV;// + (float2(0.5, 0.5) / ln_ViewportPixelSize);
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
	float4 color = ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.UV);
	return LN_CalculateToneColor(color, _Tone);
	//return color;
	//return float4(input.UV, 1, 1);
    //return ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.UV) * _BlurColor;
}

technique Forward_Geometry_UnLighting
{
    pass Pass1
    {
        VertexShader = VS_ClusteredForward_Geometry;
        PixelShader = PS_Main;
    }
}



#if 0
#include <Lumino.fxh>

/*
float4 LN_CalculateToneColor(float4 inColor, float4 inToneColor)
{
	float4 outColor = inColor;
	float y = (0.208012 * outColor.r + 0.586611 * outColor.g + 0.114478 * outColor.b) * inToneColor.w;
	outColor.rgb = (outColor.rgb * (1.0 - inToneColor.w)) + y + inToneColor.rgb;
	return outColor;
}
*/




float4 _Tone;


struct VSOutput
{
	float4 Pos		: SV_POSITION;
	float2 TexUV	: TEXCOORD0;
};

VSOutput VSMain(
	float3 pos		: POSITION,
	float2 texUV	: TEXCOORD0)
{
	VSOutput o;
	o.Pos	= float4(pos, 1.0f);
	o.TexUV	= texUV;
	return o;
}

float4 PSMain(
	float2 texUV	: TEXCOORD0) : COLOR
{
	return float4(texUV, 1, 1);
	//return float4(1, 0, 0, 0.5);
	float4 color = ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, texUV);
	return color;
	//return LN_CalculateToneColor(color, _Tone);
}

// TODO: 名前は Default とか、どんな SceneRenderer でも使えるようなのにしたい
technique Forward_Geometry_StaticMesh_UnLighting
{
	pass P0
	{
		VertexShader	= VSMain;
		PixelShader		= PSMain;
	}
}
#endif
