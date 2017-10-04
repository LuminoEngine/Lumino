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

struct LNSurfaceOutput
{
	float4	Albedo;		// diffuse color
	float3	Normal;		// tangent space normal, if written
	float3	Emission;
	float	Specular;	// specular power in 0..1 range
	float3	Gloss;		// specular intensity
};

LNVSOutput LNProcessVertex(LNVSInput vsi)
{
	LNVSOutput o;
	o.svPos			= mul(float4(v.Pos, 1.0f), ln_WorldViewProjection);
	o.VertexNormal	= vsi.Normal;
	o.WorldNormal	= mul(float4(vsi.Normal, 1.0f), ln_World).xyz;
	o.UV			= v.UV;// + ViewportOffset;
	o.Color			= v.Color;
	o.VertexPos		= v.Pos.xyz;
	o.WorldPos		= mul(float4(v.Pos, 1.0f), ln_World).xyz;
	return o;
}

//------------------------------------------------------------------------------

struct ClusteredForwardVSOutput
{
	float3	VertexPos	: TEXCOORD10;
	float3	WorldPos	: TEXCOORD11;
};



static float2	ViewportOffset = (float2(0.5, 0.5) / ln_ViewportPixelSize);
static float2	ViewportOffset2 = (float2(1.0, 1.0) / ln_ViewportPixelSize);

sampler		MaterialTextureSampler = sampler_state
{
	texture = <ln_MaterialTexture>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};


struct PSInput
{
	float4	Color	: COLOR0;
	float2	UV		: TEXCOORD0;
	float3	ViewportPos		: TEXCOORD1;
	float	ViewportPos_z		: TEXCOORD2;
	
	float3	Pos2	: TEXCOORD3;
};



//------------------------------------------------------------------------------
// Vertex Shader (auto generation)

struct LNVFInput
{
	// LNVSInput と同じ内容
	float3	Pos			: POSITION;
	float3	Normal		: NORMAL0;
	float4	Color		: COLOR0;
	float2	UV			: TEXCOORD0;
	
	// Scene 固有の処理済み頂点情報
	/**** Scene Template ****/
	ClusteredForwardVSOutput	SV;
	/**** Scene Template ****/
};

//------------------------------------------------------------------------------
// Vertex Factory (user code)

struct MyVFOutput
{
	float4	WorldPos	: TEXCOORD5;
};

void MyVFMain(LNVFInput input, inout MyVFOutput output)
{
	output.WorldPos = mul(float4(input.Pos, 1.0), ln_World);
}

//------------------------------------------------------------------------------
// Vertex Shader (auto generation)

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
	o.svsout = ProcessSceneVS(vsi);
	o.uservsout = MyVFMain(vsi);
}


//------------------------------------------------------------------------------
// Surface Shader (user code)

struct MySSInput
{
	float4	WorldPos	: TEXCOORD5;
};

// user code
void MySSMain(MySSInput input, inout LNSurfaceOutput output)
{
	output.Albedo = float4(WorldPos.xy, 0, 1);
}


//------------------------------------------------------------------------------
// Pixel Shader

// auto generation
float4 PS_ClusteredForward_Default(PSInput psi) : COLOR0
{
	LNSSInput ssi = LNMakeSurfaceInput(psi);
	LNSurfaceOutput so = SS_Common(ssi);
	// TODO: Lighting
	return so.Albedo;
}

// auto generation
float4 PS_ClusteredForward_NoLighting(PSInput p) : COLOR0
{
	LNSSInput ssi = LNMakeSurfaceInput(psi);
	LNSurfaceOutput so = SS_Common(ssi);
	return so.Albedo;
}

//------------------------------------------------------------------------------

technique ClusteredForward
{
	pass ShadowCaster_Default
	{
		VertexShader = compile vs_3_0 LNProcessVertex();
		PixelShader	 = compile ps_3_0 PS_ClusteredForward_Default();
	}
	pass ShadowCaster_NoLighting
	{
		VertexShader = compile vs_3_0 LNProcessVertex();
		PixelShader	 = compile ps_3_0 PSBasic();
	}
	pass Geometry_Default
	{
		VertexShader = compile vs_3_0 LNProcessVertex();
		PixelShader	 = compile ps_3_0 PSBasic();
	}
	pass Geometry_NoLighting
	{
		VertexShader = compile vs_3_0 LNProcessVertex();
		PixelShader	 = compile ps_3_0 PSBasic();
	}
}

END_HLSL

