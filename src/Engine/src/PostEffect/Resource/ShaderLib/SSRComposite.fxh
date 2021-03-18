

//==============================================================================
// Uniforms

//==============================================================================
// Pixel shader

sampler2D _SSRSampler;

float4 CompositeSSR(float4 color, float2 uv)
{
    const float4 ssr = tex2D(_SSRSampler, uv);
    const float4 result = float4(lerp(color.rgb, ssr.rgb, ssr.a), 1.0);
    return result;
}
