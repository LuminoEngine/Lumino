
#ifndef LUMINOSKINNING_INCLUDED
#define LUMINOSKINNING_INCLUDED

// test
const float     MMM_modelsize = 1.0;


// ボーン用頂点テクスチャサンプラー宣言
sampler2D ln_BoneTexture;
//Texture2D ln_BoneTexture;
//SamplerState ln_BoneTextureSamplerState;
/*
sampler2D ln_BoneSampler = sampler_state
{
    Texture = <ln_BoneTexture>;
    MinFilter = Point;  // 殆どのGPUでは以下のようなステート設定にしないと
    MagFilter = Point;  // 頂点テクスチャのフェッチがうまくいかない
    MipFilter = None;
    AddressU = Clamp;
    AddressV = Clamp;
};
*/

sampler2D ln_BoneLocalQuaternionTexture;
//Texture2D ln_BoneLocalQuaternionTexture;
//SamplerState ln_BoneLocalQuaternionTextureSamplerState;
/*
sampler2D ln_BoneLocalQuaternionSampler = sampler_state
{
    Texture = <ln_BoneLocalQuaternionTexture>;
    MinFilter = Point; 
    MagFilter = Point;
    MipFilter = None;
    AddressU = Clamp;
    AddressV = Clamp;
};
*/

struct MMM_SKINNING_INPUT
{
    float4    Pos                : POSITION;
    float4    BlendWeight        : BLENDWEIGHT;
    float4    BlendIndices    : BLENDINDICES;
    float4    Color            : COLOR0;        // 独自
    float3    Normal            : NORMAL;
    float2    Tex                : TEXCOORD0;
    float4    AddUV1            : TEXCOORD1;
    float4    AddUV2            : TEXCOORD2;
    float4    AddUV3            : TEXCOORD3;
    float4    AddUV4            : TEXCOORD4;
    float4    SdefC            : TEXCOORD5;
    float3    SdefR0            : TEXCOORD6;
    float3    SdefR1            : TEXCOORD7;
    float    EdgeWeight        : TEXCOORD8;
    float    Index            : PSIZE15;
};

struct MMM_SKINNING_OUTPUT
{
    float4    Position;
    float3    Normal;
};

struct LN_SkinningOutput
{
    float3    Position;
    float3    Normal;
};

float4x3 LN_GetBoneMatrix(int boneIndex)
{
    float2 uv = ln_BoneTextureReciprocalSize.xy;
    float4 tc0 = float4( 0.5f * uv.x, (boneIndex + 0.5f) * uv.y, 0, 1 );    // +0.5 は半ピクセル分
    float4 tc1 = float4( 1.5f * uv.x, (boneIndex + 0.5f) * uv.y, 0, 1 );
    float4 tc2 = float4( 2.5f * uv.x, (boneIndex + 0.5f) * uv.y, 0, 1 );
    //float4 tc3 = float4( 3.5f * uv.x, (boneIndex + 0.5f) * uv.y, 0, 1 );
    
    //return float4x3(
    //    ln_BoneTexture.SampleLevel(ln_BoneTextureSamplerState, tc0, 0),
    //    ln_BoneTexture.SampleLevel(ln_BoneTextureSamplerState, tc1, 0),
    //    ln_BoneTexture.SampleLevel(ln_BoneTextureSamplerState, tc2, 0));
    
    return float4x3(
        tex2Dlod(ln_BoneTexture, tc0),
        tex2Dlod(ln_BoneTexture, tc1),
        tex2Dlod(ln_BoneTexture, tc2));

    /*
    return float4x3(
        tex2Dlod( ln_BoneSampler, tc0 ),
        tex2Dlod( ln_BoneSampler, tc1 ),
        tex2Dlod( ln_BoneSampler, tc2 ) );
        */
}

