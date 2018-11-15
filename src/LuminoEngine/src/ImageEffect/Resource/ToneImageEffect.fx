
float4 LN_CalculateToneColor(float4 inColor, float4 inToneColor)
{
	float4 outColor = inColor;
	float y = (0.208012 * outColor.r + 0.586611 * outColor.g + 0.114478 * outColor.b) * inToneColor.w;
	outColor.rgb = (outColor.rgb * (1.0 - inToneColor.w)) + y + inToneColor.rgb;
	return outColor;
}





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
	return float4(1, 0, 0, 0.5);
	//float4 color = ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.UV);
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
