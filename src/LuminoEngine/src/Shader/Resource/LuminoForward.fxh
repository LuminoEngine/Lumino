
#ifndef LUMINO_FORWARD_INCLUDED
#define LUMINO_FORWARD_INCLUDED

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


#endif // LUMINO_FORWARD_INCLUDED
