
#ifndef LUMINO_FORWARD_INCLUDED
#define LUMINO_FORWARD_INCLUDED

//float3        ln_cameraPos;


cbuffer LNClusteredShadingParameters
{

    
// .x : Start distance (distance from camera)
// .y : Lower height
// .z : Upper height
// .w : Height fog Density
    float4    ln_FogParams;

    float4    ln_FogColorAndDensity;

    float3 ln_MainLightDirection;    // ライトの向き。Sun から地表へ向かう。ライトの正面方向


    float        ln_nearClip;
    float        ln_farClip;
}


sampler2D ln_clustersTexture;
//Texture2D ln_clustersTexture;
//SamplerState ln_clustersTextureSamplerState;

sampler2D ln_GlobalLightInfoTexture;
//Texture2D ln_GlobalLightInfoTexture;
//SamplerState ln_GlobalLightInfoTextureSamplerState;

sampler2D ln_pointLightInfoTexture;
//Texture2D ln_pointLightInfoTexture;
//SamplerState ln_pointLightInfoTextureSamplerState;

static float2 LightInfoTextureSize = float2(4, 64);

//------------------------------------------------------------------------------
// 

float4x4        ln_ViewProjection_Light0;

struct LN_VSOutput_ClusteredForward
{
    float3    WorldPos    : TEXCOORD10;
    float3    VertexPos    : TEXCOORD11;
    
    float3    vViewPosition     : TEXCOORD12;    // 頂点位置から視点位置までのベクトル
    float4    vInLightPosition     : TEXCOORD13;
};

struct LN_PSInput_ClusteredForward
{
    float3    WorldPos    : TEXCOORD10;
    float3    VertexPos    : TEXCOORD11;
    
    float3    vViewPosition     : TEXCOORD12;
    float4    vInLightPosition     : TEXCOORD13;
};

// deprecated: see _LN_ProcessVertex_ClusteredForward
LN_VSOutput_ClusteredForward LN_ProcessVertex_ClusteredForward(LN_VSInput input)
{
    LN_VSOutput_ClusteredForward output;
    output.WorldPos = mul(float4(input.Pos, 1.0), ln_World).xyz;
    output.VertexPos = input.Pos;
    
    
    float4 mvPosition = mul(float4(input.Pos, 1.0), ln_WorldView);
    output.vViewPosition  = -mvPosition.xyz;
    
    
    
    float4 pos = mul(float4(input.Pos, 1.0), ln_World);
    pos = mul(pos, ln_ViewProjection_Light0);
    output.vInLightPosition = pos;
    //output.vInLightPosition = mul(mul(float4(input.Pos, 1.0), ln_World), ln_ViewProjection_Light0);
    
    return output;
}

void _LN_ProcessVertex_ClusteredForward(
    LN_VSInput input,
    out float3 outVertexPos,
    out float3 outWorldPos,
    out float3 outViewPos,
    out float4 outvInLightPosition)
{
    float4 worldPos = mul(float4(input.Pos, 1.0), ln_World);

    outVertexPos = input.Pos;
    outWorldPos = worldPos.xyz;
    outViewPos = mul(worldPos, ln_View).xyz;
    outvInLightPosition = mul(worldPos, ln_ViewProjection_Light0);
}

struct GlobalLightInfo
{
    float3    color;    // intensity 適用済み
    float3    groundColor;    // intensity 適用済み
    float4    directionAndType;
};

struct LightInfo
{
    float3    position;
    float    range;
    float    attenuation;
    float3    color;    // intensity 適用済み
    float3    direction;
    float2    spotAngles;
};

void _LN_ClearLightInfo(out LightInfo light)
{
    light.position = float3(0.0, 0.0, 0.0);
    light.range = 0.0;
    light.attenuation = 0.0;
    light.color = float3(0.0, 0.0, 0.0);
    light.direction = float3(0.0, 0.0, 0.0);
    light.spotAngles = float2(0.0, 0.0);
}


GlobalLightInfo _LN_GetGlobalLightInfo(int index)
{
    float2 s = 1.0 / LightInfoTextureSize;
    float y = (float)index;
    y += 0.5;
    GlobalLightInfo info;
    //info.color = tex2D(ln_GlobalLightInfoSampler, (float2(0.0, y) * s));
    //info.groundColor = tex2D(ln_GlobalLightInfoSampler, (float2(1.5, y) * s));
    //info.directionAndType = tex2D(ln_GlobalLightInfoSampler, (float2(2.5, y) * s));

    //info.color = ln_GlobalLightInfoTexture.Sample(ln_GlobalLightInfoTextureSamplerState, (float2(0.0, y) * s));
    //info.groundColor = ln_GlobalLightInfoTexture.Sample(ln_GlobalLightInfoTextureSamplerState, (float2(1.5, y) * s));
    //info.directionAndType = ln_GlobalLightInfoTexture.Sample(ln_GlobalLightInfoTextureSamplerState, (float2(2.5, y) * s));
    
    float4 color = tex2D(ln_GlobalLightInfoTexture, (float2(0.0, y) * s));
    float4 groundColor = tex2D(ln_GlobalLightInfoTexture, (float2(1.5, y) * s));

    info.color = color.rgb * color.a;
    info.groundColor = groundColor.rgb * groundColor.a;
    info.directionAndType = tex2D(ln_GlobalLightInfoTexture, (float2(2.5, y) * s));
    return info;
}

