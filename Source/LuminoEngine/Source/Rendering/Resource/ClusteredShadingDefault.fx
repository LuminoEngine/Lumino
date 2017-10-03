
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
	float3	VertexNormal: NORMAL0;
	float3	WorldNormal	: NORMAL1;
	float4	Color		: COLOR0;
	float2	UV			: TEXCOORD0;
	float3	VertexPos	: TEXCOORD1;
	float3	WorldPos	: TEXCOORD2;
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
// Vertex Shader


//------------------------------------------------------------------------------

// User SurfaceShader (PixelShader) output
struct LNSurfaceOutput
{
	float4	Albedo;		// diffuse color
	float3	Normal;		// tangent space normal, if written
	float3	Emission;
	float	Specular;	// specular power in 0..1 range
	float3	Gloss;		// specular intensity
};

// user code
LNSurfaceOutput SS_Common(LNSSInput i)
{
	LNSurfaceOutput o = LNMakeSurfaceOutput(i);
	o.Albedo = float(1, 0, 0, 1);
	return o;
}

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

technique ClusteredForward_Default
{
	pass ShadowCaster
	{
		VertexShader = compile vs_3_0 LNProcessVertex();
		PixelShader	 = compile ps_3_0 PS_ClusteredForward_Default();
	}
	pass Geometry
	{
		VertexShader = compile vs_3_0 LNProcessVertex();
		PixelShader	 = compile ps_3_0 PSBasic();
	}
}

technique ClusteredForward_NoLighting
{
	pass ShadowCaster
	{
		VertexShader = compile vs_3_0 LNProcessVertex();
		PixelShader	 = compile ps_3_0 PSBasic();
	}
	pass Geometry
	{
		VertexShader = compile vs_3_0 LNProcessVertex();
		PixelShader	 = compile ps_3_0 PSBasic();
	}
}

END_HLSL

