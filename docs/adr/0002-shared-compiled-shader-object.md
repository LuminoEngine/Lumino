# 0002: コンパイル済みシェーダを Shader オブジェクトとして共有する

- 状態: 承認
- 日付: 2026-07-30
- 関連: `packages/LuminoCore/include/LuminoCore/Graphics/Shader.hpp`, [shader-conventions.md](../shader-conventions.md)

## 文脈

Material のパラメータ (`setFloat4` 等) はマテリアル単位の uniform buffer に載り、フレーム
スロットもフレーム単位のため、同一フレーム内で同じ Material を複数回描画するとパラメータは
後勝ちになる。そのためクライアントは「1 フレーム内で異なるパラメータで描く箇所の数」だけ
Material を用意する必要がある。テクスチャごとに Material を分ける場合も同様に数が増える。

一方 `MaterialFactory::createFromCompiledShader` は Material ごとに ShaderPass
(GPU シェーダモジュール 2 個 + パイプラインレイアウト 1 個) を新規生成していた。結果として
「(ポストエフェクト種別) x (Viewport 数)」がそのまま GPU シェーダモジュール数になり、
bloom の縮小段・tonemap・カラーグレード・被写界深度を追加する段階で無視できなくなる。

ビルトインシェーダは既に `GraphicsModule` が ShaderPass を保持して共有しており、
共有の仕組み自体は存在していた。コンパイル済みシェーダ経路だけがその恩恵を受けていなかった。

## 検討した選択肢

- **A. コンパイル済みシェーダを保持する共有オブジェクト (`Shader`) を追加する。**
  ビルトインと同じ「ShaderPass を共有して Material を作る」形に揃える。
- **B. per-draw パラメータを実装する。** 描画呼び出しごとにマテリアルパラメータを
  リングバッファへ書き、dynamic offset で参照する。Material を増やす必要自体が無くなる。
  ただし (1) スプライトのバッチングは `Material*` をキーにしているためパラメータごとに
  バッチが分断される (またはバッチキーにパラメータのハッシュを含める必要がある)、
  (2) BindGroup にはテクスチャも含まれるため「テクスチャごとに Material」の問題は解決しない、
  (3) 描画コマンドごとにパラメータのスナップショットを持つメモリコストがかかる。
- **C. データバッファをキーに Shader を暗黙キャッシュする。** 呼び出し側の API を変えずに
  済むが、キャッシュのライフタイムが不透明で、同じ内容の別バッファでは効かない。

## 決定

A を採用した。`ln::Shader` (C: `LNShader_CreateFromCompiledShader` /
TS: `Shader.createFromCompiledShader`) が ShaderPass 群を 1 度だけ構築して保持し、
`MaterialFactory::createFromShader` (C: `LNMaterial_CreateFromShader` /
TS: `Material.createFromShader`) がそれを共有する Material を安価に作る。
B は将来の課題として先送りする。

既存の `createFromCompiledShader` は後方互換のため残し、内部ではその Material 専用の
`Shader` を経由する (挙動は従来と同じ = 呼び出しごとに新規生成)。

## 理由

- A は既存の共有機構 (ビルトインシェーダ) と同じ形で、Renderer 側の変更が不要だった。
  `PipelineCacheKey` は `ShaderPass*` をキーにしているためパイプラインまで共有され、
  マテリアル BindGroup は `(Material, ShaderPass)` をキーにキャッシュされるため、
  パラメータの独立性はそのまま保たれる。
- A はパラメータ起因のケースとテクスチャ起因のケースの両方に効く。B はパラメータ起因の
  ケースにしか効かず、バッチングとの相互作用という新しい設計判断を必要とする。
- 「Material を増やしても安い」状態にしておけば、B を実装するかどうかは後から
  実測 (`shaderPassCount` と `drawCallCount`) を見て決められる。

## 影響・トレードオフ

- ABI 変更: `LNGraphicsProfiler` に `shaderPassCount` (生存 ShaderPass 数) を追加した
  (12 -> 16 バイト)。共有できているかの計測手段が無いと、この決定は検証できないため。
  `types.ts` の `SIZEOF_GRAPHICS_PROFILER` とシリアライズ処理も同期させている。
- `ShaderPass::buildFromUnifiedShader` はエントリポイント名を move していたため、同一の
  `UnifiedShader2` から複数パスを構築すると 2 つ目以降が名前を失う不具合があった
  (`Shader` が全パスを一括構築するようになって初めて踏む経路)。コピーに変更した。
- luminojs の `Shader` は Residency 管理の対象で、その Shader を使う Material が
  ensure されるたびに最終使用フレームが更新される。全 Material が使われなくなって初めて
  evict され、再生成後も再び共有される。
- 将来 B (per-draw パラメータ) を検討する場合は、per-draw で dynamic offset を使う
  `objectData` の経路が既に存在するため、まずは非バッチ描画向けにそこを拡張する形が
  影響範囲が小さい。バッチング (`Batch.hpp` の `Material*` キー) との整合が主要な論点になる。
