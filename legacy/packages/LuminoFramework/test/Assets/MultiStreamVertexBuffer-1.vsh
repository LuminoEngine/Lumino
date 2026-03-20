
struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Color : COLOR0;
    float3 Tex1 : TEXCOORD0;
    float4 Tex2 : TEXCOORD1;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
    float3 Tex1 : TEXCOORD0;
    float4 Tex2 : TEXCOORD1;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = float4(input.Pos, 1);
    output.Color = input.Color;
    output.Tex1 = input.Tex1;
    output.Tex2 = input.Tex2;
    return output;
}

