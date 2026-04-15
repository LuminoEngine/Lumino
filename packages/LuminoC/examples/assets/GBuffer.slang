// DepthNormal.slang — View-space linear depth + normal output
//
// Renders view-space linear depth to R channel and encoded normal to GBA.
// Output format: RGBA16Float
//   R  = linear depth (positive distance from camera)
//   GBA = view-space normal (encoded: n * 0.5 + 0.5)

import lumino;

struct MaterialParams {
    float4 color;
};

uniform ConstantBuffer<MaterialParams> u_params;
uniform Texture2D                      u_baseTexture;
uniform SamplerState                   u_baseSampler;

struct VSOutput {
    float4 position  : SV_POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD0;
    float3 worldNormal : TEXCOORD1;
    float3 viewPos   : TEXCOORD2;
};

struct FSOutput {
    // https://docs.unity3d.com/ja/2018.4/Manual/RenderTech-DeferredShading.html
    // https://docs.unity3d.com/ja/Packages/com.unity.render-pipelines.universal@14.0/manual/rendering/deferred-rendering-path.html
    // https://cgworld.jp/regular/202312-ue5toon-05.html
    // ScreenDepth:
    //   スクリーン平面からの距離。near:0.0, far=1.0。
    //   カメラ位置との距離では無いので注意。
    //   NDR Depth でもない線形の値なので注意。

    float4 gbufferA: SV_TARGET0;  // [Albedo.r, Albedo.g, Albedo.b, NONE]
    float4 gbufferB: SV_TARGET1;  // [WorldNormal.x, WorldNormal.y, WorldNormal.z, ScreenDepth]
    float4 gbufferC: SV_TARGET2;  // [Metallic, Roughness, AO, NONE] (glTF Metal-Roughness)
    
   

    // NOTE: なぜ Depth は [0,1] 線形にしているのか？
    //  一般的なプロジェクション変換された Depth は非線形で、遠くのオブジェクトほど精度が粗くなります。
    //  ハードウェアの Depth Buffer に書き込まれる値は NDC depth などと言われることがあり、
    //  パースペクティブ投影の性質上非線形です。
    //  NDC_depth = (far * (z - near)) / (z * (far - near))   ← z に対して非線形
    //  ※AIによると、 Unity と Filament は DepthBuffer を再利用しているが、
    //    Unreal Engine は G-Buffer の別チャンネルに SceneDepth（線形）を書き込むとのこと。
    //  Unity は Linear01Depth によって線形の値を取得できる。
    //  なお高度なエフェクトを実装する場合、線形にしておくと計算コストが下がることが多い。
    //  - SSR: レイマーチのたびに計算するのが不要になる。
    //  - SSAO: 近くのサンプル数の数だけ計算するのが不要になる。
    //  - CSM(Cascaded Shadow Maps)
    //  - Fog・大気散乱
    //  - Depth of Field
    //  などなど。
};

[shader("vertex")]
VSOutput vsMain(VSInput input) {
    VSOutput output;
    float4 worldPos = mul(objectData.world, float4(input.position, 1.0));
    float4 viewPos4 = mul(viewData.view, worldPos);
    output.position = mul(viewData.proj, viewPos4);
    output.viewPos  = viewPos4.xyz;
    // Transform normal to view space
    float3 worldNorm = normalize(mul((float3x3)objectData.normalMatrix, input.normal));
    output.worldNormal = worldNorm;//normalize(mul((float3x3)viewData.view, worldNorm));
    //output.viewPos4 = viewPos4;
    output.uv = input.uv;
    output.color = input.color;
    return output;
}

[shader("fragment")]
FSOutput fsMain(VSOutput input) {
    FSOutput output;
    
    // Linear view-space depth: -viewPos.z (camera looks along -Z, so this is positive
    // distance in front of the near plane). Cleared value 0 means "no geometry".
    float viewDepth = -input.viewPos.z;
    float3 worldNormal = input.worldNormal;

    float4 textureColor = u_baseTexture.Sample(u_baseSampler, input.uv);
    float4 color = textureColor * u_params.color * input.color;

    output.gbufferA = float4(color.rgb, 1);
    output.gbufferB = float4(worldNormal, viewDepth);
    // dummy: metallic=0.5, roughness=0.1, AO=1.0 — makes surfaces reflective for the SSR demo
    output.gbufferC = float4(0.5, 0.1, 1.0, 1.0);


    // float linearDepth = -input.viewPos.z; // negate: camera looks along -Z in view space
    // float3 N = normalize(input.viewNorm);
    // // Encode normal from [-1,1] to [0,1]
    // output.gbufferA = float4(linearDepth, N.x * 0.5 + 0.5, N.y * 0.5 + 0.5, N.z * 0.5 + 0.5);
    return output;
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
