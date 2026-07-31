# 0003: 生成した WGSL をコンパイル時に Dawn で検証する

- 状態: 承認
- 日付: 2026-07-31
- 関連: `packages/LuminoShader/src/WgslValidator.cpp`, [shader-conventions.md](../shader-conventions.md#wgslwebgpu-の制約)

## 文脈

クライアント (LYRIDRA) でカスタムシェーダを書いた際、画面が全面真っ黒になる不具合があった。
原因は WGSL の制約違反で、早期 `return` を挟んだ後にテクスチャをサンプルしていた。

```hlsl
if (debugParams.x > 0.5) {
    if (...) { return float4(1, 0, 0, 1); }
}
const float4 gbA = u_gbufferA.Sample(u_gbufferASampler, screenUv);   // ここで違反
```

問題は次の 3 点だった。

1. `luminosc` のコンパイルが成功してしまう。Slang は WGSL 固有の制約を検査しない。
2. 実行時エラー (ブラウザのコンソール) にシェーダ名もパス名も含まれない。
   行番号は生成された WGSL のもので、元の `.slang` とは対応しない。
3. `[Invalid RenderPipeline]` が毎フレーム大量に流れ、最初のエラーが埋もれる。

## 検討した選択肢

**A. Tint を単体でビルドしてリンクする** — 却下。Tint を単体で取得するには Dawn の
ソースツリー全体 (depot_tools / gclient) が必要で、現在のプリビルド取得構成
(`WEBGPU_BUILD_FROM_SOURCE=OFF`) を捨てることになる。依存が重すぎる。

**B. naga (wgpu) を使う** — 却下。Rust ツールチェーンが増えるうえ、Dawn とは別実装なので
通す/弾く境界がズレうる。ブラウザ (Chrome) の WebGPU は Dawn なので、Dawn で検証したい。

**C. 既にリンクしている Dawn の Null バックエンドで検証する** — 採用。

**D. 検証は諦めて、実行時エラーの識別情報だけ改善する** — 却下。効果が大きいのは
コンパイル時に落とすことなので、C が現実的である以上あわせて実施する。

## 決定

`ShaderCompiler2` が生成した WGSL を、Dawn の **Null バックエンド**で作ったデバイスの
`wgpuDeviceCreateShaderModule` に通して検証する (`WgslValidator`)。
エラーはエラースコープ (`PushErrorScope` / `PopErrorScope`) で同期的に取り出し、
行番号付きの診断は `ShaderModuleGetCompilationInfo` から取得する。

あわせて、実行時に「どのシェーダか」が分かるよう次を実施した。

- `.lcsh` (v3) にソースファイル名を記録し (`UnifiedShader2::sourceName`)、
  GPU オブジェクトのラベルへ伝播させる。ShaderModule は
  `<シェーダ名>:<パス名>:<エントリポイント名>`、RenderPipeline は `<シェーダ名>:<パス名>`。
- `WebGPUDevice` の uncaptured error ハンドラで、同一メッセージの出力を先頭 3 件に制限する。

## 理由

Null バックエンドを選んだ理由:

- **依存が増えない**。`webgpu_dawn.dll` はデスクトップビルドの WebGPU バックエンドとして
  既に取得・リンクされている。
- **本番と同じ実装で検証できる**。ブラウザ (Chrome) の WebGPU も Dawn である。
- **GPU が要らない**。Null バックエンドは実デバイスを開かないため、CI やヘッドレス環境でも
  動く。uniformity analysis は Tint のリゾルバで行われるのでバックエンドに依存しない。

実行時の識別情報をラベルに載せたのは、WebGPU の検証エラーが
`[Invalid ShaderModule "<label>"]` / `[Invalid RenderPipeline "<label>"]` の形で
ラベルを含むためである。ラベルを埋めるだけで、ブラウザのコンソール出力から
シェーダを特定できるようになる。

## 影響・トレードオフ

- luminosc の配布物に `webgpu_dawn.dll` (約 9MB) が加わる。`slang-compiler.dll` が既に
  約 25MB あるため、相対的な増加は許容範囲と判断した。
- WGSL の検証は 1 シェーダあたり 50ms 程度のコスト。既存のシェーダはすべて通ることを確認済み。
- 検証は WGSL ターゲットに対してのみ行う。SPIR-V / DXIL / Metal 固有の制約は対象外。
- 診断の行番号は生成された WGSL のものであり、`.slang` の行番号ではない。
  そのため検証失敗時は生成 WGSL を `<name>.slang.dump/WGSL.<entryPoint>.wgsl` に書き出す。
  元のソースへのマッピングは行っていない (Slang が対応情報を提供していないため)。
- CMake オプション `LUMINO_SHADER_VALIDATE_WGSL` (既定 ON、`webgpu` ターゲットがある
  デスクトップビルドのみ) と、`luminosc --no-validate-wgsl` /
  `ShaderCompiler2::setWgslValidationEnabled(false)` で無効化できる。

### 見送ったこと

- **Slang 側の自動修正** (`SampleLevel` への置き換え、サンプリングの巻き上げ)。
  今回のスコープ外。まず「気づける」ようにするのが目的で、
  自動書き換えは意図しない描画結果の変化を招く。
- **Vulkan バックエンドのデバッグオブジェクト名** (`vkSetDebugUtilsObjectName`)。
  リポジトリに debug utils の仕組みがまだ無く、今回の問題 (WebGPU 固有) にも効かない。
  Vulkan 経路には代わりに `ShaderPass` / `PipelineCache` のエラーメッセージ側で
  シェーダ名・パス名・エントリポイント名を載せた。
- **C-API / TypeScript API へのシェーダ名パラメータの追加**。`.lcsh` に名前を埋めたことで
  JS 経路でも自動的に名前が付くため不要と判断した。C++ 側には任意指定できる
  オーバーロード引数だけ用意してある。
