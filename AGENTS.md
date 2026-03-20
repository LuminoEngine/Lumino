# AGENTS.md

Lumino はゲームやデジタルアートなどのインタラクティブアプリケーションを構築するためのオープンソースライブラリです。
マルチプラットフォームで動作し複数のプログラミング言語から利用できる API を提供します。

## Project Status

現在は JavaScript で開発する、プラットフォーム移植可能なゲームの基盤として、シンプルな API を提供することを目標に開発しています。

## Communication

会話は日本語で行います。

## Development Commands

```sh
# CMake 設定 (vcpkg ツールチェイン込み)
cmake -B build -G "Visual Studio 18 2026"

# ビルド
cmake --build build --config Release --parallel

# テスト実行
ctest --test-dir build -C Release --output-on-failure
```

## リポジトリアーキテクチャ

- `/packages`: Lumino の機能は複数のパッケージに分割されています。各パッケージには README.md があり、機能の概要や使用方法が説明されています。
- `/docs`: ドキュメントや設計資料を格納する場所です。
- `/legacy`: 古いバージョンのコードです。実装の際の参考にするために保存されています。

## アーキテクチャコンセプト

### ランタイム

- ブラウザ: WebGPU
- デスクトップ: Vulkan + QuickJS
- コンシューマゲーム機: Vulkan-like + QuickJS

### 古い実装と教訓

`legacy` フォルダに入っているのは旧バージョンの Lumino です。 

DirectX12 や Vulkan をバックエンドとしつつ、 DirectX9 時代のグラフィック API と似たものを提供するつもりで開発していましたが、レンダリング中にリソースを変更するなどのために極めて複雑なコードになってしまいました。またネイティブAPIも頻繁に呼び出すため、特に WASM で使っていると JavaScript から Lumino の API を呼び出すオーバーヘッドと、 JavaScript から C++ へエクスポートされた WebGL や WebGPU の関数を呼び出すオーバーヘッドが無視できなくなりました。

ここから得られた教訓は、事前確保された共有メモリを最大限に活用し、API 関数の呼び出し回数を可能な限り抑えたいということです。
WebGPU の API 設計はまさに参考になると考えています。テクスチャなどのリソースを事前確保・構築し、描画のコマンドリストは JavaScript 側でバイナリフォーマットとして Unit8Array 等で構築し、 C++ 側へは「フラッシュ」することで送り出します。


