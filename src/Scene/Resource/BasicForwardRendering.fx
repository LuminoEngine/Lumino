
static const int MMM_LightCount = 3;

struct LN_VS_INPUT
{
	float3	Pos		: POSITION;		// 位置
	float2	Tex		: TEXCOORD0;	// テクスチャ座標
	float3	Normal	: TEXCOORD1;	// 法線
	float4	Color	: COLOR0;		// 頂点色
};

//------------------------------------------------------------------------------

// 座標変換行列
float4x4	WorldViewProjMatrix	: WORLDVIEWPROJECTION;
float4x4	WorldMatrix			: WORLD;
float4x4	ViewMatrix			: VIEW;
float4x4	ProjMatrix			: PROJECTION;

//ライト関連
bool		LightEnables[MMM_LightCount]		: LIGHTENABLES;		// 有効フラグ
float4x4	LightWVPMatrices[MMM_LightCount]	: LIGHTWVPMATRICES;	// 座標変換行列
float3		LightDirection[MMM_LightCount]		: LIGHTDIRECTIONS;	// 方向
float3		LightPositions[MMM_LightCount]		: LIGHTPOSITIONS;	// ライト位置
float		LightZFars[MMM_LightCount]			: LIGHTZFARS;		// ライトzFar値

// マテリアル色
float4		MaterialDiffuse		: DIFFUSE  <string Object = "Geometry";>;
float3		MaterialAmbient		: AMBIENT  <string Object = "Geometry";>;
float3		MaterialEmmisive	: EMISSIVE <string Object = "Geometry";>;
float3		MaterialSpecular	: SPECULAR <string Object = "Geometry";>;
float		SpecularPower		: SPECULARPOWER < string Object = "Geometry"; >;

// オブジェクトのテクスチャ
texture		ObjectTexture		: MATERIALTEXTURE;
sampler		ObjTexSampler		= sampler_state
{
	texture = <ObjectTexture>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

struct VS_OUTPUT
{
	float4	Pos		: POSITION;		// 位置
	float2	Tex		: TEXCOORD0;	// テクスチャ座標
	float3	Normal	: TEXCOORD1;	// 法線
	float4	Color	: COLOR0;		// 色
};

//------------------------------------------------------------------------------
// 頂点シェーダ
VS_OUTPUT Basic_VS(LN_VS_INPUT input)
{
	VS_OUTPUT output;
	
	// 頂点位置をスクリーン空間に変換
	output.Pos = mul(float4(input.Pos, 1.0), WorldViewProjMatrix);
	
	// 頂点法線をワールド空間に変換
	output.Normal = normalize(mul(input.Normal, (float3x3)WorldMatrix));
	
	output.Color = input.Color;
	
	// テクスチャ座標
	output.Tex = input.Tex;
	
	return output;
}

//------------------------------------------------------------------------------
// ピクセルシェーダ
float4 Basic_PS(VS_OUTPUT input) : COLOR0
{
	float4 color;
	
	float4 texColor = tex2D(ObjTexSampler, input.Tex);
	color = float4(LightDirection[0], 1);//input.Color * texColor;//MaterialDiffuse;//input.Color * texColor;
	
	return color;
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

