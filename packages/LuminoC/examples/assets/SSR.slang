// SSR.slang — Screen Space Reflection (ray marching + PBR composite)
//
// Full-screen post-process pass.
// Inputs (new GBuffer spec, see GBuffer.slang):
//   u_gbufferA — [Albedo.rgb, _]            (RGBA8_UNORM)
//   u_gbufferB — [WorldNormal.xyz, Depth]   (RGBA32_FLOAT)
//                Depth = -viewPos.z (linear view-space distance, +Z behind camera)
//   u_gbufferC — [Metallic, Roughness, AO, _] (RGBA8_UNORM, glTF Metal-Roughness)
// Output:
//   Composited scene color with screen-space reflections

import lumino;

struct SSRParams {
    float4 ssrSettings; // x=maxDistance, y=stepSize, z=thickness, w=maxSteps
};

uniform ConstantBuffer<SSRParams> u_params;
uniform Texture2D                 u_gbufferA;
uniform SamplerState              u_gbufferASampler;
uniform Texture2D                 u_gbufferB;
uniform SamplerState              u_gbufferBSampler;
uniform Texture2D                 u_gbufferC;
uniform SamplerState              u_gbufferCSampler;

struct VSOutput {
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD;
};

[shader("vertex")]
VSOutput vsMain(VSInput input) {
    VSOutput output;
    output.position = float4(input.position, 1.0);
    output.uv = input.uv;
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_TARGET {
    float4 sceneColor = u_gbufferA.Sample(u_gbufferASampler, input.uv);
    float4 gbB        = u_gbufferB.Sample(u_gbufferBSampler, input.uv);
    float4 gbC        = u_gbufferC.Sample(u_gbufferCSampler, input.uv);

    float3 worldNormal = gbB.xyz;
    float  depth       = gbB.w;

    // Skip pixels with no geometry (cleared depth = 0)
    if (depth <= 0.0) {
        return sceneColor;
    }

    float metallic  = gbC.r;
    float roughness = gbC.g;

    // Fully rough surfaces have no usable specular reflection — skip ray march.
    if (roughness >= 1.0) {
        return sceneColor;
    }

    // Reconstruct view-space position from depth + UV + inverse projection.
    float2 ndc = input.uv * 2.0 - 1.0;
    ndc.y = -ndc.y; // UV origin is top-left, NDC origin is bottom-left
    float4 viewRay = mul(viewData.invProj, float4(ndc, 1.0, 1.0));
    float3 viewDir = viewRay.xyz / viewRay.w;
    // depth = -viewPos.z, viewDir.z < 0 (camera looks along -Z)
    float3 viewPos = viewDir * (depth / -viewDir.z);

    // Convert world-space normal to view space for the reflection math.
    float3 N = normalize(mul((float3x3)viewData.view, worldNormal));
    float3 V = normalize(-viewPos);
    float3 reflected = reflect(-V, N);

    // SSR ray-march parameters
    float stepSize  = u_params.ssrSettings.y;
    float thickness = u_params.ssrSettings.z;
    float maxSteps  = u_params.ssrSettings.w;

    // PBR: F0 (Metal-Roughness workflow, glTF 2.0)
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), sceneColor.rgb, metallic);
    float NdotV = saturate(dot(N, V));
    float3 fresnel = F0 + (1.0 - F0) * pow(1.0 - NdotV, 5.0);
    float roughnessFade = 1.0 - roughness;

    // Ray march in view space, project to screen each step.
    float3 rayPos = viewPos;
    float3 reflectionRGB = float3(0.0, 0.0, 0.0);
    float  reflectionStrength = 0.0;

    for (float i = 0.0; i < maxSteps; i += 1.0) {
        rayPos += reflected * stepSize;

        float4 projPos = mul(viewData.proj, float4(rayPos, 1.0));
        projPos.xy /= projPos.w;
        float2 sampleUV = projPos.xy * 0.5 + 0.5;
        sampleUV.y = 1.0 - sampleUV.y;

        if (sampleUV.x < 0.0 || sampleUV.x > 1.0 ||
            sampleUV.y < 0.0 || sampleUV.y > 1.0) {
            break;
        }

        float sampledDepth = u_gbufferB.Sample(u_gbufferBSampler, sampleUV).w;
        float rayDepth = -rayPos.z;

        if (rayDepth > sampledDepth && rayDepth - sampledDepth < thickness) {
            float3 hitColor = u_gbufferA.Sample(u_gbufferASampler, sampleUV).rgb;
            float fade = 1.0 - (i / maxSteps);
            float2 edgeFade = smoothstep(0.0, 0.1, sampleUV) *
                              (1.0 - smoothstep(0.9, 1.0, sampleUV));
            float edgeFactor = edgeFade.x * edgeFade.y;
            reflectionRGB = hitColor;
            reflectionStrength = fade * edgeFactor;
            break;
        }
    }

    float3 reflection = reflectionRGB * fresnel * roughnessFade * reflectionStrength;
    float3 finalColor = sceneColor.rgb + reflection;
    return float4(finalColor, 1.0);
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