float4x4 LN_GetBoneMatrix4x4(int boneIndex)
{
    //return float4x4(
    //    1, 0, 0, 0,
    //    0, 1, 0, 0,
    //    0, 0, 1, 0,
    //    0, 0, 0, 1);
    float offset = 0.25;    // DX9 の half pixel offset に備える
    float2 uv = ln_BoneTextureReciprocalSize.xy;
    float4 tc0 = float4( (0.0 + offset) * uv.x, (boneIndex + offset) * uv.y, 0, 1 );    
    float4 tc1 = float4( (1.0 + offset) * uv.x, (boneIndex + offset) * uv.y, 0, 1 );
    float4 tc2 = float4( (2.0 + offset) * uv.x, (boneIndex + offset) * uv.y, 0, 1 );
    float4 tc3 = float4( (3.0 + offset) * uv.x, (boneIndex + offset) * uv.y, 0, 1 );
    
    
    //return float4x4(
    //    ln_BoneTexture.SampleLevel(ln_BoneTextureSamplerState, tc0, 0),
    //    ln_BoneTexture.SampleLevel(ln_BoneTextureSamplerState, tc1, 0),
    //    ln_BoneTexture.SampleLevel(ln_BoneTextureSamplerState, tc2, 0),
    //    ln_BoneTexture.SampleLevel(ln_BoneTextureSamplerState, tc3, 0));
    return float4x4(
        tex2Dlod(ln_BoneTexture, tc0),
        tex2Dlod(ln_BoneTexture, tc1),
        tex2Dlod(ln_BoneTexture, tc2),
        tex2Dlod(ln_BoneTexture, tc3));
    /*
    return float4x4(
        tex2Dlod( ln_BoneSampler, tc0 ),    // rot
        tex2Dlod( ln_BoneSampler, tc1 ),    // rot
        tex2Dlod( ln_BoneSampler, tc2 ),    // rot
        tex2Dlod( ln_BoneSampler, tc3 ));   // transform
        */
}

float4 LN_GetBoneLocalQuaternion(int boneIndex)
{
    float offset = 0.25;    // DX9 の half pixel offset に備える
    float2 uv = ln_BoneTextureReciprocalSize.xy;
    float4 tc0 = float4(offset, (boneIndex + offset) * uv.y, 0, 1);
    //return ln_BoneLocalQuaternionTexture.SampleLevel(ln_BoneLocalQuaternionTextureSamplerState, tc0, 0);
    return tex2Dlod(ln_BoneLocalQuaternionTexture, tc0);
    //return tex2Dlod(ln_BoneLocalQuaternionSampler, tc0);
}

float4 LN_QuaternionSlerp(float4 q1, float4 q2, float t)
{
    float4 quat;

    float opposite, inverse;
    float tt = 1;
    float dot = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;

    if (dot < 0)
    {
        tt = -1;
        dot = -dot;
    }

    if (dot > 0.999999)
    {
        inverse = 1.0 - t;
        opposite = t * tt;
    }
    else
    {
        float acosf = acos(dot);
        float invSin = 1.0 / sin(acosf);

        inverse = sin((1.0 - t) * acosf) * invSin;
        opposite = sin(t * acosf) * invSin * tt;
    }

    return float4(inverse * q1.x + opposite * q2.x, inverse * q1.y + opposite * q2.y, inverse * q1.z + opposite * q2.z, inverse * q1.w + opposite * q2.w);
}

float4x4 LN_QuaternionToMatrix(float4 quat)
{
    float4x4 mat;

    float xy = quat.x * quat.y;
    float zz = quat.z * quat.z;
    float wx = quat.w * quat.x;
    float wy = quat.w * quat.y;
    float yz = quat.y * quat.z;
    float wz = quat.w * quat.z;

    mat._11 = 1.0f - 2.0f * quat.y * quat.y - 2.0f * zz;
    mat._12 = 2.0f * xy + 2.0f * wz;
    mat._13 = 2.0f * quat.x * quat.z - 2.0f * wy;
    mat._14 = 0;

    mat._21 = 2.0f * xy - 2.0f * wz;
    mat._22 = 1.0f - 2.0f * quat.x * quat.x - 2.0f * zz;
    mat._23 = 2.0f * yz + 2.0f * wx;
    mat._24 = 0;

    mat._31 = 2.0f * quat.x * quat.z + 2.0f * wy;
    mat._32 = 2.0f * yz - 2.0f * wx;
    mat._33 = 1.0f - 2.0f * quat.x * quat.x - 2.0f * quat.y * quat.y;
    mat._34 = 0;

    mat._41 = 0;
    mat._42 = 0;
    mat._43 = 0;
    mat._44 = 1.0f;

    return mat;
}

