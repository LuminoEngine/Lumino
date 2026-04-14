// DepthNormal.slang — View-space linear depth + normal output
//
// Renders view-space linear depth to R channel and encoded normal to GBA.
// Output format: RGBA16Float
//   R  = linear depth (positive distance from camera)
//   GBA = view-space normal (encoded: n * 0.5 + 0.5)

import lumino;

struct MaterialParams {
    float4 color;
};

ParameterBlock<ViewParams>     viewData;
ParameterBlock<SceneParams>    sceneData;
ParameterBlock<ObjectParams>   objectData;

uniform ConstantBuffer<MaterialParams> u_params;
uniform Texture2D                      u_baseTexture;
uniform SamplerState                   u_baseSampler;

struct VSOutput {
    float4 position  : SV_POSITION;
    float3 viewPos   : VIEW_POS;
    float3 viewNorm  : VIEW_NORMAL;
};

[shader("vertex")]
VSOutput vsMain(VSInput input) {
    VSOutput output;
    float4 worldPos = mul(objectData.world, float4(input.position, 1.0));
    float4 viewPos4 = mul(viewData.view, worldPos);
    output.position = mul(viewData.proj, viewPos4);
    output.viewPos  = viewPos4.xyz;
    // Transform normal to view space
    float3 worldNorm = normalize(mul((float3x3)objectData.normalMatrix, input.normal));
    output.viewNorm = normalize(mul((float3x3)viewData.view, worldNorm));
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_TARGET {
    float linearDepth = -input.viewPos.z; // negate: camera looks along -Z in view space
    float3 N = normalize(input.viewNorm);
    // Encode normal from [-1,1] to [0,1]
    return float4(linearDepth, N.x * 0.5 + 0.5, N.y * 0.5 + 0.5, N.z * 0.5 + 0.5);
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
