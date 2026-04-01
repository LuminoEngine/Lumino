// HelloTriangle.slang
// 色付き三角形を描画するシンプルなシェーダー

struct VSInput {
    float2 position : POSITION;
    float3 color    : COLOR;
};

struct VSOutput {
    float4 position : SV_POSITION;
    float3 color    : COLOR;
};

[shader("vertex")]
VSOutput vsMain(VSInput input) {
    VSOutput output;
    output.position = float4(input.position, 0.0, 1.0);
    output.color    = input.color;
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_TARGET {
    return float4(input.color, 1.0);
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
