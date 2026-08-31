# AGENTS.md

Lumino はゲームやデジタルアートなどのインタラクティブアプリケーションを構築するためのオープンソースライブラリです。
マルチプラットフォームで動作し複数のプログラミング言語から利用できる API を提供します。

## Project Status

現在は JavaScript で開発する、プラットフォーム移植可能なゲームの基盤として、シンプルな API を提供することを目標に開発しています。

現在はまだ正式リリースされておらずプロトタイプの開発段階です。API は変更する可能性があります。
内部処理も、必要であれば大きく設計を変更する可能性があります。

## Communication

- 会話は日本語で行います。
- コメント内で `—` や `–`, `〜` という文字を使わないでください。 ASCII のハイフン `-` を使用してください。
- Lumino の最初のクライアントは日本のユーザーです。コードのコメントはすべて日本語で書いてください。

## Development Commands

```sh
# CMake 設定
cmake -B build/lumino-x64-windows

# ビルド
cmake --build build/lumino-x64-windows --config Release --parallel

# テスト実行
ctest --test-dir build/lumino-x64-windows -C Release --output-on-failure

# シェーダのコンパイル (共有ライブラリを -I で指定する必要があります)
build/lumino-x64-windows/packages/luminosc/Release/luminosc.exe -I packages/LuminoShader/shaders MyShader.slang
```

## リポジトリアーキテクチャ

- `/packages`: Lumino の機能は複数のパッケージに分割されています。各パッケージには README.md があり、機能の概要や使用方法が説明されています。
- `/docs`: ドキュメントや設計資料を格納する場所です。
- `/legacy`: 古いバージョンのコードです。実装の際の参考にするために保存されています。

## コーディングルール

[coding-rules.md](docs/coding-rules.md) を参照してください。

## アーキテクチャコンセプト

### ランタイム

現状 (実装済み) のグラフィックスバックエンドは次の通りです。

- ブラウザ: WebGPU (WASM 経由)
- デスクトップ: Vulkan

以下は将来構想です。QuickJS の組み込みやコンシューマゲーム機向けバックエンドはまだ実装されて
いません。現状はデスクトップ・ブラウザとも、クライアントは TypeScript/JavaScript で記述し、
`luminojs` (WASM バインディング) 経由で Lumino を呼び出す構成です。

- デスクトップ: Vulkan + QuickJS
- コンシューマゲーム機: Vulkan-like + QuickJS

### C-API

Lumino の機能は C-API として提供します。 C-API は次のヘッダファイルを参照してください。

- packages\LuminoC\include\LuminoC\lumino.h

この C-API の基本的なライフサイクルは次のサンプルプログラムを参照してください。

- packages\LuminoC\examples\ClearScreen\Main.cpp

### 古い実装と教訓

`legacy` フォルダに入っているのは旧バージョンの Lumino です。

DirectX12 や Vulkan をバックエンドとしつつ、 DirectX9 時代のグラフィック API と似たものを提供するつもりで開発していましたが、レンダリング中にリソースを変更するなどのために極めて複雑なコードになってしまいました。またネイティブAPIも頻繁に呼び出すため、特に WASM で使っていると JavaScript から Lumino の API を呼び出すオーバーヘッドと、 JavaScript から C++ へエクスポートされた WebGL や WebGPU の関数を呼び出すオーバーヘッドが無視できなくなりました。

ここから得られた教訓は、事前確保された共有メモリを最大限に活用し、API 関数の呼び出し回数を可能な限り抑えたいということです。
WebGPU の API 設計はまさに参考になると考えています。テクスチャなどのリソースを事前確保・構築し、描画のコマンドリストは JavaScript 側でバイナリフォーマットとして Unit8Array 等で構築し、 C++ 側へは「フラッシュ」することで送り出します。

### コアモジュールのメモリ使用の注意点

C/C++ で実装されている次のモジュールはコアモジュールに該当します。
これらは WASM にコンパイルされて JavaScript から呼び出されることを想定しています。

- packages\LuminoBase
- packages\LuminoCore
- packages\LuminoC

2026年の時点では WASM は 32bit アドレス空間で動作するうえ、環境によってはメモリの上限がより小さく制限されることもあります。デバイスロスト時にリソースを再アップロードするためにビットマップデータなどをキャッシュすることがありますが、これらはできるだけ、C/C++ 側ではなく JavaScript 側で管理することを推奨します。


