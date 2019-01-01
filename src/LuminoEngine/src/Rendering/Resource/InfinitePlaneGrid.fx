
float4x4 ln_WorldViewProjection;
Texture2D ln_MaterialTexture;
SamplerState ln_MaterialTextureSamplerState;

struct VS_OUTPUT
{
	float4	Pos		: SV_POSITION;		// 位置
	float2	Tex		: TEXCOORD0;	// テクスチャ座標
};

VS_OUTPUT Basic_VS(float3 Pos : POSITION)
{
	VS_OUTPUT output;
	
	// 頂点位置をスクリーン空間に変換
	output.Pos = mul(float4(Pos, 1.0), ln_WorldViewProjection);
	
	// テクスチャ座標
	output.Tex.x = Pos.x;
	output.Tex.y = Pos.z;
	
	return output;
}

float4 Basic_PS(VS_OUTPUT input) : COLOR0
{
	//return float4(1, 0, 0, 1);
	float4 texColor = ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.Tex);
	float4 texColor2 = ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.Tex / 10.0);
	return texColor + texColor2;
}

technique Forward_Geometry
{
	pass P0
	{
		VertexShader = Basic_VS;
		PixelShader	 = Basic_PS;
	}
}
