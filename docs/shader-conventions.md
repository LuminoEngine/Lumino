# シェーダの書き方

Lumino のシェーダは [Slang](https://shader-slang.org/) で記述します。`.slang` ソースを
`luminosc`（シェーダコンパイラ）でクロスコンパイルすると、SPIR-V / DXIL / WGSL / Metal の
全ターゲットを 1 つにまとめた `.lcsh` バイナリが生成され、これを `Material` として読み込みます。

座標系・行列・ワインディングなどグラフィックスの基本規約は
[graphics-conventions.md](graphics-conventions.md) を参照してください。本ドキュメントは
シェーダソースの書き方 と Material API からのパラメータ設定 に焦点を当てます。

---

## コンセプト

クロスプラットフォームをターゲットしたシンプルなシェーダ基盤を目指しています。

特に重視しているのは「バインディングの隠蔽」です。モダンなグラフィックスAPIでは、 set / binding 番号や ParameterBlock の構造など、シェーダリソースのバインディング構成を意識する必要がありますが、Lumino ではこれをランタイムが自動で管理します。これによって、後述する Material API を介して uniform やテクスチャを簡単に設定できるようになります。

## シェーダの全体構造

最小構成の Unlit シェーダを例に、構成要素を示します。

```hlsl
// MyShader.slang
import lumino;                                   // (1) 標準ライブラリ

// (2) マテリアルパラメータ
struct MaterialParams {
    float4 color;
};
uniform ConstantBuffer<MaterialParams> u_params;
uniform Texture2D                      u_baseTexture;
uniform SamplerState                   u_baseSampler;

// (3) 頂点シェーダの出力
struct VSOutput {
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD;
    float4 color    : COLOR;
};

// (4) エントリポイント
[shader("vertex")]
VSOutput vsMain(VSInput input) {                 // VSInput は lumino から
    VSOutput output;
    float4 worldPos = mul(ln_World, float4(input.position, 1.0));
    output.position = mul(ln_ViewProjection, worldPos);
    output.uv       = input.uv;
    output.color    = input.color;
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_TARGET {
    float4 texColor = u_baseTexture.Sample(u_baseSampler, input.uv);
    return texColor * u_params.color * input.color;
}

// (5) パス宣言（メタデータ）
#ifdef LUMINO_METADATA
{
    "passes": [
        { "name": "Forward", "vertex": "vsMain", "fragment": "fsMain" }
    ]
}
#endif
```

| 要素 | 説明 |
|---|---|
| (1) `import lumino;` | カメラ・オブジェクト行列などのシステムデータ（`ln_` ビルトイン変数で参照）と共通構造体を取り込む |
| (2) `uniform` 宣言 | マテリアルごとのパラメータ・テクスチャ。`$Global`（Set 0）に集約される |
| (3) VSOutput | 頂点→フラグメント間で受け渡す値。セマンティクスは HLSL/Slang 準拠 |
| (4) `[shader("...")]` | エントリポイントのステージを指定する属性 |
| (5) `LUMINO_METADATA` | パス名とエントリポイントの対応を宣言する JSON ブロック |

---

## `import lumino` が提供するもの

`import lumino;` で [lumino.slang](../packages/LuminoShader/shaders/lumino.slang) を取り込むと、
カメラ・オブジェクト行列などのシステムデータへアクセスする `ln_` ビルトイン変数と、共通構造体が
使えます。

### カメラ・行列のアクセス（`ln_` ビルトイン変数）

カメラ行列やワールド行列などのシステムデータは、`ln_` で始まるビルトイン変数で参照します。
宣言は不要で、そのまま使えます。

| ビルトイン変数 | 内容 |
|---|---|
| `ln_World` | ワールド行列（モデル → ワールド） |
| `ln_NormalMatrix` | 法線変換行列 |
| `ln_View` | View 行列（ワールド → ビュー） |
| `ln_Projection` | Projection 行列（ビュー → クリップ） |
| `ln_ViewProjection` | View × Projection（ワールド → クリップ） |
| `ln_ProjectionI` / `ln_ViewProjectionI` | 上記の逆行列 |
| `ln_CameraPosition` | ワールド空間のカメラ位置 (`float3`) |
| `ln_Resolution` | 画面解像度 (`width, height, 1/width, 1/height`) |

```hlsl
float4 worldPos = mul(ln_World, float4(input.position, 1.0));
output.position = mul(ln_ViewProjection, worldPos);
float3 viewDir  = normalize(ln_CameraPosition - worldPos.xyz);
```

> これらのシステムデータは内部的に descriptor set へ分離され、ランタイムが毎フレーム /
> 描画ごとに自動で更新します。内部のバインディング構成（ParameterBlock の構造や set 番号）は
> 変更される可能性があるため、シェーダからは必ず `ln_` ビルトイン変数経由でアクセスしてください。

### 頂点入力 `VSInput`

頂点シェーダの入力は共通の `VSInput` を使います（メッシュの頂点レイアウトと一致）。

```hlsl
struct VSInput {
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD;
    float4 color    : COLOR;
    float4 tangent  : TANGENT;
};
```

### GBuffer ヘルパ

Deferred / SSR 向けの共通 GBuffer レイアウトと、それをパックする `lnPackGBuffer(...)` /
`LNGBufferOutput` が用意されています。詳細は
[lumino.slang](../packages/LuminoShader/shaders/lumino.slang) のコメントを参照してください。

---

## リソースのバインディング構成

Lumino はシェーダリソースを 更新頻度別 に分離して管理します。シェーダ作者が宣言・設定するのは
マテリアルパラメータ（`$Global`）だけ で、それ以外のシステムデータはランタイムが用意します。

| 区分 | 内容 | 誰が用意するか |
|---|---|---|
| `$Global`（マテリアル） | マテリアルごとのパラメータ・テクスチャ・サンプラー（`uniform` で宣言） | シェーダ作者（Material API で値を設定） |
| システムデータ | カメラ・オブジェクト行列など（毎フレーム / 描画ごとに更新） | ランタイム（`import lumino;` が提供） |

システムデータの内部的なバインディング構成（descriptor set 番号や ParameterBlock の構造）は
実装の詳細であり、変更される可能性があります。シェーダからは、セクション 2 の `ln_` ビルトイン変数を
介してアクセスしてください。

---

## マテリアルパラメータ（`$Global` / Set 0）

マテリアルごとに変えたい値・テクスチャは、関数の外で `uniform` として宣言します。これらは
すべて `$Global`（Set 0）にまとめられ、Material API から値を設定します。

### 宣言の書き方

```hlsl
// 定数バッファ（推奨）: 複数の値をまとめる
struct MaterialParams {
    float4 color;       // メンバ名 "color"
    float4 specular;    // メンバ名 "specular"（xyz=色, w=shininess）
};
uniform ConstantBuffer<MaterialParams> u_params;

// 単独 uniform でも可: 変数名がそのままパラメータ名になる
uniform float4 u_myColor;

// テクスチャ / サンプラー
uniform Texture2D    u_baseTexture;
uniform SamplerState u_baseSampler;
```

### パラメータ名の決まり方（重要）

Material API（`setFloat4` 等）に渡す 名前 は、シェーダのリフレクションから決まります。

- `ConstantBuffer<T>` の場合 → 構造体 `T` のフィールド名（例: `color`, `specular`, `ssrSettings`）
- 単独 `uniform` の場合 → 変数名（例: `u_myColor`）
- テクスチャ / サンプラー → 変数名（例: `u_baseTexture`, `u_gbufferA`）

> 慣例: 拡散（メイン）テクスチャのスロットは `u_baseTexture` / `u_baseSampler` という名前を使います。
> Material の「メインテクスチャ」設定（後述）はこのスロットに対応します。

---

## エントリポイントとパス

### `[shader("...")]` 属性

エントリポイント関数には、ステージを示す属性を付けます。

```hlsl
[shader("vertex")]   VSOutput vsMain(VSInput input) { ... }
[shader("fragment")] float4   fsMain(VSOutput input) : SV_TARGET { ... }
[shader("compute")]  void     csMain(...) { ... }
```

### `LUMINO_METADATA` ブロック

パス名とエントリポイントの対応を、`#ifdef LUMINO_METADATA` で囲んだ JSON で宣言します。
`#ifdef` ガードにより Slang コンパイラからは不可視で、コンパイルには影響しません。

```hlsl
#ifdef LUMINO_METADATA
{
    "passes": [
        { "name": "Forward", "vertex": "vsMain", "fragment": "fsMain" },
        { "name": "GBuffer", "vertex": "vsGBufferMain", "fragment": "fsGBufferMain" }
    ]
}
#endif
```

| フィールド | 説明 |
|---|---|
| `passes[].name` | パス名（必須）。`"Forward"`, `"GBuffer"` など |
| `passes[].vertex` | 頂点シェーダのエントリポイント関数名 |
| `passes[].fragment` | フラグメントシェーダのエントリポイント関数名 |
| `passes[].compute` | コンピュートシェーダのエントリポイント関数名 |

- パス名の意味（Forward / GBuffer 等）は上位エンジンが定義します。 Lumino のシェーダ基盤は
  パス名を文字列として扱うだけです。
- 1 つの `.slang` に複数パスを書けます。描画時にパス名で選択します（例: G-Buffer 描画時は
  `shaderPassName: "GBuffer"`）。
- レンダーステート（ブレンド・デプステスト等）は シェーダには含めず、Material 側で指定します
  （WebGPU の設計に倣う）。

---

## 座標変換の規約

シェーダ内の行列演算は [graphics-conventions.md](graphics-conventions.md) と
[LuminoShader/ARCHITECTURE.md](../packages/LuminoShader/ARCHITECTURE.md) の規約に従います。

- 乗算は `mul(M, v)` の順（行列 × ベクトル）。変換は右から左へ適用。

  ```hlsl
  float4 worldPos = mul(ln_World, float4(input.position, 1.0));
  output.position = mul(ln_ViewProjection, worldPos);
  ```

- コンスタントバッファ内での行列の GPU レジスタパッキングは 行優先 (row-major)。転置なしで
  クライアント側（C++/TS）から書き込んだバイト列をそのまま解釈できます (下記の補足を参照)。
- NDC は Z が 0（近）〜1（遠）、Y は上が正。バックエンド差はランタイムが吸収するため、シェーダは
  バックエンドを意識する必要はありません。

> **CPU 側メモリレイアウトとシェーダレジスタパッキングの関係**
>
> [graphics-conventions.md](graphics-conventions.md) では「CPU 側 (`Matrix4x4`) は
> 列優先 (Column-major)」と説明しており、上記の「シェーダ側は行優先」と一見矛盾して見えますが、
> 同じバイト列を 2つの異なる視点 (CPU 配列の並び / GPU レジスタへの詰め方) から説明している
> だけで、矛盾ではありません。
>
> Lumino は Slang のコンパイルオプションに `SLANG_MATRIX_LAYOUT_COLUMN_MAJOR` を指定しています。
> これは CPU 側の列優先バイト列を転置せずにそのまま GPU へ渡すための設定ですが、Slang は
> HLSL 由来の命名を使うため、生成される SPIR-V の装飾は (COLUMN_MAJOR という指定にもかかわらず)
> `RowMajor` になります。詳細な経緯は
> [LuminoShader/ARCHITECTURE.md](../packages/LuminoShader/ARCHITECTURE.md#slang_matrix_layout_column_major-について)
> を参照してください。

---

## Material API からのパラメータ設定

コンパイルした `.lcsh` は `Material` として読み込み、各言語の API でパラメータを設定します。
設定した名前は シェーダ側の `uniform` 名 / CB フィールド名（[4. パラメータ名の決まり方](#パラメータ名の決まり方重要)）に対応します。

### API 対応表

| 目的 | C++ (`ln::Material`) | C (`LuminoC`) | TypeScript (`luminojs`) |
|---|---|---|---|
| 作成（Unlit） | `MaterialFactory::createUnlit(ctx)` | `LNMaterial_CreateUnlit` | `Material.createUnlit()` |
| 作成（コンパイル済み） | `MaterialFactory::createFromCompiledShader(ctx, data, size)` | `LNMaterial_CreateFromCompiledShader` | `Material.createFromCompiledShader(data)` |
| ベースカラー | `setColor(Color)` | `LNMaterial_SetColor` | `setColor(r,g,b,a)` |
| 名前付き float4 | `setFloat4(name, float*)` | `LNMaterial_SetFloat4` | `setFloat4(name, [x,y,z,w])` |
| 名前付き float | `setFloat(name, value)` | — | — |
| 鏡面反射（BasicLit） | `setSpecular(color, shininess)` | — | — |
| メインテクスチャ | `setTexture(tex)` | `LNMaterial_SetMainTexture` | `setMainTexture(tex)` |
| 名前付きテクスチャ | `setNamedTexture(name, tex)` | `LNMaterial_SetNamedTexture` | `setNamedTexture(name, tex)` |
| カリングモード | `setCullMode(mode)` | `LNMaterial_SetCullMode` | `setCullMode(mode)` |
| ブレンドモード | `setBlendMode(mode)` | `LNMaterial_SetBlendMode` | `setBlendMode(mode)` |
| デプステスト | `setDepthTestEnabled(b)` | `LNMaterial_SetDepthTestEnabled` | `setDepthTestEnabled(b)` |
| デプス書き込み | `setDepthWriteEnabled(b)` | `LNMaterial_SetDepthWriteEnabled` | `setDepthWriteEnabled(b)` |

> `setColor` はマテリアルバッファ先頭の `float4`（慣例的に `u_params.color`）に書き込みます。
> `setSpecular` はその次の `float4`（`u_params.specular`）に書き込みます。これら以外の
> パラメータは `setFloat4(name, ...)` で名前指定して設定します。
> `setFloat` / `setSpecular` は現状 C++ コアのみの提供です。

### 対応の具体例

シェーダ宣言と API 呼び出しの対応:

| シェーダ側の宣言 | 設定する API |
|---|---|
| `uniform float4 u_myColor;` | `setFloat4("u_myColor", [0,1,0,1])` |
| `struct SSRParams { float4 ssrSettings; }; ... u_params;` | `setFloat4("ssrSettings", [10,0.05,0.3,128])` |
| `uniform Texture2D u_gbufferA;` | `setNamedTexture("u_gbufferA", tex)` |
| `uniform Texture2D u_baseTexture;` | `setMainTexture(tex)`（慣例スロット） |

### C の例

```c
LNHandle material = LN_NULL_HANDLE;
LNMaterial_CreateFromCompiledShader(graphicsContext,
    shaderData.data(), (uint32_t)shaderData.size(), &material);

const float myColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f }; // green
LNMaterial_SetFloat4(material, "u_myColor", myColor);
```

参照: [CustomShader/Main.cpp](../packages/LuminoC/examples/CustomShader/Main.cpp)

### TypeScript の例

```ts
// コンパイル済みシェーダから作成
const ssrResp = await fetch(new URL("../../public/SSR.lcsh", import.meta.url).href);
const matSSR = Material.createFromCompiledShader(new Uint8Array(await ssrResp.arrayBuffer()));

matSSR.setFloat4("ssrSettings", [10.0, 0.05, 0.3, 128.0]);
matSSR.setNamedTexture("u_gbufferA", gbufferA);
matSSR.setNamedTexture("u_gbufferB", gbufferB);
matSSR.setNamedTexture("u_gbufferC", gbufferC);

// 組み込み Unlit
const mat = Material.createUnlit();
mat.setColor(0.4, 0.4, 0.4, 1.0);
mat.setCullMode(CullMode.None); // 両面描画
```

参照: [ssr/main.ts](../packages/luminojs-examples/src/ssr/main.ts)

---

## コンパイル

### CLI（`luminosc`）

```sh
# input.slang と同じ場所に input.lcsh / input.lcsh.inl を出力
luminosc input.slang

# lumino.slang など import 先のディレクトリを検索パスに追加
luminosc -I path/to/shaders input.slang

# 生成シェーダコード（SPIR-V / WGSL 等）をダンプ
luminosc --dump input.slang
```

| 出力 | 内容 |
|---|---|
| `<name>.lcsh` | 全ターゲットを束ねたバイナリ。ランタイムで読み込む |
| `<name>.lcsh.inl` | C++ に埋め込む hex 配列 |

### ランタイムコンパイル（デスクトップ）

`LUMINO_USE_SLANG` 有効ビルドでは、`.slang` を実行時にコンパイルしてマテリアル化できます。

```cpp
auto mat = MaterialFactory::createFromShaderSourceFile(
    ctx, "MyShader.slang", "path/to/shaders" /* lumino.slang の検索パス */);
```

---

## チェックリスト / よくある間違い

- [ ] `import lumino;` を先頭に書いたか（`ln_` ビルトイン変数 / `VSInput` の参照に必要）
- [ ] カメラ・行列はシステムデータに直接アクセスせず、`ln_` ビルトイン変数経由で参照したか
- [ ] マテリアルパラメータを `uniform` として 関数の外 で宣言したか
- [ ] `LUMINO_METADATA` にすべてのエントリポイントを列挙したか（書き忘れるとパスが見つからない）
- [ ] 行列乗算を `mul(M, v)` の順で書いたか
- [ ] `setFloat4` / `setNamedTexture` に渡す名前が、シェーダの CB フィールド名 / 変数名と一致するか
- [ ] テクスチャと ペアのサンプラー を両方宣言したか（`u_baseTexture` と `u_baseSampler`）
- [ ] レンダーステートはシェーダではなく Material 側で設定したか

