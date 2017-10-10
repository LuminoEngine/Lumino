/* Lumino ShadingModel combined HLSL file (RawIR:Intermediate Representation) */

//BEGIN_HLSL

//------------------------------------------------------------------------------
// Lib


float4x4	ln_World;
float4x4	ln_WorldViewProjection;
float2		ln_ViewportPixelSize;
texture		ln_MaterialTexture;


struct LN_VSInput
{
	float3	Pos			: POSITION;
	float3	Normal		: NORMAL0;
	float4	Color		: COLOR0;
	float2	UV			: TEXCOORD0;
};

struct LN_VSOutput_Common
{
	float4	svPos		: POSITION;
	float3	Normal		: NORMAL0;
	float4	Color		: COLOR0;
	float2	UV			: TEXCOORD0;
};

struct LN_VSOutput_ClusteredForward
{
	float3	WorldPos	: TEXCOORD10;
};

struct LN_PSInput_ClusteredForward
{
	float3	WorldPos	: TEXCOORD10;
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

LN_VSOutput_Common _LN_ProcessVertex_Common(LN_VSInput input)
{
	LN_VSOutput_Common o;
	o.svPos			= mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
	o.Normal		= mul(float4(input.Normal, 1.0f), ln_World).xyz;
	o.UV			= input.UV + (float2(0.5, 0.5) / ln_ViewportPixelSize);
	o.Color			= input.Color;
	return o;
}

// ★コア部分の処理は Lib に置く。Auto Generation ではない。ユーザーが vs/ps を直書きするときに使えるようにするため。
LN_VSOutput_ClusteredForward _LN_ProcessVertex_ClusteredForward(LN_VSInput input)
{
	LN_VSOutput_ClusteredForward output;
	output.WorldPos = mul(float4(input.Pos, 1.0), ln_World);
	return output;
}

float4 _LN_ProcessPixel_ClusteredForward(LN_PSInput_Common common, LN_PSInput_ClusteredForward extra, LN_SurfaceOutput surface)
{
	return float4(1, 0, 0, 1);
}

//------------------------------------------------------------------------------
// User code

//struct MyVFOutput
//{
//	float4	WorldPos	: TEXCOORD5;
//};

struct MySSInput
{
	float4	Color		: COLOR0;		// defined
	float2	UV			: TEXCOORD0;	// defined
	float4	WorldPos	: TEXCOORD5;
};

sampler		MaterialTextureSampler = sampler_state
{
	texture = <ln_MaterialTexture>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

//void MyVFMain(LNVFInput input, inout MyVFOutput output)	// ★ out 引数の型をパース
//{
//}

// Surface Shader
void MySSMain(MySSInput input, inout LN_SurfaceOutput output)
{
	output.Albedo = float4(1, 0, 0, 1);
}

//------------------------------------------------------------------------------
// ★ Vertex Shader (auto generation)

struct _VSOutput
{
	LN_VSOutput_Common				Common;
	LN_VSOutput_ClusteredForward	Extra;
	/* VF 定義なし */ //MyVFOutput						User;	// ★ MyVFMain() の最後の引数をパースして得る
};

// auto generation
_VSOutput _VS_ClusteredForward_Geometry(LN_VSInput vsi)
{
	_VSOutput o;
	o.Common	= _LN_ProcessVertex_Common(vsi);
	o.Extra		= _LN_ProcessVertex_ClusteredForward(vsi);
	// ★ Scene固有のコードはここに直接生成する (ピクセルシェーダと書き方を合わせたい)
	/* VF 定義なし */ //o.uservsout = MyVFMain(vsi);	// ★ User定義呼び出し
	return o;
}


// ★ Pixel Shader

struct _PS_Input
{
	LN_PSInput_Common				Common;
	LN_PSInput_ClusteredForward		Extra;
	MySSInput						User;	// ★ MyVFMain() の最後の引数をパースして得る
};

// auto generation
float4 _PS_ClusteredForward_Geometry(_PS_Input input) : COLOR0
{
	LN_SurfaceOutput surface;
	_LN_InitSurfaceOutput(input.Common, surface);
	MySSMain(input.User, surface);	// ★ User定義呼び出し
	
	// ★ライティングのコードはここに直接生成する (GBuffer生成などではマルチRT書き込みするため、戻り値も変えなければならない)
	// ・・・というより、ピクセルシェーダ全体を生成する。フラグメントの結合じゃダメ。
	return _LN_ProcessPixel_ClusteredForward(input.Common, input.Extra, surface);
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
	//pass ShadowCaster
	//{
	//	VertexShader = compile vs_3_0 _VS_ClusteredForward_ShadowCaster();
	//	PixelShader	 = compile ps_3_0 _PS_ClusteredForward_ShadowCaster();
	//}
	pass Geometry
	{
		VertexShader = compile vs_3_0 _VS_ClusteredForward_Geometry();
		PixelShader	 = compile ps_3_0 _PS_ClusteredForward_Geometry();
	}
}

//END_HLSL

