/*
 * FarClip を最大とした、視点からの距離を書き込むシェーダ。
 * NearClip(0.0) ～ FarClip(1.0) の値を R に出力する。
 * 視点から離れるほど値が大きくなる。
 */

#include <Lumino.fxh>

// TODO: move to lib
float3 LN_PackNormal(float3 normal)
{
	return (normal.xyz + 1.0) / 2.0;
}

struct VSInput
{
    float3 Pos : POSITION0;
    float3 Normal : NORMAL0;
};

struct VSOutput
{
    float4 svPos : SV_POSITION;
    float3 ViewSpaceNormal : NORMAL0;
    float4 ViewPos : TEXCOORD0;
};

struct PSInput
{
    float3 ViewSpaceNormal : NORMAL0;
    float4 ViewPos : TEXCOORD0;
};

struct PSOutput
{
    //float4 Depth : SV_TARGET1;
    float4 Normal : SV_TARGET0;
};

VSOutput VS_WriteLinearDepth(VSInput input)
{
    VSOutput output;
    output.ViewPos = mul(float4(input.Pos, 1.0), ln_WorldView);
    output.ViewSpaceNormal = mul(float4(input.Normal, 1.0f), ln_WorldViewIT).xyz;
    output.svPos = mul(output.ViewPos, ln_Projection);

    output.ViewSpaceNormal.z *= -1.0;

    return output;
}

PSOutput PS_WriteLinearDepth(PSInput input)
{
    float z = (input.ViewPos.z - ln_NearClip) / (ln_FarClip - ln_NearClip);
    PSOutput output;
    output.Normal = float4(LN_PackNormal(input.ViewSpaceNormal), z);
    //output.Depth = float4(z, z, z, 1);

    return output;
}

technique Default
{
    pass Pass1
    {
        VertexShader = VS_WriteLinearDepth;
        PixelShader = PS_WriteLinearDepth;
    }
}
