
#include <Lumino.fxh>

//==============================================================================
// Pixel shader

float4 PSMain(LN_PSInput input) : SV_TARGET0
{
    LN_Surface surface;
    LN_ProcessSurface(input, surface);

    float4 color = LN_ProcessPixel(input, surface);

    const float3 pos = float3(1, 0, 1);
    const float3 gridDivision = 0.2;

    const float _ringRadius = 5.0;
    const float4 _ringEffectColor = float4(0, 0.8, 0.7);

    const float d = distance(input.worldPos, pos) / _ringRadius;

    // リング1つだけ出したい
    const float d2 = clamp(d, 0, 3.14);

    // 波の頂点付近を強調
    const float ringA = saturate(pow(sin(d2), 10));

    // グリッド
    const float xa = saturate(pow(sin(input.worldPos.x / gridDivision), 10));
    const float za = saturate(pow(sin(input.worldPos.z / gridDivision), 10));
    const float gridA = 1.0 - smoothstep(0, _ringRadius, d * 2.0);

    return float4(_ringEffectColor.rgb * saturate(ringA + ((xa + za) * gridA)), 1);
}

//==============================================================================
// Techniques

technique Forward_Geometry
{
    Normal = Default;
    pass Pass1
    {
        PixelShader  = PSMain;
    }
}

technique Forward_Geometry_NormalMap
{
    Normal = NormalMap;
    pass Pass1
    {
        PixelShader  = PSMain;
    }
}
