/*
 * 計算はすべて左手座標系で行われる。
 * View 空間なら、奥が Z+, 手前が Z-。
 */

#include <Lumino.fxh>

#define LN_COORD_RH 1

// TODO:
// https://veldrid.dev/articles/backend-differences.html
//const float ln_ClipSpaceNearZ = 0.0;    // DX,Metal,Vulkan.  OpenGL の場合は -1.0
//const float ln_ClipSpaceFarZ = 1.0;


//==============================================================================
// Uniforms

sampler2D _ColorSampler;
sampler2D _NormalAndDepthSampler;
sampler2D _ViewDepthSampler;
sampler2D _MetalRoughSampler;

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

//#define MAX_ITERATIONS 300
//#define MAX_BINARY_SEARCH_ITERATIONS 64
static const float _Iterations = 50.0;                   // maximum ray iterations
static const float _BinarySearchIterations = 4.0;       // maximum binary search refinement iterations
//float _Iterations;
//float _BinarySearchIterations;
static const float _Thickness = 1.0;                    // Z size in camera space of a pixel in the depth buffer = _Thickness
static const float _MaxRayDistance = 20.0;               // maximum distance of a ray
static const float _EdgeDistance = 50.0;
static const float _EdgeExponent = 1.0;
static const float _FadeDistance = 10.0;
static const float _FadeExponent = 1.0;

// LH > far=Z+
// RH > far=Z-
//float GetViewSpaceLinearZ(float2 uv) {
//    return tex2D(_MetalRoughSampler, uv).y;
//}

float3 GetViewPosition(float2 inputUV, float projectedZ) {
    // ClipSpace 上の座標を求める
    float4 clipPosition = float4(LN_UVToClipSpacePosition(inputUV), projectedZ, 1.0);
    //float4 pos = (mul(ln_ProjectionI, clipPosition));
    float4 pos = (mul(clipPosition, ln_ProjectionI));
    return pos.xyz / pos.w;
}

float2 GetUVFromViewSpacePosition(float3 pos) {
    //float4 clip = mul(ln_Projection, float4(pos, 1.0));
    float4 clip = mul(float4(pos, 1.0), ln_Projection);
    return LN_ClipSpacePositionToUV(clip.xy / clip.w);;
}

float3 GetViewNormal(float2 screenPosition)
{
    float3 rgb = tex2D(_NormalAndDepthSampler, screenPosition).xyz;
    return normalize(LN_UnpackNormal(rgb));
}

float GetViewSpaceZ(float2 uv)
{
    const float clipSpaceZ = tex2D(_ViewDepthSampler, uv).r;
    float4 clipPosition = float4(LN_UVToClipSpacePosition(uv), clipSpaceZ, 1.0);
    //float4 pos = (mul(ln_ProjectionI, clipPosition));
    float4 pos = (mul(clipPosition, ln_ProjectionI));
    return pos.z / pos.w;
}

bool RayIntersectsDepth(float z, float2 uv) // 衝突している (zが埋まっている) か
{
    float sceneZ = GetViewSpaceZ(uv);
#ifdef LN_COORD_RH
    float dist = (-z) - (-sceneZ); 
#else
    // 遠い方が Z+.
    // サンプリングポイント z が、シーンのZ より手前にあるときは 0 より小さい。
    // サンプリングポイント z が、シーンのZ より奥あるときは 0 より大きい。
    float dist = z - sceneZ; 
#endif

    return dist > 0.0 && dist < _Thickness;
}

