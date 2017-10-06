/* Lumino ShadingModel combined HLSL file */

BEGIN_HLSL

//------------------------------------------------------------------------------
// Lib

struct LNVSInput
{
	float3	Pos			: POSITION;
	float3	Normal		: NORMAL0;
	float4	Color		: COLOR0;
	float2	UV			: TEXCOORD0;
};

struct LNVSOutput
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
	float3	VertexPos	: TEXCOORD10;
	float3	WorldPos	: TEXCOORD11;
};

struct LNSurfaceOutput
{
	float4	Albedo;		// diffuse color
	float3	Normal;		// tangent space normal, if written
	float3	Emission;
	float	Specular;	// specular power in 0..1 range
	float3	Gloss;		// specular intensity
};


LNVSOutput _LN_ProcessStandardVertex(LNVSInput vsi)
{
	LNVSOutput o;
	o.svPos			= mul(float4(v.Pos, 1.0f), ln_WorldViewProjection);
	o.WorldNormal	= mul(float4(vsi.Normal, 1.0f), ln_World).xyz;
	o.UV			= v.UV + (float2(0.5, 0.5) / ln_ViewportPixelSize);;
	o.Color			= v.Color;
	return o;
}
LNVSOutput _LN_ProcessVertex(LNVSInput vsi)
{
	LNVSOutput o;
	o.svPos			= mul(float4(v.Pos, 1.0f), ln_WorldViewProjection);
	o.WorldNormal	= mul(float4(vsi.Normal, 1.0f), ln_World).xyz;
	o.UV			= v.UV + (float2(0.5, 0.5) / ln_ViewportPixelSize);;
	o.Color			= v.Color;
	return o;
}

//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Vertex Factory (user code)

struct MyVFOutput
{
	float4	WorldPos	: TEXCOORD5;
};

void MyVFMain(LNVFInput input, inout MyVFOutput output)	// ★ out 引数の型をパース
{
	output.WorldPos = mul(float4(input.Pos, 1.0), ln_World);
}

// Surface Shader (user code)

sampler		MaterialTextureSampler = sampler_state
{
	texture = <ln_MaterialTexture>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

struct MySSInput
{
	float4	Color	: COLOR0;
	float2	UV		: TEXCOORD0;
	float4	WorldPos	: TEXCOORD5;
};

// user code
void MySSMain(MySSInput input, inout LNSurfaceOutput output)
{
	output.Albedo = float4(WorldPos.xy, 0, 1);
}

//------------------------------------------------------------------------------
// ★ Vertex Shader (auto generation)

struct VSOutput
{
	LNVSOutput		lnvsout;
	SceneVSOutput	svsout;
	MyVFOutput		uservsout;	// MyVFMain() の最後の引数をパースして得る
}

// auto generation
LNVSOutput VS_ClusteredForward_Default(LNVSInput vsi)
{
	VSOutput o;
	o.lnvsout = LNProcessVertex(vsi);
	o.svsout = ProcessSceneVS(vsi);	// ★ Scene固有
	o.uservsout = MyVFMain(vsi);
}

//------------------------------------------------------------------------------
// ★ Pixel Shader

// auto generation
float4 _PS_ClusteredForward(PSInput psi) : COLOR0
{
	LNSSInput ssi = LNMakeSurfaceInput(psi);
	LNSurfaceOutput so = SS_Common(ssi);
	// TODO: Lighting
	return so.Albedo;
}

//------------------------------------------------------------------------------
/*
technique ClusteredForward
{
	pass ShadowCaster
	{
		VertexFactory	= MyVFMain;
		SurfaceShader	= MySSMain;
	}
	pass Geometry
	{
		VertexFactory	= MyVFMain;
		SurfaceShader	= MySSMain;
	}
}
*/

technique ClusteredForward
{
	pass ShadowCaster
	{
		VertexShader = compile vs_3_0 _VS_ClusteredForward_ShadowCaster();
		PixelShader	 = compile ps_3_0 _PS_ClusteredForward_ShadowCaster();
	}
	pass Geometry
	{
		VertexShader = compile vs_3_0 _VS_ClusteredForward_Geometry();
		PixelShader	 = compile ps_3_0 _PS_ClusteredForward_Geometry();
	}
}

END_HLSL

