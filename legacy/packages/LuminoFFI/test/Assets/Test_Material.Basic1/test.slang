import lumino;

uniform float u_Value1;

public struct VSOutput {
    public float4 position: SV_POSITION;
};

[shader("vertex")]
VSOutput vsMain(lumino.VSInput input) {
    VSOutput output;
    output.position = mul(float4(input.position, 1.0f), ln_WorldViewProjection);
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input): SV_TARGET {
    return float4(u_Value1, 0.0, 0.0, 1.0);
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
