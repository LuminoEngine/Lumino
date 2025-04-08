# How to Build

Prepare vcpkg in the root of the repository by referring to [here](https://learn.microsoft.com/ja-jp/vcpkg/get_started/get-started).

```sh
python ./scripts/build_engine.py
```

The project files will be generated in `📁build/buildtrees`.

## Web

```sh
cd ./packages/lumino-js
npm install
npm run build

```


## WebGPU (Dawn)

```sh
dotnet tool install dotnet-script -g
dotnet script scripts/PrepareWebGPU.csx
dotnet run -- BuildEngine wasm32-emscripten
```

CMake の成果物 (.wasm 等) は `📁build/installed/wasm32-emscripten` に出力されます。

ローカルで Dawn を用いた動作確認をするには `LUMINO_USE_WEBGPU` を ON にして cmake を実行してください。

