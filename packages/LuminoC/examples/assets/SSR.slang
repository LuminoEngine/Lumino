// SSR.slang — Screen Space Reflection (ray marching + composite)
//
// Full-screen post-process pass.
// Inputs:
//   u_sceneColor      — scene color render target
//   u_depthNormal     — depth+normal render target (from DepthNormal pass)
// Output:
//   Composited scene color with screen-space reflections

import lumino;

struct SSRParams {
    float4 ssrSettings; // x=maxDistance, y=stepSize, z=thickness, w=maxSteps
};

ParameterBlock<ViewParams>     viewData;
ParameterBlock<SceneParams>    sceneData;
ParameterBlock<ObjectParams>   objectData;

uniform ConstantBuffer<SSRParams> u_params;
uniform Texture2D                 u_sceneColor;
uniform SamplerState              u_sceneColorSampler;
uniform Texture2D                 u_depthNormal;
uniform SamplerState              u_depthNormalSampler;

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
    float4 sceneColor = u_sceneColor.Sample(u_sceneColorSampler, input.uv);
    float4 depthNormalSample = u_depthNormal.Sample(u_depthNormalSampler, input.uv);

    //return sceneColor; // debug: show original scene color

    float depth = depthNormalSample.r;
    // Decode normal from [0,1] to [-1,1]
    float3 normal = float3(
        depthNormalSample.g,
        depthNormalSample.b,
        depthNormalSample.a) * 2.0 - 1.0;

    // Skip pixels with no geometry (depth == 0 means nothing was rendered)
    if (depth <= 0.0) {
        return sceneColor;
    }

    // Reconstruct view-space position from depth + UV + projection matrix
    float2 ndc = input.uv * 2.0 - 1.0;
    ndc.y = -ndc.y; // flip Y (UV origin is top-left, NDC origin is bottom-left)

    // Use pre-computed inverse projection to get view-space ray direction
    float4 viewRay = mul(viewData.invProj, float4(ndc, 1.0, 1.0));
    float3 viewDir = viewRay.xyz / viewRay.w;
    float3 viewPos = viewDir * (depth / -viewDir.z);

    // Reflection direction in view space
    float3 N = normalize(normal);
    float3 reflected = reflect(normalize(viewPos), N);

    // SSR parameters
    float stepSize  = u_params.ssrSettings.y;
    float thickness = u_params.ssrSettings.z;
    float maxSteps  = u_params.ssrSettings.w;

    // Ray march in view space, project to screen space each step
    float3 rayPos = viewPos;
    float4 reflectionColor = float4(0.0, 0.0, 0.0, 0.0);

    for (float i = 0.0; i < maxSteps; i += 1.0) {
        rayPos += reflected * stepSize;

        // Project ray position to screen space
        float4 projPos = mul(viewData.proj, float4(rayPos, 1.0));
        projPos.xy /= projPos.w;
        float2 sampleUV = projPos.xy * 0.5 + 0.5;
        sampleUV.y = 1.0 - sampleUV.y;

        // Check bounds
        if (sampleUV.x < 0.0 || sampleUV.x > 1.0 ||
            sampleUV.y < 0.0 || sampleUV.y > 1.0) {
            break;
        }

        // Sample depth at ray position
        float sampledDepth = u_depthNormal.Sample(u_depthNormalSampler, sampleUV).r;
        float rayDepth = -rayPos.z; // positive distance

        // Check if ray hit a surface
        if (rayDepth > sampledDepth && rayDepth - sampledDepth < thickness) {
            float4 hitColor = u_sceneColor.Sample(u_sceneColorSampler, sampleUV);
            // Fade based on ray distance
            float fade = 1.0 - (i / maxSteps);
            // Edge fade (reduce intensity near screen edges)
            float2 edgeFade = smoothstep(0.0, 0.1, sampleUV) * (1.0 - smoothstep(0.9, 1.0, sampleUV));
            float edgeFactor = edgeFade.x * edgeFade.y;
            reflectionColor = float4(hitColor.rgb, fade * edgeFactor * 0.5);
            break;
        }
    }

    // Composite: blend reflection with scene color
    float3 finalColor = lerp(sceneColor.rgb, reflectionColor.rgb, reflectionColor.a);
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
