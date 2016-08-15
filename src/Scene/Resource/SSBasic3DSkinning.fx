


// ボーンテクスチャサイズの逆数
float2 lnBoneTextureReciprocalSize = float2( 1.0/4.0, 1.0/256.0 );

// ボーン用頂点テクスチャサンプラー宣言
texture lnBoneTexture;
sampler2D lnBoneSampler = sampler_state
{
    Texture = <lnBoneTexture>;
    MinFilter = Point;  // 殆どのGPUでは以下のようなステート設定にしないと
    MagFilter = Point;  // 頂点テクスチャのフェッチがうまくいかない
    MipFilter = None;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct MMM_SKINNING_INPUT
{
	float4	Pos				: POSITION;
	float4	BlendWeight		: BLENDWEIGHT;
	float4	BlendIndices	: BLENDINDICES;
    float4	Color			: COLOR0;		// 独自
	float3	Normal			: NORMAL;
	float2	Tex				: TEXCOORD0;
	float4	AddUV1			: TEXCOORD1;
	float4	AddUV2			: TEXCOORD2;
	float4	AddUV3			: TEXCOORD3;
	float4	AddUV4			: TEXCOORD4;
	float4	SdefC			: TEXCOORD5;
	float3	SdefR0			: TEXCOORD6;
	float3	SdefR1			: TEXCOORD7;
	float	EdgeWeight		: TEXCOORD8;
	float	Index			: PSIZE15;
};

struct MMM_SKINNING_OUTPUT
{
	float4	Position;
	float3	Normal;
};

#define MaxIndices 4

MMM_SKINNING_OUTPUT MMM_SkinnedPositionNormal(
	float4	position,
	float3	normal,
	float4	weights,
	float4	indices,
	float4	sdefC,
	float3	sdefR0,
	float3	sdefR1 )
{
	MMM_SKINNING_OUTPUT o;
	o.Position = 0.0;
	o.Normal = 0.0;

	float2 uv = lnBoneTextureReciprocalSize;
	float  weights_array[MaxIndices];//  = (float[MaxIndices])weights;
	weights_array[0] = weights.x;
	weights_array[1] = weights.y;
	weights_array[2] = weights.z;
	weights_array[3] = weights.w;
	//int    index_array[MaxIndices]    = { (int)indices.x, (int)indices.y, (int)indices.z, (int)indices.w };//(int[MaxIndices])D3DCOLORtoUBYTE4(indices);
	int    index_array[MaxIndices];
	index_array[0] = (int)indices.x;
	index_array[1] = (int)indices.y;
	index_array[2] = (int)indices.z;
	index_array[3] = (int)indices.w;
	
	float  last_weight = 0.0;
    
	// 最後のひとつ以外
	int iBone = 0;
	for ( ; iBone < MaxIndices - 3; ++iBone )
	{
		if ( weights_array[iBone] != 0.0 )
		{
			last_weight = last_weight + weights_array[iBone];

			float4 tc0 = float4( 0.5f * uv.x, ( index_array[iBone] + 0.5f ) * uv.y, 0, 1 );	// +0.5 は半ピクセル分
			float4 tc1 = float4( 1.5f * uv.x, ( index_array[iBone] + 0.5f ) * uv.y, 0, 1 );
			float4 tc2 = float4( 2.5f * uv.x, ( index_array[iBone] + 0.5f ) * uv.y, 0, 1 );
			float4 tc3 = float4( 3.5f * uv.x, ( index_array[iBone] + 0.5f ) * uv.y, 0, 1 );
	        
			float4x4 bone = float4x4(
				tex2Dlod( lnBoneSampler, tc0 ),
				tex2Dlod( lnBoneSampler, tc1 ),
				tex2Dlod( lnBoneSampler, tc2 ),
				tex2Dlod( lnBoneSampler, tc3 ) );

			o.Position += mul( position , bone ) * weights_array[iBone];
			o.Normal   += (mul( float4( normal, 1.0 ), bone ) * weights_array[iBone]).xyz;
		}
	}
    
	// 最後のひとつ
	last_weight = 1.0 - last_weight;

	float4 tc0 = float4( 0.5f * uv.x, ( index_array[iBone] + 0.5f ) * uv.y, 0, 1 );
	float4 tc1 = float4( 1.5f * uv.x, ( index_array[iBone] + 0.5f ) * uv.y, 0, 1 );
	float4 tc2 = float4( 2.5f * uv.x, ( index_array[iBone] + 0.5f ) * uv.y, 0, 1 );
	float4 tc3 = float4( 3.5f * uv.x, ( index_array[iBone] + 0.5f ) * uv.y, 0, 1 );

	float4x4 bone = float4x4(
		//1, 0, 0, 0,
		//0, 1, 0, 0,
		//0, 0, 1, 0,
		//0, 0, 0, 1);
		tex2Dlod( lnBoneSampler, tc0 ),
		tex2Dlod( lnBoneSampler, tc1 ),
		tex2Dlod( lnBoneSampler, tc2 ),
		tex2Dlod( lnBoneSampler, tc3 ) );

	o.Position += mul( position, bone ) * last_weight;
	o.Normal   += (mul( float4( normal, 1.0 ), bone ) * last_weight).xyz;
	return o;
}




//=============================================================================
// SSNoLighting
//-----------------------------------------------------------------------------
//  ライティングなし SceneShader
//=============================================================================

float4x4 gWorldViewProjMatrix       : WORLDVIEWPROJECTION;

float           gOpacity            : OPACITY;
float4          gColorScale         : COLORSCALE;
float4          gBlendColor         : BLENDCOLOR;
float4          gTone               : TONE;
static float4   gColor              = float4( gColorScale.xyz, gColorScale.a * gOpacity );

texture gTexture    : MATERIALTEXTURE;
sampler gTexSampler = sampler_state
{
    texture = < gTexture >;
    MINFILTER = NONE;
    MAGFILTER = NONE;
};

struct VS_OUTPUT
{
    float4 Pos      : POSITION;
    float4 Color    : COLOR0;
    float2 TexUV    : TEXCOORD0;
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(MMM_SKINNING_INPUT IN )
{
    VS_OUTPUT o;
	
	MMM_SKINNING_OUTPUT skinOut = 
		MMM_SkinnedPositionNormal(IN.Pos, IN.Normal, IN.BlendWeight, IN.BlendIndices, IN.SdefC, IN.SdefR0, IN.SdefR1);
    o.Pos   = mul( skinOut.Position, gWorldViewProjMatrix );
	//o.Pos   = mul( IN.Pos, gWorldViewProjMatrix );
    o.Color = IN.Color;
    o.TexUV = IN.Tex;

    return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(
    float4 in_color_    : COLOR0,
    float2 in_uv_       : TEXCOORD0 ) : COLOR0
{
    float4 out_color = tex2D( gTexSampler, in_uv_ );// * in_color_ * gColor;
    
    //out_color.rgb = lerp( out_color.rgb, gBlendColor.rgb, gBlendColor.a );
    
	//float y = ( 0.208012 * out_color.r + 0.586611 * out_color.g + 0.114478 * out_color.b ) * gTone.w;
    //out_color.rgb = ( out_color.rgb * ( 1.0 - gTone.w ) ) + y + gTone.rgb;

    return out_color;
}

//-------------------------------------------------------------------------
// technique
//-------------------------------------------------------------------------
technique MainDraw
{
    pass P0
    {
        VertexShader = compile vs_3_0 vsBasic();
        PixelShader	 = compile ps_3_0 psBasic();
        CullMode = None;
    }
}

//=============================================================================
//
//=============================================================================
