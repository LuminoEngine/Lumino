// BasicLit.slang - Blinn-Phong ライティング (1 Directional Light)
//
// ディスクリプタセットのレイアウト (Slang が割り当てる):
//   Set 0 (マテリアル単位):   $Global uniforms (u_params, u_baseTexture, u_baseSampler)
//   Set 1 (ビュー単位):       viewData     - ViewParams (カメラ)
//   Set 2 (シーン単位):       sceneData    - SceneParams (ライティング)
//   Set 3 (オブジェクト単位): objectData   - ObjectParams (暗黙の CB)

import lumino;

struct MaterialParams {
    float4 color;      // ベースカラー (rgba)
    float4 specular;   // xyz = スペキュラ色, w = 光沢度
};

uniform ConstantBuffer<MaterialParams> u_params;
uniform Texture2D                      u_baseTexture;
uniform SamplerState                   u_baseSampler;

struct VSOutput {
    float4 position  : SV_POSITION;
    float3 worldPos  : WORLD_POS;
    float3 worldNorm : WORLD_NORMAL;
    float2 uv        : TEXCOORD;
    float4 color     : COLOR;
};

[shader("vertex")]
VSOutput vsMain(VSInput input) {
    VSOutput output;
    float4 worldPos = mul(objectData.world, float4(input.position, 1.0));
    output.position  = mul(viewData.viewProj, worldPos);
    output.worldPos  = worldPos.xyz;
    output.worldNorm = normalize(mul((float3x3)objectData.normalMatrix, input.normal));
    output.uv        = input.uv;
    output.color     = input.color;
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_TARGET {
    float4 texColor = u_baseTexture.Sample(u_baseSampler, input.uv);
    float4 baseColor = texColor * u_params.color * input.color;

    float3 N = normalize(input.worldNorm);
    float3 L = normalize(sceneData.lightDir.xyz);
    float3 V = normalize(viewData.cameraPos.xyz - input.worldPos);
    float3 H = normalize(L + V);

    // Diffuse (Lambert)
    float NdotL = max(dot(N, L), 0.0);
    float3 diffuse = baseColor.rgb * sceneData.lightColor.xyz * NdotL;

    // Specular (Blinn-Phong)
    float NdotH = max(dot(N, H), 0.0);
    float shininess = u_params.specular.w;
    float3 specular = u_params.specular.xyz * sceneData.lightColor.xyz * pow(NdotH, shininess);

    // Ambient
    float3 ambient = baseColor.rgb * sceneData.ambientColor.xyz;

    float3 finalColor = ambient + diffuse + specular;
    return float4(finalColor, baseColor.a);
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
