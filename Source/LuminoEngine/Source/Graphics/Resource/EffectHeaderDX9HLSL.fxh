//==============================================================================
// Lumino (HLSL DX9)
//==============================================================================

static const int LN_LightCount = 3;

// 座標変換行列
float4x4	ln_WorldViewProjection;
float4x4	ln_World;
float4x4	ln_View;
float4x4	ln_Projection;
float2		ln_ViewportPixelSize;

//ライト関連
bool		ln_LightEnables[LN_LightCount];		// 有効フラグ
float3		ln_LightDirections[LN_LightCount];		// 方向
float3		ln_LightPositions[LN_LightCount];		// ライト位置
float		ln_LightZFars[LN_LightCount];			// ライトzFar値

// マテリアル色
float4		ln_MaterialDiffuse;
float3		ln_MaterialAmbient;
float3		ln_MaterialEmmisive;
float3		ln_MaterialSpecular;
float		ln_MaterialSpecularPower;
texture		ln_MaterialTexture;

// ライト色
float3		ln_LightDiffuses[LN_LightCount];
float3		ln_LightAmbients[LN_LightCount];
float3		ln_LightSpeculars[LN_LightCount];

// ビルトインのエフェクト色
float4		ln_ColorScale = float4(1, 1, 1, 1);
float4		ln_BlendColor = float4(0, 0, 0, 1);
float4		ln_ToneColor = float4(0, 0, 0, 0);

// 各種ベース色
static float4 ln_DiffuseColor[LN_LightCount] =
{
	ln_MaterialDiffuse * float4(ln_LightDiffuses[0], 1.0f),
	ln_MaterialDiffuse * float4(ln_LightDiffuses[1], 1.0f),
	ln_MaterialDiffuse * float4(ln_LightDiffuses[2], 1.0f),
};
static float3 ln_AmbientColor[LN_LightCount] =
{
	saturate(ln_MaterialAmbient * ln_LightAmbients[0]) + ln_MaterialEmmisive,
	saturate(ln_MaterialAmbient * ln_LightAmbients[1]) + ln_MaterialEmmisive,
	saturate(ln_MaterialAmbient * ln_LightAmbients[2]) + ln_MaterialEmmisive,
};
static float3 ln_SpecularColor[LN_LightCount] =
{
	ln_MaterialSpecular * ln_LightSpeculars[0],
	ln_MaterialSpecular * ln_LightSpeculars[1],
	ln_MaterialSpecular * ln_LightSpeculars[2],
};

// Internal VS input data (WrapperFunc)
struct LN_VS_INPUT_SURFACE
{
	float3	Pos				: POSITION0;
	float4	BlendWeight		: BLENDWEIGHT;
	float4	BlendIndices	: BLENDINDICES;
	float3	Normal			: NORMAL0;
	float2	TexCoord		: TEXCOORD0;
	float2	TexCoord1		: TEXCOORD1;
	float2	TexCoord2		: TEXCOORD2;
	float2	TexCoord3		: TEXCOORD3;
	float4	Color			: COLOR0;
	float4	Tangent			: TANGENT0;
};

// User VS output data (UserFunc to WrapperFunc)
struct LN_VS_OUTPUT_SURFACE
{
	float4	Pos				: POSITION0;
	float2	TexCoord		: TEXCOORD0;
	float2	TexCoord1		: TEXCOORD1;
	float2	TexCoord2		: TEXCOORD2;
	float2	TexCoord3		: TEXCOORD3;
	float4	Color			: COLOR0;
};

// User SurfaceShader (PixelShader) output
struct LN_PS_INPUT_SURFACE
{
	float2	TexCoord		: TEXCOORD0;
	float2	TexCoord1		: TEXCOORD1;
	float2	TexCoord2		: TEXCOORD2;
	float2	TexCoord3		: TEXCOORD3;
	float4	Color			: COLOR0;
};

// User SurfaceShader (PixelShader) output
struct LN_PS_OUTPUT_SURFACE
{
	float4	Color			: COLOR0;
};



// User SurfaceShader (PixelShader) output
struct LN_SURFACE_INPUT
{
	float2	TexCoord		: TEXCOORD0;
	float2	TexCoord1		: TEXCOORD1;
	float2	TexCoord2		: TEXCOORD2;
	float2	TexCoord3		: TEXCOORD3;
	float4	Color			: COLOR0;
};

// User SurfaceShader (PixelShader) output
struct LN_SURFACE_OUTPUT
{
	float4	Albedo;  // diffuse color
	//float3	Normal;  // tangent space normal, if written
	//float3	Emission;
	//float	Specular;  // specular power in 0..1 range
	//float3	Gloss;    // specular intensity
};



#ifdef LN_HLSL_DX9
static float2 ln_DX9ViewportOffset = (float2(0.5, 0.5) / ln_ViewportPixelSize);
#endif

//------------------------------------------------------------------------------
LN_VS_OUTPUT_SURFACE LN_SurfaceVS(LN_VS_INPUT_SURFACE i)
{
	LN_VS_OUTPUT_SURFACE o;
	o.Pos = mul(float4(i.Pos, 1.0), ln_WorldViewProjection);
#ifdef LN_HLSL_DX9
	o.Pos.x -= ln_DX9ViewportOffset.x;
	o.Pos.y += ln_DX9ViewportOffset.y;
#endif
	o.TexCoord  = i.TexCoord;
	o.TexCoord1 = i.TexCoord1;
	o.TexCoord2 = i.TexCoord2;
	o.TexCoord3 = i.TexCoord3;
	o.Color = i.Color;
	return o;
}


//------------------------------------------------------------------------------

#define LN_SURFACE_VS(func)		compile vs_3_0 LN_SurfaceVS()
#define LN_SURFACE_PS(func)		compile ps_3_0 LN_SurfacePS_##func


//------------------------------------------------------------------------------
float4 LN_GetLambertVertexColor(float3 normal)
{
	// ディフューズ色＋アンビエント色 計算
	float3 color = float3(0, 0, 0);
	float3 ambient = float3(0, 0, 0);
	float count = 0;
	for (int i = 0; i < 3; i++)
	{
		if (ln_LightEnables[i])
		{
			color += (float3(1,1,1) - color) * (max(0, ln_DiffuseColor[i].rgb * dot(normal, -ln_LightDirections[i])));
			//output.Color.rgb = float3(dot(output.Normal, -LightDirection[i]), 0, 0);
			ambient += ln_AmbientColor[i];
			count = count + 1.0;
		}
	}
	
	float4 outColor;
	outColor.rgb = saturate(ambient / count + color);
	outColor.a = ln_MaterialDiffuse.a;
	return outColor;
}

//------------------------------------------------------------------------------
float4 LN_CalculateToneColor(float4 inColor, float4 inToneColor)
{
	float4 outColor = inColor;
	float y = (0.208012 * outColor.r + 0.586611 * outColor.g + 0.114478 * outColor.b) * inToneColor.w;
	outColor.rgb = (outColor.rgb * (1.0 - inToneColor.w)) + y + inToneColor.rgb;
	return outColor;
}

//------------------------------------------------------------------------------
float4 LN_GetBuiltinEffectColor(float4 inColor)
{
	// apply color scale.
	float4 outColor = inColor * ln_ColorScale;
	
	// apply blend color.
	outColor.rgb = lerp(outColor.rgb, ln_BlendColor.rgb, ln_BlendColor.a);

	// apply tone. (NTSC Coef method)
	outColor = LN_CalculateToneColor(outColor, ln_ToneColor);
	return outColor;
}



#line 5
