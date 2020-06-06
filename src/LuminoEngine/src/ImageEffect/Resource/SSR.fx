
#include <Lumino.fxh>


//==============================================================================
// Uniforms


//==============================================================================
// Vertex shader

struct VS_Input
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD0;
};

struct VS_Output
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};

VS_Output VS_Main(LN_VSInput input)
{
    VS_Output output;
    output.Pos = float4(input.Pos, 1.0);
    output.UV = input.UV;
    return output;
}

//==============================================================================
// Pixel shader

#define MAX_ITERATIONS 300
#define MAX_BINARY_SEARCH_ITERATIONS 64
sampler2D _ColorSampler;
sampler2D _NormalAndDepthSampler;
sampler2D _MetalRoughSampler;
//uniform float4 Resolution;    // xy: BufferSize, zw: InvBufferSize
//float ln_NearClip;
//float ln_FarClip;
float4x4 _CameraProjectionMatrix;        // projection matrix that maps to screen pixels (not NDC)
float4x4 _CameraInverseProjectionMatrix; // inverse projection matrix (NDC to camera space)
const float _Iterations = 50.0;                   // maximum ray iterations
const float _BinarySearchIterations = 4.0;       // maximum binary search refinement iterations
const float _Thickness = 1.0;                    // Z size in camera space of a pixel in the depth buffer = _Thickness
const float _MaxRayDistance = 20.0;               // maximum distance of a ray
const float _EdgeDistance = 50.0;
const float _EdgeExponent = 1.0;
const float _FadeDistance = 10.0;
const float _FadeExponent = 1.0;

float3 transformDirection(in float3 dir, in float4x4 mat) {
  return normalize((mat * float4(dir, 0.0)).xyz);
}

float3 inverseTransformDirection(in float3 dir, in float4x4 mat) {
  return normalize((float4(dir, 0.0) * mat).xyz);
}

float viewZToOrthographicDepth(float viewZ) {
    return (viewZ + ln_NearClip) / (ln_NearClip - ln_FarClip);
}

float perspectiveDepthToViewZ(float invClipZ) {
    return (ln_NearClip*ln_FarClip) / ((ln_FarClip-ln_NearClip)*invClipZ-ln_FarClip);
}

float getDepth(float2 screenPosition) {
    return tex2D(_NormalAndDepthSampler, screenPosition).a;
}

float getLinearDepth(float2 screenPosition) {
    float fragCoordZ = tex2D(_NormalAndDepthSampler, screenPosition).a;
    float nz = ln_NearClip * fragCoordZ;
    return -nz / (ln_FarClip * (fragCoordZ-1.0) - nz);
}

float getViewZ(float depth) {
    return (ln_NearClip*ln_FarClip) / ((ln_FarClip-ln_NearClip)*depth-ln_FarClip);
}

float3 getViewPosition(float2 screenPosition, float depth, float viewZ) {
    float clipW = _CameraProjectionMatrix[2][3] * viewZ + _CameraProjectionMatrix[3][3];
    float4 clipPosition = float4((float3(screenPosition, depth) - 0.5)*2.0, 1.0);
    clipPosition *= clipW; // unprojection
    return (_CameraInverseProjectionMatrix * clipPosition).xyz;
}

float3 getViewNormal(float2 screenPosition) {
    float3 rgb = tex2D(_NormalAndDepthSampler, screenPosition).xyz;
    return 2.0*rgb.xyz - 1.0;
}

//----------------------------------------------------------------------------
bool rayIntersectsDepth(float z, float2 uv)
{
    float sceneZMax = getViewZ(getDepth(uv));
    float dist = z - sceneZMax;
    return dist < 0.0 && dist > -_Thickness;
}

