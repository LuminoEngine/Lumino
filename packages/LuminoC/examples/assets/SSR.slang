// SSR.slang - スクリーンスペースリフレクション (レイマーチング + PBR 合成)
//
// フルスクリーンのポストプロセスパス。
// 入力 (新しい GBuffer 仕様。GBuffer.slang を参照):
//   u_gbufferA - [Albedo.rgb, _]            (RGBA8_UNORM)
//   u_gbufferB - [WorldNormal.xyz, Depth]   (RGBA32_FLOAT)
//                Depth = -viewPos.z (ビュー空間の線形距離。+Z はカメラの後方)
//   u_gbufferC - [Metallic, Roughness, AO, _] (RGBA8_UNORM, glTF Metal-Roughness)
// 出力:
//   スクリーンスペース反射を合成したシーンカラー
// スクリーンスペースリフレクション (SSR)
// https://zenn.dev/mebiusbox/articles/43ecf1bb12831c
import lumino;

struct SSRParams {
    float4 ssrSettings; // x=maxDistance, y=stepSize, z=thickness, w=maxSteps
};

uniform ConstantBuffer<SSRParams> u_params;
uniform Texture2D                 u_gbufferA;
uniform SamplerState              u_gbufferASampler;
uniform Texture2D                 u_gbufferB;
uniform SamplerState              u_gbufferBSampler;
uniform Texture2D                 u_gbufferC;
uniform SamplerState              u_gbufferCSampler;

struct VSOutput {
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD;
};

