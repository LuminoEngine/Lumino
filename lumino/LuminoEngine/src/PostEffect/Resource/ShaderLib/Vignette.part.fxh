


//const float4 vignetteColor = float4(0, 0, 0, 1); 
//const float2 vignetteCenter = float2(0.5, 0.5);
//const float4 vignetteSettings = float4(0.6, 0.5, 1.0, 0); // x: intensity, y: smoothness, z: roundness, w: rounded

float3 Vignette(float3 color, float2 uv, float4 vignetteColor, float2 vignetteCenter, float4 vignetteSettings)
{
    float2 uvDistorted = uv;
    
        float2 d = abs(uvDistorted - vignetteCenter) * vignetteSettings.x;
        d *= 2.0;

        d.x *= lerp(1.0, ln_Resolution.x / ln_Resolution.y, vignetteSettings.w);
        d = pow(saturate(d), vignetteSettings.z); // Roundness
        //return float4(d, 0, 1);
        float vfactor = pow(saturate(1.0 - dot(d, d)), vignetteSettings.y);
        //return float4(vfactor, 0, 0, 1);

        const float3 result = color * lerp(vignetteColor.rgb, float3(1.0, 1.0, 1.0), vfactor);

        //float a = lerp(1.0, color.a, vfactor);
        //return float4(color.a, 0, 0, 1);

        //rgb = color.rgb * color.a;
    
    return result;

}