LightInfo _LN_GetLightInfoClusterd(int index)
{
    float2 s = 1.0 / LightInfoTextureSize;
    float y = (float)index;
    y += 0.5;
    //float4 tc0 = float4((0.0 + 0.5f) * s.x, (index + 0.5f) * s.y, 0, 1);    // +0.5 は半ピクセル分
    //float4 tc1 = float4((1.0 + 0.5f) * s.x, (index + 0.5f) * s.y, 0, 1);    // +0.5 は半ピクセル分
    //tc0.y = 1.0 - tc0.y;
    //tc1.y = 1.0 - tc1.y;
    
    //float4 posAndRange = tex2D(pointLightInfoSampler, tc0.xy);
    //float4 spotDirection = tex2D(pointLightInfoSampler, tc1.xy);

    //float4 posAndRange = tex2D(pointLightInfoSampler, (float2(0.0, y) * s));
    //float4 spotDirection = tex2D(pointLightInfoSampler, (float2(1.0, y) * s));
    //float4 spotAngle = tex2D(pointLightInfoSampler, (float2(2.0, y) * s));
    //float4 color = tex2D(pointLightInfoSampler, (float2(3.0, y) * s));

    //float4 posAndRange = ln_pointLightInfoTexture.Sample(ln_pointLightInfoTextureSamplerState, (float2(0.0, y) * s));
    //float4 spotDirection = ln_pointLightInfoTexture.Sample(ln_pointLightInfoTextureSamplerState, (float2(1.0, y) * s));
    //float4 spotAngle = ln_pointLightInfoTexture.Sample(ln_pointLightInfoTextureSamplerState, (float2(2.0, y) * s));
    //float4 color = ln_pointLightInfoTexture.Sample(ln_pointLightInfoTextureSamplerState, (float2(3.0, y) * s));
    
    float4 posAndRange = tex2D(ln_pointLightInfoTexture, (float2(0.0, y) * s));
    float4 spotDirection = tex2D(ln_pointLightInfoTexture, (float2(1.0, y) * s));
    float4 spotAngle = tex2D(ln_pointLightInfoTexture, (float2(2.0, y) * s));
    float4 color = tex2D(ln_pointLightInfoTexture, (float2(3.0, y) * s));

    LightInfo light;
    light.position = posAndRange.xyz;
    light.range = posAndRange.w;
    light.attenuation = spotDirection.w;
    light.color = (color.rgb * color.a);
    light.direction = spotDirection.xyz;
    light.spotAngles = spotAngle.xy;
    
    /*
    float2 uv = 1.0 / LightInfoTextureSize;
    float4 tc0 = float4((0.0 + 0.5f) * uv.x, (index + 0.5f) * uv.y, 0, 1);    // +0.5 は半ピクセル分
    float4 tc1 = float4((1.0 + 0.5f) * uv.x, (index + 0.5f) * uv.y, 0, 1);    // +0.5 は半ピクセル分
    LightInfo light;
    
    tc0.y = 1.0 - tc0.y;
    tc1.y = 1.0 - tc1.y;
    //o.pos = tex2Dlod(m_pointLightInfoSampler, tc0);
    //o.color = tex2Dlod(m_pointLightInfoSampler, tc1);
    light.position = tex2D(pointLightInfoSampler, tc0.xy);
    light.color = tex2D(pointLightInfoSampler, tc1.xy);
    
    light.range = 1;
    light.direction = float3(1, 0, 0);
    light.spotAngles = float2(0, 0);
    */
    return light;
}







// ライト情報の取り出しを抽象化するためのデータ
struct _LN_LocalLightContext
{
    float lightIndices[4]; 
};

// LightClusters の bias の逆変換
float _LN_FlustumClustereDepthBias(float z)
{
    return sqrt(z);
}

#define LN_CLUSTER_WIDTH (16.0)
#define LN_CLUSTER_HEIGHT (16.0)
#define LN_CLUSTER_DEPTH (32.0)
#define LN_CLUSTER_WIDTH_INV (1.0 / 16.0)
#define LN_CLUSTER_HEIGHT_INV (1.0 / 16.0)
#define LN_CLUSTER_DEPTH_INV (1.0 / 32.0)

#define LN_CLUSTER_TEXTURE_WIDTH (LN_CLUSTER_WIDTH * LN_CLUSTER_HEIGHT)
#define LN_CLUSTER_TEXTURE_X_PITCH (1.0 / LN_CLUSTER_WIDTH)

