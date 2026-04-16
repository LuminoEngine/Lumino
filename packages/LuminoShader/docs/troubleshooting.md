# Troubleshooting

## textureSample must only be called from uniform control flow

### 症状

WebGPU バックエンドでシェーダーのロード時に次のようなエラーが発生する。

```
[WebGPU] Uncaptured error [type=2]: Error while parsing WGSL: error: 'textureSample' must only be called from uniform control flow
```

### 原因

WebGPU (WGSL) では、`textureSample()` は **uniform control flow** (同一 wave 内の全スレッドが同じ実行パスを通る箇所) からしか呼び出せない。

これは `textureSample()` が mip-map LOD の選択のために暗黙的な微分計算 (implicit derivatives, `dpdx`/`dpdy`) を行うためであり、スレッド間で実行パスが分岐している状態では微分値が定義できない。

典型的に non-uniform control flow になる状況:

- `break` / `continue` / `return` を含むループ内
- 上記の分岐後にある条件式 (`if`) 内
- 早期リターン (`if (...) return ...`) の後続コード

Slang の `.Sample()` メソッドは WGSL では `textureSample()` に変換されるため、同じ制約を受ける。

### 修正方法

ループや条件分岐など non-uniform control flow になりうる箇所では、`.Sample()` の代わりに `.SampleLevel()` を使用して LOD を明示的に指定する。

```slang
// NG: ループや条件分岐内での Sample()
float depth = u_gbufferB.Sample(u_gbufferBSampler, uv).w;

// OK: SampleLevel() で LOD を明示指定
float depth = u_gbufferB.SampleLevel(u_gbufferBSampler, uv, 0).w;
```

`SampleLevel()` は implicit derivatives を使わないため、non-uniform control flow 内でも呼び出せる。SSR のレイマーチなど post-process での参照では LOD=0 固定で問題ない場合がほとんど。

フラグメントシェーダーのトップレベル (ループ・分岐の外) での `.Sample()` 呼び出しは uniform control flow であり変更不要。

### 発生事例

`SSR.slang` のレイマーチループ内での GBuffer サンプリング (`packages/LuminoC/examples/assets/SSR.slang`)。
