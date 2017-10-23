//==============================================================================
// ImageFilter_Tone
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
#ifdef LN_HLSL_DX9

static float2   gViewportOffset = (float2( 0.5, 0.5 ) / ln_ViewportPixelSize);


// スクリーン
sampler ScreenSampler = sampler_state
{ 
	Texture		= <ln_MaterialTexture>;
	//MinFilter	= LINERE;
	//MagFilter	= POINT;
	//MipFilter	= NONE;
	MinFilter	= LINEAR;
	MagFilter	= LINEAR;
	MipFilter	= LINEAR;
	AddressU	= CLAMP;
	AddressV	= CLAMP;
};

struct VSOutput
{ 
	float4 Pos		: POSITION;
	float2 TexUV	: TEXCOORD0;
};


static float bb = 1.5;
static float2 sampOffset[5] = {
	//float2(-1.0,  0.0) / ln_ViewportPixelSize,
	//float2(-0.5, -0.5) / ln_ViewportPixelSize,
	//float2( 0.5, -0.5) / ln_ViewportPixelSize,
	//float2( 1.0,  0.0) / ln_ViewportPixelSize,
	//float2( 0.5,  0.5) / ln_ViewportPixelSize,
	//float2(-0.5,  0.5) / ln_ViewportPixelSize,
	float2(-2.0,  0.0) / ln_ViewportPixelSize * bb,
	float2(-1.4,  1.4) / ln_ViewportPixelSize * bb * 2,
	float2( 0.0,  2.0) / ln_ViewportPixelSize * bb * 3,
	float2( 1.4,  1.4) / ln_ViewportPixelSize * bb * 4,
	float2( 2.0,  0.0) / ln_ViewportPixelSize * bb * 5,
};    // サンプリング点のオフセット値 

//-----------------------------------------------------------------------------
VSOutput VSMain(
	float3 pos		: POSITION,
	float2 texUV	: TEXCOORD0)
{
	VSOutput o;
	o.Pos	= float4(pos, 1.0f);
	o.TexUV	= texUV + gViewportOffset;
	return o;
}

// http://developer.wonderpla.net/entry/blog/engineer/iPhone5S_SSAO/
/*
#version 300 es  
  
precision mediump float;  
  
in lowp vec2 texcoord;  
  
layout (location = 0) out mediump vec4 fragColor;  
  
uniform lowp sampler2D albedo;         // カラーテクスチャ  
uniform mediump sampler2D normalDepth; // 法線、深度マップ  
  
uniform mediump vec2 sampOffset[6];    // サンプリング点のオフセット値  
  
uniform mediump sampler2D rotPallet;   // サンプリング点回転用テクスチャ  
uniform mediump vec2 palletUvScale;    // 回転テクスチャ用UVスケール  
  
float tangent(vec3 p, vec3 s) {  
    return (p.z - s.z) / length(s.xy - p.xy);  
}  
  
float EdgeBias = 0.08;  
float PI = 3.14159;  
  
void main()  
{  
    float depth = texture(normalDepth, texcoord).w;  
    vec3 pos = vec3(texcoord.xy, depth);  
      
    // テクスチャから回転行列を作成  
    vec4 pallet = texture(rotPallet, texcoord * palletUvScale);  
    mat2 rot;  
    rot[0] = pallet.xy;  
    rot[1] = pallet.zw;  
      
    float d = 0.0;  
    for( int i=0; i&lt;6; ++i ) {  
          
        // サンプリングオフセット値の回転  
        vec2 samp = sampOffset[i] * rot;  
          
        // サンプリング点Aの深度値取得  
        vec2 sl = texcoord + samp;  
        vec3 pl = vec3( sl.xy, texture(normalDepth, sl).w );  
          
        // モデルのエッジを検出  
        if( pl.z &lt; pos.z - EdgeBias ) continue;  
          
        // 中心点からの角度を計算  
        float tl = atan( tangent(pos, pl) );  
  
        // サンプリング点A’の深度値取得  
        vec2 sr = texcoord - sampOffset[i];  
        vec3 pr = vec3( sl.xy, texture(normalDepth, sr).w );  
  
        if( pr.z &lt; pos.z - EdgeBias ) continue;  
  
        float tr = atan( tangent(pos, pr) );  
          
        // 遮蔽率を足し込む  
        d += clamp( (tl+tr) / PI, 0.0, 1.0 );  
    }  
      
    d = clamp( d / 6.0, 0.0, 1.0 );   
      
    fragColor = texture(albedo, texcoord) * (1.0-d);  
}  
*/


float tangent(float3 p, float3 s) {  
    return (p.z - s.z) / length(s.xy - p.xy);  
}  
  
float EdgeBias = 3.08; 
float PI = 3.14159; 

//-----------------------------------------------------------------------------
float4 PSMain(
	float2 texcoord	: TEXCOORD0) : COLOR
{
	//return tex2D(ScreenSampler, texcoord);
	float depth = tex2D(ScreenSampler, texcoord).r;//texture(normalDepth, texcoord).w;  
    float3 pos = float3(texcoord.xy, depth);
	
	
    float d = 0.0;  
    for( int i=0; i < 5; ++i ) {  

        // サンプリングオフセット値の回転  
        float2 samp = sampOffset[i];// * rot;  
          
        // サンプリング点Aの深度値取得  
        float2 sl = texcoord + samp;  
        float3 pl = float3( sl.xy, tex2D(ScreenSampler, sl).r);//tex2D(normalDepth, sl).w );  
          
        // モデルのエッジを検出  
        if( pl.z < pos.z - EdgeBias ) continue;  
          
        // 中心点からの角度を計算  
        float tl = atan( tangent(pos, pl) );  
  
        // サンプリング点A’の深度値取得  
        float2 sr = texcoord - sampOffset[i];  
        float3 pr = float3( sl.xy, tex2D(ScreenSampler, sr).r );  //tex2D(normalDepth, sr).w );  
  
        if( pr.z < pos.z - EdgeBias ) continue;  
  
        float tr = atan( tangent(pos, pr) );  
        
        
		//float r = length(sampOffset[i]) / 2;
        
        
        
        // 遮蔽率を足し込む  
        d += clamp( (tl+tr) / PI, 0.0, 1.0 );// * r;  
    }
    d = clamp( d / 6.0, 0.0, 1.0 ); 
	
	return float4((1.0-d), (1.0-d), (1.0-d), 1.0);
	//return tex2D(ScreenSampler, texUV);//LN_CalculateToneColor(tex2D(ScreenSampler, texUV), _Tone);
}

//-----------------------------------------------------------------------------
technique MainDraw
{
	pass P0
	{
		VertexShader	= compile vs_3_0 VSMain();
		PixelShader		= compile ps_3_0 PSMain();
	}
}
#endif


//=============================================================================
#ifdef LN_GLSL_VERTEX_Main


#endif

#ifdef LN_GLSL_FRAGMENT_Main


#endif
