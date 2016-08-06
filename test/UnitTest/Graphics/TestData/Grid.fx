
static const int MMM_LightCount = 3;

struct LN_VS_INPUT
{
	float3	Pos		: POSITION;		// 位置
	float2	Tex		: TEXCOORD0;	// テクスチャ座標
	float3	Normal	: NORMAL0;//TEXCOORD1;	// 法線
	float4	Color	: COLOR0;		// 頂点色
};

//------------------------------------------------------------------------------

// 座標変換行列
float4x4	WorldViewProjMatrix;

// オブジェクトのテクスチャ
texture		ObjectTexture;
sampler		ObjTexSampler = sampler_state
{
	texture = <ObjectTexture>;	// サイズは、速度と品質のバランス的に 512px がちょうどいい。
	MINFILTER = ANISOTROPIC;	// NONE,POINT だと、MipMap の境界にシャギのようなものが見える。LINEAR だと近景でもボケすぎる。
	MAGFILTER = LINEAR;			// ANISOTROPIC でも変わらない。LINEAR で良いのでは。
	MIPFILTER = LINEAR;			// ANISOTROPIC でも変わらない。LINEAR で良いのでは。
	MAXANISOTROPY = 16;
};

struct VS_OUTPUT
{
	float4	Pos		: POSITION;		// 位置
	float2	Tex		: TEXCOORD0;	// テクスチャ座標
};

//------------------------------------------------------------------------------
// 頂点シェーダ
VS_OUTPUT Basic_VS(LN_VS_INPUT input)
{
	VS_OUTPUT output;
	
	// 頂点位置をスクリーン空間に変換
	output.Pos = mul(float4(input.Pos, 1.0), WorldViewProjMatrix);
	
	// テクスチャ座標
	output.Tex.x = input.Pos.x;
	output.Tex.y = input.Pos.z;
	
	return output;
}

//------------------------------------------------------------------------------
// ピクセルシェーダ
float4 Basic_PS(VS_OUTPUT input) : COLOR0
{
	float4 texColor = tex2D(ObjTexSampler, input.Tex);
	float4 texColor2 = tex2D(ObjTexSampler, input.Tex / 10.0);
	return texColor + texColor2;
}

//------------------------------------------------------------------------------
// 描画テクニック
technique MainTec0
{
	pass P0
	{
		VertexShader = compile vs_3_0 Basic_VS();
		PixelShader	 = compile ps_3_0 Basic_PS();
	}
}

