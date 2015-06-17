//=============================================================================
// SpriteRenderer
//-----------------------------------------------------------------------------
//  SpriteRenderer 用シェーダ
//=============================================================================

float4x4        gViewProjMatrix;

float2          gViewportSize;
static float2   gViewportOffset = ( float2( 0.5, 0.5 ) / gViewportSize );

texture2D gMaterialTexture;
sampler gTexSampler = sampler_state
{
    Texture = < gMaterialTexture >;
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
    float3 inPos        : POSITION,
    float4 inColor      : COLOR0,
    float2 inUV         : TEXCOORD0 )
{
    VS_OUTPUT o;

    o.Pos = mul( float4( inPos, 1.0f ), gViewProjMatrix );
	o.Pos.x -= gViewportOffset.x;
	o.Pos.y += gViewportOffset.y;
    o.Color = inColor;
    o.TexUV = inUV;// + gViewportOffset;

    return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(
    float4 in_color_    : COLOR0,
    float2 in_uv_       : TEXCOORD0 ) : COLOR0
{
	return tex2D( gTexSampler, in_uv_ ) * in_color_;
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
//
//=============================================================================
