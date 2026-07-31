// WGSL 検証テスト用: Slang では通るが WGSL では不正になるシェーダ。
//
// 早期 return を挟んだ後にテクスチャをサンプルしているため、WGSL の
// "textureSample must only be called from uniform control flow" に違反する。
// 検証が無いと、この不具合は WebGPU で実行して画面が真っ黒になるまで気づけない。

import lumino;

struct MaterialParams {
    float4 debugParams;
};

uniform ConstantBuffer<MaterialParams> u_params;
uniform Texture2D                      u_gbufferA;
uniform SamplerState                   u_gbufferASampler;

struct VSOutput {
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD;
};

[shader("vertex")]
VSOutput vsMain(VSInput input) {
    VSOutput output;
    float4 worldPos = mul(objectData.world, float4(input.position, 1.0));
    output.position = mul(viewData.viewProj, worldPos);
    output.uv = input.uv;
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_TARGET {
    // NG: 早期 return の後にテクスチャをサンプルしている。
    if (u_params.debugParams.x > 0.5) {
        if (input.uv.x > 0.5) {
            return float4(1, 0, 0, 1);
        }
    }
    const float4 gbA = u_gbufferA.Sample(u_gbufferASampler, input.uv);
    return gbA;
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
