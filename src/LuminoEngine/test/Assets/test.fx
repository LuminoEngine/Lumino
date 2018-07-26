technique MainTech
{
	pass Pass0
	{
		VertexShader = vsMain;
		PixelShader = psMain;
	}
}

struct VS_INPUT
{
    float4 Pos : POSITION;
};
 
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
};
 
VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = input.Pos;
    return output;
}

cbuffer ConstBuff : register(b0)
{
    float4 g_color;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
};

float4 psMain(PS_INPUT input) : SV_TARGET
{
    return g_color;
}


