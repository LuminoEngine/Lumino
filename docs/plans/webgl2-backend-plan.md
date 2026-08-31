# WebGL2 (OpenGL ES 3.0) バックエンド対応 計画書

- ステータス: P0 / P1 / P2 / P3 完了。P4 (デスクトップ ES 3.0 と検証) 待ち
- 作成日: 2026-08-30
- 関連資料:
  - `AGENTS.md` "コアモジュールのメモリ使用の注意点" (WASM の 32bit メモリ制約)
  - `docs/graphics-conventions.md`
  - `docs/shader-conventions.md`
  - `packages/LuminoShader/ARCHITECTURE.md` (座標変換の規約)
  - [gpuweb/gpuweb#5272](https://github.com/gpuweb/gpuweb/issues/5272)

## 1. 背景と目標

### 1.1 きっかけ: ARM Windows で WebGPU が使えない

Windows on ARM (Snapdragon X / Qualcomm Adreno X1 系) の実機で、Chrome / Edge ともに
`navigator.gpu.requestAdapter()` が null を返す。原因はハードやドライバの故障ではなく、
Chromium (Dawn) の WebGPU アダプタブロックリストで Windows ARM64 + Adreno が既定で
ブロックされているため。x86/x64 Windows では既定で有効なので、ARM 機だけの挙動になる。

判明している事実:

- 回避策は `chrome://flags/#enable-unsafe-webgpu` を Enabled にすること。動作は確認済み。
- ブロックリストはユーザーが編集できない。実体は Chromium ソースの
  `gpu/config/software_rendering_list.json` だが、ビルド時に C++ へ変換されバイナリに
  焼き込まれるため、インストール済みブラウザに編集対象のファイルが存在しない。
- 解除のロードマップは存在しない。gpuweb#5272 は 2025-08-06 に
  "working-as-intended that `--enable-unsafe-webgpu` is still needed" として
  クローズされており、修正されたのは仕様ドキュメントの記述だけ。2026-08 時点でも状況は同じ。
- ゲートは Chromium 側ではなく Qualcomm の Adreno ドライバ品質とみられる。
  Qualcomm 自身のフォーラムに Adreno X1-85 の ANGLE/D3D11 描画破損報告があり、
  WebGL 経路すら安定していない。

Web にデプロイしたゲームのプレイヤーに `chrome://flags` を触らせることはできない。
ARM ノート PC は今後増える見込みであり、この制約は当面解消しない前提で設計する。

### 1.2 位置づけの変更: フォールバックから Web の第一級バックエンドへ

当初 WebGL2 は「古いデバイス向けのフォールバック」として検討していたが、上記により
**Web における既定のバックエンド**へ位置づけを変更する。

ただし WebGPU バックエンドは削除しない。既に動作しており、実行時コストもゼロ
(バックエンドの選択のみが変わる) であるため、明示的な opt-in として残す。

自動フォールバック (WebGPU を試して失敗したら WebGL2) は採用しない。プレイヤーの環境ごとに
2 本の描画経路が走ることになり、動作確認の対象が倍になるわりに、2D スプライトと
タイルマップ主体の用途では WebGPU の性能上の利得がほとんど無いため。

### 1.3 目標

- WebGL2 を Web の既定バックエンドとして実装する。
- デスクトップでも OpenGL ES 3.0 で動作させ、Web の主経路をデスクトップ上で再現できる
  デバッグ環境にする。
- Vulkan と WebGPU の**実行時**の処理速度とメモリ使用量を増やさない。

### 1.4 非目標

- WebGL 1.0 (ES 2.0) 対応。
- コンピュートシェーダおよびストレージバッファの WebGL2 上でのエミュレーション。
- 自動バックエンドフォールバック。

### 1.5 前提の確認: WebGL2 と ES 3.0 の関係

WebGL 2.0 は OpenGL ES 3.0 をベースにしており、API レベルはほぼ同一である。
Lumino に影響する差は次の 3 点。

| WebGL2 に無いもの | Lumino への影響 |
|---|---|
| バッファマッピング (`glMapBufferRange`) | 影響なし。RHI は `Buffer` に map を持たせていない (`Rhi.hpp:400-405`) |
| テクスチャスウィズル (`GL_TEXTURE_SWIZZLE_*`) | BGRA8 のエミュレーションができない。5.2 参照 |
| client-side array | 影響なし |

したがって WebGL2 で通る設計にしておけば、デスクトップ ES 3.0 でも同じコードが通る。

## 2. 現状調査の結論

RHI (`packages/LuminoCore/include/LuminoCore/Graphics/rhi/Rhi.hpp`) を調査した結果、
**設計上の致命的な障害は無い**。2026-08-29 に実施した事前整備 (`writeBuffer` の順序契約の
明文化、`RenderPipelineDesc::renderPassLayout` の値化、`SwapChain::format()` の追加) が
効いており、大半の差異はバックエンド内部で吸収できる。

RHI およびシェーダバイナリのスキーマに追加が必要なのは **3.2 の combined sampler の
対応表だけ**であり、これも GLSL ターゲット専用のフィールドにできるため Vulkan / WebGPU への
影響はゼロにできる。

公開 API (`packages/LuminoC/include/LuminoC/lumino.h`) は Texture2D / Shader / Material /
Mesh / Camera / Renderer の粒度で、bind group もパイプラインオブジェクトもコマンド
エンコーダも露出していない。逆に GL 固有概念 (テクスチャユニット番号など) も漏れていない。
バックエンド追加のために公開 API を作り直す必要は無い。

## 3. 対応が必要な設計上の障害

### 3.1 GLSL ES 300 ターゲットが存在しない (作業量の大半)

`ShaderTarget` は SPIRV / DXIL / WGSL / METAL の 4 つ
(`packages/LuminoShader/include/LuminoShader/Common.hpp`)、`rhi::ShaderCodeFormat` は
SPIRV / WGSL のみ (`Rhi.hpp:239`)。

Slang の GLSL 出力は Vulkan 風 GLSL であり ESSL 300 ではないため、
**SPIR-V を SPIRV-Cross (`CompilerGLSL`, version 300 es) に通す**経路を採用する。
SPIRV-Cross は legacy では使用していたが、現行の `vcpkg.json` には含まれていないため追加する。
luminosc (ビルド時ツール) だけの依存であり、ランタイムには載らない。

**先に修正が必要な箇所**: `UnifiedShader2.hpp:85` の `targetShaderPassIds{-1, -1, -1, -1}`
は 4 要素固定で、同 87 行の `getTargetShaderPassId()` が `targetShaderPassIds[target - 1]`
で添字する。5 番目のターゲットを追加すると範囲外アクセスになる。

### 3.2 Texture と Sampler の分離 (combined sampler)

`BindingType::SampledTexture` と `BindingType::Sampler` は別エントリであり、実際のシェーダも
`Texture2D` と `SamplerState` に分かれている
(例: `packages/LuminoC/examples/assets/GBuffer.slang:15-16`)。
ESSL 300 は combined sampler (`sampler2D`) しか持たない。

SPIRV-Cross の `build_combined_image_samplers()` で結合できるが、実行時に
**「(テクスチャの binding, サンプラーの binding) -> テクスチャユニット」の対応表**が必要になる。
現在の `TargetBindingLayout2` にはこれを格納する場所が無い。

**本計画で唯一、シェーダバイナリのスキーマに追加が必要な箇所。**
GLSL ターゲット専用の `TargetBindingLayout2` に載せることで、Vulkan / WebGPU 側は
完全に無変更かつ無コストにできる。具体的なスキーマは P1 で確定した (8 章を参照)。

なお「ビルド時に一度だけ逆引き表を解決して保持する」仕組みは
`ShaderPass::materialSamplerTextureNames()` に既にあるため、発想を流用できる。

### 3.3 NDC Z が 0..1 前提

`Matrix4x4::perspectiveRH` はクリップ空間 Z を 0..1 とする ZO 規約
(`packages/LuminoBase/src/math/Matrix4x4.cpp`)。WebGL2 に `glClipControl` は無い。

対応は SPIRV-Cross の `fixup_clipspace` オプションで、頂点シェーダに
`gl_Position.z = 2.0 * gl_Position.z - gl_Position.w;` を注入する。
シェーダのコンパイル時に完結するため、他バックエンドへの影響は無い。

**残る注意点**: 深度値を直接読む処理 (`invProj` から線形深度を復元する SSR など) は
clip z の定義が変わるため、シェーダ側の前提がずれる。現在の `SSR.slang` はまだそこまで
書かれていないが、書き始める際に踏む。

### 3.4 フレームバッファ原点の差

Vulkan は負の高さのビューポートで +Y を上向きに揃えており
(`packages/LuminoCore/src/Graphics/rhi/vulkan/VulkanBackend.cpp:546`)、WebGPU も +Y 上。
GL も NDC は +Y 上だが、テクスチャの行 0 が下端になるため、レンダーターゲットの内容だけが
上下反転する。

対応は SPIRV-Cross の `flip_vert_y` で `gl_Position.y` を反転し、あわせて
バックエンド側でワインディングの反転と viewport / scissor の y 座標変換を行う。
**RHI の変更は不要。**

**残る注意点**: `gl_FragCoord` の原点。ESSL 300 に `origin_upper_left` は無い。
現在のシェーダはフラグメントステージで SV_POSITION を読んでいないため当面は問題ないが、
読み始めると破綻する。

### 3.5 バックバッファに独自デプスを付ける構成

`GraphicsContext::createForWindow` は「SwapChain のカラー + 自前のデプステクスチャ」で
フレームバッファを組む (`packages/LuminoCore/src/Graphics/GraphicsContext.cpp`)。
WebGL2 の FBO 0 には何もアタッチできない。

対応は SwapChain 実装をオフスクリーン FBO にして、present で既定のフレームバッファへ
blit すること。`acquireNextTexture()` が `TextureView*` を返す契約はそのまま満たせる。
追加コストは WebGL2 のみ。

`SwapChain::format()` は 2026-08-29 の整備で追加済みのため、RGBA8 を返せばよい。

### 3.6 writeBuffer の順序契約により即時発行できない

`Rhi.hpp:586-597` に「即時実行の API を使うバックエンドは、コマンドを記録して submit() 時に
再生することでこの契約を満たすこと。draw を即時に発行してはならない」と明文化済み。

`DynamicUniformAllocator::flushFrame()` が submit 直前に `writeBuffer` するため、
この契約は実際に効く。WebGL2 バックエンドは自前のコマンド記録と再生を実装する必要がある。
コストは WebGL2 のみ。

## 4. RHI 変更は不要だがバックエンド実装で効く点

| 項目 | 対応 |
|---|---|
| VAO | `setVertexBuffer` を描画ごとに呼ぶため、(頂点レイアウト, バッファ, オフセット) をキーにキャッシュするか、毎回属性を張り直す。バックエンド内部の判断 |
| dynamic offset | `glBindBufferRange`。`DeviceLimits` を GL の実値から返せば `DynamicUniformAllocator` が自動追随する (`DynamicUniformAllocator.cpp:23-27`)。ES 3.0 の `MAX_UNIFORM_BLOCK_SIZE` 下限は 16KB なのでページが小さくなるだけ |
| LoadOp / StoreOp | `glClearBufferfv` と `glInvalidateFramebuffer` でアタッチメントごとに対応可能。どちらも ES 3.0 core |
| ShaderModule とプログラム | GL はリンク済みプログラム単位だが、`createRenderPipeline` でリンクすればよい。`PipelineCache` があるため再リンクは起きない。ESSL のエントリポイントは `main` 固定だが、SPIRV-Cross がエントリポイントごとに別ソースを出力するため `TargetEntryPoint2` = 1 blob のモデルと一致する |
| readbackTexture | Y 反転の契約は `Rhi.hpp:600-608` に明記済み。`glReadPixels` のフォーマット制約 (RGBA / UNSIGNED_BYTE 以外は実装依存) に注意 |
| per-attachment blend | RHI は `blendStates` を配列で持つが、`PipelineCache.cpp:138` が常に 1 要素しか設定しないため実質全アタッチメント共通。WebGL2 に `glBlendFunci` は無いが実使用では問題にならない |
| baseVertex / firstInstance | 呼び出し側は常に 0。WebGL2 にはどちらも無いため、非ゼロが来たらエラーを返す |
| mipmap 生成 | RHI に生成の口が無い (`TextureDesc::mipLevels` はある)。GL では `glGenerateMipmap` が必要。将来の課題 |

## 5. 公開 API (lumino.h) への影響

### 5.1 コンピュートとストレージバッファ

現在の `lumino.h` にはコンピュートシェーダもストレージバッファも露出していないため、
今日の時点で壊れるものは無い。制約が効くのは**今後追加する機能**のほう。

| 機能 | WebGL2 で通る書き方 |
|---|---|
| GPU スキニング | UBO にボーン行列を置く (SSBO を使わない)。ES 3.0 の `MAX_UNIFORM_BLOCK_SIZE` 下限 16KB は float4x4 で 256 本に相当 |
| パーティクル | CPU 更新 + 動的頂点バッファ、または transform feedback (ES 3.0 / WebGL2 にある) |
| ポストエフェクト | フラグメントシェーダのみで実装する。コンピュート前提の手法は採らない |

### 5.2 整理が必要な既存の公開定義

| 定義 | 問題 | 対応案 |
|---|---|---|
| `LN_MAX_COLOR_ATTACHMENTS = 8` | ES 3.0 の `MAX_DRAW_BUFFERS` 保証下限は 4 | (P1 で実施) 上限を 4 に下げた。`GBuffer.slang` は 3 本なので実害なし |
| `LN_TEXTURE_FORMAT_BGRA8_UNORM` / `_SRGB` | WebGL2 は内部フォーマットとして持たず、スウィズルも使えないためエミュレート不可 | (P1 で実施) 列挙値は残したまま deprecated にし、RGBA8 に寄せた |
| `LN_TEXTURE_FORMAT_RGBA32_FLOAT` | ES 3.0 では既定で renderable でも filterable でもない (`EXT_color_buffer_float` / `OES_texture_float_linear` 依存) | (P1 で実施) 注記を入れ、RGBA16_FLOAT を推奨とした |
| sRGB のレンダーターゲット | ES 3.0 / WebGL2 に `GL_FRAMEBUFFER_SRGB` の制御が無い (`EXT_sRGB_write_control`)。SRGB8_ALPHA8 は常に変換が入る | Vulkan / WebGPU と挙動が一致するか実測で確認する |

## 6. シェーダバイナリのサイズとメモリ

`ShaderCompiler2.cpp:23-29` の `kTargets` は 4 ターゲットを無条件で全てコンパイルし、
`UnifiedShaderSerializer2` は全 blob を無条件で読み込む。ターゲットによる絞り込みが無い。

GLSL ES を追加すると `.lcsh` は約 25% 増え、その分がそのまま WASM の実行時メモリと、
`.lcsh.inl` 経由でバイナリサイズに乗る。現状すでに DXIL と METAL はバックエンドが無いのに
載っている。

対応は**デシリアライズ時に不要ターゲットの blob をスキップする**こと。
「1 つの `.lcsh` を全プラットフォームで配る」前提を壊さずに済む。
これを WebGL2 の実装より先に片付けると、WebGPU 側のメモリはむしろ減る。

AGENTS.md の "コアモジュールのメモリ使用の注意点" に直結するため、優先度は高い。

## 7. デスクトップでの ES 3.0 実行環境

位置づけが「動作確認しやすいように」から「Web の主経路をデスクトップで再現する主要な
デバッグ環境」へ上がる。

**ANGLE (D3D11 バックエンド) を推奨する。** 素の デスクトップ GL 3.3 core より、
ブラウザの実際の実行経路 (Chromium は Windows で ANGLE を使う) に近い挙動が得られる。
GLFW は既に依存にあるが、GL 関数ローダが無いため新規依存の追加が必要になる。

**要確認事項**: Adreno X1-85 で ANGLE/D3D11 の描画破損報告があることから、
ARM Windows では Vulkan バックエンドも同程度に不安定な可能性がある。
Lumino をネイティブで ARM Windows に出す予定が生じた場合は、実機で別途確認が必要。

## 8. 実装フェーズ分割

### P0: 事前整備 (WebGL2 に依存しない、単独で価値がある) - 完了

1. (完了) `.lcsh` のターゲットフィルタ。`loadFromData` に `ShaderTarget` 引数を追加し、
   指定ターゲット以外のコード blob を実体化しないようにした。blob 本体は位置と長さだけを
   控えて読み飛ばし、TargetEntryPoint を読んだあとで必要なものだけ確保する。
   実行時の呼び出し側 (`ShaderPass` / `Shader` / `GraphicsModule`) は
   `detail::backendToShaderTarget()` でバックエンドのターゲットを渡す。
2. (完了) `targetShaderPassIds` を可変長にした。`ShaderTarget_Last` を追加し、
   コンストラクタでその数だけ確保する。`getTargetShaderPassId()` は、古い `.lcsh` を
   読み込んで配列が短い場合に備えて範囲外なら -1 を返す。

### P1: 設計判断の確定 - 完了

3. (完了) 公開 API の互換性に関わる決定 (WebGL2 を Web の既定とする、自動フォールバックを
   実装しない、MRT 上限 4、BGRA8 の deprecated 化)。記録先は `lumino.h` / `lumino_types.h` /
   `luminojs/src/types.ts` と `docs/graphics-conventions.md`「バックエンドと機能の制約」。
4. (完了) combined sampler の対応表のスキーマ (下記)。clipspace と Y 反転はシェーダの
   コンパイル時に補正する方針 (3.3 / 3.4) を確定し、シェーダ作者から見た影響を
   `docs/shader-conventions.md`「WebGL2 (GLSL ES 300) の制約」に書いた。

#### combined sampler の対応表のスキーマ

`TargetBindingLayout2` に GLSL ターゲット専用の配列を 1 本追加する。

```cpp
// GLSL ES 300 は sampler2D しか持たないため、(テクスチャ, サンプラー) の組ごとに
// 結合後の変数が 1 つ生成される。その対応を保持する。GLSL 以外のターゲットでは空。
struct CombinedSamplerBinding2 {
    std::string name;             // 結合後の GLSL 変数名。glGetUniformLocation で引く
    int16_t textureSetIndex;
    int16_t textureBindingIndex;
    int16_t samplerSetIndex;
    int16_t samplerBindingIndex;
};

struct TargetBindingLayout2 {
    std::vector<TargetBinding2> bindings;
    std::vector<CombinedSamplerBinding2> combinedSamplers; // 添字がテクスチャユニット番号
};
```

- テクスチャユニット番号は配列の添字とする。専用のフィールドは持たない。
- `TargetBinding2` 側にユニット番号を足す案は採らない。テクスチャとサンプラーの組み合わせは
  N 対 M になりうるため、どちらか一方に 1 つの番号として持たせられない。
- GLSL ES 300 には `layout(binding = N)` が無いため、名前からユニフォームロケーションを引いて
  `glUniform1i` でユニット番号を設定する。そのため `name` が必要になる。
- シリアライザは配列を 1 本足すだけ。GLSL 以外のターゲットでは要素数 0 なので、
  Vulkan / WebGPU の `.lcsh` の増加はカウントのフィールド分だけで済む。

### P2: シェーダコンパイルパイプライン - 完了

5. (完了) `vcpkg.json` に SPIRV-Cross を追加。`LUMINO_USE_SLANG` のときだけリンクするため、
   ランタイム (WASM を含む) には載らない。
6. (完了) `ShaderTarget_GLSL_ES300` と `rhi::ShaderCodeFormat::GLSL` を追加。
7. (完了) `ShaderCompiler2::buildGlslEs300Target()` で SPIRV ターゲットの blob を
   SPIRV-Cross に通す。
8. (完了) `CombinedSamplerBinding2` を載せ、`.lcsh` を `FileVersion_4` に上げた。

**実装時に判明した点**: 識別子として使えない文字 (`materialData.baseTexture` の `.` など) の
置き換えは `set_name()` ではなく `compile()` の中で行われる。`glGetUniformLocation` で引く
名前は `compile()` の**あと**に `get_name()` で読み直す必要がある。

### P3: WebGL2 バックエンド - 完了

**P2 で判明した追加の課題**: ESSL 300 には `layout(binding = N)` が無いのは combined sampler
だけでなくユニフォームブロックも同じで、こちらは `glGetUniformBlockIndex` に渡す
**ブロック名**が要る。SPIRV-Cross が出すブロック名は `ViewParams_std140` のように
Slang の**型名**由来であり、`TargetBinding2::name` (インスタンス名。暗黙の CB では空文字列)
からは導けない。

(決定) **`ShaderCompiler2` 側で `set_name()` により書き換える**方を採った。スキーマの追加が
不要で差分が小さい。名前の規則は `shader::glslUniformBlockName()` (`LuminoShader/Common.hpp`)
に置き、コンパイラとバックエンドの両方がこれを使う。

9. (完了) `Backend::WebGL2` を追加。`DeviceFactory_Web.cpp` は WebGL2 / WebGPU の両方を
   生成できるようにした。`backendToShaderTarget` / `backendToCodeFormat` に分岐を追加。
   公開 API には `LN_GRAPHICS_BACKEND_WEBGL2` を追加した (Web の既定はまだ WebGPU のまま。
   ブラウザでの動作確認が済む P4 で切り替える)。
10. (完了) Device / Buffer / Texture / TextureView / Sampler / ShaderModule /
    PipelineLayout / BindGroup / RenderPipeline を
    `packages/LuminoCore/src/Graphics/rhi/webgl2` に実装。
11. (完了) コマンド記録と再生 (3.6)、オフスクリーン FBO + present blit (3.5)。
    VAO は使わず、描画ごとに頂点属性を張り直している (ES 3.0 / WebGL2 は既定 VAO が使えるため)。
    描画数が増えて JS 呼び出しがボトルネックになったら VAO キャッシュへ差し替える。
12. (完了) `readbackTexture`。行の反転は不要だった。`flip_vert_y` によりレンダーターゲットの
    行 0 が画像の最上段になるため、`glReadPixels` が返す順序がそのまま RHI の契約と一致する。
    同じ理由で viewport / scissor の y 座標変換も不要になった (4 章の想定から変わった点)。

**P3 で追加した RHI / 公開 API の変更**

- `rhi::PipelineLayoutDesc::combinedSamplers` (GLSL 専用。他のバックエンドでは空)。
  `ShaderPass` が `.lcsh` の対応表から詰める。
- `rhi::DeviceDesc::canvasSelector` と `LNInstanceInitializeSettings::canvasSelector`。
  WebGL のコンテキストは canvas に結び付いており、デバイスの生成直後に組み込みシェーダと
  既定テクスチャを作るため、SwapChain の生成を待てない。
  `LNInstanceInitializeSettings` のサイズが 8 -> 12 バイトに変わったため、
  `types.ts` の `SIZEOF_INSTANCE_INIT_SETTINGS` も更新した。

**未検証の点** (P4 で確認する)

- ブラウザ上での実描画。ビルドと GLSL 生成までしか確認していない。
- 頂点シェーダとフラグメントシェーダで同じユニフォームブロックのメンバ構成が一致するか。
  ずれていると GLSL ES 300 のリンクが失敗する。
- sRGB レンダーターゲットの挙動が Vulkan / WebGPU と一致するか。

### P4: デスクトップ ES 3.0 と検証

13. ANGLE + GL ローダの導入、デスクトップでの ES 3.0 実行。
14. 既存のビジュアルテスト (`packages/LuminoC/test`) を WebGL2 バックエンドで通す。

## 9. 決定事項と要判断事項

### 決定事項

- WebGL2 を Web の既定バックエンドとする。WebGPU は削除せず明示的な opt-in として残す。
- 自動バックエンドフォールバックは実装しない。
- 公開 API (`lumino.h`) の粒度は現状のまま維持する。作り直しは不要。
- GLSL ES の生成経路は SPIR-V から SPIRV-Cross。Slang の GLSL 出力は使わない。
- Y 反転と clipspace の補正はシェーダのコンパイル時に行う (SPIRV-Cross のオプション)。
  実行時のコストを他バックエンドに持ち込まない。
- 決定内容は ADR ではなく公開ヘッダと規約ドキュメントに書く。ここで決まる制約はクライアントが
  日常的に踏むものなので、別ファイルの決定記録より目に入りやすい。

### 要判断事項

- デスクトップの GL ローダの選定 (ANGLE 前提で glad / EGL のどちらを使うか)。
- sRGB レンダーターゲットの挙動が Vulkan / WebGPU と一致するかの実測。
