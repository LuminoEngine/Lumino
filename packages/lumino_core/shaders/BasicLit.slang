// BasicLit.slang — Blinn-Phong ライティング (1 Directional Light)
//
// BindGroup layout:
//   Set 0 (per-view):     ViewParams  { viewProj, cameraPos, lightDir, lightColor, ambientColor }
//   Set 1 (per-material): MaterialParams { color, specular, shininess } + texture + sampler
//   Set 2 (per-object):   ObjectParams { world, normalMatrix }

import lumino;

struct MaterialParams {
    float4 color;      // base color (rgba)
    float4 specular;   // xyz = specular color, w = shininess
};

[[vk::binding(0, 0)]] ConstantBuffer<ViewParams>      viewParams;
[[vk::binding(0, 1)]] ConstantBuffer<MaterialParams>   materialParams;
[[vk::binding(1, 1)]] Texture2D                        baseTexture;
[[vk::binding(2, 1)]] SamplerState                     baseSampler;
[[vk::binding(0, 2)]] ConstantBuffer<ObjectParams>     objectParams;

struct VSOutput {
    float4 position  : SV_POSITION;
    float3 worldPos  : WORLD_POS;
    float3 worldNorm : WORLD_NORMAL;
    float2 uv        : TEXCOORD;
    float4 color     : COLOR;
};

[shader("vertex")]
VSOutput vsMain(VSInput input) {
    VSOutput output;
    float4 worldPos = mul(objectParams.world, float4(input.position, 1.0));
    output.position  = mul(viewParams.viewProj, worldPos);
    output.worldPos  = worldPos.xyz;
    output.worldNorm = normalize(mul((float3x3)objectParams.normalMatrix, input.normal));
    output.uv        = input.uv;
    output.color     = input.color;
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_TARGET {
    float4 texColor = baseTexture.Sample(baseSampler, input.uv);
    float4 baseColor = texColor * materialParams.color * input.color;

    float3 N = normalize(input.worldNorm);
    float3 L = normalize(viewParams.lightDir.xyz);
    float3 V = normalize(viewParams.cameraPos.xyz - input.worldPos);
    float3 H = normalize(L + V);

    // Diffuse (Lambert)
    float NdotL = max(dot(N, L), 0.0);
    float3 diffuse = baseColor.rgb * viewParams.lightColor.xyz * NdotL;

    // Specular (Blinn-Phong)
    float NdotH = max(dot(N, H), 0.0);
    float shininess = materialParams.specular.w;
    float3 specular = materialParams.specular.xyz * viewParams.lightColor.xyz * pow(NdotH, shininess);

    // Ambient
    float3 ambient = baseColor.rgb * viewParams.ambientColor.xyz;

    float3 finalColor = ambient + diffuse + specular;
    return float4(finalColor, baseColor.a);
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
