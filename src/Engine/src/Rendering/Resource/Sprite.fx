
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
#ifdef LN_USE_INSTANCING

/*
struct LN_VSIInput
{
    //float3    Pos                : POSITION;
    //float3    Normal            : NORMAL0;
    //float2    UV                : TEXCOORD0;
    //float4    Color            : COLOR0;
    //float4  tangent: TANGENT;
    
    float3    Pos                : POSITION;
    float3    Normal            : NORMAL0;
    float2    UV                : TEXCOORD0;
    float4    Color            : COLOR0;
    float4  tangent: TANGENT0;
#ifdef LN_USE_SKINNING
    float4    BlendIndices    : BLENDINDICES;
    float4    BlendWeight        : BLENDWEIGHT;
#endif

};
*/

struct VSIOutput
{
    LN_VS_OUTPUT_DECLARE;
};

struct PSIInput
{
    LN_PS_INPUT_DECLARE;
};

/*
struct LN_PSIInput
{
    //float3    Normal        : NORMAL0;
    float2    UV            : TEXCOORD0;
    float4    Color        : COLOR0;
    float4    svPos        : SV_POSITION;
};
*/

VSIOutput VSI_Main(LN_VSInput input)//LN_VSIInput input)
{
    VSIOutput output;
    LN_ProcessVertex(input, output);
    return output;
#if 0
    float4x4 worldMatrix = float4x4(
        input.InstanceTransform0.x, input.InstanceTransform0.y, input.InstanceTransform0.z, input.InstanceTransform0.w,
        input.InstanceTransform1.x, input.InstanceTransform1.y, input.InstanceTransform1.z, input.InstanceTransform1.w,
        input.InstanceTransform2.x, input.InstanceTransform2.y, input.InstanceTransform2.z, input.InstanceTransform2.w,
        input.InstanceTransform3.x, input.InstanceTransform3.y, input.InstanceTransform3.z, input.InstanceTransform3.w);
    float4 worldPos = mul(float4(input.Pos, 1.0f), worldMatrix);


    VSIOutput o;
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
#endif
}

float4 PSI_Main2(PSIInput input) : SV_TARGET
{
    //return float4(1, 0, 0, 1);
    float4 color = tex2D(ln_MaterialTexture, input.UV) * input.Color;
    clip(color.a - 0.0001); // Alpha test

    return LN_GetBuiltinEffectColor(color);
}

#endif

//------------------------------------------------------------------------------
technique Forward_Geometry_UnLighting
{
    pass Pass0
    {
        VertexShader = VS_ClusteredForward_Geometry;
        PixelShader  = PS_Main;
    }
}

technique Forward_Geometry_UnLighting_Instancing
{
    Instancing = true;
    pass Pass0
    {
        VertexShader = VSI_Main;
        PixelShader  = PSI_Main2;
    }
}
