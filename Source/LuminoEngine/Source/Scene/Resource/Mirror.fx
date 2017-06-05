
struct LN_VSInput
{
	float3	Pos		: POSITION;		// 位置
	float2	UV		: TEXCOORD0;	// テクスチャ座標
	float4	Color	: COLOR0;		// 頂点色
};


static float2	ViewportOffset = (float2(0.5, 0.5) / ln_ViewportPixelSize);
static float2	ViewportOffset2 = (float2(1.0, 1.0) / ln_ViewportPixelSize);

sampler		MaterialTextureSampler = sampler_state
{
	texture = <ln_MaterialTexture>;
	//MINFILTER = ANISOTROPIC;	// NONE,POINT だと、MipMap の境界にシャギのようなものが見える。LINEAR だと近景でもボケすぎる。
	MAGFILTER = LINEAR;			// ANISOTROPIC でも変わらない。LINEAR で良いのでは。
	MIPFILTER = LINEAR;			// ANISOTROPIC でも変わらない。LINEAR で良いのでは。
	//MAXANISOTROPY = 16;
};

struct VSOutput
{
	float4	Pos		: POSITION;
	float4	Color	: COLOR0;
	float4	UV		: TEXCOORD1;
};

struct PSInput
{
	float4	Color	: COLOR0;
	float4	UV		: TEXCOORD1;
};

//------------------------------------------------------------------------------
VSOutput VSBasic(LN_VSInput v)
{
	VSOutput o;
	o.Pos	= mul(float4(v.Pos, 1.0f), ln_WorldViewProjection);
	//o.Pos.x -= ViewportOffset2.x;
	//o.Pos.y += ViewportOffset2.y;
	
	
	
	//float x = ((o.Pos.x / o.Pos.w) / 2) + 0.5;
	//float y = ((o.Pos.y / o.Pos.w) / 2) + 0.5;
	
	//o.UV.x = -x;//((o.Pos.x / o.Pos.w) + 1.0) / 2;
	//o.UV.y = 1.0 - y;//1.0 - (((o.Pos.y / o.Pos.w) + 1.0) / 2);
	o.UV = o.Pos;//o.Pos.xy / o.Pos.w;
	
	o.Color	= v.Color;
	return o;
}

//------------------------------------------------------------------------------
float4 PSBasic(PSInput p) : COLOR0
{
    float2 ProjectedTexCoords;
    ProjectedTexCoords.x = p.UV.x/p.UV.w/2.0f + 0.5f;
    ProjectedTexCoords.y = -p.UV.y/p.UV.w/2.0f + 0.5f;
	
	//float x = ((p.UV.x) / 2) + 0.5;
	//float y = ((p.UV.y) / 2) + 0.5;
	//return float4(ProjectedTexCoords.x, ProjectedTexCoords.y, 0, 1);
	//y = 1.0 - y;
	return (tex2D(MaterialTextureSampler, ProjectedTexCoords) * p.Color) * ln_ColorScale + float4(-0.1,-0.1, -0.0,-0.2);//float4(p.UV, 0, 1);//
}

//------------------------------------------------------------------------------
technique MainDraw
{
	pass P0
	{
		VertexShader = compile vs_3_0 VSBasic();
		PixelShader	 = compile ps_3_0 PSBasic();
	}
}
