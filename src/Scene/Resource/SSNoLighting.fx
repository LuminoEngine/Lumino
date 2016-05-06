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
VS_OUTPUT vsBasic(
    float3 in_pos_      : POSITION,
    float4 in_color_    : COLOR0,
    float2 in_uv_       : TEXCOORD0 )
{
    VS_OUTPUT o;

    o.Pos   = mul( float4( in_pos_, 1.0f ), gWorldViewProjMatrix );
    o.Color = in_color_;
    o.TexUV = in_uv_;

    return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(
    float4 in_color_    : COLOR0,
    float2 in_uv_       : TEXCOORD0 ) : COLOR0
{
    float4 out_color = tex2D( gTexSampler, in_uv_ ) * in_color_ * gColor;
    
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
        VertexShader = compile vs_2_0 vsBasic();
        PixelShader	 = compile ps_2_0 psBasic();
        CullMode = None;
    }
}

//=============================================================================
//
//=============================================================================
