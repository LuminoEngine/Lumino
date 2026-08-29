// Unlit.slang - テクスチャ × カラー、ライティングなし
//
// Set 0 ($Global / マテリアル):  u_params, u_baseTexture, u_baseSampler
// Set 1 (ビュー単位):           viewData   - ViewParams
// Set 2 (シーン単位):           sceneData  - SceneParams (未使用)
// Set 3 (オブジェクト単位):     objectData - ObjectParams

import lumino;

//------------------------------------------------------------------------------
// マテリアルパラメータ (ベアな uniform -> $Global, Set 0)
//------------------------------------------------------------------------------
struct MaterialParams {
    float4 color;
};

uniform ConstantBuffer<MaterialParams> u_params;
uniform Texture2D                      u_baseTexture;
uniform SamplerState                   u_baseSampler;

struct VSOutput {
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD;
    float4 color    : COLOR;
};

[shader("vertex")]
VSOutput vsMain(VSInput input) {
    VSOutput output;
    float4 worldPos = mul(objectData.world, float4(input.position, 1.0));
    output.position = mul(viewData.viewProj, worldPos);
    output.uv = input.uv;
    output.color = input.color;
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_TARGET {
    float4 texColor = u_baseTexture.Sample(u_baseSampler, input.uv);
    return texColor * u_params.color * input.color;
}

//------------------------------------------------------------------------------
// GBuffer パス: Unlit マテリアルの albedo / 法線 / 線形深度を GBuffer に書き込む。
// SSR や後段のポストエフェクト用。Metallic/Roughness は 0 を出力 (Unlit なので反射質を持たない)。
//------------------------------------------------------------------------------
struct VSOutputGB {
    float4 position    : SV_POSITION;
    float2 uv          : TEXCOORD0;
    float4 color       : COLOR;
    float3 worldNormal : TEXCOORD1;
    float3 viewPos     : TEXCOORD2;
};

[shader("vertex")]
VSOutputGB vsGBufferMain(VSInput input) {
    VSOutputGB o;
    float4 worldPos = mul(objectData.world, float4(input.position, 1.0));
    float4 viewPos4 = mul(viewData.view, worldPos);
    o.position = mul(viewData.proj, viewPos4);
    o.viewPos  = viewPos4.xyz;
    o.worldNormal = normalize(mul((float3x3)objectData.normalMatrix, input.normal));
    o.uv = input.uv;
    o.color = input.color;
    return o;
}

[shader("fragment")]
LNGBufferOutput fsGBufferMain(VSOutputGB input) {
    float4 texColor = u_baseTexture.Sample(u_baseSampler, input.uv);
    float4 albedo = texColor * u_params.color * input.color;
    float viewDepth = -input.viewPos.z;
    // Unlit マテリアルは金属度や粗さを持たないので 0 を出力。AO は 1.0 (遮蔽なし)。
    return lnPackGBuffer(albedo.rgb, input.worldNormal, viewDepth, 0.0, 0.0, 1.0);
}

#ifdef LUMINO_METADATA
{
    "passes": [
        {
            "name": "Forward",
            "vertex": "vsMain",
            "fragment": "fsMain"
        },
        {
            "name": "GBuffer",
            "vertex": "vsGBufferMain",
            "fragment": "fsGBufferMain"
        }
    ]
}
#endif
