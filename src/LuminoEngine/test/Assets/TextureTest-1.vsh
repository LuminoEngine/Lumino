//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

cbuffer ConstBuff : register(b0)
{
    matrix mtxProj;
    matrix mtxView;
    matrix mtxWorld;
    float4 Diffuse;
};


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul(float4(input.Pos,1), mtxWorld);
    output.Pos = mul(output.Pos, mtxView);
    output.Pos = mul(output.Pos, mtxProj);
    output.Tex = input.Tex;
    return output;
}

