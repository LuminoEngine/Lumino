
#include <Lumino.fxh>


//==============================================================================
// Lib

// TODO: postproces で描くと、0.0~1.0 になってしまうのどうするか考える
//#define CameraNear (0.3)    // ln_NearClip
//#define CameraFar (100.0)    // ln_FarClip
const float CameraNear = 0.3;
const float CameraFar = 1000.0;

// https://veldrid.dev/articles/backend-differences.html
const float ln_ClipSpaceNearZ = 0.0;    // DX,Metal,Vulkan.  OpenGL の場合は -1.0
const float ln_ClipSpaceFarZ = 1.0;

float2 LN_ClipSpacePositionToUV(float2 pos)
{
    //return pos.xy*0.5+0.5;
    return (float2(1.0, -1.0) * pos) * 0.5 + 0.5;
    //return (float2(-1.0, -1.0) * pos) * 0.5 + 0.5;
}

float2 LN_UVToClipSpacePosition(float2 pos)
{
    return ((pos - 0.5) * 2.0) * float2(1.0, -1.0);
}

float LN_ViewSpaceZToClipSpaceZ(float z)
{
    // near:0.0 ~ far:1.0
    const float tz = (z - CameraNear) / (CameraFar - CameraNear);

    // To native ClipSpace
    return (tz * (ln_ClipSpaceFarZ - ln_ClipSpaceNearZ) + ln_ClipSpaceNearZ);
}

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



float _Projection23;
float _Projection33;

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
    return (viewZ + CameraNear) / (CameraNear - CameraFar);
}

float perspectiveDepthToViewZ(float invClipZ) {
    //return (CameraNear*CameraFar) / ((CameraFar-CameraNear)*invClipZ-CameraFar);
    return -((CameraNear*CameraFar) / ((CameraFar-CameraNear)*invClipZ-CameraFar));
}

// depth: near:0~far:1
//float getDepth(float2 screenPosition) {
    //return tex2D(_NormalAndDepthSampler, screenPosition).a;
//    return tex2D(_MetalRoughSampler, screenPosition).y;
//}

// LH. far=Z+
float getViewSpaceLinearZ(float2 uv) {
    return tex2D(_MetalRoughSampler, uv).y;
    //return ((CameraFar-CameraNear)*depth-CameraFar);
    //return (CameraNear*CameraFar) / ((CameraFar-CameraNear)*depth-CameraFar);
}

float getViewSpaceLinearZTotViewSpaceZ(float linearZ) {
    return (linearZ * (CameraFar - CameraNear)) + CameraNear;
}


float3 getViewPosition(float2 inputUV, float projectedZ, float linearZ) {
    // ClipSpace 上の座標を求める
    float4 clipPosition = float4(LN_UVToClipSpacePosition(inputUV), projectedZ, 1.0);
    //return clipPosition.xyz;

#if 0
    // Projection 逆変換
    //   _Projection23: 右手の場合は -1, 左手の場合は 1 になる。
    //   _Projection33: Perspective の場合は 0.0, Ortho の場合は 1.0 になる。
    const float viewSpaceZ = getViewSpaceLinearZTotViewSpaceZ(linearZ);
    float clipW = _Projection23 * viewSpaceZ + _Projection33;
    clipPosition *= clipW; // unprojection
    //return clipPosition.xyz;
    float3 pos = (mul(clipPosition, _CameraInverseProjectionMatrix)).xyz;
    pos.z *= -1.0;
    return pos;
#else
    float4 pos = (mul(_CameraInverseProjectionMatrix, clipPosition));
    float3 p = pos.xyz / pos.w;
    //p.z *= -1.0;
    return p;
#endif

    /*
    screenPosition.y = (-screenPosition.y) + 1.0;
    // _CameraProjectionMatrix[2][3] は 右手か左手かの符号。Matrix::makePerspectiveFovLH() 参考。
    // 
    // [3][3] は Perspective の場合は 0.0, Ortho の場合は 1.0 になる。
    //float clipW = _CameraProjectionMatrix[2][3] * viewZ + _CameraProjectionMatrix[3][3];
    float clipW = _Projection23 * viewZ + _Projection33;
    float4 clipPosition = float4((float3(screenPosition, depth) - 0.5)*2.0, 1.0);
    clipPosition *= clipW; // unprojection
    //return clipPosition.xyz;
    return (mul(clipPosition, _CameraInverseProjectionMatrix)).xyz;
    */
}

float2 getUVFromViewSpacePosition(float3 pos) {
    //float4 clip = mul(float4(pos * float3(1, 1, -1), 1.0), _CameraProjectionMatrix);
    //float2 uv = (clip.xy / clip.w);
    //return float4(fmod(uv, 1.0), 0, 1);
    float4 clip = mul(_CameraProjectionMatrix, float4(pos, 1.0));

    return LN_ClipSpacePositionToUV(clip.xy / clip.w);;
}

