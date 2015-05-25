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
