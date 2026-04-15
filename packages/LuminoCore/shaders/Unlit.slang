// Unlit.slang — テクスチャ × カラー、ライティングなし
//
// Set 0 ($Global / material): u_params, u_baseTexture, u_baseSampler
// Set 1 (per-view):           viewData   — ViewParams
// Set 2 (per-scene):          sceneData  — SceneParams (unused)
// Set 3 (per-object):         objectData — ObjectParams

import lumino;

//------------------------------------------------------------------------------
// Material parameters (bare uniforms → $Global, Set 0)
//------------------------------------------------------------------------------
struct MaterialParams {
    float4 color;
};

uniform ConstantBuffer<MaterialParams> u_params;
uniform Texture2D                      u_baseTexture;
uniform SamplerState                   u_baseSampler;

struct VSOutput {
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD;
    float4 color    : COLOR;
};

[shader("vertex")]
VSOutput vsMain(VSInput input) {
    VSOutput output;
    float4 worldPos = mul(objectData.world, float4(input.position, 1.0));
    output.position = mul(viewData.viewProj, worldPos);
    output.uv = input.uv;
    output.color = input.color;
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_TARGET {
    float4 texColor = u_baseTexture.Sample(u_baseSampler, input.uv);
    return texColor * u_params.color * input.color;
}

#ifdef LUMINO_METADATA
{
    "passes": [
        {
            "name": "Forward",
            "vertex": "vsMain",
            "fragment": "fsMain"
        }
    ]
}
#endif