float3 getViewNormal(float2 screenPosition) {
    float3 rgb = tex2D(_NormalAndDepthSampler, screenPosition).xyz;
    float3 n = 2.0*rgb.xyz - 1.0;
    return normalize(n);
}

float getViewSpaceZ(float2 uv) {
#if 1
    const float clipSpaceZ = tex2D(_NormalAndDepthSampler, uv).a;
    float4 clipPosition = float4(LN_UVToClipSpacePosition(uv), clipSpaceZ, 1.0);
    float4 pos = (mul(_CameraInverseProjectionMatrix, clipPosition));
    return pos.z / pos.w;
#endif
    //return getViewSpaceLinearZTotViewSpaceZ(getViewSpaceLinearZ(uv));
    //const float projectedZ = tex2D(_NormalAndDepthSampler, uv).a;
    //return (CameraNear*CameraFar) / ((CameraFar-CameraNear)*projectedZ-CameraFar);
#if 0
    const float projectedZ = tex2D(_NormalAndDepthSampler, uv).a;
    float4 clipPosition = float4(LN_UVToClipSpacePosition(uv), projectedZ, 1.0);


    // m14 = 0,
    // m24 = 0,
    // m34 = 1 of -1,
    // m44 = 0 of 1,

    float w = 1.0f / ((vec.z * mat.m34) + mat.m44);

    return Vector3(
            ((vec.x * mat.m11) + (vec.y * mat.m21) + (vec.z * mat.m31) + mat.m41) * w,
            ((vec.x * mat.m12) + (vec.y * mat.m22) + (vec.z * mat.m32) + mat.m42) * w,
            ((vec.x * mat.m13) + (vec.y * mat.m23) + (vec.z * mat.m33) + mat.m43) * w);

#endif

#if 1   // 愚直のとりあえず成功版
    const float linearZ = getViewSpaceLinearZ(uv);
    const float projectedZ = tex2D(_NormalAndDepthSampler, uv).a;
    // URL 先の図の、地面との衝突点と、その法線 (赤矢印)
    // https://qiita.com/mebiusbox2/items/e69ef326b211880d7549#%E8%A1%9D%E7%AA%81%E5%88%A4%E5%AE%9A
    float3 viewPosition = getViewPosition(uv, projectedZ, linearZ);
    return viewPosition.z;
#endif

    //const float projectedZ = tex2D(_NormalAndDepthSampler, uv).a;
    //return projectedZ;
    //return perspectiveDepthToViewZ(projectedZ);
}

//----------------------------------------------------------------------------
bool rayIntersectsDepth(float z, float2 uv)
{
    float sceneZ = getViewSpaceZ(uv);

    // 遠い方が Z+.
    // サンプリングポイント z が、シーンのZ より手前にあるときは 0 より小さい。
    // サンプリングポイント z が、シーンのZ より奥あるときは 0 より大きい。
    float dist = z - sceneZ; 

    return dist > 0.0 && dist < _Thickness;
    //return dist < 0.0;// && dist < _Thickness;
    
    //float dist = z - sceneZMax;
    //return dist < 0.0 && dist > -Thickness;
}

bool traceCameraSpaceRay(
    float3 rayOrg,
    float3 rayDir,
    float dotNV,
    out float2 hitPixel,
    out float3 hitPoint,
    out float iterationCount)
{
    // どの程度レイを飛ばすか？
    const float rayDist = _MaxRayDistance * (1.0 - saturate(dotNV)*step(0.6,dotNV));
    //float rayDist = _MaxRayDistance;

    // Clip to the near plane
    // クリップ領域外に飛ばしても意味がないので、無駄な計算をしないようにする
    //float rayLength = ((rayOrg.z + rayDir.z * rayDist) > -CameraNear) ?
    //    (-CameraNear - rayOrg.z) / rayDir.z : rayDist;
    float rayLength = rayDist;
    float3 rayEnd = rayOrg + rayDir * rayLength;

    float3 Q0 = rayOrg;
    float3 Q1 = rayEnd;
    float3 delta = Q1 - Q0;     // 終点 - 開始点
    float3 deltaStep = delta / _Iterations; // 1 iteration でどれだけ進めるか\

    float3 Q = Q0;
    float2 P;

    if (false){
        hitPixel.x = getViewSpaceZ( getUVFromViewSpacePosition(Q));// / 100;
        //hitPixel.x = Q.z / 100;
        return true;
    }
    
    // Track ray step and derivatives in a float4 to parallelize
    bool intersect = false;
    float count = 0.0;
    for (int i=0; i<MAX_ITERATIONS; i++)
    {
        if (float(i) >= _Iterations) break;
        if (intersect) break;

        // ViewSpace 上のサンプリング点を進める
        Q += deltaStep;

        // ViewSpace の Q を ClipSpace に変換する。
        //float4 clip = mul(float4(Q,1.0), _CameraProjectionMatrix);
        //P = clip.xy / clip.w;

        // ClipSpace を UV 座標に直す（ここからまた深度をサンプリングする）
        //hitPixel = LN_ClipSpacePositionToUV(P.xy);
        hitPixel = getUVFromViewSpacePosition(Q);

        intersect = rayIntersectsDepth(Q.z, hitPixel);

        //hitPixel.x = getViewSpaceZ(hitPixel) / 100;
        //hitPixel.x = Q.z / 1000;
        //intersect = true;
        //hitPoint = 
        //hitPixel.x = getDepth(hitPixel);// / 10.0;
        //hitPixel.x = -0.01 * getViewZ(getDepth(hitPixel));// / 100.0;

        count = float(i);
    }

    // Binary search refinement
    //if (_BinarySearchIterations > 1.0 && intersect)
    if (false)
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

            //hitPixel = P.xy*0.5+0.5;
            hitPixel = LN_ClipSpacePositionToUV(P.xy);

            originalStride *= 0.5;
            stride = rayIntersectsDepth(Q.z, hitPixel) ? -originalStride : originalStride;
        }
    }

    hitPoint = Q;
    iterationCount = count;

    //hitPixel.x = length(deltaStep);
    //hitPixel.y = length(delta) / 50.0;
    //hitPixel.x = rayLength / 40.0;
    //hitPixel.x = length(delta) / 40.0;
    //hitPoint = normalize(delta);

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
    //return float4(0, getDepth(input.UV), 0, 1);

    const float projectedZ = tex2D(_NormalAndDepthSampler, input.UV).a;

    const float linearZ = getViewSpaceLinearZ(input.UV);

    


    //return float4(linearZ, 0, 0, 1);
    //return float4(CameraNear, CameraFar / 200, 0, 1);
