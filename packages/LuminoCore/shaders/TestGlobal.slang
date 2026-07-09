// TestGlobal.slang - Test shader for $Global + ParameterBlock coexistence
//
// Purpose: Verify Slang set/binding assignment when mixing ParameterBlock
// and bare uniforms ($Global).

import lumino;

// --- Material parameters (bare uniforms → $Global) ---
struct MaterialParams {
    float4 color;
    float4 specular;
};

uniform ConstantBuffer<MaterialParams> u_params;
uniform Texture2D    u_baseTexture;
uniform SamplerState u_baseSampler;
uniform float        u_time;

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
    float4 texColor = u_baseTexture.Sample(u_baseSampler, input.uv);
    float4 baseColor = texColor * u_params.color * input.color;

    float3 N = normalize(input.worldNorm);
    float3 L = normalize(sceneData.lightDir.xyz);
    float3 V = normalize(viewData.cameraPos.xyz - input.worldPos);
    float3 H = normalize(L + V);

    float NdotL = max(dot(N, L), 0.0);
    float3 diffuse = baseColor.rgb * sceneData.lightColor.xyz * NdotL;

    float NdotH = max(dot(N, H), 0.0);
    float shininess = u_params.specular.w;
    float3 specular = u_params.specular.xyz * sceneData.lightColor.xyz * pow(NdotH, shininess);

    float3 ambient = baseColor.rgb * sceneData.ambientColor.xyz;

    float timeEffect = sin(u_time) * 0.1 + 0.9;
    float3 finalColor = (ambient + diffuse + specular) * timeEffect;
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