bool TraceCameraSpaceRay(
    float3 rayOrg,
    float3 rayDir,
    float dotNV,
    out float2 hitPixel,
    out float3 hitPoint,
    out float iterationCount)
{
    // どの程度レイを飛ばすか？
    const float rayDist = _MaxRayDistance * (1.0 - saturate(dotNV)*step(0.6,dotNV));

    // Clip to the near plane.
    // クリップ領域外に飛ばしても意味がないので、無駄な計算をしないようにする
#ifdef LN_COORD_RH
    float rayLength = ((rayOrg.z + rayDir.z * rayDist) > -ln_NearClip) ? (-ln_NearClip + rayOrg.z) / rayDir.z : rayDist;
    //float rayLength = ((rayOrg.z + rayDir.z * rayDist) < ln_NearClip) ? (ln_NearClip + rayOrg.z) / rayDir.z : rayDist;
#else
    float rayLength = ((rayOrg.z + rayDir.z * rayDist) < ln_NearClip) ? (ln_NearClip + rayOrg.z) / rayDir.z : rayDist;
#endif
    float3 rayEnd = rayOrg + rayDir * rayLength;

    float3 Q0 = rayOrg;
    float3 Q1 = rayEnd;
    float3 delta = Q1 - Q0;     // 終点 - 開始点
    float3 deltaStep = delta / _Iterations; // 1 iteration でどれだけ進めるか

    float3 Q = Q0;
    float2 P;
    
    // Track ray step and derivatives in a float4 to parallelize
    bool intersect = false;
    float count = 0.0;
    const int MAX_ITERATIONS = (int)_Iterations;
    //[unroll(300)] for (int i=0; i<MAX_ITERATIONS; i++)
    [loop] for (int i=0; i<MAX_ITERATIONS; i++) // [loop]: https://akinow.livedoor.blog/archives/52404331.html
    {
        //if (float(i) >= _Iterations) break;
        if (intersect) break;

        // ViewSpace 上のサンプリング点を進める
        Q += deltaStep;

        // ClipSpace を UV 座標に直す（ここからまた深度をサンプリングする）
        hitPixel = GetUVFromViewSpacePosition(Q);

        intersect = RayIntersectsDepth(Q.z, hitPixel);

        count = float(i);
    }

    // Binary search refinement
    //   衝突点と、前回のサンプリング点の間を Binary search してずれを補う
    if (_BinarySearchIterations > 1.0 && intersect)
    {
        Q -= deltaStep;
        deltaStep /= _BinarySearchIterations;

        float originalStride = _BinarySearchIterations * 0.5;
        float stride = originalStride;

        const int MAX_BINARY_SEARCH_ITERATIONS = (int)_BinarySearchIterations;
        //[unroll(64)] for (int j=0; j<MAX_BINARY_SEARCH_ITERATIONS; j++)
        [loop] for (int i=0; i<MAX_ITERATIONS; i++)
        {
            //if (float(j) >= _BinarySearchIterations) break;

            Q += deltaStep * stride;
            hitPixel = GetUVFromViewSpacePosition(Q);

            originalStride *= 0.5;
            stride = RayIntersectsDepth(Q.z, hitPixel) ? -originalStride : originalStride;
        }
    }

    hitPoint = Q;
    iterationCount = count;

    return intersect;
}

float CalculateAlpha(
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
    alpha *= saturate(1.0 - pow(abs(d / _FadeDistance), _FadeExponent));

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
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};

float4 PS_Main(PS_Input input) : SV_TARGET
{
    const float projectedZ = tex2D(_ViewDepthSampler, input.UV).r;
    //const float linearZ = GetViewSpaceLinearZ(input.UV);

    // URL 先の図の、地面との衝突点と、その法線 (赤矢印)
    // https://qiita.com/mebiusbox2/items/e69ef326b211880d7549#%E8%A1%9D%E7%AA%81%E5%88%A4%E5%AE%9A
    float3 viewPosition = GetViewPosition(input.UV, projectedZ);
    float3 viewNormal   = GetViewNormal(input.UV);
    
 #if 0   // DEBUG: この2つは同じ値にならないとダメ
    // この時点の色を確認すると、右が赤、上が緑になるのが正しい。
    // 拡大して、座標が 0~1のところを確認するとグラデーションしているのがわかる。
    //return float4(viewPosition, 1);
    // viewNormal は左手。手前はZ-なので色がついているようには見えない。
    //return float4(viewNormal, 1);
#endif

#if 0   // DEBUG: この2つは同じ値にならないとダメ
    return float4(
        GetViewSpaceZ( GetUVFromViewSpacePosition(viewPosition)) / 100,
        viewPosition.z / 100,
        0, 1);
#endif

    // 反射の原点と方向ベクトル。
    // rayDir は、視点から飛ばしたベクトルを反射させた方向が入るので、例えば手前向きの垂直の壁を、左斜め前から見ると反射ベクトルは (1, 0, 0) となり、赤く見える
    float3 rayOrg = viewPosition;       // ViewSpace 上での計算であるため、viewPosition=視点からそこへ向かうレイ と考えられる
    float3 rayDir = reflect(normalize(rayOrg), viewNormal);

    float2 hitPixel;
    float3 hitPoint;
    float iterationCount;

    // 視点へ向かうベクトルと、法線の内積。
    // 視点に対して対面するなら 1, 逆向きなら -1
    float dotNV = dot(normalize(-rayOrg), viewNormal);

    bool intersect = TraceCameraSpaceRay(rayOrg, rayDir, dotNV, hitPixel, hitPoint, iterationCount);

    const float4 metalRoughness = tex2D(_MetalRoughSampler, input.UV);
    const float specularStrength = 1.0 - metalRoughness.z;

    float alpha = CalculateAlpha(input.UV, intersect, iterationCount, specularStrength, hitPixel, hitPoint, rayOrg, rayDir);
    
    float3 color = tex2D(_ColorSampler, hitPixel).xyz;

    color = lerp(float3(0.3, 0.3, 0.3), color, projectedZ < 1.0 ? 1.0 : 0.0);

    return float4(color.xyz, alpha);
}

//==============================================================================
// Technique

technique Forward_Geometry_UnLighting
{
    ShadingModel = Unlit;
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}

