import lumino;

public struct VSOutput {
    public float4 position: SV_POSITION;
    public float2 uv: TEXCOORD0;
};

[shader("vertex")]
VSOutput vsMain(lumino.VSInput input) {
    VSOutput output;
    output.position = mul(float4(input.position, 1.0f), ln_WorldViewProjection);
    output.uv = input.uv;
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input): SV_TARGET {
    return ln_MaterialTexture.Sample(input.uv);
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
