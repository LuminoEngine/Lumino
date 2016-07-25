
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


// ライト色
float3   LightDiffuses[MMM_LightCount]      : LIGHTDIFFUSECOLORS;
float3   LightAmbients[MMM_LightCount]      : LIGHTAMBIENTCOLORS;
float3   LightSpeculars[MMM_LightCount]     : LIGHTSPECULARCOLORS;

// ライト色
static float4 DiffuseColor[3] =
{
	MaterialDiffuse * float4(LightDiffuses[0], 1.0f),
	MaterialDiffuse * float4(LightDiffuses[1], 1.0f),
	MaterialDiffuse * float4(LightDiffuses[2], 1.0f),
};
static float3 AmbientColor[3] =
{
	saturate(MaterialAmbient * LightAmbients[0]) + MaterialEmmisive,
	saturate(MaterialAmbient * LightAmbients[1]) + MaterialEmmisive,
	saturate(MaterialAmbient * LightAmbients[2]) + MaterialEmmisive,
};
static float3 SpecularColor[3] =
{
	MaterialSpecular * LightSpeculars[0],
	MaterialSpecular * LightSpeculars[1],
	MaterialSpecular * LightSpeculars[2],
};

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
	
	// ディフューズ色＋アンビエント色 計算
	float3 color = float3(0, 0, 0);
	float3 ambient = float3(0, 0, 0);
	float count = 0;
	for (int i = 0; i < 3; i++)
	{
		if (LightEnables[i])
		{
			color += (float3(1,1,1) - color) * (max(0, DiffuseColor[i] * dot(output.Normal, -LightDirection[i])));
			//output.Color.rgb = float3(dot(output.Normal, -LightDirection[i]), 0, 0);
			ambient += AmbientColor[i];
			count = count + 1.0;
		}
	}
	output.Color.rgb = saturate(ambient / count + color);
	output.Color.a = MaterialDiffuse.a;
	
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
	color = input.Color;// * texColor;//MaterialDiffuse;//input.Color * texColor;
	
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

