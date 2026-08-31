# グラフィックス基本仕様

Lumino でグラフィックスプログラムを書く際に理解しておくべき基本仕様をまとめます。

基本的には [glTF](https://github.com/khronosgroup/gltf) 及びそのエコシステムに従います。

## 座標系

Lumino は **右手座標系** を採用しています。

- **X 軸**: 右方向が正
- **Y 軸**: 上方向が正
- **Z 軸**: 手前方向が正（カメラはデフォルトで -Z 方向を見る）

これは OpenGL や Godot と同じ規約です。

> glTF:
> - https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#coordinate-system-and-units
> - https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#projection-matrices

## 面の表裏（ワインディングオーダー）

**反時計回り (Counter-Clockwise, CCW)** で定義された面が正面です。

- デフォルト: `FrontFace = CCW`, `CullMode = Back`
- 右手座標系において、右ねじの法則に従って法線が手前を向く方向が正面となります

三角形の頂点を反時計回りに並べると、その面がカメラに向かって表示されます。

> glTF:
> - https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#instantiation

## NDC（正規化デバイス座標）

Lumino の NDC は以下の範囲です:

| 軸 | 範囲 |
|---|---|
| X | -1（左）〜 +1（右） |
| Y | -1（下）〜 +1（上） |
| Z | 0（近）〜 1（遠） |

### バックエンドごとの差異吸収

上記に従うよう、バックエンドの API やシェーダコードのインジェクションを使い、調整を行います。例えば Vulkan はデフォルトで Y 軸が下向き正ですが、ビューポートの高さを負にすることで Y 軸を反転させています。（そのため動作環境は VK_KHR_maintenance1 のサポートが必要です）

WebGL2 (OpenGL ES 3.0) は NDC の Z 範囲とレンダーターゲットの行の並びが異なりますが、これはシェーダのコンパイル時に補正します。

これにより、クライアントコードやシェーダーはバックエンド (Vulkan / WebGPU / WebGL2 等) の違いを意識する必要がありません。

## 深度（デプス）

- **深度範囲**: 0（近クリップ面）〜 1（遠クリップ面）
- **デフォルトクリア値**: `1.0f`（遠クリップ面でクリア）

## スクリーン座標とテクスチャ座標

### スクリーン座標

- **原点**: 左上
- X は右方向、Y は下方向に増加します

### テクスチャ座標 (UV)

- **原点**: 左上
- U は右方向（0〜1）、V は下方向（0〜1）に増加します

```
(0,0)-----(1,0)
  |         |
  |         |
(0,1)-----(1,1)
```

> glTF:
> - https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#images

## 行列

### メモリレイアウト

**列優先 (Column-major)** で格納されます。

```
// メモリ上の並び: m[0]〜m[15]
// 論理的には以下の配置
| m[0] m[4] m[8]  m[12] |
| m[1] m[5] m[9]  m[13] |
| m[2] m[6] m[10] m[14] |
| m[3] m[7] m[11] m[15] |
```

主な計算は [glm](https://github.com/g-truc/glm) と同様の結果になるようテストされています。

> glTF:
> - https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#data-alignment

> **CPU 側メモリレイアウトとシェーダレジスタパッキングの関係**
>
> [shader-conventions.md](shader-conventions.md) では「シェーダ側は行優先 (row-major)」と
> 説明していますが、これはこのページの「列優先」と矛盾していません。同じバイト列を
> 2つの異なる視点 (CPU 配列の並び / GPU レジスタへの詰め方) から説明しているだけです。
>
> - **CPU 側 (このページの主題)**: `Matrix4x4` の `m[16]` 配列上での値の並び方。列優先
>   (`m[0]` から `m[3]` が第1列)。
> - **シェーダ側**: Slang コンパイラがコンスタントバッファ内で行列を GPU レジスタへ
>   詰める際の規約。Lumino は `SLANG_MATRIX_LAYOUT_COLUMN_MAJOR` を指定していますが、
>   Slang は HLSL 由来の命名を使うため、生成される SPIR-V の装飾は `RowMajor` になります。
>
> **転置は行われません**。CPU 側で書き込んだ列優先のバイト列は、そのまま (バイト単位で)
> GPU に渡され、GPU 側はそれを「行優先」の詰め方として解釈します。「列優先」「行優先」は
> 同じデータに対する別の慣習上のラベルです。詳細な経緯は
> [LuminoShader/ARCHITECTURE.md](../packages/LuminoShader/ARCHITECTURE.md#slang_matrix_layout_column_major-について)
> を参照してください。

### 乗算順序

**左から乗算** する規約です。変換は右から左へ適用されます。

```
// C++ 側
viewProj = projection * view;

// シェーダー側 (Slang/HLSL)
output.position = mul(viewProj, worldPos);  // matrix * vector
```

## バックエンドと機能の制約

Lumino が想定するバックエンドは Vulkan (デスクトップ) と WebGPU / WebGL2 (Web) です。
上記の座標系や NDC、UV の規約はどのバックエンドでも同じですが、**使える機能の上限**は
最も制約の厳しい WebGL2 (OpenGL ES 3.0) に合わせています。

### バックエンドの選択

| プラットフォーム | 既定 | 明示指定できるもの |
|---|---|---|
| デスクトップ | Vulkan | - |
| Web | WebGPU (暫定。WebGL2 へ移行予定) | WebGL2 / WebGPU |

Web の既定を WebGL2 にするのは、Windows on ARM の Chrome / Edge が WebGPU を既定で
無効にしているためです。Chromium のアダプタブロックリストによるもので、プレイヤーが
`chrome://flags` を変更しない限り WebGPU は使えず、解除のロードマップもありません。
WebGL2 バックエンドの実装は入りましたが、ブラウザでの動作確認が済むまでは既定を
切り替えず、`LN_GRAPHICS_BACKEND_WEBGL2` を明示した場合のみ有効になります。

### WebGL2 は canvas を初期化時に決める

WebGL のコンテキストは canvas に結び付いており、あとから別の canvas へ移せません。
Lumino は初期化の直後に組み込みシェーダと既定テクスチャを構築するため、描画先の canvas は
`LNInstance_Initialize` (TypeScript では `Runtime.initialize`) の時点で決まっている必要があります。

```ts
await Lumino.initialize({
    backend: GraphicsBackend.WEBGL2,
    canvasSelector: "#my_canvas",
});
const ctx = await GraphicsContext.createFromCanvas("#my_canvas");
```

`canvasSelector` を省略すると Emscripten の既定 canvas (`"#canvas"`) を使います。
あとから別の canvas で `GraphicsContext` を作ろうとした場合は警告を出し、初期化時の
canvas をそのまま使います。WebGPU と Vulkan はサーフェスを SwapChain の生成時に作るため、
この指定を必要としません。

**実行時の自動フォールバックは行いません。** 指定したバックエンドが使えない環境では
初期化がエラーになります。自動で切り替えると、プレイヤーの環境ごとに 2 本の描画経路が
走ることになり、2D 主体の用途では得られる性能の差に見合わないためです。

### 機能の上限

| 項目 | 上限 / 方針 | 理由 |
|---|---|---|
| カラーアタッチメント (MRT) | 4 (`LN_MAX_COLOR_ATTACHMENTS`) | ES 3.0 の `MAX_DRAW_BUFFERS` の保証下限 |
| コンピュートシェーダ | 使いません | ES 3.0 に無い |
| ストレージバッファ (SSBO) | 使いません | 同上。ボーン行列などは定数バッファ (UBO) に置きます |
| アタッチメントごとの個別ブレンド | 使いません | WebGL2 に `glBlendFunci` が無い |

この上限は今後追加する機能にも適用します。シェーダを書く側から見た制約は
[shader-conventions.md](shader-conventions.md) の「WebGL2 (GLSL ES 300) の制約」にまとめています。

### テクスチャフォーマット

| フォーマット | 状態 |
|---|---|
| `RGBA8_UNORM` / `_SRGB` | 推奨。どのバックエンドでも使えます |
| `BGRA8_UNORM` / `_SRGB` | **非推奨**。WebGL2 は内部フォーマットとして持たず、テクスチャスウィズルも使えないためエミュレートできません。列挙値はスワップチェーンのフォーマットとして返ることがあるため残していますが、自分で指定しないでください |
| `RGBA16_FLOAT` | HDR のレンダーターゲットにはこれを使ってください |
| `RGBA32_FLOAT` | ES 3.0 では既定でレンダーターゲットにもフィルタリング対象にもできません (拡張依存) |
