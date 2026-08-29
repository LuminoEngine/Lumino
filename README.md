Lumino
==========

Lumino is a platform abstraction library for game development. [Lyric Dragon RPG Editor](https://www.lyridra.com/) is currently our primary client. The documentation is written in Japanese, as it is intended for reference by their development team.

Luminoは、ゲーム開発向けのプラットフォーム抽象化ライブラリです。現在、LYRIDRA社が当社の主要なクライアントです。そちらの開発チームが参照することを想定しているため、ドキュメントは日本語で作成されています。

このリポジトリには現在以下のものが含まれています:

- LuminoCore: コアモジュール (C++)
- LuminoC: C API (FFI 境界)
- luminosc: シェーダコンパイラ (Input: slang)
- luminojs: JavaScript / TypeScript バインディング

## バックエンドおよびプラットフォームのサポート

- Platform
  - Windows
  - Web Browser

- Graphics
  - Vulkan
  - WebGPU

- Input (TODO)
- Audio (TODO)

## ドキュメント

- [グラフィックス基本仕様](./docs/graphics-conventions.md)

## ビルド

```sh
# vcpkg 準備
git clone -b 2026.03.18 https://github.com/microsoft/vcpkg.git ./build/vcpkg
./build/vcpkg/bootstrap-vcpkg.bat

# webgpu 準備
git clone https://github.com/eliemichel/WebGPU-distribution.git ./build/webgpu
git -C ./build/webgpu checkout 17dcd42a7683355e7a40ac4e97e77f36dff5b5ab

# CMake 設定
cmake -B build/lumino-x64-windows -G "Visual Studio 18 2026" -A x64 -DCMAKE_TOOLCHAIN_FILE=[PATH_TO_REPO]/build/vcpkg/scripts/buildsystems/vcpkg.cmake

# ビルド
cmake --build build/lumino-x64-windows --config Release --parallel

# テスト実行
ctest --test-dir build/lumino-x64-windows -C Release --output-on-failure
```

```sh
# wasm ビルド
git clone -b 5.0.5 https://github.com/emscripten-core/emsdk.git ./build/emsdk
./build/emsdk/emsdk install 5.0.5
./build/emsdk/emsdk activate 5.0.5
python scripts/build_wasm.py           # Release ビルド (最適化 -O2)、build/lumino-wasm32-emscripten に生成
python scripts/build_wasm.py --debug   # Debug ビルド (最適化なし、ソースマップ付き)、build/lumino-wasm32-emscripten-debug に生成

# TypeScript バインディングのビルド
cd packages/luminojs
npm install
npm run build
npm run test
npm run test:smoke

# サンプルの実行
cd packages/luminojs-examples
npm install
npm run dev
```

## FAQ

### Windows でパスの上限エラーでビルドできない

`MSB4023 OS のパスの上限を越えています。完全修飾のファイル名は 260 文字以下にする必要があります。` のようなビルドエラーが表示されることがあります。これは dawn のビルド成果物の中に非常に長いパスが含まれるためです。

1. Win + R キーを押し、gpedit.msc と入力してEnter
2. コンピューターの構成 > 管理用テンプレート > システム > ファイル システム から [Win32の長いパスを有効にする] を有効にする。
