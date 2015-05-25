
float4x4	g_worldMatrix;
float4x4	g_viewProjMatrix;


float2 g_viewportSize;
static float2 g_pixelStep = (float2(2.0, 2.0) / g_viewportSize);

texture		g_texture;
sampler2D	g_texSampler = sampler_state
{
	texture = <g_texture>;
	MINFILTER = NONE;
	MAGFILTER = NONE;
};

struct VS_OUTPUT
{
	float4 Pos			: POSITION;
	float4 Color		: COLOR0;
	float4 UVOffset		: TEXCOORD0;
	float2 UVTileUnit	: TEXCOORD1;
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(
	float3 inPos		: POSITION,
	float4 inColor		: COLOR0,
	float4 inUVOffset	: TEXCOORD0,
	float2 inUVTileUnit	: TEXCOORD1)
{
	inPos.xy -= 0.5;
	VS_OUTPUT o;
	o.Pos			= mul(float4(inPos, 1.0f), g_worldMatrix);
	o.Pos			= mul(o.Pos, g_viewProjMatrix);
	//o.Pos.xy -= g_pixelStep;
	o.Color			= inColor;
	o.UVOffset		= inUVOffset;
	o.UVTileUnit	= inUVTileUnit;
	return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(
	float4 inColor			: COLOR0,
	float4 inUVOffset		: TEXCOORD0,
	float2 inUVTileUnit		: TEXCOORD1) : COLOR0
{
	// ì]ëóå≥ç∂è„ UV
	float2 uvUpperLeft = inUVOffset.xy;//float2(inUVOffset.x, inUVOffset.y);
	
	// ì]ëóå≥ãÈå`ÇÃïù UV
	float2 uvWidth = inUVOffset.zw ;//uvUpperLeft - float2(inUVOffset.zw / inUVTileUnit);
	
	// 1Ç¬ÇÃéläpå`ÇÃíÜÇÃÇ«Ç±Ç…Ç¢ÇÈÇÃÇ© (0.0Å`1.0)
	float2 uvRatio = fmod(inUVTileUnit, 1.0);
	
	float2 uv = lerp(uvUpperLeft, uvUpperLeft + uvWidth, uvRatio);
	//uv -= g_pixelStep;
	
    return tex2D(g_texSampler, uv);// * inColor;
}

//-------------------------------------------------------------------------
// technique
//-------------------------------------------------------------------------
technique MainDraw
{
	pass P0
	{
		VertexShader = compile vs_2_0 vsBasic();
		PixelShader	 = compile ps_2_0 psBasic();
	}
}
