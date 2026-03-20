// Unlit.slang — テクスチャ × カラー、ライティングなし
//
// BindGroup layout:
//   Set 0 (per-view):  ViewParams   { viewProj }
//   Set 1 (per-material): MaterialParams { color } + texture + sampler
//   Set 2 (per-object): ObjectParams { world }

struct ViewParams {
    float4x4 viewProj;
    float4   cameraPos;  // xyz = position
    float4   lightDir;   // xyz = direction (unused in Unlit)
    float4   lightColor; // xyz = color     (unused in Unlit)
    float4   ambientColor;
};

struct MaterialParams {
    float4 color;
};

struct ObjectParams {
    float4x4 world;
    float4x4 normalMatrix; // unused in Unlit but keeps layout consistent
};

[[vk::binding(0, 0)]] ConstantBuffer<ViewParams>     viewParams;
[[vk::binding(0, 1)]] ConstantBuffer<MaterialParams>  materialParams;
[[vk::binding(1, 1)]] Texture2D                       baseTexture;
[[vk::binding(2, 1)]] SamplerState                    baseSampler;
[[vk::binding(0, 2)]] ConstantBuffer<ObjectParams>    objectParams;

struct VSInput {
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD;
    float4 color    : COLOR;
    float4 tangent  : TANGENT;
};

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
