

#include <Lumino.fxh>

//sampler2D tDiffuse;
float3 defaultColor = float3(0.0, 0.0, 0.0);
float defaultOpacity = 0.0;
float luminosityThreshold = 1.0;
float smoothWidth = 1.0;

LN_VSOutput_Common VS_Main(LN_VSInput input)
{
	LN_VSOutput_Common output = LN_ProcessVertex_Common(input);
	output.svPos = float4(input.Pos, 1.0f);
	return output;
}

float4 PS_Main(LN_PSInput_Common input) : SV_TARGET
{
	//float4 texel = tex2D( tDiffuse, input.UV );
	float4 texel = tex2D( ln_MaterialTexture, input.UV );
	//texel.r = 1;
	//return float4(texel);

	float3 luma = float3( 0.299, 0.587, 0.114 );
	//return float4(texel.xyz + float3( 0.299, 0.587, 0.114 ), 1 );

	float v = saturate(dot( texel.xyz, luma ));
	//return float4( v,0,0, 1 );

	float4 outputColor = float4( defaultColor.rgb, defaultOpacity );
	float alpha = smoothstep( luminosityThreshold, luminosityThreshold + smoothWidth, v );
	return lerp( outputColor, texel, alpha );
}

technique Forward_Geometry_UnLighting
{
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}


