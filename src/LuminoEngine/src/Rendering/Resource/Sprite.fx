
#include <Lumino.fxh>

LN_VSOutput_Common VS_ClusteredForward_Geometry(LN_VSInput input)
{
    return LN_ProcessVertex_Common(input);
}

float4 PS_Main(LN_PSInput_Common input) : SV_TARGET
{
    return float4(1,0,0,1);
    float4 color = tex2D(ln_MaterialTexture, input.UV) * input.Color;
    
    // Alpha test
    clip(color.a - 0.0001);

	return LN_GetBuiltinEffectColor(color);
}


//------------------------------------------------------------------------------
struct LN_VSIInput
{
	float3	Pos				: POSITION;
	float3	Normal			: NORMAL0;
	float2	UV				: TEXCOORD0;
	float4	Color			: COLOR0;

    float4 InstanceTransform0 : POSITION1;
    float4 InstanceTransform1 : POSITION2;
    float4 InstanceTransform2 : POSITION3;
    float4 InstanceTransform3 : POSITION4;
};

struct LN_VSIOutput
{
	float4	svPos		: SV_POSITION;
	float3	Normal		: NORMAL0;
	float2	UV			: TEXCOORD0;
	float4	Color		: COLOR0;
};

struct LN_PSIInput
{
	float3	Normal		: NORMAL0;
	float2	UV			: TEXCOORD0;
	float4	Color		: COLOR0;
};

LN_VSIOutput VSI_Main(LN_VSIInput input)
{
    //float4x4 worldMatrix = float4x4(
    //    input.InstanceTransform1.x, input.InstanceTransform2.x, input.InstanceTransform3.x, 0.0,
    //    input.InstanceTransform1.y, input.InstanceTransform2.y, input.InstanceTransform3.y, 0.0,
    //    input.InstanceTransform1.z, input.InstanceTransform2.z, input.InstanceTransform3.z, 0.0,
    //    input.InstanceTransform1.w, input.InstanceTransform2.w, input.InstanceTransform3.w, 0.0);
#if 0
    float4x4 worldMatrix = float4x4(
        input.InstanceTransform0.x, input.InstanceTransform0.y, input.InstanceTransform0.z, input.InstanceTransform0.w,
        input.InstanceTransform1.x, input.InstanceTransform1.y, input.InstanceTransform1.z, input.InstanceTransform1.w,
        input.InstanceTransform2.x, input.InstanceTransform2.y, input.InstanceTransform2.z, input.InstanceTransform2.w,
        input.InstanceTransform3.x, input.InstanceTransform3.y, input.InstanceTransform3.z, input.InstanceTransform3.w);
    float4 worldPos = mul(float4(input.Pos, 1.0f), worldMatrix);
#endif
    float4 worldPos = float4(input.Pos, 1.0f);
    worldPos.xyz += input.InstanceTransform3.xyz;

	LN_VSIOutput o;
	//o.svPos			= mul(worldPos, ln_View);
	//o.svPos			= mul(o.svPos, ln_Projection);
    //o.svPos			= mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
    //o.svPos			= mul(ln_Projection, mul(float4(input.Pos, 1.0f), ln_WorldView));
    o.svPos			= mul(worldPos, ln_World);
    o.svPos			= mul(o.svPos, ln_View);
    o.svPos			= mul(o.svPos, ln_Projection);
	o.Normal		= mul(float4(input.Normal, 1.0f), ln_WorldViewIT).xyz;
	o.UV			= input.UV;
	o.Color			= input.Color;
	return o;
}

float4 PSI_Main(LN_PSIInput input) : SV_TARGET
{
    float4 color = tex2D(ln_MaterialTexture, input.UV) * input.Color;
    clip(color.a - 0.0001); // Alpha test

	return LN_GetBuiltinEffectColor(color);
}

//------------------------------------------------------------------------------

technique Forward_Geometry_UnLighting_Instancing
{
    pass Pass1
    {
        VertexShader = VSI_Main;
        PixelShader = PSI_Main;
    }
}