void _LN_InitLocalLightContext(out _LN_LocalLightContext context, float3 vertexPos, float4 viewPos)
{
    float depth = (viewPos.z - ln_nearClip) / (ln_farClip - ln_nearClip);

    float4 vp = mul(float4(vertexPos, 1.0f), ln_WorldViewProjection);
    vp.xyz /= vp.w;
    
    // uv座標系 (0.0 ~ 1.0)
    float cx = (vp.x + 1.0) / 2.0;
    float cy = (vp.y + 1.0) / 2.0;
    float cz = _LN_FlustumClustereDepthBias(depth);

    float vy = floor(cy * LN_CLUSTER_HEIGHT);    // 0.0~16.0
    float tx = (vy * LN_CLUSTER_TEXTURE_X_PITCH) + (cx * LN_CLUSTER_TEXTURE_X_PITCH);

    //float4 cluster = ln_clustersTexture.Sample(ln_clustersTextureSamplerState, float3(cx, cy, cz));
    //float4 cluster = ln_clustersTexture.Sample(ln_clustersTextureSamplerState, float2(tx, cz));
    float4 cluster = tex2D(ln_clustersTexture, float2(tx, cz));
    float lightIndices[4] = {cluster.r, cluster.g, cluster.b, cluster.a};
    context.lightIndices = lightIndices;
}

bool _LN_GetLocalLightInfo(_LN_LocalLightContext context, float index, out LightInfo light)
{
    if (context.lightIndices[index] > 0.0) {
        light = _LN_GetLightInfoClusterd((context.lightIndices[index] * 255) + 0.5 - 1);
        return true;
    }
    else {
        _LN_ClearLightInfo(light);
        return false;
    }
}


//------------------------------------------------------------------------------
// Fog library

#if 0
float _LN_CalcFogFactor(float depth)
{
    float f = ln_FogParams.a * depth;
    return exp2(-f);
    //return exp2(-f*f);
}

float smoothstep(float a, float b, float x)
{
  float t = saturate((x - a)/(b - a));
  return t * t * (3.0 - (2.0 * t));
}
#endif

// UserProgram に公開する API としては、ベースカラーと Fog は分けたほうがよさそう。
// Unity もそんな感じ。https://light11.hatenadiary.com/entry/2019/04/04/214958
// return: fog density (0.0~)
float _LN_CalcFogFactor2(float depth, float samplePointY)
{
    float lower = ln_FogParams.y;//-30.0;
    float upper = ln_FogParams.z;//100.0;
    float lengthH = ln_FogParams.x;//5.0;    // 水平方向、どのあたりから Fog を生成するか
    float fogDensity = ln_FogColorAndDensity.a;//1.0 / 10.0;// 1.0;    // Fog の濃さ。1.0 の場合、2^(x*1.0) で、座標1進むと完全に Fog になる。
                                // 0.5 だと座標2、0.25だと座標4という具合に、逆数で入力すると距離を測ることができる。

    // g(x)=-2^(x*x)+2
    float halfVolumeHeight = (upper - lower) / 2;
    float center = lower + halfVolumeHeight;
    //float d = abs(samplePointY - center) / halfVolumeHeight;    // Fog volume 中央からどれだけ離れているか (0.0中央 1.0:上端または下端 1.0~:Fogの外)
    //float density = -exp2(d * 0.5) + 2;
    //float density = -exp2(d*d) + 2;
    //float d = 1.0 - (abs(samplePointY - center) / halfVolumeHeight);    // Fog volume 外側境界からどれだけ離れているか (0.0:上端または下端, 1.0:中央 1.0~)
    float d = (halfVolumeHeight - abs(samplePointY - center));// / lengthH;
    //d = saturate(d);
    float densityV = exp2(d * ln_FogParams.w) - 1.0;
    //return saturate(densityV);

    // h(x)=2^((x+2)*0.5)
    float f = ((depth - lengthH));    // 0.0:視点位置, 1.0:フォグの出始め
    //ln_FogParams.a * depth;
    
    float densityH = exp2(f * fogDensity) - 1.0;

    // Input-density が小さいと境界が少し不自然に明るくなるが、ほとんどのケースでは問題ないのでこのままとする。
    // TODO: 何かいい補完方法思いついたら修正するかも。
    //return saturate(densityH) * saturate(densityV);
    return saturate(densityH) + (saturate(densityH) * saturate(densityV));

    //float mu = saturate(densityH * densityV);
    //return saturate(mu * saturate(densityV));

    //return saturate(max(densityH, 0.0) * densityV);
    //return saturate(smoothstep(0.1, 0.9, densityH) * densityV);
    //return lerp(saturate(densityH * densityV), densityV, saturate(densityH));
    //return saturate(densityH * densityV);

    
    //return saturate(densityH) * saturate(densityV);

    //return saturate(densityH);

    //return saturate(density);


    //float lower = -5.0;
    //float upper = 0.0;
    //float d =  abs(height - lerp(upper, lower, 0.5));
    //float f = 0.5 * d;
    //return exp2(-f);
    //return exp2(-f*f);
}

/**
 * 指定視されたベースカラーとワールド位置をもとに、フォグを適用した色を返します。
 */
float3 LN_ApplyFog(float3 color, float3 worldPos)
{
    const float viewLength = length(ln_CameraPosition - worldPos);
    const float fogFactor = _LN_CalcFogFactor2(viewLength, worldPos.y);
    return lerp(color, ln_FogColorAndDensity.rgb, fogFactor);
}

#endif // LUMINO_FORWARD_INCLUDED
