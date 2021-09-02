
struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 TexUV : TEXCOORD;
};
 
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 TexUV : TEXCOORD;
};
 
VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = input.Pos;
    output.TexUV = input.TexUV;
    return output;
}

struct PS_INPUT
{
    float4 TexUV : TEXCOORD;
};


cbuffer ConstBuff
{
    float time;
};

float4 psMain(PS_INPUT input) : SV_TARGET
{
    float2 st = input.TexUV;
    st -= 0.5;
	float3 color = float3(0.0, 0.0, 0.0);
    float c = 0.0;
    for (float i = 0; i < 12; i++) {

		st.x += sin(st.y * 4.0 + time * 2.0 + c * 40.0) * 0.2 * cos(time + (c + 2.0) * 300.0);
		color += (1.0 - float3(pow(abs(st.x), 0.03)));
        c += 1.0;
	}

	color = float3(color.r * 0.2, color.g * 1.0, color.b * 1.5);
	return float4(color, 1.0);
}

technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain;
    }
}
