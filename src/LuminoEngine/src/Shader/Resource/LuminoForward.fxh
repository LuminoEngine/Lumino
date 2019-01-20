
#ifndef LUMINO_FORWARD_INCLUDED
#define LUMINO_FORWARD_INCLUDED

Texture2D ln_GlobalLightInfoTexture;
SamplerState ln_GlobalLightInfoTextureSamplerState;

Texture2D ln_pointLightInfoTexture;
SamplerState ln_pointLightInfoTextureSamplerState;

static float2 LightInfoTextureSize = float2(4, 64);

//------------------------------------------------------------------------------
// 

float4x4        ln_ViewProjection_Light0;

struct LN_VSOutput_ClusteredForward
{
	float3	WorldPos	: TEXCOORD10;
	float3	VertexPos	: TEXCOORD11;
	
	float3	vViewPosition 	: TEXCOORD12;	// 頂点位置から視点位置までのベクトル
	float4	vInLightPosition 	: TEXCOORD13;
};

struct LN_PSInput_ClusteredForward
{
	float3	WorldPos	: TEXCOORD10;
	float3	VertexPos	: TEXCOORD11;
	
	float3	vViewPosition 	: TEXCOORD12;
	float4	vInLightPosition 	: TEXCOORD13;
};

LN_VSOutput_ClusteredForward LN_ProcessVertex_ClusteredForward(LN_VSInput input)
{
	LN_VSOutput_ClusteredForward output;
	output.WorldPos = mul(float4(input.Pos, 1.0), ln_World).xyz;
	output.VertexPos = input.Pos;
	
	
	float4 mvPosition = mul(float4(input.Pos, 1.0), ln_WorldView);
	output.vViewPosition  = -mvPosition.xyz;
	
	
	
	float4 pos = mul(float4(input.Pos, 1.0), ln_World);
	pos = mul(pos, ln_ViewProjection_Light0);
	output.vInLightPosition = pos;
	//output.vInLightPosition = mul(mul(float4(input.Pos, 1.0), ln_World), ln_ViewProjection_Light0);
	
	return output;
}




struct GlobalLightInfo
{
	float3	color;	// intensity 適用済み
	float3	groundColor;	// intensity 適用済み
	float4	directionAndType;
};

struct LightInfo
{
	float3	position;
	float	range;
	float	attenuation;
	float3	color;	// intensity 適用済み
	float3	direction;
	float2	spotAngles;
};



GlobalLightInfo _LN_GetGlobalLightInfo(int index)
{
	float2 s = 1.0 / LightInfoTextureSize;
	float y = (float)index;
	y += 0.5;
	GlobalLightInfo info;
	//info.color = tex2D(ln_GlobalLightInfoSampler, (float2(0.0, y) * s));
	//info.groundColor = tex2D(ln_GlobalLightInfoSampler, (float2(1.5, y) * s));
	//info.directionAndType = tex2D(ln_GlobalLightInfoSampler, (float2(2.5, y) * s));
	info.color = ln_GlobalLightInfoTexture.Sample(ln_GlobalLightInfoTextureSamplerState, (float2(0.0, y) * s));
	info.groundColor = ln_GlobalLightInfoTexture.Sample(ln_GlobalLightInfoTextureSamplerState, (float2(1.5, y) * s));
	info.directionAndType = ln_GlobalLightInfoTexture.Sample(ln_GlobalLightInfoTextureSamplerState, (float2(2.5, y) * s));
	return info;
}

LightInfo _LN_GetLightInfoClusterd(int index)
{
	float2 s = 1.0 / LightInfoTextureSize;
	float y = (float)index;
	y += 0.5;
	//float4 tc0 = float4((0.0 + 0.5f) * s.x, (index + 0.5f) * s.y, 0, 1);	// +0.5 は半ピクセル分
	//float4 tc1 = float4((1.0 + 0.5f) * s.x, (index + 0.5f) * s.y, 0, 1);	// +0.5 は半ピクセル分
	//tc0.y = 1.0 - tc0.y;
	//tc1.y = 1.0 - tc1.y;
	
	//float4 posAndRange = tex2D(pointLightInfoSampler, tc0.xy);
	//float4 spotDirection = tex2D(pointLightInfoSampler, tc1.xy);

	//float4 posAndRange = tex2D(pointLightInfoSampler, (float2(0.0, y) * s));
	//float4 spotDirection = tex2D(pointLightInfoSampler, (float2(1.0, y) * s));
	//float4 spotAngle = tex2D(pointLightInfoSampler, (float2(2.0, y) * s));
	//float4 color = tex2D(pointLightInfoSampler, (float2(3.0, y) * s));
	float4 posAndRange = ln_pointLightInfoTexture.Sample(ln_pointLightInfoTextureSamplerState, (float2(0.0, y) * s));
	float4 spotDirection = ln_pointLightInfoTexture.Sample(ln_pointLightInfoTextureSamplerState, (float2(1.0, y) * s));
	float4 spotAngle = ln_pointLightInfoTexture.Sample(ln_pointLightInfoTextureSamplerState, (float2(2.0, y) * s));
	float4 color = ln_pointLightInfoTexture.Sample(ln_pointLightInfoTextureSamplerState, (float2(3.0, y) * s));
	
	LightInfo light;
	light.position = posAndRange.xyz;
	light.range = posAndRange.w;
	light.attenuation = spotDirection.w;
	light.color = color;
	light.direction = spotDirection.xyz;
	light.spotAngles = spotAngle.xy;
	
	/*
	float2 uv = 1.0 / LightInfoTextureSize;
	float4 tc0 = float4((0.0 + 0.5f) * uv.x, (index + 0.5f) * uv.y, 0, 1);	// +0.5 は半ピクセル分
	float4 tc1 = float4((1.0 + 0.5f) * uv.x, (index + 0.5f) * uv.y, 0, 1);	// +0.5 は半ピクセル分
	LightInfo light;
	
	tc0.y = 1.0 - tc0.y;
	tc1.y = 1.0 - tc1.y;
	//o.pos = tex2Dlod(m_pointLightInfoSampler, tc0);
	//o.color = tex2Dlod(m_pointLightInfoSampler, tc1);
	light.position = tex2D(pointLightInfoSampler, tc0.xy);
	light.color = tex2D(pointLightInfoSampler, tc1.xy);
	
	light.range = 1;
	light.direction = float3(1, 0, 0);
	light.spotAngles = float2(0, 0);
	*/
	return light;
}

#endif // LUMINO_FORWARD_INCLUDED
