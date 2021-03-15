
#include <Lumino.fxh>

sampler2D _LensflareOcclusionMap;
//const float _DiscRadius = 0.5;
const float _DiscRadius = 1.0;
const float _FlareRadius = 0.25; // Against screen
const float _FlareDistance = 0.2;

//#define DEBUG_OCCLUSION 1

//==============================================================================
// Vertex shader

struct VS_Input
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD0;
};

struct VS_Output
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
    float Density : TEXCOORD1;
};

float2 screenToTex(float2 p)
{
    //float2 f = float2(p.x, p.y);
    float2 f = float2(p.x, -p.y);
    return (f.xy + 1.0) / 2.0;
}

VS_Output VS_Main(LN_VSInput input)
{
    float inverseAspect = ln_Resolution.y / ln_Resolution.x;

    // ln_WorldViewProjection は視点を向く回転も適用されている。
    float4 lightPos = mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
    float2 lightScreenPos = lightPos.xy / lightPos.w;
    float2 flareDir = -lightScreenPos;
    float2 flareOffset = flareDir * _FlareDistance;

    VS_Output output;
    //output.Pos = mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
    output.Pos = float4(input.Pos.xy * _FlareRadius, 0, 1);
    output.Pos.x *= inverseAspect;
    output.Pos.xy += flareOffset;

    output.UV = input.UV;

    // 濃さの計算は頂点ごとに行う必要はないのだが、かと言って CPU 側に
    // RenderTarget の内容を転送して計算するとかえって負荷が大きくなってしまう。
    {
        // サンプリング範囲の上下左右、中央
        float4 s1 = mul(float4(-_DiscRadius,  0, 0, 1), ln_WorldViewProjection);
        float4 s2 = mul(float4( _DiscRadius,  0, 0, 1), ln_WorldViewProjection);
        float4 s3 = mul(float4( 0,  _DiscRadius, 0, 1), ln_WorldViewProjection);
        float4 s4 = mul(float4( 0, -_DiscRadius, 0, 1), ln_WorldViewProjection);
        float4 s5 = mul(float4( 0, 0, 0, 1), ln_WorldViewProjection);

        // スクリーン空間上のサンプリング範囲
        float2 p1 = screenToTex(s1.xy / s1.w);
        float2 p2 = screenToTex(s2.xy / s2.w);
        float2 p3 = screenToTex(s3.xy / s3.w);
        float2 p4 = screenToTex(s4.xy / s4.w);
        float2 p5 = screenToTex(s5.xy / s5.w);

        // Disc の外周付近をサンプリングして、平均値から濃さを求める
        float4 color = tex2Dlod(_LensflareOcclusionMap, float4(p1, 0, 1));
        color += tex2Dlod(_LensflareOcclusionMap, float4(p2, 0, 1));
        color += tex2Dlod(_LensflareOcclusionMap, float4(p3, 0, 1));
        color += tex2Dlod(_LensflareOcclusionMap, float4(p4, 0, 1));
        color += tex2Dlod(_LensflareOcclusionMap, float4(p5, 0, 1));
        output.Density = color.r / 5.0;

        //output.Density = tex2Dlod(_LensflareOcclusionMap, float4(p5, 0, 0));
        //output.Density = tex2D(_LensflareOcclusionMap, p5);
        //output.Density = float4(p5, 0, 1);

#if DEBUG_OCCLUSION
        output.UV.x = lerp(p1.x, p2.x, input.UV.x);
        output.UV.y = lerp(p3.y, p4.y, input.UV.y);
#endif
    }

    
    //output.Pos.x = lerp(s1.x, s2.x, input.UV.x);
    //output.Pos.y = lerp(s3.y, s4.y, input.UV.y);
    //output.Pos.z = s1.z;
    //output.Pos.w = s1.w;
    //output.Pos = mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
    //output.Pos.z = 0;
    //output.Pos.w = 1;

    return output;
}

//==============================================================================
// Pixel shader

struct PS_Input
{
    float2 UV : TEXCOORD0;
    float Density : TEXCOORD1;
};

float4 PS_Main(PS_Input input) : SV_TARGET
{
#if DEBUG_OCCLUSION
    float4 color = tex2D(_LensflareOcclusionMap, input.UV);
#else
    float4 color = tex2D(ln_MaterialTexture, input.UV);
    color.a = input.Density;
#endif
    return color;
}

//==============================================================================
// Technique

technique Forward_Geometry
{
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}
