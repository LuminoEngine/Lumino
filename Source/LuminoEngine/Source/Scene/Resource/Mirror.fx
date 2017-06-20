#if 1


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
	return (tex2D(MaterialTextureSampler, ProjectedTexCoords) * p.Color) * ln_ColorScale + float4(-0.05,-0.05, -0.05,-0.05);//float4(p.UV, 0, 1);//
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



#else
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


Texture xWaterBumpMap;


sampler WaterBumpMapSampler = sampler_state { texture = <xWaterBumpMap>; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = mirror; AddressV = mirror;};

// TODO: 標準で視点位置がほしいなぁ
float3 xCamPos = float3(0, 1, -10);

float xWaveHeight = 0.4;

//specular input variables
float specPerturb = 4;//0.5;
float specPower = 10;//0.5;

float time;

struct VSOutput
{
	float4	Pos		: POSITION;
	float4	Color	: COLOR0;
	float4	ScreenPos	: TEXCOORD1;
	float4	UV		: TEXCOORD2;
    float4 Position3D	: TEXCOORD4;
};

struct PSInput
{
	float4	Color	: COLOR0;
	float4	ScreenPos	: TEXCOORD1;
	float4	UV		: TEXCOORD2;
    float4 Position3D	: TEXCOORD4;
};

//------------------------------------------------------------------------------
VSOutput VSBasic(LN_VSInput v)
{
	VSOutput o;
	o.Pos	= mul(float4(v.Pos, 1.0), ln_WorldViewProjection);
	//o.Pos.x -= ViewportOffset2.x;
	//o.Pos.y += ViewportOffset2.y;
	
	
	
	//float x = ((o.Pos.x / o.Pos.w) / 2) + 0.5;
	//float y = ((o.Pos.y / o.Pos.w) / 2) + 0.5;
	
	//o.UV.x = -x;//((o.Pos.x / o.Pos.w) + 1.0) / 2;
	//o.UV.y = 1.0 - y;//1.0 - (((o.Pos.y / o.Pos.w) + 1.0) / 2);
	o.ScreenPos = o.Pos;//o.Pos.xy / o.Pos.w;
	o.UV = float4(v.UV, 0, 0);;//o.Pos.xy / o.Pos.w;
	
	o.UV.y += time;
	
	o.Color	= v.Color;
	o.Position3D = float4(v.Pos, 1.0);
	return o;
}

//------------------------------------------------------------------------------
float4 PSBasic(PSInput p) : COLOR0
{
    float2 ProjectedTexCoords;
    ProjectedTexCoords.x = p.ScreenPos.x/p.ScreenPos.w/2.0f + 0.5f;
    ProjectedTexCoords.y = -p.ScreenPos.y/p.ScreenPos.w/2.0f + 0.5f;
	
	//float x = ((p.UV.x) / 2) + 0.5;
	//float y = ((p.UV.y) / 2) + 0.5;
	//return float4(ProjectedTexCoords.x, ProjectedTexCoords.y, 0, 1);
	//y = 1.0 - y;
	float4 color = (tex2D(MaterialTextureSampler, ProjectedTexCoords) * p.Color) * ln_ColorScale;// + float4(-0.1,-0.1, -0.0,-0.2);

	color -= float4(0.05, 0.025, 0.0, 0.0);
	
    // sampling the bump map
    float4 bumpColor = tex2D(WaterBumpMapSampler, p.UV);
	
    // perturbating the color
    float2 perturbation = xWaveHeight*(bumpColor.rg - 0.5f);
	
	
    float3 eyeVector = normalize(xCamPos - p.Position3D);
    float3 normalVector = float3(0,0,1);
	
/////////////////////////////////////////////////
// Specular Highlights
/////////////////////////////////////////////////

	float4 speccolor;

	float3 lightSourceDir = normalize(float3(0.1f,-0.6f,-0.5f));

	float3 halfvec = normalize(eyeVector+lightSourceDir+float3(perturbation.x*specPerturb,perturbation.y*specPerturb,0));
	
	float3 temp = 0;

	temp.x = pow(dot(halfvec,normalVector),specPower);
	
	speccolor = float4(0.98,0.97,0.7,0.6);
	
	speccolor = speccolor*temp.x;

	speccolor = float4(speccolor.x*speccolor.w,speccolor.y*speccolor.w,speccolor.z*speccolor.w,0);
	
	return color +speccolor ;// - bumpColor;//float4(p.UV, 0, 1);//
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
#endif

