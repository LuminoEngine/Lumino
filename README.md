Lumino
==========

Lumino is an open source library for building interactive applications such as games and digital art.
It provides a API that runs on multiple platforms and is available for multiple programming languages.

Lumino はゲームやデジタルアートなどのインタラクティブアプリケーションを構築するためのオープンソースライブラリです。
マルチプラットフォームで動作し複数のプログラミング言語から利用できる API を提供します。

## 💡 Project Status

Lumino は現在、次の2つのユースケースを想定して開発を続けています。

- A. ゲームエンジンのコアモジュール (Graphics, Audio, Input 等) を 他のゲームエンジンやアプリケーションで利用する。
- [B. 基礎的なモジュール (String, Array, FileSystem, 等) を IoTプラットフォームやテスティングツール で利用する。（補助的）](./docs/use-base-module.md)

v0.10.1 までの Lumino はスタンドアロンゲームエンジンとして開発を続けていましたが、その主な動機は技術的探求でした。

ここ数年は開発者が忙しく、メンテナンスが難しくなっていました。しかし、小規模ながら投資を得られる機会があり、今後はそれらにフォーカスしたいと考えています。

今後の開発コンセプトは次の通りです。

- フレームワークのような機能をすべて廃止します。例えばゲームUIやシーングラフです。これらの機能はクライアント側のプログラム言語で書いた方が都合が良いことに気づきました。
- ゲームエンジンのコアモジュールを、 FFI (Foreign function interface) として提供します。 Lumino を直接利用してゲームを開発することは想定しません。他のゲームエンジンを開発する際の低レイヤーコンポーネントを想定し、プラットフォームの抽象化に注力します。
- FFI の安定化に努めます。つまり、純粋な C 言語の API として提供することになります。おそらく Lumino を使って流暢なゲームプログラムを書くことはできなくなります。しかし今後の Lumino の活用のために、イディオムやコーディングスタイルのトレンドに左右されない基盤が必要と考えました。

## How to Build

```powershell
# vcpkg
git clone https://github.com/microsoft/vcpkg.git ./vcpkg
./vcpkg/bootstrap-vcpkg.bat

# CMake 設定
cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=E:/Proj/Lumino//vcpkg/scripts/buildsystems/vcpkg.cmake -DLUMINO_ENABLE_WEBGPU=ON

# ビルド
cmake --build build --config Release --parallel

# テスト実行
ctest --test-dir build -C Release --output-on-failure
```

```sh
# wasm (emscripten)
git clone https://dawn.googlesource.com/dawn third_party/dawn
git clone -b 5.0.5 https://github.com/emscripten-core/emsdk.git ./emsdk
./emsdk/emsdk install 5.0.5
./emsdk/emsdk activate 5.0.5
python scripts/build_wasm.py
```