bool traceCameraSpaceRay(
    float3 rayOrg,
    float3 rayDir,
    float dotNV,
    out float2 hitPixel,
    out float3 hitPoint,
    out float iterationCount)
{
    float rayDist = _MaxRayDistance * (1.0 - saturate(dotNV)*step(0.6,dotNV));
    // float rayDist = _MaxRayDistance;

    // Clip to the near plane
    float rayLength = ((rayOrg.z + rayDir.z * rayDist) > -ln_NearClip) ?
        (-ln_NearClip - rayOrg.z) / rayDir.z : rayDist;
    float3 rayEnd = rayOrg + rayDir * rayLength;

    float3 Q0 = rayOrg;
    float3 Q1 = rayEnd;
    float3 delta = Q1 - Q0;
    float3 deltaStep = delta / _Iterations;

    float3 Q = Q0;
    float2 P;
    
    // Track ray step and derivatives in a float4 to parallelize
    bool intersect = false;
    float count = 0.0;
    for (int i=0; i<MAX_ITERATIONS; i++)
    {
        if (float(i) >= _Iterations) break;
        if (intersect) break;

        Q += deltaStep;
        float4 clip = _CameraProjectionMatrix * float4(Q,1.0);
        P = clip.xy / clip.w;

        hitPixel = P.xy*0.5+0.5;
        intersect = rayIntersectsDepth(Q.z, hitPixel);

        count = float(i);
    }

    // Binary search refinement
    if (_BinarySearchIterations > 1.0 && intersect)
    {
        Q -= deltaStep;
        deltaStep /= _BinarySearchIterations;

        float originalStride = _BinarySearchIterations * 0.5;
        float stride = originalStride;

        for (int j=0; j<MAX_BINARY_SEARCH_ITERATIONS; j++)
        {
            if (float(j) >= _BinarySearchIterations) break;

            Q += deltaStep * stride;
            float4 clip = _CameraProjectionMatrix * float4(Q,1.0);
            P = clip.xy / clip.w;

            hitPixel = P.xy*0.5+0.5;

            originalStride *= 0.5;
            stride = rayIntersectsDepth(Q.z, hitPixel) ? -originalStride : originalStride;
        }
    }

    hitPoint = Q;
    iterationCount = count;

    return intersect;
}

float calculateAlpha(
    float2 uv,
    bool intersect,
    float iterationCount,
    float specularStrength,
    float2 hitPixel, 
    float3 hitPoint,
    float3 rayOrg,
    float3 rayDir)
{
    /// Roughness
    float alpha = specularStrength;
    // float alpha = min(1.0, specularStrength + 1.0);

    /// Distance Fade
    // float h = max(0.0, hitPoint.y - viewPosition.y);
    // alpha = saturate(1.0 - pow(h / _FadeDistance, _FadeExponent));
    float d = length(rayOrg - hitPoint);
    alpha *= saturate(1.0 - pow(d / _FadeDistance, _FadeExponent));

    /// ScreenEdge Fade
    float2 edgeuv = uv * (1.0 - uv.yx);
    float edge = edgeuv.x * edgeuv.y * _EdgeDistance;
    edge = saturate(pow(abs(edge), _EdgeExponent));
    alpha *= edge;

    // float uvFactor = 2.0 * length(hitPixel - float2(0.5,0.5));
    // uvFactor *= uvFactor;
    // float edge = max(0.0, 1.0 - uvFactor);

    alpha *= intersect ? 1.0 : 0.0;

    return alpha;
}




struct PS_Input
{
    float2 UV : TEXCOORD0;
};

float4 PS_Main(PS_Input input) : SV_TARGET
{
    float depth = getDepth(input.UV);
    float viewZ = getViewZ(depth);

    float3 viewPosition = getViewPosition(input.UV, depth, viewZ);
    float3 viewNormal   = getViewNormal(input.UV);

    float4 metalRoughness = tex2D(_MetalRoughSampler, input.UV);
    float specularStrength = 1.0 - metalRoughness.z;

    float3 rayOrg = viewPosition;
    float3 rayDir = reflect(normalize(rayOrg), viewNormal);

    float2 hitPixel;
    float3 hitPoint;
    float iterationCount;

    float dotNV = dot(normalize(-rayOrg), viewNormal);
    bool intersect = traceCameraSpaceRay(rayOrg, rayDir, dotNV, hitPixel, hitPoint, iterationCount);
    float alpha = calculateAlpha(input.UV, intersect, iterationCount, specularStrength, hitPixel, hitPoint, rayOrg, rayDir);

    float3 color = tex2D(_ColorSampler, hitPixel).xyz;
    color = lerp(float3(.3), color, depth < 1.0 ? 1.0 : 0.0);

    return float4(color.xyz, alpha);
}

//==============================================================================
// Technique

technique Forward_Geometry_UnLighting
{
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}
