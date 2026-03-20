uniform float u_Value1;

public struct VSOutput {
    public float4 position: SV_POSITION;
};

[shader("vertex")]
VSOutput vsMain(float4 position: POSITION) {
    VSOutput output;
    output.position = position;
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
            "name": "Forward",
            "vertex": "vsMain",
            "fragment": "fsMain"
        }
    ]
}
#endif