//    return float4(-_CameraProjectionMatrix[2][3] / 2, 0, 0, 1);

    // URL 先の図の、地面との衝突点と、その法線 (赤矢印)
    // https://qiita.com/mebiusbox2/items/e69ef326b211880d7549#%E8%A1%9D%E7%AA%81%E5%88%A4%E5%AE%9A
    float3 viewPosition = getViewPosition(input.UV, projectedZ, linearZ);
    float3 viewNormal   = getViewNormal(input.UV);
    // この時点の色を確認すると、右が赤、上が緑になるのが正しい。
    // 拡大して、座標が 0~1のところを確認するとグラデーションしているのがわかる。
    //return float4(viewPosition, 1);
    //return float4(fmod(viewPosition, 1.0), 1);
    //return float4(viewPosition / 10.0, 1);
    // viewNormal は左手。手前はZ-なので色がついているようには見えない。
    //return float4(viewNormal, 1);

    //float4 clip = mul(float4(viewPosition * float3(1, 1, -1), 1.0), _CameraProjectionMatrix);
    //float2 uv = (clip.xy / clip.w);
    //return float4(fmod(uv, 1.0), 0, 1);
    //float2 uv = LN_ClipSpacePositionToUV(clip.xy / clip.w);
    //float2 uv = getUVFromViewSpacePosition(viewPosition);
    //return float4(getUVFromViewSpacePosition(viewPosition), 0, 1);

 #if 0   // この2つは同じ値にならないとダメ
    return float4(
        getViewSpaceZ( getUVFromViewSpacePosition(viewPosition)) / 100,
        viewPosition.z / 100,
        0, 1);
#endif

    // 反射の原点と方向ベクトル。
    // rayDir は、視点から飛ばしたベクトルを反射させた方向が入るので、例えば手前向きの垂直の壁を、左斜め前から見ると反射ベクトルは (1, 0, 0) となり、赤く見える
    float3 rayOrg = viewPosition;
    float3 rayDir = reflect(normalize(rayOrg), viewNormal);
    //return float4(rayOrg, 1);
    //return float4(viewNormal, 1);
    //return float4(rayDir, 1);

    float2 hitPixel;
    float3 hitPoint;
    float iterationCount;

    // 視点へ向かうベクトルと、法線の内積。
    // 視点に対して対面するなら 1, 逆向きなら -1
    float dotNV = dot(normalize(-rayOrg), viewNormal);

    bool intersect = traceCameraSpaceRay(rayOrg, rayDir, dotNV, hitPixel, hitPoint, iterationCount);

    //return float4(hitPoint, 1);
    //return float4((intersect ? 1 : 0), 0, 0, 1);
    //return float4(dotNV, 0, 0, 1);
    //return float4(hitPixel.x, 0, 0, 1);
    //return float4(hitPixel.x, hitPixel.y, 0, 1);
    //return float4(iterationCount / _Iterations, 0, 0, 1);
    //return float4(hitPoint.xy, -hitPoint.z, 1);
    
    
    const float4 metalRoughness = tex2D(_MetalRoughSampler, input.UV);
    const float specularStrength = 1.0 - metalRoughness.z;

    float alpha = calculateAlpha(input.UV, intersect, iterationCount, specularStrength, hitPixel, hitPoint, rayOrg, rayDir);
    
    float3 color = tex2D(_ColorSampler, hitPixel).xyz;

    color = lerp(float3(.3), color, linearZ < 1.0 ? 1.0 : 0.0);

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
