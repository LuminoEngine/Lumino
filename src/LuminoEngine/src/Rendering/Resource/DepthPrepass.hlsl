/*
 * FarClip を最大とした、視点からの距離を書き込むシェーダ。
 * NearClip(0.0) ～ FarClip(1.0) の値を R に出力する。
 * 視点から離れるほど値が大きくなる。
 */

float4x4 ln_WorldView;
float4x4 ln_Projection;
float ln_NearClip;
float ln_FarClip;

struct VSInput
{
    float3 Pos : POSITION;
};

struct VSOutput
{
    float4 svPos : SV_POSITION;
    float4 ViewPos : TEXCOORD0;
};

struct PSInput
{
    float4 ViewPos : TEXCOORD0;
};

VSOutput VS_WriteLinearDepth(VSInput input)
{
    VSOutput output;
    output.ViewPos = mul(float4(input.Pos, 1.0), ln_WorldView);
    output.svPos = mul(output.ViewPos, ln_Projection);
    return output;
}

float4 PS_WriteLinearDepth(PSInput input) : COLOR0
{
    float z = (input.ViewPos.z - ln_NearClip) / (ln_FarClip - ln_NearClip);
    return float4(z, 0, 0, 1);
}

technique Default
{
    pass Pass1
    {
        VertexShader = VS_WriteLinearDepth;
        PixelShader = PS_WriteLinearDepth;
    }
}
