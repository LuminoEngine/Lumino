// Unlit.slang — テクスチャ × カラー、ライティングなし
//
// ParameterBlock layout:
//   Set 0 (per-view):     viewData     — ViewParams (camera)
//   Set 1 (per-scene):    sceneData    — SceneParams (lighting, unused)
//   Set 2 (per-material): materialData — MaterialData (CB + Texture + Sampler)
//   Set 3 (per-object):   objectData   — ObjectParams (implicit CB)

import lumino;

struct MaterialParams {
    float4 color;
};

struct MaterialData {
    ConstantBuffer<MaterialParams> params;
    Texture2D                      baseTexture;
    SamplerState                   baseSampler;
};

ParameterBlock<ViewParams>     viewData;       // -> Set 0
ParameterBlock<SceneParams>    sceneData;      // -> Set 1 (unused; required for PipelineLayout compatibility)
ParameterBlock<MaterialData>   materialData;   // -> Set 2
ParameterBlock<ObjectParams>   objectData;     // -> Set 3

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
    float4 texColor = materialData.baseTexture.Sample(materialData.baseSampler, input.uv);
    return texColor * materialData.params.color * input.color;
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
