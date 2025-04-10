import lumino;

public struct VSOutput {
    public float4 position: SV_POSITION;
    public float3 normal: NORMAL0;
    public float2 uv: TEXCOORD0;
    public float4 color: COLOR0;
};

[shader("vertex")]
VSOutput vsMain(lumino.VSInput input) {
    VSOutput output;
    output.position = mul(float4(input.position, 1.0f), ln_WorldViewProjection);
    output.normal = mul(float4(input.normal, 1.0f), ln_WorldViewIT).xyz;
    output.uv = input.uv;
    output.color = input.color;
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input): SV_TARGET {
    float4 color = ln_MaterialTexture.Sample(input.uv) * input.color;
    clip(color.a - 0.0001);
    return color;
}

#ifdef LUMINO_METADATA
{
    "passes": [
        {
            "vertex": "vsMain",
            "fragment": "fsMain"
        }
    ]
}
#endif
