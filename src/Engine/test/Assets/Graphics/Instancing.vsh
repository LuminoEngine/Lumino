
struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 InstancePos : POSITION1;
    float4 InstanceColor : COLOR0;
};
 
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};
 
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = input.Pos;
    output.Pos.xyz += input.InstancePos.xyz;
    output.Color = input.InstanceColor;
    return output;
}

