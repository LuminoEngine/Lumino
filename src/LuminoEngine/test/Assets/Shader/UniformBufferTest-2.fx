
float4x4	ln_View;
float4x4	ln_Projection;
float4x4	ln_World;
float4x4	ln_WorldViewProjection;
float4x4	ln_WorldView;
float4x4	ln_WorldViewIT;
float2		ln_ViewportPixelSize;
sampler2D		ln_MaterialTexture;
float4		ln_ColorScale = float4(1, 1, 1, 1);
float4		ln_BlendColor = float4(0, 0, 0, 1);
float4		ln_ToneColor = float4(0, 0, 0, 0);

float		ln_nearClip;
float		ln_farClip;
sampler2D ln_clustersTexture;
sampler2D ln_GlobalLightInfoTexture;

sampler2D ln_pointLightInfoTexture;
float4x4        ln_ViewProjection_Light0;

float4	ln_MaterialColor;
float	ln_MaterialRoughness;
float	ln_MaterialMetallic;
float	ln_MaterialSpecular;
float4	ln_MaterialEmissive;

sampler2D         ln_DirectionalShadowMap;
float2          ln_DirectionalShadowMapPixelSize;

sampler2D ln_BoneTexture;
sampler2D ln_BoneLocalQuaternionTexture;

//------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
};
 
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
};
 
VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = input.Pos;
    return output;
}

//------------------------------------------------------------------------------
float4 psMain() : SV_TARGET
{
    return ln_MaterialColor;
}

//------------------------------------------------------------------------------
technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain;
    }
}

