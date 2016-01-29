//=============================================================================
// SpriteRenderer
//-----------------------------------------------------------------------------
//  SpriteRenderer 用シェーダ
//=============================================================================

//=============================================================================
#ifdef LN_HLSL_DX9

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

#endif
//=============================================================================
#ifdef LN_GLSL_VERTEX
uniform mat4	gViewProjMatrix;
attribute vec3	ln_Vertex;			// POSITION
attribute vec4	ln_Color0;			// COLOR0
attribute vec2	ln_MultiTexCoord0;	// TEXCOORD0
varying vec4	v_Color;
varying vec2	v_TexUV;

void main()
{
	gl_Position		= vec4(ln_Vertex, 1.0f) * gViewProjMatrix;
	v_Color			= ln_Color0;
	v_TexUV			= ln_MultiTexCoord0;
}
#endif
//=============================================================================
#ifdef LN_GLSL_FRAGMENT
uniform sampler2D gMaterialTexture;
varying vec4	v_Color;
varying vec2	v_TexUV;

void main()
{
    gl_FragColor = texture2D(gMaterialTexture, v_TexUV) * v_Color;
}
#endif










