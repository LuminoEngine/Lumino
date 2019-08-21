
#include <Lumino.fxh>

float4x4 _localWorld;
float4x4 _scaleMatrix;

struct VSOutput
{
	float4	svPos		: SV_POSITION;
	float3	vertex		: TEXCOORD0;
	float3	eyeRay		: TEXCOORD1;
};

struct PSInput
{
	float3	vertex		: TEXCOORD0;
	float3	eyeRay		: TEXCOORD1;
};

//------------------------------------------------------------------------------

VSOutput VS_Main(LN_VSInput v)
{
	VSOutput o;
	o.svPos = float4(v.Pos, 1.0);

	float3 viewportPos = float3(v.Pos.x, v.Pos.y, 1.0);
	float3 localRay = mul(viewportPos, (float3x3)_scaleMatrix);
	//float3 eyeRayRaw = mul((float3x3)_localWorld, viewportPos);
	//float3 eyeRay = normalize(eyeRayRaw);

	o.vertex = viewportPos;
	o.eyeRay = localRay;

	return o;
}
//------------------------------------------------------------------------------


float4 PS_Main(PSInput input) : SV_TARGET
{
	float3 localRay = mul(input.vertex, (float3x3)_scaleMatrix);
	float4 eyeRayRaw = mul(float4(localRay, 1.0), _localWorld);
	float3 eyeRay = normalize(eyeRayRaw.xyz / eyeRayRaw.w);
	float3 color = round((eyeRay + 1.0) / 2.0);

	return float4(color, 1);
}

//------------------------------------------------------------------------------


technique Forward_Geometry_UnLighting
{
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}
