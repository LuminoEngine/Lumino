
#include <Lumino.fxh>

struct PSInput
{
    float4 svPos    : SV_POSITION;
    float2 UV       : TEXCOORD0;
    float4 Color    : COLOR0;
};

PSInput VS_ClusteredForward_Geometry(LN_VSInput input)
{
    PSInput o;
    o.svPos = mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
    //o.svPos = mul(ln_WorldViewProjection, float4(input.Pos, 1.0f));
    o.UV = input.UV;
    o.Color = input.Color;
    return o;
}

float4 PS_Main(PSInput input) : SV_TARGET0
{
    float4 color = tex2D(ln_MaterialTexture, input.UV) * input.Color;
    
    // Alpha test
    clip(color.a - 0.0001);

    return LN_GetBuiltinEffectColor(color);
}


//------------------------------------------------------------------------------
struct LN_VSIInput
{
    float3    Pos                : POSITION;
    float3    Normal            : NORMAL0;
    float2    UV                : TEXCOORD0;
    float4    Color            : COLOR0;
    float4  tangent: TANGENT;

    float4 InstanceTransform0 : POSITION1;
    float4 InstanceTransform1 : POSITION2;
    float4 InstanceTransform2 : POSITION3;
    float4 InstanceTransform3 : POSITION4;
    float4 InstanceUVOffset: TEXCOORD2;
    float4 InstanceColorScale: COLOR1;
};

/*
struct LN_VSIOutput
{
    //float3    Normal        : NORMAL0;
    float2    UV            : TEXCOORD0;
    float4    Color        : COLOR0;
    float4    svPos        : SV_POSITION;
};
*/

struct LN_PSIInput
{
    //float3    Normal        : NORMAL0;
    float2    UV            : TEXCOORD0;
    float4    Color        : COLOR0;
    float4    svPos        : SV_POSITION;
};

LN_PSIInput VSI_Main(LN_VSIInput input)
{
    //float4x4 worldMatrix = float4x4(
    //    input.InstanceTransform1.x, input.InstanceTransform2.x, input.InstanceTransform3.x, 0.0,
    //    input.InstanceTransform1.y, input.InstanceTransform2.y, input.InstanceTransform3.y, 0.0,
    //    input.InstanceTransform1.z, input.InstanceTransform2.z, input.InstanceTransform3.z, 0.0,
    //    input.InstanceTransform1.w, input.InstanceTransform2.w, input.InstanceTransform3.w, 0.0);
#if 1
    float4x4 worldMatrix = float4x4(
        input.InstanceTransform0.x, input.InstanceTransform0.y, input.InstanceTransform0.z, input.InstanceTransform0.w,
        input.InstanceTransform1.x, input.InstanceTransform1.y, input.InstanceTransform1.z, input.InstanceTransform1.w,
        input.InstanceTransform2.x, input.InstanceTransform2.y, input.InstanceTransform2.z, input.InstanceTransform2.w,
        input.InstanceTransform3.x, input.InstanceTransform3.y, input.InstanceTransform3.z, input.InstanceTransform3.w);
    float4 worldPos = mul(float4(input.Pos, 1.0f), worldMatrix);
#else
    float4 worldPos = float4(input.Pos, 1.0f);
    worldPos.xyz += input.InstanceTransform3.xyz;
#endif

    LN_PSIInput o;
    //o.svPos            = mul(worldPos, ln_View);
    //o.svPos            = mul(o.svPos, ln_Projection);
    //o.svPos            = mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
    //o.svPos            = mul(ln_Projection, mul(float4(input.Pos, 1.0f), ln_WorldView));
    o.svPos            = mul(worldPos, ln_World);
    o.svPos            = mul(o.svPos, ln_View);
    o.svPos            = mul(o.svPos, ln_Projection);
    //o.Normal        = mul(float4(input.Normal, 1.0f), ln_WorldViewIT).xyz;
    o.UV            = (input.UV * input.InstanceUVOffset.zw) + input.InstanceUVOffset.xy;
    o.Color            = input.Color * input.InstanceColorScale;
    return o;
}

float4 PSI_Main2(LN_PSIInput input) : SV_TARGET
{
    //return float4(1, 0, 0, 1);
    float4 color = tex2D(ln_MaterialTexture, input.UV) * input.Color;
    clip(color.a - 0.0001); // Alpha test

    return LN_GetBuiltinEffectColor(color);
}

//------------------------------------------------------------------------------
@module
techniques:
    Forward_Geometry_UnLighting:
        passes:
        -   Pass0:
                vertexShader: VS_ClusteredForward_Geometry
                pixelShader: PS_Main

    Forward_Geometry_UnLighting_Instancing:
        passes:
        -   Pass0:
                vertexShader: VSI_Main
                pixelShader: PSI_Main2
@end
