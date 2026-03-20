
struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Tex : TEXCOORD;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = float4(input.Pos, 1);
    output.Tex = input.Tex;
    return output;
}

