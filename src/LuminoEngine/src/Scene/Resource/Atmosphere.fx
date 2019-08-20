
#include <Lumino.fxh>

struct VSOutput
{
	float4	svPos		: SV_POSITION;
	float4	ScreenPos	: TEXCOORD0;
	//float4 tmpUV: TEXCOORD12;
};

struct PSInput
{
	float4 ScreenPos	: TEXCOORD0;
	//float4 tmpUV: TEXCOORD12;
};

VSOutput VS_Main(LN_VSInput input)
{
	VSOutput o;
	o.svPos = float4(input.Pos, 1.0);
	o.ScreenPos = o.svPos;
	return o;
}

float4 PS_Main(PSInput input) : SV_TARGET
{
	return float4(1, 0, 0, 1);
}

technique Forward_Geometry_UnLighting
{
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}
