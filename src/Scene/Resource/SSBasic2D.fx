//=============================================================================
// SSBasic2D
//-----------------------------------------------------------------------------
//  2D描画基本
//=============================================================================

float4x4        gWorldMatrix    : WORLD;
float4x4        g2DProjMatrix   : VIEWPROJECTION; // スクリーンの左上を原点にとし、-0.5 のピクセルオフセットを含めてスクリーン座標空間上に座標変換する行列

float2          gViewportSize   : VIEWPORTPIXELSIZE;
static float2   gViewportOffset = ( float2( 0.5, 0.5 ) / gViewportSize );

float           gOpacity        : OPACITY;
float4          gColorScale     : COLORSCALE;
float4          gBlendColor     : BLENDCOLOR;
float4          gTone           : TONE;
static float4   gColor          = float4( gColorScale.xyz, gColorScale.a * gOpacity );

texture gMaterialTexture	    : MATERIALTEXTURE;
sampler gTexSampler = sampler_state
{
    texture = < gMaterialTexture >;
    MINFILTER = NONE;
    MAGFILTER = NONE;
};

struct VS_OUTPUT 
{
    float4 Pos      : POSITION;
    float2 TexUV    : TEXCOORD0;
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(
    float3 inPos  : POSITION,
	float4 inColor	: COLOR0,
    float2 inUV   : TEXCOORD0 )
{
    VS_OUTPUT o;
    
    o.Pos = mul( float4( inPos, 1.0 ), gWorldMatrix );
    o.Pos = mul( o.Pos, g2DProjMatrix );
	//o.Pos = float4(inPos, 1.0);
	//o.Pos.x /= 640;
	//o.Pos.y /= 480;
    o.TexUV = inUV + gViewportOffset;

    return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(
    float2 in_uv_   : TEXCOORD0  ) : COLOR
{
    float4 out_color = tex2D( gTexSampler, in_uv_ ) * gColor;
    
    // ブレンドの計算
    //out_color.rgb = ( gBlendColor.rgb * gBlendColor.a ) + ( out_color.rgb * ( 1.0 - gBlendColor.a ) );
    out_color.rgb = lerp( out_color.rgb, gBlendColor.rgb, gBlendColor.a );
    
    // 色調の計算。(NTSC 系加重平均法というらしい。緑だけがなんとなく明るく感じるとかを防ぐ)
	float y = ( 0.208012 * out_color.r + 0.586611 * out_color.g + 0.114478 * out_color.b ) * gTone.w;
    out_color.rgb = ( out_color.rgb * ( 1.0 - gTone.w ) ) + y + gTone.rgb;
    
	return out_color;
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

//=============================================================================
//                              end of file
//=============================================================================
