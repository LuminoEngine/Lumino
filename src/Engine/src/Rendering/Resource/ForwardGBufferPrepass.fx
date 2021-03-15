/*
 * FarClip を最大とした、視点からの距離を書き込むシェーダ。
 * NearClip(0.0) ～ FarClip(1.0) の値を R に出力する。
 * 視点から離れるほど値が大きくなる。
 */

#include <Lumino.fxh>
#include <LuminoForward.fxh>
#include <LuminoPBR.fxh>

/*
struct VSInput
{
    float3 Pos : POSITION0;
    float3 Normal : NORMAL0;
    float2 UV : TEXCOORD0;
};
*/

struct VSOutput
{
    LN_VS_OUTPUT_DECLARE;
    //float4 svPos : SV_POSITION; // svPos
    //float3 ViewSpaceNormal : NORMAL0;   // viewNormal
    float4 ViewPos : TEXCOORD0;
    //float2 UV : TEXCOORD1;              // UV
    float4 ClipSpacePos : TEXCOORD2;
};

struct PSInput
{
    LN_PS_INPUT_DECLARE;
    //float3 ViewSpaceNormal : NORMAL0; // viewNormal
    float4 ViewPos : TEXCOORD0;
    //float2 UV : TEXCOORD1;
    float4 ClipSpacePos : TEXCOORD2;
};

struct PSOutput
{
    //float4 Depth : SV_TARGET1;
    float4 Normal : SV_TARGET0;
    float4 Depth: SV_TARGET1;
    float4 Material: SV_TARGET2;
    int4 ObjectId: SV_TARGET3;
};

VSOutput VSMain(LN_VSInput input)
{
    VSOutput output;
    LN_ProcessVertex(input, output);


    //return output;

    //VSOutput output;
    output.ViewPos = mul(float4(input.Pos, 1.0), ln_WorldView);
    //output.ViewSpaceNormal = mul(input.Normal, (float3x3)ln_WorldViewIT);
    //output.svPos = mul(float4(input.Pos, 1.0), ln_WorldViewProjection);
    //output.UV = input.UV;
    output.ClipSpacePos = output.svPos;
    return output;
}

PSOutput PS_WriteLinearDepth(PSInput input)
{
    // SSR 用。シャドウマップと同じく PS で w 除算で計算する。ViewSpace じゃなくて ClipSpace。
    float clipSpaceZ = input.ClipSpacePos.z / input.ClipSpacePos.w;

    // 左手系では、near:0.0 ~ far:1.0
    float clipSpaceLinearZ = (input.ViewPos.z - ln_NearClip) / (ln_FarClip - ln_NearClip);

    //float z = input.ClipSpacePos.z;
    PSOutput output;

    // 左手系の場合、Z+ が正面(奥)
    // 右手系の場合、Z- が正面(奥)
    output.Normal = float4(LN_PackNormal(normalize(input.viewNormal)), 1.0); // normalize 必須

    output.Depth = float4(clipSpaceZ, clipSpaceLinearZ, 0.0, 1.0);

#ifdef LN_USE_ROUGHNESS_MAP
    // metallic 値は .b からサンプリングする。
    // roughness 値は .g からサンプリングする。
    // それぞれマテリアルの metallic, roughness 値と乗算しなければならない。
    // https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#pbrmetallicroughnessmetallicroughnesstexture
    // https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#pbrmetallicroughnessroughnessfactor
    const float4 mr = tex2D(ln_MetallicRoughnessTexture, input.UV);
    const float metallic = mr.b * ln_MaterialMetallic;
    const float roughness = mr.g * ln_MaterialRoughness;
#else
    const float metallic = ln_MaterialMetallic;
    const float roughness = ln_MaterialRoughness;
#endif
    output.Material = float4(metallic, clipSpaceLinearZ, roughness, 1);

    output.ObjectId = uint4(ln_objectId, 0, 0, 0);

    return output;
}

technique Default
{
    Phase = ForwardGBufferPrepass;
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader = PS_WriteLinearDepth;
    }
}

technique RoughnessMap
{
    Phase = ForwardGBufferPrepass;
    Roughness = RoughnessMap;
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader = PS_WriteLinearDepth;
    }
}

technique Forward_Geometry_SkinnedMesh
{
    Phase = ForwardGBufferPrepass;
    VertexProcessing = Skinned;
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader  = PS_WriteLinearDepth;
    }
}
