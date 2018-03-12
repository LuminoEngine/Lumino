
#ifndef LUMINO_INCLUDED
#define LUMINO_INCLUDED

//------------------------------------------------------------------------------
// Lib

#define LN_EPSILON				1e-6
#define LN_MAX_GLOBAL_LIGHTS	4

float4x4	ln_View;
float4x4	ln_Projection;
float4x4	ln_World;
float4x4	ln_WorldViewProjection;
float4x4	ln_WorldView;
float4x4	ln_WorldViewIT;
float2		ln_ViewportPixelSize;
texture		ln_MaterialTexture;


struct LN_VSInput
{
	float3	Pos				: POSITION;
	float3	Normal			: NORMAL0;
	float4	Color			: COLOR0;
	float2	UV				: TEXCOORD0;
	float4	BlendWeight		: BLENDWEIGHT;
	float4	BlendIndices	: BLENDINDICES;
};

struct LN_VSOutput_Common
{
	float4	svPos		: POSITION;
	float3	Normal		: NORMAL0;
	float4	Color		: COLOR0;
	float2	UV			: TEXCOORD0;
};

struct LNVFInput
{
	// LNVSInput と同じ内容
	float3	Pos			: POSITION;
	float3	Normal		: NORMAL0;
	float4	Color		: COLOR0;
	float2	UV			: TEXCOORD0;
	
	// その他、Forward, Clusterd, Differd すべてで必要になるもの
	// (ターゲットごとに分けたりすると煩雑なのでまとめる。そんなに数は多くならないだろう)
	//float3	VertexPos	: TEXCOORD10;
	//float3	WorldPos	: TEXCOORD11;
};

struct LN_PSInput_Common
{
	// POSITION 以外は LNVSInput と同じ
	float3	Normal		: NORMAL0;
	float4	Color		: COLOR0;
	float2	UV			: TEXCOORD0;
};

struct LN_SurfaceOutput
{
	float4	Albedo;		// diffuse color
	float3	Normal;		// tangent space normal, if written
	float3	Emission;
	float	Specular;	// specular power in 0..1 range
	float3	Gloss;		// specular intensity
};

void _LN_InitSurfaceOutput(LN_PSInput_Common common, inout LN_SurfaceOutput surface)
{
	surface.Albedo = float4(0, 0, 0, 1);
	surface.Normal = common.Normal;
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
	o.UV			= input.UV + (float2(0.5, 0.5) / ln_ViewportPixelSize);
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


#endif // LUMINO_INCLUDED
