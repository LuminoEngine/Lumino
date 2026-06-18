// Composite.slang — オフスクリーン RT を上位ターゲット (バックバッファ等) へ
//                    フルスクリーンで転写する最小の blit (合成) シェーダ。
//
// マテリアル定数バッファ ($Material CB) を一切持たない (テクスチャ+サンプラのみ) ため、
// 「CB を必要としないシェーダ」の読み込みが通ることを検証するためのテスト用フィクスチャ。
import lumino;

uniform Texture2D    u_sceneColor;
uniform SamplerState u_sceneColorSampler;

struct VSOutput {
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD;
};

[shader("vertex")]
VSOutput vsMain(VSInput input) {
    VSOutput output;
    // 頂点は既に NDC。そのまま出力する (camera 非依存)。
    output.position = float4(input.position, 1.0);
    output.uv = input.uv;
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_TARGET {
    return u_sceneColor.Sample(u_sceneColorSampler, input.uv);
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