[shader("vertex")]
VSOutput vsMain(VSInput input) {
    VSOutput output;
    output.position = float4(input.position, 1.0);
    output.uv = input.uv;
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_TARGET {
    // GBuffer については packages\LuminoC\examples\assets\GBuffer.slang を参照。
    float4 sceneColor = u_gbufferA.Sample(u_gbufferASampler, input.uv);
    float4 gbB        = u_gbufferB.Sample(u_gbufferBSampler, input.uv);
    float4 gbC        = u_gbufferC.Sample(u_gbufferCSampler, input.uv);
    //return gbB;

    float3 worldNormal = gbB.xyz;
    float  depth       = gbB.w; // 遠い方が値が大きい。max は far plane.
    //return float4(depth/10.0f, 0, 0, 1);

    // ジオメトリのないピクセルはスキップ (クリア後の深度 = 0)
    if (depth <= 0.0) {
        return sceneColor;
    }

    // float metallic  = gbC.r;
    // float roughness = gbC.g;
    float metallic  = 0.0;//gbC.r;
    float roughness = 0.0;//gbC.g;

    // 完全にラフな表面には使えるスペキュラ反射がない - レイマーチをスキップする。
    if (roughness >= 1.0) {
        return sceneColor;
    }

    // [-1..1] の NDC 空間。
    // 左上が (-1,-1), 右下が (1,1), 中央が (0,0).
    float2 ndc = input.uv * 2.0 - 1.0;
    //return float4(ndc, 0, 1);

    // 深度情報、UV座標、逆投影情報からビュー空間の位置を再構築する。

    // まず、 NDC の Y 座標を反転する。
    // 次の View 空間への変換で、Y+Up の右手系座標系になるようにするため。
    ndc.y = -ndc.y;

    // NDC の点 (ndc.x, ndc.y, 1.0) を逆投影してビュー空間のレイ方向を得る。
    // これは SSR のようなレイトレーシングを行う場合の典型的な計算方法。
    // viewRay: (ndc.x, ndc.y, 1.0, 1.0)、つまり NDC の Far Plane 上の点を逆投影して得られるビュー空間のレイ方向。
    // 画面中央だと (0,0,-1.0,0.1) とかになり、zw はどの座標でも同じ。 x y は画面右下に行くほど大きくなり、左上に行くほど小さくなる。
    const float4 viewRay = mul(viewData.invProj, float4(ndc, 1.0, 1.0));
    //return viewRay;
    const float3 viewDir = viewRay.xyz / viewRay.w;
    // この時点で、 viewDir は (x, y, -カメラのFarPlane, 1.0) のようになる。
    // つまり、カメラ位置を (0,0,0) としたときの、 Far Plane 上の点のビュー空間での座標。
    // 画面中央だと (0,0,-far) になる。
    // マイナスが付いてるのに注意。(Lumino は godot などと同様カメラの前方を -Z としている)
    //return float4(viewDir.xy, viewDir.z, 1);

    // depth = -viewPos.z, viewDir.z < 0 (カメラは -Z 方向を向く)
    // viewPos は、現在のピクセルに映っているジオメトリのビュー空間での位置。
    // カメラから見たレイがジオメトリ表面と当たった点のビュー空間座標と考えてもOK。
    // 上が Y+, 奥が -Z になるのに注意。
    const float3 viewPos = viewDir * (depth / -viewDir.z);

    // 反射計算のために、ワールド空間の法線をビュー空間の法線に変換する。
    // N: Z+が手前。
    // V: ジオメトリからカメラへ向かう、正規化した向きベクトル
    // reflected: 視点から飛ばしたレイがジオメトリ表面で反射した後の向きベクトル
    const float3 N = normalize(mul((float3x3)viewData.view, worldNormal));
    const float3 V = normalize(-viewPos);
    const float3 reflected = reflect(-V, N);
    //return float4(reflected, 1);

    // SSR レイマーチのパラメータ
    // const float stepSize  = u_params.ssrSettings.y;
    // const float thickness = u_params.ssrSettings.z;
    // const float maxSteps  = u_params.ssrSettings.w;
    //return float4(u_params.ssrSettings);
    //const float maxDistance = 10.0f;
    const float stepSize  = 0.05f; // deltaStep
    const float thickness =  0.3f;
    const float maxSteps  = 128.0f; // _Iterations

    // PBR: F0 (Metal-Roughness ワークフロー, glTF 2.0)
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), sceneColor.rgb, metallic);
    float NdotV = saturate(dot(N, V));
    float3 fresnel = F0 + (1.0 - F0) * pow(1.0 - NdotV, 5.0);
    float roughnessFade = 1.0 - roughness;

    // ビュー空間でレイマーチし、各ステップでスクリーンへ投影する。
    float3 rayPos = viewPos;
    float3 reflectionRGB = float3(0.0, 0.0, 0.0);
    float  reflectionStrength = 0.0;

    for (float i = 0.0; i < maxSteps; i += 1.0) {
        rayPos += reflected * stepSize;

        float4 projPos = mul(viewData.proj, float4(rayPos, 1.0));
        projPos.xy /= projPos.w;
        float2 sampleUV = projPos.xy * 0.5 + 0.5;
        sampleUV.y = 1.0 - sampleUV.y;

        if (sampleUV.x < 0.0 || sampleUV.x > 1.0 ||
            sampleUV.y < 0.0 || sampleUV.y > 1.0) {
            break;
        }

        float sampledDepth = u_gbufferB.SampleLevel(u_gbufferBSampler, sampleUV, 0).w;
        float rayDepth = -rayPos.z;

        if (rayDepth > sampledDepth && rayDepth - sampledDepth < thickness) {
            float3 hitColor = u_gbufferA.SampleLevel(u_gbufferASampler, sampleUV, 0).rgb;
            float fade = 1.0 - (i / maxSteps);
            float2 edgeFade = smoothstep(0.0, 0.1, sampleUV) * (1.0 - smoothstep(0.9, 1.0, sampleUV));
            float edgeFactor = edgeFade.x * edgeFade.y;
            reflectionRGB = hitColor;
            reflectionStrength = fade * edgeFactor;
            break;
        }
    }

    // TODO: ここでは sceneColor と加算しているが、実際にはその前に reflection の値だけ書きだした後ブラーを適用してからブレンドすると良い。
    float3 reflection = reflectionRGB * fresnel * roughnessFade * reflectionStrength;
    float3 finalColor = sceneColor.rgb + reflection;
    return float4(finalColor, 1.0);
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
