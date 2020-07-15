


const float4 _Vignette_Color = float4(0, 0, 0, 1); 
const float2 _Vignette_Center = float2(0.5, 0.5);
const float4 _Vignette_Settings = float4(0.5, 0.5, 1.0, 0); // x: intensity, y: smoothness, z: roundness, w: rounded

float3 Vignette(float3 color, float2 uv)
{
    float2 uvDistorted = uv;
    
        float2 d = abs(uvDistorted - _Vignette_Center) * _Vignette_Settings.x;
        d *= 2.0;

        d.x *= lerp(1.0, ln_Resolution.x / ln_Resolution.y, _Vignette_Settings.w);
        d = pow(saturate(d), _Vignette_Settings.z); // Roundness
        //return float4(d, 0, 1);
        float vfactor = pow(saturate(1.0 - dot(d, d)), _Vignette_Settings.y);
        //return float4(vfactor, 0, 0, 1);

        const float3 result = color * lerp(_Vignette_Color.rgb, float3(1.0, 1.0, 1.0), vfactor);

        //float a = lerp(1.0, color.a, vfactor);
        //return float4(color.a, 0, 0, 1);

        //rgb = color.rgb * color.a;
    
    return result;

}
