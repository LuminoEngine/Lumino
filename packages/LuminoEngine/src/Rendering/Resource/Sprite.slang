
cbuffer LNRenderElementBuffer
{
    /* [0]   */ float4x4 ln_World;
    /* [64]  */ float4x4 ln_WorldI;
    /* [128] */ float4x4 ln_WorldViewProjection;
    /* [192] */ float4x4 ln_WorldView;
    /* [256] */ float4x4 ln_WorldViewIT;
    /* [320] */ float4 ln_BoneTextureReciprocalSize;
    /* [336] */ float4 ln_MorphWeights;
    /* [352] */ int ln_objectId;
};  /* [368(alignd:16)] */

struct VSInput {
    float3 Pos: POSITION;
    float3 Normal: NORMAL0;
    float2 UV: TEXCOORD0;
    float4 Color: COLOR0;
};

Sampler2D ln_MaterialTexture;

//-----------

struct VSOutput {
    float4 svPos: SV_POSITION;
    float3 Normal: NORMAL0;
    float2 UV: TEXCOORD0;
    float4 Color: COLOR0;
};

[shader("vertex")]
VSOutput VS_Main(VSInput input) {
    VSOutput output = (VSOutput)0;
    output.svPos = mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
    output.Normal = mul(float4(input.Normal, 1.0f), ln_WorldViewIT).xyz;
    output.UV = input.UV;
    output.Color = input.Color;
    return output;
}

[shader("fragment")]
float4 FS_Main(VSOutput input): SV_TARGET {
    float4 color = ln_MaterialTexture.Sample(input.UV) * input.Color;
    clip(color.a - 0.0001);
    return color;
}

#ifdef LUMINO_METADATA
{
    "passes": [
        {
            "vertex": "VS_Main",
            "fragment": "FS_Main"
        }
    ]
}
#endif
