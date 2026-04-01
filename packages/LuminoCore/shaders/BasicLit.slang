// BasicLit.slang — Blinn-Phong ライティング (1 Directional Light)
//
// ParameterBlock layout:
//   Set 0 (per-view):     viewData     — ViewParams (camera)
//   Set 1 (per-scene):    sceneData    — SceneParams (lighting)
//   Set 2 (per-material): materialData — MaterialData (CB + Texture + Sampler)
//   Set 3 (per-object):   objectData   — ObjectParams (implicit CB)

import lumino;

struct MaterialParams {
    float4 color;      // base color (rgba)
    float4 specular;   // xyz = specular color, w = shininess
};

struct MaterialData {
    ConstantBuffer<MaterialParams> params;
    Texture2D                      baseTexture;
    SamplerState                   baseSampler;
};

ParameterBlock<ViewParams>     viewData;       // -> Set 0
ParameterBlock<SceneParams>    sceneData;      // -> Set 1
ParameterBlock<MaterialData>   materialData;   // -> Set 2
ParameterBlock<ObjectParams>   objectData;     // -> Set 3

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
    float4 worldPos = mul(objectData.world, float4(input.position, 1.0));
    output.position  = mul(viewData.viewProj, worldPos);
    output.worldPos  = worldPos.xyz;
    output.worldNorm = normalize(mul((float3x3)objectData.normalMatrix, input.normal));
    output.uv        = input.uv;
    output.color     = input.color;
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_TARGET {
    float4 texColor = materialData.baseTexture.Sample(materialData.baseSampler, input.uv);
    float4 baseColor = texColor * materialData.params.color * input.color;

    float3 N = normalize(input.worldNorm);
    float3 L = normalize(sceneData.lightDir.xyz);
    float3 V = normalize(viewData.cameraPos.xyz - input.worldPos);
    float3 H = normalize(L + V);

    // Diffuse (Lambert)
    float NdotL = max(dot(N, L), 0.0);
    float3 diffuse = baseColor.rgb * sceneData.lightColor.xyz * NdotL;

    // Specular (Blinn-Phong)
    float NdotH = max(dot(N, H), 0.0);
    float shininess = materialData.params.specular.w;
    float3 specular = materialData.params.specular.xyz * sceneData.lightColor.xyz * pow(NdotH, shininess);

    // Ambient
    float3 ambient = baseColor.rgb * sceneData.ambientColor.xyz;

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
