
#ifndef LUMINO_INCLUDED
#define LUMINO_INCLUDED

//------------------------------------------------------------------------------
// Lib

#define LN_EPSILON				1e-6
#define LN_MAX_GLOBAL_LIGHTS	4

cbuffer LNRenderViewBuffer
{
	/* [0]   */ float4x4 ln_View;
	/* [64]  */ float4x4 ln_Projection;
	/* [128] */ float4x4 ln_ProjectionI;
	/* [192] */ float4 ln_Resolution;
	/* [208] */ float3 ln_CameraPosition;
	/* [224] */ float3 ln_CameraDirection;
	/* [236] */ float ln_NearClip;
	/* [240] */ float ln_FarClip;
};  /* [244(alignd:16)] */

cbuffer LNRenderElementBuffer
{
	/* [0]   */ float4x4 ln_World;
	/* [64]  */ float4x4 ln_WorldI;
	/* [128] */ float4x4 ln_WorldViewProjection;
	/* [192] */ float4x4 ln_WorldView;
	/* [256] */ float4x4 ln_WorldViewIT;
	/* [320] */ float4 ln_BoneTextureReciprocalSize;
	/* [336] */	int ln_objectId;
};  /* [352(alignd:16)] */

// Builtin effect colors
cbuffer LNEffectColorBuffer
{
	/* [0]  */ float4 ln_ColorScale;
	/* [16] */ float4 ln_BlendColor;
	/* [32] */ float4 ln_ToneColor;
};

sampler2D ln_MaterialTexture;
sampler2D ln_MaterialRoughnessMap;

struct LN_VSInput
{
	float3	Pos				: POSITION;
	float3	Normal			: NORMAL0;
	float2	UV				: TEXCOORD0;
	float4	Color			: COLOR0;
	float4  tangent: TANGENT;
	float4	BlendIndices	: BLENDINDICES;
	float4	BlendWeight		: BLENDWEIGHT;
};

struct LN_VSOutput_Common
{
	float4	svPos		: SV_POSITION;

	/** 座標変換後の法線 [View-space] 左手座標系である点に注意。視点に対して、右は X+, 上はY+, 奥はZ+ */
	float3	Normal		: NORMAL0;

	float2	UV			: TEXCOORD0;

	float4	Color		: COLOR0;
};

struct LNVFInput
{
	// LNVSInput と同じ内容
	float3	Pos			: POSITION;
	float3	Normal		: NORMAL0;
	float2	UV			: TEXCOORD0;
	float4	Color		: COLOR0;
	
	// その他、Forward, Clusterd, Differd すべてで必要になるもの
	// (ターゲットごとに分けたりすると煩雑なのでまとめる。そんなに数は多くならないだろう)
	//float3	VertexPos	: TEXCOORD10;
	//float3	WorldPos	: TEXCOORD11;
};

struct LN_PSInput_Common
{
	// POSITION 以外は LNVSInput と同じ
	float3	Normal		: NORMAL0;
	float2	UV			: TEXCOORD0;
	float4	Color		: COLOR0;
};

struct LN_SurfaceOutput
{
	float4	Albedo;		// diffuse color
	float3	Normal;		// tangent space normal, if written
	float3	Emission;
	float	Specular;	// specular power in 0..1 range
	float3	Gloss;		// specular intensity
};

void _LN_InitSurfaceOutput(float3 normal, inout LN_SurfaceOutput surface)
{
	surface.Albedo = float4(0, 0, 0, 1);
	surface.Normal = normal;
	surface.Emission = float3(0, 0, 0);
	surface.Specular = 0;
	surface.Gloss = float3(0, 0, 0);
}

LN_VSOutput_Common LN_ProcessVertex_Common(LN_VSInput input)
{
	//float4x4 normalMatrix  = transpose(/*inverse*/(ln_WorldView));
	
	LN_VSOutput_Common o;
	o.svPos			= mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
	o.Normal		= mul(float4(input.Normal, 1.0f), ln_WorldViewIT).xyz;
	o.UV			= input.UV;// + (float2(0.5, 0.5) / ln_Resolution.xy);
	o.Color			= input.Color;
	return o;
}

float LN_Square(float x)
{
	return x * x;
}

float2 LN_Square(float2 x)
{
	return x * x;
}

float3 LN_Square(float3 x)
{
	return x * x;
}

float4 LN_Square(float4 x)
{
	return x * x;
}

float4 LN_CalculateToneColor(float4 inColor, float4 inToneColor)
{
	float4 outColor = inColor;
	float y = (0.208012 * outColor.r + 0.586611 * outColor.g + 0.114478 * outColor.b) * inToneColor.w;
	outColor.rgb = (outColor.rgb * (1.0 - inToneColor.w)) + y + inToneColor.rgb;
	return outColor;
}

float4 LN_GetBuiltinEffectColor(float4 inColor)
{
	// apply color scale.
	float4 outColor = inColor * ln_ColorScale;
	
	// apply blend color.
	outColor.rgb = lerp(outColor.rgb, ln_BlendColor.rgb, ln_BlendColor.a);

	// apply tone. (NTSC Coef method)
	return LN_CalculateToneColor(outColor, ln_ToneColor);
}

/**
 * 整数型の RGB テクスチャなどに書き込まれている 0.0 ~ 1.0 にパックされた法線を -1.0 ~ 1.0 に展開します。
 */
float3 LN_UnpackNormal(float3 packednormal)
{
	return (packednormal * 2.0) - 1.0;
}

/**
 * Clip-space 上の xy 座標 (-1.0 ~ 1.0) を、UV 座標 (0.0 ~ 1.0) へ変換します。
 */
float2 LN_ClipSpacePositionToUV(float2 pos)
{
    return (float2(1.0, -1.0) * pos) * 0.5 + 0.5;
}

/**
 * UV 座標 (0.0 ~ 1.0) を、Clip-space 上の xy 座標 (-1.0 ~ 1.0) へ変換します。
 */
float2 LN_UVToClipSpacePosition(float2 uv)
{
    return ((uv - 0.5) * 2.0) * float2(1.0, -1.0);
}

#endif // LUMINO_INCLUDED
