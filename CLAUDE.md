# CLAUDE.md

このファイルは、Claude Code (claude.ai/code) がこのリポジトリ内のコードを操作する際のガイダンスを提供します。


## リポジトリアーキテクチャ

Lumino は、以下の主要なアーキテクチャレイヤーを備えたマルチプラットフォームゲームエンジンライブラリです。

### コアパッケージ構造
- **LuminoCore**: 基盤レイヤー (演算、I/O、スレッド、シリアル化、文字列)
- **LuminoEngine**: コアエンジンモジュール (グラフィックス RHI、オーディオ、レンダリング、プラットフォーム抽象化)
- **LuminoFramework**: 高レベルゲームフレームワーク (シーン、物理演算、UI、エフェクト)
- **LuminoFFI**: 他言語との統合のための C API バインディング
- **lumino-js**: JavaScript/WebAssembly バインディング

### 主要なアーキテクチャコンセプト

**マルチプラットフォーム設計**: エンジンは、Windows (DirectX12/OpenGL)、Linux/macOS (OpenGL/Vulkan)、WebAssembly (WebGL/WebGPU)、Android/iOS (OpenGL ES) をサポートします。

**グラフィックス RHI**: 複数のバックエンドをサポートするハードウェア抽象化レイヤー(DirectX12、Vulkan、OpenGL、WebGL、WebGPU)

**アセットパイプライン**: メッシュ、テクスチャ、シェーダー、オーディオをサポートするランタイムアセットのロードとコンパイル

**コンポーネントアーキテクチャ**: ゲームオブジェクト用のエンティティコンポーネントシステムを備えたシーングラフ

**モダンC++17**: モダンC++機能、STLコンテナ、スマートポインタ、オプショナル型を使用

### パッケージ依存関係
- LuminoCore → 内部依存関係なし
- LuminoEngine → LuminoCore
- LuminoFramework → LuminoEngine + LuminoCore
- LuminoFFI → LuminoEngine + LuminoCore

### 外部依存関係
vcpkg経由で管理: zlib、libpng、freetype、Vulkan、glslang、SPIRV-Cross、OpenAL、bullet3、box2d、ImGui、glTF、OBJローダー

### シェーダーシステム
- プライマリ: HLSL/GLSL へのクロスコンパイルが可能な Slang シェーディング言語
- レガシー: HLSL エフェクトシステム
- 場所: `packages/LuminoEngine/shader/`

### プラットフォーム抽象化
プラットフォーム固有のコードは `Platform/` ディレクトリに分離され、マネージャークラスによって Windows (Win32)、Linux/macOS (GLFW)、Web (Emscripten) 間で統一された API が提供されます。

## 開発ワークフロー

1. **セットアップ**: `docs/how-to-build.md` に記載されているとおり、vcpkg をリポジトリのルートにクローンします。
2. **ビルド**: テストを含む完全なビルドには、`dotnet run -- Bootstrap` を使用します。
3. **イテレーション**: エンジンのみのリビルドには、`dotnet run -- BuildEngine` を使用します。
4. **テスト**: エンジンのビルド中に ctest を介してテストが自動的に実行されます。
5. **Web**: `packages/lumino-js/` で別の npm ビルドプロセスを使用します。

## 重要なファイルの場所

- メイン CMake: `CMakeLists.txt`
- コアエンジン: `packages/LuminoEngine/src/`
- パブリックヘッダー: `packages/*/include/`
- サンプル: `packages/*/examples/`、`examples/`
- テスト: `packages/*/test/`
- ビルドシステム: `tools/LuminoBuild/`
- ドキュメント: `docs/`
