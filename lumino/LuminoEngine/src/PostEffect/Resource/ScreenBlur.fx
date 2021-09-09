
#include <Lumino.fxh>

float4			_BlurColor;		// ブラーの色・強さ
float4x4		_BlurMatrix;	// ブラーイメージの座標変換行列

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
	output.svPos = mul(float4(input.Pos, 1.0f), _BlurMatrix);
	return output;
}

float4 PS_Main(LN_PSInput_Common input) : SV_TARGET
{
    return tex2D(ln_MaterialTexture, input.UV) * _BlurColor;
    //ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.UV) * _BlurColor;
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


