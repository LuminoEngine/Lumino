# luminojs

Lumino グラフィックスエンジンの TypeScript / WebAssembly バインディングです。

`luminojs` は、Emscripten で WebAssembly にコンパイルされた Lumino レンダリングエンジンの TypeScript API を提供します。ネイティブ C/C++ 版と同じコアエンジンを使用して、ブラウザ上で GPU アクセラレーションによる 2D/3D レンダリングを実現します。

## テスト

実際にロードする WASM バイナリを検証するスモークテストがあります (Playwright + Chromium)。

```sh
npm run test:smoke
```

実行前提と採用方式の詳細は [test/smoke/README.md](test/smoke/README.md) を参照してください。