MMM_SKINNING_OUTPUT MMM_SkinnedPositionNormal(
    float4    position,
    float3    normal,
    float4    weights,
    float4    indices,
    float4    sdefC,
    float3    sdefR0,
    float3    sdefR1 )
{
    MMM_SKINNING_OUTPUT o;
    float4 pos = 0.0;
    float4 nml = 0.0;

    float blendWeights[4] = { weights.x, weights.y, weights.z, weights.w };//(float[4])weights;
    float BlendIndices[4] = { indices.x, indices.y, indices.z, indices.w };//(float[4])indices;
    
    if (sdefC.w >= 0)
    {
        float4 center = float4(sdefC.xyz, 1.0f);
        float4 r0 = float4(sdefR0, 1.0f);
        float4 r1 = float4(sdefR1, 1.0f);

        float3 pp0 = mul(r0, LN_GetBoneMatrix(BlendIndices[0]));
        float3 pp1 = mul(r1, LN_GetBoneMatrix(BlendIndices[1]));

        float3 prc = pp0 * blendWeights[0] + pp1 * blendWeights[1];

        float3 v1, diff;

        v1 = mul(center, LN_GetBoneMatrix(BlendIndices[0]));
        diff = (v1 - center.xyz) * blendWeights[0];

        v1 = mul(center, LN_GetBoneMatrix(BlendIndices[1]));
        diff += (v1 - center.xyz) * blendWeights[1];

        float3 trans = center.xyz + diff;
        trans = (trans + prc) * 0.5;

        float4 quat = LN_QuaternionSlerp(LN_GetBoneLocalQuaternion(BlendIndices[0]), LN_GetBoneLocalQuaternion(BlendIndices[1]), blendWeights[1]);
        float4x4 mat = LN_QuaternionToMatrix(quat);
        
        pos = mul(position - center, mat) * MMM_modelsize + float4(trans, 0.0f);
        nml = mul(float4(normal, 1.0f), mat) * MMM_modelsize;
        
        
    }
    else
    {
        float lastWeight = 0.0f;

        for (int i = 0; i < 3; i++)
        {
            lastWeight += blendWeights[i];
            pos.xyz += mul(float4(position.xyz, 1.0f), LN_GetBoneMatrix(BlendIndices[i])) * blendWeights[i];
            nml.xyz += mul(float4(normal.xyz, 1.0f), LN_GetBoneMatrix(BlendIndices[i])) * blendWeights[i];
        }
        lastWeight = 1.0f - lastWeight;
        pos.xyz += mul(float4(position.xyz, 1.0f), LN_GetBoneMatrix(BlendIndices[3])) * lastWeight;
        nml.xyz += mul(float4(normal.xyz, 1.0f), LN_GetBoneMatrix(BlendIndices[3])) * lastWeight;
        
        pos.w = 1.0f;
    }
    
    o.Position = pos;
    o.Normal = nml.xyz;
    
    return o;
}

LN_SkinningOutput LN_SkinningVertex(
    float3    position,
    float3    normal,
    float4    weights,
    float4    indices)
{
    float4 pos = 0.0;
    float4 nml = 0.0;
    float blendWeights[4] = { weights.x, weights.y, weights.z, weights.w };//(float[4])weights;
    float BlendIndices[4] = { indices.x, indices.y, indices.z, indices.w };//(float[4])indices;

    float lastWeight = 0.0;
    for (int i = 0; i < 3; i++)
    {
        lastWeight += blendWeights[i];
        float4x4 mat = LN_GetBoneMatrix4x4((int)BlendIndices[i]);
        pos.xyz += mul(float4(position.xyz, 1.0f), mat).xyz * blendWeights[i];
        nml.xyz += mul(float4(normal.xyz, 1.0f), mat).xyz * blendWeights[i];
    }
    lastWeight = 1.0f - lastWeight;
    float4x4 mat = LN_GetBoneMatrix4x4((int)BlendIndices[3]);
    pos.xyz += mul(float4(position.xyz, 1.0f), mat).xyz * lastWeight;
    nml.xyz += mul(float4(normal.xyz, 1.0f), mat).xyz * lastWeight;
    
    LN_SkinningOutput o;
    o.Position = pos.xyz;
    o.Normal = nml.xyz;
    return o;
}

void _LN_ProcessVertex_SkinnedMesh(
    const float3 position,
    float3    normal,
    float4    blendIndices,
    float4    blendWeight,
    out float4 outSVPos, out float3 outViewNormal)
{
    LN_SkinningOutput local = LN_SkinningVertex(position, normal, blendWeight, blendIndices);

    LN_VSOutput_Common o;
    outSVPos = mul(float4(local.Position, 1.0f), ln_WorldViewProjection);
    outViewNormal = mul(float4(local.Normal, 1.0f), ln_WorldViewIT).xyz;
}

#endif // LUMINOSKINNING_INCLUDED
