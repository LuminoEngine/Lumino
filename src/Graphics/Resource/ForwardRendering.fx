
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
float4x4	ln_WorldViewProjection;
float4x4	ln_World;
float4x4	ln_View;
float4x4	ln_Projection;

//ライト関連
bool		ln_LightEnables[MMM_LightCount];		// 有効フラグ
float3		ln_LightDirections[MMM_LightCount];		// 方向
float3		ln_LightPositions[MMM_LightCount];		// ライト位置
float		ln_LightZFars[MMM_LightCount];			// ライトzFar値

// マテリアル色
float4		ln_MaterialDiffuse;
float3		ln_MaterialAmbient;
float3		ln_MaterialEmmisive;
float3		ln_MaterialSpecular;
float		ln_MaterialSpecularPower;

// ライト色
float3		ln_LightDiffuses[MMM_LightCount];
float3		ln_LightAmbients[MMM_LightCount];
float3		ln_LightSpeculars[MMM_LightCount];

// ライト色
static float4 DiffuseColor[3] =
{
	ln_MaterialDiffuse * float4(ln_LightDiffuses[0], 1.0f),
	ln_MaterialDiffuse * float4(ln_LightDiffuses[1], 1.0f),
	ln_MaterialDiffuse * float4(ln_LightDiffuses[2], 1.0f),
};
static float3 AmbientColor[3] =
{
	saturate(ln_MaterialAmbient * ln_LightAmbients[0]) + ln_MaterialEmmisive,
	saturate(ln_MaterialAmbient * ln_LightAmbients[1]) + ln_MaterialEmmisive,
	saturate(ln_MaterialAmbient * ln_LightAmbients[2]) + ln_MaterialEmmisive,
};
static float3 SpecularColor[3] =
{
	ln_MaterialSpecular * ln_LightSpeculars[0],
	ln_MaterialSpecular * ln_LightSpeculars[1],
	ln_MaterialSpecular * ln_LightSpeculars[2],
};

// オブジェクトのテクスチャ
texture		ln_MaterialTexture;
sampler		ObjTexSampler		= sampler_state
{
	texture = <ln_MaterialTexture>;
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
	output.Pos = mul(float4(input.Pos, 1.0), ln_WorldViewProjection);
	
	// 頂点法線をワールド空間に変換
	output.Normal = normalize(mul(input.Normal, (float3x3)ln_World));
	
	// ディフューズ色＋アンビエント色 計算
	float3 color = float3(0, 0, 0);
	float3 ambient = float3(0, 0, 0);
	float count = 0;
	for (int i = 0; i < 3; i++)
	{
		if (ln_LightEnables[i])
		{
			color += (float3(1,1,1) - color) * (max(0, DiffuseColor[i] * dot(output.Normal, -ln_LightDirections[i])));
			//output.Color.rgb = float3(dot(output.Normal, -LightDirection[i]), 0, 0);zzzzzz
			ambient += AmbientColor[i];
			count = count + 1.0;
		}
	}
	output.Color.rgb = saturate(ambient / count + color);
	output.Color.a = ln_MaterialDiffuse.a;
	
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
	color = input.Color * texColor;//MaterialDiffuse;//input.Color * texColor;
	
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

