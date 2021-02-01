#ifndef LUMINO_NORMAL_MAP_INCLUDED
#define LUMINO_NORMAL_MAP_INCLUDED

//==============================================================================
//

sampler2D ln_NormalMap;

//==============================================================================
//

void _LN_ProcessVertex_NormalMap(
    LN_VSInput input, float3 viewNormal,
    out float3 outTangent, out float3 outBitangent)
{
    const float3 objectTangent = input.tangent.xyz;
    //const float3 transformedTangent = ( ln_WorldView * float4( objectTangent, 0.0 ) ).xyz;
    const float3 transformedTangent = mul(float4(objectTangent, 0.0), ln_WorldView).xyz;
    const float3 tangent = normalize( transformedTangent );
    outTangent = tangent;
    outBitangent= normalize( cross( viewNormal, tangent) * input.tangent.w );
    //output.Debug = objectTangent;
/*
    float3 eyePosition = ln_CameraPosition;
    float3 lightPosition = float3(1, 1, -1);

    float3 invEye   = (ln_WorldI * float4(eyePosition, 0.0)).xyz;
    float3 invLight = (ln_WorldI * float4(lightPosition, 0.0)).xyz;
    float3 eye      = invEye - common.svPos.xyz;
    float3 light    = invLight - common.svPos.xyz;

    // ローカル空間上での接空間ベクトルの方向を求める
    float3 normal = common.Normal;
    float3 n = normalize(normal);
    float3 t = normalize(cross(normal, float3(0.0, 1.0, 0.0)));
    float3 b = cross(n, t);

    // 視線ベクトルとライトベクトルを接空間上に変換
    float3 vEyeDirection;
    vEyeDirection.x   = dot(t, eye);
    vEyeDirection.y   = dot(b, eye);
    vEyeDirection.z   = dot(n, eye);
    output.vEyeDirection = normalize(vEyeDirection);
    float3 vLightDirection;
    vLightDirection.x = dot(t, light);
    vLightDirection.y = dot(b, light);
    vLightDirection.z = dot(n, light);
    output.vLightDirection = normalize(vLightDirection);
    */
}

float3 LN_GetPixelNormalFromNormalMap(float2 texUV, float3 viewSpaceTangent, float3 viewSpaceBitangent, float3 viewSpaceNormal)
{
    const float3 tangent = normalize(viewSpaceTangent);
    const float3 bitangent = normalize(viewSpaceBitangent);
    const float3 normal = normalize(viewSpaceNormal);
    const float3 mapNormal = LN_UnpackNormal(tex2D(ln_NormalMap, texUV).rgb);

    const float3x3 TBN = float3x3(tangent, bitangent, normal);
    return mul(mapNormal, TBN);

    /*
    const float3 normal = normalize(float3(
        dot(tangent, mapNormal),
        dot(bitangent, mapNormal),
        dot(normal, mapNormal)));
    const float3 normal = normalize(float3(
        dot(mapNormal, tangent),
        dot(mapNormal, bitangent),
        dot(mapNormal, normal)));
    */
}

#endif // LUMINO_NORMAL_MAP_INCLUDED
