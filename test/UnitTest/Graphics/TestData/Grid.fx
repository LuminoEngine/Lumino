
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
float4x4	WorldViewProjMatrix	: WORLDVIEWPROJECTIONINVERSE;
float4x4	WorldMatrix			: WORLD;
float4x4	ViewMatrix			: VIEW;
float4x4	ProjMatrix			: PROJECTION;

// オブジェクトのテクスチャ
texture		ObjectTexture		: MATERIALTEXTURE;
sampler		ObjTexSampler		= sampler_state
{
	texture = <ObjectTexture>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

struct VS_OUTPUT
{
	float4	Pos		: POSITION;		// 位置
	float2	Tex		: TEXCOORD0;	// テクスチャ座標
};

//------------------------------------------------------------------------------
// 頂点シェーダ
VS_OUTPUT Basic_VS(LN_VS_INPUT input)
{
	VS_OUTPUT output;
	
	// 頂点位置をスクリーン空間に変換
	output.Pos = float4(input.Pos, 1.0);
	
	// テクスチャ座標
	output.Tex = input.Pos.xy;//input.Tex;
	
	return output;
}

float grid(float3 v)
{
	float g;
	g = abs((fmod(v.x*4.0,0.25)*4.0)-0.5);
	g = max(g,abs((fmod(v.y*4.,0.25)*4.)-0.5));
	g = max(g,abs((fmod(v.z*4.,0.25)*4.)-0.5));
	
	g = smoothstep(0.5+length(v)*0.25,0.5,1.-g);
	return g;
}
static float3 plane_normal = float3(0, 1, 0);// float4(0, 1, 0, dot(float3(0, 1, 0), float3(0, 0, 0)));
static float  plane_d = dot(plane_normal, float3(0, 0, 0));
static float3 start = float3(0, 0, 0);

static float3 camPos = float3(0, 0, 0);

float3 CameraPosition : POSITION  < string Object = "Camera"; >;

//------------------------------------------------------------------------------
// ピクセルシェーダ
float4 Basic_PS(VS_OUTPUT input) : COLOR0
{
	//float4 color;
	
	//float4 texColor = tex2D(ObjTexSampler, input.Tex);
	//color = texColor;
	
	float4 ray1 = mul(float4(input.Tex, 0, 0), WorldViewProjMatrix);
	float3 ray = ray1.xyz;// / ray1.w;
	ray *= 100;
	
	float3 direction = ray - CameraPosition;
	
	float dt = dot(plane_normal, direction);
	if (dt == 0.0) return float4(1, 0, 0, 1);
	
	float t = (plane_d + dot(plane_normal, CameraPosition)) / dt;
	float3 pt = CameraPosition - (t * direction);
	
	float2 uv = fmod(pt.xz, 1.0);
	float c = step(0.9, max(uv.x, uv.y));
	
	
	//grid(float3(input.Tex, 0.0));
	return float4(c, 0, 0, 1);
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

