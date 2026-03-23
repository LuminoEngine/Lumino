// Unlit.slang — テクスチャ × カラー、ライティングなし
//
// BindGroup layout:
//   Set 0 (per-view):  ViewParams   { viewProj }
//   Set 1 (per-material): MaterialParams { color } + texture + sampler
//   Set 2 (per-object): ObjectParams { world }

import lumino;

struct MaterialParams {
    float4 color;
};

[[vk::binding(0, 0)]] ConstantBuffer<ViewParams>     viewParams;
[[vk::binding(0, 1)]] ConstantBuffer<MaterialParams>  materialParams;
[[vk::binding(1, 1)]] Texture2D                       baseTexture;
[[vk::binding(2, 1)]] SamplerState                    baseSampler;
[[vk::binding(0, 2)]] ConstantBuffer<ObjectParams>    objectParams;

struct VSOutput {
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD;
    float4 color    : COLOR;
};

[shader("vertex")]
VSOutput vsMain(VSInput input) {
    VSOutput output;
    float4 worldPos = mul(objectParams.world, float4(input.position, 1.0));
    output.position = mul(viewParams.viewProj, worldPos);
    output.uv = input.uv;
    output.color = input.color;
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_TARGET {
    float4 texColor = baseTexture.Sample(baseSampler, input.uv);
    return texColor * materialParams.color * input.color;
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
